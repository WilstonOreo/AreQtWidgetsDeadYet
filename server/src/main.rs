// Copyright © Michael Winkelmann <michael@winkelmann.site>
// SPDX-License-Identifier: AGPL-3.0-or-later

use axum::{
    Router,
    http::{HeaderMap, header, HeaderValue},
    extract::Host,
    handler::HandlerWithoutStateExt,
    http::{StatusCode, Uri},
    response::Redirect,
    BoxError,
};
use lazy_static::lazy_static;
use axum_server::tls_rustls::RustlsConfig;
use std::{future::Future, net::SocketAddr, path::PathBuf, time::Duration};
use tokio::signal;
use tracing_subscriber::{layer::SubscriberExt, util::SubscriberInitExt};

use std::collections::HashMap;

use common_macros::hash_map;
use clap::Parser;

#[derive(Clone, Copy)]
struct Ports {
    http: u16,
    https: u16,
}


#[derive(Parser, Debug)]
#[command(author="Michael Winkelmann", version, about="AQWDY Webserver")]
struct Arguments{
    /// Server address
    #[arg(long, default_value_t = String::from("127.0.0.1"))]
    ip: String,
    #[arg(long)]
    key: String,
    #[arg(long)]
    cert: String,
    #[arg(long, name = "http", default_value_t = 8080)]
    http_port: u16,
    #[arg(long, name = "https", default_value_t = 3000)]
    https_port: u16,
}



lazy_static! {
    static ref MIMETYPES: HashMap<&'static str, &'static str> = {
        hash_map! {
            "html" => "text/html",
            "ico" => "image/x-icon",
            "jpg" => "image/jpeg",
            "js" => "application/javascript",
            "png" => "image/png",
            "ttf" => "font/ttf",
            "wasm" => "application/wasm",
        }
    };
}

fn header_map(filename: &'static str) -> HeaderMap<HeaderValue> {
    let mut headers = HeaderMap::new();
    headers.insert("Cross-Origin-Embedder-Policy", "require-corp".parse().unwrap());
    headers.insert("Cross-Origin-Opener-Policy", "same-origin".parse().unwrap());
    headers.insert(header::CONTENT_TYPE, MIMETYPES[&filename.split(".").last().unwrap()].parse().unwrap());
    headers
}

macro_rules! get_static_str {
    ($l:literal) => {
        axum::routing::get(|| async {
            (header_map($l), std::include_str!($l))
        })
    };
}

macro_rules! get_static_bytes {
    ($l:literal) => {
        axum::routing::get(|| async {
            (header_map($l), std::include_bytes!($l))
        })
    };
}

#[tokio::main]
async fn main() {
    tracing_subscriber::registry()
        .with(
            tracing_subscriber::EnvFilter::try_from_default_env()
                .unwrap_or_else(|_| "axum=debug".into()),
        )
        .with(tracing_subscriber::fmt::layer())
        .init();


    let args = Arguments::parse();
    let handle = axum_server::Handle::new();
    let shutdown_future = shutdown_signal(handle.clone());


    let ports = Ports {
        http: args.http_port,
        https: args.https_port,
    };

    // optional: spawn a second server to redirect http requests to this server
    tokio::spawn(redirect_http_to_https(ports, shutdown_future));

    // configure certificate and private key used by https
    let config = RustlsConfig::from_pem_file(
        PathBuf::from(args.cert),
        PathBuf::from(args.key),
    )
    .await
    .unwrap();

    let app = Router::new()
        .route("/", get_static_str!("../assets/index.html"))
        .route("/index.html", get_static_str!("../assets/index.html"))
        .route("/favicon.ico", get_static_bytes!("../assets/favicon.ico"))
        .route("/qtloader.js", get_static_str!("../assets/qtloader.js"))
        .route("/AreQtWidgetsDeadYet.js", get_static_str!("../assets/AreQtWidgetsDeadYet.js"))
        .route("/AreQtWidgetsDeadYet.wasm", get_static_bytes!("../assets/AreQtWidgetsDeadYet.wasm"))
        .route("/AreQtWidgetsDeadYet.worker.js", get_static_str!("../assets/AreQtWidgetsDeadYet.worker.js"))
        .route("/DIN.ttf", get_static_bytes!("../assets/DIN.ttf"))
        ;
    
    // Address that server will bind to.
    let addr = format!("{}:{}", args.ip, args.https_port).parse().unwrap();
    
    // Use `hyper::server::Server` which is re-exported through `axum::Server` to serve the app.
    tracing::debug!("listening on {addr}");
    axum_server::bind_rustls(addr, config)
        .handle(handle)
        .serve(app.into_make_service())
        .await
        .unwrap();
}

async fn shutdown_signal(handle: axum_server::Handle) {
    let ctrl_c = async {
        signal::ctrl_c()
            .await
            .expect("failed to install Ctrl+C handler");
    };

    #[cfg(unix)]
    let terminate = async {
        signal::unix::signal(signal::unix::SignalKind::terminate())
            .expect("failed to install signal handler")
            .recv()
            .await;
    };

    #[cfg(not(unix))]
    let terminate = std::future::pending::<()>();

    tokio::select! {
        _ = ctrl_c => {},
        _ = terminate => {},
    }

    tracing::info!("Received termination signal shutting down");
    handle.graceful_shutdown(Some(Duration::from_secs(10))); // 10 secs is how long docker will wait
                                                             // to force shutdown
}


async fn redirect_http_to_https(ip: String, ports: Ports, signal: impl Future<Output = ()>) {
    fn make_https(host: String, uri: Uri, ports: Ports) -> Result<Uri, BoxError> {
        let mut parts = uri.into_parts();

        parts.scheme = Some(axum::http::uri::Scheme::HTTPS);

        if parts.path_and_query.is_none() {
            parts.path_and_query = Some("/".parse().unwrap());
        }

        let https_host = host.replace(&ports.http.to_string(), &ports.https.to_string());
        parts.authority = Some(https_host.parse()?);

        Ok(Uri::from_parts(parts)?)
    }

    let redirect = move |Host(host): Host, uri: Uri| async move {
        match make_https(host, uri, ports) {
            Ok(uri) => Ok(Redirect::permanent(&uri.to_string())),
            Err(error) => {
                tracing::warn!(%error, "failed to convert URI to HTTPS");
                Err(StatusCode::BAD_REQUEST)
            }
        }
    };

    let addr = format!("{}:{}", ip, ports.http).parse().unwrap();
    //let listener = tokio::net::TcpListener::bind(addr).await.unwrap();
    tracing::debug!("listening on {addr}");
    hyper::Server::bind(&addr)
        .serve(redirect.into_make_service())
        .with_graceful_shutdown(signal)
        .await
        .unwrap();
}

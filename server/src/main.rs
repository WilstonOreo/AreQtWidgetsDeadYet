use axum::{
    Router,
    http::{HeaderMap, header, HeaderValue},
};
use lazy_static::lazy_static;

use std::collections::HashMap;

use common_macros::hash_map;
use clap::Parser;

#[derive(Parser, Debug)]
#[command(author="Michael Winkelmann", version, about="AQWDY Webserver")]
struct Arguments{
    #[arg(short, long, default_value_t = String::from("127.0.0.1:8080"))]
    address: String,
}



lazy_static! {
    static ref MIMETYPES: HashMap<&'static str, &'static str> = {
        hash_map! {
            "html" => "text/html",
            "ico" => "image/x-icon",
            "png" => "image/png",
            "jpg" => "image/jpeg",
            "js" => "application/javascript",
            "wasm" => "application/wasm"
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
    let args = Arguments::parse();

    let app = Router::new()
        .route("/", get_static_str!("../assets/index.html"))
        .route("/index.html", get_static_str!("../assets/index.html"))
        .route("/favicon.ico", get_static_bytes!("../assets/favicon.ico"))
        .route("/qtloader.js", get_static_str!("../assets/qtloader.js"))
        .route("/AreQtWidgetsDeadYet.js", get_static_str!("../assets/AreQtWidgetsDeadYet.js"))
        .route("/AreQtWidgetsDeadYet.wasm", get_static_bytes!("../assets/AreQtWidgetsDeadYet.wasm"))
        .route("/AreQtWidgetsDeadYet.worker.js", get_static_str!("../assets/AreQtWidgetsDeadYet.worker.js"))
        ;
    
    // Address that server will bind to.
    let addr = args.address.parse().unwrap();
    
    // Use `hyper::server::Server` which is re-exported through `axum::Server` to serve the app.
    axum::Server::bind(&addr)
        // Hyper server takes a make service.
        .serve(app.into_make_service())
        .await
        .unwrap();
}

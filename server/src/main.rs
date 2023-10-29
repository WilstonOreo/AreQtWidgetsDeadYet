use axum::{routing::get, response::Html, Router};
use axum::{
    Json,
    response::{IntoResponse, Response},
    body::{Full, Bytes},
    http::{StatusCode, HeaderMap, Uri, header},
};

use std::net::SocketAddr;

#[tokio::main]
async fn main() {
    let app = Router::new()
        .route("/", get(index))
        .route("/AreQtWidgetsDeadYet.js", get(wasm_loader_js))
        .route("/AreQtWidgetsDeadYet.wasm", get(wasm));
    

    // Address that server will bind to.
    let addr = SocketAddr::from(([127, 0, 0, 1], 3000));

    // Use `hyper::server::Server` which is re-exported through `axum::Server` to serve the app.
    axum::Server::bind(&addr)
        // Hyper server takes a make service.
        .serve(app.into_make_service())
        .await
        .unwrap();
}

async fn index() -> Html<&'static str> {
    Html(std::include_str!("../assets/index.html"))
}


async fn qt_logo_svg() -> impl IntoResponse {
    let mut headers = HeaderMap::new();
    headers.insert(header::CONTENT_TYPE, "image/svg+xml".parse().unwrap());
    (headers, std::include_str!("../assets/qtloader.js"))
}


async fn qt_loader_js() -> impl IntoResponse {
    let mut headers = HeaderMap::new();
    headers.insert(header::CONTENT_TYPE, "application/javascript".parse().unwrap());
    (headers, std::include_str!("../assets/qtloader.js"))
}


async fn wasm_loader_js() -> impl IntoResponse {
    let mut headers = HeaderMap::new();
    headers.insert(header::CONTENT_TYPE, "application/javascript".parse().unwrap());
    (headers, std::include_str!("../assets/AreQtWidgetsDeadYet.js"))
}

async fn wasm_worker_js() -> impl IntoResponse {
    let mut headers = HeaderMap::new();
    headers.insert(header::CONTENT_TYPE, "application/javascript".parse().unwrap());
    (headers, std::include_str!("../assets/AreQtWidgetsDeadYet.worker.js"))
}

async fn wasm() -> impl IntoResponse {
    let mut headers = HeaderMap::new();
    headers.insert(header::CONTENT_TYPE, "application/wasm".parse().unwrap());
    (headers, std::include_bytes!("../assets/AreQtWidgetsDeadYet.wasm"))
}




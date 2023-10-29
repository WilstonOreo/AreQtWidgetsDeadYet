use axum::{
    Router,
    routing::get,
    response::{IntoResponse, Html},
    http::{HeaderMap, Method, header},
};

use tower_http::cors::{Any, CorsLayer};

use std::net::SocketAddr;
//res.header("Cross-Origin-Embedder-Policy", "require-corp");
//res.header("Cross-Origin-Opener-Policy", "same-origin");

#[tokio::main]
async fn main() {
    let cors = CorsLayer::new()
    // allow `GET` and `POST` when accessing the resource
    .allow_methods([Method::GET, Method::POST])
    // allow requests from any origin
    .allow_origin(Any);


    let app = Router::new()
        .route("/", get(index))
        .route("/qtlogo.svg", get(qt_logo_svg))
        .route("/favicon.ico", get(favicon))
        .route("/qtloader.js", get(qt_loader_js))
        .route("/AreQtWidgetsDeadYet.html", get(index))
        .route("/AreQtWidgetsDeadYet.js", get(wasm_loader_js))
        .route("/AreQtWidgetsDeadYet.wasm", get(wasm))
        .route("/AreQtWidgetsDeadYet.worker.js", get(wasm_worker_js))
        .layer(cors)
        ;
    
    // Address that server will bind to.
    let addr = SocketAddr::from(([127, 0, 0, 1], 3000));

    // Use `hyper::server::Server` which is re-exported through `axum::Server` to serve the app.
    axum::Server::bind(&addr)
        // Hyper server takes a make service.
        .serve(app.into_make_service())
        .await
        .unwrap();
}


async fn index() -> impl IntoResponse {
    let mut headers = HeaderMap::new();
    headers.insert("Cross-Origin-Embedder-Policy", "require-corp".parse().unwrap());
    headers.insert("Cross-Origin-Opener-Policy", "same-origin".parse().unwrap());
    headers.insert(header::CONTENT_TYPE, "text/html".parse().unwrap());

    (headers, std::include_str!("../assets/index.html"))
}


async fn favicon() -> impl IntoResponse {
    let mut headers = HeaderMap::new();
    headers.insert(header::CONTENT_TYPE, "image/vnd.microsoft.icon".parse().unwrap());
    (headers, std::include_bytes!("../assets/favicon.ico"))
}

async fn qt_logo_svg() -> impl IntoResponse {
    let mut headers = HeaderMap::new();
    headers.insert(header::CONTENT_TYPE, "image/svg+xml".parse().unwrap());
    (headers, std::include_str!("../assets/qtlogo.svg"))
}


async fn qt_loader_js() -> impl IntoResponse {
    let mut headers = HeaderMap::new();
    headers.insert(header::CONTENT_TYPE, "text/javascript".parse().unwrap());
    headers.insert("Cross-Origin-Embedder-Policy", "require-corp".parse().unwrap());
    headers.insert("Cross-Origin-Opener-Policy", "same-origin".parse().unwrap());
    (headers, std::include_str!("../assets/qtloader.js"))
}


async fn wasm_loader_js() -> impl IntoResponse {
    let mut headers = HeaderMap::new();
    headers.insert(header::CONTENT_TYPE, "text/javascript".parse().unwrap());
    headers.insert("Cross-Origin-Embedder-Policy", "require-corp".parse().unwrap());
    headers.insert("Cross-Origin-Opener-Policy", "same-origin".parse().unwrap());
    (headers, std::include_str!("../assets/AreQtWidgetsDeadYet.js"))
}

async fn wasm_worker_js() -> impl IntoResponse {
    let mut headers = HeaderMap::new();
    headers.insert(header::CONTENT_TYPE, "text/javascript".parse().unwrap());
    headers.insert("Cross-Origin-Embedder-Policy", "require-corp".parse().unwrap());
    headers.insert("Cross-Origin-Opener-Policy", "same-origin".parse().unwrap());
    (headers, std::include_str!("../assets/AreQtWidgetsDeadYet.worker.js"))
}

async fn wasm() -> impl IntoResponse {
    let mut headers = HeaderMap::new();
    headers.insert("Cross-Origin-Embedder-Policy", "require-corp".parse().unwrap());
    headers.insert("Cross-Origin-Opener-Policy", "same-origin".parse().unwrap());
    headers.insert(header::CONTENT_TYPE, "application/wasm".parse().unwrap());
    (headers, std::include_bytes!("../assets/AreQtWidgetsDeadYet.wasm"))
}




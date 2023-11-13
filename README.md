# Are Qt Widgets dead yet?

I often hear the question from my [Qt](https://qt.io) Customers if [Qt Widgets](https://doc.qt.io/qt-6/qtwidgets-index.html) are dead and whether they should switch to [QML](https://doc.qt.io/qt-6/qtqml-index.html) immediately. Hence I made this single page website as a proof that Qt Widgets are alive and well and that they can even be run in a web browser!

## TL;DR

Run the pre-compiled [Qt for WebAssembly](https://doc.qt.io/qt-6/wasm.html) web app with:

```shell
cd server
cargo run --release 
```

This will compile and start a webserver listening on port 8080 by default.
Type `127.0.0.1:8080` in your web browser to see the Are Qt Widgets Dead Yet app running.

## Repository structure

The repository consists of 3 parts:

* `generator`: The generator renders a collection of widgets into images. The output PNG files are used by the frontend.
* `frontend`: The frontend is compiled to WebAssembly using [Qt for WebAssembly](https://doc.qt.io/qt-6/wasm.html). The frontend does only use `QPushButton`. All other widgets are pre-rendered by the generator. This decision was made in order to make rendering faster and the resulting WASM file smaller.
* `server`: A single binary webserver written in [Rust](https://www.rust-lang.org/) using [axum](https://github.com/tokio-rs/axum/). For HTTPS, I use Caddy for my setup.

## Compiling the Qt part

Open the project folder containing `CMakeLists.txt` in Qt Creator and compile it to WebAssembly and to the desktop. Make sure to run the generator first to compile the image assets!
You can also run the frontend app on the desktop.

## Compiling the Webserver

Prebuilt WASM binaries are already contained in the repository.
Run the pre-compiled [Qt for WebAssembly](https://doc.qt.io/qt-6/wasm.html) web app with:

```shell
cd server
cargo run --release 
```

All WASM binaries are compiled into the web server binary.

## License

This program is distributed under the terms of the MIT license.
See `LICENSE.txt` for details.

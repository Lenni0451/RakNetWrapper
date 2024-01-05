#!/bin/bash

cd ../../raknet_rs
cargo build --release
mv ../../raknet_rs/target/release/librust_raknet.so ../../raknet_rs/target/release/librust-raknet-x64.so

rustup default stable
rustup target add i686-unknown-linux-musl
cargo build --release --target i686-unknown-linux-musl
mv ../../raknet_rs/target/i686-unknown-linux-musl/release/librust_raknet.so ../../raknet_rs/target/i686-unknown-linux-musl/release/librust-raknet-x64_86.so

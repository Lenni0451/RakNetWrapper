#!/bin/bash

cd ../../raknet_rs
cargo build --release
mv ../../raknet_rs/target/release/librust_raknet.so ../../raknet_rs/target/release/librust-raknet-x64.so

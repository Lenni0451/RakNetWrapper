#!/bin/bash

cd ../../raknet_rs
cargo build --release
mv target/release/librust_raknet.so target/release/librust-raknet.so

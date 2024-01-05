#!/bin/bash

cd ../../raknet_rs
cargo build --release
mv target/release/$1 target/release/$2

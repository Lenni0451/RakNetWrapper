@echo off

cd ..\..\raknet_rs
cargo build --release
rename target\release\rust_raknet.dll rust-raknet.dll

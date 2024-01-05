@echo off

cd ..\..\raknet_rs
cargo build --release
REM rename target\release\librust_raknet.dll target\release\librust-raknet.dll

@echo off

cd ..\..\raknet_go
go build -ldflags="-s -w" -trimpath -o go-raknet.dll -buildmode=c-shared dllmain.go

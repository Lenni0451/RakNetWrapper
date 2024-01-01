@echo off

go build -ldflags="-s -w" -trimpath -o go-raknet.dll -buildmode=c-shared ..\..\raknet_go\dllmain.go

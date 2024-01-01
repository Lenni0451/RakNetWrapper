#!/bin/bash

cd ../../raknet_go
go build -ldflags="-s -w" -trimpath -o go-raknet.so -buildmode=c-shared dllmain.go

#!/bin/bash

cd ../../raknet_go
go build -ldflags="-s -w" -trimpath -o $1 -buildmode=c-shared dllmain.go

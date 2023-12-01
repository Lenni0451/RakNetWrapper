#!/bin/bash

g++ -Os -ffunction-sections -fdata-sections -s -shared -fPIC ../fb-raknet/Source/*.cpp ../raknet_c/*.cpp -I ../fb-raknet/Source -o raknet.so -fvisibility=hidden

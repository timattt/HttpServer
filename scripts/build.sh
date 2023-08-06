#!/bin/bash
mkdir build
cd build
cmake ../sources
make
cp WebServer /artifacts/WebServer
#!/bin/sh

version=1.1.0
export CFLAGS=-Oz

clang -arch arm64 -o twc twc.c
tar -czvf twc-$version-arm64-apple-darwin.tar.gz twc

clang -arch x86_64 -o twc twc.c
tar -czvf twc-$version-x86_64-apple-darwin.tar.gz twc

docker build -t twc .
docker run --name dist twc
docker cp dist:/app/twc .
docker stop dist
docker rm dist
tar -czvf twc-$version-arm64-linux-debian.tar.gz twc

# docker run --name dist --rm twc

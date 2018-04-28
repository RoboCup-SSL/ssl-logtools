#!/bin/bash

# stop on errors
set -e

if [ "`id -nu`" != "root" ]; then
  echo "Must be called as root or with sudo"
  exit 1
fi

if which dnf 2>/dev/null >/dev/null; then
  FLAGS="-y"
  dnf $FLAGS install cmake gcc-c++ git protobuf-compiler zlib-devel boost-program-options
fi

if which apt-get 2>/dev/null >/dev/null; then
  FLAGS="-qq -y"
  apt-get $FLAGS install cmake g++ git libprotobuf-dev protobuf-compiler zlib1g-dev libboost-program-options-dev
fi

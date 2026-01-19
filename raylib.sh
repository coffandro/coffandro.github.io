#!/bin/bash
INCLUDE_DIR="include"
RAY_DIR="raylib"
DEST_DIR="$INCLUDE_DIR/$RAY_DIR"
SRC_DIR="$DEST_DIR/src"

EMSDK_PATH="/home/frida/dev/emsdk"
NODE_PATH="$EMSDK_PATH/node/22.16.0_64bit/bin"
PYTHON_PATH="/usr/bin/python"

if [[ ! -d $DEST_DIR ]]; then
    git clone https://github.com/raysan5/raylib $DEST_DIR
fi

cd $SRC_DIR && \
make PLATFORM=PLATFORM_WEB RAYLIB_SRC_PATH=. \
    RAYLIB_RELEASE_PATH=../../ \
    EMSDK_PATH=$EMSDK_PATH \
    NODE_PATH=$NODE_PATH \
    PYTHON_PATH=$PYTHON_PATH -B

cd ../../ && \
    mv libraylib.web.a libraylib.a
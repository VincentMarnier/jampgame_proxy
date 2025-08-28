#!/bin/sh

SCRIPT_DIR=$(dirname "$0")
DOCKER_IMAGE_NAME=jampgame-proxy-builder

if [ -z "$(docker images -q $DOCKER_IMAGE_NAME 2> /dev/null)" ]; then
  docker build -t $DOCKER_IMAGE_NAME - < "$SCRIPT_DIR/build.Dockerfile"
fi

docker run --rm -v $SCRIPT_DIR:/proxy $DOCKER_IMAGE_NAME sh -c "cmake -B /proxy/build -S /proxy -DTARGET_ARCH=x86 && cmake --build /proxy/build"
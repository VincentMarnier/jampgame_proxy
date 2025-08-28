FROM i386/debian:trixie

RUN apt update && apt install -y cmake make g++
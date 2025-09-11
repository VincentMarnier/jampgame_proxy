FROM i386/debian:bookworm

RUN apt update && apt install -y cmake make g++
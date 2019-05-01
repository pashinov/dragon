FROM fedora:latest
MAINTAINER Alexey Pashinov <pashinov93@gmail.com>

RUN  dnf -y update && \
     dnf -y install gcc \
                    gcc-c++ \
                    make \
                    cmake \
                    protobuf-devel \
                    zeromq-devel && \
	 dnf clean all

RUN curl https://sh.rustup.rs -sSf | sh -s -- -y

ENV PATH=/root/.cargo/bin:$PATH

RUN cargo install protobuf-codegen

WORKDIR /home/develop

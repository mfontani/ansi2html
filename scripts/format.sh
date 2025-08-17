#!/bin/bash
set -euo pipefail
DEBIAN_VERSION=bookworm
CLANG_VERSION=20
docker build -t ansi2html-formatter . -f-<<END_DOCKERFILE
FROM debian:$DEBIAN_VERSION
RUN apt-get -qq update; \
    apt-get install -qqy --no-install-recommends \
        gnupg2 wget ca-certificates apt-transport-https \
        autoconf automake cmake dpkg-dev file make patch libc6-dev
RUN echo "deb https://apt.llvm.org/$DEBIAN_VERSION llvm-toolchain-$DEBIAN_VERSION-$CLANG_VERSION main" \
        > /etc/apt/sources.list.d/llvm.list && \
    wget -qO /etc/apt/trusted.gpg.d/llvm.asc \
        https://apt.llvm.org/llvm-snapshot.gpg.key && \
    apt-get -qq update && \
    apt-get install -qqy -t $DEBIAN_VERSION \
      clang-$CLANG_VERSION clang-tidy-$CLANG_VERSION clang-format-$CLANG_VERSION lld-$CLANG_VERSION \
    && \
    for f in /usr/lib/llvm-*/bin/*; do ln -sf "\$f" /usr/bin; done && \
    ln -sf clang /usr/bin/cc && \
    ln -sf clang /usr/bin/c89 && \
    ln -sf clang /usr/bin/c99 && \
    ln -sf clang++ /usr/bin/c++ && \
    ln -sf clang++ /usr/bin/g++ && \
    rm -rf /var/lib/apt/lists/*
END_DOCKERFILE
docker run -it --rm --user "$(id -u)" -v "$(pwd):/src" --workdir /src ansi2html-formatter "$@"

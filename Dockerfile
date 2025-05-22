FROM debian:bookworm AS builder_base
WORKDIR /app
COPY ./conanfile.py     /app/conanfile.py
RUN apt update \
    && apt upgrade -y \
    && apt install -y \ 
        cmake \
        make \
        python3 \
        python3-pip \
        python3-venv \
        g++ \
        gcc \
    && python3 -m venv venv \
    && venv/bin/pip install conan==2.16 \
    && venv/bin/conan profile detect --force \
	&& venv/bin/conan install . --build missing


FROM builder_base AS bot_builder
COPY ./bot            /app/bot
COPY ./Makefile       /app/Makefile
COPY ./CMakeLists.txt /app/CMakeLists.txt
RUN make build_bot


FROM busybox AS base
WORKDIR /app
RUN addgroup -S runner \
    && adduser -S runner -G runner
USER runner
CMD ["/app/run"]
   

FROM base AS bot
COPY --from=bot_builder /app/build/KEBER_BOT /app/run
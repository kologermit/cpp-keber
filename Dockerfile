FROM debian AS builder_base
WORKDIR /app
COPY ./conanfile.py     /app/conanfile.py
COPY ./requirements.txt /app/requirements.txt
RUN \
    apt update \
    && apt upgrade -y \
    && apt install -y \ 
        cmake \
        python3 \
        python3-pip \
        python3-venv \
        g++ \
    && python3 -m venv venv \
    && venv/bin/pip install -r requirements.txt \
    && venv/bin/conan profile detect --force \
	&& venv/bin/conan install . --build missing \
    && rm -rf /var/cache/apt/*  
COPY ./utils            /app/utlis
COPY ./CMakeLists.txt   /app/CMakeLists.txt
COPY ./Makefile         /app/Makefile
RUN cmake . -DCMAKE_BUILD_TYPE=Release -B ./build


FROM busybox AS base
WORKDIR /app
RUN addgroup -S runner \
    && adduser -S runner -G runner
USER runner
CMD ["/app/run"]


FROM builder_base AS bot_builder
COPY ./bot /app/bot
RUN make build_bot
   

FROM base AS bot
COPY --from=bot_builder /app/build/BOT /app/run


FROM builder_base AS downloader_build
COPY ./downloader /app/downloader
RUN make build_downloader

FROM base AS downloader
COPY --from=downloader_builder /app/build/DOWNLOADER /app/run


FROM builder_base AS cron_builder
COPY ./cron /app/cron
RUN make cron_build


FROM base AS cron
COPY --from=cron_builder /app/build/CRON /app/run


FROM builder_base AS delete_builder
COPY ./delete /app/delete
RUN make delete_build

FROM base AS delete
COPY --from=delete_builder /app/build/DELETE /app/run
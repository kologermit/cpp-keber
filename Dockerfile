ARG BUILDER_IMAGE=debian
ARG BASE_IMAGE=busybox
ARG USER_ID=1001
ARG GROUP_ID=1001

FROM ${BUILDER_IMAGE:-debian} AS builder_base
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
    && mkdir -p /home/runner /app\
    && chmod 777 -R /home/runner /app\
    && groupadd -g ${GROUP_ID:-1001} runner \
    && useradd -l -u ${USER_ID:-1001} -g runner runner
USER runner
COPY ./utils                /app/utlis
COPY ./scripts/venv.json    /app/scripts/venv.json
COPY ./scripts/install.json /app/scripts/install.json
COPY ./scripts/conan.json   /app/scripts/conan.json
COPY ./manager.py           /app/manager.py
RUN python3 manager.py install deps
COPY ./CMakeLists.txt       /app/CMakeLists.txt
RUN python3 manager.py conan build_deps

FROM ${BASE_IMAGE:-busybox} AS base
WORKDIR /app
RUN addgroup -S runner \
    && adduser -S runner -G runner
USER runner
CMD ["/app/run"]


FROM builder_base AS bot_builder
COPY ./bot /app/bot
COPY ./scripts/bot.json /app/scripts/bot.json
RUN python3 manager.py bot build
   

FROM base AS bot
COPY --from=bot_builder /app/build/BOT /app/run


FROM builder_base AS downloader_builder
COPY ./downloader /app/downloader
RUN python3 manager.py downloader build

FROM base AS downloader
COPY --from=downloader_builder /app/build/DOWNLOADER /app/run


FROM builder_base AS cron_builder
COPY ./cron /app/cron
RUN python3 manager.py cron build


FROM base AS cron
COPY --from=cron_builder /app/build/CRON /app/run


FROM builder_base AS cleaner_builder
COPY ./cleaner /app/cleaner
RUN python3 manager.py cleaner build

FROM base AS cleaner
COPY --from=cleaner_builder /app/build/CLEANER /app/run
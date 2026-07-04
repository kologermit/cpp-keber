ARG RUNNER_IMAGE=ubuntu:latest
ARG BUILDER_IMAGE=ubuntu:latest

FROM ${RUNNER_IMAGE} AS base-runner
ARG USER_ID=1001
ARG GROUP_ID=1001
RUN if command -v apt &> /dev/null; then \
        groupadd -g ${GROUP_ID} runner && \
        useradd -u ${USER_ID} -g runner -m -s /bin/bash runner && \
        apt update && \
        apt install -y libpq5 && \
        rm -rf /var/lib/apt/lists/*; \
    elif command -v apk &> /dev/null; then \
        addgroup -g ${GROUP_ID} runner && \
        adduser -u ${USER_ID} -G runner -D -s /bin/bash runner && \
        apk update && \
        apk add --no-cache postgresql-libs openssl libstdc++; \
    else \
        echo "Unsupported package manager" >&2 && \
        exit 1; \
    fi \
    && touch /env.json && mkdir -p /volumes/logs /volumes/tmp /volumes/file_buffer \
    && chown runner:runner /volumes/logs /volumes/tmp /volumes/file_buffer /env.json \
    && chmod 755 /volumes/logs /volumes/tmp /volumes/file_buffer /env.json
WORKDIR /
CMD ["/app"]

FROM ${BUILDER_IMAGE} AS base-builder
RUN if command -v apt-get &> /dev/null; then \
        apt-get update && \
        apt-get install -y --no-install-recommends \
            python3 make g++ git ca-certificates patch cmake \
            libpq-dev libminizip-dev pkg-config zlib1g-dev libexpat1-dev && \
        rm -rf /var/lib/apt/lists/*; \
    elif command -v apk &> /dev/null; then \
        apk update && \
        apk add --no-cache \
            cmake python3 git ca-certificates \
            g++ make patch perl postgresql-dev \
            minizip-dev pkgconf zlib-dev expat-dev \
            linux-headers; \
    else \
        echo "Unsupported package manager" >&2 && \
        exit 1; \
    fi
WORKDIR /src
COPY ./manager.py /src/manager.py
COPY ./scripts/deps.json /src/scripts/deps.json
RUN python3 manager.py deps download-and-build-all
COPY ./utils /src/utils
COPY ./CMakeLists.txt /src/CMakeLists.txt
COPY ./scripts/build.json /src/scripts/build.json
RUN python3 manager.py build utils-debug && python3 manager.py build utils-release

FROM base-builder AS build-bot-release
COPY ./bot /src/bot
RUN python3 manager.py build bot-release


FROM base-builder AS build-bot-debug
COPY ./bot /src/bot
RUN python3 manager.py build bot-debug


FROM base-runner AS runner-bot-release
COPY --from=build-bot-release /src/build/Release/BOT /app


FROM base-runner AS runner-bot-debug
COPY --from=build-bot-debug /src/build/Debug/BOT /app


FROM base-builder AS build-task-tracker-release
COPY ./task_tracker /src/task_tracker
RUN python3 manager.py build task-tracker-release


FROM base-builder AS build-task-tracker-debug
COPY ./task_tracker /src/task_tracker
RUN python3 manager.py build task-tracker-debug


FROM base-runner AS runner-task-tracker-release
COPY --from=build-task-tracker-release /src/build/Release/TASK_TRACKER /app


FROM base-runner AS runner-task-tracker-debug
COPY --from=build-task-tracker-debug /src/build/Debug/TASK_TRACKER /app
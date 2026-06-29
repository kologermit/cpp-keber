ARG RUNNER_IMAGE=ubuntu:latest
ARG BUILDER_IMAGE=ubuntu:latest

FROM ${RUNNER_IMAGE} AS base-runner
ARG USER_ID=1001
ARG GROUP_ID=1001
RUN groupadd -g $GROUP_ID runner && useradd -u $USER_ID -g runner -m -s /bin/bash runner
RUN touch /env.json && \
    mkdir -p /volumes/logs /volumes/tmp /volumes/file_buffer \
    && chown runner:runner / /volumes/logs /volumes/tmp /volumes/file_buffer /env.json \
    && chmod 755 /volumes/logs /volumes/tmp /volumes/file_buffer /env.json \
    && apt update && apt install -y libpq5 && rm -rf /var/lib/apt/lists/*
WORKDIR /
CMD ["/app"]

FROM ${BUILDER_IMAGE} AS base-builder
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    python3 make g++ git ca-certificates patch python3-pip\
    libpq-dev libminizip-dev pkg-config zlib1g-dev libexpat1-dev && \
    rm -rf /var/lib/apt/lists/* && pip install --break cmake==3.25.0
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
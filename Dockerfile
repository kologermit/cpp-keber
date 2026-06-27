ARG RUNNER_IMAGE=gcr.io/distroless/base-debian12
ARG BUILDER_IMAGE=ubuntu:latest

FROM ${RUNNER_IMAGE} AS base_runner
ARG USER_ID=1001
ARG GROUP_ID=1001
RUN groupadd -g $GROUP_ID runner && useradd -u $USER_ID -g runner -m -s /bin/bash runner
RUN touch /env.json && \
    mkdir -p /volumes/logs /volumes/tmp /volumes/file_buffer \
    && chown runner:runner / /volumes/logs /volumes/tmp /volumes/file_buffer /env.json \
    && chmod 755 /volumes/logs /volumes/tmp /volumes/file_buffer /env.json
WORKDIR /
CMD ["/app"]

FROM ${BUILDER_IMAGE} AS base_builder
RUN apt update && apt upgrade -y
RUN apt install -y \
    python3 \
    python3-venv \
    clang \
    gcc \
    cmake \
    libfindbin-libs-perl \
    bison \
    flex \
    make
WORKDIR /src
COPY ./manager.py ./requirements.txt /src/
COPY ./scripts/venv.json /src/scripts/venv.json
RUN python3 manager.py venv venv && python3 manager.py venv requirements
COPY \
    ./conanfile.py \
    ./Release-conanprofile \
    ./Debug-conanprofile \
    ./CMakeLists.txt \
        /src/
COPY ./scripts/conan.json /src/scripts/conan.json
COPY ./recipes /src/recipes
RUN python3 manager.py conan build
COPY ./utils /src/utils
COPY ./cmake /src/cmake
COPY ./.env  /src/.env


FROM base_builder AS base_builder_bot
COPY ./bot /src/bot
COPY ./scripts/bot.json /src/scripts/bot.json


FROM base_builder_bot AS build_bot_release
RUN python3 manager.py bot build_release


FROM base_builder_bot AS build_bot_debug
RUN python3 manager.py bot build_debug


FROM base_runner AS runner_bot_release
COPY --from=build_bot_debug /src/build/Release/bot/BOT /app


FROM base_runner AS runner_bot_debug
COPY --from=build_bot_debug /src/build/Debug/bot/BOT /app


FROM base_builder AS base_builder_task_tracker
COPY ./scripts/task_tracker.json /src/scripts/task_tracker.json
COPY ./task_tracker /src/task_tracker


FROM base_builder_task_tracker AS build_task_tracker_debug
RUN python3 manager.py task_tracker build_debug


FROM base_builder_task_tracker AS build_task_tracker_release
RUN python3 manager.py task_tracker build_release


FROM base_runner AS runner_task_tracker_release
COPY --from=build_task_tracker_debug /src/build/Release/task_tracker/TASK_TRACKER /app


FROM base_runner AS runner_task_tracker_debug
COPY --from=build_task_tracker_debug /src/build/Debug/task_tracker/TASK_TRACKER /app
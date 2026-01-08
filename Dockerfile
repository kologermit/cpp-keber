ARG RUNNER_IMAGE=ubuntu:latest

FROM ${RUNNER_IMAGE}

ARG USER_ID=1001
ARG GROUP_ID=1001

RUN groupadd -g $GROUP_ID runner && \
    useradd -u $USER_ID -g runner -m -s /bin/bash runner

RUN touch /env.json && \
    mkdir -p /volumes/logs /volumes/tmp /volumes/file_buffer \
    && chown runner:runner / /volumes/logs /volumes/tmp /volumes/file_buffer /env.json \
    && chmod 755 /volumes/logs /volumes/tmp /volumes/file_buffer /env.json

WORKDIR /

CMD ["/app"]
import sys; sys.path.extend(['../'*i for i in range(10)]+['.'])

from utils.Python.config import (
    default_config, 
    LISTEN_IP,
    LISTEN_PORT,
    TEST_YOUTUBE_VIDEO,
    DB_HOST,
    DB_PORT,
    DB_NAME,
    DB_USER,
    DB_PASSWORD,
    RABBITMQ_HOST,
    RABBITMQ_PORT,
    RABBITMQ_VHOST,
    RABBITMQ_USER,
    RABBITMQ_PASS,
    DOWNLOADER_QUEUE_NAME,
    GOOGLE_EMAIL,
    GOOGLE_PASSWORD,
    SELENIUM_PAGE_LOAD_TIME,
    SELENIUM_HOST,
    LOGS_DIR,
    USE_OAUTH,
)
from utils.Python.parse_config import (
    parse_config_to_exec,
    required,
    project_summary
)

PROJECT_NAME = 'cpp-keber-downloader'
PROJECT_SUMMARY = 'Сервис скачивания с ютуба'

for key in [
    LISTEN_IP,
    LISTEN_PORT,
    TEST_YOUTUBE_VIDEO,
    DB_HOST,
    DB_PORT,
    DB_NAME,
    DB_USER,
    DB_PASSWORD,
    RABBITMQ_HOST,
    RABBITMQ_PORT,
    RABBITMQ_VHOST,
    RABBITMQ_USER,
    RABBITMQ_PASS,
    DOWNLOADER_QUEUE_NAME,
    GOOGLE_EMAIL,
    GOOGLE_PASSWORD,
    SELENIUM_PAGE_LOAD_TIME,
    SELENIUM_HOST,
    LOGS_DIR,
    USE_OAUTH,
]:
    default_config[key][required] = True

exec(parse_config_to_exec({
    **default_config,
    project_summary: PROJECT_SUMMARY,
}))
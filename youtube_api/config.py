import sys; sys.path.extend(['../'*i for i in range(10)]+['.'])

from utils.Python.config import (
    LOGS_DIR, 
    LISTEN_IP, 
    LISTEN_PORT,
    TEST_YOUTUBE_VIDEO,
    BOT_URL,
    BOT_ADMINS,
    USE_OAUTH,
    AUTH_KEY,
    default_config
)
from utils.Python.parse_config import (
    parse_config_to_exec,
    project_summary,
    required,
)

PROJECT_NAME = 'cpp-keber-youtube-api'
PROJECT_SUMMARY = 'Сервис работы с АПИ Ютуб'

for key in [
    LOGS_DIR, 
    LISTEN_IP, 
    LISTEN_PORT,
    TEST_YOUTUBE_VIDEO,
    USE_OAUTH,
    BOT_URL,
    BOT_ADMINS,
    AUTH_KEY,
]:
    default_config[key][required] = True

exec(parse_config_to_exec({
    **default_config,
    project_summary: PROJECT_SUMMARY, # type: ignore
}))
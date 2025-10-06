import sys; sys.path.extend(['../'*i for i in range(10)]+['.'])

from utils.Python.config import (
    SELENIUM_HOST,
    SELENIUM_PAGE_LOAD_TIME,
    LOGS_DIR, 
    LISTEN_IP, 
    LISTEN_PORT,
    GOOGLE_PASSWORD, 
    GOOGLE_EMAIL,
    TEST_YOUTUBE_VIDEO,
    DB_HOST, 
    DB_PORT, 
    DB_NAME, 
    DB_USER, 
    DB_PASSWORD, 
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
    SELENIUM_HOST,
    SELENIUM_PAGE_LOAD_TIME,
    LOGS_DIR, 
    LISTEN_IP, 
    LISTEN_PORT,
    GOOGLE_PASSWORD, 
    GOOGLE_EMAIL,
    TEST_YOUTUBE_VIDEO,
    DB_HOST, 
    DB_PORT, 
    DB_NAME, 
    DB_USER, 
    DB_PASSWORD
]:
    default_config[key][required] = True

exec(parse_config_to_exec({
    **default_config,
    project_summary: PROJECT_SUMMARY,
}))
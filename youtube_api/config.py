import sys; sys.path.extend(['../'*i for i in range(10)]+['.'])

from utils.Python.config import *
from utils.Python.parse_config import (
    parse_config_to_exec,
    summary,
    default,
    project_summary,
    required,
    json
)

PROJECT_NAME = 'cpp-keber-youtube-api'
PROJECT_SUMMARY = 'Сервис работы с АПИ Ютуб'

GOOGLE_EMAIL = 'GOOGLE_EMAIL'
GOOGLE_PASSWORD = 'GOOGLE_PASSWORD'
SELENIUM_HOST = 'SELENIUM_HOST'
SELENIUM_PAGE_LOAD_TIME: int = 'SELENIUM_PAGE_LOAD_TIME'
TEST_YOUTUBE_VIDEO = 'TEST_YOUTUBE_VIDEO'

exec(parse_config_to_exec({
    **default_config,
    project_summary: PROJECT_SUMMARY,
    GOOGLE_EMAIL: {
        summary: 'Гугл-почта для активации через selenium',
        required: True
    },
    GOOGLE_PASSWORD: {
        summary: 'Пароль от гугл-почты',
        required: True
    },
    SELENIUM_HOST: {
        summary: 'Хост от селениума. По умолчанию http://localhost:4444/wd/hub',
        default: 'http://localhost:4444/wd/hub'
    },
    SELENIUM_PAGE_LOAD_TIME: {
        summary: 'Время в секундах для загрузки страницы в селениум. По умолчанию 10',
        json: True, default: 10
    },
    TEST_YOUTUBE_VIDEO: {
        summary: 'Видео для проверки доступа к Ютубу. По умолчанию https://www.youtube.com/watch?v=jiT2Mak9AzI',
        default: 'https://www.youtube.com/watch?v=jiT2Mak9AzI'
    }
}))
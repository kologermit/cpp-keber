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

LOGS_DIR='LOGS_DIR'
BOT_URL = 'BOT_URL'
LISTEN_IP = 'LISTEN_IP'
LISTEN_PORT = 'LISTEN_PORT'
GOOGLE_EMAIL = 'GOOGLE_EMAIL'
GOOGLE_PASSWORD = 'GOOGLE_PASSWORD'
SELENIUM_HOST = 'SELENIUM_HOST'
SELENIUM_PAGE_LOAD_TIME: int = 'SELENIUM_PAGE_LOAD_TIME'
TEST_YOUTUBE_VIDEO = 'TEST_YOUTUBE_VIDEO'

exec(parse_config_to_exec({
    project_summary: PROJECT_SUMMARY,
    LOGS_DIR: {
        summary: 'Путь к папке с логами. \n'
                        'Если значение не задано, то лог не будет выводиться в файлы. \n'
                        'По умолчанию ""',
        default: ''
    },
    BOT_URL: {
        summary: 'Ссылка на на АПИ ботаю По умолчанию http://localhost:8080',
        default: 'http://localhost:8080',
    },
    LISTEN_IP: {
        summary: 'Прослушиваемый IP. По умолчанию 0.0.0.0',
        default: '0.0.0.0'
    },
    LISTEN_PORT: {
        summary: 'Прослушиваемый порт. По умолчанию 8080',
        default: '8080'
    },
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
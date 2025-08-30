from utils.Python.parse_config import (
    parse_config_to_exec,
    summary,
    default,
    project_summary
)

PROJECT_NAME = 'cpp-keber-youtube-api'
PROJECT_SUMMARY = 'Сервис работы с АПИ Ютуб'

LOGS_DIR='LOGS_DIR'
BOT_URL = 'BOT_URL'
LISTEN_IP = 'LISTEN_IP'
LISTEN_PORT = 'LISTEN_PORT'

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
    }
}))
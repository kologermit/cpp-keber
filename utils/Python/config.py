# Общий конфиг для всех питон-сервисов
from copy import deepcopy
from .parse_config import (
    summary,
    default,
    required,
    json
)

LOGS_DIR = 'LOGS_DIR'
TMP_DIR = 'TMP_DIR'
FILE_BUFFER_DIR = 'FILE_BUFFER_DIR'
BOT_URL = 'BOT_URL'
BOT_ADMINS: list[int] = 'BOT_ADMINS' # type: ignore
LISTEN_IP = 'LISTEN_IP'
LISTEN_PORT: int = 'LISTEN_PORT' # type: ignore
PROXY_HOST = 'PROXY_HOST'
PROXY_PORT: int = 'PROXY_PORT' # type: ignore
DB_HOST = 'DB_HOST'
DB_PORT: int = 'DB_PORT' # type: ignore
DB_NAME = 'DB_NAME'
DB_USER = 'DB_USER'
DB_PASSWORD = 'DB_PASSWORD'
DOWNLOADER_QUEUE_NAME = 'DOWNLOADER_QUEUE_NAME'
MERGER_QUEUE_NAME = 'MERGER_QUEUE_NAME'
TEST_YOUTUBE_VIDEO = 'TEST_YOUTUBE_VIDEO'
USE_OAUTH: bool = 'USE_OAUTH' # type: ignore
AUTH_KEY = 'AUTH_KEY'

default_config: dict[str, dict[str, str|bool]] = {
    LOGS_DIR: {
        summary: 'Путь к папке с логами. \n'
            'Если значение не задано, то лог не будет выводиться в файлы. \n'
            'По умолчанию ""',
        default: ''
    },
    TMP_DIR: {
        summary: 'Путь ко временным файлам. По умолчанию ./temp',
        default: './temp'
    },
    FILE_BUFFER_DIR: {
        summary: 'Путь к папке обмена (буффер) между сервисами. По умолчанию ./file_buffer',
        default: './file_buffer'
    },
    BOT_URL: {
        summary: 'Ссылка на АПИ бота. По умолчанию http://localhost:8080',
        default: 'http://localhost:8080',
    },
    BOT_ADMINS: { # type: ignore
        summary: 'Список tg-id админов дял рассылки системных уведомлений. По умолчанию []',
        default: '[]',
        json: True,
    },
    AUTH_KEY: {
        summary: 'Ключ авторизации для сервисов. По умолчанию ""',
        default: 'qwerty',
    },
    LISTEN_IP: {
        summary: 'Прослушиваемый IP. По умолчанию 0.0.0.0',
        default: '0.0.0.0'
    },
    LISTEN_PORT: {
        summary: 'Прослушиваемый порт. По умолчанию 8080',
        default: '8080',
        json: True,
    },
    PROXY_HOST: {
        summary: 'http хост для прокси. По умолчанию пуст',
        default: '',
    },
    PROXY_PORT: {
        summary: 'порт для http прокси. По умолчанию -1',
        json: True,
        default: -1,
    },
    DB_HOST: {
        summary: 'Хост БД Postgres. По умолчанию localhost', 
        default: 'localhost'
    },
    DB_PORT: {
        summary: 'Порт БД Postgres. По умолчанию 5432', 
        json: True,
        default: '5432'
    },
    DB_USER: {
        summary: 'Имя пользователя БД. По муолчанию postgres', 
        default: 'postgres'
    },
    DB_PASSWORD: {
        summary: 'Пароль БД Postgres. По умолчанию qwerty', 
        default: 'qwerty'
    },
    DB_NAME: {
        summary: 'Имя БД Postgres. По умолчанию bot', 
        default: 'bot'
    },
    DOWNLOADER_QUEUE_NAME: {
        summary: 'Очередь скачивания. По умолчанию downloader',
        default: 'downloader'
    },
    MERGER_QUEUE_NAME: {
        summary: 'Очередь объединения. По умолчанию merger',
        default: 'merger'
    },
    TEST_YOUTUBE_VIDEO: {
        summary: 'Видео для проверки доступа к Ютубу. По умолчанию https://www.youtube.com/watch?v=jiT2Mak9AzI',
        default: 'https://www.youtube.com/watch?v=jiT2Mak9AzI'
    },
    USE_OAUTH: {
        summary: 'Используется ли авторизация по гугл-почте. По умолчанию false',
        json: True,
        default: 'false'
    }
}

for key in deepcopy(default_config):
    default_config[key][required] = False
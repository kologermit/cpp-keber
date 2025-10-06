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
BOT_URL = 'BOT_URL'
LISTEN_IP = 'LISTEN_IP'
LISTEN_PORT = 'LISTEN_PORT'
DB_HOST = 'DB_HOST'
DB_PORT = 'DB_PORT'
DB_NAME = 'DB_NAME'
DB_USER = 'DB_USER'
DB_PASSWORD = 'DB_PASSWORD'
RABBITMQ_HOST = 'RABBITMQ_HOST'
RABBITMQ_PORT: int = 'RABBITMQ_PORT'
RABBITMQ_USER = 'RABBITMQ_USER'
RABBITMQ_PASS = 'RABBITMQ_PASS'
RABBITMQ_VHOST = 'RABBITMQ_VHOST'
DOWNLOADER_QUEUE_NAME = 'DOWNLOADER_QUEUE_NAME'
MERGER_QUEUE_NAME = 'MERGER_QUEUE_NAME'
GOOGLE_EMAIL = 'GOOGLE_EMAIL'
GOOGLE_PASSWORD = 'GOOGLE_PASSWORD'
SELENIUM_HOST = 'SELENIUM_HOST'
SELENIUM_PAGE_LOAD_TIME: int = 'SELENIUM_PAGE_LOAD_TIME'
TEST_YOUTUBE_VIDEO = 'TEST_YOUTUBE_VIDEO'

default_config = {
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
    BOT_URL: {
        summary: 'Ссылка на АПИ бота. По умолчанию http://localhost:8080',
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
    RABBITMQ_HOST: {
        summary: 'Хост AMQP. По умолчанию localhost',
        default: 'localhost'
    },
    RABBITMQ_PORT: {
        summary: 'Порт AMQP. По умолчанию 5672',
        json: True,
        default: '5672'
    },
    RABBITMQ_USER: {
        summary: 'Пользователь AMQP. По умолчанию admin',
        default: 'admin'
    },
    RABBITMQ_PASS: {
        summary: 'Пароль AMQP. По умолчанию qwerty',
        default: 'qwerty'
    },
    RABBITMQ_VHOST: {
        summary: 'AMQP vhost. По умолчанию /',
        default: '/'
    },
    DOWNLOADER_QUEUE_NAME: {
        summary: 'Очередь скачивания. По умолчанию downloader',
        default: 'downloader'
    },
    MERGER_QUEUE_NAME: {
        summary: 'Очередь объединения. По умолчанию merger',
        default: 'merger'
    },
    GOOGLE_EMAIL: {
        summary: 'Гугл-почта для активации через selenium',
    },
    GOOGLE_PASSWORD: {
        summary: 'Пароль от гугл-почты',
    },
    SELENIUM_HOST: {
        summary: 'Хост от селениума. По умолчанию http://localhost:4444/wd/hub',
        default: 'http://localhost:4444/wd/hub'
    },
    SELENIUM_PAGE_LOAD_TIME: {
        summary: 'Время в секундах для загрузки страницы в селениум. По умолчанию 10',
        json: True,
        default: '10'
    },
    TEST_YOUTUBE_VIDEO: {
        summary: 'Видео для проверки доступа к Ютубу. По умолчанию https://www.youtube.com/watch?v=jiT2Mak9AzI',
        default: 'https://www.youtube.com/watch?v=jiT2Mak9AzI'
    }
}

for key in deepcopy(default_config):
    default_config[key][required] = False
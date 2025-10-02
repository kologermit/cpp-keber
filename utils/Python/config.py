# Общий конфиг для всех питон-сервисов

from .parse_config import (
    summary,
    default,
    json
)

LOGS_DIR = 'LOGS_DIR'
TMP_DIR = 'TMP_DIR'
BOT_URL = 'BOT_URL'
LISTEN_IP = 'LISTEN_IP'
LISTEN_PORT = 'LISTEN_PORT'
RABBITMQ_HOST = 'RABBITMQ_HOST'
RABBITMQ_PORT: int = 'RABBITMQ_PORT'
RABBITMQ_USER = 'RABBITMQ_USER'
RABBITMQ_PASS = 'RABBITMQ_PASS'
RABBITMQ_VHOST = 'RABBITMQ_VHOST'
DOWNLOADER_QUEUE_NAME = 'DOWNLOADER_QUEUE_NAME'
MERGER_QUEUE_NAME = 'MERGER_QUEUE_NAME'

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
    }
}

__all__ = [
    default_config,
    LOGS_DIR,
    TMP_DIR,
    BOT_URL,
    LISTEN_IP,
    LISTEN_PORT,
    RABBITMQ_HOST,
    RABBITMQ_PORT,
    RABBITMQ_USER,
    RABBITMQ_PASS,
    RABBITMQ_VHOST,
    DOWNLOADER_QUEUE_NAME,
    MERGER_QUEUE_NAME,
]
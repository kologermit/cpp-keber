# Встроенные модули
from json.decoder import JSONDecodeError

# Внутренние модули
from config import (
    PROJECT_NAME,
    TEST_YOUTUBE_VIDEO,
    RABBITMQ_HOST, 
    RABBITMQ_PORT, 
    RABBITMQ_PASS, 
    RABBITMQ_USER, 
    RABBITMQ_VHOST, 
    DOWNLOADER_QUEUE_NAME,
    LOGS_DIR,
    SELENIUM_PAGE_LOAD_TIME,
    SELENIUM_HOST,
    GOOGLE_EMAIL,
    GOOGLE_PASSWORD,
    DB_HOST,
    DB_PORT,
    DB_NAME,
    DB_USER,
    DB_PASSWORD,
    USE_OAUTH,
)
from models import list_models
from audio_handler import audio_handler
from utils.Python.types import Message, DownloadType
from utils.Python.logger import init as init_logger, log_sync_exception
from utils.Python.db import init as init_db
from utils.Python.pythubefix_settings import init as init_pytubefix

# Внешние модули
from loguru import logger
from pika import PlainCredentials, ConnectionParameters, BlockingConnection
from pika.adapters.blocking_connection import BlockingChannel
from pika.spec import Basic, BasicProperties

@log_sync_exception
def handle_message(
        channel: BlockingChannel, 
        method: Basic.Deliver, 
        properties: BasicProperties, 
        body: bytes
    ):
    logger.info({'event': 'NEW_MESSAGE', 'payload': body.decode()})
    try:
        message = Message(body.decode())
        if message.data[0] == DownloadType.AUDIO:
            audio_handler(message)
    except (JSONDecodeError, KeyError):
        pass

def waiting_work():
    try:
        credentials = PlainCredentials(RABBITMQ_USER, RABBITMQ_PASS)
        parameters = ConnectionParameters(
            host=RABBITMQ_HOST,
            port=RABBITMQ_PORT,
            virtual_host=RABBITMQ_VHOST,
            credentials=credentials
        )
        connection = BlockingConnection(parameters)
        channel = connection.channel()
        channel.queue_declare(
        queue=DOWNLOADER_QUEUE_NAME,
            durable=True,
            auto_delete=False,
            arguments={'x-queue-mode': 'lazy'}
        )
        channel.basic_consume(
            queue=DOWNLOADER_QUEUE_NAME,
            on_message_callback=handle_message,
            auto_ack=True
        )
        logger.info({'event': 'START_SERVICE', 'service': {PROJECT_NAME}})
        channel.start_consuming()
    except (KeyboardInterrupt):
        logger.info({'event': 'GRACEFUL_STOP'})
        if connection:
            logger.info({'event': 'CLOSING_CONNECTION'})
            connection.close()
            return

def main():
    init_logger(
        LOGS_DIR, 
        PROJECT_NAME
    )
    init_db(
        DB_HOST,
        DB_PORT,
        DB_NAME,
        DB_USER,
        DB_PASSWORD,
        list_models
    )
    init_pytubefix(
        GOOGLE_EMAIL,
        GOOGLE_PASSWORD,
        SELENIUM_HOST,
        SELENIUM_PAGE_LOAD_TIME,
        TEST_YOUTUBE_VIDEO,
        USE_OAUTH
    )
    waiting_work()
    
if __name__ == "__main__":
    main()
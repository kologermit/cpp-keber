# Встроенные модули
from asyncio import Future, run
from asyncio.exceptions import CancelledError

# Внутренние модули
from config import (
    PROJECT_NAME,
    LISTEN_IP,
    LISTEN_PORT,
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
from utils.Python.logger import init as init_logger, log_async_exception
from utils.Python.db import init as init_db
from utils.Python.pythubefix_settings import init as init_pytubefix

# Внешние модули
from loguru import logger
from aio_pika import connect, IncomingMessage, Connection

@log_async_exception
async def handle_message(message: IncomingMessage):
    logger.info({'event': 'NEW_MESSAGE', 'payload': message.body.decode()})

async def create_rabbitmq_connection():
    logger.info({'event': 'CREATE_AMQP_CONNECTION', 'host': f'{RABBITMQ_HOST}:{RABBITMQ_PORT}', 'vhost': RABBITMQ_VHOST, 'user': RABBITMQ_USER})
    amqp_url = f"amqp://{RABBITMQ_USER}:{RABBITMQ_PASS}@{RABBITMQ_HOST}:{RABBITMQ_PORT}{RABBITMQ_VHOST}"
    return await connect(amqp_url)

async def setup_consumer(connection: Connection):
    prefetch_count = 1
    durable = True
    logger.info({'event': 'SETUP_CONSUMER', 'queue': DOWNLOADER_QUEUE_NAME, 'prefetch_count': prefetch_count, 'durable': durable})
    channel = await connection.channel()
    await channel.set_qos(prefetch_count=prefetch_count)
    queue = await channel.declare_queue(
        DOWNLOADER_QUEUE_NAME, 
        durable=True
    )
    tag = await queue.consume(handle_message)
    logger.info({'event': 'NEW_CONSUMER', 'tag': tag})
    return tag

@log_async_exception
async def waiting_work():
    while True:
        connection = None
        try:
            connection = await create_rabbitmq_connection()
            await setup_consumer(connection)
            await Future()
        except (KeyboardInterrupt, CancelledError):
            logger.info({'event': 'GRACEFUL_ENGING'})
            if connection:
                logger.info({'event': 'CLOSING_CONNECTION'})
                await connection.close()
                return

async def main():
    init_logger(
        LOGS_DIR, 
        PROJECT_NAME
    )
    await init_db(
        DB_HOST,
        DB_PORT,
        DB_NAME,
        DB_USER,
        DB_PASSWORD
    )
    init_pytubefix(
        GOOGLE_EMAIL,
        GOOGLE_PASSWORD,
        SELENIUM_HOST,
        SELENIUM_PAGE_LOAD_TIME,
        TEST_YOUTUBE_VIDEO,
        USE_OAUTH
    )
    logger.info({'event': 'START_SERVICE', 'service': {PROJECT_NAME}, 'listen': f'{LISTEN_IP}:{LISTEN_PORT}'})
    await waiting_work()
    
if __name__ == "__main__":
    run(main())
import asyncio
from config import RABBITMQ_HOST, RABBITMQ_PORT, RABBITMQ_PASS, RABBITMQ_USER, RABBITMQ_VHOST, DOWNLOADER_QUEUE_NAME
from aio_pika import connect, IncomingMessage

async def handle_message(message: IncomingMessage):
    try:
        print(f" [x] Received message: {message.body.decode()}")
        
        await asyncio.sleep(1)
        
        print(" [x] Message processed successfully")
        
    except Exception as e:
        print(f" [!] Error processing message: {e}")
        raise

async def create_rabbitmq_connection():
    amqp_url = f"amqp://{RABBITMQ_USER}:{RABBITMQ_PASS}@{RABBITMQ_HOST}:{RABBITMQ_PORT}{RABBITMQ_VHOST}"
    return await connect(amqp_url)

async def setup_consumer(connection):
    channel = await connection.channel()
    await channel.set_qos(prefetch_count=1)
    
    queue = await channel.declare_queue(
        DOWNLOADER_QUEUE_NAME, 
        durable=True
    )
    
    return await queue.consume(handle_message)

async def run_worker():
    connection = None
    try:
        print(" [*] Connecting to RabbitMQ...")
        connection = await create_rabbitmq_connection()
        
        print(" [*] Setting up consumer...")
        consumer_tag = await setup_consumer(connection)
        print(f" [*] Consumer started with tag: {consumer_tag}")
        print(" [*] Waiting for messages. Press CTRL+C to stop")
        
        await asyncio.Future()
        
    except KeyboardInterrupt:
        print("\n [x] Received interrupt signal")
    except Exception as e:
        print(f" [!] Worker error: {e}")
    finally:
        if connection:
            print(" [x] Closing connection...")
            await connection.close()

def main():
    asyncio.run(run_worker())
    
if __name__ == "__main__":
    main()
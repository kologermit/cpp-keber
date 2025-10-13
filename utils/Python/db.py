from loguru import logger
from peewee import PostgresqlDatabase, Model

def init(
        host='localhost', 
        port=5432, 
        db='bot', 
        user='postgres', 
        password='qwerty', 
        models: list[Model] = []
    ):
    connection = PostgresqlDatabase(db, user=user, password=password, host=host, port=port)
    connection.bind(models)
    logger.info({
        'event': 'INIT_DB_CONNECTION', 
        'user': user, 
        'db': db, 
        'host': host,
        'port': port
    })
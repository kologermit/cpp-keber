# Модуль настройки логгера

# Встроенные модули
from os import path, environ
from uuid import uuid4 as uuid
from datetime import datetime

# Внешние модули
from loguru import logger

# Глобальные переменные, которые меняются при инициации
service = None

# Процедура подготовки логгера
def init(
        logs_dir: str='',
        service_name: str='defaultservice',
) -> None:

    # Обновление глобальных переменных
    global service
    service = service_name

    # Отключение буффера для вывода в stdout
    environ['PYTHONUNBUFFERED']='1'

    # Если в конфиге указана папка для логов,
    # то будет создана эта папка.
    # И файлы лога будут создаваться каждый день
    if logs_dir:
        logger.add(
            path.join(logs_dir, service+'-{time:YYYY-MM-DD}.log'),
            rotation='00:00',
            serialize=True,
            level='INFO',
            backtrace=True,
            diagnose=True,
        )
        logger.info({'event': 'INIT_LOGS_DIR', 'log_path': logs_dir})

# Общая функция для лога ошибки и рассылки сообщения админам
def log_err_with_code(err: Exception) -> str:
    code = str(uuid())
    logger.exception(code, err)
    return code

# Декоратор для отлова любых ошибок
def log_sync_exception(f):
    def _(*args, **kwargs):
        try:
            return f(*args, **kwargs)
        except Exception as err:
            log_err_with_code(err)
            return err
    return _

# Декоратор для отлова любых ошибок в асинхронных функциях
def log_async_exception(f):
    async def _(*args, **kwargs):
        try:
            return await f(*args, **kwargs)
        except Exception as err:
            log_err_with_code(err)
            return err
    return _
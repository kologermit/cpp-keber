import sys; sys.path.extend(['../'*i for i in range(10)]+['.'])

from utils.Python.config import *
from utils.Python.parse_config import (
    parse_config_to_exec,
    project_summary
)

PROJECT_NAME = 'cpp-keber-downloader'
PROJECT_SUMMARY = 'Сервис скачивания с ютуба'

exec(parse_config_to_exec({
    **default_config,
    project_summary: PROJECT_SUMMARY,
}))
#!python3

from os import path, listdir, environ
from copy import deepcopy
from json import loads, dumps
from json.decoder import JSONDecodeError
from sys import argv
from subprocess import run


PROCESS_ARG = '--process'
PROCESS_ARG_WITH_SPACE = f' {PROCESS_ARG} '
PROCESS = argv.count(PROCESS_ARG)
DESCRIPTION_KEY = 'description'
SCRIPTS_KEY = 'scripts'
WORKING_DIR_KEY = 'working_dir'

COMMAND_KEY_DATA_IS_NOT_DICT_ERROR = f'File: {{file}}. Key: {{key}}. Key \'{SCRIPTS_KEY}\' not found'
JSON_DECODE_ERROR = 'Cannot json decode file {file}'
LEN_OF_ARGS_ERROR = 'Count of arguments must be >= 2!'
PERMISSION_DENIED_ERROR = 'Permission denied to file {file}!'
ROOT_IS_NOT_DICT_ERROR = 'File: {file}. Root is not dict!'
SCRIPT_NOT_FOUND_ERROR = 'Script \'{script}\' not found!'
VALUE_IS_NOT_TYPE_ERROR = 'File: {file}. Key: {key}. Key {key2}. Type is not {type}'
VALUE_TYPE_IS_NOT_DICT_ERROR = 'File: {file}. Key: {key}. Type is not dict!'
WORKING_DIR_NOT_FOUND_ERROR = 'Working dir {dir} not found!'

FILE_BASENAME = path.basename(__file__)
USAGE_COMMAND = f'python3 {FILE_BASENAME}'
HELP_COMMANDS = ['help', '--help']
HELP_STR = f'To get help run {USAGE_COMMAND} {HELP_COMMANDS[0]}'
START_SCRIPT_MSG = f'{FILE_BASENAME}[{{process}}]: {{script}}'
KEYBOARD_INTERRUPT_MSG = 'KeyboardInterrupt (Crtl+C)'
ABSOLUTE_DIR = path.dirname(path.abspath(__file__))

EMPTY_STR = ''
FOLDER_WITH_SCRIPTS = path.join(ABSOLUTE_DIR, './scripts')
ENV_FILE = '.env'
ENV_FILE_SPLIT_SYMBOL = '='
ENV_FILE_COMMENT_SYMBOL = '#'
SELF_RUN_SYMBOL = '@@'
JSON_FILE_EXTENSION = '.json'
MAIN_NAME = '__main__'
PARAM_ARG_FLAG = '--'
READ_MODE = 'r'

ERROR_RETURN_CODE = 1
SUCCESS_RETURN_CODE = 0

def dump_message(msg: str, key="error", info=None, **kwargs) -> str:
    return dumps(
        {
            key: msg.format(**kwargs),
            "help": HELP_STR,
            **({"info": info} if info is not None else {})
        }
    , indent=2, ensure_ascii=False)

def load_env_file() -> None:
    if not path.exists(ENV_FILE):
        return
    
    with open(ENV_FILE, READ_MODE) as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith(ENV_FILE_COMMENT_SYMBOL) or ENV_FILE_SPLIT_SYMBOL not in line:
                continue
            
            if "=" in line:
                key, value = line.split(ENV_FILE_SPLIT_SYMBOL, 1)
                key = key.strip()
                value = value.strip()                
                environ[key] = value

FILE_DESCRIPTION = lambda available_scripts: dump_message(
    msg=f'This script is used to run json scripts from the folder \'{FOLDER_WITH_SCRIPTS}\'',
    key=DESCRIPTION_KEY,
    info={
        "usage": f'{USAGE_COMMAND} [file] [script]',
        "available_scripts": {   
            f"{script.get_action()[0]} {script.get_action()[1]}": script.get_description()
            for script in available_scripts
        }
    }
)


class Script:
    __action__: tuple[str]
    __scripts__: tuple[str]
    __description__: str
    __working_dir__: str

    def __init__(self, action: tuple[str], scripts: tuple[str], working_dir: str = "", description: str = EMPTY_STR):
        self.__action__     = deepcopy(action)
        self.__scripts__   = deepcopy(scripts)
        self.__description__= deepcopy(description)
        self.__working_dir__ = deepcopy(working_dir)
        if not working_dir.startswith('/'):
            self.__working_dir__ = path.join(ABSOLUTE_DIR, working_dir)

    def get_action(self) -> tuple[str]:
        return self.__action__

    def get_scripts(self) -> tuple[str]:
        return self.__scripts__
    
    def get_description(self) -> str:
        return self.__description__

    def get_working_dir(self) -> str:
        return self.__working_dir__

def main() -> int:
    load_env_file()

    for arg in deepcopy(argv):
        if arg.startswith(PARAM_ARG_FLAG):
            argv.remove(arg)

    script_files = listdir(FOLDER_WITH_SCRIPTS)

    scripts: dict[tuple[str], Script] = {}

    for script_file in script_files:
        if JSON_FILE_EXTENSION not in script_file:
            continue
        
        full_path = path.join(FOLDER_WITH_SCRIPTS, script_file)
        not_except = False

        try:
            file = open(full_path, READ_MODE)
            text = file.read()
            file.close()
            data: dict[str, dict[str, list[str]|str]] = loads(text)

            if not isinstance(data, dict):
                raise TypeError(ROOT_IS_NOT_DICT_ERROR.format(file=script_file))
            
            for key, value in data.items():
                if not isinstance(value, dict):
                    raise TypeError(VALUE_TYPE_IS_NOT_DICT_ERROR.format(file=script_file, key=key))
                if SCRIPTS_KEY not in value:
                    raise KeyError(COMMAND_KEY_DATA_IS_NOT_DICT_ERROR.format(file=script_file, key=key))
                if not isinstance(value.get(SCRIPTS_KEY, []), list):
                    raise TypeError(VALUE_IS_NOT_TYPE_ERROR.format(file=script_file, key=key, key2=SCRIPTS_KEY, type=list))   
            not_except = True
            
        except PermissionError:
            print(dump_message(PERMISSION_DENIED_ERROR, file=script_file))
        except IsADirectoryError:
            continue
        except JSONDecodeError:
            print(dump_message(JSON_DECODE_ERROR, file=script_file))
        except (TypeError, KeyError) as err:
            print(dump_message(str(err)))

        if not not_except:
            return ERROR_RETURN_CODE

        clear_filename = script_file\
            .replace(JSON_FILE_EXTENSION, EMPTY_STR)\
            .strip()
        
        for key, value in data.items():
            action = tuple([clear_filename] + [key])
            scripts[action] = Script(
                action=action, 
                scripts=tuple(map(str, value[SCRIPTS_KEY])),
                working_dir=str(value.get(WORKING_DIR_KEY, path.curdir)),
                description=str(value.get(DESCRIPTION_KEY, EMPTY_STR))
            )
    
    if set(HELP_COMMANDS) & set(argv):
        print(FILE_DESCRIPTION(scripts.values()))
        return SUCCESS_RETURN_CODE
    
    if len(argv) < 3:
        print(dump_message(LEN_OF_ARGS_ERROR))
        return ERROR_RETURN_CODE
    
    key = (argv[1], argv[2])
    
    if (script := scripts.get(key)) is None:
        print(dump_message(SCRIPT_NOT_FOUND_ERROR, script=key))
        return ERROR_RETURN_CODE
    
    environ[__file__] = str(int(PROCESS) + 1)

    for final_script in script.get_scripts():
        final_script = final_script.replace(SELF_RUN_SYMBOL, USAGE_COMMAND)
        print(START_SCRIPT_MSG.format(script=final_script, process=PROCESS))
        try:
            result = run(
                final_script\
                    .replace(USAGE_COMMAND, USAGE_COMMAND+PROCESS_ARG_WITH_SPACE*(PROCESS+1)), 
                shell=True, 
                cwd=script.get_working_dir()
            )
        except KeyboardInterrupt:
            print(KEYBOARD_INTERRUPT_MSG)
            return SUCCESS_RETURN_CODE
        except FileNotFoundError:
            print(dump_message(WORKING_DIR_KEY, dir=script.get_working_dir()))
            return ERROR_RETURN_CODE
        if result.returncode != 0:
            return result.returncode

    return SUCCESS_RETURN_CODE    

if __name__ == MAIN_NAME:
    result = main()
    exit(result)
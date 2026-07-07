from copy import deepcopy
from requests import post, patch
from json import dumps

from loguru import logger

class BotAPI:
    def __init__(self, url: str, auth_key: str, admins: list[int]=[]):
        self.url = deepcopy(url)
        self.auth_key = deepcopy(auth_key)
        self.admins = deepcopy(admins)

    def send_message(
            self, 
            chat_id: int, 
            text: str, 
            reply_message_id: int|None=None,
            file_path_in_buffer: str|None=None,
        ) -> int:
        response = post(
            f'{self.url}/message', 
            data=dumps({
                'chat_telegram_id': chat_id, 
                'text': text,
                **(
                    {'reply_message_id': reply_message_id}
                    if reply_message_id is not None else
                    {}
                ),
                **(
                    {'file': file_path_in_buffer}
                    if file_path_in_buffer else
                    {}
                )
            }),
            headers={
                'Authorization': self.auth_key,
            }
        )
        return response.status_code

    def send_message_to_admins(self, text: str):
        try:
            for admin in self.admins:
                status_code = self.send_message(admin, text)
                if status_code != 200:
                    logger.warning({
                        'event': 'FAILED_TO_SEND_MESSAGE_TO_ADMIN',
                        'message': f'Failed to send message to admin {admin}',
                        'status_code': status_code,
                    })
                else:
                    logger.info({'event': 'SEND_MESSAGE_TO_ADMIN', 'admin': admin, 'text': text})
        except ConnectionError:
            logger.warning({
                'event': 'BOT_CONNECTION_ERROR', 
                'message': 'Failed to connect to bot API', 
                'bot_url': self.url
            })

    def edit_message_text(
            self, 
            chat_id: int, 
            message_id: int,
            text: str
        ) -> bool:
        response = patch(
            f'{self.url}/edit_message_text',
            data=dumps({
                'chat_id': chat_id,
                'message_id': message_id,
                'text': text
            })
        )

        if response.status_code != 200:
            return False
        
        return response.json()
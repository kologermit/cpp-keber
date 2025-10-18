from copy import deepcopy
from requests import post, patch

class BotAPI:
    def __init__(self, url: str):
        self.url = deepcopy(url)

    def send_message(
            self, 
            chat_id: int, 
            text: str, 
            reply_message_id: int|None=None,
            file_path_in_buffer: str|None=None,
        ) -> int:
        response = post(
            f'{self.url}/send_text', 
            data={
                'chat_id': chat_id, 
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
            },
        )
        if response.status_code != 200:
            return -1
        
        return response.json()

    def edit_message_text(
            self, 
            chat_id: int, 
            message_id: int,
            text: str
        ) -> bool:
        response = patch(
            f'{self.url}/edit_message_text',
            data={
                'chat_id': chat_id,
                'message_id': message_id,
                'text': text
            }
        )

        if response.status_code != 200:
            return False
        
        return response.json()
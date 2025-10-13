from datetime import datetime
from peewee import Model, BigIntegerField, CharField, DateTimeField, ForeignKeyField

class BaseModel(Model):
    id: int = BigIntegerField(null=False, index=True, column_name="id")
    created_at: datetime = DateTimeField(null=False, column_name="created_at")
    class Meta: 
        database = None

class EnumModel(BaseModel):
    name: str = CharField(max_length=1000, null=False, column_name="name")

class YouTubeAudioSetting(BaseModel):
    user_id: int = BigIntegerField(null=False, column_name="user_id")
    url: str = CharField(max_length=1000, null=False, column_name="url")
    file_name: str = CharField(max_length=1000, null=False, column_name="file_name")
    download_url: str = CharField(max_length=1000, null=False, column_name="download_url")
    updated_at: datetime = DateTimeField(null=False, column_name="updated_at")
    deleted_at: datetime = DateTimeField(null=True, column_name="deleted_at")
    class Meta:
        db_table = "youtube_audio_settings"

class ResultType(EnumModel):
    class Meta:
        db_table = "result_types"

class YouTubeDownloadType(EnumModel):
    class Meta:
        db_table = "youtube_download_types"

class YouTubeDownload(BaseModel):
    youtube_download_type = ForeignKeyField(model=YouTubeDownloadType, null=False, column_name="youtube_download_type")
    result_type = ForeignKeyField(model=ResultType, column_name="result_type")
    youtube_audio_setting = ForeignKeyField(model=YouTubeAudioSetting, column_name="youtube_audio_setting")
    user_id = BigIntegerField(null=False, column_name="user_id")
    url: str = CharField(max_length=1000, null=False, column_name="url")
    class Meta:
        db_table = "youtube_downloads"

list_models: list[Model] = [YouTubeAudioSetting, ResultType, YouTubeDownloadType, YouTubeDownload]
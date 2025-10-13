from datetime import datetime
from peewee import Model, BigIntegerField, CharField, DateTimeField, ForeignKeyField

class BaseModel(Model):
    id: int = BigIntegerField(null=False, index=True)
    created_at: datetime = DateTimeField(null=False)
    class Meta: 
        database = None

class EnumModel(BaseModel):
    name: str = CharField(max_length=1000, null=False)

class YouTubeAudioSetting(BaseModel):
    user_id: int = BigIntegerField(null=False)
    url: str = CharField(max_length=1000, null=False)
    file_name: str = CharField(max_length=1000, null=False)
    download_url: str = CharField(max_length=1000, null=False)
    updated_at: datetime = DateTimeField(null=False)
    deleted_at: datetime = DateTimeField(null=True)
    class Meta:
        db_table = "youtube_audio_settings"

class ResultType(EnumModel):
    class Meta:
        db_table = "result_types"

class YouTubeDownloadType(EnumModel):
    class Meta:
        db_table = "youtube_download_types"

class YouTubeDownload(BaseModel):
    youtube_download_type = ForeignKeyField(model=YouTubeDownloadType, null=False)
    result_type = ForeignKeyField(model=ResultType)
    youtube_audio_setting = ForeignKeyField(model=YouTubeAudioSetting)
    user_id = BigIntegerField(null=False)
    url: str = CharField(max_length=1000, null=False)
    class Meta:
        db_table = "youtube_downloads"

list_models: list[Model] = [YouTubeAudioSetting, ResultType, YouTubeDownloadType, YouTubeDownload]
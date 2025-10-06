from typing import Any
from datetime import (
    datetime as DateTime, 
    date as     Date, 
    time as     Time
)

from tortoise.models import Model
from tortoise import fields as ORMField
from tortoise.expressions import Q
from tortoise.queryset import QuerySet

Bool = bool

class CommonFields:
    id: int =                       ORMField.BigIntField(pk=True, null=False)
    string: str =                   ORMField.CharField(max_length=1000, null=False)
    string_or_null: str|None=       ORMField.CharField(max_length=1000, null=True)
    created_at: DateTime =          ORMField.DatetimeField(auto_now=True, null=False)
    date: Date =                    ORMField.DateField(null=False)
    date_or_null: Date|None =       ORMField.DateField(null=False)
    datetime: DateTime =            ORMField.DatetimeField(null=False)
    datetime_or_null: DateTime|None=ORMField.DatetimeField(null=True)
    time: Time =                    ORMField.TimeField(null=False)
    time_or_null: Time|None =       ORMField.TimeDeltaField(null=True)
    json: Any =                     ORMField.JSONField(null=False)
    json_or_null: Any =             ORMField.JSONField(null=True)
    number: int =                   ORMField.BigIntField(null=False)
    number_or_null: int|None =      ORMField.BigIntField(null=True)
    bool: Bool =                    ORMField.BooleanField(null=False)
    bool_or_null: Bool|None =       ORMField.BooleanField(null=True)
    text: str =                     ORMField.TextField(null=False)
    text_or_null: str|None =        ORMField.TextField(null=True)
    youtube_audio_setting =         ORMField.ForeignKeyField('models.YouTubeAudioSetting', null=True)
    youtube_download_type =         ORMField.ForeignKeyField('models.YouTubeDownloadType', null=True)
    result_type =                   ORMField.ForeignKeyField('models.ResultType', null=False)

@classmethod
def filter_deleted(cls: Model, q=Q(), *args, **kwargs) -> QuerySet[Model]:
    kwargs['deleted_at__isnull']=True
    return cls.filter(q, *args, **kwargs)

class YouTubeAudioSetting(Model):
    id = CommonFields.id
    user_id = CommonFields.number
    url = CommonFields.string
    file_name = CommonFields.string
    download_url = CommonFields.string
    created_at = CommonFields.created_at
    updated_at = CommonFields.date
    deleted_at = CommonFields.date_or_null

    filter_all = filter_deleted

    class Meta:
        table = "youtube_audio_settings"

class ResultType(Model):
    id = CommonFields.id
    name = CommonFields.string
    created_at = CommonFields.created_at
    
    class Meta:
        table = "result_types"

class YouTubeDownloadType(Model):
    id = CommonFields.id
    name = CommonFields.string
    created_at = CommonFields.created_at

    class Meta:
        table = "youtube_download_types"

class YouTubeDownload(Model):
    id = CommonFields.id
    youtube_download_type = CommonFields.youtube_download_type
    result_type = CommonFields.result_type
    youtube_audio_setting = CommonFields.youtube_audio_setting
    user_id = CommonFields.number
    url = CommonFields.string
    created_at = CommonFields.created_at

    class Meta:
        table = "youtube_downloads"

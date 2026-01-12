-- Функция для автоматического обновления updated_at
CREATE OR REPLACE FUNCTION update_updated_at()
RETURNS TRIGGER AS $$
BEGIN
    NEW.updated_at = CURRENT_TIMESTAMP;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;



CREATE TABLE user_screens (
    id         BIGINT UNIQUE NOT NULL,
    name       VARCHAR(255) NOT NULL,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
CREATE INDEX idx_user_screens ON user_screens(id);

CREATE TABLE users (
    id          BIGINT UNIQUE NOT NULL,
    name        VARCHAR(255) NOT NULL,
    username    VARCHAR(255) NULL,
    screen      BIGINT NOT NULL DEFAULT 1 REFERENCES user_screens(id),
    created_at  TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at  TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    deleted_at  TIMESTAMP NULL,

    UNIQUE(id),
    CONSTRAINT fk_users_screen FOREIGN KEY (screen) REFERENCES user_screens(id)
);
CREATE INDEX idx_users ON users(id);
CREATE INDEX ids_users_username ON users(username);
CREATE TRIGGER trigger_update_users_updated_at
BEFORE UPDATE ON users
FOR EACH ROW
EXECUTE FUNCTION update_updated_at();


CREATE TABLE chat_types (
    id         BIGINT UNIQUE NOT NULL,
    name       VARCHAR(255) NOT NULL,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
CREATE INDEX idx_chat_types ON chat_types(id);

CREATE TABLE chats (
    id          BIGINT UNIQUE NOT NULL,
    name        VARCHAR(255) NOT NULL,
    username    VARCHAR(255) NULL,
    type        BIGINT NOT NULL REFERENCES chat_types(id),
    created_at  TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at  TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    deleted_at  TIMESTAMP NULL,

    UNIQUE(id),
    CONSTRAINT fk_chat_types FOREIGN KEY (type) REFERENCES chat_types(id)
);
CREATE INDEX idx_chats ON chats(id);
CREATE INDEX idx_chats_username ON chats(username) WHERE username = '';
CREATE TRIGGER trigger_update_chats_updated_at
BEFORE UPDATE ON chats
FOR EACH ROW
EXECUTE FUNCTION update_updated_at();


CREATE TABLE message_content_types (
    id         BIGINT UNIQUE NOT NULL,
    name       VARCHAR(255) NOT NULL,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,

    UNIQUE(id)
);
CREATE INDEX idx_message_content_types ON message_content_types(id);
CREATE TABLE messages (
    id                  BIGINT NOT NULL,
    text                VARCHAR(6000) NULL,
    file_download_id    VARCHAR(255) NULL,
    file_name           VARCHAR(255) NULL,
    file_content_type   BIGINT NOT NULL,
    chat_id             BIGINT NOT NULL,
    user_id             BIGINT NOT NULL,
    reply_message_id    BIGINT NULL,
    reply_chat_id       BIGINT NULL,
    created_at          TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at          TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    deleted_at          TIMESTAMP NULL,
    
    UNIQUE(id, chat_id),
    CONSTRAINT fk_messages_user FOREIGN KEY (user_id) REFERENCES users(id),
    CONSTRAINT fk_messages_chat FOREIGN KEY (chat_id) REFERENCES chats(id),
    CONSTRAINT fk_messages_reply FOREIGN KEY (reply_message_id, reply_chat_id) REFERENCES messages(id, chat_id),
    CONSTRAINT fk_message_content_types FOREIGN KEY (file_content_type) REFERENCES message_content_types(id)
);
CREATE INDEX idx_messages_id_and_chat_id ON messages(id, chat_id);
CREATE INDEX idx_messages_user_id ON messages(user_id);
CREATE TRIGGER trigger_update_messages_updated_at
BEFORE UPDATE ON messages
FOR EACH ROW
EXECUTE FUNCTION update_updated_at();

CREATE TABLE callbacks (
    id                  VARCHAR(255) NOT NULL,
    data                VARCHAR(1000) NOT NULL,
    message_id          BIGINT NOT NULL,
    user_id             BIGINT NOT NULL,
    chat_id             BIGINT NOT NULL,
    created_at          TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at          TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    deleted_at          TIMESTAMP NULL,

    UNIQUE(id),
    CONSTRAINT fk_messages_reply FOREIGN KEY (message_id, chat_id) REFERENCES messages(id, chat_id),
    CONSTRAINT fk_callbacks_user FOREIGN KEY (user_id) REFERENCES users(id),
    CONSTRAINT fk_callbacks_chat FOREIGN KEY (chat_id) REFERENCES chats(id)
);
CREATE INDEX idx_callbacks ON callbacks(id);
CREATE TRIGGER trigger_update_callbacks_updated_at
    BEFORE UPDATE ON callbacks
    FOR EACH ROW
EXECUTE FUNCTION update_updated_at();


CREATE TABLE api_request_services (
    id         BIGINT NOT NULL,
    name       VARCHAR(255) NOT NULL,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,

    UNIQUE (id)
);
CREATE INDEX idx_api_request_services ON api_request_services(id);
CREATE TABLE api_requests (
    id         SERIAL PRIMARY KEY,
    "from"     BIGINT NOT NULL REFERENCES api_request_services(id),
    "to"       BIGINT NOT NULL REFERENCES api_request_services(id),
    request    JSONB NOT NULL,
    response   JSONB NOT NULL,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    deleted_at TIMESTAMP NULL,

    CONSTRAINT fk_api_request_services_from FOREIGN KEY ("from") REFERENCES api_request_services(id),
    CONSTRAINT fk_api_request_services_to FOREIGN KEY ("to") REFERENCES api_request_services(id)
);
CREATE TRIGGER trigger_update_api_request_updated_at
BEFORE UPDATE ON api_requests
FOR EACH ROW
EXECUTE FUNCTION update_updated_at();



CREATE TABLE access_types (
    id          BIGINT NOT NULL,
    name        VARCHAR(255) NOT NULL,
    created_at  TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,

    UNIQUE(id)
);
CREATE INDEX idx_access_types ON access_types(id);
CREATE TABLE accesses (
    id          SERIAL PRIMARY KEY,
    user_id     BIGINT NOT NULL REFERENCES users(id),
    type        BIGINT NOT NULL REFERENCES access_types(id),
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    deleted_at TIMESTAMP NULL,
    CONSTRAINT fk_access_type FOREIGN KEY (type) REFERENCES access_types(id)
);
CREATE TRIGGER trigger_update_accesses_updated_at
BEFORE UPDATE ON accesses
FOR EACH ROW
EXECUTE FUNCTION update_updated_at();

CREATE TABLE youtube_audio_settings(
    id         SERIAL PRIMARY KEY,
    user_id    BIGINT NOT NULL,
    url        VARCHAR(1000) NOT NULL,
    file_name  VARCHAR(1000) NOT NULL,
    download_url VARCHAR(1000) NOT NULL,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    deleted_at TIMESTAMP NULL,

    CONSTRAINT fk_youtube_audio_settings_user_id FOREIGN KEY (user_id) REFERENCES users(id)
);
CREATE TRIGGER trigger_update_youtube_audio_settings_updated_at
BEFORE UPDATE ON youtube_audio_settings
FOR EACH ROW
EXECUTE FUNCTION update_updated_at();


CREATE TABLE result_types(
    id          SERIAL PRIMARY KEY,
    name        VARCHAR(255) NOT NULL,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE youtube_download_types(
    id          SERIAL PRIMARY KEY,
    name        VARCHAR(255) NOT NULL,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE youtube_downloads(
    id         SERIAL PRIMARY KEY,
    youtube_download_type       BIGINT NOT NULL,
    result_type     BIGINT NOT NULL,
    youtube_audio_setting   BIGINT NULL,
    user_id    BIGINT NOT NULL,
    url        VARCHAR(1000) NOT NULL,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT fk_youtube_download_type FOREIGN KEY (youtube_download_type) REFERENCES youtube_download_types(id),
    CONSTRAINT fk_youtube_download_result_type FOREIGN KEY (result_type) REFERENCES result_types(id),
    CONSTRAINT fk_youtube_download_youtube_audio_setting FOREIGN KEY (youtube_audio_setting) REFERENCES youtube_audio_settings(id),
    CONSTRAINT fk_youtube_download_user_id FOREIGN KEY (user_id) REFERENCES users(id)
);

CREATE TABLE tasks(
    id         SERIAL PRIMARY KEY,
    title      VARCHAR(1000) NOT NULL,
    "description" VARCHAR(2000) NOT NULL,
    user_id    BIGINT NOT NULL,
    start_at TIMESTAMP NOT NULL,
    in_work_at TIMESTAMP NULL,
    completed_at TIMESTAMP NULL,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    deleted_at TIMESTAMP NULL,


    CONSTRAINT fk_task_user_id FOREIGN KEY (user_id) REFERENCES users(id)
);  

CREATE TRIGGER trigger_update_tasks_updated_at
BEFORE UPDATE ON tasks
FOR EACH ROW
EXECUTE FUNCTION update_updated_at();
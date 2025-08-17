-- Функция для автоматического обновления updated_at
CREATE OR REPLACE FUNCTION update_updated_at()
RETURNS TRIGGER AS $$
BEGIN
    NEW.updated_at = CURRENT_TIMESTAMP;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;



CREATE TABLE IF NOT EXISTS user_screens (
    id         SERIAL PRIMARY KEY,
    name       VARCHAR(255) NOT NULL,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
CREATE TABLE IF NOT EXISTS users (
    id          SERIAL PRIMARY KEY,
    name        VARCHAR(255) NOT NULL,
    username    VARCHAR(255) NULL,
    screen      INT NOT NULL DEFAULT 1 REFERENCES user_screens(id),
    telegram_id BIGINT UNIQUE NOT NULL,
    created_at  TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at  TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    deleted_at  TIMESTAMP NULL,
    CONSTRAINT fk_users_screen FOREIGN KEY (screen) REFERENCES user_screens(id)
);
CREATE INDEX idx_users_telegram_id ON users(telegram_id);
CREATE TRIGGER trigger_update_users_updated_at
BEFORE UPDATE ON users
FOR EACH ROW
EXECUTE FUNCTION update_updated_at();



CREATE TABLE IF NOT EXISTS chat_types (
    id         SERIAL PRIMARY KEY,
    name       VARCHAR(255) NOT NULL,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
CREATE TABLE IF NOT EXISTS chats (
    id          SERIAL PRIMARY KEY,
    name        VARCHAR(255) NOT NULL,
    username    VARCHAR(255) NULL,
    telegram_id BIGINT UNIQUE NOT NULL,
    type        INT NOT NULL REFERENCES chat_types(id),
    created_at  TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at  TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    deleted_at  TIMESTAMP NULL,
    CONSTRAINT fk_chat_types FOREIGN KEY (type) REFERENCES chat_types(id)
);
CREATE INDEX idx_chats_telegram_id ON chats(telegram_id);
CREATE TRIGGER trigger_update_chats_updated_at
BEFORE UPDATE ON chats
FOR EACH ROW
EXECUTE FUNCTION update_updated_at();



CREATE TABLE IF NOT EXISTS message_content_types (
    id         SERIAL PRIMARY KEY,
    name       VARCHAR(255) NOT NULL,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
CREATE TABLE IF NOT EXISTS messages (
    id                  SERIAL PRIMARY KEY,
    telegram_id         BIGINT NOT NULL,
    text                VARCHAR(6000) NULL,
    file_download_id    VARCHAR(255) NULL,
    file_name           VARCHAR(255) NULL,
    file_content_type   INT NOT NULL DEFAULT 1 REFERENCES message_content_types(id),
    chat_id             INT NOT NULL,
    user_id             INT NOT NULL,
    reply_message_id    INT NULL,
    created_at          TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at          TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    deleted_at          TIMESTAMP NULL,
    
    CONSTRAINT fk_messages_user FOREIGN KEY (user_id) REFERENCES users(id),
    CONSTRAINT fk_messages_chat FOREIGN KEY (chat_id) REFERENCES chats(id),
    CONSTRAINT fk_messages_reply FOREIGN KEY (reply_message_id) REFERENCES messages(id),
    CONSTRAINT fk_message_content_types FOREIGN KEY (file_content_type) REFERENCES message_content_types(id)
);
CREATE TRIGGER trigger_update_messages_updated_at
BEFORE UPDATE ON messages
FOR EACH ROW
EXECUTE FUNCTION update_updated_at();



CREATE TABLE IF NOT EXISTS api_request_services (
    id         INT PRIMARY KEY,
    name       VARCHAR(255) NOT NULL,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
CREATE TABLE IF NOT EXISTS api_requests (
    id         SERIAL PRIMARY KEY,
    "from"     INT NOT NULL REFERENCES api_request_services(id),
    "to"       INT NOT NULL REFERENCES api_request_services(id),
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



CREATE TABLE IF NOT EXISTS user_access_types (
    id          INT PRIMARY KEY,
    name        VARCHAR(255) NOT NULL,
    created_at  TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
CREATE TABLE IF NOT EXISTS user_accesses (
    id          SERIAL PRIMARY KEY,
    user_id     INT NOT NULL REFERENCES user(id),
    type        INT NOT NULL REFERENCES user_access_types(id),
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    deleted_at TIMESTAMP NULL,
    CONSTRAINT fk_user_access_type FOREIGN KEY (type) REFERENCES user_access_types(id)
);
CREATE TRIGGER trigger_update_api_request_updated_at
BEFORE UPDATE ON user_accesses
FOR EACH ROW
EXECUTE FUNCTION update_updated_at();
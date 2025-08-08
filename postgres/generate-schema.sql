-- Функция для автоматического обновления updated_at
CREATE OR REPLACE FUNCTION update_updated_at()
RETURNS TRIGGER AS $$
BEGIN
    NEW.updated_at = CURRENT_TIMESTAMP;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;



-- Таблица пользователей users
CREATE TABLE IF NOT EXISTS users (
    id          SERIAL PRIMARY KEY,
    name        VARCHAR(255) NOT NULL,
    username    VARCHAR(255) NULL,
    screen      INT NOT NULL DEFAULT 1,
    telegram_id BIGINT UNIQUE NOT NULL,
    created_at  TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at  TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    deleted_at  TIMESTAMP NULL
);
CREATE INDEX idx_users_telegram_id ON users(telegram_id);
CREATE TRIGGER trigger_update_users_updated_at
BEFORE UPDATE ON users
FOR EACH ROW
EXECUTE FUNCTION update_updated_at();

-- Таблица user_screens
CREATE TABLE IF NOT EXISTS user_screens (
    id   SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
-- Внешний ключ для screen
ALTER TABLE users ADD CONSTRAINT fk_users_screen
FOREIGN KEY (screen) REFERENCES user_screens(id)
ON DELETE SET DEFAULT;
-- Таблица чатов chats
CREATE TABLE IF NOT EXISTS chats (
    id          SERIAL PRIMARY KEY,
    name        VARCHAR(255) NOT NULL,
    username    VARCHAR(255) NULL,
    telegram_id BIGINT UNIQUE NOT NULL,
    type        INT NOT NULL,
    created_at  TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at  TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    deleted_at  TIMESTAMP NULL
);
CREATE INDEX idx_chats_telegram_id ON chats(telegram_id);
-- Триггер для updated_at
CREATE TRIGGER trigger_update_chats_updated_at
BEFORE UPDATE ON chats
FOR EACH ROW
EXECUTE FUNCTION update_updated_at();
-- Таблица chat_types
CREATE TABLE IF NOT EXISTS chat_types (
    id   SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
-- Внешний ключ для type
ALTER TABLE chats ADD CONSTRAINT fk_chat_types
FOREIGN KEY (type) REFERENCES chat_types(id)
ON DELETE SET DEFAULT;
-- Таблица сообщений messages
CREATE TABLE IF NOT EXISTS messages (
    id                  SERIAL PRIMARY KEY,
    telegram_id         BIGINT NOT NULL,
    text                VARCHAR(6000) NULL,
    file_download_id    VARCHAR(255) NULL,
    file_name           VARCHAR(255) NULL,
    file_content_type   INT NOT NULL DEFAULT 1,
    chat_id             INT NOT NULL,
    user_id             INT NOT NULL,
    reply_message_id    INT NULL,
    created_at          TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at          TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    deleted_at          TIMESTAMP NULL,
    
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE SET NULL,
    FOREIGN KEY (chat_id) REFERENCES chats(id) ON DELETE SET NULL,
    FOREIGN KEY (reply_message_id) REFERENCES messages(id) ON DELETE SET NULL
);
-- Триггер для updated_at
CREATE TRIGGER trigger_update_messages_updated_at
BEFORE UPDATE ON messages
FOR EACH ROW
EXECUTE FUNCTION update_updated_at();
-- Таблица message_content_types
CREATE TABLE IF NOT EXISTS message_content_types (
    id   SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
-- Внешний ключ для file_content_type
ALTER TABLE messages ADD CONSTRAINT fk_message_content_types
FOREIGN KEY (file_content_type) REFERENCES message_content_types(id)
ON DELETE SET DEFAULT;

CREATE TABLE IF NOT EXISTS api_requests (
    id         SERIAL PRIMARY KEY,
    "from"     INT NOT NULL,
    "to"       INT NOT NULL,
    request    JSONB NOT NULL,
    response   JSONB NOT NULL,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    deleted_at TIMESTAMP NULL
);
-- Триггер для updated_at
CREATE TRIGGER trigger_update_api_request_updated_at
BEFORE UPDATE ON api_requests
FOR EACH ROW
EXECUTE FUNCTION update_updated_at();
CREATE TABLE IF NOT EXISTS api_request_services (
    id         INT PRIMARY KEY,
    name       VARCHAR(255) NOT NULL,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
-- Внешний ключ для to и from
ALTER TABLE api_requests ADD CONSTRAINT fk_api_request_services_from
FOREIGN KEY ("from") REFERENCES api_request_services(id)
ON DELETE SET DEFAULT;
ALTER TABLE api_requests ADD CONSTRAINT fk_api_request_services_to
FOREIGN KEY ("to") REFERENCES api_request_services(id)
ON DELETE SET DEFAULT;
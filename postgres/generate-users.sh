#!/bin/bash
set -e

echo "
    -- BOT_USER
    CREATE USER $BOT_USER WITH PASSWORD '$BOT_PASSWORD';
    GRANT SELECT, INSERT, UPDATE ON TABLE
        users, user_screens, 
        chats, chat_types, 
        messages, message_content_types,
        callbacks,
        api_requests, api_request_services,
        accesses, access_types,
        youtube_audio_settings
    TO $BOT_USER;
    GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO $BOT_USER;


    -- DOWNLOADER_USER
    CREATE USER $DOWNLOADER_USER WITH PASSWORD '$DOWNLOADER_PASSWORD';
    GRANT SELECT, INSERT ON TABLE
        youtube_audio_settings,
        youtube_download_types,
        youtube_downloads,
        result_types
    TO $DOWNLOADER_USER;
    GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO $DOWNLOADER_USER;

    
    -- YOUTUBE_API
    CREATE USER $YOUTUBE_API_USER WITH PASSWORD '$YOUTUBE_API_PASSWORD';
    GRANT SELECT, INSERT ON TABLE 
        api_requests, api_request_services
    TO $YOUTUBE_API_USER;
    GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO $YOUTUBE_API_USER;


    -- READER_USER
    CREATE ROLE $READER_USER WITH
        LOGIN
        NOSUPERUSER
        NOINHERIT
        NOCREATEDB
        NOCREATEROLE
        NOREPLICATION
        PASSWORD '$READER_PASSWORD';
    GRANT CONNECT ON DATABASE $POSTGRES_DB TO $READER_USER;
    GRANT USAGE ON SCHEMA public TO $READER_USER;
    GRANT SELECT ON ALL TABLES IN SCHEMA public TO $READER_USER;
    ALTER DEFAULT PRIVILEGES IN SCHEMA public GRANT SELECT ON TABLES TO $READER_USER;
    GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO $READER_USER;
    ALTER DEFAULT PRIVILEGES IN SCHEMA public GRANT USAGE, SELECT ON SEQUENCES TO $READER_USER;
" \
| psql --username "$POSTGRES_USER" --dbname "$POSTGRES_DB"
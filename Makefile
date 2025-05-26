ifneq (,$(wildcard ./.env))
    include .env
    export
endif

venv:
	python3 -m venv venv

install_requirements:
	venv/bin/pip install -r requirements.txt

build_conan:
	venv/bin/conan profile detect --force
	venv/bin/conan install . --build missing

.env:
	cp .env.example .env

install_deps:
	make venv
	make install_requirements
	make build_conan
	make .env || true

build_deps:
	cmake . -DCMAKE_BUILD_TYPE=Release -B ./build

build_bot:
	make build_deps
	cmake \
		--build ./build \
		--target BOT
	chmod 777 ./build/BOT

run_bot:
	./build/BOT

dc_all_logs:
	$$DOCKER_COMPOSE_COMMAND logs --tail 100 -f
dc_all_logs0:
	$$DOCKER_COMPOSE_COMMAND logs --tail 0 -f
dc_all_logs_full:
	$$DOCKER_COMPOSE_COMMAND logs -f
dc_all_run:
	$$DOCKER_COMPOSE_COMMAND up -d
dc_all_run_and_logs:
	make dc_all_run
	make dc_all_logs_full
dc_all_force_recreate:
	$$DOCKER_COMPOSE_COMMAND up -d --force-recreate --build
dc_all_force_recreate_and_logs:
	make dc_all_force_recreate
	make dc_all_logs_full
dc_all_restart:
	$$DOCKER_COMPOSE_COMMAND restart
dc_all_restart_and_logs:
	make dc_all_restart
	make dc_all_logs
dc_all_stop:
	$$DOCKER_COMPOSE_COMMAND stop
dc_all_remove:
	$$DOCKER_COMPOSE_COMMAND rm -sfv
dc_all_remove_with_data:
	make dc_all_remove
	$$DOCKER_COMPOSE_COMMAND down --volumes



dc_db_logs:
	$$DOCKER_COMPOSE_COMMAND logs --tail 100 -f db
dc_db_logs0:
	$$DOCKER_COMPOSE_COMMAND logs --tail 0 -f db
dc_db_logs_full:
	$$DOCKER_COMPOSE_COMMAND logs -f db
dc_db_run:
	$$DOCKER_COMPOSE_COMMAND up -d db
dc_db_run_and_logs:
	make dc_db_run
	make dc_db_logs_full
dc_db_force_recreate:
	$$DOCKER_COMPOSE_COMMAND up -d --force-recreate db
dc_db_force_recreate_and_logs:
	make dc_db_force_recreate
	make dc_db_logs_full
dc_db_restart:
	$$DOCKER_COMPOSE_COMMAND restart db
dc_db_restart_and_logs:
	make dc_db_restart
	make dc_db_logs
dc_db_stop:
	$$DOCKER_COMPOSE_COMMAND stop
dc_db_remove:
	$$DOCKER_COMPOSE_COMMAND rm -sfv db
dc_db_exec:
	$$DOCKER_COMPOSE_COMMAND exec db sh


dc_bot_logs:
	$$DOCKER_COMPOSE_COMMAND logs --tail 100 -f bot
dc_bot_logs0:
	$$DOCKER_COMPOSE_COMMAND logs --tail 0 -f bot
dc_bot_logs_full:
	$$DOCKER_COMPOSE_COMMAND logs -f bot
dc_bot_run:
	$$DOCKER_COMPOSE_COMMAND up -d bot
dc_bot_run_and_logs:
	make dc_bot_run
	make dc_bot_logs_full
dc_bot_force_recreate:
	$$DOCKER_COMPOSE_COMMAND up -d --force-recreate --build bot
dc_bot_force_recreate_and_logs:
	make dc_bot_force_recreate
	make dc_bot_logs_full
dc_bot_restart:
	$$DOCKER_COMPOSE_COMMAND restart bot
dc_bot_restart_and_logs:
	make dc_bot_restart
	make dc_bot_logs
dc_bot_stop:
	$$DOCKER_COMPOSE_COMMAND stop bot
dc_bot_remove:
	$$DOCKER_COMPOSE_COMMAND rm -sfv bot
dc_bot_exec:
	$$DOCKER_COMPOSE_COMMAND exec bot sh


dc_web_server_logs:
	$$DOCKER_COMPOSE_COMMAND logs --tail 100 -f web_server
dc_web_server_logs0:
	$$DOCKER_COMPOSE_COMMAND logs --tail 0 -f web_server
dc_web_server_logs_full:
	$$DOCKER_COMPOSE_COMMAND logs -f web_server
dc_web_server_run:
	$$DOCKER_COMPOSE_COMMAND up -d web_server
dc_web_server_run_and_logs:
	make dc_web_server_run
	make dc_web_server_logs_full
dc_web_server_force_recreate:
	$$DOCKER_COMPOSE_COMMAND up -d --force-recreate web_server
dc_web_server_force_recreate_and_logs:
	make dc_web_server_force_recreate
	make dc_web_server_logs_full
dc_web_server_restart:
	$$DOCKER_COMPOSE_COMMAND restart web_server
dc_web_server_restart_and_logs:
	make dc_web_server_restart
	make dc_web_server_logs
dc_web_server_stop:
	$$DOCKER_COMPOSE_COMMAND stop web_server
dc_web_server_remove:
	$$DOCKER_COMPOSE_COMMAND rm -sfv web_server
dc_web_server_exec:
	$$DOCKER_COMPOSE_COMMAND exec web_server
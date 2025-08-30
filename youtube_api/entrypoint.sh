export VENV="/venv/venv"
export REQ_FILE="/app/requirements.txt"

if [[ ! -d "$VENV" ]]; then
    echo "Creating venv"
    python3 -m venv "$VENV"
    echo "Install requirements"
    "$VENV/bin/pip" install -r "$REQ_FILE"
fi

$VENV/bin/python3 /app/main.py
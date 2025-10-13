if [[ ! -d /volumes/venv/venv ]]; then
    echo "Creating venv"
    python3 -m venv /volumes/venv/venv
    echo "Install requirements"
    /volumes/venv/venv/bin/pip install -r /app/requirements.txt
fi

echo 'freeze'
/volumes/venv/venv/bin/pip freeze
/volumes/venv/venv/bin/python3 /app/main.py
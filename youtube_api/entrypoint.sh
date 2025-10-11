if [[ ! -d /venv/venv ]]; then
    echo "Creating venv"
    python3 -m venv /venv/venv
    echo "Install requirements"
    /venv/venv/bin/pip install -r /app/requirements.txt
fi

echo 'freeze'
/venv/venv/bin/pip freeze
# /bin/python3 /app/main.py
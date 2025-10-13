if [[ ! -d ./venv ]]; then
    echo "Creating venv"
    python3 -m venv ./venv
    echo "Install requirements"
    ./venv/bin/pip install -r ./app/requirements.txt
fi

echo 'freeze'
./venv/bin/pip freeze
./venv/bin/python3 ./app/main.py
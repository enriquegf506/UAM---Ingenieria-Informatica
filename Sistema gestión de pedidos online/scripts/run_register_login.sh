#!/bin/bash

# 🧪 Script to simulate a basic warehouse operation with one client
# 📝 Usage: ./run_basic.sh

# Configuration
VENV_PATH="../venv"
PYTHON_SCRIPTS_DIR=".."
RABBITMQ_HOST="localhost"
CONTROLLER_PID=""
ROBOT_PID=""
REPARTIDOR_PID=""
CLIENT_PID=""
CHECK_INTERVAL=10  # Seconds between order status checks
TIMEOUT=300       # Max seconds to wait for orders to complete (5 minutes)
USERNAME="daniel"
PASSWORD="daniel1234"

# Ensure RabbitMQ is running
echo "Checking if RabbitMQ is running..."
if ! sudo rabbitmqctl status > /dev/null 2>&1; then
    echo "Error: RabbitMQ server is not running. Please start it first."
    exit 1
fi
echo "RabbitMQ is running."

# Function to clean up processes
cleanup() {
    echo "Cleaning up processes..."
    for pid in $CONTROLLER_PID $ROBOT_PID $REPARTIDOR_PID $CLIENT_PID; do
        if [ -n "$pid" ] && ps -p "$pid" > /dev/null; then
            kill "$pid" 2>/dev/null
            wait "$pid" 2>/dev/null
        fi
    done
    echo "All processes terminated."
}

# Trap interrupts and exit to ensure cleanup
trap cleanup INT TERM EXIT

echo "Starting basic operation scenario..."

# Start controller
echo "Launching launch_controler.py..."
python3 "$PYTHON_SCRIPTS_DIR/launch_controler.py" &
CONTROLLER_PID=$!
sleep 2
if ! ps -p "$CONTROLLER_PID" > /dev/null; then
    echo "Error: Failed to start launch_controler.py."
    exit 1
fi
echo "Controller running. PID: $CONTROLLER_PID"

# Start client
echo "Launching regiter_login.py with user $USERNAME..."
python3 "$PYTHON_SCRIPTS_DIR/register_login.py" &
CLIENT_PID=$!
sleep 2
if ! ps -p "$CLIENT_PID" > /dev/null; then
    echo "Client running and terminated. PID: $CLIENT_PID"
else
    echo "Client running. PID: $CLIENT_PID"
fi

# Wait for client to finish
echo "Waiting for client to complete..."
wait "$CLIENT_PID" 2>/dev/null
CLIENT_PID=""

echo "Basic Register and Login scenario completed."
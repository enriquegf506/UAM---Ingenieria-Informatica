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

# Start robot
echo "Launching launch_robot.py..."
python3 "$PYTHON_SCRIPTS_DIR/launch_robot.py" &
ROBOT_PID=$!
sleep 2
if ! ps -p "$ROBOT_PID" > /dev/null; then
    echo "Error: Failed to start launch_robot.py."
    exit 1
fi
echo "Robot running. PID: $ROBOT_PID"

# Start repartidor
echo "Launching launch_repartidor.py..."
python3 "$PYTHON_SCRIPTS_DIR/launch_repartidor.py" &
REPARTIDOR_PID=$!
sleep 2
if ! ps -p "$REPARTIDOR_PID" > /dev/null; then
    echo "Error: Failed to start launch_repartidor.py."
    exit 1
fi
echo "Repartidor running. PID: $REPARTIDOR_PID"

# Start client
echo "Launching proof_delivery.py with user $USERNAME..."
python3 "$PYTHON_SCRIPTS_DIR/proof_delivery.py" &
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

# Verify all orders are delivered or canceled
echo "Verifying all orders are processed..."
start_time=$(date +%s)
while true; do
    python3 "check_orders.py" "$USERNAME"
    result=$?

    if [ $result -eq 0 ]; then
        echo "All orders have been delivered or canceled."
        break
    fi

    current_time=$(date +%s)
    elapsed=$((current_time - start_time))
    if [ $elapsed -ge $TIMEOUT ]; then
        echo "Timeout: Not all orders were processed within $TIMEOUT seconds."
        break
    fi

    echo "Orders still processing. Checking again in $CHECK_INTERVAL seconds (elapsed: $elapsed seconds)..."
    sleep $CHECK_INTERVAL
done

echo "Basic operation scenario completed."

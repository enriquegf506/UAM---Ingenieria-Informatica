#!/bin/bash

# 🧪 Script to simulate multiple clients placing orders concurrently
# 🔄 Terminates when all orders are delivered or canceled
# 📝 Usage: ./run_multiple_clients.sh

# Configuration
VENV_PATH="../venv"
PYTHON_SCRIPTS_DIR=".."
LOG_DIR="./logs/multiple_clients_$(date +%Y%m%d_%H%M%S)"
RABBITMQ_HOST="localhost"
NUM_CLIENTS=3
CHECK_INTERVAL=10  # Seconds between order status checks
TIMEOUT=300        # Max seconds to wait for orders to complete (5 minutes)
CONTROLLER_PID=""
ROBOT_PID=""
REPARTIDOR_PID=""
CLIENT_PIDS=""

# Ensure RabbitMQ is running
echo "Checking if RabbitMQ is running..."
if ! sudo rabbitmqctl status > /dev/null 2>&1; then
    echo "Error: RabbitMQ server is not running. Please start it first."
    exit 1
fi
echo "RabbitMQ is running."

# Create log directory
mkdir -p "$LOG_DIR"

# Activate virtual environment
echo "Activating virtual environment..."
source "$VENV_PATH/bin/activate"

# Function to clean up processes
cleanup() {
    echo "Cleaning up processes..."
    for pid in $CONTROLLER_PID $ROBOT_PID $REPARTIDOR_PID $CLIENT_PIDS; do
        if [ -n "$pid" ] && ps -p "$pid" > /dev/null; then
            kill "$pid" 2>/dev/null
            wait "$pid" 2>/dev/null
        fi
    done
    echo "All processes terminated."
}

# Trap interrupts and exit to ensure cleanup
trap cleanup INT TERM EXIT

echo "Starting multiple clients scenario..."

# Start controller
echo "Launching launch_controler.py..."
python3 "$PYTHON_SCRIPTS_DIR/launch_controler.py" > "$LOG_DIR/controller.log" 2>&1 &
CONTROLLER_PID=$!
sleep 2
if ! ps -p "$CONTROLLER_PID" > /dev/null; then
    echo "Error: Failed to start launch_controler.py. Check $LOG_DIR/controller.log"
    exit 1
fi
echo "Controller running. PID: $CONTROLLER_PID"

# Start robot
echo "Launching launch_robot.py..."
python3 "$PYTHON_SCRIPTS_DIR/launch_robot.py" > "$LOG_DIR/robots.log" 2>&1 &
ROBOT_PID=$!
sleep 2
if ! ps -p "$ROBOT_PID" > /dev/null; then
    echo "Error: Failed to start launch_robot.py. Check $LOG_DIR/robots.log"
    exit 1
fi
echo "Robot running. PID: $ROBOT_PID"

# Start repartidor
echo "Launching launch_repartidor.py..."
python3 "$PYTHON_SCRIPTS_DIR/launch_repartidor.py" > "$LOG_DIR/repartidores.log" 2>&1 &
REPARTIDOR_PID=$!
sleep 2
if ! ps -p "$REPARTIDOR_PID" > /dev/null; then
    echo "Error: Failed to start launch_repartidor.py. Check $LOG_DIR/repartidores.log"
    exit 1
fi
echo "Repartidor running. PID: $REPARTIDOR_PID"

# Start multiple clients
echo "Launching $NUM_CLIENTS clients with proof_delivery.py..."
for i in $(seq 1 $NUM_CLIENTS); do
    python3 "$PYTHON_SCRIPTS_DIR/proof_delivery.py" > "$LOG_DIR/client_$i.log" 2>&1 &
    pid=$!
    CLIENT_PIDS="$CLIENT_PIDS $pid"
    sleep 1
    
done
echo "Clients launched."

# Wait for all clients to finish
echo "Waiting for all clients to complete..."
for pid in $CLIENT_PIDS; do
    if ps -p "$pid" > /dev/null; then
        wait "$pid"
    fi
done
CLIENT_PIDS=""

# Verify all orders are delivered or canceled
echo "Verifying all orders are processed..."
start_time=$(date +%s)
while true; do
    python3 "check_orders.py" daniel >> "$LOG_DIR/check_orders.log" 2>&1
    if [ $? -eq 0 ]; then
        echo "All orders have been delivered or canceled."
        break
    fi

    current_time=$(date +%s)
    elapsed=$((current_time - start_time))
    if [ $elapsed -ge $TIMEOUT ]; then
        echo "Timeout: Not all orders were processed within $TIMEOUT seconds. Check logs for details."
        cat "$LOG_DIR/check_orders.log"
        break
    fi

    echo "Orders still processing. Checking again in $CHECK_INTERVAL seconds (elapsed: $elapsed seconds)..."
    sleep $CHECK_INTERVAL
done

# Final status check
echo "Final order status:"
tail -n 10 "$LOG_DIR/check_orders.log"

echo "Multiple clients scenario completed. Logs are in $LOG_DIR"

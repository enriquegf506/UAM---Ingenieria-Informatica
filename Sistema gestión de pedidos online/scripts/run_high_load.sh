#!/bin/bash

# Script to simulate a high-load scenario with many robots, repartidores, and clients
# Usage: ./run_high_load.sh

# Configuration
VENV_PATH="../venv"
PYTHON_SCRIPTS_DIR=".."
LOG_DIR="./logs/high_load_$(date +%Y%m%d_%H%M%S)"
RABBITMQ_HOST="localhost"
NUM_ROBOTS=5
NUM_REPARTIDORES=3
NUM_CLIENTS=5
CHECK_INTERVAL=10  # seconds between checks
TIMEOUT=600        # max time to wait (10 minutes)
CONTROLLER_PID=""
ROBOT_PIDS=""
REPARTIDOR_PIDS=""
CLIENT_PIDS=""

# Ensure RabbitMQ is running
if ! sudo rabbitmqctl status > /dev/null 2>&1; then
    echo "Error: RabbitMQ server is not running. Please start it first."
    exit 1
fi

# Create log directory
mkdir -p "$LOG_DIR"

# Activate virtual environment
source "$VENV_PATH/bin/activate"

# Function to clean up processes
cleanup() {
    echo "Cleaning up processes..."
    for pid in $CONTROLLER_PID $ROBOT_PIDS $REPARTIDOR_PIDS $CLIENT_PIDS; do
        if [ -n "$pid" ] && ps -p "$pid" > /dev/null; then
            kill "$pid" 2>/dev/null
            wait "$pid" 2>/dev/null
        fi
    done
    echo "All processes terminated."
}

# Trap interrupts and exit to ensure cleanup
trap cleanup INT TERM EXIT

echo "Starting high-load scenario..."

# Start controller
echo "Launching launch_controler.py..."
python3 "$PYTHON_SCRIPTS_DIR/launch_controler.py" > "$LOG_DIR/controler.log" 2>&1 &
CONTROLLER_PID=$!
sleep 2
if ! ps -p "$CONTROLLER_PID" > /dev/null; then
    echo "Error: Failed to start launch_controler.py. Check $LOG_DIR/controler.log"
    exit 1
fi

# Start multiple robot instances
echo "Launching $NUM_ROBOTS robot instances..."
for i in $(seq 1 $NUM_ROBOTS); do
    python3 "$PYTHON_SCRIPTS_DIR/launch_robot.py" > "$LOG_DIR/robots_$i.log" 2>&1 &
    pid=$!
    ROBOT_PIDS="$ROBOT_PIDS $pid"
    sleep 1
    if ! ps -p "$pid" > /dev/null; then
        echo "Error: Failed to start robot instance $i. Check $LOG_DIR/robots_$i.log"
        exit 1
    fi
done

# Start multiple repartidor instances
echo "Launching $NUM_REPARTIDORES repartidor instances..."
for i in $(seq 1 $NUM_REPARTIDORES); do
    python3 "$PYTHON_SCRIPTS_DIR/launch_repartidor.py" > "$LOG_DIR/repartidores_$i.log" 2>&1 &
    pid=$!
    REPARTIDOR_PIDS="$REPARTIDOR_PIDS $pid"
    sleep 1
    if ! ps -p "$pid" > /dev/null; then
        echo "Error: Failed to start repartidor instance $i. Check $LOG_DIR/repartidores_$i.log"
        exit 1
    fi
done

# Start multiple clients
echo "Launching $NUM_CLIENTS clients..."
for i in $(seq 1 $NUM_CLIENTS); do
    python3 "$PYTHON_SCRIPTS_DIR/proof_delivery.py" > "$LOG_DIR/client_$i.log" 2>&1 &
    pid=$!
    CLIENT_PIDS="$CLIENT_PIDS $pid"
    echo "Client $i launched with PID $pid"
done


# Wait for all clients to finish
echo "Waiting for all clients to complete..."
for pid in $CLIENT_PIDS; do
    wait "$pid"
done
CLIENT_PIDS=""

# Periodically check if all orders are completed
echo "Verifying when all orders are delivered or canceled..."
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
        echo "Timeout: Not all orders completed in $TIMEOUT seconds. See logs for details."
        cat "$LOG_DIR/check_orders.log"
        break
    fi

    echo "Orders still being processed... checking again in $CHECK_INTERVAL seconds (elapsed: $elapsed seconds)."
    sleep $CHECK_INTERVAL
done


# Final cleanup and status
cleanup
echo "Final order status (last 10 lines):"
tail -n 10 "$LOG_DIR/check_orders.log"

echo "High-load scenario completed. Logs are in $LOG_DIR"
exit 0

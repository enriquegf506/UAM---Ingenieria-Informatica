#!/bin/bash

# 💥 Script to simulate a failure scenario (e.g. low resources causing stuck orders)
# Usage: ./run_failure.sh

VENV_PATH="../venv"
PYTHON_SCRIPTS_DIR=".."
LOG_DIR="./logs/failure_scenario_$(date +%Y%m%d_%H%M%S)"
RABBITMQ_HOST="localhost"
NUM_ROBOTS=1
NUM_REPARTIDORES=0
NUM_CLIENTS=3
CONTROLLER_PID=""
ROBOT_PIDS=""
REPARTIDOR_PIDS=""
CLIENT_PIDS=""

echo "Verifying RabbitMQ is running..."
if ! sudo rabbitmqctl status > /dev/null 2>&1; then
    echo "Error: RabbitMQ server is not running."
    exit 1
fi
echo "RabbitMQ is running."

mkdir -p "$LOG_DIR"
source "$VENV_PATH/bin/activate"

cleanup() {
    echo "Cleaning up processes..."
    for pid in $CONTROLLER_PID $ROBOT_PIDS $REPARTIDOR_PIDS $CLIENT_PIDS; do
        if [ -n "$pid" ] && ps -p "$pid" > /dev/null; then
            kill "$pid"
            wait "$pid" 2>/dev/null
        fi
    done
    echo "All processes terminated."
}

trap cleanup INT TERM EXIT

echo "Starting FAILURE scenario..."

# Controller
echo "Launching controller..."
python3 "$PYTHON_SCRIPTS_DIR/launch_controler.py" > "$LOG_DIR/controler.log" 2>&1 &
CONTROLLER_PID=$!
sleep 2
if ! ps -p "$CONTROLLER_PID" > /dev/null; then
    echo "Error: Failed to start controller. Check $LOG_DIR/controler.log"
    exit 1
fi
echo "Controller launched. PID: $CONTROLLER_PID"

# One robot
echo "Launching 1 robot..."
python3 "$PYTHON_SCRIPTS_DIR/launch_robot.py" > "$LOG_DIR/robot_1.log" 2>&1 &
pid=$!
ROBOT_PIDS="$ROBOT_PIDS $pid"
echo "Robot launched. PID: $pid"

# No repartidores
echo "Not launching any repartidores (simulate delivery bottleneck)..."

# Clients
echo "Launching $NUM_CLIENTS clients..."
for i in $(seq 1 $NUM_CLIENTS); do
    python3 "$PYTHON_SCRIPTS_DIR/proof_delivery.py" > "$LOG_DIR/client_$i.log" 2>&1 &
    pid=$!
    CLIENT_PIDS="$CLIENT_PIDS $pid"
    echo "Client $i launched with PID $pid"
done

# Wait for clients to finish
echo "Waiting for clients to finish..."
for pid in $CLIENT_PIDS; do
    wait "$pid"
done
echo "All clients finished."

# Check for stuck/canceled orders
echo "Checking for unprocessed orders..."
python3 "check_orders.py" daniel --show-all > "$LOG_DIR/orders_check.log"
if [ $? -ne 0 ]; then
    echo "Some orders are still pending. Check $LOG_DIR/orders_check.log"
else
    echo "All orders are finalized or canceled."
fi

echo "FAILURE scenario complete. Logs saved in: $LOG_DIR"

#!/bin/bash

# Set PYTHONPATH to include the src/ directory
export PYTHONPATH=$PYTHONPATH:$(pwd)/src

# Check if simulator is already running
if [ -f /tmp/simulator.lock ]; then
    echo "Simulator.sh is already running. Exiting."
    exit 1
fi
touch /tmp/simulator.lock

# Array to store PIDs of background processes
declare -a PIDS=()

# Function to clean up background processes
cleanup() {
    echo "Terminating all background processes..."
    # Kill all recorded PIDs
    for pid in "${PIDS[@]}"; do
        if kill -0 "$pid" 2>/dev/null; then
            kill -TERM "$pid"
            echo "Terminated process $pid"
        fi
    done
    # Remove lock file
    rm -f /tmp/simulator.lock
    exit 0
}

# Trap SIGINT (Ctrl+C) and call cleanup
trap cleanup SIGINT

# Kill any existing processes using ports 5000 (bridge) and 8081 (controller)
lsof -i :5000 | grep LISTEN | awk '{print $2}' | xargs -r kill -9
lsof -i :8081 | grep LISTEN | awk '{print $2}' | xargs -r kill -9

# Kill any running device processes
pkill -f dummy-clock.py
pkill -f dummy-sensor.py
pkill -f dummy-switch.py

# Clear retained MQTT messages to avoid stale data
mosquitto_pub -h localhost -p 1883 -t "redes2/2321/6/1" -n -r
mosquitto_pub -h localhost -p 1883 -t "redes2/2321/6/1/set" -n -r
mosquitto_pub -h localhost -p 1883 -t "redes2/2321/6/2" -n -r
mosquitto_pub -h localhost -p 1883 -t "redes2/2321/6/2/set" -n -r
mosquitto_pub -h localhost -p 1883 -t "redes2/2321/6/3" -n -r
mosquitto_pub -h localhost -p 1883 -t "redes2/2321/6/3/set" -n -r

# Start bridge (Discord bot)
echo "Starting bridge..."
python3 src/bridge.py &
PIDS+=($!)
# Wait 5 seconds for the Discord bot to log in
sleep 5

# Start controller with a 3-second delay
echo "Starting controller..."
sleep 3
python3 src/controller.py &
PIDS+=($!)

# Start devices: clock 1, switch 2, sensor 3
echo "Starting devices..."
python3 src/devices/dummy_clock.py 1 --host localhost --port 1883 --time 09:00:00 --increment 60 --rate 12 &
PIDS+=($!)
python3 src/devices/dummy_switch.py 2 --host localhost --port 1883 --probability 0.3 &
PIDS+=($!)
python3 src/devices/dummy_sensor.py 3 --host localhost --port 1883 --interval 5 --min 20 --max 30 --increment 1 &
PIDS+=($!)

# Wait for all background processes
wait
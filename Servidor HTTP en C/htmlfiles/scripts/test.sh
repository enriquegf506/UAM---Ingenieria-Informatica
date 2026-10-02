#!/bin/bash


for i in $(seq 0 1000); do
	curl -X GET "http://localhost:8080/index.html" &
done; wait


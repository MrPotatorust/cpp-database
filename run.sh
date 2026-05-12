#!/bin/bash

# Compile both server and client
make compile

# Start server in a new terminal
gnome-terminal -- bash -c "./server; bash" &

# Start client in another new terminal
gnome-terminal -- bash -c "./client; bash" &

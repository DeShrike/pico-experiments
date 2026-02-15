#!/bin/bash

docker run -d --rm -it --name picosdk --mount type=bind,source=${PWD},target=/home/dev lukstep/raspberry-pi-pico-sdk:latest


#!/bin/bash

{ sudo airodump-ng mon0 -w primera_captura --output-format csv 2>> output.txt; } &

sleep 30

rm -rf output.txt

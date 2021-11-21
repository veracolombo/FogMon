#!/bin/sh

tc qdisc add dev eth0 root tbf rate 1mbit burst 32kbit latency 400ms

#/FogMon
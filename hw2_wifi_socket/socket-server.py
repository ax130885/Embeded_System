#!/usr/bin/env python3
import socket
import numpy as np
import json
import time
import random

HOST = '192.168.50.107' # IP address
PORT = 61234 # Port to listen on (use ports > 1023)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    conn, addr = s.accept()
    with conn:
        print('Connected by', addr)
        while True:
            print("server wait data.\r\n")
            data = conn.recv(1024).decode('utf-8')
            print('Received from socket server : ', data)
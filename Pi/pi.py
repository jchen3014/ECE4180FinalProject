import socket
import tkinter as tk
import threading
import RPi.GPIO as GPIO
from mfrc522 import SimpleMFRC522
import time
import sys
import serial

GPIO.setmode(GPIO.BOARD)
pin_num = 18
GPIO.setup(pin_num, GPIO.OUT)
GPIO.setwarnings(False)
reader = SimpleMFRC522()

# Constants
LOG_SIZE = 6
HOST = '0.0.0.0'  # Listen on all available network interfaces
PORT = 12345

# Create the TCP server socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((HOST, PORT))
server_socket.listen(1)

ser = serial.Serial('/dev/ttyS0', 9600, timeout=1)

def listen_for_mbed():
    while True:
        # Listen for incoming data
        if ser.in_waiting > 0:
            data = ser.readline().decode()
            print(f'Received: {data}')
        text = "placeholder"
        id, text = reader.read()
        text=text.strip()
        if text == "hi":
            ser.write("1\n".encode())
            update_log_display("UNLOCKED USING BLUETOOTH!")
        else:
            print("u no allowed")
            ser.write("intruder alert\n".encode())
            update_log_display("BLUETOOTH DENIED!")
        time.sleep(5)


        text_to_send = input()+'\n'
        ser.write(text_to_send.encode())  
        #ser.write(text_to_send.encode()
        time.sleep(5)

input_thread2 = threading.Thread(target=listen_for_mbed, daemon=True)
input_thread2.start()

def listen_for_input():
    while True:
        # Listen for incoming data
        if ser.in_waiting > 0:
            data = ser.readline().decode()
            print(f'Received: {data}')
        text = "placeholder"
        id, text = reader.read()
        text=text.strip()
        if text == "hi":
            ser.write("1\n".encode())
            update_log_display("UNLOCKED USING KEY!")
        else:
            print("u no allowed")
            ser.write("intruder alert\n".encode())
            update_log_display("KEY DENIED!")
        time.sleep(5)

input_thread = threading.Thread(target=listen_for_input, daemon=True)
input_thread.start()

def on_closing():
    sock.close()
    root.destroy()
    ser.close()
    
# Define the method to update the log display
def update_log_display(log_str):
    # Insert the new log message at the top of the list
    log_list.insert(0, log_str)
    # If the log is too long, remove the oldest log message
    if log_list.size() > LOG_SIZE:
        log_list.delete(LOG_SIZE)

# Define the method to handle new client connections
def handle_client(conn, addr):
    with conn:
        print(f'New client connected: {addr}')
        while True:
            data = conn.recv(1024)
            if not data:
                print(f'Client {addr} disconnected')
                break
            else:
                log_str = f'Received from {addr}: {data.decode()}'
                print(log_str)
                update_log_display("Unlocked From Laptop!")

# Define the method to start the server loop
def start_server_loop():
    while True:
        print('Waiting for incoming connections...')
        conn, addr = server_socket.accept()
        print(f'Accepted connection from {addr}')

        #with conn:
        print(f'New client connected: {addr}')
        while True:
            data = conn.recv(1024)
            if not data:
                print(f'Client {addr} disconnected')
                break
            else:
                #log_str = f'Received from {addr}: {data.decode()}'
                #print(log_str)
                update_log_display("UNLOCKED FROM LAPTOP!")

# Set constants
WINDOW_WIDTH = 800
WINDOW_HEIGHT = 600

# Create the GUI
root = tk.Tk()
root.title('TCP Server Log Viewer')
# Set the size of the window
root.geometry('{}x{}'.format(WINDOW_WIDTH, WINDOW_HEIGHT))

# Create the log display
log_list = tk.Listbox(root, height=LOG_SIZE, font=('Arial', 12))
log_list.pack(expand=True, fill='both', padx=20, pady=20)
# Create the log display
#log_list = tk.Listbox(root, height=LOG_SIZE)
#log_list.pack()

# Start the server loop in a separate thread
server_thread = threading.Thread(target=start_server_loop)
server_thread.start()

# Start the GUI main loop
try:
    root.mainloop()
except KeyboardInterrupt:
    print('CTL C')
    on_closing()
# Close the server socket
server_socket.close()

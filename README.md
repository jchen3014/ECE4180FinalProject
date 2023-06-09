# Team Members
* Connor Talley (Section B)
* Jingsong Guo (Section B)
* Addison Elliott (Section A)
* Jackie Chen (Section A)

<!-- ABOUT THE PROJECT -->
# About The Project: Smart Lock Module

![project pic](/images/project.jpeg)


The Smart lock module is a solenoid-based lock that can be controlled by a variety of sources and unlock attempts are displayed on the screen.

Main components of the module:
* Mbed directly controls solenoid driver circuit and two authentication methods
    * Password protected bluetooth connection that controls the lock
    * Keycode-based entry controlled by touch keypad via I2C 
* Raspberry Pi communicates with mbed for additional peripheral control of the lock
    * Recieves TCP socket unlock commands
    * Manages a RFID reader and records user database
    * Keeps a local log file of all recorded unlock methods and user IDs and displays on GUI
* Solenoid-based lock can be activated for 5 seconds before returning to locked state
* Mbed and Rasperry Pi communicates via Serial UART




### Built With

* mbed LPC1768​
* Raspberry Pi (Zero W)​
* MFRC-522 RFID Reader​
* Bluefruit LE UART Friend​
* MPR121 Capacitive Touch Keypad​
* Adafruit 9V-12V Lock-Style Solenoid​
* 5V Relay​
* BJT​
* Diode​
* 1k Resistor​
* 10k Resistor​
* 2x 4k7 Resistor​
* 9V Power Supply​
* 5V Power Supply​
* Connecting Wires​
* Raspberry Pi Monior 10.1"

<!-- GETTING STARTED -->
## Getting Started
### Preparing Mbed

1. Open Keil Studio, create a project, and load the code files in the mbed folder.
2. Connect all peripherals according to the wiring diagram.
3. Connect the additional power sources for the solenoid and bluetooth.
4. Make sure the mbed is sharing the same ground with Raspberry Pi.

### Preparing Raspberry Pi

1. We first have to make changes to the Raspberry Pi configurations. By default, the SPI (Serial Peripheral Interface) is disabled, so open the raspi-config tool, using the following command.
```
sudo raspi-config
```
2. Use keyboard arrow keys to select **“5 Interfacing Options“** and press press Enter.
3. Use keyboard arrow keys to select **“P4 SPI“** and press Enter.
4. Select Yes with your arrow keys to enable the SPI Interface and press Enter.
5. Restart the Pi by getting back to the terminal by pressing Enter, then ESC, and the following command.
```
sudo reboot
```
6. Once rebooted, check if spi_bcm2835 is listed when following command is run.
```
lsmod | grep spi
```
  
### Installing Python and Libraries on Pi

1. Check if Pi is up to date.
```
sudo apt update
sudo apt upgrade
```
2. Install all required packages for RFID reader.
```
sudo apt install python3-dev python3-pip
```
3. Install the spidev Python library that handles interactions with the SPI. Note that sudo is used to ensure the package is installed so that all users can utilize it and not just the current user.
```
sudo pip3 install spidev
```
4. Install the MFRC522 library.
```
sudo pip3 install mfrc522
```

### Running code on Pi
1. Move the write.py and unlock.py files into the Pi's directory.
2. Move the GUI.py file to your computer's directory.
3. To write to an RFID fob or card, open terminal on the Pi and run the following.
```
sudo python3 write.py
```
4. Enter what you want to input onto the RFID device and place the RFID device next to the scanner until the data finsihed writing.
5. Now, run the following command on the Pi to set up the TCP server.
```
sudo python3 unlock.py
```
6. Run the following command on your computer to set up the TCP client.
```
sudo python3 GUI.py
```
7. A window on the Pi monitor will display the most recent unlock attempts. You can now attempt to unlock the lock using the various methods mentioned.


## Connections and Wiring
<img src="images/block_diagram.png" width="600">
<img src="images/wiring_diagram.png" width="600">

## Demonstration
[![Alt text](https://img.youtube.com/vi/8DHuXTUlJBs/0.jpg)](https://www.youtube.com/watch?v=8DHuXTUlJBs)
<!-- USAGE EXAMPLES -->
## Usage

The project can be integrated for indoor environments for additional door-locking controls. The keypad and the RFID can be part of the lock hardware on the door, which will control the lock via physical contact. TCP and bluetooth provides remote control capabilities from within the same house or in remote locations. The embedded control hardware can be stationed in the room behind the locked door, where the recorded log file can be easily accessed.

## Future Upgrades

Additional support hardware can be added, such as a camera system that takes a picture of the person accessing the lock via physical contact.

Additional security encryption of the user ID, passcodes, and TCP connection can be added for expanded outdoor and commercial usage.

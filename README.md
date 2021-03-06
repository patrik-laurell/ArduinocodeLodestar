# Lodestar BEXUS

The code used in the [BEXUS](http://rexusbexus.net/bexus/) 27 [Lodestar](http://rexusbexus.net/experiments/scientific-research/materials-science/). For intrest in results of the project read [this](http://rexusbexus.net/wp-content/uploads/2019/08/A-184laurell.pdf).

## Ground Station

The ground station control panel for Lodestar BEXUS consists of a Python backend and a HTML/JavaScript frontend. The back- and frontend communicate using SocketIO.

### Python backend
The backend has only been tested using Python 3.6. Using this version of Python is suggested. By default the backend listens for UDP packages on port 8888. To function with the Arduino code the ground station needs to be assigned the IP-adress 192.168.0.3. 

#### Setup
1. Make sure the ground station computer is running Linux or OSX with Python 3 installed along with Pip for Python 3. Running Ubuntu 18.04 you will have Python 3.6 installed by default. To install Pip run ``sudo apt-get install python3-pip``.
2. Using pip install the package ``Flask-SocketIO`` by running ``sudo pip3 install Flask-SocketIO``
3. Clone this Git repo.

#### Run
To run the ground station navigate to the directory ``groundstation/webapp`` and run one of the following commands. Then navigate to ``localhost:5000`` in your web browser.

To run in live monitor mode, listening for incomming UDP packages on port 8888 run:
```
$ python3 ground_station.py
```
To run in file inspection mode to inspect .lodestar datafiles run:
```
$ python3 ground_station.py -f <path-to-file>
```
To export data from a .lodestar file to a .mat file run:
```
$ python3 ground_station.py -e <path-to-file>
```

## On board software
The on board software consists of two programs, one running on an Arduino Mega and one running on two separate Arduino Nanos. To load the on board software onto the Arduino Mega and the two Arduino Nano the required libraries first needs to be added to arduinosketches/libraries/. When loading the nanocode.ino onto the Arduino Nanos it is important to edit the I2C address in nanocode.h so that they don't share the same address. It is also important to take note of which address is given to which Nano. The Nano that is given NANO_ADDRESS_1 as its I2C address will lable the cigs data with number 1-3 and the one loaded with NANO_ADDRESS_2 will lable its data with number 4-6.

The micro-SD card should be an 2 GB formated as FAT16.

### Required libraries
* Afafruit Sensor
* Adafruit BMP280
* MemoryFree

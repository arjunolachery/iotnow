# iotnow

iotnow - Software Stack for Next Generation IoT Devices (www.iotnow.co.in)

Codes unless mentioned otherwise are written for the NodeMCU board


## Usage

```bash
1) Import iotnow-library
2) Create programs using the iotnow-template
``` 

## iotnow-programs directory

The binary file and the ino file of the corresponding program is found in this directory. Each program is associated by its program number and version number as programNumber_versionNumber

2_2 corresponds to the temperature and humidity logger program built with iotnow

4_1 and 4_2 are demo programs to showcase updating and downgrading of programs

## previousCodes directory

The initial codes are found in this directory which have now been depreciated

5.1 - Code for Temperature Logger with OTA

5.2: User Interface – Core Code

5.3: Code for Temperature Logger with OTA and UI

5.4: Code for Device Logger with OTA and UI

*OTA - Over-The-Air Updates

*UI - User Interface (Bluetooth Terminal)

## web directory

The codes for the web interface are found here

## Installation for iotnow-surveillance-app

A home surveillance application used on the Raspberry Pi with the Raspberry Pi Camera

```bash
sudo apt-get update
sudo apt-get upgrade
sudo pip3 install virtualenv
python3 -m venv iotnow-env
source iotnow-env/bin/activate
bash install_pi_requirements.sh
python3 surveillance.py
```

## To activate the virtual environment

```python
cd iotnow-surveillance-app
source iotnow-env/bin/activate
python3 surveillance.py
```


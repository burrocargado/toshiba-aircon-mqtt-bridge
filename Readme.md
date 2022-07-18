# toshiba-aircon-mqtt-bridge 
Monitor and control Toshiba industrial air conditioner via MQTT broker by connecting ESP32 MCU (M5Stamp Pico) to the wired remote controller (RBC-AMT31). This project can also be applied to central air conditioner for home use with wired remote controller, which is equipped with the same pc-board. Other types of wired remote controller might be able to use with this project with little circuit modifications (not confirmed).

Functions of the firmware are as follows:
- Publish control and reply packets received from the AB-wire to the mqtt topic 'aircon/packet/rx'.
- Subscribe to the mqtt topic 'aircon/packet/tx' and transmit packets received from the topic to the AB-wire.

## Hardware

### Partial schematic diagram of the wired remote controller (RBC-AMT31)
![RBC-AMT31 partial schematic](schematic/KiCad/remote_controller-Toshiba%20Remote%20Controller.png "RBC-AMT31 partial schematic")

### Schematic diagram of the wireless controller
![Wireless bridge schematic](schematic/KiCad/wireless_controller-Wireless%20Controller.png "Wireless bridge schematic")
Plug a shorting jumper to 2-3 of J2 to supply ESP32 MCU from AB-wire in normal use. Current consumption from the AB-wire is less than 20mA in normal operation, maximum current is about 50mA when powering up,  charging EDLC (C2). When flashing the firmware to ESP32, move the shorting jumper to 1-4 of J2.

### Connection of the remote controller and the wireless controller
|remote controller|wireless controller|
|:----:|:----:|
|H_TAP|RC_H|
|GND_TAP|GND|
|TX_TAP|RC_TX|
|VCC_TAP|RC_VCC|
|RX_TAP|RC_RX|

## Building and flashing the ESP32 firmware
- Using Visual Studio Code, install PlatformIO.
- Clone this repository and copy stamp-pico.json and pins_arduino.h to [PIODIR]\platforms\espressif32\boards and [PIODIR]\packages\framework-arduinoespressif32\variants\stamp_pico respectively.  
(Thanks to the author of https://qiita.com/WhiteGrouse/items/10c36e382efd2872d267)
- Open the project directory with the VSCode.
- Rename src\credentials_example.h to credentials.h and edit it to add your credentials.
- Connect programmer to J1 and upload from the "PROJECT TASKS" menu of the PIO.

## Disclaimer
The safety and functionality of the hardware design and firmware of this project are not guaranteed at all. You could damage your facility, lose the manufacturer's warranty, or have your maintenance contract terminated. Use of this project is at your own risk.
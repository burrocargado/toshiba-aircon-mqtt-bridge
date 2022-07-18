EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr User 7874 5906
encoding utf-8
Sheet 1 3
Title "Wireless Controller Connection"
Date "2022-07-03"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 2150 1800 1300 800 
U 62D7BC2C
F0 "Toshiba Remote Controller" 50
F1 "remote_controller.sch" 50
F2 "TERMINAL_B" I L 2150 2300 50 
F3 "TERMINAL_A" I L 2150 2100 50 
F4 "TX_TAP" I R 3450 2200 50 
F5 "RX_TAP" I R 3450 2400 50 
F6 "H_TAP" I R 3450 1950 50 
F7 "VCC_TAP" I R 3450 2300 50 
F8 "GND_TAP" I R 3450 2100 50 
$EndSheet
$Sheet
S 3900 1800 1300 800 
U 62DAB02B
F0 "Wireless Controller" 50
F1 "wireless_controller.sch" 50
F2 "RC_RX" I L 3900 2400 50 
F3 "RC_TX" I L 3900 2200 50 
F4 "RC_VCC" I L 3900 2300 50 
F5 "RC_H" I L 3900 1950 50 
F6 "GND" I L 3900 2100 50 
$EndSheet
Wire Wire Line
	3900 1950 3450 1950
Wire Wire Line
	3450 2100 3900 2100
Wire Wire Line
	3450 2200 3900 2200
Wire Wire Line
	3450 2300 3900 2300
Wire Wire Line
	3450 2400 3900 2400
$EndSCHEMATC

EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	8400 2700 7500 2700
Wire Wire Line
	8900 2700 8700 2700
$Comp
L power:GND #PWR09
U 1 1 60974DF8
P 8900 2700
F 0 "#PWR09" H 8900 2450 50  0001 C CNN
F 1 "GND" H 8905 2527 50  0000 C CNN
F 2 "" H 8900 2700 50  0001 C CNN
F 3 "" H 8900 2700 50  0001 C CNN
	1    8900 2700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 60973A53
P 8550 2700
F 0 "R4" V 8343 2700 50  0000 C CNN
F 1 "10k" V 8434 2700 50  0000 C CNN
F 2 "Resistor_SMD:R_0201_0603Metric_Pad0.64x0.40mm_HandSolder" V 8480 2700 50  0001 C CNN
F 3 "~" H 8550 2700 50  0001 C CNN
	1    8550 2700
	0    1    1    0   
$EndComp
Wire Wire Line
	7500 2600 8350 2600
Wire Wire Line
	8350 2100 8350 2600
Wire Wire Line
	7500 2500 8150 2500
Wire Wire Line
	8150 2100 8150 2500
Wire Wire Line
	7950 2300 7500 2300
Wire Wire Line
	7950 2100 7950 2300
Wire Wire Line
	8350 1650 8350 1800
Wire Wire Line
	8150 1650 8150 1800
Wire Wire Line
	7950 1650 7950 1800
$Comp
L Device:R R3
U 1 1 60971B52
P 8350 1950
F 0 "R3" H 8420 1996 50  0000 L CNN
F 1 "10k" H 8420 1905 50  0000 L CNN
F 2 "Resistor_SMD:R_0201_0603Metric_Pad0.64x0.40mm_HandSolder" V 8280 1950 50  0001 C CNN
F 3 "~" H 8350 1950 50  0001 C CNN
	1    8350 1950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 60971759
P 8150 1950
F 0 "R2" H 8220 1996 50  0000 L CNN
F 1 "10k" H 8220 1905 50  0000 L CNN
F 2 "Resistor_SMD:R_0201_0603Metric_Pad0.64x0.40mm_HandSolder" V 8080 1950 50  0001 C CNN
F 3 "~" H 8150 1950 50  0001 C CNN
	1    8150 1950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 609713B2
P 7950 1950
F 0 "R1" H 8020 1996 50  0000 L CNN
F 1 "10k" H 8020 1905 50  0000 L CNN
F 2 "Resistor_SMD:R_0201_0603Metric_Pad0.64x0.40mm_HandSolder" V 7880 1950 50  0001 C CNN
F 3 "~" H 7950 1950 50  0001 C CNN
	1    7950 1950
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR08
U 1 1 60970D2B
P 8350 1650
F 0 "#PWR08" H 8350 1500 50  0001 C CNN
F 1 "+5V" H 8365 1823 50  0000 C CNN
F 2 "" H 8350 1650 50  0001 C CNN
F 3 "" H 8350 1650 50  0001 C CNN
	1    8350 1650
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR07
U 1 1 60970A64
P 8150 1650
F 0 "#PWR07" H 8150 1500 50  0001 C CNN
F 1 "+5V" H 8165 1823 50  0000 C CNN
F 2 "" H 8150 1650 50  0001 C CNN
F 3 "" H 8150 1650 50  0001 C CNN
	1    8150 1650
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR06
U 1 1 609701A7
P 7950 1650
F 0 "#PWR06" H 7950 1500 50  0001 C CNN
F 1 "+5V" H 7965 1823 50  0000 C CNN
F 2 "" H 7950 1650 50  0001 C CNN
F 3 "" H 7950 1650 50  0001 C CNN
	1    7950 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 1700 7600 2000
Connection ~ 7600 1700
Wire Wire Line
	7450 1700 7600 1700
Wire Wire Line
	7600 1650 7600 1700
Wire Wire Line
	6800 1700 7150 1700
Text GLabel 7500 2300 2    50   Input ~ 0
ALERT
Wire Wire Line
	7100 2900 7100 2950
Text GLabel 7500 2700 2    50   Input ~ 0
GND
Text GLabel 7500 2600 2    50   Input ~ 0
SDA
Text GLabel 7500 2500 2    50   Input ~ 0
SCL
$Comp
L Device:C C2
U 1 1 6096CA1B
P 7300 1700
F 0 "C2" V 7048 1700 50  0000 C CNN
F 1 "1uF" V 7139 1700 50  0000 C CNN
F 2 "Capacitor_SMD:C_0201_0603Metric_Pad0.64x0.40mm_HandSolder" H 7338 1550 50  0001 C CNN
F 3 "~" H 7300 1700 50  0001 C CNN
	1    7300 1700
	0    1    1    0   
$EndComp
Wire Wire Line
	7100 2000 7600 2000
$Comp
L power:+5V #PWR05
U 1 1 6096B5AD
P 7600 1650
F 0 "#PWR05" H 7600 1500 50  0001 C CNN
F 1 "+5V" H 7615 1823 50  0000 C CNN
F 2 "" H 7600 1650 50  0001 C CNN
F 3 "" H 7600 1650 50  0001 C CNN
	1    7600 1650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 6096ADDF
P 7100 2950
F 0 "#PWR04" H 7100 2700 50  0001 C CNN
F 1 "GND" H 7105 2777 50  0000 C CNN
F 2 "" H 7100 2950 50  0001 C CNN
F 3 "" H 7100 2950 50  0001 C CNN
	1    7100 2950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 6096A5E1
P 6800 1700
F 0 "#PWR03" H 6800 1450 50  0001 C CNN
F 1 "GND" H 6805 1527 50  0000 C CNN
F 2 "" H 6800 1700 50  0001 C CNN
F 3 "" H 6800 1700 50  0001 C CNN
	1    6800 1700
	1    0    0    -1  
$EndComp
$Comp
L Analog_ADC:ADS1115IDGS U1
U 1 1 6096941A
P 7100 2500
F 0 "U1" H 7100 3181 50  0000 C CNN
F 1 "ADS1115IDGS" H 7100 3090 50  0000 C CNN
F 2 "Package_SO:TSSOP-10_3x3mm_P0.5mm" H 7100 2000 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/ads1113.pdf" H 7050 1600 50  0001 C CNN
	1    7100 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 4450 8350 4450
Wire Wire Line
	8350 3950 8350 4450
Wire Wire Line
	7500 4350 8150 4350
Wire Wire Line
	8150 3950 8150 4350
Wire Wire Line
	7950 4150 7500 4150
Wire Wire Line
	7950 3950 7950 4150
Wire Wire Line
	8350 3500 8350 3650
Wire Wire Line
	8150 3500 8150 3650
Wire Wire Line
	7950 3500 7950 3650
$Comp
L Device:R R7
U 1 1 609EC0E0
P 8350 3800
F 0 "R7" H 8420 3846 50  0000 L CNN
F 1 "10k" H 8420 3755 50  0000 L CNN
F 2 "Resistor_SMD:R_0201_0603Metric_Pad0.64x0.40mm_HandSolder" V 8280 3800 50  0001 C CNN
F 3 "~" H 8350 3800 50  0001 C CNN
	1    8350 3800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 609EC0E6
P 8150 3800
F 0 "R6" H 8220 3846 50  0000 L CNN
F 1 "10k" H 8220 3755 50  0000 L CNN
F 2 "Resistor_SMD:R_0201_0603Metric_Pad0.64x0.40mm_HandSolder" V 8080 3800 50  0001 C CNN
F 3 "~" H 8150 3800 50  0001 C CNN
	1    8150 3800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 609EC0EC
P 7950 3800
F 0 "R5" H 8020 3846 50  0000 L CNN
F 1 "10k" H 8020 3755 50  0000 L CNN
F 2 "Resistor_SMD:R_0201_0603Metric_Pad0.64x0.40mm_HandSolder" V 7880 3800 50  0001 C CNN
F 3 "~" H 7950 3800 50  0001 C CNN
	1    7950 3800
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR017
U 1 1 609EC0F2
P 8350 3500
F 0 "#PWR017" H 8350 3350 50  0001 C CNN
F 1 "+5V" H 8365 3673 50  0000 C CNN
F 2 "" H 8350 3500 50  0001 C CNN
F 3 "" H 8350 3500 50  0001 C CNN
	1    8350 3500
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR016
U 1 1 609EC0F8
P 8150 3500
F 0 "#PWR016" H 8150 3350 50  0001 C CNN
F 1 "+5V" H 8165 3673 50  0000 C CNN
F 2 "" H 8150 3500 50  0001 C CNN
F 3 "" H 8150 3500 50  0001 C CNN
	1    8150 3500
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR015
U 1 1 609EC0FE
P 7950 3500
F 0 "#PWR015" H 7950 3350 50  0001 C CNN
F 1 "+5V" H 7965 3673 50  0000 C CNN
F 2 "" H 7950 3500 50  0001 C CNN
F 3 "" H 7950 3500 50  0001 C CNN
	1    7950 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 3550 7600 3850
Connection ~ 7600 3550
Wire Wire Line
	7450 3550 7600 3550
Wire Wire Line
	7600 3500 7600 3550
Wire Wire Line
	6800 3550 7150 3550
Text GLabel 7500 4150 2    50   Input ~ 0
ALERT
Text GLabel 7500 4550 2    50   Input ~ 0
+5V
Text GLabel 7500 4450 2    50   Input ~ 0
SDA
Text GLabel 7500 4350 2    50   Input ~ 0
SCL
$Comp
L Device:C C4
U 1 1 609EC112
P 7300 3550
F 0 "C4" V 7048 3550 50  0000 C CNN
F 1 "1uF" V 7139 3550 50  0000 C CNN
F 2 "Capacitor_SMD:C_0201_0603Metric_Pad0.64x0.40mm_HandSolder" H 7338 3400 50  0001 C CNN
F 3 "~" H 7300 3550 50  0001 C CNN
	1    7300 3550
	0    1    1    0   
$EndComp
Wire Wire Line
	7100 3850 7600 3850
$Comp
L power:+5V #PWR014
U 1 1 609EC119
P 7600 3500
F 0 "#PWR014" H 7600 3350 50  0001 C CNN
F 1 "+5V" H 7615 3673 50  0000 C CNN
F 2 "" H 7600 3500 50  0001 C CNN
F 3 "" H 7600 3500 50  0001 C CNN
	1    7600 3500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR013
U 1 1 609EC11F
P 7100 4850
F 0 "#PWR013" H 7100 4600 50  0001 C CNN
F 1 "GND" H 7105 4677 50  0000 C CNN
F 2 "" H 7100 4850 50  0001 C CNN
F 3 "" H 7100 4850 50  0001 C CNN
	1    7100 4850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 609EC125
P 6800 3550
F 0 "#PWR012" H 6800 3300 50  0001 C CNN
F 1 "GND" H 6805 3377 50  0000 C CNN
F 2 "" H 6800 3550 50  0001 C CNN
F 3 "" H 6800 3550 50  0001 C CNN
	1    6800 3550
	1    0    0    -1  
$EndComp
$Comp
L Analog_ADC:ADS1115IDGS U2
U 1 1 609EC12B
P 7100 4350
F 0 "U2" H 7100 5031 50  0000 C CNN
F 1 "ADS1115IDGS" H 7100 4940 50  0000 C CNN
F 2 "Package_SO:TSSOP-10_3x3mm_P0.5mm" H 7100 3850 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/ads1113.pdf" H 7050 3450 50  0001 C CNN
	1    7100 4350
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR018
U 1 1 609ED7BC
P 8600 4550
F 0 "#PWR018" H 8600 4400 50  0001 C CNN
F 1 "+5V" H 8615 4723 50  0000 C CNN
F 2 "" H 8600 4550 50  0001 C CNN
F 3 "" H 8600 4550 50  0001 C CNN
	1    8600 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 4550 8600 4550
$Comp
L power:+5V #PWR01
U 1 1 609A937C
P 3250 950
F 0 "#PWR01" H 3250 800 50  0001 C CNN
F 1 "+5V" H 3265 1123 50  0000 C CNN
F 2 "" H 3250 950 50  0001 C CNN
F 3 "" H 3250 950 50  0001 C CNN
	1    3250 950 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 609A9E29
P 3250 1250
F 0 "#PWR02" H 3250 1000 50  0001 C CNN
F 1 "GND" H 3255 1077 50  0000 C CNN
F 2 "" H 3250 1250 50  0001 C CNN
F 3 "" H 3250 1250 50  0001 C CNN
	1    3250 1250
	1    0    0    -1  
$EndComp
$Comp
L pspice:INDUCTOR L1
U 1 1 609AA2DD
P 2850 950
F 0 "L1" H 2850 1165 50  0000 C CNN
F 1 "INDUCTOR" H 2850 1074 50  0000 C CNN
F 2 "Inductor_SMD:L_0201_0603Metric_Pad0.64x0.40mm_HandSolder" H 2850 950 50  0001 C CNN
F 3 "~" H 2850 950 50  0001 C CNN
	1    2850 950 
	1    0    0    -1  
$EndComp
$Comp
L pspice:INDUCTOR L2
U 1 1 609B1826
P 2850 1250
F 0 "L2" H 2850 1465 50  0000 C CNN
F 1 "INDUCTOR" H 2850 1374 50  0000 C CNN
F 2 "Inductor_SMD:L_0201_0603Metric_Pad0.64x0.40mm_HandSolder" H 2850 1250 50  0001 C CNN
F 3 "~" H 2850 1250 50  0001 C CNN
	1    2850 1250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 609B209F
P 2500 1100
F 0 "C1" H 2385 1054 50  0000 R CNN
F 1 "1uF" H 2385 1145 50  0000 R CNN
F 2 "Capacitor_SMD:C_0201_0603Metric_Pad0.64x0.40mm_HandSolder" H 2538 950 50  0001 C CNN
F 3 "~" H 2500 1100 50  0001 C CNN
	1    2500 1100
	-1   0    0    1   
$EndComp
Wire Wire Line
	2250 950  2500 950 
Wire Wire Line
	2500 950  2600 950 
Connection ~ 2500 950 
Wire Wire Line
	3100 950  3250 950 
Wire Wire Line
	3250 1250 3100 1250
Wire Wire Line
	2600 1250 2500 1250
Wire Wire Line
	2250 950  2250 1150
Connection ~ 2500 1250
Text GLabel 1100 1250 0    50   Input ~ 0
SCL
Text GLabel 1100 1150 0    50   Input ~ 0
SDA
Wire Wire Line
	7100 4750 7100 4850
Text GLabel 2200 1150 2    50   Input ~ 0
VDD
Text GLabel 2200 1250 2    50   Input ~ 0
GND
$Comp
L Connector:Conn_01x06_Male Add48_01
U 1 1 60A560F7
P 4450 1900
F 0 "Add48_01" H 4558 2281 50  0000 C CNN
F 1 "Sensor 1 " H 4558 2190 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 4450 1900 50  0001 C CNN
F 3 "~" H 4450 1900 50  0001 C CNN
	1    4450 1900
	1    0    0    -1  
$EndComp
Text GLabel 4950 2200 2    50   Input ~ 0
S1_VDD
Text GLabel 4950 2100 2    50   Input ~ 0
S1_GND
Text GLabel 4950 3150 2    50   Input ~ 0
S2_VDD
Text GLabel 4950 3050 2    50   Input ~ 0
S2_GND
Text GLabel 4950 2950 2    50   Input ~ 0
S2_WE+
Text GLabel 4950 2750 2    50   Input ~ 0
S2_AUX+
$Comp
L Connector:Conn_01x06_Male Add48_23
U 1 1 60A6FBF8
P 4450 2850
F 0 "Add48_23" H 4558 3231 50  0000 C CNN
F 1 "Sensor 2" H 4558 3140 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 4450 2850 50  0001 C CNN
F 3 "~" H 4450 2850 50  0001 C CNN
	1    4450 2850
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x06_Male Add49_01
U 1 1 60A85268
P 4450 3750
F 0 "Add49_01" H 4558 4131 50  0000 C CNN
F 1 "Sensor 3" H 4558 4040 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 4450 3750 50  0001 C CNN
F 3 "~" H 4450 3750 50  0001 C CNN
	1    4450 3750
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x06_Male Add49_23
U 1 1 60A861B4
P 4450 4700
F 0 "Add49_23" H 4558 5081 50  0000 C CNN
F 1 "Sensor 4" H 4558 4990 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 4450 4700 50  0001 C CNN
F 3 "~" H 4450 4700 50  0001 C CNN
	1    4450 4700
	1    0    0    -1  
$EndComp
Text GLabel 4950 4050 2    50   Input ~ 0
S3_VDD
Text GLabel 4950 3950 2    50   Input ~ 0
S3_GND
Text GLabel 4950 3850 2    50   Input ~ 0
S3_WE+
Text GLabel 4950 3650 2    50   Input ~ 0
S3_AUX+
Text GLabel 4950 5000 2    50   Input ~ 0
S4_VDD
Text GLabel 4950 4900 2    50   Input ~ 0
S4_GND
Text GLabel 4950 4800 2    50   Input ~ 0
S4_WE+
Text GLabel 4950 4600 2    50   Input ~ 0
S4_AUX+
$Comp
L power:+5V #PWR010
U 1 1 60AB2508
P 5550 2300
F 0 "#PWR010" H 5550 2150 50  0001 C CNN
F 1 "+5V" H 5565 2473 50  0000 C CNN
F 2 "" H 5550 2300 50  0001 C CNN
F 3 "" H 5550 2300 50  0001 C CNN
	1    5550 2300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR011
U 1 1 60AB5310
P 5750 2100
F 0 "#PWR011" H 5750 1850 50  0001 C CNN
F 1 "GND" H 5755 1927 50  0000 C CNN
F 2 "" H 5750 2100 50  0001 C CNN
F 3 "" H 5750 2100 50  0001 C CNN
	1    5750 2100
	1    0    0    -1  
$EndComp
Text GLabel 4950 2000 2    50   Input ~ 0
S1_WE+
Text GLabel 4950 1800 2    50   Input ~ 0
S1_AUX+
Text GLabel 6700 2400 0    50   Input ~ 0
S1_WE+
Text GLabel 6700 2500 0    50   Input ~ 0
S1_AUX+
Text GLabel 6700 2600 0    50   Input ~ 0
S2_WE+
Text GLabel 6700 2700 0    50   Input ~ 0
S2_AUX+
Text GLabel 6700 4250 0    50   Input ~ 0
S3_WE+
Text GLabel 6700 4350 0    50   Input ~ 0
S3_AUX+
Text GLabel 6700 4450 0    50   Input ~ 0
S4_WE+
Text GLabel 6700 4550 0    50   Input ~ 0
S4_AUX+
$Comp
L power:+5V #PWR019
U 1 1 60AC7907
P 5550 3250
F 0 "#PWR019" H 5550 3100 50  0001 C CNN
F 1 "+5V" H 5565 3423 50  0000 C CNN
F 2 "" H 5550 3250 50  0001 C CNN
F 3 "" H 5550 3250 50  0001 C CNN
	1    5550 3250
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR021
U 1 1 60AC8300
P 5550 4150
F 0 "#PWR021" H 5550 4000 50  0001 C CNN
F 1 "+5V" H 5565 4323 50  0000 C CNN
F 2 "" H 5550 4150 50  0001 C CNN
F 3 "" H 5550 4150 50  0001 C CNN
	1    5550 4150
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR023
U 1 1 60AC8AC6
P 5550 5100
F 0 "#PWR023" H 5550 4950 50  0001 C CNN
F 1 "+5V" H 5565 5273 50  0000 C CNN
F 2 "" H 5550 5100 50  0001 C CNN
F 3 "" H 5550 5100 50  0001 C CNN
	1    5550 5100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR020
U 1 1 60AC9329
P 5750 3050
F 0 "#PWR020" H 5750 2800 50  0001 C CNN
F 1 "GND" H 5755 2877 50  0000 C CNN
F 2 "" H 5750 3050 50  0001 C CNN
F 3 "" H 5750 3050 50  0001 C CNN
	1    5750 3050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR022
U 1 1 60AC97B0
P 5750 3950
F 0 "#PWR022" H 5750 3700 50  0001 C CNN
F 1 "GND" H 5755 3777 50  0000 C CNN
F 2 "" H 5750 3950 50  0001 C CNN
F 3 "" H 5750 3950 50  0001 C CNN
	1    5750 3950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR024
U 1 1 60AC9C9D
P 5800 4900
F 0 "#PWR024" H 5800 4650 50  0001 C CNN
F 1 "GND" H 5805 4727 50  0000 C CNN
F 2 "" H 5800 4900 50  0001 C CNN
F 3 "" H 5800 4900 50  0001 C CNN
	1    5800 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 2100 5450 2100
Wire Wire Line
	5450 2100 5450 1900
Wire Wire Line
	5450 1900 4650 1900
Connection ~ 5450 2100
Wire Wire Line
	5450 2100 4650 2100
Wire Wire Line
	5450 1900 5450 1700
Wire Wire Line
	5450 1700 4650 1700
Connection ~ 5450 1900
Wire Wire Line
	4950 1800 4650 1800
Wire Wire Line
	4950 2000 4650 2000
Wire Wire Line
	5450 2200 5450 2300
Wire Wire Line
	5450 2300 5550 2300
Wire Wire Line
	4650 2200 5450 2200
Wire Wire Line
	4650 3150 5450 3150
Wire Wire Line
	5450 3150 5450 3250
Wire Wire Line
	5450 3250 5550 3250
Wire Wire Line
	5750 3050 5450 3050
Wire Wire Line
	5450 3050 5450 2850
Wire Wire Line
	5450 2850 4650 2850
Connection ~ 5450 3050
Wire Wire Line
	5450 3050 4650 3050
Wire Wire Line
	5450 2850 5450 2650
Wire Wire Line
	5450 2650 4650 2650
Connection ~ 5450 2850
Wire Wire Line
	4950 2750 4650 2750
Wire Wire Line
	4950 2950 4650 2950
Wire Wire Line
	5550 4150 5450 4150
Wire Wire Line
	5450 4150 5450 4050
Wire Wire Line
	5450 4050 4650 4050
Wire Wire Line
	4650 3950 5450 3950
Wire Wire Line
	5450 3950 5450 3750
Wire Wire Line
	5450 3750 4650 3750
Connection ~ 5450 3950
Wire Wire Line
	5450 3950 5750 3950
Wire Wire Line
	5450 3750 5450 3550
Wire Wire Line
	5450 3550 4650 3550
Connection ~ 5450 3750
Wire Wire Line
	4650 3650 4950 3650
Wire Wire Line
	4950 3850 4650 3850
Wire Wire Line
	4650 5000 5450 5000
Wire Wire Line
	5450 5000 5450 5100
Wire Wire Line
	5450 5100 5550 5100
Wire Wire Line
	5800 4900 5450 4900
Wire Wire Line
	4650 4800 4950 4800
Wire Wire Line
	4650 4600 4950 4600
Wire Wire Line
	4650 4500 5450 4500
Wire Wire Line
	5450 4500 5450 4700
Connection ~ 5450 4900
Wire Wire Line
	5450 4900 4650 4900
Wire Wire Line
	5450 4700 4650 4700
Connection ~ 5450 4700
Wire Wire Line
	5450 4700 5450 4900
$Comp
L Connector_Generic_MountingPin:Conn_02x20_Odd_Even_MountingPin J1
U 1 1 609D6820
P 1550 1950
F 0 "J1" H 1600 3067 50  0000 C CNN
F 1 "Conn_02x20_Odd_Even_MountingPin" H 1600 2976 50  0000 C CNN
F 2 "Connector_IDC:IDC-Header_2x20-1MP_P2.54mm_Latch6.5mm_Vertical" H 1550 1950 50  0001 C CNN
F 3 "~" H 1550 1950 50  0001 C CNN
	1    1550 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 1150 2250 1150
Wire Wire Line
	1850 1250 2500 1250
Wire Wire Line
	1100 1150 1350 1150
Wire Wire Line
	1350 1250 1100 1250
$EndSCHEMATC

# PowerPuck
I started this project because I was tired of my fatshark beeping and shutting down at 3.6 volts. I am using a 2s 18650 3500mAh pack. This means my batteries are still almost half full when this happens. I first just used a 9v stepup to boost the voltage, but then tracking the battery level was a pain. Then I came up with this solution. 

It not only lets you use your 18650 batteries up to 3v (per cell), it also adds voltage monitoring with adjustable alarms and a power on/off switch. On top of that the fan cable is now tidy and nice and the complete unit remains on the goggles the whole time while the battery is sitting in the pocket or on the belt.

The battery case has also a basic battery indicator - it gives a pretty good orientation of the battery state without hooking the battery up. 

<img src="https://raw.githubusercontent.com/skaman82/PowerPuck/master/pictures/unit.jpg"/>
<img src="https://raw.githubusercontent.com/skaman82/PowerPuck/master/pictures/unit2.jpg"/>


<img src="https://raw.githubusercontent.com/skaman82/PowerPuck/master/pictures/operation_f.jpg"/>


# MAIN UNIT COMPONENTS
<img src="https://raw.githubusercontent.com/skaman82/PowerPuck/master/PCB%20Files/Final%20Gerber/square%20Rev22.jpg"/>

<b>Printed Case</b></br>
See 3D files directory</br>
or order it here:</br>
https://www.shapeways.com/product/Y7TP35MAT/powerpuck-case-front</br>
https://www.shapeways.com/product/XVMKZEAJ4/powerpuck-case-back</br>
</br></br>

<b>Order the PCB on jlcpcb.com (1mm thickness)</b></br>
See "PCB Files > Final Gerber" directory</br></br>

<b>ATMEGA382 AU</b></br>
<b>Y1: Xtal 16MHZ TXC 7A-16.000MAAJ-T</b></br>
<b>C1: 0,1uF cap 0603</b></br>
<b>C5: 100nF cap 0603</b></br>
<b>C3, C4: 18pF cap 0603 (2x)</b></br>
<b>10K 0603 Resistor</b></br>
<b>R3-R6: 330Ohm 0603 Resistor (4x)</b></br>
<b>D2-D4: GREEN LED 1206 (3x)</b></br>
<b>D1: 1x RED LED 1206</b></br>
<b>R1: 18K Resistor 1206</b></br>
<b>R2: 4,7K Resistor 1206</b></br>
<b>C6, C7: 10uF Cap 1206 (2x)</b></br>
<b>D6: Diode S1A</b></br>
<b>Pololu 9V Step-Up Voltage Regulator U3V12F9</b></br>
<b>Pololu Mini Pushbutton Power Switch SV</b></br>
<b>JST-SH Jack 6pin</b></br>
<b>9mm active Buzzer</b></br>
<b>8mm tact button</b></br>
<b>AMASS XT30 female plug</b></br>
<b>AMS1117/LM1117 regulator</b></br>
<b>Epoxy</b></br></br>



<b>ISCP USB Programmer like</b></br>
https://de.aliexpress.com/item/5-v-Micro-USB-Tiny-AVR-ISP-ATtiny44-USBTinyISP-Programmierer-Modul-F-r-Bootloader-ISP-Mikrocontroller/32925343387.html?spm=a2g0s.9042311.0.0.725e4c4dZq4br5</br></br>


<img src="https://raw.githubusercontent.com/skaman82/PowerPuck/master/pictures/wiring.jpg"/>

<img src="https://raw.githubusercontent.com/skaman82/PowerPuck/master/pictures/iscp.jpg"/>

<img src="https://raw.githubusercontent.com/skaman82/PowerPuck/master/pictures/soldered.jpg"/>



# 18650 CASE COMPONENTS
<b>Printed Case</b></br>
See 3D files directory</br></br>

<b>2S Power-Indicator</b></br>
https://www.banggood.com/2S-3S-4S-5S-Lipo-Battery-Voltage-Display-Indicator-Board-Polymer-Power-Indicator-Panel-p-1095698.html?p=03160313547712015036</br></br>

<b>2S BMS PCB</b></br>
https://www.aliexpress.com/item/2S-7-4V-8-4V-8A-16A-18650-Lithium-Battery-Protection-Board-Current-Li-ion-2/32851804815.html?spm=a2g0s.9042311.0.0.27424c4dYKwYNW</br></br>

<b>XT30 connectors</b></br>
https://www.banggood.com/de/Tarot-RC-Helicopter-Part-Amass-One-Pair-XT30-Non-slip-Plugs-TL2918-p-954269.html?p=03160313547712015036</br></br>

<b>2S Balance plug</b></br>
https://www.banggood.com/de/Lipo-Battery-Balance-Charging-Extension-Wire-Cable-20cm-2S-3S-4S-6S-p-951866.html?p=03160313547712015036
</br>
<b>Epoxy</b></br></br>

<img src="https://raw.githubusercontent.com/skaman82/PowerPuck/master/pictures/case1.jpg"/>
<img src="https://raw.githubusercontent.com/skaman82/PowerPuck/master/pictures/case2.jpg"/>

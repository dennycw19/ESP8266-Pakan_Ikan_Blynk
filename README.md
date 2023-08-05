# ESP8266-Pakan_Ikan_Blynk
Automatic fish feeder using ESP8266 integrated with Blynk

This device feeds the fish according to a pre-set schedule using the Blynk app. The method of feeding fish is by using the MG995 servo to move the fish feed container, so that the feed that is in the container spills into the aquarium. The LCD displays the time obtained from the DS3231 RTC Module and the temperature obtained from the DS18B20 temperature sensor module. All of these components are put in a black plastic box to make it look neater.

The components used in this device are:
1. NodeMCU V3 ESP8266
2. NodeMCU V3 ESP8266 Base Plate Board
3. LCD 16x2 I2C
4. Servo MG995
5. RTC 3231
6. DS18B20
7. Resistor 4.7K Ohm

# Device Schematic
![PakanIkan_Skematik_bb](https://github.com/dennycw19/ESP8266-Pakan_Ikan_Blynk/assets/7360210/0e4ff1fe-4eb6-49b6-a601-f07c616f4b38)

# Project result
![WhatsApp Image 2023-08-04 at 7 40 44 PM](https://github.com/dennycw19/ESP8266-Pakan_Ikan_Blynk/assets/7360210/bb3b0bc0-532f-489d-a5e3-3c1a4c789b6f)

# Upcoming features
1. Add a temperature monitoring feature via the Blynk application
2. Add turbidity sensor to monitor water turbidity level via LCD and Blynk application

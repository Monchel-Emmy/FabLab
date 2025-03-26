## Project:  🌊SMART SWIMMING POOL

**Swimming pools**  are a popular source of recreation, but ensuring the safety of swimmers and maintaining optimal water quality are crucial concerns.
This project introduces the IoT-based Smart Swimming Pool Monitoring System, designed to enhance safety and water quality management. Situated in Rwanda,
this system leverages state-of-the-art technology to provide a safer and more enjoyable swimming experience.

## INTRODUCTION TO SMART SWIMMING POOL

Swimming pools are a popular source of recreation, but ensuring the safety of 
swimmers and maintaining optimal water quality are crucial concerns. This 
project introduces the IoT-based Smart Swimming Pool Monitoring System, 
designed to enhance safety and water quality management. 
Situated in Rwanda, this system leverages state-of-the-art technology to provide a 
safer and more enjoyable swimming experience.

The primary objective is to improve swimming pool safety, water quality, and 
operational efficiency by integrating IoT technology. This system features a 
wearable monitoring device equipped with pH,  temperature, and heartbeat sensors that continuously collect real-time data, 
monitoring both water conditions and swimmers' well-being.

At its core, the system relies on IoT connectivity. An ESP8266 module, integrated 
into the wearable device, facilitates real-time data collection and secure 
underwater Wi-Fi communication for transmitting data to a cloud-based platform. 
This cloud platform, integrated with the Arduino IoT Cloud, serves as the hub for 
data storage, analysis, and visualization.

Users, including pool operators and authorities, access the cloud platform 
through web interfaces and mobile apps. Real-time data visualization, historical 
analysis, and customizable alerts empower users to make informed decisions. 
Alerts can be configured to trigger when predefined thresholds for pH, 
temperature, or irregular heartbeats are exceeded, enabling proactive 
interventions

 ## Objectives
 
 1. Water Quality Monitoring:
Develop a robust IoT-based system capable of continuous and real-time monitoring and maintenance of swimming pool water quality, 
ensuring it consistently meets established safety and hygiene standards.
2. Swimmer Safety:
Implement an integrated real-time heartbeat monitoring system using wearable devices or underwater sensors to promptly detect irregularities in swimmers' heart rates and enhance their safety while swimming.
3. Data Analytics and Reporting:
Employ advanced data analytics algorithms to process the collected data, generating insights, trends, and real-time reporting for pool operators and relevant authorities, enabling data-driven decision-making.

## CIRCUITLY SYSTEM WORKING PRINCIPLE 💼

In the Smart Swimming Pool Monitoring System, each swimmer wears a 
specialized wearable device equipped with integrated sensors designed for 
comprehensive monitoring. These sensors 
include a pH water sensor, responsible for continuously measuring and 
monitoring the pH level of the pool water, ensuring optimal water quality. 
Additionally, a water temperature sensor is incorporated, providing real-time 
temperature data of the pool water for comfort and safety 
assessment. Furthermore, the wearable device features a heartbeat sensor, 
constantly monitoring the swimmer's heart rate to ensure their well-being during 
aquatic activities. The integrated ESP8266 module within the wearable device 
facilitates Wi-Fi communication, including underwater data transmission to the 
cloud. This collected data, which includes pH levels, water temperature, and 
swimmers' heart rates, is organized and processed within the wearable device, 
securely transmitted to the cloud platform, and integrated with the Arduino IoT 
Cloud. This platform empowers users, such as pool operators and authorities, to 
access real-time data visualization and historical analysis, enabling data-driven 
decisions. The system can also be configured to issue alerts and notifications 
when predefined thresholds are exceeded, thereby enhancing swimmer safety.

# REQUIREMENTS TO ACCOMPLISH AND SOLVE THE PROBLEM 🧰

1. Arduino nano 
2. ESP8266 Wi-Fi module 
3.  heartbeat sensor(pulse rate sensor)
4.  pH sensor
5.  temperature sensors
6.  Vibration  
7.   resistors
8. a 12V DC battery.
9. voltage regulator 
10.  waterproof device

11. ## This project involves developing a two-part IoT system designed to monitor a swimmer's heart rate and the quality of the pool water. The system aims to enhance safety by providing real-time data and alerts. The two systems involved are:

Wearable Device: Monitors the swimmer's heartbeat and provides haptic feedback (via a vibration motor) in emergency situations.
Underwater Device: Measures the pH and temperature of the pool water, receives heartbeat data from the wearable device, and sends all the data to ThingSpeak and WhatsApp.

## System Components

# Wearable Device

Heartbeat Sensor
Vibration Motor
ESP8266 NodeMCU
Battery Pack

# Underwater Device

pH Sensor (Analog)
DS18B20 Temperature Sensor
ESP8266 NodeMCU
WiFi Network
ThingSpeak API
CallMeBot API for WhatsApp messages
System Workflow
Wearable Device:

Continuously monitors the swimmer's heartbeat.
Triggers the vibration motor to alert the swimmer if an abnormal heart rate is detected.
Sends heartbeat data to the underwater device via ESP-NOW.
Underwater Device:

Measures the pH and temperature of the pool water.
Receives heartbeat data from the wearable device.
Sends collected data (pH, temperature, heartbeat) to ThingSpeak for visualization.
Sends alerts to WhatsApp using the CallMeBot API if predefined conditions are met (e.g., abnormal pH levels or heart rate).
Detailed Code and Explanation

# Underwater Device Code::

 '''#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <UrlEncode.h>
#include <espnow.h>
#include <OneWire.h>
#include <DallasTemperature.h>

ESP8266WiFiMulti WiFiMulti;

// Define the MAC address of the sender ESP8266 module (Wearable Device)
uint8_t senderMac[] = {0x4C, 0x11, 0xAE, 0x06, 0xD8, 0x9E};

typedef struct struct_message {
    char message[32];
    int pulse_value;
    int heartRate;
    int ph_value;
    float temp_value;
} struct_message;

struct_message myData;
struct_message incomingData;

// PH sensor configuration
const int pH_pin = A0;
const float calibration_value = 21.34 - 0.7;

// Temperature sensor configuration
const int SENSOR_PIN = D2;
OneWire oneWire(SENSOR_PIN);
DallasTemperature tempSensor(&oneWire);

// WhatsApp configuration
String phoneNumber = "+250780904149";  // replace yours
String apiKey = "67319997250"; // replace yours

// Function to read pH value
float readPH() {
    int sensorValue = analogRead(pH_pin);
    float voltage = sensorValue * (5.0 / 1023.0); // Assuming 5V Arduino
    float pH_value = 7 - (voltage - calibration_value);
    return pH_value;
}

// Function to read temperature
float readTemperature() {
    tempSensor.requestTemperatures();
    float tempCelsius = tempSensor.getTempCByIndex(0);
    return tempCelsius;
}

// Callback function that will be executed when data is received
void onDataRecv(uint8_t *mac, uint8_t *incomingDataBytes, uint8_t len) {
    memcpy(&incomingData, incomingDataBytes, sizeof(incomingData));
    Serial.print("Bytes received: ");
    Serial.println(len);
    Serial.print("Message: ");
    Serial.println(incomingData.message);
    Serial.print("Pulse Value: ");
    Serial.println(incomingData.pulse_value);
    Serial.print("Heart Rate: ");
    Serial.println(incomingData.heartRate);

    // Respond with pH and temperature values
    #include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <UrlEncode.h>
#include <espnow.h>
#include <OneWire.h>
#include <DallasTemperature.h>

ESP8266WiFiMulti WiFiMulti;

// Define the MAC address of the sender ESP8266 module (Wearable Device)
uint8_t senderMac[] = {0x4C, 0x11, 0xAE, 0x06, 0xD8, 0x9E};  // replace yours

typedef struct struct_message {
    char message[32];
    int pulse_value;
    int heartRate;
    int ph_value;
    float temp_value;
} struct_message;

struct_message myData;
struct_message incomingData;

// PH sensor configuration
const int pH_pin = A0;
const float calibration_value = 21.34 - 0.7;

// Temperature sensor configuration
const int SENSOR_PIN = D2;
OneWire oneWire(SENSOR_PIN);
DallasTemperature tempSensor(&oneWire);

// WhatsApp configuration
String phoneNumber = "+250780904149";
String apiKey = "67319997250";

// Function to read pH value
float readPH() {
    int sensorValue = analogRead(pH_pin);
    float voltage = sensorValue * (5.0 / 1023.0); // Assuming 5V Arduino
    float pH_value = 7 - (voltage - calibration_value);
    return pH_value;
}

// Function to read temperature
float readTemperature() {
    tempSensor.requestTemperatures();
    float tempCelsius = tempSensor.getTempCByIndex(0);
    return tempCelsius;
}

// Callback function that will be executed when data is received
void onDataRecv(uint8_t *mac, uint8_t *incomingDataBytes, uint8_t len) {
    memcpy(&incomingData, incomingDataBytes, sizeof(incomingData));
    Serial.print("Bytes received: ");
    Serial.println(len);
    Serial.print("Message: ");
    Serial.println(incomingData.message);
    Serial.print("Pulse Value: ");
    Serial.println(incomingData.pulse_value);
    Serial.print("Heart Rate: ");
    Serial.println(incomingData.heartRate);

    // Respond with pH and temperature values
    myData.ph_value = readPH();
    myData.temp_value = readTemperature();
    strcpy(myData.message, "Hello from me..");
    if (esp_now_send(mac, (uint8_t *) &myData, sizeof(myData)) == 0) {
        Serial.println("Response sent successfully");
    } else {
        Serial.println("Error sending response");
    }
    Serial.print("pH Value: ");
    Serial.println(myData.ph_value);
    Serial.print("Temperature Value: ");
    Serial.println(myData.temp_value);
}

// Function to send data to ThingSpeak
void sendDataToThingSpeak(float ph_value) {
    WiFiClient client;
    HTTPClient http;
    if (http.begin(client, "http://api.thingspeak.com/update?api_key=OCJK240V3191119994KU6H7Z&field1=" + String(ph_value))) {
        Serial.print("[HTTP] GET...\n");
        int httpCode = http.GET();
        if (httpCode > 0) {
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
                String payload = http.getString();
                Serial.println(payload);
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    } else {
        Serial.println("[HTTP] Unable to connect");
    }
}

// Function to send a message to WhatsApp
void sendMessage(String message) {
    String url = "http://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);
    WiFiClient client;
    HTTPClient http;
    http.begin(client, url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpResponseCode = http.POST("");
    if (httpResponseCode == 200) {
        Serial.println("Message sent successfully");
    } else {
        Serial.println("Error sending the message");
        Serial.print("HTTP response code: ");
        Serial.println(httpResponseCode);
    }
    delay(1000);
    http.end();
}

void setup() {
    Serial.begin(115200);
    for (uint8_t t = 4; t > 0; t--) {
        Serial.printf("[SETUP] WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
    }

    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP("UR-CST", "");
    tempSensor.begin();

    if (esp_now_init() != 0) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    esp_now_register_recv_cb(onDataRecv);
    esp_now_add_peer(senderMac, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

    strcpy(myData.message, "Hello from me..");
}

void loop() {
    while (WiFiMulti.run() != WL_CONNECTED) {
        Serial.print("loading..");
        delay(3000);
    }
    if (WiFiMulti.run() == WL_CONNECTED) {
        float ph_value = readPH();
        float temp_value = readTemperature();
        Serial.print("pH Value: ");
        Serial.println(ph_value);
        Serial.print("Temperature Value: ");
        Serial.println(temp_value);

        if (ph_value == 24) {
            sendMessage(String(ph_value));
        }
        sendDataToThingSpeak(ph_value);
    }
    delay(1000);
}



## Wearable Device Code (Example Outline)

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

// Heartbeat Sensor configuration
const int heartBeatPin = A0;

// Vibration Motor configuration
const int vibrationMotorPin = D1;

// Define the MAC address of the receiver ESP8266 module (Underwater Device)
uint8_t receiverMac[] = {0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX};

typedef struct struct_message {
    char message[32];
    int pulse_value;
    int heartRate;
} struct_message;

struct_message myData;

// Function to read heart rate
int readHeartRate() {
    // Implementation of heart rate reading
    int heartRate = analogRead(heartBeatPin);
    return heartRate;
}

// Function to activate vibration motor
void activateVibrationMotor() {
    digitalWrite(vibrationMotorPin, HIGH);
    delay(1000);
    digitalWrite(vibrationMotorPin, LOW);
}

void setup() {
    Serial.begin(115200);
    pinMode(heartBeatPin, INPUT);
    pinMode(vibrationMotorPin, OUTPUT);

    if (esp_now_init() != 0) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    esp_now_add_peer(receiverMac, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

    strcpy(myData.message, "Hello from wearable..");
}

void loop() {
    myData.heartRate = readHeartRate();
    Serial.print("Heart Rate: ");
    Serial.println(myData.heartRate);

    if (myData.heartRate > THRESHOLD) {  // THRESHOLD should be defined based on the safe heart rate range
        activateVibrationMotor();
    }

    if (esp_now_send(receiverMac, (uint8_t *) &myData, sizeof(myData)) == 0) {
        Serial.println("Data sent successfully");
    } else {
        Serial.println("Error sending data");
    }

    delay(1000);  // Adjust the delay as necessary
}


System Integration
## Wearable Device:

Continuously reads heart rate.
Activates vibration motor if the heart rate exceeds the safe threshold.
Sends heart rate data to the underwater device using ESP-NOW.

Underwater Device:

Receives heart rate data from the wearable device.
Reads pH and temperature of the water.
Sends data to ThingSpeak.
Sends alerts via WhatsApp if conditions are met.
Photos and Visuals

# 1. Circuit Diagram
Wearable Device Circuit Diagram:

Diagram showing connections between the ESP8266, heartbeat sensor, and vibration motor.
Underwater Device Circuit Diagram:

Diagram showing connections between the ESP8266, pH sensor, and temperature sensor.

# 2. Physical Setup

Photos of the actual physical setup, including:

Wearable device with ESP8266, heartbeat sensor, and vibration motor.
Underwater device with ESP8266, pH sensor, and temperature sensor.

# 3. Serial Monitor Output

Screenshots of the serial monitor output showing:

Initial setup messages.
Heart rate readings from the wearable device.
pH and temperature readings from the underwater device.
Confirmation messages for data sent to ThingSpeak and WhatsApp.

## Conclusion
This document provides a comprehensive overview of the IoT health and environmental monitoring system. It includes the source code for both the wearable and underwater devices, detailed explanations, and instructions for setting up the hardware and software components. The project successfully integrates heart rate, pH, and temperature monitoring, allowing for real-time data visualization and alerts.

## Future Work

Integrate additional sensors for more comprehensive environmental monitoring.
Enhance the wearable device for better comfort and durability.
Improve data transmission reliability and efficiency.





1.ip68 waterproof



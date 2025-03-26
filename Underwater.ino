// Include the necessary libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <UrlEncode.h>
#include <espnow.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Set up WiFi and ESP-NOW
ESP8266WiFiMulti WiFiMulti;
uint8_t senderMac[] = {0x4C, 0x11, 0xAE, 0x06, 0xD8, 0x9E}; // MAC of the sender ESP

// Define message structure
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
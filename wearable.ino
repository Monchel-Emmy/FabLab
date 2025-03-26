// Include the necessary libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

// Set up the heart rate sensor and vibration motor
const int heartRatePin = A0;
const int vibrationMotorPin = D1;
const int THRESHOLD = 100;  // Safe heart rate threshold
uint8_t receiverMac[] = {0x4C, 0x11, 0xAE, 0x06, 0xD8, 0x9E}; // MAC of the receiver ESP

// Define message structure
typedef struct struct_message {
    char message[32];
    int pulse_value;
    int heartRate;
    int ph_value;
    float temp_value;
} struct_message;

struct_message myData;

void activateVibrationMotor() {
    digitalWrite(vibrationMotorPin, HIGH);
    delay(500);
    digitalWrite(vibrationMotorPin, LOW);
}

void setup() {
    Serial.begin(115200);
    pinMode(vibrationMotorPin, OUTPUT);
    digitalWrite(vibrationMotorPin, LOW);

    WiFi.mode(WIFI_STA);
    if (esp_now_init() != 0) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    esp_now_add_peer(receiverMac, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

    strcpy(myData.message, "Hello from wearable..");
}

void loop() {
    int heartRate = analogRead(heartRatePin);  // Simplified for demonstration
    myData.heartRate = heartRate;

    if (heartRate > THRESHOLD) {  // THRESHOLD should be defined based on the safe heart rate range
        activateVibrationMotor();
    }

    if (esp_now_send(receiverMac, (uint8_t *) &myData, sizeof(myData)) == 0) {
        Serial.println("Data sent successfully");
    } else {
        Serial.println("Error sending data");
    }

    delay(1000);  // Adjust the delay as necessary
}
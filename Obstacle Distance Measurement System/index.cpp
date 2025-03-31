#include <WiFi.h>  // For ESP32
#include <BlynkSimpleEsp32.h>  // Use BlynkSimpleEsp8266.h for ESP8266

// Blynk Authentication Token
#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_DEVICE_NAME "Distance Sensor"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

char ssid[] = "Your_WiFi_SSID";  // WiFi Name
char pass[] = "Your_WiFi_Password";  // WiFi Password

// Ultrasonic Sensor Pins
#define TRIG_PIN 5
#define ECHO_PIN 18

BlynkTimer timer;

// Function to Measure Distance
float getDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    
    float duration = pulseIn(ECHO_PIN, HIGH);
    float distance = (duration * 0.0343) / 2;  // Convert to cm
    return distance;
}

// Send Data to Blynk
void sendData() {
    float distance = getDistance();
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    
    Blynk.virtualWrite(V1, distance);  // Send to Blynk App
    
    if (distance < 10) {  // Alert if object is too close
        Blynk.logEvent("obstacle_alert", "Obstacle detected too close!");
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
    timer.setInterval(1000L, sendData);  // Send data every second

    Serial.println("IoT Distance Measurement System Started.");
}

void loop() {
    Blynk.run();
    timer.run();
}
#include <ESP8266WiFi.h>

//-----------------------Wi-Fi setup----------------------
#define Wifi_led D4

const char* ssid = "CELENE";
const char* password = "1533498109";
const char* host = "192.168.1.79";
//-----------------XBee nodes ID and MAC-------------------
byte nodes[1][8] = {{0x00,0x13,0xA2,0x00,0x40,0xB5,0xEA,0x16}};
//------------------------Frame----------------------------
byte length_MSB;
byte length_LSB;
byte frame_data[] = {};
byte checksum;

void setup() {
    Serial.begin(115200);
    Serial1.begin(9600);

    pinMode(Wifi_led, OUTPUT);

    Serial.printf("Connecting with %s...", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        digitalWrite(Wifi_led, LOW);
        Serial.print('.');
        delay(500);
        digitalWrite(Wifi_led, HIGH);
        delay(500);
    }

    Serial.printf("\nConnected with %s\n", ssid);
    digitalWrite(Wifi_led, LOW);

}

void loop() {
    WiFiClient client;
    
    if (Serial1.available() && Serial1.read() == byte(0x7E)) {
        catch_frame();
    }
}

void catch_frame() {
    Serial.print("Frame: 7E");
    byte checksum_verify; //This variables will check if the checksum is correct
    length_MSB = byte(Serial1.read());
    Serial.print(length_MSB, HEX);
    length_LSB = byte(Serial1.read());
    Serial.print(length_LSB, HEX);
    for(int i = 0; i < length_LSB; i++) {
        frame_data[i] = byte(Serial1.read());
        Serial.print(frame_data[i], HEX);
        checksum_verify += frame_data[i];
    }
    checksum = byte(Serial1.read());
    Serial.println(checksum, HEX);
    checksum_verify += checksum;
    if(checksum_verify == byte(0xFF)) {
        Serial.println("Checksum verified!");
        Serial.println("Message recived successfully!");
    } else {
        Serial.println("Checksum failed.");
    }
}   

String get_message() {

}

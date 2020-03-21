
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WLAN_SSID       ""
#define WLAN_PASS       ""

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, "10.0.1.1", 1883);
Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, "domos/bedroom/switch");

void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(D1, INPUT);

  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
}

void loop() {
  if (!digitalRead(D1)) {

    MQTT_connect();
    Serial.print("...");
    if (! photocell.publish("1")) {
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("OK!"));
    }
    delay(1000);
  }
  delay(100);
}

void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { 
    mqtt.disconnect();
    delay(5000); 
    retries--;
    if (retries == 0) {
      while (1);
    }
  }
}

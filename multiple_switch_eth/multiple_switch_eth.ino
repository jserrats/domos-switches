#include <UIPEthernet.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


EthernetClient client;
Adafruit_MQTT_Client mqtt(&client, "10.0.1.1", 1883);
Adafruit_MQTT_Publish button = Adafruit_MQTT_Publish(&mqtt, "domos/desk/button");

void setup() {
  uint8_t mac[6] = {0x7e, 0x1e, 0xc0, 0xba, 0x55, 0x12};
    if (Ethernet.begin(mac) == 0) {
    Serial.println(F("Unable to configure Ethernet using DHCP"));
    for (;;);
  }
  for (int i = 3; i < 10; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  MQTT_connect();
  Serial.begin(9600);
  Serial.println("begin");
}

void loop() {
  for (int i = 3; i < 10; i++) {
    int sensorVal = digitalRead(i);
    Serial.print(sensorVal);
    if (sensorVal == HIGH) {
      //button.publish("high");
    } else {
      MQTT_connect();

      if (!button.publish("test")) {
        Serial.println(F("Failed"));
      }
      else
      {
        Serial.println(F("OK!"));
        delay(400);
      }
    }
  }
  Serial.println();
  delay(100);
}

void MQTT_connect()
{
  int8_t ret;
  Serial.println("trying to connect..");
  if (mqtt.connected())
  {
    return;
    Serial.println("mqtt connected");
  }

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0)
  {
    mqtt.disconnect();
    Serial.println("mqtt disconnected :(");
    delay(1000);
    retries--;
    if (retries == 0)
    {
      while (1)
        ;
    }
  }
}

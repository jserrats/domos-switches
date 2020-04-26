#include <UIPEthernet.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


EthernetClient client;
Adafruit_MQTT_Client mqtt(&client, "10.0.1.1", 1883);
Adafruit_MQTT_Publish button = Adafruit_MQTT_Publish(&mqtt, "domos/desk/button");

void setup() {
  uint8_t mac[6] = {0x7e, 0x1e, 0xc0, 0xba, 0x55, 0x06};
  Ethernet.begin(mac);
  for (int i = 2; i < 12; i++) {
    Serial.println(i);
    //pinMode(i, INPUT_PULLUP);
  }
  pinMode(2, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("begin");
}

void loop() {
  MQTT_connect();
  for (int i = 2; i < 12; i++) {
    Serial.println(i);
    int sensorVal = digitalRead(i);
    if (sensorVal == HIGH) {
      Serial.print(i);
      Serial.print(" HIGH\n");
      //button.publish("high");
    } else {
      //button.publish("low");
      Serial.print(i);
      Serial.print(" LOW\n");
    }
  }
  delay(1000);
}

void MQTT_connect()
{
  int8_t ret;
  if (mqtt.connected())
  {
    return;
  }
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0)
  {
    mqtt.disconnect();
    delay(5000);
    retries--;
    if (retries == 0)
    {
      while (1)
        ;
    }
  }
}

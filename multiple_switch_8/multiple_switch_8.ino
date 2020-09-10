#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WLAN_SSID       ""
#define WLAN_PASS       ""

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, "mqtt.lan", 1883);
Adafruit_MQTT_Publish button = Adafruit_MQTT_Publish(&mqtt, "domos/desk/button");

int pin_list[] = {D0, D1, D2, D3, D4, D5, D6, D7};
void MQTT_connect();


void setup()
{
  Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);
  delay(10);
  digitalWrite(BUILTIN_LED, LOW);

  for (int i = 0; i < 8; i++) {
    pinMode(pin_list[i], INPUT_PULLUP);
  }
  
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    
    Serial.println(WiFi.status());
    delay(500);
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(BUILTIN_LED, HIGH);


}


void loop()
{
  for (int i = 0; i < 8; i++)
  {
    int sensorVal = digitalRead(pin_list[i]);
    //Serial.print(sensorVal);
    if (sensorVal != HIGH)
    {
      Serial.println(i);
      MQTT_connect();
      Serial.print("...");
      if (!button.publish(i))
      {
        Serial.println(F("Failed"));
      }
      else
      {
        digitalWrite(BUILTIN_LED, LOW);
        Serial.println(F("OK!"));
        delay(500);
        digitalWrite(BUILTIN_LED, HIGH);
      }
      //    delay(500);
    }
  }
  delay(100); // delay in between reads for stability*/
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

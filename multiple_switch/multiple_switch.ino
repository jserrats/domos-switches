#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WLAN_SSID       ""
#define WLAN_PASS       "s"

int buttons[] = {540, 1024, 995, 912, 840, 779, 726, 680, 636, 600, 568, 514};

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, "mqtt.lan", 1883);
Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, "domos/bedroom/multiple-switch");

void setup()
{
  Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);
  delay(10);
  digitalWrite(BUILTIN_LED, LOW);

  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(BUILTIN_LED, HIGH);
}

bool isInRange(int value, int reference)
{
  int margin = 10;
  int lower = reference - margin;
  int upper = reference + margin;
  return (lower < value && upper > value);
}

void loop()
{
  int sensorValue = analogRead(A0);
  for (int i = 0; i < 12; i++)
  {
    if (isInRange(sensorValue, buttons[i]))
    {
      Serial.println(i);
      MQTT_connect();
      Serial.print("...");
      if (!photocell.publish(i))
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
  delay(100); // delay in between reads for stability
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

#include "WiFiMQTTHandler.h"

void setup_wifi(const char* ssid, const char* password) {
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void mqtt_init(PubSubClient& client, const char* mqtt_server) {
  client.setServer(mqtt_server, 1883);

  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected.");
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void handle_mqtt(PubSubClient& client) {
  if (!client.connected()) {
    mqtt_init(client, "192.168.138.158"); // Use o IP do broker MQTT
  }
  client.loop();
}

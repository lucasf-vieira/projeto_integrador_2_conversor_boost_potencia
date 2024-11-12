#include "WiFiMQTTHandler.h"

void setup_wifi(const char* ssid, const char* password){
  delay(10);
  Serial.println("\nConnecting to " + String(ssid));

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected! IP address: ");
  Serial.println(WiFi.localIP());
}



void reconnect(PubSubClient client) {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX); 

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000); 
    }
  }
}

void mqtt_init(PubSubClient client, const char* ip){
  client.setServer(ip, 1883);
}

void handle_mqtt(PubSubClient client){
  if (!client.connected()) {
    reconnect(client);
  }
  client.loop();
}
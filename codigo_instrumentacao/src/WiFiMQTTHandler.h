#ifndef WIFIMQTTHANDLER_H
#define WIFIMQTTHANDLER_H

#include <PubSubClient.h>
#include <WiFi.h>

void setup_wifi(const char* ssid, const char* password);
void mqtt_init(PubSubClient client, const char* ip);
void reconnect(PubSubClient client);
void handle_mqtt(PubSubClient client);
#endif
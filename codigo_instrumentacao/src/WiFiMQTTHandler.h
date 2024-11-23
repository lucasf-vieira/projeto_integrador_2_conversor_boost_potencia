#ifndef WIFI_MQTT_HANDLER_H
#define WIFI_MQTT_HANDLER_H

#include <WiFi.h>
#include <PubSubClient.h>

// Funções de conexão Wi-Fi
void setup_wifi(const char* ssid, const char* password);

// Funções de configuração e manejo do MQTT
void mqtt_init(PubSubClient& client, const char* mqtt_server);
void handle_mqtt(PubSubClient& client);

#endif

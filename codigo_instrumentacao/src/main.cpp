#include "WiFiMQTTHandler.h"


const char* ssid = "nome_da_rede";
const char* password =  "senha_da_rede";
const char* ip_addr = "IP_do_pc";
const int MAX_BUFFER_SIZE = 50;


WiFiClient espClient;
PubSubClient client(espClient);
char msg[MAX_BUFFER_SIZE];


void setup() {
  Serial.begin(115200);

  setup_wifi(ssid, password);
  mqtt_init(client, ip_addr);
}


void loop() {
  // Teste de conexão com o servidor MQTT
  handle_mqtt(client);

  // Envio da Mensagem ao Topico "outTopic"
  snprintf (msg, MAX_BUFFER_SIZE, "hello world!");
  Serial.print("Publish message: \n" + String(msg));
  client.publish("outTopic", msg);
  delay(500);
}


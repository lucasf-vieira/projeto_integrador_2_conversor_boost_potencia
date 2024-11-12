#include "WiFiMQTTHandler.h"

const char* ssid = "nome_da_rede";
const char* password =  "senha_da_rede";
const char* ip_addr = "IP_do_pc";
const int MAX_BUFFER_SIZE = 50;

WiFiClient espClient;
PubSubClient client(espClient);
char msg[MAX_BUFFER_SIZE];


void setup_wifi(){
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


void reconnect() {
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

void mqtt_init(const char* ip){
  client.setServer(ip, 1883);
}


void setup() {
  Serial.begin(115200);

  setup_wifi(ssid, password);
  mqtt_init(ip_addr);
}




void loop() {
  // Teste de conexão com o servidor MQTT
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Envio da Mensagem ao Topico "outTopic"
  snprintf (msg, MAX_BUFFER_SIZE, "hello world!");
  Serial.print("Publish message: \n" + String(msg));
  client.publish("outTopic", msg);
  delay(500);
}


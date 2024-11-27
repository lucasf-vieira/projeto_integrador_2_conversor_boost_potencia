#include <WiFiMQTTHandler.h>
#include <Adafruit_INA219.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Definições para o display OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 // Pino de reset do display, se não utilizado, defina como -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define MOCK true
#define DISPLAY false

// Configurações de rede e broker
const char* ssid = "Vini";
const char* password = "senha123";
const char* mqtt_server = "192.168.235.158";

// Objetos WiFi e MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// INA219
Adafruit_INA219 ina219;

// Atualiza o display com os valores
void update_display(float voltage, float current) {
  // Limpa o display
  display.clearDisplay();

  // Configura o texto para exibição
  display.setTextSize(1); // Tamanho do texto
  display.setTextColor(SSD1306_WHITE); // Cor do texto
  display.setCursor(0, 0); // Posição do texto

  // Adiciona as informações no display
  display.print("Voltage: ");
  display.print(voltage, 2); // Mostra com 2 casas decimais
  display.println(" V");

  display.print("Current: ");
  display.print(current, 2);
  display.println(" mA");

  // Atualiza o display
  display.display();
}

// Publica os valores do INA219
void publish_sensor_data() {
  // Leitura de tensão e corrente do INA219
  float bus_voltage = ina219.getBusVoltage_V();
  float current_mA = ina219.getCurrent_mA();

  // Publicação nos tópicos MQTT
  String voltage_str = String(bus_voltage, 2); // 2 casas decimais
  String current_str = String(current_mA, 2);

  client.publish("sensor/voltage", voltage_str.c_str());
  client.publish("sensor/current", current_str.c_str());

  Serial.println("Voltage: " + voltage_str + " V");
  Serial.println("Current: " + current_str + " mA");

  #if DISPLAY
  // Atualiza o display (somente se MOCK estiver desativado)
  update_display(bus_voltage, current_mA);
  #endif
}

// Publica dados mock
void publish_mock_data() {
  // Gera valores de mock
  float mock_voltage = random(460, 500) / 10.0; // Voltagem entre 0.0V e 5.0V
  float mock_current = random(15000, 30000) / 10.0; // Corrente entre 0.0mA e 200.0mA

  // Publica no tópico mock
  String voltage_str = String(mock_voltage, 2);
  String current_str = String(mock_current, 2);

  client.publish("sensor/voltage", voltage_str.c_str());
  client.publish("sensor/current", current_str.c_str());

  Serial.println("Mock Voltage: " + voltage_str + " V");
  Serial.println("Mock Current: " + current_str + " mA");
  
  #if DISPLAY
  // Atualiza o display (somente se MOCK estiver desativado)
  update_display(mock_voltage, mock_current);
  #endif
}

void setup() {
  Serial.begin(9600);

  // Inicializa o display
  #if DISPLAY
  if (!display.begin(SSD1306_I2C_ADDRESS, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    while (true);
  }
  display.clearDisplay();
  display.display();
  #endif

  // Inicializa Wi-Fi e MQTT
  setup_wifi(ssid, password);

  #if !MOCK
  if (!ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) {
      delay(10);
    }
  }
  #endif

  mqtt_init(client, mqtt_server);
}

void loop() {
  // Garante que o cliente MQTT está conectado
  if (!client.connected()) {
    mqtt_init(client, mqtt_server);
  }
  client.loop();

  // Publica dados mock ou do sensor
  #if MOCK
    publish_mock_data();
  #else
    publish_sensor_data();
  #endif

  delay(300); // Publica a cada 1 segundo
}

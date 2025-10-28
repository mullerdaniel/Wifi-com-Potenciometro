// Conectar ao Wi-Fi e ao broker MQTT

#include <WiFi.h>
#include <PubSubClient.h>

// Configurações da rede Wi-Fi
const char* ssid = "Wokwi-GUEST";
const char* password = ""; // Incluir senha na implementação física

// Endereço do broker MQTT
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;                 // Cria o cliente Wi-Fi
PubSubClient client(espClient);      // Cria o cliente MQTT usando o cliente Wi-Fi

// Pinos dos LEDs
const int ledWifi = 2;
const int ledBroker = 15;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA); // Modo estação (cliente)
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi conectado");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Tenta reconectar ao broker MQTT até conseguir
  while (!client.connected()) {
    Serial.print("Tentando conectar ao broker MQTT...");
    if (client.connect("WOKWI_Client")) {
      Serial.println("conectado");
      // Pode incluir inscrição em tópicos ou publicação aqui, se desejar
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

// Liga ou desliga o LED conforme status da conexão Wi-Fi
void wifi_on() {
  digitalWrite(ledWifi, WiFi.status() == WL_CONNECTED ? HIGH : LOW);
}

// Liga ou desliga o LED conforme status da conexão MQTT
void broker_on() {
  digitalWrite(ledBroker, client.connected() ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);

  pinMode(ledWifi, OUTPUT);
  pinMode(ledBroker, OUTPUT);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop(); // Mantém a comunicação MQTT funcionando

  wifi_on();
  broker_on();

  delay(100); // Pequeno atraso para estabilidade
}
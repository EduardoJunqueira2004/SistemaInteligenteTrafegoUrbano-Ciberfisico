#include "esp_timer.h" // Biblioteca nativa para temporizadores do ESP32
#include <WiFi.h>      // Biblioteca para ter ligação ao Wi-Fi
#include <HTTPClient.h>

/*
Trabalho Realizado por: 
Ana Sá nº303 ERSC IPVC
Eduardo Junqueira nº30241 ERSC IPVC
*/

// Definir pinos atuador:
#define LedRed 23     // LED vermelho no semáforo no pino 23 do ESP32
#define LedYellow 22  // LED amarelo no semáforo no pino 22 do ESP32
#define LedGreen 21   // LED verde no semáforo no pino 21 do ESP32
// Definir pinos sensor:
#define sensorTouch 19 // Sensor de toque no pino 19 do ESP32

/** Definir estados do semáforo com enum **/
enum State {
  OFF,    // Todos os LEDs desligados
  RED,    // LED vermelho ligado
  YELLOW, // LED amarelo ligado
  GREEN,  // LED verde ligado
};

// Variáveis globais
bool touchActivated = false;      // Estado do sensor de toque inicial desligado.
State trafficLightState = OFF;    // Estado inicial do semáforo OFF.
volatile bool timerFlag = false;  // Flag para indicar que o temporizador disparou

esp_timer_handle_t periodic_timer; // Identificador do temporizador

// Credenciais Wi-Fi
const char* ssid = "SEU_SSID";//Alterar
const char* password = "SUA_PASSWORD";//Alterar

// URL do servidor
const char* serverURL = "http://172.16.192.157:3000/trafficlight";

// Variáveis para o delay entre cliques
unsigned long lastTouchTime = 0;

// Função para conectar ao Wi-Fi
void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

/** Função para configurar os LEDs do semáforo **/
void controlTrafficLight(State state) {
  // Desligar todos os LEDs
  digitalWrite(LedRed, LOW);
  digitalWrite(LedYellow, LOW);
  digitalWrite(LedGreen, LOW);

  // Configurar LEDs e tempo do temporizador
  switch (state) {
    case OFF:
      Serial.println("Semáforo: OFF");
      break;

    case RED:
      digitalWrite(LedRed, HIGH);
      Serial.println("Semáforo: Vermelho");
      break;

    case YELLOW:
      digitalWrite(LedYellow, HIGH);
      Serial.println("Semáforo: Amarelo");
      break;

    case GREEN:
      digitalWrite(LedGreen, HIGH);
      Serial.println("Semáforo: Verde");
      break;
  }
}

// Função para verificar o sensor de toque
void checkTouchSensor() {
  unsigned long currentTime = millis();
  if (digitalRead(sensorTouch) == HIGH && (currentTime - lastTouchTime > 1000)) {
    lastTouchTime = currentTime;
    Serial.println("Sensor Ativado!");

    // Alterar manualmente o estado do semáforo
    if (trafficLightState == RED) {
      trafficLightState = GREEN;
    } else if (trafficLightState == GREEN) {
      trafficLightState = YELLOW;
    } else if (trafficLightState == YELLOW) {
      trafficLightState = RED;
    } else {
      trafficLightState = RED; // Inicializar no vermelho
    }

    controlTrafficLight(trafficLightState);
    sendTrafficLightState(); // Enviar estado ao servidor
  }
}

// Função para enviar estado do semáforo ao servidor
void sendTrafficLightState() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverURL);
    http.addHeader("Content-Type", "application/json");

    String jsonPayload = "{\"state\":\"";
    if (trafficLightState == RED) jsonPayload += "RED\"}";
    else if (trafficLightState == GREEN) jsonPayload += "GREEN\"}";
    else if (trafficLightState == YELLOW) jsonPayload += "YELLOW\"}";
    else jsonPayload += "OFF\"}";

    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Resposta do servidor: " + response);
    } else {
      Serial.println("Erro ao enviar dados: " + String(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("Wi-Fi desconectado. Não foi possível enviar dados.");
  }
}

// Função para obter comandos do servidor
void getServerCommand() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverURL);

    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Comando recebido: " + response);

      if (response == "RED") trafficLightState = RED;
      else if (response == "YELLOW") trafficLightState = YELLOW;
      else if (response == "GREEN") trafficLightState = GREEN;
      else if (response == "OFF") trafficLightState = OFF;

      controlTrafficLight(trafficLightState);
    } else {
      Serial.println("Erro ao obter comando: " + String(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("Wi-Fi desconectado. Não foi possível obter comando.");
  }
}

/** Setup inicial **/
void setup() {
  Serial.begin(115200);
  Serial.println("Setup concluído!");

  // Configurar pinos
  pinMode(LedRed, OUTPUT);
  pinMode(LedYellow, OUTPUT);
  pinMode(LedGreen, OUTPUT);
  pinMode(sensorTouch, INPUT);

  connectToWiFi();
  controlTrafficLight(OFF); // Iniciar no estado OFF
}

/** Loop principal **/
void loop() {
  checkTouchSensor();
  getServerCommand();
  delay(1000); // Intervalo de 1 segundo
}


/** Loop principal **/
//void loop() {
  // Verificar o sensor de toque
  //checkTouchSensor();

  // Alterar estados automaticamente com base no temporizador
  //if (timerFlag) {
    //timerFlag = false; // Resetar a flag

    // Alternar para o próximo estado
   //if (trafficLightState == RED) {
    //  trafficLightState = GREEN;
    //} else if (trafficLightState == GREEN) {
    //  trafficLightState = YELLOW;
    //} else if (trafficLightState == YELLOW) {
    //  trafficLightState = RED;
    //}

    // Atualizar o semáforo com o novo estado
   // controlTrafficLight(trafficLightState);
  //}
//}


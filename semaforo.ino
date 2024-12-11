#include "esp_timer.h" // Biblioteca nativa para temporizadores do ESP32

/*
Trabalho Realizado por: 
Ana Sá nº303 ERSC IPVC
Eduardo Junqueira nº30241 ERSC IPVC

Trabalho consiste em :
Gerir 1 sensor( sensor de touch);
Gerir 1 atuador(semáforo de 3 cores )
Gerir http via wifi para gerir o nosso html

*/


// Definir pinos atuador:
#define LedRed 23     // LED vermelho no semáforo no pino 23 do ESP32
#define LedYellow 22  // LED amarelo no semáforo no pino 22 do ESP32
#define LedGreen 21   // LED verde no semáforo no pino 21 do ESP32

// Definir pinos sensor:
#define sensorTouch 19 // Sensor de toque no pino 19 do ESP32

/** Definir estados do semáforo com enum **/
enum State {
  OFF,    // Todos os LEDs desligados caso1
  RED,    // LED vermelho ligado. caso2
  YELLOW, // LED amarelo ligado.  caso3
  GREEN,  // LED verde ligado.  caso4
  ON      // Todos os LEDs ligados. caso5
};

// Variáveis globais
bool touchActivated = false;      // Estado do sensor de toque
State trafficLightState = OFF;    // Estado inicial do semáforo
volatile bool timerFlag = false;  // Flag para indicar que o temporizador disparou

esp_timer_handle_t periodic_timer; // Identificador do temporizador

/** Função chamada quando o temporizador dispara **/
void IRAM_ATTR onTimer(void *param) {
  timerFlag = true; // Ativar flag para indicar que o temporizador disparou
}

/** Função para configurar o estado do semáforo **/
void controlTrafficLight(State state) {
  // Desligar todos os LEDs
  digitalWrite(LedRed, LOW);
  digitalWrite(LedYellow, LOW);
  digitalWrite(LedGreen, LOW);

  // Configurar LEDs e tempo do temporizador
  switch (state) {
    case OFF:
      Serial.println("Semáforo: OFF");
       esp_timer_stop(periodic_timer); // Parar temporizador anterior
       esp_timer_start_periodic(periodic_timer, 4 * 1000000); // Configurar para 4 segundos
      break;
     

    case ON:
      digitalWrite(LedRed, HIGH);
      digitalWrite(LedYellow, HIGH);
      digitalWrite(LedGreen, HIGH);
      Serial.println("Semáforo: ON");
       esp_timer_stop(periodic_timer); // Parar temporizador anterior
      esp_timer_start_periodic(periodic_timer, 1 * 1000000); // Configurar para 1 segundo
      break;

    case RED:
      digitalWrite(LedRed, HIGH);
      Serial.println("Semáforo: Vermelho");
      // Atualizar temporizador para 10 segundos
      esp_timer_stop(periodic_timer); // Parar temporizador anterior
      esp_timer_start_periodic(periodic_timer, 15 * 1000000); // Configurar para 10 segundos
      break;

    case YELLOW:
      digitalWrite(LedYellow, HIGH);
      Serial.println("Semáforo: Amarelo");

      // Atualizar temporizador para 2 segundos
      esp_timer_stop(periodic_timer);
      esp_timer_start_periodic(periodic_timer, 5 * 1000000); // Configurar para 2 segundos
      break;

    case GREEN:
      digitalWrite(LedGreen, HIGH);
      Serial.println("Semáforo: Verde");

      // Atualizar temporizador para 5 segundos
      esp_timer_stop(periodic_timer);
      esp_timer_start_periodic(periodic_timer, 15 * 1000000); // Configurar para 5 segundos
      break;
  }
}

/** Função para verificar o sensor de toque **/
void checkTouchSensor() {
  if (digitalRead(sensorTouch) == HIGH) {
    if (!touchActivated) {
      touchActivated = true;
      Serial.println("Sensor Ativado!");

      // Alterar manualmente o estado do semáforo
      trafficLightState = static_cast<State>((trafficLightState + 1) % 5);
      controlTrafficLight(trafficLightState);
    }
  } else {
    touchActivated = false;
  }
}

/** Setup inicial **/
void setup() {
  // Configurar pinos
  pinMode(LedRed, OUTPUT);
  pinMode(LedYellow, OUTPUT);
  pinMode(LedGreen, OUTPUT);
  pinMode(sensorTouch, INPUT);

  // Inicializar comunicação serial
  Serial.begin(115200);
  Serial.println("Setup concluído!");

  // Configurar o temporizador
  esp_timer_create_args_t timer_args = {
      .callback = &onTimer, // Função de callback
      .arg = NULL,
      .dispatch_method = ESP_TIMER_TASK,
      .name = "periodicTimer"};
  esp_timer_create(&timer_args, &periodic_timer); // Criar o temporizador

  // Iniciar semáforo no estado OFF
  controlTrafficLight(OFF);
}

/** Loop principal **/
void loop() {
  // Verificar o sensor de toque
  checkTouchSensor();

  // Alterar estados automaticamente com base no temporizador
  if (timerFlag) {
    timerFlag = false; // Resetar a flag

    // Alternar para o próximo estado
    if (trafficLightState == RED) {
      trafficLightState = GREEN;
    } else if (trafficLightState == GREEN) {
      trafficLightState = YELLOW;
    } else if (trafficLightState == YELLOW) {
      trafficLightState = RED;
    }

    // Atualizar o semáforo com o novo estado
    controlTrafficLight(trafficLightState);
  }
}

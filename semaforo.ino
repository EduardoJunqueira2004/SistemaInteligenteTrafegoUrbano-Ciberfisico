// Definir pinos atuador:
#define LedRed 23     // LED vermelho no semáforo ​
#define LedYellow 22  // LED amarelo no semáforo ​
#define LedGreen 21   // LED verde no semáforo

// Definir pinos sensor:
#define sensorTouch 19 // Sensor de toque​

// Estes podem vir a ser usados mais adiante mas neste momento não são necessários.
//#define sensorPin 34 // Sensor de temperatura​
//#define ledPin 23 // Pino do LED para MQTT Pull ​

/** Definir estados do semáforo com enum **/
enum State {
  OFF,    // Todos os LEDs desligados vou usar
  RED,    // LED vermelho ligado vou usar 
  YELLOW, // LED amarelo ligado vou usar 
  GREEN,  // LED verde ligado vou usar 
  ON      // Todos os LEDs ligados ainda não sei em que situação vou usar
};

// Definir variáveis globais
bool touchActivated = false; // Estado do sensor de toque passa para true quando eu quiser que passe
State trafficLightState = OFF; // Estado inicial do Semáforo

// Definir flag timer1
bool timerFlag = false; // Flag para indicar quando o timer dispara

/** Definir função semáforo de estado **/
void controlTrafficLight(State state) {
  // Desligar todos os LEDs
  digitalWrite(LedRed, LOW);
  digitalWrite(LedYellow, LOW);
  digitalWrite(LedGreen, LOW);

  // Ligar o LED correspondente ao estado fazer Switch Case temos 5 estados apenas por enquanto nesta ordem:
  switch (state) {
    // State OFF:
    case OFF:
      digitalWrite(LedRed, LOW);
      digitalWrite(LedYellow, LOW);
      digitalWrite(LedGreen, LOW);
      Serial.println("Semáforo: OFF");
      break;

    // State ON:
    case ON:
      digitalWrite(LedRed, HIGH);
      digitalWrite(LedYellow, HIGH);
      digitalWrite(LedGreen, HIGH);
      Serial.println("Semáforo: ON");
      break;

    // State RED:
    case RED:
      digitalWrite(LedRed, HIGH);
      Serial.println("Semáforo: Vermelho");
      break;

    // State YELLOW:
    case YELLOW:
      digitalWrite(LedYellow, HIGH);
      Serial.println("Semáforo: Amarelo");
      break;

    // State GREEN:
    case GREEN:
      digitalWrite(LedGreen, HIGH);
      Serial.println("Semáforo: Verde");
      break;
  }
}

/** Definir função sensor de touch para verificar o sensor de toque **/
void checkTouchSensor() {
  // Leitura do sensor de toque:
  if (digitalRead(sensorTouch) == HIGH) { // Se o sensor de toque for detetado como HIGH, significa que foi tocado

    // Apenas processa no primeiro toque, mesmo que a pessoa faça um toque extenso ou múltiplos rápidos
    if (!touchActivated) { // Verifica se a variável touchActivated foi ativada
      touchActivated = true; // Caso não tenha sido ativada ele ativa
      Serial.println("Sensor Ativado!");

      // Altera o estado do semáforo
      trafficLightState = static_cast<State>((trafficLightState + 1) % 5); // Altera entre os 5 estados definidos no enum
      controlTrafficLight(trafficLightState); // Controla e atualiza o estado do semáforo
    }
  } else {
    touchActivated = false; // Reset do estado quando o toque for deixado
  }
}

/** Função Setup de início do código **/
void setup() {
  // Definir pinos como OUTPUT de saída para os LEDs
  pinMode(LedRed, OUTPUT);
  pinMode(LedYellow, OUTPUT);
  pinMode(LedGreen, OUTPUT);

  // Definir pinos como INPUT para o sensor de toque
  pinMode(sensorTouch, INPUT);

  // Inicializar comunicação serial
  Serial.begin(115200);
  Serial.println("In setup function!");

  // Chamar funções:
  controlTrafficLight(OFF); // Função de controlo do Semáforo começa com o estado OFF
}

/** Função Loop de controlo do código **/
void loop() {
  // Verificação constante do sensor de toque no loop
  checkTouchSensor(); // Função de controlo do toque com base nos estados
}

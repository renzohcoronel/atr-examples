/* Aplicaciones en tiempo real - FRLP
  Ejemplo: 02
  Circuito simulado: https://www.tinkercad.com/things/ffl84zS9Lln-threads/editel?sharecode=2TBQMTCOTI6vwOd5Vc-v3kFIP0pLY9kaTZ8JEdZDrDg
*/

#define LED_1_PIN 10
#define LED_2_PIN 11
#define LED_3_PIN 12
#define PIN_BUTTON_1 9

unsigned long previousTimeLed1 = 0;
unsigned long previousTimeLed2 = 0;
unsigned long TIME_INTERVAL_1 = 1000;
unsigned long TIME_INTERVAL_2 = 2000;

void setup() {
  Serial.begin(9600);
  
  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);
  pinMode(LED_3_PIN, OUTPUT);
  pinMode(PIN_BUTTON_1, INPUT);
}

void loop() {
    unsigned long currentTime = millis();
    // Task 1 - Intermitent led
    if(currentTime - previousTimeLed1 > TIME_INTERVAL_1) {  
      Serial.println("Task1");
      previousTimeLed1 = currentTime;
      int currentValueLed1 = digitalRead(LED_1_PIN);
      currentValueLed1 == HIGH ? digitalWrite(LED_1_PIN, LOW): digitalWrite(LED_1_PIN, HIGH);
    }
    

    // Task 2 - Intermitent led
    if(currentTime - previousTimeLed2 > TIME_INTERVAL_2) {  
      Serial.println("Task2");
      previousTimeLed2 = currentTime;
      int currentValueLed2 = digitalRead(LED_2_PIN);
      currentValueLed2 == HIGH ? digitalWrite(LED_2_PIN, LOW): digitalWrite(LED_2_PIN, HIGH);
      
    }

    // Task 3 - PUSH Button
    Serial.println("Task3");
    int buttonValue = digitalRead(PIN_BUTTON_1);
    digitalWrite(LED_3_PIN, buttonValue);
}

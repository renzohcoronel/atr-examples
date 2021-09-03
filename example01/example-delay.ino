/* Aplicaciones en tiempo real - FRLP
  Ejemplo: 01
  Circuito simulado: https://www.tinkercad.com/things/ffl84zS9Lln-threads/editel?sharecode=2TBQMTCOTI6vwOd5Vc-v3kFIP0pLY9kaTZ8JEdZDrDg
*/

#define LED_1_PIN 10
#define LED_2_PIN 11
#define LED_3_PIN 12

void setup() {
  Serial.begin(9600);
  
  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);
  pinMode(LED_3_PIN, OUTPUT);
}

void loop() {
    
    // Task 1 - Intermitent led
    Serial.println("Task1");
    int currentValueLed1 = digitalRead(LED_1_PIN);
    currentValueLed1 == HIGH ? digitalWrite(LED_1_PIN, LOW): digitalWrite(LED_1_PIN, HIGH);
    delay(2000);

    // Task 2 - Intermitent led
     Serial.println("Task2");
    int currentValueLed2 = digitalRead(LED_2_PIN);
    currentValueLed2 == HIGH ? digitalWrite(LED_2_PIN, LOW): digitalWrite(LED_2_PIN, HIGH);
    delay(1000);

    // Task 3 - Read value from serial port
    if (Serial.available()) {
         Serial.println("Task3");
        int userInput = Serial.parseInt();
        if (userInput >= 0 && userInput < 2) {
            digitalWrite(LED_3_PIN, userInput);
        }
  }
}

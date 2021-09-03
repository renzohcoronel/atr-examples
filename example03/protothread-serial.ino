/* Aplicaciones en tiempo real - FRLP
  Ejemplo: 03
  Circuito simulado: https://www.tinkercad.com/things/ffl84zS9Lln-threads/editel?sharecode=2TBQMTCOTI6vwOd5Vc-v3kFIP0pLY9kaTZ8JEdZDrDg
*/

#include <pt.h>
// Crear tus protothread(s)
#define LED_1_PIN 10
#define LED_2_PIN 11
#define LED_3_PIN 12

unsigned int TIME_INTERVAL_1 = 1000;
unsigned int TIME_INTERVAL_2 = 2000;

static struct pt ptHumedity, ptTemperature, ptDH11; // ref:. https://www.programiz.com/c-programming/c-structures

// Funcion que ejecutara el thread.
static int protothreadBlinkLed1(struct pt *pt, int PIN_ID, int INTERVAL)
{
    static unsigned long timestap = 0;
    // Arrancar el protothread
    PT_BEGIN(pt);
    while (1)
    {

        timestap = millis();
        PT_WAIT_UNTIL(pt, (millis() - timestap) > INTERVAL);

        int currentValueLed1 = digitalRead(PIN_ID);
        currentValueLed1 == HIGH ? digitalWrite(PIN_ID, LOW) : digitalWrite(PIN_ID, HIGH);
    }
    PT_END(pt);
}

static int protothreadBlinkLed2(struct pt *pt, int PIN_ID, int INTERVAL)
{
    static unsigned long timestap = 0;
    // Arrancar el protothread
    PT_BEGIN(pt);
    while (1)
    {

        timestap = millis();
        PT_WAIT_UNTIL(pt, (millis() - timestap) > INTERVAL);

        int currentValueLed1 = digitalRead(PIN_ID);
        currentValueLed1 == HIGH ? digitalWrite(PIN_ID, LOW) : digitalWrite(PIN_ID, HIGH);
    }
    PT_END(pt);
}

static int protothreadSerialRead(struct pt *pt, int PIN_ID)
{
    static int userInput = Serial.parseInt();
    // Arrancar el protothread
    PT_BEGIN(pt);
    while (1)
    {

        PT_WAIT_UNTIL(pt, Serial.available() > 0);
        userInput = Serial.parseInt();
        switch (userInput)
        {
        case 1:
            digitalWrite(LED_3_PIN, HIGH);
            break;
        case 2:
            digitalWrite(LED_3_PIN, LOW);
            break;
        default:
            break;
        }
    }
    PT_END(pt);
}

void setup()
{
    // Se debe iniciar el thread
    Serial.begin(9600);
    pinMode(LED_1_PIN, OUTPUT);
    pinMode(LED_2_PIN, OUTPUT);
    pinMode(LED_3_PIN, OUTPUT);
    PT_INIT(&ptHumedity);
    PT_INIT(&ptTemperature);
    PT_INIT(&ptDH11);
}

void loop()
{
    // Ejecutar la funcion del thread
    protothreadBlinkLed1(&ptHumedity, LED_1_PIN, TIME_INTERVAL_1); // Siempre se pasa el parametro por referencia
    protothreadBlinkLed2(&ptTemperature, LED_2_PIN, TIME_INTERVAL_2);
    protothreadSerialRead(&ptDH11, LED_2_PIN);
}
/* Aplicaciones en tiempo real - FRLP
  Ejemplo: 03 DTH11
  Circuito simulado: https://www.tinkercad.com/things/ffl84zS9Lln-threads/editel?sharecode=2TBQMTCOTI6vwOd5Vc-v3kFIP0pLY9kaTZ8JEdZDrDg
*/

#include <pt.h>
// #include <DHT.h>

#define LED_1_PIN 10 // humedity actuator
#define LED_2_PIN 11 // temperature actuator
#define DHTPIN 12
// #define DHTTYPE DHT11

// DHT11 Mock
class DHT11Mock
{
public:
    float h = 0;
    float t = 0;

public:
    DHT11Mock() {}
    float readHumidity()
    {
        return this->h;
    }

    float readTemperature()
    {
        return this->t;
    }

    void begin(){};

    void update(String read)
    {
    
        this->h = this->getValue(read, ',', 0).toFloat();
        this->t = this->getValue(read, ',', 1).toFloat();
    }

    String getValue(String data, char separator, int index)
    {
        int found = 0;
        int strIndex[] = {0, -1};
        int maxIndex = data.length() - 1;

        for (int i = 0; i <= maxIndex && found <= index; i++)
        {
            if (data.charAt(i) == separator || i == maxIndex)
            {
                found++;
                strIndex[0] = strIndex[1] + 1;
                strIndex[1] = (i == maxIndex) ? i + 1 : i;
            }
        }

        return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
    };
};

//DHT dht(DHTPIN, DHTTYPE);
DHT11Mock *dht = new DHT11Mock();

unsigned int TIME_SENSOR = 5000;
static struct pt ptHumedity, ptTemperature, ptDH11; // ref:. https://www.programiz.com/c-programming/c-structures

static float humedityValue = 60.0f;
static float deltaHumedity = 2.0f;
bool activateHumedity = false;

static float temperatureValue = 24.0f;
static float deltaTemperature = 2.0f;
bool activateTemp = false;

static int threadTemperature(struct pt *pt, int PIN_ID, bool activate)
{
    static unsigned long timestap = 0;
    PT_BEGIN(pt);
    while (true)
    {
        timestap = millis();
        PT_WAIT_UNTIL(pt, activate && (millis() - timestap) > 500);
        timestap = millis();
        int currentValueLed1 = digitalRead(PIN_ID);
        currentValueLed1 == HIGH ? digitalWrite(PIN_ID, LOW) : digitalWrite(PIN_ID, HIGH);
    
    }

    PT_END(pt);
}

static int threadHumedity(struct pt *pt, int PIN_ID, bool activate)
{
    static unsigned long timestap = 0;
    PT_BEGIN(pt);
    while (true)
    {
        timestap = millis();
        PT_WAIT_UNTIL(pt, activate && (millis() - timestap) > 500);
        timestap = millis();
        int currentValueLed1 = digitalRead(PIN_ID);
        currentValueLed1 == HIGH ? digitalWrite(PIN_ID, LOW) : digitalWrite(PIN_ID, HIGH);

        /* code */
    }

    PT_END(pt);
}

static int threadSensorDTH11(struct pt *pt, bool *activateH, bool *activateT)
{

    // Arrancar el protothread
    PT_BEGIN(pt);
    while (1)
    {
        PT_WAIT_UNTIL(pt, Serial.available() > 0);
        Serial.println("Leer datos");
        String read = Serial.readString();
        dht->update(read);
        *activateH = checkValue(dht->readHumidity(), deltaHumedity, humedityValue);
        *activateT = checkValue( dht->readTemperature(), deltaTemperature, temperatureValue);
        Serial.println("Humedity:" + String(dht->h) + " Status:" + activateHumedity);
        Serial.println("Temperature:" + String(dht->t) + " Status:" + activateTemp);
        Serial.flush();
    }
    PT_END(pt);
}

static bool checkValue(float current, float delta, float value)
{
    Serial.println(String(value - delta) + " >= " + String(current) + "<=" + String(value + delta));
    if ((current >= (value - delta) && current <= (value + delta)))
    {
        return false;
    }
    return true;
}

void setup()
{
    // Se debe iniciar el thread
    Serial.begin(9600);

    pinMode(LED_1_PIN, OUTPUT);
    digitalWrite(LED_1_PIN, LOW);

    pinMode(LED_2_PIN, OUTPUT);
    digitalWrite(LED_2_PIN, LOW);

    dht->begin();

    PT_INIT(&ptHumedity);
    PT_INIT(&ptTemperature);
    PT_INIT(&ptDH11);
}

void loop()
{
    // Ejecutar la funcion del thread

    threadSensorDTH11(&ptDH11, &activateHumedity, &activateTemp);
    threadTemperature(&ptTemperature, LED_1_PIN, activateTemp); // Siempre se pasa el parametro por referencia
    threadHumedity(&ptHumedity, LED_2_PIN, activateHumedity);
}
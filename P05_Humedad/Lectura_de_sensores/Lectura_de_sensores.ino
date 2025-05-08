#include <Adafruit_AHTX0.h> //libreria del sensor 
//declaracion de pines analogicos para lectura de sensores de temperatura 
#define PIN_LM35 A0
#define PIN_TERMISTOR A1

const float BETA = 3950; // Coeficiente beta del termistor
const float SERIES_RESISTOR = 10000; // Valor de la resistencia en el divisor de voltaje
const float NOMINAL_RESISTANCE = 10000; // Resistencia del termistor a 25°C
const float NOMINAL_TEMPERATURE = 25.0; // Temperatura nominal (°C)

Adafruit_AHTX0 aht;

void setup() {
  Serial.begin(9600);//inicializacion de comunicacion serial

//inicializacion de sensor 
  if (!aht.begin()) {
    Serial.println("¡No se encontró el sensor AHT20!");
    while (1) delay(10);
  }

  Serial.println("Iniciando lectura de sensores...");
}

void loop() {
  // === LM35 ===
  int lm35Raw = analogRead(PIN_LM35);//declaracion de lectura en pin analogico 
  float voltageLM35 = lm35Raw * (5.0 / 1023.0); //acondicionamiento de variable deacuerdo con resolucion del adc y voltaje
  float tempLM35 = voltageLM35 * 100; // 10mV por grado

  // === Termistor ===
  int rawTermistor = analogRead(PIN_TERMISTOR); //declaracion de lectura en pin analogico 
  float voltage = rawTermistor * (5.0 / 1023.0);//acondicionamiento de variable deacuerdo con resolucion del adc y voltaje
  float resistance = (5.0 - voltage) * SERIES_RESISTOR / voltage;

  float steinhart;
  steinhart = resistance / NOMINAL_RESISTANCE;           // (R/Ro)
  steinhart = log(steinhart);                            // ln(R/Ro)
  steinhart /= BETA;                                     // 1/B * ln(R/Ro)
  steinhart += 1.0 / (NOMINAL_TEMPERATURE + 273.15);     // + (1/To)
  steinhart = 1.0 / steinhart;                           // Invertir
  float tempTermistor = -1*(steinhart - 273.15);              // K a °C

  // === AHT20 ===
  sensors_event_t humidity, tempAHT;
  aht.getEvent(&humidity, &tempAHT);

  // === Imprimir resultados ===
  Serial.println("---------");
  Serial.print("Temperatura LM35: ");
  Serial.print(tempLM35);
  Serial.println(" °C");

  Serial.print("Temperatura Termistor: ");
  Serial.print(tempTermistor);
  Serial.println(" °C");

  Serial.print("Temperatura AHT20: ");
  Serial.print(tempAHT.temperature);
  Serial.println(" °C");

  Serial.print("Humedad AHT20: ");
  Serial.print(humidity.relative_humidity);
  Serial.println(" %");

  delay(2000);
}
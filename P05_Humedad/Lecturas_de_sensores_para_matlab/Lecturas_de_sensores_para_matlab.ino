#include <Adafruit_AHTX0.h> //libreria sensor 

#define PIN_LM35 A0 //declaracion pines analogicos 
#define PIN_TERMISTOR A1//declaracion piines analogicos 

const float BETA = 3950; // Coeficiente beta del termistor
const float SERIES_RESISTOR = 10000.0; // Resistencia del divisor (Ohmios)
const float NOMINAL_RESISTANCE = 10000.0;
const float NOMINAL_TEMPERATURE = 25.0; // °C

Adafruit_AHTX0 aht;
//inizializacion del sensor 
void setup() {
  Serial.begin(9600);
  while (!Serial); // Esperar a conexión USB en placas como Leonardo

  if (!aht.begin()) {
    Serial.println("No se encontró el sensor AHT20");
    while (1) delay(10);
  }
}

void loop() {
  // === LM35 ===
  int lm35Raw = analogRead(PIN_LM35);//lectura del pin analogico 
  float voltageLM35 = lm35Raw * (5.0 / 1023.0); //acondicionamiento de variable deacuerdo al voltaje y resolucion del adc 
  float tempLM35 = voltageLM35 * 100.0; // °C

  // === Termistor ===
  int rawTermistor = analogRead(PIN_TERMISTOR);//lectura del pin analogico 
  float voltage = rawTermistor * (5.0 / 1023.0);//acondicionamiento de variable deacuerdo al voltaje y resolucion del adc
  float resistance = (5.0 - voltage) * SERIES_RESISTOR / voltage;
//calculo termistor 
  float steinhart = resistance / NOMINAL_RESISTANCE;
  steinhart = log(steinhart);
  steinhart /= BETA;
  steinhart += 1.0 / (NOMINAL_TEMPERATURE + 273.15);
  steinhart = 1.0 / steinhart;
  float tempTermistor = -1*(steinhart - 273.15);

  // === AHT20 ===
  sensors_event_t humidity, tempAHT;
  aht.getEvent(&humidity, &tempAHT);

  // === Enviar por Serial ===
  Serial.print("H:");
  Serial.print(humidity.relative_humidity, 2);
  Serial.print(" T_LM35:");
  Serial.print(tempLM35, 2);
  Serial.print(" T_TERM:");
  Serial.print(tempTermistor, 2);
  Serial.print(" T_AHT:");
  Serial.println(tempAHT.temperature, 2);

  delay(1000); // 1 segundo entre muestras
}
// Definiciones
#define RL 5.0       // Resistencia de carga en kilo-ohms
#define RO_CLEAN_AIR 9.83 // Resistencia del sensor en aire limpio (ajustar según calibración)

// Pines
const int mq2_pin = A0; // Pin analógico conectado al sensor

// Variables para cálculos
float Ro = 172.38; // Resistencia en aire limpio se planteo por medio de una calibracion previa 

void setup() {
  Serial.begin(9600); // se inicia comunicacion UART
  delay(1000);
}

void loop() {
  // Leer valor analógico
  int sensorValue = analogRead(mq2_pin);
  
  // Calcular Rs (resistencia del sensor)
  float Rs = ((1023.0 / sensorValue) - 1) * RL;
  
  // Calcular concentraciones para cada gas
  float co_ppm = getCO(Rs);
  float ethanol_ppm = getEthanol(Rs);
  float h2_ppm = getH2(Rs);
  float methane_ppm = getMethane(Rs);
  
  // Mostrar resultados
  Serial.println("Concentraciones detectadas:");
  Serial.print("CO: "); Serial.print(co_ppm); Serial.println(" ppm");
  Serial.print("Etanol: "); Serial.print(ethanol_ppm); Serial.println(" ppm");
  Serial.print("H2: "); Serial.print(h2_ppm); Serial.println(" ppm");
  Serial.print("Metano: "); Serial.print(methane_ppm); Serial.println(" ppm");
  Serial.println("---------------------------------------");
  
  delay(2000); // Esperar 2 segundos entre lecturas
}

// Funciones para calcular concentraciones específicas (basadas en curvas características)
float getCO(float Rs) {
  // Curva característica para CO: Rs/Ro = a * ppm^b
  float a = 605.18, b = -3.937; // Parámetros de la curva (ajustar según datasheet)
  float ratio = Rs / Ro;
  return pow((ratio / a), (1 / b));
}

float getEthanol(float Rs) {
  // Curva característica para Etanol
  float a = 75.31, b = -3.35;
  float ratio = Rs / Ro;
  return pow((ratio / a), (1 / b));
}

float getH2(float Rs) {
  // Curva característica para H2
  float a = 987.99, b = -4.52;
  float ratio = Rs / Ro;
  return pow((ratio / a), (1 / b));
}

float getMethane(float Rs) {
  // Curva característica para Metano
  float a = 110.47, b = -2.56;
  float ratio = Rs / Ro;
  return pow((ratio / a), (1 / b));
}
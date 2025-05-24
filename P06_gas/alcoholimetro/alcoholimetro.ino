const int MQ_PIN = A0;  // Pin analógico para el sensor MQ-2

// Valores de calibración (ajustar según necesidad)
const float RL = 171.38;   // Resistencia de carga en kilo-ohms
const float R0 = 10.0;   // Resistencia del sensor en aire limpio (ajustar después de calibración)

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando alcoholímetro MQ-2...");
}

void loop() {
  // Leer valor del sensor
  float sensorValue = analogRead(MQ_PIN);
  
  // Convertir a voltaje
  float voltage = sensorValue * (5.0 / 1023.0);
  
  // Calcular resistencia del sensor (Rs)
  float Rs = RL * (5.0 - voltage) / voltage;
  
  // Calcular relación Rs/R0
  float ratio = Rs / R0;
  
  // Estimación de concentración de alcohol en ppm (partes por millón)
  float alcohol_ppm = 1000 * pow(ratio / 60.0, -2.5);
  
  // Estimación aproximada de BAC (Blood Alcohol Content)
  // NOTA: Esta es una conversión ilustrativa, no precisa
  float bac = map(alcohol_ppm, 0, 500, 0, 30) / 100.0;
  bac = constrain(bac, 0.0, 0.30); // Limitar a rango razonable
  
  // Mostrar resultados en el monitor serial
  Serial.print("Valor crudo: ");
  Serial.print(sensorValue);
  Serial.print(" | Voltaje: ");
  Serial.print(voltage, 2);
  Serial.print("V");
  Serial.print(" | Alcohol ppm: ");
  Serial.print(alcohol_ppm, 1);
  Serial.print(" | BAC estimado: ");
  Serial.print(bac, 3);
  Serial.println("%");
  
  // Evaluación básica del resultado
  if (bac > 0.08) {
    Serial.println("¡ADVERTENCIA! Nivel sobre el límite legal ");
  } else if (bac > 0.03) {
    Serial.println("Nivel detectable de alcohol");
  } else {
    Serial.println("Nivel bajo o no detectable");
  }
  Serial.println("--------------------------");
  
  delay(2000); // Esperar 2 segundos entre mediciones
}
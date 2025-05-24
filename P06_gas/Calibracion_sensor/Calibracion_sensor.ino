#define MQ2_PIN A0   // Pin conectado al sensor
#define RL 5       // kΩ - resistencia de carga (verifica si es 5k en tu módulo)
#define NUM_SAMPLES 50 

float readRs() {
  float rs = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    int adcValue = analogRead(MQ2_PIN); // lee el valor del adc 
    float vout = adcValue * (5.0 / 1023.0); //transforma el valor deacuerdo a la resolucion y al voltaje de entrada 
    float rs_sample = ((5.0 - vout) / vout) * RL; //da el valor de muestra 
    rs += rs_sample; 
    delay(100);
  }
  return rs / NUM_SAMPLES;  // promedio de muestras
}

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando calibración del MQ-2 (aire limpio)...");

  float Ro = readRs();  // Se asume aire limpio
  Serial.print("Ro estimado en aire limpio: ");
  Serial.print(Ro);
  Serial.println(" kΩ");
}

void loop() {
  // No es necesario usar loop para calibrar, puede quedar vacío
}

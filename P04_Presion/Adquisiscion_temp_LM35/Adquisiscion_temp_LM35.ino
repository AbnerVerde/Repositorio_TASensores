const int sensorPin = A0;  // Pin analógico conectado al LM35

void setup() {
  Serial.begin(9600);  // Inicia comunicación serial
}

void loop() {
  int sensorValue = analogRead(sensorPin);  // Lee el valor del sensor (0-1023)
  float voltage = (sensorValue / 1023.0) * 5.0;  // Convierte a voltaje (0-5V)
  float temperature = voltage * 100.0;  // Convierte a temperatura (10mV/°C)
  //lee la temperatura 
  Serial.println(temperature);

  delay(1000);  // Espera 1 segundo antes de la próxima lectura
}
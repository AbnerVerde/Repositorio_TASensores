#include <Wire.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(9600);
  if (!bmp.begin()) {
    Serial.println("No se encontró el sensor BMP180, verifica la conexión!");
    while (1) {}
  }
}

void loop() {
  float temperatura = bmp.readTemperature();
  float presion = bmp.readPressure() / 100.0; // Convertir a hPa
  
  Serial.print("Temperatura: ");
  Serial.println(temperatura, 2);
  
  
  delay(1800000); // Espera 30 min entre lecturas
}
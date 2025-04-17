#include <Wire.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(9600);
  if (!bmp.begin()) {
    Serial.println("Error: BMP180 no detectado");
    while (1);
  }
}

void loop() {
  float temperatura = bmp.readTemperature();
  Serial.println(temperatura, 2);  // Envía solo temperatura (ej: "23.45")
  delay(1000);  // 1 muestra por segundo (1 Hz)
}
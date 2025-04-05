#include <SoftwareSerial.h>
#include <LiquidCrystal.h> // LCD sin I2C

// Configuración del Bluetooth (HC-06)
SoftwareSerial bluetooth(2, 3); // RX, TX

// Configuración de la LCD (pines RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(4, 5, 6, 7, 8, 9);

String mensaje; // Almacena el mensaje recibido

void setup() {
 // Serial.begin(9600);  
  bluetooth.begin(9600);
  lcd.begin(16, 2); // LCD de 16x2
  lcd.print("Esperando datos...");
}

void loop() {
  if (bluetooth.available()) {
    mensaje = bluetooth.readStringUntil('\n');
    mensaje.trim(); // Eliminar espacios o saltos de línea
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Posicion:");
    lcd.setCursor(0, 1);
    lcd.print(mensaje);
  }
  delay(200);
}
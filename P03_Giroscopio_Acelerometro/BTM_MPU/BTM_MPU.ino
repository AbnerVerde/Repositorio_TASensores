#include <Wire.h>
#include <SoftwareSerial.h>

// Configuración Bluetooth (HC-05)
SoftwareSerial bluetooth(2, 3); // RX, TX

// Dirección I2C del MPU6050
const int MPU_ADDR = 0x68;

// Variables para el sensor
int16_t AcX, AcY, AcZ;

// Umbral de detección (ajustable)
const float UMBRAL = 0.7;

void setup() {
  Serial.begin(19200); //iniciamos comunicacion serial a velocidad distinta a la del buetooth
  bluetooth.begin(9600); //iniciamos comunicacion por bluetooth 
  Wire.begin(); //inicializamos comunicacion I2C
  // Inicializar MPU6050
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); // PWR_MGMT_1
  Wire.write(0);    // Encender el sensor
  Wire.endTransmission(true);
  delay(100);
}

void loop() {
  leerMPU6050(); //funcion para leer datos del MPU
  int mensaje = detectarPosicion(); // Devuelve 1-6
  bluetooth.println(mensaje); // Envía el número y un salto de lines por Bluetooth
  Serial.println(mensaje); //Imprimimos el mensaje en la consola con un salto de lina 
  delay(200);
}

void leerMPU6050() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // Registro ACCEL_XOUT_H
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true);
  AcX = Wire.read() << 8 | Wire.read();//lee dos datos del acelerometro para el eje x 
  AcY = Wire.read() << 8 | Wire.read();//lee dos datos del acelerometro para el eje y
  AcZ = Wire.read() << 8 | Wire.read();//lee dos datos del acelerometro para el eje z
}

int detectarPosicion() {
  float accX = AcX / 16384.0;
  float accY = AcY / 16384.0;
  float accZ = AcZ / 16384.0;

  // Asignar números según la orientación
  if (accZ > UMBRAL)      return 1; // ARRIBA
  else if (accZ < -UMBRAL) return 2; // ABAJO
  else if (accX > UMBRAL)  return 3; // DERECHA
  else if (accX < -UMBRAL) return 4; // IZQUIERDA
  else if (accY > UMBRAL)  return 5; // ADELANTE
  else if (accY < -UMBRAL) return 6; // ATRÁS
  else                     return 0; // DESCONOCIDO
}
//libreria para trabajar con i2c
#include <Wire.h>

// Dirección I2C del MPU6050 (0x68 por defecto)
const int MPU_ADDR = 0x68;

// Variables para almacenar los datos del sensor
int16_t AcX, AcY, AcZ;  // Acelerómetro
int16_t GyX, GyY, GyZ;  // Giroscopio
int16_t temp;           // Temperatura (opcional)
const int m=0;          //masa del objeto

// Variables para Pitch y Roll
float pitch, roll;

void setup() {
  Serial.begin(9600);     //iniciar comunicacion UART
  Wire.begin();          // Iniciar comunicación I2C
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);      // Registro PWR_MGMT_1
  Wire.write(0);         // Despertar al MPU6050
  Wire.endTransmission(true);
  delay(100);
}

void loop() {
  // Leer datos del MPU6050
  leerMPU6050();
  //obtencion de fuerza por medio de ley de newton F=m*a donde a es el modulo de las aceleraciones por los 3 ejes 
  int F= m*sqrt(AcX^2 + AcY^2 + AcZ^2);
  // Mostrar resultados
  Serial.print("Fuerza resultante ");
  Serial.print(F);
  //Serial.print("° | Roll: ");
  //Serial.print(roll);
  //Serial.println("°");

  delay(500);  // Esperar medio segundo
}

void leerMPU6050() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);  // Empezar a leer desde el registro 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true);  // Leer 14 bytes (acelerómetro, temperatura, giroscopio)

  // Leer acelerómetro (2 bytes por eje)
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();

  // Leer temperatura (opcional, no se usa en este ejemplo)
  temp = Wire.read() << 8 | Wire.read();

  // Leer giroscopio (2 bytes por eje)
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();
}

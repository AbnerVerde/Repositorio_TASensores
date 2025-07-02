#include <Wire.h>

// Direcciones I2C de los MPU6050
const int MPU_ADDR_1 = 0x68;  // Primer sensor
const int MPU_ADDR_2 = 0x69;  // Segundo sensor

// Variables para almacenar los datos de los sensores
int16_t AcX1, AcY1, AcZ1, AcX2, AcY2, AcZ2;  // Acelerómetros
int16_t GyX1, GyY1, GyZ1, GyX2, GyY2, GyZ2;  // Giroscopios

// Variables para Pitch y Roll de ambos sensores (ahora como enteros)
int pitch1, roll1, pitch2, roll2;
int f0,f1,f2,f3,f4;
void setup() {
  Serial.begin(9600);     // Iniciar comunicación serial 
  Wire.begin();           // Iniciar comunicación I2C
  
  // Configurar primer sensor (0x68)
  Wire.beginTransmission(MPU_ADDR_1);
  Wire.write(0x6B);       // Registro PWR_MGMT_1
  Wire.write(0);          // Despertar al MPU6050
  Wire.endTransmission(true);
  
  // Configurar segundo sensor (0x69)
  Wire.beginTransmission(MPU_ADDR_2);
  Wire.write(0x6B);       // Registro PWR_MGMT_1
  Wire.write(0);          // Despertar al MPU6050
  Wire.endTransmission(true);
  
  delay(100);             // Pequeña pausa para inicialización
}

void loop() {
  // Leer datos de ambos sensores
  leerMPU6050(MPU_ADDR_1, &AcX1, &AcY1, &AcZ1, &GyX1, &GyY1, &GyZ1);
  leerMPU6050(MPU_ADDR_2, &AcX2, &AcY2, &AcZ2, &GyX2, &GyY2, &GyZ2);

  // Calcular ángulos para ambos sensores
  calcularAngulos(AcX1, AcY1, AcZ1, &pitch1, &roll1);
  calcularAngulos(AcX2, AcY2, AcZ2, &pitch2, &roll2);

  // Enviar tramas de datos para ambos sensores
  enviarTramas();
  //golpes
 //if ((593> pitch1 >560) && (243>roll1>338) &&(406>pitch2>397) &&(377>roll2>363)){
 //Serial.println("0");
 //delay(2000);
 //}
 //if ((707> pitch1 >690) && (485>roll1>480) &&(477>pitch2>451) &&(542>roll2>537)){
 //Serial.println("1");
 //delay(2000);
 //}
 //if ((527> pitch1 >517) && (522>roll1>517) &&(326>pitch2>321) &&(480>roll2>468)){
 //Serial.println("2");
 //delay(2000);
 //}
 //if ((710> pitch1 >690) && (511>roll1>488) &&(485>pitch2>429) &&(329>roll2>301)){
 //Serial.println("3");
 //delay(2000);
 //}
 //if ((608> pitch1 >602) && (360>roll1>352) &&(295>pitch2>3281) &&(508>roll2>497)){
 //Serial.println("4");
 //delay(2000);
 //}
 
// 0 560-593, 338-343, 397-406, 363-377
// 1 690-707, 480-485, 451-477, 537-542
// 2 517-527, 517-522, 321-326, 468-480
// 3 690-710, 488-511, 429-485, 301-329
// 4 602-608, 352-360, 281-295, 497-508

  delay(500);  // Esperar medio segundo
}

// Función para leer los datos de un MPU6050 (sin temperatura)
void leerMPU6050(int mpu_addr, int16_t* AcX, int16_t* AcY, int16_t* AcZ, 
                 int16_t* GyX, int16_t* GyY, int16_t* GyZ) {
  Wire.beginTransmission(mpu_addr);
  Wire.write(0x3B);  // Empezar a leer desde el registro 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(mpu_addr, 14, true);  // Leer 14 bytes

  // Leer acelerómetro (2 bytes por eje)
  *AcX = Wire.read() << 8 | Wire.read();
  *AcY = Wire.read() << 8 | Wire.read();
  *AcZ = Wire.read() << 8 | Wire.read();

  // Saltar los 2 bytes de temperatura (0x41 y 0x42)
  Wire.read(); Wire.read();

  // Leer giroscopio (2 bytes por eje)
  *GyX = Wire.read() << 8 | Wire.read();
  *GyY = Wire.read() << 8 | Wire.read();
  *GyZ = Wire.read() << 8 | Wire.read();
}

// Función para calcular Pitch y Roll (ahora devuelve enteros)
void calcularAngulos(int16_t AcX, int16_t AcY, int16_t AcZ, int* pitch, int* roll) {
  // Convertir los valores del acelerómetro a gravedades (g)
  float accX = AcX / 16384.0;  // Escala ±2g (16384 LSB/g)
  float accY = AcY / 16384.0;
  float accZ = AcZ / 16384.0;

  // Calcular Pitch y Roll en radianes
  float pitch_float = -atan2(accY, sqrt(accX * accX + accZ * accZ)) * 180 / PI;
  float roll_float = atan2(-accX, sqrt(accY * accY + accZ * accZ)) * 180 / PI;
  
  // Mapear a valores de 0 a 1023 y convertir a entero
  *pitch = (int)map(pitch_float, -180, 180, 0, 1023);
  *roll = (int)map(roll_float, -180, 180, 0, 1023);
}

// Función para enviar las tramas de datos de ambos sensores
void enviarTramas() {
  // Enviar trama para el primer sensor (0x68)
  Serial.print("<");
  enviarValores(pitch1, roll1);
  Serial.print(",");
  // Enviar trama para el segundo sensor (0x69)
  enviarValores(pitch2, roll2);
  Serial.println(">");
}

// Función auxiliar para formatear y enviar valores (ahora con enteros)
void enviarValores(int value1, int value2) {
  // Formatear el primer valor (pitch)
  if (value1 > 999) {
    Serial.print(value1);
  }
  else if (value1 > 99) {
    Serial.print("0");
    Serial.print(value1);
  }
  else if (value1 > 9) {
    Serial.print("00");
    Serial.print(value1);
  }
  else {
    Serial.print("000");
    Serial.print(value1);
  }
  
  // Separador
  Serial.print(",");
  
  // Formatear el segundo valor (roll)
  if (value2 > 999) {
    Serial.print(value2);
  }
  else if (value2 > 99) {
    Serial.print("0");
    Serial.print(value2);
  }
  else if (value2 > 9) {
    Serial.print("00");
    Serial.print(value2);
  }
  else {
    Serial.print("000");
    Serial.print(value2);
  }

 

  
}




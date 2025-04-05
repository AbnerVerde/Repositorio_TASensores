#include <Wire.h>

// Dirección I2C del MPU6050 (0x68 por defecto)
const int MPU_ADDR = 0x68;

// Variables para almacenar los datos del sensor
int16_t AcX, AcY, AcZ;  // Acelerómetro
int16_t GyX, GyY, GyZ;  // Giroscopio
int16_t temp;           // Temperatura (opcional)

// Variables para Pitch y Roll
float pitch, roll;

void setup() {
  Serial.begin(9600);     //iniciar comunicacion serial 
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

  // funcion para Calcular Pitch y Roll a partir del acelerómetro
  calcularAngulos();

  // Mostrar resultados
  //Serial.print("Pitch: ");
  //Serial.print(pitch);
  //Serial.print("° | Roll: ");
  //Serial.print(roll);
  //Serial.println("°");
trama(pitch,roll);
  delay(500);  // Esperar medio segundo
}

// Función para leer los datos del MPU6050
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

//funcion para adecuar la trama de datos 
void trama(int value1, int value2){
  //inicia trama con caracter <
  Serial.print("<");
  //analisa el valor uno para saber cuantos ceros añadir 
  if (value1>999) {
    Serial.print(value1);
  }
  //añade un cero a la izquierda de los datos 
  else if (value1>99){
    Serial.print("0");
    Serial.print(value1);
  }
   //añade dos cero a la izquierda de los datos 
  else if (value1>9){
    Serial.print("00");
    Serial.print(value1);
  }
   //añade tres cero a la izquierda de los datos 
  else {
    Serial.print("000");
    Serial.print(value1);
  }
//Añade una coma para separar datos entre value1 y value2 
  Serial.print(",");
//repite el proceso de añadir ceros al valor 2 
 if (value2>999) {
    Serial.print(value2);
  }
  else if (value2>99){
    Serial.print("0");
    Serial.print(value2);
  }
  else if (value2>9){
    Serial.print("00");
    Serial.print(value2);
  }
  else {
    Serial.print("000");
    Serial.print(value2);
  }
  //finaliza la trama de datos con > y un salto de linea
Serial.println(">");
}

// Función para calcular Pitch y Roll
void calcularAngulos() {
  // Convertir los valores del acelerómetro a gravedades (g)
  float accX = AcX / 16384.0;  // Escala ±2g (16384 LSB/g)
  float accY = AcY / 16384.0;
  float accZ = AcZ / 16384.0;

  // Calcular Pitch y Roll en radianes
  pitch = -atan2(accY, sqrt(accX * accX + accZ * accZ)) * 180 / PI;
  roll = atan2(-accX, sqrt(accY * accY + accZ * accZ)) * 180 / PI;
  pitch=map(pitch,-180,180,0,1023); //ponemos pitch en parametros de 0 a 1023
  roll=map(roll,-180,180,0,1023); //ponemos roll en parametros de 0 a 1023 
  trama(pitch,roll);// obtenemos la trama de datos con pitch y roll 
  //past_timemillis();
}
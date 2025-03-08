//Se declaran las librerias que nos ayudan a la comunicacion y obtencion de datos del sensor y la comunicacion I2C 
#include <Wire.h>
#include <VL53L0X.h>
// Se abre la comunicacion con el sensor 
VL53L0X sensor;

// Se establecen arámetros del filtro EMA
float alpha = 0.25;  // Factor de suavizado en este caso 25 
float filteredDistance = 0;  // Valor filtrado inicial


void setup() {
  //se inicia la comunacion serial 
  Serial.begin(9600);
  //se inicia la comunacion I2C
  Wire.begin();
 
//declaraciones para el posterior inicializado del sensor 
  sensor.init();
  sensor.setTimeout(500);

  // Iniciar el sensor
  if (!sensor.init()) {
    Serial.println("Error al iniciar el sensor VL53L0X!");
    while (1) {}
  }

  // Configurar la medición continua
  sensor.startContinuous();
}

void loop() {
  // Leer la distancia del sensor
  int distance = sensor.readRangeContinuousMillimeters();

  // Aplicar el filtro EMA
  filteredDistance = alpha * distance + (1 - alpha) * filteredDistance;

  //Mostrar los valores en el monitor serial
  Serial.print("Distancia cruda: ");
  Serial.print(distance);
  Serial.print(" mm\t");
  Serial.print("Distancia filtrada: ");
  Serial.print(filteredDistance);
  Serial.println(" mm");
 
 // se agrega un delay para una mejor lectura de datos 
  delay(100); 
}
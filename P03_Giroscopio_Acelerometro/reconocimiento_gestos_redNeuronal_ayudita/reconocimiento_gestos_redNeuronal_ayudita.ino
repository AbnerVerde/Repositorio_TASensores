#include <Wire.h>
#include <MPU6050.h>
//agrgamos librería de nuestro sensor MPU6050 para facilitar las funciones
MPU6050 mpu;

int pitch = 0; //valor inical de pitch
int roll = 0; //valor inicial de roll
unsigned long past_time = 0;

////// Variables Red Neuronal //////
const int node = 2;
const int input = 2;
const int outputs = 2;

float P[input][1]; //Define el arreglo P que el arduino tiene de OCTAVE (red neuronal)

//Los siguientes encontrados por el TRAIN de redes neuronales en octave después d etomar 500 muestras
float W1[node][input] = {{-3.2748, 2.5797}, //Peso W1 obtenido 
                        {2.2112, 3.2672}};
float b1[node][1] = {{0.5801},  //Bia b1 obtenida
                    {-0.23792}};
float a1[node][1];

float W2[outputs][node] = {{1.309, -4.2272}, //Peso W2 obtenido
                          {4.2201, 1.2829}};
float b2[outputs][1] = {{-0.12685}, //Bía b2 obtenida
                       {0.084799}};
float a2[outputs][1];
float aux = 0.0;

int maxValue = 1023;
int minValue = 0;

void setup() {
  Serial.begin(9600); //Inicia la comunicación
  Wire.begin();
  
  // Inicializa el MPU6050
  mpu.initialize();
  
  // Verifica conexión del sensor
  Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  
  // Configurar rango del giroscopio segun la librería que utilizamos
    mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_2000);
}

void loop() {
  // Leer datos del acelerómetro
  int16_t ax, ay, az; //Dividirlas por ejes X, Y, Z
  mpu.getAcceleration(&ax, &ay, &az);
  
  // Calcular pitch y roll (en radianes)
  pitch = -(atan2(ax, sqrt(ay*ay + az*az)) * 180.0/PI);
  roll = (atan2(ay, az) * 180.0/PI);

  // Mapear valores para obtenerlos en el rango que buscamos graficarlos
  pitch = map(pitch, -180, 180, 0, 1023);
  roll = map(roll, -180, 180, 0, 1023);
 
 //Normalizar los valores para obtener una mejor respuesta
  P[0][0] = dataNormalized((float)pitch, minValue, maxValue);
  P[1][0] = dataNormalized((float)roll, minValue, maxValue);

  ////// Primera Capa //////
  for(int i = 0; i < node; i++) {
    aux = 0.0;
    for(int j = 0; j < input; j++) {
      aux += W1[i][j] * P[j][0];
    }
    a1[i][0] = tansig(aux + b1[i][0]);  // Corregido: b1[i][0] en lugar de b1[1][0]
  }

  ////// Segunda Capa //////
  for(int i = 0; i < outputs; i++) {
    aux = 0.0;
    for(int j = 0; j < node; j++) {
      aux += W2[i][j] * a1[j][0];
    }
    a2[i][0] = tansig(aux + b2[i][0]);
  }

  // Determinar dirección
  //Cambiamos la etiqueta de dirección según nuestro entrenamiento del sensor MPU6050
  if (a2[0][0] <= 0 && a2[1][0] <= 0) {
    Serial.println("Izquierda");
  }
  else if(a2[0][0] <= 0 && a2[1][0] > 0) {
    Serial.println("Derecha");
  }
  else if(a2[0][0] > 0 && a2[1][0] <= 0) {
    Serial.println("Adelante");
  }
  else {
    Serial.println("Quieto");
  }
  
  delay(100); //Espera de 100ms
}

float tansig(float n) {
  float a = exp(n);
  float b = exp(-n);
  return (a-b)/(a+b);
}
//Datos normalizados
float dataNormalized(int inputData, int minData, int maxData) {
  return 2.0 * (inputData - minData) / (maxData - minData) - 1.0;
}
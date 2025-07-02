#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

int pitch = 0;
int roll = 0;
unsigned long past_time = 0;

////// Variables Red Neuronal ////// 
const int node = 2;       // 2 neuronas ocultas
const int input = 6;      // 6 entradas
const int outputs = 6;    // 6 clases

float P[input][1]; // Entrada

float W1[node][input] = { // Peso 1
  {2.1688, 0.29503, 0.69241, -0.35115, 0.75328, 1.0434},
  {3.8915, -0.32614, -0.043533, 1.3214, -0.65081, 4.5923}
};

float b1[node][1] = { // Bía 1
  {-0.37754},
  {0.52559}
};

float a1[node][1];

float W2[outputs][node] = { // Peso 2
  {2.0238, -0.79039},
  {0.99649, 0.98282},
  {-1.6305, 3.6605},
  {2.9288, 3.1086},
  {0.14155, -0.53914},
  {0.037974, 4.6476}
};

float b2[outputs][1] = { // Bía 2
  {0.45446},
  {0.71637},
  {0.55637},
  {1.3201},
  {-1.0914},
  {-0.37285}
};

float a2[outputs][1];
float aux = 0.0;

int maxValue = 1023;
int minValue = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  mpu.initialize();
  Serial.println(mpu.testConnection() ? "MPU6050 conexión exitosa" : "MPU6050 conexión fallida");

  mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_2000);
}

void loop() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Calcular pitch y roll
  pitch = -(atan2(ax, sqrt(ay * ay + az * az)) * 180.0 / PI);
  roll = (atan2(ay, az) * 180.0 / PI);

  // Mapear
  pitch = map(pitch, -180, 180, 0, 1023);
  roll = map(roll, -180, 180, 0, 1023);

  // Entradas adicionales (puedes modificar estas combinaciones)
  float normPitch   = dataNormalized((float)pitch, minValue, maxValue);
  float normRoll    = dataNormalized((float)roll, minValue, maxValue);
  float normSum     = dataNormalized((float)(pitch + roll), 2 * minValue, 2 * maxValue);
  float normDiff    = dataNormalized((float)(pitch - roll), -maxValue, maxValue);
  float normPitch2  = dataNormalized((float)pitch, minValue, maxValue); // placeholder
  float normRoll2   = dataNormalized((float)roll, minValue, maxValue);  // placeholder

  // Llenar vector de entrada
  P[0][0] = normPitch;
  P[1][0] = normRoll;
  P[2][0] = normSum;
  P[3][0] = normDiff;
  P[4][0] = normPitch2;
  P[5][0] = normRoll2;

  ////// Primera Capa ////// 
  for (int i = 0; i < node; i++) {
    aux = 0.0;
    for (int j = 0; j < input; j++) {
      aux += W1[i][j] * P[j][0];
    }
    a1[i][0] = tansig(aux + b1[i][0]);
  }

  ////// Segunda Capa ////// 
  for (int i = 0; i < outputs; i++) {
    aux = 0.0;
    for (int j = 0; j < node; j++) {
      aux += W2[i][j] * a1[j][0];
    }
    a2[i][0] = tansig(aux + b2[i][0]);
  }

  ////// Clasificación ////// 
  int maxIndex = 0;
  float maxOut = a2[0][0];
  for (int i = 1; i < outputs; i++) {
    if (a2[i][0] > maxOut) {
      maxOut = a2[i][0];
      maxIndex = i;
    }
  }

  // Imprimir resultado
  switch (maxIndex) {
    case 0: Serial.println("0"); break;
    case 1: Serial.println("4"); break;
    case 2: Serial.println("5  2"); break;
    case 3: Serial.println("3"); break;
    case 4: Serial.println("1"); break;
    case 5: Serial.println("2"); break;
  }

  delay(100);
}

float tansig(float n) {
  float a = exp(n);
  float b = exp(-n);
  return (a - b) / (a + b);
}

float dataNormalized(int inputData, int minData, int maxData) {
  return 2.0 * (inputData - minData) / (maxData - minData) - 1.0;
}

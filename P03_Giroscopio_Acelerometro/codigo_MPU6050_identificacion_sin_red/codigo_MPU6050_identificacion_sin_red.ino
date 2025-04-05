#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup() {
    Serial.begin(9600);
    Wire.begin();
    mpu.initialize();

    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);

    if (!mpu.testConnection()) {
        Serial.println("MPU6050 conexión fallida!");
        while (1);
    }
    Serial.println("MPU6050 conectado correctamente");
}

void loop() {
    int16_t ax, ay, az;
    mpu.getAcceleration(&ax, &ay, &az);

    // Determina la orientación del sensor
    if (ax > 15000) {
        Serial.println("Posición: Derecha");
        digitalWrite(2, HIGH);
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(6, LOW);
        digitalWrite(7, LOW);
    } else if (ax < -15000) {
        Serial.println("Posición: Izquierda");
        digitalWrite(3, HIGH);
        digitalWrite(2, LOW);
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(6, LOW);
        digitalWrite(7, LOW);
    } else if (ay > 15000) {
        Serial.println("Posición: Adelante");
        digitalWrite(4, HIGH);
        digitalWrite(3, LOW);
        digitalWrite(2, LOW);
        digitalWrite(5, LOW);
        digitalWrite(6, LOW);
        digitalWrite(7, LOW);
    } else if (ay < -15000) {
        Serial.println("Posición: Atrás");
        digitalWrite(5, HIGH);
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
        digitalWrite(2, LOW);
        digitalWrite(6, LOW);
        digitalWrite(7, LOW);
    } else if (az > 15000) {
        Serial.println("Posición: Arriba");
        digitalWrite(6, HIGH);
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(2, LOW);
        digitalWrite(7, LOW);
    } else if (az < -15000) {
        Serial.println("Posición: Abajo");
        digitalWrite(7, HIGH);
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(6, LOW);
        digitalWrite(2, LOW);
    } else {
        Serial.println("Posición: Indefinida");
    }

    delay(1000); // Esperar medio segundo antes de la siguiente lectura
}
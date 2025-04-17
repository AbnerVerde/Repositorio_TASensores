#include <Wire.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp; // Objeto para el sensor BMP180
int c=0;



void setup() {
    Serial.begin(9600);
    
    // Inicializar sensor BMP180
    if (!bmp.begin()) {
        Serial.println("Error al iniciar BMP180!");
        while (1); // Detener si hay error
    }
}

void loop() {
    // Leer presión (en Pascales) y temperatura (en °C) del BMP180
    float presion_Pa = bmp.readPressure(); // BMP180 devuelve presión en Pa
    float temp_C = bmp.readTemperature();
    
    //verifica si la presion leida es mayor a 85000 lo que significa que un carro paso 
    //la presion en estado inicial ronda en los 730hp 
    if (presion_Pa > 85000){
      //suma en un contador la cantidad de veces que ha habido un aumento de presion 
       c++;
       //imprime la variable c  y su respectivo ensaje 
      Serial.print(c);

      if (c==1){
        Serial.println("carro ha pasado");
      }
      else{
        
Serial.println("carros han pasado");
      }
    }
    else {
    //si no detecta ningun carro se imprime el siguiente mensaje 
      Serial.println("no se ha registrado ningun carro ");

    }
    
    delay(2000); // Esperar 2 segundos entre lecturas
}
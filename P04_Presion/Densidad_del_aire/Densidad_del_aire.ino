#include <Wire.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp; // Objeto para el sensor BMP180

// Constantes para el cálculo de la densidad del aire
#define Rd 287.0        // Constante del aire seco [J/(kg·K)]
#define HR 63.5         // Humedad relativa fija (63.5%)

// Función para calcular densidad del aire (kg/m³)
float calcularDensidadAire(float presion_Pa, float temp_C, float humedad_rel) {
    // Convertir temperatura a Kelvin
    float T = temp_C + 273.15;
    
    // 1. Calcular presión de vapor de saturación (e_s) en Pa (fórmula de Magnus)
    float e_s = 611.0 * exp((17.27 * temp_C) / (temp_C + 237.3));
    
    // 2. Calcular presión de vapor real (e) en Pa
    float e = (humedad_rel / 100.0) * e_s;
    
    // 3. Calcular humedad específica (q)
    float q = (0.622 * e) / (presion_Pa - (0.378 * e));
    
    // 4. Calcular temperatura virtual (T_v)
    float T_v = T * (1.0 + 0.61 * q);
    
    // 5. Calcular densidad del aire (ρ)
    float rho = presion_Pa / (Rd * T_v);
    
    return rho; // Devuelve densidad en kg/m³
}

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
    
    // Calcular densidad del aire con HR = 63.5%
    float densidad = calcularDensidadAire(presion_Pa, temp_C, HR);
    
    // Mostrar resultados
    Serial.print("Temperatura: "); Serial.print(temp_C); Serial.println(" °C");
    Serial.print("Presión: "); Serial.print(presion_Pa / 100.0); Serial.println(" hPa");
    Serial.print("Densidad del aire: "); Serial.print(densidad); Serial.println(" kg/m³");
    Serial.println("-------------------");
    
    delay(2000); // Esperar 2 segundos entre lecturas
}
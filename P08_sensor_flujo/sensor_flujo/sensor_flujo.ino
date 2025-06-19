volatile int pulsos = 0; 
float caudal_L_min;
unsigned long tiempoAnterior = 0;

const byte pinSensor = 2;  // Usa una entrada digital con interrupción (pin 2 en Arduino Uno)

void setup() {
  Serial.begin(9600);
  pinMode(pinSensor, INPUT);  // Definimos como entrada la señal del sensor de flujo para adquirir los datos de ahí
  attachInterrupt(digitalPinToInterrupt(pinSensor), contarPulsos, RISING);
}

void loop() {
  if (millis() - tiempoAnterior >= 1000) {  // cada 1 segundo
    detachInterrupt(digitalPinToInterrupt(pinSensor));

    caudal_L_min = (pulsos / 7.5);  // Fórmula de conversión 7.5 pulsos/seg = 1 L/min

    Serial.print("Caudal: ");
    Serial.print(caudal_L_min);  // Imprime el flujo que hay en el caudal en L/min
    Serial.print(" L/min");
    Serial.print("     Pulsos: ");
    Serial.print(pulsos);   // Imprime los pulsos obtenidos en la medición tomada
    Serial.println(" ");
    
    pulsos = 0;  // Reinicia el contador
    tiempoAnterior = millis();

    attachInterrupt(digitalPinToInterrupt(pinSensor), contarPulsos, RISING);
  }
}

void contarPulsos() {
  pulsos++;
}

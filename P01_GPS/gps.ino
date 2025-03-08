//Se agrega la libreria para trabajar la comunicacion con nuestro sensor
#include <SoftwareSerial.h>
//se seleccionan dos pines para establecer la comunicacion con el sensor 
const int rxpin=2;
const int txpin=3;
//se declara el rx y tx para el gps 
 SoftwareSerial gpsSerial(rxpin,txpin);
void setup() {
  //se inicializa la comunicacion serial a la misma velocidad en el monitor serial y en el GPS
   Serial.begin(9600);
   gpsSerial.begin(9600);
}

void loop() {
  //iniciamos una condicion para que solo reciba datos si hay comunicacion 
   if (gpsSerial.available() > 0){
    //declaramos una cadena de caracteres verificando que la informacion de la cadena este completa por medio de \n
    String c=gpsSerial.readStringUntil('\n');
    //aplicamos un filtro para solo obtener las coordenadas de interes 
    if (c.startsWith("$GPGLL")){
      //imprimimos en el monitor serial la cadena de carcteres 
    Serial.println(c);
    }
  }
  //aplicamos un delay 
 delay (300000);
}

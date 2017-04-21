#define TEMP_SENSOR       0            // Definimos el pin analogico del sensor de temperaturaa

void setup(){
  Serial.begin(9600); // Iniciamos el puerto serial
}

void loop(){

 int VOLT_TEMP = analogRead(TEMP_SENSOR); // Nivel de voltaje que entrega el sensor con valores de 0 y 1023
 float TEMP = (5 * VOLT_TEMP * 100) / 1024; // Se realiza la conversion a grados celsius
 Serial.println(TEMP);
 delay(5000); // Esperamos 5s antes de empezar la siguiente lectura.
}

#define TEMP_SENSOR       0            // Definimos el pin analogico del sensor de temperaturaa
#define CO2_SENSOR        1           // Definimos el pin analogico del sensor de CO2
//#define MONCARB_SENSOR  1            // Definimos el pin analogico del sensor de monoxido de carbono
//#define OZONO_SENSOR    2            // Definimos el pin analogico del senosr de ozono
//#define GAS_SENSOR      3            //Definimos el pin analogico del sensor de gas


//------- para el sensor de Co2 --------
//Sacado de: https://www.dfrobot.com/wiki/index.php/CO2_Sensor_SKU:SEN0159
#define DC_GAIN (8.5)   //Define la ganancia de corriente continua del amplificador
//These values differ from sensor to sensor. User should derermine this value.
#define         ZERO_POINT_X                 (2.602) //lg400=2.602, the start point_on X_axis of the curve
#define         ZERO_POINT_VOLTAGE           (0.324) //define the output of the sensor in volts when the concentration of CO2 is 400PPM
#define         MAX_POINT_VOLTAGE            (0.265) //define the output of the sensor in volts when the concentration of CO2 is 10,000PPM
#define         REACTION_VOLTGAE             (0.059) //define the voltage drop of the sensor when move the sensor from air into 1000ppm CO2

/***********************Software Related Macros************************************/
#define         READ_SAMPLE_TIMES            (10)     //define how many samples you are going to take in normal operation
#define         READ_SAMPLE_INTERVAL         (50)    //define the time interval(in milisecond) between each samples in
//normal operation

//Globales 
float           CO2Curve[3]  =  {ZERO_POINT_X, ZERO_POINT_VOLTAGE, (REACTION_VOLTGAE / (2.602 - 4))};
//Two points are taken from the curve.With these two points, a line is formed which is
//"approximately equivalent" to the original curve. You could use other methods to get more accurate slope

//CO2 Curve format:{ x, y, slope};point1: (lg400=2.602, 0.324), point2: (lg10000=4, 0.265)
//slope = (y1-y2)(i.e.reaction voltage)/ x1-x2 = (0.324-0.265)/(log400 - log10000)

void setup(){
  Serial.begin(9600); // Iniciamos el puerto serial
}

void loop(){
 //-----------Sensor de temperatura-----
 
 int VOLT_TEMP = analogRead(TEMP_SENSOR); // Nivel de voltaje que entrega el sensor con valores de 0 y 1023
 int TEMP = (5 * VOLT_TEMP * 100) / 1024; // Se realiza la conversion a grados celsius
 Serial.println(TEMP);
 delay(500); // Esperamos 500ms antes de empezar la siguiente lectura.

 //----------- Sensor de CO2 ----------------------
 int CO2;
 float volts;
 int VOLT_CO2 = MGRead(CO2_SENSOR);

 CO2 = MGGetPercentage(volts, CO2Curve);
 Serial.print("CO2:");
 if (CO2 == -1) {
    Serial.print("Under heating/beyond range(400~10,000)");
  } else {
    Serial.print(CO2);
  }
  Serial.println( "ppm" );
  delay(500);
}


/*****************************  MGRead CO2*********************************************
Input:   mg_pin - analog channel
Output:  output of SEN-000007
Remarks: This function reads the output of SEN-000007
************************************************************************************/
float MGRead(int mg_pin) {
  int i;
  float v = 0;

  for (i = 0; i < READ_SAMPLE_TIMES; i++) {
    v += analogRead(mg_pin);
    delay(READ_SAMPLE_INTERVAL);
  }
  v = (v / READ_SAMPLE_TIMES) * 5 / 1024 ;
  return v;
}
/*****************************  MQGetPercentage CO2 **********************************
Input:   volts   - SEN-000007 output measured in volts
         pcurve  - pointer to the curve of the target gas
Output:  ppm of the target gas
Remarks: By using the slope and a point of the line. The x(logarithmic value of ppm)
         of the line could be derived if y(MG-811 output) is provided. As it is a
         logarithmic coordinate, power of 10 is used to convert the result to non-logarithmic
         value.
************************************************************************************/
int  MGGetPercentage(float volts, float *pcurve) {
  volts = volts / DC_GAIN;
  if (volts > ZERO_POINT_VOLTAGE || volts < MAX_POINT_VOLTAGE ) {
    return -1;
  } else {
    return pow(10, (volts - pcurve[1]) / pcurve[2] + pcurve[0]);
    volts = 0;
  }
}



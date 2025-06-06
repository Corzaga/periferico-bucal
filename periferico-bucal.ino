//Mouse Bucal V3
//Mario Aguilera
//C+

//Requisitos del sistema: Arduino pro micro, BME280, y joystick

//LIBRERIAS

#include <Wire.h>
#include <SPI.h>
#include <Mouse.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;


//VARAIBLES Y CONSTANTES

int presionInicial;   //Presion en estado de reposo
int presionActual;    //Presion al soplar

//Pines para el joystick
const int switchPin = A1;   //switch para prender o apagar el mouse
const int xAxis = A3;       //eje X del joystick
const int yAxis = A2;       //eje Y del joystick
int xReading;               //lo que lee del eje X
int yReading;               //lo que lee del eje Y

//Parametros del joystick
int range = 5;               //rango de movimiento de X e Y
int responseDelay = 1;        //retardo de respuesta del mouse, en ms
int threshold = range / 5;    //no c
int center = range / 2;       //valor de posición de descanso

bool mouseIsActive = true;    //estado del mouse??
int switchState;
int lastSwitchState = true;   //ultimo estado del mouse


//FUNCIONES

//Funcion que lee y mapea el eje necesario
//Entrada: entero
//Salida: distanncia
int readAxis(int thisAxis) {
  // read the analog input:
  int reading = analogRead(thisAxis);

  // map the reading from the analog input range to the output range:
  reading = map(reading, 0, 1023, 0, range);

  // if the output reading is outside from the rest position threshold, use it:
  int distance = reading - center;

  if (abs(distance) < threshold) {
    distance = 0;
  }

  // return the distance for this axis:
  return distance;
}

void setup() {
 
  pinMode(switchPin, INPUT_PULLUP);
  Serial.begin(9600);
  Mouse.begin();

  unsigned estado;
  estado = bme.begin(0x76);   //inicializa el BME con direccion 0x76
  if(!estado){
    Serial.println("Error en el sensor de presion");
    delay(10);
  }

  presionInicial = bme.readPressure();
  
}

void loop() {
  
  //Lecturas
  presionActual = bme.readPressure();
  xReading = (readAxis(xAxis)*-1); //se agrego un factor inverso segun sea necesario
  yReading = (readAxis(yAxis)*-1); //se agrego un factor inverso segun sea necesario
  Serial.println("X: "); Serial.print(yReading);
  Serial.print(", Y: "); Serial.print(xReading);

  //Movimiento mouse
  Mouse.move(xReading, yReading, 0);

  //Click del mouse
  if((presionActual - presionInicial) > 100){   //si dif entre presiones es mayor a 100
    Mouse.click();
    Serial.print(" Mouse ha hecho click");
    delay(1000);
  }
  else{
    Serial.print(" Mouse sin hacer click");
  }
  
}
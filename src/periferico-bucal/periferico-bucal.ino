//Mouse Bucal V3
//Mario Aguilera, C+
//Requisitos del sistema: Arduino Pro Micro, BME180, y modulo joystick

//LIBRERIAS

#include <Mouse.h>
#include <OneButton.h>      //https://github.com/mathertel/OneButton
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;

//VARAIBLES Y CONSTANTES

bool estadoActualBMP;   //true si esta siendo soplado
bool estadoPrevioBMP;   //valor anterior del BMP

int intervalo = 1000;   //segundos que debe soplar para click derecho
unsigned long tiempoActual;   //variable del contador

int presionInicial;   //Presion en estado de reposo
int presionActual;    //Presion al soplar

//Pines para el joystick
const int switchPin = A1;   //switch para mantener presionado el click izquierdo
const int xAxis = A3;       //eje X del joystick
const int yAxis = A2;       //eje Y del joystick
int xReading;               //lo que lee del eje X
int yReading;               //lo que lee del eje Y

//Parametros del joystick
int range = 10;               //rango de movimiento de X e Y, MOVER ESTE PARAMETRO PARA AUMENTAR VELOCIDAD
int responseDelay = 5;        //retardo de respuesta del mouse, en ms
int threshold = range / 4;    //no c
int center = range / 2;       //valor de posición de descanso

bool lastSwitchState = false;   //ultimo estado del click, apagado = false

OneButton botonJoystick(switchPin, true, true);   //Instancia del boton del joystick

void setup() {
 
  pinMode(switchPin, INPUT_PULLUP);
  Serial.begin(9600);
  Mouse.begin();

  unsigned estado;
  estado = bmp.begin(0x76);   //inicializa el bmp con direccion 0x76
  if(!estado){
    Serial.println("Error en el sensor de presion");
    delay(10);
  }
  
  presionInicial = bmp.readPressure();    //lee el BMP
  estadoPrevioBMP = false;  //BMP sin presion

  botonJoystick.attachClick(joystickClick);
}

void loop() {
  
  //Lecturas
  presionActual = bmp.readPressure();
  actualizarEstadoBMP();    //se actualiza la lectura actual
  
  xReading = (readAxis(xAxis)*-1); //se agrego un factor inverso segun sea necesario
  yReading = (readAxis(yAxis)*-1); //se agrego un factor inverso segun sea necesario
  
  //Movimiento mouse
  Mouse.move(xReading, yReading, 0);

  clickMouse();   //lee si el mouse ha sido clickeado (soplado)

  botonJoystick.tick();   //lee si el boton del joystick ha hecho click
}

//FUNCIONES

//Funcion que lee y mapea el eje del joystick
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

//Funcion que actualiza el estado del BMP, true cuando hay dif presion
//Entrada: nada
//Salida: distancia
void actualizarEstadoBMP(){
  if((presionActual - presionInicial) > 50){   //si existe dif presion
    estadoActualBMP = true;
  }
  else{
    estadoActualBMP = false;
  }
}

//Funcion que lee si se ha hecho click con el mouse
//Entrada: nada
//Salida: nada
void clickMouse(){
  if(estadoActualBMP == true){
    if(estadoActualBMP != estadoPrevioBMP){
      tiempoActual = millis();
      Serial.println("inicia timer 2");    
    }
  }
  else{
    if(estadoActualBMP != estadoPrevioBMP){
      if(millis() - tiempoActual >= intervalo){
        Mouse.click(MOUSE_RIGHT);
        Serial.println("click derecho");
      }
      else{
        Mouse.click(MOUSE_LEFT);
        Serial.println("click izquierdo");        
      }
    }
  }
  estadoPrevioBMP = estadoActualBMP;
}

//Funcion que se acciona cuando se clickea el switch del joystick
void joystickClick(){
  Serial.println("click");
  if(lastSwitchState == true){
    Mouse.release(MOUSE_LEFT);
    lastSwitchState = false;
    Serial.println("click del joystick soltado");
  }
  else{
    Mouse.press(MOUSE_LEFT);
    lastSwitchState = true;
    Serial.println("click del joystick presionado");
  }
}

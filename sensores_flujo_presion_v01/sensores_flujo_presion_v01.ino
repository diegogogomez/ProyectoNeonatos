#include "HX711.h"
#include <Wire.h>

HX711 loadcell;
const int LOADCELL_DOUT_PIN = A1;
const int LOADCELL_SCK_PIN = A0;
float outputValue = 0;

unsigned long calibracion = 0;
volatile unsigned long presion = 0;

uint8_t parte_baja = 0;
uint8_t parte_alta = 0;
uint16_t valor_flujo_sin_offset = 0; 
int valor_flujo = 0;
uint16_t offset = 32768; // Offset for the sensor

  
void setup() {
  pinMode(12, OUTPUT);  
  
  Wire.begin();
  Serial.begin(9600);
  
  
  delay(100);
  Wire.beginTransmission(byte(0x40)); // transmit to device #064 (0x40)
  Wire.write(byte(0x10));      //
  Wire.write(byte(0x00));      //
  Wire.endTransmission(); 
  delay(5);
 
  Wire.requestFrom(0x40, 3); // read 3 bytes from device with address 0x40
  uint16_t a = Wire.read(); // first received byte stored here. The variable "uint16_t" can hold 2 bytes, this will be relevant later
  uint8_t b = Wire.read(); // second received byte stored here
  
  // put your setup code here, to run once:
  
  loadcell.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  loadcell.set_scale();
  loadcell.set_offset();
  
  for(int i = 0; i < 10; i ++){
    calibracion += loadcell.get_units(10);
    delay(10);
  }  
  calibracion /= 10;
  
  //Serial.print("Sensor_presión,Sensor_flujo");
}

void loop() {
 //sensor de flujo
  Wire.requestFrom(0x40, 3); // read 3 bytes from device with address 0x40
  parte_alta = Wire.read(); // first received byte stored here. The variable "uint16_t" can hold 2 bytes, this will be relevant later
  parte_baja = Wire.read(); // second received byte stored here
  valor_flujo_sin_offset = ((parte_alta << 8) | parte_baja);
  valor_flujo = (valor_flujo_sin_offset - offset);
  //sensor de flujo

  //sensor de presion
  presion = loadcell.get_units(1) - calibracion;
  outputValue = mapfloat(presion, 0, 0xFFFFFF, 0, 40) * 5;
  //outputValue = mapfloat(presion, 0, 0xFFFFFF, 0, 40) * 10.1972;
  
  //Serial.print(outputValue);      //sensor de presión
  //Serial.print(",");
  Serial.println(valor_flujo);    //sensor de flujo
  //Serial.print(String("[") + valor_flujo + String(",") + outputValue + String("]"));

  if(valor_flujo > 25){
    digitalWrite(12, HIGH); // sets the digital pin 13 on
  }
  else{
    digitalWrite(12, LOW); // sets the digital pin 13 on
    }
  
  delay(10);

}

float mapfloat(long x, long in_min, long in_max, long out_min, long out_max)
{
 return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

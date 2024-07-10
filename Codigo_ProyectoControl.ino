#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

LiquidCrystal_I2C lcd(0x27, 16, 2); //Asignar dirección del LCD

//Ajustes para el DHT22
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321
const int DHTPin = 2;
DHT dht(DHTPin, DHTTYPE);


#define VSETPOINT A0
#define VSENSE A1
#define PWM 5
#define RELE 7

float V_MAX = 0;

#define MUESTRAS 50

int RawValue= 0;
int ADCvRef = 0;
int i = 0;
int valorPWM = 0;
double volts = 0;
double tensionMedida = 0;
float tensionSetPoint = 0;
float tension = 0;
float atenuacion = 1.0;
float desvioTension = 0;
float tempobjetivo = 0;

//-----------------------------------------------------------
//Funcion Setup de Configuracion
//-----------------------------------------------------------
void setup(){
  lcd.init();
  lcd.backlight();

  Serial.begin(9600);
   
  pinMode(RELE, OUTPUT);

   ADCvRef = vRefADC();
   Serial.print("Vref = ");
   Serial.println(ADCvRef);
   lcd.setCursor(0, 0);
   lcd.print("Tension Verf:  ");
   lcd.setCursor(0, 1);
   lcd.print(ADCvRef);
   lcd.setCursor(5, 1);
   lcd.print(" mV");
   delay(2000);
   pinMode(PWM, OUTPUT);
   valorPWM=128;
   analogWrite(PWM, valorPWM);
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("SET PWM INI: ");
   lcd.setCursor(0, 1);
   lcd.print(valorPWM);
   delay(2000);
   lcd.clear();

//Prueba de DHT22
  Serial.println("DHTxx test!");
  dht.begin();

}

//-----------------------------------------------------------
//Funcion Repetitiva
//-----------------------------------------------------------
void loop(){
  
  //V_MAX = mideSetPoint();
  
  tempobjetivo = mideSetPointTemperatura();

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Fallo en la lectura del sensor DHT");
    return;
  }

  //Regulación automática con voltaje y Relé

  if(tempobjetivo+0.1<t){
    if((tempobjetivo>17.5) && (t<tempobjetivo+0.25)){
      V_MAX = 3;
      digitalWrite(RELE, HIGH);
    }
    
    else{
      V_MAX = 12;
      digitalWrite(RELE, LOW);
    }
    
  }

  else if(tempobjetivo>=t){
    V_MAX = 3;
    digitalWrite(RELE, HIGH);
  }

  Serial.print("SET: ");
  Serial.println(V_MAX);
  mideTension();
  Serial.print("OUT: ");
  Serial.println(tension);
  ajustaPWM();


  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println(" °C ");

  imprimirLCD(tempobjetivo,t);
}


/////////// FUNCIONES
//Imprimir LCD

void imprimirLCD(float tempobj, float temp){
  lcd.setCursor(0,0);
  lcd.print("Temp. obj: ");
  lcd.print(tempobj); 
  //lcd.setCursor(12,0);}
  lcd.setCursor (0,1);
  lcd.print("Temp. act.:");
  lcd.print(temp);

}


//-----------------------------------------------------------
//Controla el PWM
//-----------------------------------------------------------
void ajustaPWM(){
   if(tension < V_MAX){
      if(valorPWM>0)
         valorPWM--;
      delay(1);
      analogWrite(PWM, valorPWM);
      //lcd.setCursor(9, 0);
      //lcd.print("PWM:");
      //lcd.print(valorPWM); 
   }else{
      if(valorPWM<255)
         valorPWM++;
      delay(1);
      analogWrite(PWM, valorPWM);
      //lcd.setCursor(9, 0);
      //lcd.print("PWM:");
      //lcd.print(valorPWM); 
   }
}   

//-----------------------------------------------------------
//Funcion de calculo de tension de alimentacion
//-----------------------------------------------------------
int vRefADC(){
   long result;
   ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
   delay(2);
   ADCSRA |= _BV(ADSC);
   while (bit_is_set(ADCSRA,ADSC));
   result = ADCL;
   result |= ADCH<<8;
   result = 1125300L / result;
   return result;
}


//-----------------------------------------------------------
//Funcion de medicion de SetPoint de la temperatura
//-----------------------------------------------------------
float mideSetPointTemperatura(){
   float tempobj = 0;
   RawValue = analogRead(VSETPOINT);
   tempobj = 5*(RawValue / 1023.0) + 14;
   delay(1);
   return tempobj;
}

/*float mideSetPoint(){
   RawValue = analogRead(VSETPOINT);
   tensionSetPoint = (RawValue / 1023.0) * ADCvRef;
   delay(1);
   return (tensionSetPoint/1000)*3.7;
}
*/

//-----------------------------------------------------------
//Funcion de medicion de Tension
//-----------------------------------------------------------
float mideTension(){
   volts = 0;
   for(i=0;i<MUESTRAS;i++){
      vRefADC();
      RawValue = analogRead(VSENSE);
      tensionMedida = (RawValue / 1023.0) * ADCvRef;
      volts = volts + tensionMedida;
      delay(1);
   }
   tension = ((((volts / MUESTRAS) / 1000) / atenuacion) - desvioTension)*3.7;
   return tension;
}


















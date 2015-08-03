#include <SoftwareSerial.h>

#include <LiquidCrystal.h>
#include <Wire.h> 

LiquidCrystal lcd(13,12,11,10,9,8);
int estado;
String destinos[7]={"Catedral", "Mus. Rufino Tam", "La Soledad", "Santo Domingo", "J. Etnobotanico","El Llano", "Merc. 20 de Nov"};

int destino;
int cod;

long tiempoultimocambio;
char codigo[]="0000";

SoftwareSerial modulo(4, 5); // rx, tx
SoftwareSerial ucl(2, 3); // rx, tx

void setup(){ 
  Serial.begin(9600);
  ucl.begin(9600);
  modulo.begin(9600);
  lcd.begin(16, 2);
  estado=0;
  destino=0;
  cod=0000;
  tiempoultimocambio=millis();
  codigo[0]='0';
  codigo[1]='0';
  codigo[2]='0';
  codigo[3]='0';
  pinMode(A0,INPUT);//Boton aceptar
  pinMode(6,INPUT);//Abajo
  pinMode(7,INPUT);//Arriba
  pinMode(A1, OUTPUT); // Encender BT
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Seleccione");
  lcd.setCursor(0,1);  
  lcd.print("Destino");
  apagarBT();
  delay(1000);
}

void loop(){
  
  
switch(estado)
{
  case 0:
  esperarOpcion();
  break;
  case 1:
  esperarNotificacion();
  break;
  case 2:
  esperarSenial();
  break;
  
}
}

void esperarOpcion()
{
   int ti=millis();
   if(digitalRead(6)==HIGH) //Boton de subir para seleccionar ruta
   {
     destino=destino-1;
     if(destino<=-1)
     {
       destino=6;
     }
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print(destinos[destino]);
     lcd.setCursor(0,1);
     lcd.print("Seleccionado");
     tiempoultimocambio=millis();
     delay(150);
   }
   if(digitalRead(7)==HIGH) //Boton de subir para seleccionar ruta
   {
     destino=destino+1;
     if(destino>=7)
     {
       destino=0;
     }
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print(destinos[destino]);
     lcd.setCursor(0,1);
     lcd.print("Seleccionado");
     tiempoultimocambio=millis();
     delay(150);
   }   

   //comparar  tiempo actual con ultimo cambio
   if((millis()-tiempoultimocambio)>20000)
   {
       tiempoultimocambio = millis();
       destino=0;
       //mostrar en display seleccionar opcion;
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("Seleccione");
       lcd.setCursor(0,1);  
       lcd.print("Destino");
   }
   if(analogRead(A0)>50&&(destino!=-1)) //boton aceptar
   {
   //generar codigo
   String str;
   randomSeed(tiempoultimocambio);
   cod=random(1000,9999);
   str=String(cod);
   str.toCharArray(codigo,5);
   //mostrar codigo en display
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Codigo: ");
   lcd.print(codigo);
   lcd.setCursor(0,1);
   lcd.print("Guardelo");
   //enviar codigo a UCL
     estado=1;
     tiempoultimocambio = millis();
    delay(500); // para que no entre al siguiente estado mientras sigue pulsado 
   }
   //Serial.println(destino);
}

void esperarNotificacion()
{
  if(analogRead(A0)>50)
  {
      Serial.write(codigo);
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("Encendiendo bluetooth")
      encenderBT();
      ucl.write(codigo);
      lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("Conectese");
      estado = 2;
      
  
  }
  
  if((millis() - tiempoultimocambio) > 90000){
    apagar();
  }

}
//espera codigos para envio de datos o apagar bluetooth.
void esperarSenial()
{

  if(ucl.available()>0){
    char lectura = ucl.read();
    Serial.print("Lectura: ");
   Serial.println(lectura);
    if(lectura == 'x'){
      apagar();
    }
  }
  
  if(modulo.available()>0){
      Serial.println("Lect: ");
    char lectura = modulo.read();
 
    if(lectura == 'a'){
      enviarDatos();
    }else{
      if(lectura == 'x'){
        apagar();
      }
    }
  }
  
  // codigo de espera de señal
}
void encenderBT(){
  analogWrite(A1, 189);
  delay(10000);
  modulo.begin(9600);
  delay(2000);
  char ccc[10];
  sprintf(ccc,"AT+PIN%s",codigo);
  modulo.write(ccc);
  Serial.println(ccc);
}

void apagarBT(){
  analogWrite(A1, 0);
}

void enviarDatos(){
  modulo.print(codigo);
  modulo.print(":");
  modulo.println(destino);
Serial.print(codigo);
  Serial.print(":");
  Serial.println(destino);
}

void apagar(){
  
  estado=0;
  destino=-1;
  tiempoultimocambio=millis();
  codigo[0]='0';
  codigo[1]='0';
  codigo[2]='0';
  codigo[3]='0';
  apagarBT();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Seleccione");
  lcd.setCursor(0,1);  
  lcd.print("Destino");
  delay(500);
}

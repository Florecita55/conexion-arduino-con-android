#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <Wire.h> 
#include <Keypad.h>

const byte rows = 4;
const byte cols = 4;
int num;


char codigo[4];
String codigos[15]={"","","","","","","","","","","","","","",""};
int k;
long tiempoultimocambio;
char ncodigo[4];
int currentPosition = 0;
char keys[rows][cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

SoftwareSerial ucs(2, 3); // tx, rx



byte rowPins[rows] = {A0,A1,A2,A3};
byte colPins[cols] = {7,6,5,4};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

LiquidCrystal lcd(13,12,11,10,9,8);

int estado;
/*
0 - Esperando codigo desde el otro arduino
1 - Esperando codigo de usuario
*/

void setup(){
  Serial.begin(9600);
  ucs.begin(9600);
  randomSeed(analogRead(0));
  num=-1;
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(1,0);  
  lcd.write("En espera");
  lcd.setCursor(3,1);
  k = 0;
}

void loop(){
  switch(estado){
    case 0: esperarArduino(); break;
    case 1: esperarCodigo(); break;
  }
}


void esperarArduino(){ //recibe los codigos para almacenarlos
  if(ucs.available()>0){
    if(k == 0){
      k++;
    }else{
    codigo[k-1] = ucs.read();
    k++;
    if(k == 5){
      String cod_aux = "";
      for(int i = 0; i < 4; i++){
        Serial.print(codigo[i]);
        cod_aux += codigo[i];
      }
      Serial.println();

      estado = 1;
      k = 0;
      num=num+1;
      //guardar codigo
      codigos[num]="";
      codigos[num]=cod_aux;
          lcd.clear();
     lcd.setCursor(1,0);
     lcd.write("Codigo:");
     lcd.setCursor(3,1);
      }
    }
  }
  
  if(Serial.available()>0){
    if(k == 1 || k == 0){
      k++;
    }else{
    codigo[k-2] = Serial.read();
    k++;
    if(k == 6){
      String cod_aux = "";
      for(int i = 0; i < 4; i++){
        Serial.print(codigo[i]);
        cod_aux += codigo[i];
      }
      Serial.println();
      estado = 1;
      k = 0;
      num=num+1;
      //guardar codigo
      codigos[num]="";
      codigos[num]=cod_aux;
          lcd.clear();
     lcd.setCursor(1,0);
     lcd.write("Codigo:");
     lcd.setCursor(3,1);
      }
    }
  }
  
}

void esperarCodigo(){ //espera el codigo del keypad y tambien espera mas codigos para almacenarlos
  
  
  char key = keypad.getKey();
  if (int(key) != 0) {
    lcd.write(key);
    ncodigo[currentPosition]=key;
    currentPosition++;
      if (currentPosition == 4)
      {
        int val=valida();
        if(valida()>-1){
          lcd.clear();
          lcd.setCursor(1,0);
          lcd.write("Codigo Correcto :)");
          delay(2000);
          elimina(val);
          currentPosition = 0;
        }
        else
        {
          lcd.clear();
          lcd.setCursor(1,0);
          lcd.write("Codigo incorrecto  ");
          delay(2000);
          initt();
          currentPosition = 0;
        }
      }
    }
    if(Serial.available()>0){
        if(k == 1 || k == 0){
          k++;
        }else{
        codigo[k-2] = Serial.read();
        k++;
        if(k == 6){
          String cod_aux = "";
          for(int i = 0; i < 4; i++){
            Serial.print(codigo[i]);
            cod_aux += codigo[i];
          }
          Serial.println();
          estado = 1;
          k = 0;
          num=num+1;
          //guardar codigo
          codigos[num]="";
          codigos[num]=cod_aux;
          imprimeLista();
          }
        }
      } 
      if(ucs.available()>0){
        if(k == 0){
          k++;
        }else{
        codigo[k-1] = ucs.read();
        k++;
        if(k == 5){
          String cod_aux = "";
          for(int i = 0; i < 4; i++){
            Serial.print(codigo[i]);
            cod_aux += codigo[i];
          }
          Serial.println();
    
          estado = 1;
          k = 0;
          num=num+1;
          //guardar codigo
          codigos[num]="";
          codigos[num]=cod_aux;
              lcd.clear();
         lcd.setCursor(1,0);
         lcd.write("Codigo:");
         lcd.setCursor(3,1);
          }
        }
      } 
}
void initt(){
 lcd.clear();
 lcd.setCursor(1,0);
 lcd.write("Codigo:");
 lcd.setCursor(3,1);
}

int valida(){ //funcion de validacion de codigos
  int i;
  imprimeLista();
  for (i=0;i<15;i++)
  {
    Serial.print(codigos[i]);
    Serial.print("-->");
    Serial.println(ncodigo);
    if (codigos[i]==ncodigo)
    {return i;}
  }
  return -1;
}
void elimina(int ind) //eliminacion de un codigo en el arreglo donde se guardan
{
  for (int i=ind;i<14;i++)
  {
    codigos[i]=codigos[i+1];
  }
  codigos[14]="";
  num=num-1;
  if (num==-1)
  {
    estado=0;
      lcd.clear();
  lcd.setCursor(1,0);  
  lcd.write("En espera");
  lcd.setCursor(3,1);
  }
  else
  {
   lcd.clear();
 lcd.setCursor(1,0);
 lcd.write("Codigo:");
 lcd.setCursor(3,1);
 }
  
}

void imprimeLista(){
  Serial.println("---");
  for(int i = 0; i < 15; i++){
    Serial.println(codigos[i]);
  }
  Serial.println("---");
}


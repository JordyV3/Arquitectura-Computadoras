#include <LiquidCrystal.h>


#include <Keypad.h>
LiquidCrystal lcd(13,12,5,4,3,2);
int frec=1000; //potencia en HZ del buzzer
int lectura=0; //variable para la lectura del sensor de movimiento
const byte ROWS = 4;
const byte COLS = 4;
char tecla;
int teclasIngresadas=0, intentos=0, intentosR=5;
char* contrasena="1234";
int x=0;
boolean alarmaActivada;

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {11,10,9,1};
byte colPins[COLS] = {0,A0,A1,A2};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup(){
	pinMode(8,OUTPUT); //buzzer
	pinMode(7,INPUT); //sensor
  	pinMode(6,OUTPUT); //led externo
  	lcd.begin(16, 2); //iniciar LCD con 16 caracteres en dos líneas
}
void loop(){
  lectura=digitalRead(7);
  if (lectura==HIGH && intentos!=5){ //si detecta movimiento
    lcd.setCursor(0,0); //pararse en la primera línea del display
    lcd.print("ALARMA ACTIVADA"); //mensaje a imprimir en LCD
    lcd.setCursor(0,1); //pararse en la segunda línea del display
    lcd.print("INGRESE EL CONTRASEÑA."); //mensaje a imprimir en LCD
    tone(8, frec); //buzzing
    digitalWrite(6, HIGH);
    x=0;//esta línea nos permite tan solo ingresar CONTRASEÑA cuando el sensor deja de detectar movimiento.
  	alarmaActivada=true;
  }
    tecla=customKeypad.getKey(); //obtiene digitos desde keypad
  if(tecla){
  	teclasIngresadas++;
  }
  if(tecla==contrasena[x]){ //si coincide digito a digito
     x++; //aumentamos x: cuantas veces coincide el pin con los digitos que ingresa el usuario (de menor a mayor)
     lcd.setCursor(0,12);
     lcd.print("PUERTA ABIERTA");
  }
  if (x==4 || alarmaActivada==false) {//coinciden los 4 digitos
    digitalWrite(6, LOW); //dejar apagado led externo
    	noTone(8); //dejar apagado buzzer
    	lcd.setCursor(0,0); //pararse en la primera línea del display
    	lcd.print("   PUERTA CERRADA    "); //mensaje a imprimir en LCD
    	lcd.setCursor(0,1); //pararse en la segunda línea del display
    	lcd.print(" SEGURIDAD CASA "); //mensaje a imprimir en LCD
    	alarmaActivada=false;
    	x=0;
    	teclasIngresadas=0;
    	intentosR=5;
    	intentos=0;
    }
  if(teclasIngresadas==4 && teclasIngresadas!=0 && x!=4 && alarmaActivada==true){
    lcd.setCursor(0,0); //pararse en la primera línea del display
    lcd.print("ERROR DE CONTRASEÑA ! !"); //mensaje a imprimir en LCD
    lcd.setCursor(0,1); //pararse en la primera línea del display
    lcd.print(intentosR-1);
    lcd.print(" INTENTOS FALLIDOS!!");//mensaje a imprimir en LCD
    intentos++;
    if(x!=4)
    intentosR--;
   	teclasIngresadas=0;
  }
  if(intentos==5){
    tone(8, frec);
    lcd.setCursor(0,0); //pararse en la primera línea del display
    lcd.print("ALARMA ACTIVADA!"); //mensaje a imprimir en LCD
    lcd.setCursor(0,1); //pararse en la primera línea del display
    lcd.print("SE LLAMA AL FBI!");//mensaje a imprimir en LCD
    digitalWrite(6, HIGH);
    delay(700);
    digitalWrite(6, LOW);
    delay(700);
    lectura==LOW;
    x=5;
    teclasIngresadas=5;
  }	
}
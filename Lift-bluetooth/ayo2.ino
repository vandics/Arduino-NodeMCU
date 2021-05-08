#include <EEPROM.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#define enA 11 //port pwm motor
#define in1 9 //port motor
#define in2 10 //port motor
#define IR1 5
#define IR2 6
#define butt1 2
#define butt2 3
#define butt3 4
#define LED1 11
#define LED2 12
#define LED3 13

LiquidCrystal_I2C lcd(0x27, 16, 2);


SoftwareSerial BTserial(8,7); 

int butts1=0;
int butts2=0;
int butts3 = 0;
boolean state1;
boolean state2;
boolean state3;
int irs1;
int irs2;
int LEDS1;
int LEDS2;
int LEDS3;
int lantaisekarang;
int lantai;
int lantaiini;

boolean one = true;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  BTserial.begin(9600); 
  pinMode(IR1,INPUT);
  pinMode(IR2,INPUT);
  pinMode(butt1,INPUT);
  pinMode(butt2,INPUT);
  pinMode(butt3,INPUT);
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("SMART LIFT");
  delay(1000);
  lantaiini = EEPROM.get(161,lantai);
  
  char posisi=lantaiini;
  BTserial.write(posisi);
  lcd.clear();
  lcd.print("kita dilantai ");
  lcd.print(lantaiini);
}

void loop() {
  // put your main code here, to run repeatedly:
  butts1 = digitalRead(butt1);
  butts2 = digitalRead(butt2);
  butts3 = digitalRead(butt3);
  irs1 = digitalRead(IR1);
  irs2 = digitalRead(IR2);

  if(BTserial.available()>0){
   char character = BTserial.read();
    switch(character){
      case 'a':
        butts1 = HIGH;
        break;
      case 'b':
        butts2 = HIGH;
        break;
      case 'c':
        butts3 = HIGH;
        break;
      default:
        break;
      }
    }


  if(butts1 == HIGH){
    state1 = true;
    lantai = 1;
    EEPROM.put(161,lantai);
    BTserial.write("1");
    if(state1 == true){ 
      bandingkanlantai(lantai);
      digitalWrite(LED1,HIGH);
      }
  }
  else if(state1 == true && irs1 == HIGH && irs2 == LOW){
    analogWrite(in1,0);
    analogWrite(in2,0);
    state1 = false;
    digitalWrite(LED1,LOW);
    lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Sudah sampai ");
      lcd.setCursor(0,1);
      lcd.print("di lantai ");
      lcd.print(lantai);
    }


  if(butts2 == HIGH){
    state2 = true;
    lantai = 2;
    BTserial.write("2");
    EEPROM.put(161,lantai);
    if(state2 == true){
      //analogWrite(in1,200);
      //analogWrite(in2,0);
      bandingkanlantai(lantai);
      digitalWrite(LED2,HIGH);
      }
    }
    else if(state2 == true && irs2 == HIGH && irs1 == LOW){
      analogWrite(in1,0);
      analogWrite(in2,0);
      state2 = false;
      digitalWrite(LED2,LOW);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Sudah sampai ");
      lcd.setCursor(0,1);
      lcd.print("di lantai ");
      lcd.print(lantai);
      }
      
  if(butts3 == HIGH){
    state3 = true;
    lantai = 3;
    BTserial.write("3");
    EEPROM.put(161,lantai);
    if(state3 == true){
      bandingkanlantai(lantai);
      digitalWrite(LED3,HIGH);
      }
    }
    else if(state3 == true && irs1 == HIGH){
      analogWrite(in1,0);
      analogWrite(in2,0);
      state3 = false;
      digitalWrite(LED3,LOW);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Sudah sampai ");
      lcd.setCursor(0,1);
      lcd.print("di lantai ");
      lcd.print(lantai);
      }


}

void bandingkanlantai(int lantaitujuan){
  lcd.init();
  if(one == true){
    lantaiini = EEPROM.get(161,lantai);;
    one = false;
    }


  if(lantaitujuan>lantaiini){
    for(int i=0;i<100;i++){
    analogWrite(in1,90);
    analogWrite(in2,0);
    lcd.setCursor(0,0);
    lcd.print("Sedang menuju ");
    lcd.setCursor(0,1);
    lcd.print("ke lantai ");
    lcd.print(lantaitujuan);
      }
      lantaiini = lantaitujuan;
    }
  if(lantaitujuan<lantaiini){
    for(int i=0;i<50;i++){
    analogWrite(in1,0);
    analogWrite(in2,30);
    lcd.setCursor(0,0);
    lcd.print("Sedang menuju ");
    lcd.setCursor(0,1);
    lcd.print("ke lantai ");
    lcd.print(lantaitujuan);
    }
    lantaiini = lantaitujuan;
    }

  if(lantaitujuan == lantaiini){
    

    }  

  //delay(1500);
        
}
  


  

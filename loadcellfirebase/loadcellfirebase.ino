#include <ESP8266WiFi.h>
#include<FirebaseArduino.h>
#include "HX711.h"
#include <LiquidCrystal_I2C.h>

#define FIREBASE_HOST "monitoringkapasitasgas-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "tRRipSUSCrFY8SHSO1Dbf4CimEqsrgzVoCazoY7S"
#define WIFI_SSID "KOS_32"
#define WIFI_PASSWORD "karisidenan99"
#define dout D5
#define sck D6
int persen;

LiquidCrystal_I2C lcd(0x27, 16, 2);
//hubungkan pin SCL lcd I2C ke pin D1 nodemcu, hubungkan pin SDA lcd I2C ke pin D2 nodemcu
//library LCD I2C silahkan download disini : https://www.dropbox.com/s/asp8w5ja64bn7u9/Arduino-LiquidCrystal-I2C-library-master.zip
 
HX711 scale;
float weight;
float calibration_factor = 210; // for me this vlaue works just perfect 419640

void setup()
{
  Serial.begin(115200);      // Set the baud rate 115200 in Serial Monitor to display debug messages
  scale.begin(dout, sck);
  scale.set_scale();
  scale.tare(); //Reset the scale to 0
  long zero_factor = scale.read_average(); //Get a baseline reading

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected:");
  Serial.println(WiFi.localIP());

  lcd.init();
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("connected");
  

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {

  if (Firebase.failed()) {
    Serial.print("Firebase Error");
    Serial.println(Firebase.error());
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Firebase Error");
    setup();
  }

  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  weight = scale.get_units(4);
  weight = constrain(weight, 5000, 8000);
  Serial.println(weight);
  persen=map(weight,5000,8000,0,100);
  Serial.println(persen);
  Firebase.setString("tabung/Kapasitas", String(persen));
  Firebase.setString("tabung/Pesan", "1");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Kapasitas tabung");
  lcd.setCursor(0,1);
  lcd.print(String(persen) + "% atau " + String(weight/1000) + "Kg")
}

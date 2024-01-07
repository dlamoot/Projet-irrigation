#include <MKRWAN.h>
#include "ArduinoLowPower.h"
#include "Air_Hum_temp.hpp"
#include "Hum_sol.hpp"
#include "Luminosite.hpp"
#include "Temp_sol.hpp"
#include "Electrovanne.hpp"
#include "Batterie.hpp"
#include "Allumer_ESP.hpp"
#include "Conversion.hpp"

//int Done = 11;
#define enableCapteur 12
#define SHDN  8
#define Sleep  2
#define enableESP 10

//Duree
int frequenceCapteur = 600000; // 10 minutes en ms
int frequenceEsp = 43200000; //12 heures en ms
int dureeOuvertureElectrovanne = 5; //5 secondes
unsigned long currentMillis = 43200000; //Pour prendre une photo au démarage du système
unsigned long previousMillis = 0;

LoRaModem modem;

//Find this on your TTN account 
String appEui = "";
String appKey = "";

bool connected;
int err_count;
short con; 
void setup() {
  //Serial.begin(115200);
  setup_electrovanne();
  setup_dht();
  setup_lum();
  //pinMode(Done,OUTPUT);
  //pinMode(enableCapteur,OUTPUT);
  pinMode(SHDN,OUTPUT);
  pinMode(Sleep,OUTPUT);
  pinMode(enableESP,OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  //while (!Serrial);
  modem.begin(EU868);
  delay(1000);
  connected=false;
  err_count=0;
  con =0;
}

void loop() {
  //Démarrage système
  digitalWrite(LED_BUILTIN, HIGH);

  digitalWrite(enableCapteur,HIGH); //Alimentation des capteurs activée
  //digitalWrite(Delay,HIGH);
  delay(1000);
  digitalWrite(SHDN,LOW); //Alimentation pont en H désactivée
  digitalWrite(Sleep,LOW); //Pont en H en mode sleep
  digitalWrite(enableESP,LOW); //ESP non alimenté
  delay(1000);

  //Récupération donnée capteur
  short hum_air_short = (short)(humidity_dht());
  short temp_air_short = (short)(temp_dht());
  short temp_sol_short = (short)(temp_sol());
  delay(500);
  temp_sol_short = (short)(temp_sol());
  short hum_sol_short = (short) (hum_sol());
  short lum_short = (short) (lum());
  short battery_short = (short) (battery());
  short electrovanne = (short) 0; //électrovanne fermée

  //Arrosage
  if (hum_sol()/10 < 25){ //Arrosage si humidité inférieur à 25%
    digitalWrite(SHDN,HIGH); //Alimentation pont en H
    digitalWrite(Sleep,HIGH); //Pont en H quitte le mode sleep
    ouverture_fermeture(dureeOuvertureElectrovanne);
    digitalWrite(SHDN,LOW); //Coupe alimentation pont en H
    delay(500);
    digitalWrite(Sleep,LOW); //Pont en H en mode sleep
    electrovanne = (short) 1; // électrovanne ouverte
  }

  //ESP
  if (currentMillis - previousMillis >= frequenceEsp){ //Prend une photo à chaque fois que l'intervalle écoulé est passé
    currentMillis = millis();
    previousMillis = currentMillis;
    allumer_ESP();
  }

  //Envoi données
  char msg[2] = {3,4};
  short test = 27; 
  if ( !connected ) {
    //Serial.print("Join test : ");
    //Serial.println(++con);
    int ret=modem.joinOTAA(appEui, appKey);
    if ( ret ) {
      connected=true;
      modem.minPollInterval(60);
      //Serial.println("Connected");
      modem.dataRate(5);   // switch to SF7
      delay(100);          // because ... more stable
      err_count=0;
    }
  }
  if ( connected ) {
    int err=0;
    modem.beginPacket();
    modem.write(temp_air_short);
    modem.write(hum_air_short);
    modem.write(temp_sol_short);
    modem.write(hum_sol_short);
    modem.write(lum_short);
    modem.write(battery_short);
    modem.write(electrovanne);

    err = modem.endPacket();
    if ( err <= 0 ) {
      //Serial.print("Error : ");
      //Serial.println(err);
      // Confirmation not received - jam or coverage fault
      err_count++;
      if ( err_count > 50 ) {
        connected = false;
      }
      // wait for 2min for duty cycle with SF12 - 1.5s frame
      for ( int i = 0 ; i < 120 ; i++ ) {
        delay(1000);
      }
    } else {
      err_count = 0;
      // wait for 20s for duty cycle with SF7 - 55ms frame
      delay(20000);
      //Serial.println("Message envoyé");  
    } 
  }

  //Downlinks
  char rcv[64];
  int i = 0;
  while (modem.available()){
    rcv[i++] = (char) modem.read();
  }
  if ((int)rcv[0] == 1){
    frequenceCapteur = minutesToMilliseconds((int)rcv[1]);
  }
  else if (rcv[0] == 2){
    frequenceEsp = hoursToMilliseconds((int)rcv[1]);
  }
  else if (rcv[0] == 3){
    dureeOuvertureElectrovanne = (int)rcv[1];
  }

  //Coupure alimentation capteur
  digitalWrite(enableCapteur,LOW); 
  
  //Duree de fonctionnement du systeme
  currentMillis = millis();

  //Consommation
  if (battery()/100 < 20){
      digitalWrite(LED_BUILTIN, LOW);
      LowPower.deepSleep(frequenceCapteur);
  }
  else{
      digitalWrite(LED_BUILTIN, LOW);
      LowPower.sleep(frequenceCapteur);
  }
  //digitalWrite(Delay,LOW);
  //digitalWrite(enableCapteur,LOW); 
  //delay(60000);
}
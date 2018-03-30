/***************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board:
  ----> https://www.adafruit.com/product/2471

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/*************************  Point d'Acces Wi-fi *********************************/

#define WLAN_SSID       "UTILISATEUR"
#define WLAN_PASS       "MOT DE PASSE"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "ADRESSE BROKER"
#define AIO_SERVERPORT  PORTE
#define AIO_USERNAME    "UTILISATEUR"
#define AIO_KEY         "MOT DE PASSE"

/************************ Bande de LED ********************************/

#define PIN        D4        //GPIO used to send the data to the LED
#define N_LEDS     60      //Number of LEDs = (number of Etages)*(number of LEDS per Etage) = 5*12 = 60 LEDS


/************************ Mode de DEBUG ********************************/

#define DEBUG 1

/************ Ticker ******************/

String message;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);


/************ Global State (you don't need to change this!) ******************/
// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);

/****************************** Publish ***************************************/

Adafruit_MQTT_Subscribe *subscription;


/****************************** Feeds ***************************************/

// Setup a feed called 'onoff' for subscribing to changes to the button
Adafruit_MQTT_Subscribe tourS1 = Adafruit_MQTT_Subscribe(&mqtt, "/12/Etage1", MQTT_QOS_1);
Adafruit_MQTT_Subscribe tourS2 = Adafruit_MQTT_Subscribe(&mqtt, "/12/Etage2", MQTT_QOS_1);
Adafruit_MQTT_Subscribe tourS3 = Adafruit_MQTT_Subscribe(&mqtt, "/12/Etage3", MQTT_QOS_1);

//Remarque: Nous avons chosi QoS = 1 (at least one). Pour savoir les niveaux de QoS, acceder: https://www.hivemq.com/blog/mqtt-essentials-part-6-mqtt-quality-of-service-levels
/************************* Tour Parametres *************************/


/*************************** Sketch Code ************************************/
//



//Ici nous déclarons les fonctions callback (CB) de la tour pour chaque étage
void tourCB1(char *data, uint16_t len) {

  if (DEBUG) Serial.print("* DEBUG: Message reçue - Étage1:  ");
  if (DEBUG) Serial.println(data);
  traitement1(data, len);

}

void tourCB2(char *data, uint16_t len) {

  if (DEBUG) Serial.print("* DEBUG: Message reçue - Étage2:  ");
  if (DEBUG) Serial.println(data);
  traitement2(data, len);

}

void tourCB3(char *data, uint16_t len) {

  if (DEBUG) Serial.print("* DEBUG: Message reçue - Étage3:  ");
  if (DEBUG) Serial.println(data);
  traitement3(data, len);

}

// Ici nous faisons le traitement de chaque entrée
void traitement1(char *data, uint16_t len) {
  int k = 0;
  int j1 = 0;
  int j2 = 0;
  int j3 = 0;
  String chaine1[16];
  String chaine3[16];

//Lire le message jusqu'à W
  while (data[k] != 'W') {

    if (DEBUG)     Serial.println(" ");
    if (DEBUG)     Serial.println("------------MISE EN CHAINE------------");
    k++;
    while (data[k] != 'T'  &&  data[k] != 'L' &&  data[k] != 'O' &&  data[k] != 'R'  &&  data[k] != 'S'  &&  data[k] != 'P'  &&  data[k] != 'W') {
      if (k == 0)  message = String(data[k]);
      else   message =  message + String(data[k]);
      k++;
      delay(20);
    }

    chaine3[j3++] = data[k];
    chaine1[j1++] = message;


    if (DEBUG)     Serial.println(" ");
    if (DEBUG)     Serial.print("*DEBUG: CHAINE1: ");
    if (DEBUG)     if (j1 != 0) Serial.print(chaine1[j1 - 1]);
    if (DEBUG)     Serial.print(" ");
    if (DEBUG)     Serial.print("ELEMENTS: ");
    if (DEBUG)     Serial.println(j1);
    if (DEBUG)     Serial.print("*DEBUG: CHAINE3: ");
    if (DEBUG)     if (j3 != 0) Serial.print(chaine3[j3 - 1]);
    if (DEBUG)     Serial.print(" ");
    if (DEBUG)     Serial.print("ELEMENTS: ");
    if (DEBUG)     Serial.println(j3);


    message = "";
    delay(20);

  }
//On demarre les effects de la bande de LED
  int r, g, b;
  int r1, g1, b1;
  int r2, g2, b2;
  int m3 = 0;
  int m1 = 0;
  int m2 = 0;
  int temps = chaine1[0].toInt();

  if (DEBUG)     Serial.println(" ");
  if (DEBUG)     Serial.print("*DEBUG  Temps: ");
  if (DEBUG)     Serial.println(temps);

  while (chaine3[m3] != "W") {
    if (chaine3[m3] == "L") {
      int *rgb = hexaToRGB(chaine1[m1 + 1]);
      r = rgb[0];
      g = rgb[1];
      b = rgb[2];
      if (DEBUG)     Serial.print("*DEBUG  RGB Allumer:  ");
      if (DEBUG)     Serial.print(r); Serial.print(","); Serial.print(g); Serial.print(","); Serial.println(b);
      Etage1(r, g, b);
      delay(temps);
      m1++;
    }
    if (chaine3[m3] == "P") {
      int *rgb = hexaToRGB(chaine1[m1 + 1]);
      r = rgb[0];
      g = rgb[1];
      b = rgb[2];
      if (DEBUG)     Serial.print("*DEBUG  RGB Clignoter:  ");
      if (DEBUG) Serial.print(r); Serial.print(","); Serial.print(g); Serial.print(","); Serial.println(b);
      int temp = temps;
      while (temp > 0) {
        Etage1(r, g, b);
        delay(40);
        Etage1(0, 0, 0);
        delay(40);
        temp = temp - 80;
      }
      m1++;
    }
    if (chaine3[m3] == "O") {
      Etage1(0, 0, 0);
      delay(temps);
      m1++;
    }
    if (chaine3[m3] == "R") {
      int *rgb1 = hexaToRGB(chaine1[m1 + 1]);
      r1 = rgb1[0];
      g1 = rgb1[1];
      b1 = rgb1[2];
      if (DEBUG)     Serial.print("*DEBUG  RGB Degradé 1:  ");
      if (DEBUG)     Serial.print(r1); Serial.print(","); Serial.print(g1); Serial.print(","); Serial.println(b1);
      m3++;
      m1++;
      int *rgb2 = hexaToRGB(chaine1[m1 + 1]);
      r2 = rgb2[0];
      g2 = rgb2[1];
      b2 = rgb2[2];
      if (DEBUG)     Serial.print("*DEBUG  RGB Degradé:  ");
      if (DEBUG)     Serial.print(r2); Serial.print(","); Serial.print(g2); Serial.print(","); Serial.println(b2);
      int temp = temps;
      int rc, gc, bc;
      if (r1 > r2) rc = -1;
      else rc = 1;
      if (g1 > g2) gc = -1;
      else gc = 1;
      if (b1 > b2) bc = -1;
      else bc = 1;

      Etage1(r1, g1, b1);
      delay(temps / 5);

      bool fin = false;
      int transR = r1;
      int transG = g1;
      int transB = b1;

      while (!fin) {
        Etage1(transR, transG, transB);
        delay(10);
        if (transR != r2) transR = transR + rc;
        if (transG != g2) transG = transG + gc;
        if (transB != b2) transB = transB + bc;
        if (transR == r2 &&  transG == g2 && transB == b2) fin = true;

        if (DEBUG)     Serial.print("*DEBUG  Trans R, Trans G, Trans B;  ");
        if (DEBUG)     Serial.print(transR); Serial.print(","); Serial.print(transG); Serial.print(","); Serial.println(transB);

      }

      Etage1(r2, g2, b2);
      delay(temps / 5);
      m1++;
    }
    m3++;
  }
}


void traitement2(char *data, uint16_t len) {
  int k = 0;
  int j1 = 0;
  int j2 = 0;
  int j3 = 0;
  String chaine1[16];
  String chaine3[16];


  while (data[k] != 'W') {

    if (DEBUG)     Serial.println(" ");
    if (DEBUG)     Serial.println("------------MISE EN CHAINE------------");
    k++;
    while (data[k] != 'T'  &&  data[k] != 'L' &&  data[k] != 'O' &&  data[k] != 'R'  &&  data[k] != 'S'  &&  data[k] != 'P'  &&  data[k] != 'W') {
      if (k == 0)  message = String(data[k]);
      else   message =  message + String(data[k]);
      k++;
      delay(20);
    }

    chaine3[j3++] = data[k];
    chaine1[j1++] = message;


    if (DEBUG)     Serial.println(" ");
    if (DEBUG)     Serial.print("*DEBUG: CHAINE1: ");
    if (DEBUG)     if (j1 != 0) Serial.print(chaine1[j1 - 1]);
    if (DEBUG)     Serial.print(" ");
    if (DEBUG)     Serial.print("ELEMENTS: ");
    if (DEBUG)     Serial.println(j1);
    if (DEBUG)     Serial.print("*DEBUG: CHAINE3: ");
    if (DEBUG)     if (j3 != 0) Serial.print(chaine3[j3 - 1]);
    if (DEBUG)     Serial.print(" ");
    if (DEBUG)     Serial.print("ELEMENTS: ");
    if (DEBUG)     Serial.println(j3);


    message = "";
    delay(20);

  }

  int r, g, b;
  int r1, g1, b1;
  int r2, g2, b2;
  int m3 = 0;
  int m1 = 0;
  int m2 = 0;
  int temps = chaine1[0].toInt();

  if (DEBUG)     Serial.println(" ");
  if (DEBUG)     Serial.print("*DEBUG  Temps: ");
  if (DEBUG)     Serial.println(temps);

  while (chaine3[m3] != "W") {
    if (chaine3[m3] == "L") {
      int *rgb = hexaToRGB(chaine1[m1 + 1]);
      r = rgb[0];
      g = rgb[1];
      b = rgb[2];
      if (DEBUG)     Serial.print("*DEBUG  RGB Allumer:  ");
      if (DEBUG)     Serial.print(r); Serial.print(","); Serial.print(g); Serial.print(","); Serial.println(b);
      Etage2(r, g, b);
      delay(temps);
      m1++;
    }
    if (chaine3[m3] == "P") {
      int *rgb = hexaToRGB(chaine1[m1 + 1]);
      r = rgb[0];
      g = rgb[1];
      b = rgb[2];
      if (DEBUG)     Serial.print("*DEBUG  RGB Clignoter:  ");
      if (DEBUG) Serial.print(r); Serial.print(","); Serial.print(g); Serial.print(","); Serial.println(b);
      int temp = temps;
      while (temp > 0) {
        Etage2(r, g, b);
        delay(40);
        Etage2(0, 0, 0);
        delay(40);
        temp = temp - 80;
      }
      m1++;
    }
    if (chaine3[m3] == "O") {
      Etage2(0, 0, 0);
      delay(temps);
      m1++;
    }
    if (chaine3[m3] == "R") {
      int *rgb1 = hexaToRGB(chaine1[m1 + 1]);
      r1 = rgb1[0];
      g1 = rgb1[1];
      b1 = rgb1[2];
      if (DEBUG)     Serial.print("*DEBUG  RGB Degradé 1:  ");
      if (DEBUG)     Serial.print(r1); Serial.print(","); Serial.print(g1); Serial.print(","); Serial.println(b1);
      m3++;
      m1++;
      int *rgb2 = hexaToRGB(chaine1[m1 + 1]);
      r2 = rgb2[0];
      g2 = rgb2[1];
      b2 = rgb2[2];
      if (DEBUG)     Serial.print("*DEBUG  RGB Degradé:  ");
      if (DEBUG)     Serial.print(r2); Serial.print(","); Serial.print(g2); Serial.print(","); Serial.println(b2);
      int temp = temps;
      int rc, gc, bc;
      if (r1 > r2) rc = -1;
      else rc = 1;
      if (g1 > g2) gc = -1;
      else gc = 1;
      if (b1 > b2) bc = -1;
      else bc = 1;

      Etage2(r1, g1, b1);
      delay(temps / 5);

      bool fin = false;
      int transR = r1;
      int transG = g1;
      int transB = b1;

      while (!fin) {
        Etage2(transR, transG, transB);
        delay(10);
        if (transR != r2) transR = transR + rc;
        if (transG != g2) transG = transG + gc;
        if (transB != b2) transB = transB + bc;
        if (transR == r2 &&  transG == g2 && transB == b2) fin = true;

        if (DEBUG)     Serial.print("*DEBUG  Trans R, Trans G, Trans B;  ");
        if (DEBUG)     Serial.print(transR); Serial.print(","); Serial.print(transG); Serial.print(","); Serial.println(transB);

      }

      Etage2(r2, g2, b2);
      delay(temps / 5);
      m1++;
    }
    m3++;
  }
}


void traitement3(char *data, uint16_t len) {
  int k = 0;
  int j1 = 0;
  int j2 = 0;
  int j3 = 0;
  String chaine1[16];
  String chaine3[16];


  while (data[k] != 'W') {

    if (DEBUG)     Serial.println(" ");
    if (DEBUG)     Serial.println("------------MISE EN CHAINE------------");
    k++;
    while (data[k] != 'T'  &&  data[k] != 'L' &&  data[k] != 'O' &&  data[k] != 'R'  &&  data[k] != 'S'  &&  data[k] != 'P'  &&  data[k] != 'W') {
      if (k == 0)  message = String(data[k]);
      else   message =  message + String(data[k]);
      k++;
      delay(20);
    }

    chaine3[j3++] = data[k];
    chaine1[j1++] = message;


    if (DEBUG)     Serial.println(" ");
    if (DEBUG)     Serial.print("*DEBUG: CHAINE1: ");
    if (DEBUG)     if (j1 != 0) Serial.print(chaine1[j1 - 1]);
    if (DEBUG)     Serial.print(" ");
    if (DEBUG)     Serial.print("ELEMENTS: ");
    if (DEBUG)     Serial.println(j1);
    if (DEBUG)     Serial.print("*DEBUG: CHAINE3: ");
    if (DEBUG)     if (j3 != 0) Serial.print(chaine3[j3 - 1]);
    if (DEBUG)     Serial.print(" ");
    if (DEBUG)     Serial.print("ELEMENTS: ");
    if (DEBUG)     Serial.println(j3);


    message = "";
    delay(20);

  }

  int r, g, b;
  int r1, g1, b1;
  int r2, g2, b2;
  int m3 = 0;
  int m1 = 0;
  int m2 = 0;
  int temps = chaine1[0].toInt();

  if (DEBUG)     Serial.println(" ");
  if (DEBUG)     Serial.print("*DEBUG  Temps: ");
  if (DEBUG)     Serial.println(temps);

  while (chaine3[m3] != "W") {
    if (chaine3[m3] == "L") {
      int *rgb = hexaToRGB(chaine1[m1 + 1]);
      r = rgb[0];
      g = rgb[1];
      b = rgb[2];
      if (DEBUG)     Serial.print("*DEBUG  RGB Allumer:  ");
      if (DEBUG)     Serial.print(r); Serial.print(","); Serial.print(g); Serial.print(","); Serial.println(b);
      Etage3(r, g, b);
      delay(temps);
      m1++;
    }
    if (chaine3[m3] == "P") {
      int *rgb = hexaToRGB(chaine1[m1 + 1]);
      r = rgb[0];
      g = rgb[1];
      b = rgb[2];
      if (DEBUG)     Serial.print("*DEBUG  RGB Clignoter:  ");
      if (DEBUG) Serial.print(r); Serial.print(","); Serial.print(g); Serial.print(","); Serial.println(b);
      int temp = temps;
      while (temp > 0) {
        Etage3(r, g, b);
        delay(40);
        Etage3(0, 0, 0);
        delay(40);
        temp = temp - 80;
      }
      m1++;
    }
    if (chaine3[m3] == "O") {
      Etage3(0, 0, 0);
      delay(temps);
      m1++;
    }
    if (chaine3[m3] == "R") {
      int *rgb1 = hexaToRGB(chaine1[m1 + 1]);
      r1 = rgb1[0];
      g1 = rgb1[1];
      b1 = rgb1[2];
      if (DEBUG)     Serial.print("*DEBUG  RGB Degradé 1:  ");
      if (DEBUG)     Serial.print(r1); Serial.print(","); Serial.print(g1); Serial.print(","); Serial.println(b1);
      m3++;
      m1++;
      int *rgb2 = hexaToRGB(chaine1[m1 + 1]);
      r2 = rgb2[0];
      g2 = rgb2[1];
      b2 = rgb2[2];
      if (DEBUG)     Serial.print("*DEBUG  RGB Degradé:  ");
      if (DEBUG)     Serial.print(r2); Serial.print(","); Serial.print(g2); Serial.print(","); Serial.println(b2);
      int temp = temps;
      int rc, gc, bc;
      if (r1 > r2) rc = -1;
      else rc = 1;
      if (g1 > g2) gc = -1;
      else gc = 1;
      if (b1 > b2) bc = -1;
      else bc = 1;

      Etage3(r1, g1, b1);
      delay(temps / 5);

      bool fin = false;
      int transR = r1;
      int transG = g1;
      int transB = b1;

      while (!fin) {
        Etage3(transR, transG, transB);
        delay(10);
        if (transR != r2) transR = transR + rc;
        if (transG != g2) transG = transG + gc;
        if (transB != b2) transB = transB + bc;
        if (transR == r2 &&  transG == g2 && transB == b2) fin = true;

        if (DEBUG)     Serial.print("*DEBUG  Trans R, Trans G, Trans B;  ");
        if (DEBUG)     Serial.print(transR); Serial.print(","); Serial.print(transG); Serial.print(","); Serial.println(transB);

      }

      Etage3(r2, g2, b2);
      delay(temps / 5);
      m1++;
    }
    m3++;
  }
}



void setup() {



  Serial.begin(115200);
  delay(10);

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

  }
  Serial.println();

  Serial.println("WiFi connected");
  if (DEBUG) Serial.println("IP address: "); Serial.println(WiFi.localIP());

//Fonctions Callback
    tourS1.setCallback(tourCB1);
    tourS2.setCallback(tourCB2);
    tourS3.setCallback(tourCB3);


  // Setup MQTT subscription for time feed.

  mqtt.subscribe(&tourS1);
  mqtt.subscribe(&tourS2);
  mqtt.subscribe(&tourS3);

  if (DEBUG) Serial.println("Fite de LED demarée");

//On definit les LED's et les étaindre
  strip.begin();
  Etage3(0, 0, 0);
  Etage2(0, 0, 0);
  Etage1(0, 0, 0);
  strip.show();



}

uint32_t x = 0;




void loop() {

  MQTT_connect(); //Fonction qui permet la stabilité de connexion
  Serial.print("."); // Affiche une message en attente
  Etage4(0,0,255); //Pas obligatoire. Dans le cas réel, il ne sera pas allumé.
  mqtt.processPackets(10000); //Callback 


}



// Function pour la recconexion
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}


//Fonctions de bande de LED pour chaque étage

/* Etage 1 */
void Etage1(int rouge, int vert, int bleu) {

  for (int i = 0; i <= 11; i++) {
    strip.setPixelColor(i, strip.Color(rouge, vert, bleu));
  }
  strip.show();
}

/* Etage 2 */
void Etage2(int rouge, int vert, int bleu) {
  for (int i = 12; i <= 23; i++) {
    strip.setPixelColor(i, strip.Color(rouge, vert, bleu));
  }
  strip.show();
}

/* Etage 3 */
void Etage3(int rouge, int vert, int bleu) {
  for (int i = 24; i <= 35; i++) {
    strip.setPixelColor(i, strip.Color(rouge, vert, bleu));
  }
  strip.show();
}

/* Etage 4 */
void Etage4(int rouge, int vert, int bleu) {
  for (int i = 48; i <= 59; i++) {
    strip.setPixelColor(i, strip.Color(rouge, vert, bleu));
  }
  strip.show();
}


//Fonction qui permet convertir Hexa to RGB

int* hexaToRGB (String etage) {
  etage = "#" + etage;
  if (DEBUG)  Serial.print("*DEBUG: Valeur en Hexa:");
  if (DEBUG)  Serial.println(etage);
  long number = (long) strtol( &etage[1], NULL, 16);
  int r = number >> 16;
  int g = number >> 8 & 0xFF;
  int b = number & 0xFF;
  if (DEBUG) Serial.print("*DEBUG: Valeur en RGB: ");
  if (DEBUG) Serial.print(r); Serial.print(","); Serial.print(g); Serial.print(","); Serial.println(b);
  int *rgb = new int[3] ;
  rgb[0] = r;
  rgb[1] = g;
  rgb[2] = b;

  return rgb;
}

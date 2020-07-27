#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
//Librerias para OTA
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "../lib/ota.h"
#include "../lib/interfazweb.h"
#include "../lib/reloj.h"

//Librerias para OTA

conexionWeb *webInterface;


DateTime *reloj;
StaticJsonDocument<512> horaActual;
StaticJsonDocument<512> post_ejemplo;
StaticJsonDocument<1024> get_ejemplo;

const char *urlPost = "http://192.168.1.74/esp32/public/api/sensores";
const char *geturl = "http://192.168.99.100/app/public/api/resetpassword";

void setup()
{
  Serial.begin(115200);
  setupOTA("TemplateSketch");

  webInterface = new conexionWeb(ssid, passwrd);
  reloj = new DateTime();

  pinMode(2, OUTPUT);
}
int sumatoria = 0;
void loop()
{
#ifndef ESP32_RTOS
  ArduinoOTA.handle();
#endif
  sumatoria++;

  horaActual.clear();
  post_ejemplo.clear();
  reloj->getTime();

  horaActual["hora"] = reloj->timeStringBuff;
  horaActual["Sumatoria"] = sumatoria;

 String Direc[]={"Local","Jojutla","Galeana","Zacatepec",};
  post_ejemplo["idpedidos"]=random(10);
  post_ejemplo["Cantidad"]=random(10);
  post_ejemplo["Tipo_Pedido"]= Direc[random(4)];
  post_ejemplo["Estado_Pedido"]="PREPARACION";
  post_ejemplo["FK_idProd"]=random(54);
  post_ejemplo["FK_idCli"]=random(15);

  webInterface->webPOST(post_ejemplo, urlPost);
  //  webInterface->webGET(geturl);

  deserializeJson(get_ejemplo, webInterface->POSTresponse);
  serializeJsonPretty(get_ejemplo, Serial);
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  
  // serializeJson(horaActual,Serial);
}

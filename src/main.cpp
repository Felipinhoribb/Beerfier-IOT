#define BLYNK_PRINT Serial

#include <Arduino.h>
#include <Servo.h>
#include <HX711.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//DEFINIÇÕES DO BLYNK E AUTENTICAÇÃO DO WIFI
char auth[] = "R3b6mKoiKvR4Psa-p-Bs3k-U68hJC1PY";
char ssid[] = "2G LUIZ E ANA";
char pass[] = "9751836100ribeirO";

//CONFIGURAÇÃO DOS PINOS

// Motor
const int PINO_MOTOR = D4;

//BOMBA
const int BOMBA = D7;

//LED
const int LED_VERMELHO = D0;
const int LED_VERDE = D2;

Servo myservo;
int pos = 0;  //posição inicial do

BLYNK_WRITE(V0) 
{
  if (param.asInt() == 1) //Verifica se o botão foi acionado
  {
    //Alternancia dos LED's
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, HIGH);

    //O motor age girando a 90º, inclinando o copo
    for (pos = 0; pos <= 90; pos += 1) {
    myservo.write(pos);
    delay(15);
    }

    digitalWrite(BOMBA, HIGH);  //Liga a bomba
    delay(12000);  //Tempo de espera pra  encher o copo
    digitalWrite(BOMBA, LOW); //Desliga a bomba

    //O motor volta a posição inicial
    for (pos = 90; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(30);
    }

    //Alternancia dos LED's
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_VERMELHO, LOW);

    Blynk.virtualWrite(V0, 0);  //Volta o botão ao estado inicial
  }
}

  BLYNK_WRITE(V1) 
{
  if (param.asInt() == 1) //Verifica se o botão foi acionado
  {
    digitalWrite(BOMBA, HIGH);  //Liga a bomba

    //Alternancia dos LED's
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, HIGH);
    Blynk.virtualWrite(V1, 1);
    Blynk.virtualWrite(V2, 1);
  }
  
}

BLYNK_WRITE(V2) 
{
  if (param.asInt() == 0) //Verifica se o botão foi acionado
  {
    digitalWrite(BOMBA, LOW);  //Desliga a bomba

    //Alternancia dos LED's
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_VERMELHO, LOW);
    Blynk.virtualWrite(V2, 0);
    Blynk.virtualWrite(V1, 0);
  }
  
}

void setup ()
{
  Serial.begin(9600); //INICIALIZAÇÃO DA PORTA SERIAL

  pinMode(BOMBA, OUTPUT);     //INICIALIZAÇÃO DA BOMBA

  myservo.attach(PINO_MOTOR); //INICIALIZA A PORTA DIGITAL

  myservo.write(0); //INICIALIZA A POSIÇÃO INICIAL

  //INICIALIZAÇÃO DA BOMBA E DOS LEDS (DESLIGADOS)
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);

  digitalWrite(LED_VERDE, HIGH);

  Blynk.begin(auth, ssid, pass);
}

void loop ()
{
  Blynk.run();
}

/*
CONEXÕES QUE FORAM UTILIZADAS

Conexões Micro-Servo
Cabo Amarelo: porta D#
Cabo Vermelho: porta 3 Volts - 3v
Cabo Preto: Ground - G

Conexões Balança:
Lado Esquerdo
- E+ = vermelho
- E- = preto
- A- = branco
- A+ = verde

Lado Direito
- Ground (preto) = ground
- DT (verde) = D#
- SKT (marrom) = D#
- VCC (vermelho) = 3V

Conexões LED:
Cabo Branco: porta Ground - G
Cabo Preto: porta D#
*/
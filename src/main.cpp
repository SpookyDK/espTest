#include <Arduino.h>
#include <WiFi.h>
#include <AsyncUDP.h>
#include <iostream>
#include <string>

// Replace with your network credentials
const char *ssid = "kage";
const char *password = "123456789";
String speedx;
String speedy;
int speedRight = 0;
int speedLeft = 0;
AsyncUDP Udp;

int right_forward = 33;
int right_reverse = 32;
int left_forward = 26;
int left_reverse = 25;

void UpdateSpeed(String speedX, String speedY)
{
  int speedx = speedX.toFloat() - 2048;
  int speedy = (speedY.toFloat() - 2048);

  //Serial.println(speedX + "  " + speedY);
  
  float scale = (float)speedy / (float)2048;
  //Serial.println(speedy);
  Serial.println(scale + ' : ' + speedx);

  if (speedx < -140)
  {
    if (speedy > 0.1)
    {
      analogWrite(right_reverse, constrain(0, 0, 255));
      analogWrite(right_forward, constrain(-speedx / 8*(1-scale), 0, 255));
      analogWrite(left_reverse, constrain(0, 0, 255));
      analogWrite(left_forward, constrain(-speedx / 8, 0, 255));
      
    }
    else if (speedy < -0.1)
    {
      analogWrite(left_reverse, constrain(0, 0, 255));
      analogWrite(left_forward, constrain(-speedx / 8*(1+scale), 0, 255));
      analogWrite(right_reverse, constrain(0, 0, 255));
      analogWrite(right_forward, constrain(-speedx / 8, 0, 255));
      
    }
    else
    {
      analogWrite(left_reverse, constrain(0, 0, 255));
      analogWrite(right_reverse, constrain(0, 0, 255));
      analogWrite(right_forward, constrain(-speedx / 8, 0, 255));
      analogWrite(left_forward, constrain(-speedx / 8, 0, 255));
      
    }
  }
  else if (speedx > 140)
  {
    if (speedy > 0.1)
    {
      analogWrite(right_forward, constrain(0, 0, 255));
      analogWrite(right_reverse, constrain(speedx / 8*(1-scale), 0, 255));
      analogWrite(left_forward, constrain(0, 0, 255));
      analogWrite(left_reverse, constrain(speedx / 8, 0, 255));
      
    }
    else if (speedy < -0.1)
    {
      analogWrite(left_forward, constrain(0, 0, 255));
      analogWrite(left_reverse, constrain(speedx / 8*(1+scale), 0, 255));
      analogWrite(right_forward, constrain(0, 0, 255));
      analogWrite(right_reverse, constrain(speedx / 8, 0, 255));
      
    }
    else
    {
      analogWrite(left_forward, constrain(0, 0, 255));
      analogWrite(right_forward, constrain(0, 0, 255));
      analogWrite(right_reverse, constrain(speedx / 8, 0, 255));
      analogWrite(left_reverse, constrain(speedx / 8, 0, 255));
      
    }
  }
  else
  {
    analogWrite(right_reverse, constrain(0, 0, 255));
    analogWrite(right_forward, constrain(0, 0, 255));
    analogWrite(left_reverse, constrain(0, 0, 255));
    analogWrite(left_forward, constrain(0, 0, 255));
  }
}

void setup()
{
  pinMode(33, OUTPUT);
  pinMode(21, OUTPUT);

  Serial.begin(115200);
  delay(1000);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.softAP(ssid, password);

  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println(WiFi.dnsIP());

  Udp.listen(69);
}

void loop()
{
  if (Udp.listen(69))
  {
    Udp.onPacket([](AsyncUDPPacket packet)
                 {
                   String kage2 = String((char *)packet.data());
                   //Serial.println(kage2);

                   int index = kage2.indexOf(':');
                   speedy = kage2.substring(index + 1,index+5);
                   speedx = kage2.substring(0, index);
                   //Serial.println(speedx + "  " + speedy);
                   UpdateSpeed(speedx, speedy);
                 });
  }
  else
  {
    Serial.println("test");
  }
  delay(30);
  //Serial.println("clear");
}

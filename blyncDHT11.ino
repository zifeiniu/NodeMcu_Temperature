
#define BLYNK_PRINT Serial // 开启串口监视
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"

char auth[] = "xxxxxxxxxxxxxxxxxxxxxx";//授权码
char ssid[] = "wifiname";//wifi名称
char pass[] = "wifipassword";//wifi密码
#define DHTPIN 2//传感器连接管脚
#define DHTTYPE DHT11     // DHT 11 
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
SSD1306Wire display(0x3c, SDA, SCL);   

int ci = 0;
//该函数将会每秒钟发送数据给V5,V6,
//在app端，组件的读取频率应该设为PUSH。
void sendSensor()
{
  display.clear();
  ci++;
  float h = dht.readHumidity();
  float t = dht.readTemperature(); //摄氏度
  //float t = dht.readTemperature(true); //华氏度
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V5, h);///给V5
  Blynk.virtualWrite(V6, t);//将湿度发送给V6

   

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, (String)ci);
  display.drawString(0, 16, "Temp:"+(String)t+"°C");
  display.drawString(0, 32, "Humi:"+(String)h+"%");

  display.display();
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);//官方服务器
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8080);//自建服务器域名模式
  //Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 1, 158), 8080);//自建服务器ip模式
  dht.begin();
  timer.setInterval(1000L, sendSensor);
   display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  
}

void loop()
{
  Blynk.run();
  timer.run();
}

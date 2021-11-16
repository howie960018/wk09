#include "WiFi.h"
#include "WebServer.h"


//輸入你的SSID/password
const char* ssid = "OHI數位牙醫學院";
const char* password = "happy108";

WebServer server(80);
void setup() {

  ledcSetup(0,5000,8);
  ledcAttachPin(1,0);
  Serial.begin(115200);
  pinMode(1,OUTPUT);
  pinMode(3,OUTPUT);
  
  Serial.println("Try Connecting to ");
  Serial.println(ssid);

  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  

  createWebServer();
  
  //server啟動
  server.begin();
  Serial.println("HTTP server started");
  
}

void loop(){
  server.handleClient();  //handleClient要寫在loop
}

String indexContent = "\
  <!DOCTYPE html>\
   <html>\
    <head>\
      <style>\
        body{ display: flex; flex-direction: column;}\
        a { background: #CEF2F4; width: 200px; height: 50px; margin: 20px; font-size: 30pt; text-decoration: none;}\
      </style>\
    </head>\
    <body>\
      <a href='/led/on'>LED ON</a>\
      <a href='/led/off'>LED OFF</a>\
      <form method='get' action='/led/setLightBrightness'>\
        <label>Led Brightness</label>\
        <input type='number' name='lightBrightness'><input type='submit'>\
      </form>\
    </body>\
  </html>";

String LEDOpenPage = "\
  <!DOCTYPE html>\
   <html>\
    <head>\
      <style>\
        body{ display: flex; flex-direction: column;}\
         a { background: #CEF2F4; width: 200px; height: 50px; margin: 20px; font-size: 30pt; text-decoration: none;}\
      </style>\
    </head>\
    <body>\
      <h1>LED OPEN</h1>\
      <a href='/'>Back</a>\
    </body>\
  </html>";

String LEDCLOSEPage = "\
  <!DOCTYPE html>\
   <html>\
    <head>\
      <style>\
        body{ display: flex; flex-direction: column;}\
        a { background: #CEF2F4; width: 200px; height: 50px; margin: 20px; font-size: 30pt; text-decoration: none;}\
      </style>\
    </head>\
    <body>\
      <h1>LED CLOSE</h1>\
      <a href='/'>Back</a>\
    </body>\
  </html>";

 void createWebServer()
{

  server.on("/", []() {
      server.send(200, "text/html", indexContent);
  });


   server.on("/led/on", []() {
    Serial.println("LED On");

    //add your code here
    digitalWrite(3,HIGH);
    
    
    String response = "{\"LED On Success\"}";
     server.send(200, "text/html", LEDOpenPage);
    
  });
  server.on("/led/off", []() {
    Serial.println("LED Off");
    
    //add your code here
     digitalWrite(3,LOW);
    
    String response = "{\"LED Off Success\"}";
     server.send(200, "text/html", LEDCLOSEPage);
  });

  server.on("/led/setLightBrightness", []() {
    //以server.arg&引數名稱，取得form傳來的資料
    int lightBrightness = server.arg("lightBrightness").toInt();
    //判斷條件    
    if(lightBrightness>=0 && lightBrightness<=1024){
      Serial.print("Set Brightness: ");
      Serial.println(lightBrightness);

      //add your code here
       digitalWrite(1,HIGH);
       ledcWrite(1, lightBrightness); 
     
       
      
      String response = "Set Brightness: " + (String)lightBrightness;
      server.send(200, "application/json", response);
    } 
    else {
      String response = "{\"Error\":\"over range 0-1024\"}";
      server.send(404, "application/json", response);
    }
  });
}

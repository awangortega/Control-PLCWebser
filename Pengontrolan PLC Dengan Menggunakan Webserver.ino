#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "TP-Link_CD7E";
const char* pasword = "80157281";

const int relay1 = 14;
const int relay2 = 12;
const int relay3 = 13;

AsyncWebServer server(80);
  
LiquidCrystal_I2C lcd(0x27, 16, 2);

// HTML web page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Pengontrol PLC</title>
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
</head>
<style>
    body {
    text-align: center;
    background-color: #201e1e;
    position: relative;    
    }
    p {
    margin: 20px 0 20px 0;
    padding: 20px 0 20px 0;
    }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
    
    .button {
        padding: 10px 20px;
        font-size: 24px;
        text-align: center;
        font-weight: bold;
        outline: none;
        color: #fff;
        border: none;
        border-radius: 5px;
        cursor: pointer;
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        -webkit-tap-highlight-color: rgba(0,0,0,0);
      }  
      .button:hover {background-color: #ff0522 }
      .button:active {
        background-color: #ffffff;
        transform: translateY(2px);
      }
</style>
<body>
    <h3 style="color: #ffffff;">
        CONTROLLING & MONITORING WITH ESP32 FOR CONTROL FORWARD REVERSE IN MOTOR 
    </h3>
    <p></p>
    <h3 style="color: #ffffff;position: relative; background-color: #000000;">Controlling for PLC</h3>
    <button class="button" style="background-color: red; width: 30%;" onmousedown="toggleCheckbox('R1on');" ontouchstart="toggleCheckbox('R1on');" onmouseup="toggleCheckbox('R1off');" ontouchend="toggleCheckbox('OFF');">STOP</button>
    <div style="letter-spacing: 4px; margin-top: 10px;">
      <button class="button" style="background-color: green; width: 30%;" onmousedown="toggleCheckbox('R2on');" ontouchstart="toggleCheckbox('R2on');" onmouseup="toggleCheckbox('R2off');" ontouchend="toggleCheckbox('OFF');">FORWARD</button>
      <button class="button" style="background-color: green; width: 30%;" onmousedown="toggleCheckbox('R3on');" ontouchstart="toggleCheckbox('R3on');" onmouseup="toggleCheckbox('R3off');" ontouchend="toggleCheckbox('OFF');">REVERSE</button>       
    </div>
    <script>
      function toggleCheckbox(x) {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/" + x, true);
        xhr.send();
      }
     </script>
</body>
</html>)rawliteral";

void setup() {
  Serial.begin(115200);
  lcd.begin();
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, pasword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
  lcd.setCursor(0,0);
  lcd.print("Connecting to");
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
  
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  
  // Send web page to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Receive an HTTP GET request
  server.on("/R1on", HTTP_GET, [] (AsyncWebServerRequest *request) {
    digitalWrite(relay1, HIGH);
    request->send(200, "text/plain", "OK");
  });

  server.on("/R1off", HTTP_GET, [] (AsyncWebServerRequest *request) {
    digitalWrite(relay1, LOW);
    request->send(200, "text/plain", "OK");
  });

    server.on("/R2on", HTTP_GET, [] (AsyncWebServerRequest *request) {
    digitalWrite(relay2, HIGH);
    request->send(200, "text/plain", "OK");
  });

  server.on("/R2off", HTTP_GET, [] (AsyncWebServerRequest *request) {
    digitalWrite(relay2, LOW);
    request->send(200, "text/plain", "OK");
  });

   server.on("/R3on", HTTP_GET, [] (AsyncWebServerRequest *request) {
    digitalWrite(relay3, HIGH);
    request->send(200, "text/plain", "OK");
  });

  server.on("/R3off", HTTP_GET, [] (AsyncWebServerRequest *request) {
    digitalWrite(relay3, LOW);  
    request->send(200, "text/plain", "OK");
  }); 
 
  server.begin();
}

void loop() {

}

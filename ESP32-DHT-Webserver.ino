#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <DHT.h>

const char *ssid = "Yi";
const char *password = "esp32";

//
const int potPin = 35;
float ph;
float Value = 0;
//


WebServer server(80);
DHT dht(4, DHT22);

void handleRoot() {
  char msg[1500];

  int sensorValue = analogRead(33);
  float voltage = sensorValue * (5.0 / 1024.0);

  snprintf(msg, 1500,
           "<html>\
  <head>\
    <meta http-equiv='refresh' content='4'/>\
    <meta name='viewport' content='width=device-width, initial-scale=1'>\
    <title>WATER QUALITY MONITORING SYSTEM WITH GUI</title>\
    <style>\
    html { font-family: Arial; display: inline-block; margin: 0px auto; text-align: center;}\
    h2 { font-size: 3.0rem; }\
    p { font-size: 3.0rem; }\
    .units { font-size: 1.2rem; }\
    .dht-labels{ font-size: 1.5rem; vertical-align:middle; padding-bottom: 15px;}\
    </style>\
  </head>\
  <body>\
      <h2>WATER QUALITY MONITORING SYSTEM </h2>\
      <p>\
         <i class='fas fa-thermometer-half' style='color:#ca3517;'></i>\
        <span class='dht-labels'>Temperature</span>\
        <span>%.2f</span>\
        <sup class='units'>&deg;C</sup>\
      </p>\
      <p>\
        <i class='fas fa-tint' style='color:#00add6;'></i>\
        <span class='dht-labels'>Humidity</span>\
        <span>%.2f</span>\
        <sup class='units'>&percnt;</sup>\
      </p>\
      <p>\
        <i class='fas fa-tint' style='color:#00add6;'></i>\
        <span class='dht-labels'>Turbidity</span>\
        <span>%.2f</span>\
        <sup class='units'>V;</sup>\
      </p>\
      <p>\
        <i class='fas fa-tint' style='color:#00add6;'></i>\
        <span class='dht-labels'>PH</span>\
        <span>%.2f</span>\
        <sup class='units'></sup>\
      </p>\
  </body>\
</html>",
           readDHTTemperature(), readDHTHumidity(), voltage, readPH());
  server.send(200, "text/html", msg);
}

void setup(void) {

  Serial.begin(115200);
  delay(1000);
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.softAP(ssid);
  Serial.println("");
  Serial.println("Wifi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.softAPIP());
  Serial.println("");

  // Start The Server
  server.begin();
  Serial.println("HTTP server started.");

  dht.begin();
  server.on("/", handleRoot);

  //
  pinMode(potPin, INPUT);
  delay(1000);
  //
}

void loop(void) {

  server.handleClient();
  int sensorValue = analogRead(33);
  float voltage = sensorValue * (5.0 / 1024.0);

  Serial.println("Sensor Output (V):");
  Serial.println(voltage);
  Serial.println();
  delay(1000);
  delay(2);  //allow the cpu to switch to other tasks
}


float readDHTTemperature() {
  // Sensor readings may also be up to 2 seconds
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  } else {
    t = t ;
    Serial.println(t);
    return t;
  }
}

float readDHTHumidity() {
  // Sensor readings may also be up to 2 seconds
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  } else {
    Serial.println(h);
    return h;
  }
}


float readPH() {
  Value = analogRead(potPin);
  Serial.print(Value);
  Serial.print(" | ");
  float voltage_ph = Value * (3.3 / 4095.0);
  ph = (3.3 * voltage_ph);
  Serial.println(ph);
  delay(500);
  return ph;
}
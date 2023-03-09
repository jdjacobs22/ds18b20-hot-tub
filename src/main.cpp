#include <Arduino.h>
#include <ESP8266WiFi.h>
//   #include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <LittleFS.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is connected to GPIO 4
const int ONE_WIRE_BUS = 4;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

// Create AsyncWebServer object
AsyncWebServer server(80);
// AsyncWebSocket ws("/ws");
// AsyncWebSocketClient *wsClientGlobal;

const char *location = "hottub";
const char *ssid = "windsong";
const char *wifi_password = "fubsey00";

// // Variables to store temperature values
float temperatureF = 0;
float temperatureC = 0;

// // Timer variables
// unsigned long lastTime = 0;
// unsigned long timerDelay = 30000;

// // Create AsyncWebServer object on port 80
// AsyncWebServer server(80);

// String readDSTemperatureC() {
//   // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
//   sensors.requestTemperatures();
//   float tempC = sensors.getTempCByIndex(0);

//   if(tempC == -127.00) {
//     Serial.println("Failed to read from DS18B20 sensor");
//     return "--";
//   } else {
//     Serial.print("Temperature Celsius: ");
//     Serial.println(tempC);
//   }
//   return String(tempC);
// }

// String readDSTemperatureF() {
//   // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
//   sensors.requestTemperatures();
//   float tempF = sensors.getTempFByIndex(0);

//   if(int(tempF) == -196){
//     Serial.println("Failed to read from DS18B20 sensor");
//     return "--";
//   } else {
//     Serial.print("Temperature Fahrenheit: ");
//     Serial.println(tempF);
//   }
//   return String(tempF);
// }

// // Replaces placeholder with DS18B20 values
String processor(const String &var)
{
  // Serial.println(var);
  if (var == "TEMPERATUREC")
  {
    return String(temperatureC);
  }
  else if (var == "TEMPERATUREF")
  {
    return String(temperatureF);
  }
  return String();
}

void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

void setup()
{

  // Serial port for debugging purposes
  Serial.begin(115200);

  // Setup wifi
  setup_wifi();
  Serial.print("\n\n");
  Serial.print("Location: ");
  Serial.println(location);

  // Start up the DS18B20 library
  sensors.begin();

  // Initialze LittleFS
  if (!LittleFS.begin())
  {
    Serial.println("An error has occurred while mounting LittleFS");
    return;
  }
  else
  {
    Serial.println("LittleFS successfully mounted");
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
          { request->send(LittleFS, "/index.html"); });
  server.on("/temperaturec", HTTP_GET, [](AsyncWebServerRequest *request)
          { request->send_P(200, "text/plain", String(temperatureC).c_str()); });
  server.on("/temperaturef", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", String(temperatureF).c_str()); });

  server.begin();
  Serial.println("HTTP server started");

  // temperatureC = readDSTemperatureC();
  // temperatureF = readDSTemperatureF();
}

void loop()
{
    // if ((millis() - lastTime) > timerDelay) {
    //   temperatureC = readDSTemperatureC();
    //   temperatureF = readDSTemperatureF();
    //   lastTime = millis();
    // }
    sensors.requestTemperatures();
    temperatureC = sensors.getTempCByIndex(0);
    temperatureF = sensors.getTempFByIndex(0);
    Serial.print(temperatureC);
    Serial.println("ºC");
    Serial.print(temperatureF);
    Serial.println("ºF");
    delay(5000);
}
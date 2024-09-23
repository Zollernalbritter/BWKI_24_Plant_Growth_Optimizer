#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <SD.h>

#include <HTTPClient.h>

float temperature = 14.2;
float pressure = 200.0;
float humidity = 40.0;
float gasResistance = 20.0;
float realAltitude = 1020.0;
float co2 = 40.0;
float soilMoisture = 23.4;
float brightness = 20.0;
int requestId = 240;


#define SEALEVELPRESSURE_HPA (1013.25)
#define CO2_SENSOR_PIN 6
#define HUMIDITY_SENSOR_PIN 7
#define LIGHT_SENSOR_PIN 15
#define SESSION_TOGGLE 5

#define RANGE 5000

File file;

Adafruit_BME680 bme; // I2C

AsyncWebServer server(80);

struct settings {
  char ssid[30];
  char password[30];
} user_wifi = {};

bool connected = false; // Flag to indicate WiFi connection status

void setup() {
  Serial.begin(115200);

  Wire.begin(5,4);

  if (!bme.begin(0x77,&Wire)) {
    Serial.println(F("Could not find a valid BME680 sensor, check wiring!"));
    while (1);
  }

  // Error Checking
  if(!SD.begin()){
      Serial.println("Card Mount Failed");
      return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
      Serial.println("No SD card attached");
      return;
  }

  delay(5000);

  EEPROM.begin(sizeof(struct settings));
  EEPROM.get(0, user_wifi);

  WiFi.mode(WIFI_STA);
  WiFi.begin(user_wifi.ssid, user_wifi.password);

  byte tries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    if (tries++ > 30) {
      WiFi.mode(WIFI_AP);
      WiFi.softAP("Setup Portal", "plantBox24");
      break;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    connected = true;
    connectedToWiFi();
  } else {
    failedToConnectToWiFi();
  }
  Serial.println("");


  server.on("/", HTTP_GET, handlePortal);
  server.on("/", HTTP_POST, handleWiFi);
  server.begin();

    // Set up oversampling and filter initialization
  
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

  Serial.println("initialization done.");
  
  document_setup();

  printData();
}

void loop() {
  if (connected) {
    connectedToWiFiLoop();
  } else {
    failedToConnectToWiFiLoop();
  }
}

void connectedToWiFiLoop() {
  while (digitalRead(SESSION_TOGGLE)) {
    writeData();
    sendHTTPRequest();
    delay(60000);
  } 
  delay(60000);
}


void writeData() {
  file = SD.open("/data.txt", FILE_WRITE);

  if (file) {
    Serial.print("Writing to data.txt...");
    } else {
    Serial.println("error opening data.txt");
  }

  if (!bme.endReading()) {
    Serial.println(F("Failed to complete reading :("));
    return;
  }
  delay(1000);

  temperature = bme.temperature;
  pressure = bme.pressure;
  humidity = bme.humidity;
  gasResistance = bme.gas_resistance;
  realAltitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  co2 = analogRead(CO2_SENSOR_PIN);
  soilMoisture = analogRead(HUMIDITY_SENSOR_PIN);
  brightness = analogRead(LIGHT_SENSOR_PIN);

  // *C
  file.print(bme.temperature);
  file.print(";");

  // hPa
  file.print(bme.pressure / 100.0);
  file.print(";");

  // %
  file.print(bme.humidity);
  file.print(";");

  // KOhms
  file.print(bme.gas_resistance / 1000.0);
  file.print(";");

  // m
  file.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  file.print(";");

  // CO2
  file.print(analogRead(CO2_SENSOR_PIN));
  file.print(";");
  
  //Feutchtigkeit
  file.print(analogRead(HUMIDITY_SENSOR_PIN));
  file.print(";");

  //Helligkeit
  file.println(analogRead(LIGHT_SENSOR_PIN));
  file.print(";");

  Serial.println("finished Writing one Dataset!");
  file.close();
  Serial.println("Closed File!");
}

void document_setup() {
  
  file = SD.open("/data.txt", FILE_WRITE);

  if (file) {
    Serial.print("Writing to data.txt...");
    } else {
    Serial.println("error opening data.txt");
  }
  
  Serial.println("finished Writing Start Line!");
  file.println("\n\n");
  file.println("Tempeture;pressure;humidity;gas_resistance;realAltitude;CO2;Feuchtigkeit;Helligkeit");
  file.close();
}

void printData(){
  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  Serial.print("Temperature = ");
  Serial.print(bme.temperature);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bme.pressure / 100.0);
  Serial.println(" hPa");

  Serial.print("Humidity = ");
  Serial.print(bme.humidity);
  Serial.println(" %");

  Serial.print("Gas = ");
  Serial.print(bme.gas_resistance / 1000.0);
  Serial.println(" KOhms");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.println();
  delay(2000);

}

void sendHTTPRequest() {
  if (WiFi.status() == WL_CONNECTED) { // Check WiFi connection
    HTTPClient http;

    // Variables for request parameters
    String authKey = "e499817768bf9a845";

    // Construct the request URL with variables
    String url = "https://plantapi.hou-mea.de/auth?";
    url += "authkey=" + authKey;
    url += "&temperature=" + String(temperature);
    url += "&pressure=" + String(pressure);
    url += "&humidity=" + String(humidity);
    url += "&gasresistance=" + String(gasResistance);
    url += "&realAltitude=" + String(realAltitude);
    url += "&co2=" + String(co2);
    url += "&soil_moisture=" + String(soilMoisture);
    url += "&brightness=" + String(brightness);
    url += "&request_id=" + String(requestId);

    Serial.print("Sending request to: ");
    Serial.println(url);

    http.begin(url); // Specify request destination

    int httpCode = http.GET(); // Send the request

    if (httpCode > 0) { // Check the response
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end(); // Close connection

    // Increment request ID

    delay(5000); // Wait for 5 seconds before sending the next request
  } else {
    Serial.println("WiFi Disconnected. Reconnecting...");
    WiFi.begin(user_wifi.ssid, user_wifi.password);
    //WiFi.begin(ssid, password); // Reconnect to WiFi
  }
}

void failedToConnectToWiFiLoop() {
  // Your code when failed to connect to WiFi
}

void connectedToWiFi() {
  Serial.println("Connected to WiFi!");
}

void failedToConnectToWiFi() {
  Serial.println("Failed to connect to WiFi!");
}

void handlePortal(AsyncWebServerRequest *request) {
  request->send_P(200, "text/html", "<!doctype html><html lang='en'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'><title>Wifi Setup</title> <style>*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{cursor: pointer;border:1px solid transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:400px;padding:15px;margin:auto;}h1{text-align: center}</style> </head> <body><main class='form-signin'> <form action='/' method='post'> <h1 class=''>Wifi Setup</h1><br/><div class='form-floating'><label>SSID</label><input type='text' class='form-control' name='ssid'> </div><div class='form-floating'><br/><label>Password</label><input type='password' class='form-control' name='password'></div><br/><br/><button type='submit'>Save</button><p style='text-align: right'><a href='https://www.mrdiy.ca' style='color: #32C5FF'>mrdiy.ca</a></p></form></main> </body></html>");
}
void handleWiFi(AsyncWebServerRequest *request) {
  if (request->method() == HTTP_POST) {
    String ssid = request->arg("ssid");
    String password = request->arg("password");
    strncpy(user_wifi.ssid, ssid.c_str(), sizeof(user_wifi.ssid));
    strncpy(user_wifi.password, password.c_str(), sizeof(user_wifi.password));
    user_wifi.ssid[ssid.length()] = user_wifi.password[password.length()] = '\0';
    EEPROM.put(0, user_wifi);
    EEPROM.commit();
    request->send_P(200, "text/html", "<!doctype html><html lang='en'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'><title>Wifi Setup</title><style>*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:400px;padding:15px;margin:auto;}h1,p{text-align: center}</style> </head> <body><main class='form-signin'> <h1>Wifi Setup</h1> <br/> <p>Your settings have been saved successfully!<br />Please restart the device.</p></main></body></html>");
  }
}

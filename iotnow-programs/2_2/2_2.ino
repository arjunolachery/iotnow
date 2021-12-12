#include <iotnow.h>
// temperature and humidity sensor logger with iotnow

// libraries for specific program
//ESP8266WiFiMulti WiFiMulti;
#include "HTTPSRedirect.h"
#include "DebugMacros.h"
#include <DHT.h> // library for Temperature and Humidity Sensor

// variables for the specific program
#define sensorPin D1 //assigning pin for temperature and humidity sensor                                                           
#define sensorType DHT11 // assigning type of sensor                                                      
DHT dht(sensorPin, sensorType);

float humidityValue, temperatureValue; // raw values from sensors
String humidityProcessed = "",temperatureProcessed = ""; // values from sensors with units
const char* scriptLinkMain = "script.google.com"; // google script link - sheets
const char* IdScriptLink = "AKfycby8xsHR-th2K-zKiWFTnLMrNkx1xOsPd6DRdlysA10I4INAaPxEwmxXKEyEzaD92nlS"; // google script id - sheets
int portNum = 443; // port number for connecting to gsheets

String scriptSubLink = String("/macros/s/") + IdScriptLink + "/exec?value=Temperature"; // sublink containing properties of sheet
String scriptSubLink2 = String("/macros/s/") + IdScriptLink + "/exec?cal";
// data of sheet stored
String info_base =  "{\"command\": \"appendRow\", \ 
                    \"sheet_name\": \"SensorSheet\", \
                       \"values\": ";
// creating a pointer to a client object
HTTPSRedirect* httpDevice = nullptr;

int programId = 2; // current programId
int versionId = 2; // current versionId

void setup() {
  Serial.begin(115200);
//  WiFi.mode(WIFI_STA); // mentioning current device as wifi station
  iotnow(programId,versionId);

  //setup for the specific program
  dht.begin();     //initialise temperature and humidity sensor

  httpDevice = new HTTPSRedirect(portNum); // the client object properties are set here
  httpDevice->setInsecure();
  httpDevice->setPrintResponseBody(true);
  httpDevice->setContentTypeHeader("application/json");
  Serial.print("Connection initiated...");
  bool connectFlag = false; 
  for (int i = 0; i < 10; i++) { // the connection is checked around 10 times to ensure that a connection can be made before proceeding
    int response = httpDevice->connect(scriptLinkMain, portNum);
    if (response == 1) {
      connectFlag = true;
      break;
    }
    else
      Serial.println("Wait...");
  }

  if (!connectFlag) {
    Serial.print("Connection couldn't be established... ");
    return;
  }

  httpDevice->GET(scriptSubLink, scriptLinkMain); // set as a get request
  httpDevice->GET(scriptSubLink2, scriptLinkMain);
  
  // delete HTTPSRedirect object
  delete httpDevice;
  httpDevice = nullptr;
}

void loop() {
  // loop for specific program
  temperatureValue = dht.readTemperature();  // raw temperature and humidity values are collected from the sensor 
  humidityValue = dht.readHumidity();                                                
  if (isnan(humidityValue) || isnan(temperatureValue)) {           
    Serial.println(F("Failed to read from Sensor")); // if sensor values cannot be read
    return;
  }   
  temperatureProcessed = String(temperatureValue) + String("°C"); // temperature and humidity values appended with units
  humidityProcessed = String(humidityValue) + String("%");  
  Serial.println(temperatureProcessed);  
  Serial.println(humidityProcessed);   
  static int errorTrack = 0; // to keep track of the counts of errors
  static int connectTrack = 0; // to keep track of the counts of connects 
  const unsigned int limitConnect = 20; // maximum limit for connects set here
  static bool connectFlag2 = false; // flag that stores if connect has happened

  String info = info_base + "\"" + temperatureProcessed + "," + humidityProcessed + "\"}"; // data to be sent to google script

  if (!connectFlag2) { // connection is made here
    httpDevice = new HTTPSRedirect(portNum);
    httpDevice->setInsecure();
    connectFlag2 = true;
    httpDevice->setPrintResponseBody(true);
    httpDevice->setContentTypeHeader("application/json");
  }

  if (httpDevice != nullptr) { // values are sent here to the google script
    if (!httpDevice->connected()) {
      httpDevice->connect(scriptLinkMain, portNum);
      httpDevice->POST(scriptSubLink2, scriptLinkMain, info, false);
      Serial.println("Delivered "); 
    }
  }
  else { 
    Serial.println("Error creating httpDevice object!");
    errorTrack = 5;
  }

  if (connectTrack > limitConnect) { // if connection doesn't happen within limit then cancel 
    connectTrack = 0;
    connectFlag2 = false;
    delete httpDevice;
    return;
  }

  if (httpDevice->POST(scriptSubLink2, scriptLinkMain, info)) { // values are sent here to the google script
    ;
  }
  else { // error connecting handled here
    ++errorTrack;
    Serial.println("Error while connecting.");
    DPRINTLN(errorTrack);
  }

  if (errorTrack > 5) { // if counts of errors are more than 5 then stop sending to google sheet
    Serial.println("Stopping...");
    delete httpDevice;
    httpDevice = nullptr;
    Serial.flush();
    ESP.deepSleep(0);
  }
  
  delay(5000); // keep a delay to ensure google sheet is updated within atleast 5 seconds
  

}

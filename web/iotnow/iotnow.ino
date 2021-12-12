// important libraries
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <Board_Identify.h>
#include <ESP8266httpUpdate.h>
#include <EEPROM.h> //header file for accessing memory 

//global variables
String wifiSsid = "iotnow"; // regular wifiSsid
String wifiPassword = "iotnowpass"; // regular wifiPassword
int MCUId=0; // identification of board
int programId = 1; // current programId
int versionId = 1; // current versionId

void identifyBoard(){ //to identify board
#if defined(ARDUINO_ESP8266_NODEMCU)
      MCUId = 1;
#elif defined(ARDUINO_AVR_UNO)
      MCUId = 2;
#elif defined(ARDUINO_AVR_NANO)
      MCUId = 3;
#endif
}

void clearMemoryAll(int startAddress){
  int limit=512; // usual limit
  Serial.println("\nClearing Memory at address "+startAddress);
  for (int i = startAddress; i < limit+startAddress; i++) {
      EEPROM.write(i, 0);
    }
    EEPROM.commit();
}

void clearMemoryAt (int startAddress){ //clearMemoryAt sets values to 0 at specified memory locations
  int limit=100; // usual limit
  Serial.println("\nClearing Memory at address "+startAddress);
  for (int i = startAddress; i < limit+startAddress; i++) {
      EEPROM.write(i, 0);
    }
    EEPROM.commit();
}

void writeMemory(int startAddress, String writeData){ //writes to Memory with parameters start address and data
  int i;
  clearMemoryAt(startAddress);  
  Serial.println("\nWriting data '"+writeData+"' with length: "+writeData.length());
  for (i = 0; i < writeData.length(); i++)
  {
    EEPROM.write(startAddress + i, writeData[i]);
  }
  EEPROM.write(startAddress + i, '|');
  if (EEPROM.commit()) {
    Serial.println("Data successfully written");
  } else {
    Serial.println("ERROR: Data write failed");
  }
}

String readMemory(int startAddress){ //reads characters from memory at specified address and returns the string that was read
  int limit=100; //usual limit
  Serial.println("\nReading data from location "+String(startAddress));
  String readData="";
  char readChar;
  int k=startAddress;
  while(((readChar=char(EEPROM.read(k))) != '|') && k < (startAddress+limit)){ //read characters until '|' is encountered or upto address limit - whichever comes first
    readData += readChar;
    k++;
  }
  Serial.println("Data read: "+readData);
  return readData; // returns the data that was read as a string - can be stored as a variable or printed
}

void wifiSetup(){ // connects to wifi - will wait inifinitely if wifi network not found
  String wifiSsidMem = readMemory(0); // reads the wifiSsid from memory
  String wifiPasswordMem = readMemory(100); // reads the wifiPassword from memory
  if(wifiSsidMem == "")
  WiFi.begin(wifiSsid.c_str(), wifiPassword.c_str()); // use regular wifi ssid and password iotnow and iotnowpass
  else
  WiFi.begin(wifiSsidMem.c_str(), wifiPasswordMem.c_str()); // use user defined wifi ssid and password
  bool lightMode = 0; // set the built-in light status
  while (WiFi.status() != WL_CONNECTED) { // connect to wifi 
     delay(600);
     digitalWrite(LED_BUILTIN, lightMode); // blink the built in led
     lightMode=!lightMode;
  }
  digitalWrite(LED_BUILTIN, 0);
}

void updateToServer(){ // device gets updated or registered onto "devices" database table of server
  HTTPClient http;   // create http object 
  String dataPost = "macAddress="+WiFi.macAddress()+"&MCUId="+MCUId; // data to be sent over http link through post
  http.begin("http://iotnow.co.in/send.php"); // set http object to point to the link and start http connection
  http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // http header added as type content
  int httpVal = http.POST(dataPost);  // do post request over http with the data and link mentioned as content-type. store the http result code
  http.end();  // Terminate the http connection
}

String programVersionCheck(){
  HTTPClient http;   // create http object 
  String dataPost = "macAddress="+WiFi.macAddress()+"&versionId="+versionId+"&programId="+programId+"&MCUId="+MCUId; // data to be sent over http link through post
  http.begin("http://iotnow.co.in/programVersionCheck.php"); // set http object to point to the link and start http connection
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");  // http header added as type content
  int httpVal = http.POST(dataPost);   // do post request over http with the data and link mentioned as content-type. store the http result code
  String httpResult = http.getString();    // get back the result of the post request 
  http.end();  // Terminate the http connection
  return httpResult; // return the string of http result
}

String wifiSsidCheck(){ //writes the new wifiSsid to memory
  HTTPClient http;    // create http object 
  String dataPost = "macAddress="+WiFi.macAddress(); // data to be sent over http link through post
  http.begin("http://iotnow.co.in/wifiSsidCheck.php"); // set http object to point to the link and start http connection
  http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // http header added as type content
  int httpVal = http.POST(dataPost);   // do post request over http with the data and link mentioned as content-type. store the http result code
  String httpResult = http.getString();    // get back the result of the post request 
  writeMemory(0,httpResult); // write the latest wifiSsid to memory
  http.end();  // Terminate the http connection
  return httpResult; // return the string of http result
}

String wifiPasswordCheck(){ //writes the new wifiPassword to memory
  HTTPClient http; // create http object 
  String dataPost = "macAddress="+WiFi.macAddress(); // data to be sent over http link through post
  http.begin("http://iotnow.co.in/wifiPasswordCheck.php");  // set http object to point to the link and start http connection
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");  // http header added as type content
  int httpVal = http.POST(dataPost);   // do post request over http with the data and link mentioned as content-type. store the http result code
  String httpResult = http.getString();    // get back the result of the post request 
  writeMemory(100,httpResult);  // write the latest wifiPassword to memory
  http.end();  // Terminate the http connection
  return httpResult; // return the string of http result
} 

void performOTAUpdate(){
  String updateFileName = programVersionCheck(); //to perform ota update
  if(updateFileName != "0"){
  Serial.println("yes perform");
  String serverLink="http://www.iotnow.co.in/files"; 
  String latestFwLink = serverLink+"/"+updateFileName+".bin";
  t_httpUpdate_return updateResponse = ESPhttpUpdate.update( latestFwLink ); // update performed here
  }
}

void iotnow(){
  EEPROM.begin(512); // to set up flash memory 
  pinMode(LED_BUILTIN, OUTPUT);
  identifyBoard();
  wifiSetup();
  wifiSsidCheck();
  wifiPasswordCheck();
  updateToServer();
  performOTAUpdate();
}

void setup(void)
{
  iotnow();
}
void loop() {
  // Nothing
}

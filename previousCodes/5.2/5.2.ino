//libraries for wireless communication
#include <ESP8266WiFiMulti.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266HTTPClient.h>
ESP8266WiFiMulti WiFiMulti;
#include <ESP8266WiFi.h> // ESP8266 (NodeMcu) device library
#include "HTTPSRedirect.h"
#include "DebugMacros.h"
#include <DHT.h> // library for Temperature and Humidity Sensor
#include <EEPROM.h> //header file for accessing memory

//essential variables
int fwVersion = 1000; // set the version of this file.
int programOption=1; // type of program - 1 - barebones (entry-level) program
String serverLink="http://www.iotnow.co.in/files"; //set the address of the server here which store the binary and text files.
bool checkedUpdate=0; // variable that keeps track of whether updates have been checked
String latestVersion=""; 
int checkUpdateCount=0;

// feedback responses to the User Interface - Bluetooth Terminal
String messageConfig="Welcome to iotnow! Let's configure your device! \n1. Set a name \n2. Set location \n3. Set WiFi SSID \n4. Set WiFi Password \n5. Exit";
String messageRead="What would you like to know about this device? \n1. Name \n2. Location \n3. WiFi SSID \n4. WiFi Password \n5. MAC Address\n6. Auto Update Status \n7. Desired Firmware \n8. Desired Program Choice \n9. Current Firmware Version \n10. Exit";
String messageHelp="Welcome to iotnow! Here are the list of commands - help,config,read,reformat,reset,autoupdate,setfw,setprogram,updatevalues,update,switch";
String messageInvalid="Welcome to iotnow! That was an invalid command. Try 'help' for suggestions";
String messageReformatCheck="Are you sure you would like to reformat flash memory? 1 - Yes, 0 - No";
String messageReformat="Reformatted Flash Memory";
String messageReformatNot="Reformat Operation Aborted";
String messageResetCheck="Are you sure you would like to reset this device? 1 - Yes, 0 - No";
String messageReset="Resetted Device";
String messageResetNot="Reset Operation Aborted";
String messageAutoUpdateCheck="Type 1 to enable Auto Updates and 0 to disable Auto Updates 1 - Yes, 0 - No";
String messageAutoUpdateTrue="Auto Updates are enabled";
String messageAutoUpdateFalse="Auto Updates are disabled";
String messageSwitchCheck="Are you sure you would like to switch to your desired program? 1 - Yes, 0 - No";
String messageSwitchFalse="Switch Operation Aborted";
String messageSetFw="Firmware versions start from 1000 and the closest firmware version value would be considered. Type your desired firmware version for the program chosen.";
String messageSetProgram="Program Choice can be set as follows: \n 1. Core\n2. Temperature Logger";
String messageEndCase="\nExiting Configuration. Try 'help' for suggestions.";

// temporary variables
String data="",data3s="";
int data2,data3;

// addresses of variables stored in flash memory
int addressName=0;
int addressLocation=100;
int addressSSID=200;
int addressWifiPassword=300;
int addressAutoUpdate=400;
int addressProgramChoice=410;
int addressVersionChoice=420;

// variables found in flash memory
String name,location,wifiSsid,wifiPassword,autoUpdate,programChoice,versionChoice;

//clearMemory sets all values to 0 in memory
void clearMemory(){
  Serial.println("\nClearing Memory");
  for (int i = 0; i < 512; ++i) {
      EEPROM.write(i, 0);
    }
    EEPROM.commit();
    delay(500);
}

//clearMemoryAt sets values to 0 at specified memory locations
void clearMemoryAt (int startAddress){
  int limit=100; // usual limit
  if(startAddress>=400) 
  limit=10; // limit is set to 10 for addresses addressAutoUpdate,addressProgramChoice,addressVersionChoice
  Serial.println("\nClearing Memory at address "+startAddress);
  for (int i = startAddress; i < limit+startAddress; i++) {
      EEPROM.write(i, 0);
    }
    EEPROM.commit();
}

//writes to Memory with parameters start address and data
void writeMemory(int startAddress, String writeData){
  int i;
  if(startAddress>=400)
  clearMemoryAt(startAddress); // flash memory is explicitly reformatted for addresses addressAutoUpdate,addressProgramChoice,addressVersionChoice
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

//reads characters from memory at specified address and returns the string that was read
String readMemory(int startAddress){
  int limit=100; //usual limit
  if(startAddress>=400)
  limit=10; // limit is set to 10 for addresses addressAutoUpdate,addressProgramChoice,addressVersionChoice
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

//to write to memory with specialized operations - container
void writeMemoryContainer(String variableName,int address){
   // clear memory at address
   clearMemoryAt(address);
   Serial.println("Enter "+variableName+" to set for this device");
   //wait for input from user
   while(!Serial.available()){};
   String data = Serial.readString();
   writeMemory(address,data);
}

void updateValues(){ // updates all the temporary variables as the flash memory data
  name=readMemory(addressName);
  location=readMemory(addressLocation);
  wifiSsid=readMemory(addressSSID);
  wifiPassword=readMemory(addressWifiPassword);
  autoUpdate=readMemory(addressAutoUpdate);
  programChoice=readMemory(addressProgramChoice);
  versionChoice=readMemory(addressVersionChoice);
}

void UserInterface(){ // Users inputs and outputs operate here through Bluetooth Terminal as User Interface.
  if (Serial.available()){ // input on user interface -  bluetooth terminal always checked
  data = Serial.readString(); // first input by user read
  Serial.println(data); // printed out for user convenience
  if(data=="config") // config operations performed here
  {
    Serial.println(messageConfig); //print statements related to config operations
    while(!Serial.available()){}; //wait for input from user
    data2 = Serial.parseInt(); // second input by user that is part of config chain input
    switch (data2) { 
    case 1: {
       writeMemoryContainer("name",addressName); // writes new name of device to appropriate address
       break;
    }
    case 2: { 
       writeMemoryContainer("location",addressLocation); // writes new location of device to appropriate address
       break;
    }
    case 3: { 
       writeMemoryContainer("WiFi SSID",addressSSID); // writes new WiFi SSID of device to appropriate address
       break;
    }
    case 4: { 
       writeMemoryContainer("WiFi Password",addressWifiPassword); // writes new WiFi Password of device to appropriate address
       break;
    }
    default: { 
       Serial.println(messageEndCase); // default case that considers any other values
       break;
    }
    }
  }
  
  else if(data=="read") // read operations performed here
  {
    Serial.println(messageRead);
    while(!Serial.available()){}; //wait for input from user
    data2 = Serial.parseInt(); // second input by user that is part of read chain input
    switch (data2) { 
    case 1: {
       readMemory(addressName); // prints out the current name of the device
       break;
    }
    case 2: { 
       readMemory(addressLocation); // prints out the current location of the device
       break;
    }
    case 3: { 
       readMemory(addressSSID); // prints out the current WiFi SSID of the device
       break;
    }
    case 4: { 
       readMemory(addressWifiPassword); // prints out the current WiFi Password of the device
       break;
    }
    case 5: { 
       Serial.println(WiFi.macAddress()); // prints out the MAC Address of the device
       break;
    }
    case 6: { 
       readMemory(addressAutoUpdate); // prints out the current flag for Auto Update of the device
       break;
    }
    case 7: { 
       readMemory(addressVersionChoice); // prints out the current Version Choice of the user
       break;
    }
    case 8: { 
       readMemory(addressProgramChoice); // prints out the current Program Choice of the user
       break;
    }
    case 9: { 
       Serial.println("Firmware version: " +String(fwVersion)); // prints out the current Firmware Version of this program
       break;
    }
    default: { 
       Serial.println(messageEndCase); // default case that considers any other values
       break;
    }
    }
  }
  else if(data=="help") // help operations performed here
  {
    Serial.println(messageHelp); // large feedback of help printed
  }
  else if(data=="reformat") // reformat operation performed here - reformats flash memory that stores variables.
  {
    Serial.println(messageReformatCheck); // to reconfirm with the user whether to proceed to reformat.
    while(!Serial.available()){};
    data3 = Serial.parseInt(); // second input by user that is part of reformat chain input. Considers int
    if(data3==1)
    {
    clearMemory();
    Serial.println(messageReformat);
    }
    else
    Serial.println(messageReformatNot);
  }
  else if(data=="reset") // reset operations performed here
  {
    Serial.println(messageResetCheck);
    while(!Serial.available()){};
    data3 = Serial.parseInt();
    if(data3==1)
    {
    ESP.reset();
    Serial.println(messageReset);
    }
    else
    Serial.println(messageResetNot);
  }
  else if(data=="autoupdate") // Auto Update operations performed here - flag is updated
  {
    Serial.println(messageAutoUpdateCheck);
    while(!Serial.available()){};
    data3 = Serial.parseInt();
    if(data3==1) {
    writeMemory(addressAutoUpdate,"1");
    Serial.println(messageAutoUpdateTrue);
    }
    else {
    writeMemory(addressAutoUpdate,"0");
    Serial.println(messageAutoUpdateFalse);
    }
  }
  else if(data=="setfw") // Desired firmware version by the user set here
  {
    Serial.println(messageSetFw);
    while(!Serial.available()){};
    data3s = Serial.readString();
    writeMemory(addressVersionChoice,data3s);
  }
  else if(data=="setprogram") // Desired program option by the user set here
  {
    Serial.println(messageSetProgram);
    while(!Serial.available()){};
    data3s = Serial.readString();
    writeMemory(addressProgramChoice,data3s);
  }
  else if(data=="updatevalues")
  {
    updateValues(); // updates all the temporary variables as the flash memory data
  }
  else if(data=="update") //updates current program to latest firmware
  {
    initialSetup();
    updateSetup(String(programOption),0);
  }
  else if(data=="switch")
  {
    Serial.println(messageSwitchCheck);
    while(!Serial.available()){};
    data3 = Serial.parseInt();
    if(data3==1) {
    updateSetup(programChoice,1);
    }
    else {  
    Serial.println(messageSwitchFalse);
    }
  }
  else
  {
    Serial.println("Error:" + data +  " - " + messageInvalid); // print out invalid message and suggest options for user
  }
  
}
}

void checkForUpdates(String programCode,int switchEnable) { //updates is checked for the specific program code
  //check latest fwVersion
  if (WiFiMulti.run(2000) == WL_CONNECTED) { // check if WiFi is connected
    Serial.println("Connection established");
    WiFiClient client; // http and client objects are set here
    HTTPClient http;
    if (http.begin(client, serverLink+"/"+programCode+"/version.txt")) {   // the text file link is set here
      int getResultNum = http.GET();
      if (getResultNum > 0) {
        Serial.printf("HTTP GET Number: %d\n", getResultNum);
        if (getResultNum == HTTP_CODE_OK || getResultNum == HTTP_CODE_MOVED_PERMANENTLY) {
          latestVersion = http.getString(); // the latest version of the existing binary file is returned
        }
      } else {
        Serial.printf("Error");
      }
      http.end();
    } else {
      Serial.printf("Failed to Connect\n");
    }
    if( (latestVersion.toInt() > fwVersion) || switchEnable ) { //update if latest firmware version higher than current firmware version.
      Serial.println("Newer firmware found, version "+latestVersion);
      Serial.println("Current firmware version "+fwVersion);
      Serial.println("Proceeding to update...");
      String latestFwLink = serverLink+"/"+latestVersion+".bin";
      t_httpUpdate_return updateResponse = ESPhttpUpdate.update( latestFwLink ); // update performed here

      switch(updateResponse) { // handle cases where connection is not successful
        case HTTP_UPDATE_FAILED:
          Serial.printf("Error 10");
          break;

        case HTTP_UPDATE_NO_UPDATES:
          Serial.println("Error 11");
          break;
      }
    }
    else {
      Serial.println( "Already on latest version" );
    }
    checkedUpdate=1; // this variable is set after the updates are checked
  }
  else{
    Serial.println("Connection not established");
    yield(); // essential to continue without updates
  }
}

void initialSetup(){ // initial setup of wifi common to all programs
  EEPROM.begin(512); // to set up flash memory
  Serial.begin(9600);
  WiFi.mode(WIFI_STA); // mentioning current device as wifi station
  WiFiMulti.addAP(wifiSsid.c_str(),wifiPassword.c_str()); // setting an access point for current device with username and password of wifi mentioned
}

void updateSetup(String programCode,int switchEnable){ // update is done with the specified program code
  Serial.println("Proceed to check for updates");
  Serial.println("Connecting to server ...");
  Serial.flush();
  checkUpdateCount=0;
  checkedUpdate=0;
  while(!checkedUpdate){
    checkUpdateCount++;
    checkForUpdates(programCode,switchEnable); // first updates are checked here to see if there is a newer version of this file
    if(checkUpdateCount>10)
    checkedUpdate=1;
  }
  Serial.println("Update Setup over");
}

void setupCore(){ // the cetup for the core program - will be used in every other program 
  initialSetup();
  updateValues();
  if(autoUpdate) // updates only if autoupdate activated
  updateSetup(String(programOption),0);
}

void setup() {
  setupCore();
}

void loop() {
  UserInterface(); // User's inputs and outputs operate here through Bluetooth Terminal as User Interface.
} 

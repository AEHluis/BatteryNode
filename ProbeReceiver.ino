#define  MQTTBROKER           true

#include <ESP8266WiFi.h>
#if MQTTBROKER
#include "uMQTTBroker.h"

char ssid[] = "";     // your network SSID (name)
char pass[] = ""; // your network password
char gateway[] = "ESP"; // your gateway name

#endif

// ==================== start of TUNEABLE PARAMETERS ====================

int device;
int voltage;
int temperature;
int humidity;
int pressure;
int light;
//int openClose = 0;    // 0 or 1
//int level;            // 0 to 255
//int presence = 0;     // 0 or 1
//int motion = 0;       // 0 to 6 (0=no motion, 1=up, 2=down, 3=right, 4=left, 5=on, 6=off).
/*
 Predefined sensor type table is below:
 volatage = 16, temperature = 26, humidity= 36, pressure= 46, light= 56, OpenClose = 66,
 level = 76, presence = 86, motion = 96 etc.
 */ 

int sensorType1;
int sensorType2;
int sensorType3;
int sensorType4;
int sensorType5;


//uint8_t probeData[] = {temperature, humidity, pressure, voltage, light, unit};

int command1;
int command2;
int command3;
int command4;
int command5;
int command6;

//uint8_t mac[6] = {19,01,25,30,01,01};
//uint8_t mac[6] = {19,01,250,value4,value5,01};
uint8_t mac[6] = {command1, command2, command3, command4, command5, command6};

extern "C" void preinit() {
  wifi_set_opmode(STATIONAP_MODE);
  wifi_set_macaddr(SOFTAP_IF, mac);
}


/*/////////////////////////////////////////////////////////////////
   Custom broker class with overwritten callback functions
*/


#if MQTTBROKER

class myMQTTBroker: public uMQTTBroker
{
  public:
    virtual bool onConnect(IPAddress addr, uint16_t client_count) {
      Serial.println(addr.toString() + " connected");
      return true;
    }

    virtual bool onAuth(String username, String password) {
      Serial.println("Username/Password: " + username + "/" + password);
      return true;
    }

    virtual void onData(String topic, const char *data, uint32_t length) {
      char data_str[length + 1];
      os_memcpy(data_str, data, length);
      data_str[length] = '\0';

      Serial.println("received topic '" + topic + (String)data_str);
    }
};

myMQTTBroker myBroker;

#endif
/////////////////////////////////////////////////////////////////////

WiFiEventHandler probeRequestPrintHandler;
//volatile boolean buttonPressed;

void setup() {
  
  Serial.begin(115200);
  Serial.println();

  // WiFi.persistent(false);

  Serial.println("MQTT Gateway");
  Serial.println();
  Serial.print("This Device MAC ID is: ");
  Serial.println(WiFi.macAddress());

  WiFi.hostname("Controller");
  Serial.print("This Device's Host Name is: ");
  Serial.println(WiFi.hostname());
  Serial.print("This Device is connected to  AP MAC: ");
  Serial.println(WiFi.BSSIDstr());

  Serial.println();
 
  WiFi.persistent(false);
  probeRequestPrintHandler = WiFi.onSoftAPModeProbeRequestReceived(&onProbeRequest);
  delay(1);
  wifi_set_macaddr(SOFTAP_IF, mac); 
  
////////////////////////////////
#if MQTTBROKER
  // Connect to a WiFi network
  startWiFiClient();

  // Start the broker
  Serial.println("Starting MQTT broker");
  myBroker.init();

  /*
     Subscribe to anything
  */
  myBroker.subscribe("#");
#endif

  ////////////////////////////////////////////////////////////////////


}  //END OF SETUP


void loop() {

  command1 = 10;  //random(25);
  command2 = 20;  //random(256);
  command3 = 30;  //random(256);
  command4 = 40;  //random(256);
  command5 = random(256);
  command6 = 01;
  mac[0] = command1;
  mac[1] = command2;
  mac[2] = command3;
  mac[3] = command4;
  mac[4] = command5;
  mac[5] = command6;

  wifi_set_macaddr(SOFTAP_IF, mac);

  yield();

}  // End of main loop.

///////////////////////////////////////////////////////////////////////////////////////////////////

#if MQTTBROKER
void startWiFiClient()
{
  Serial.println("Connecting to " + (String)ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.print("WiFi connected to: ");
  Serial.println(WiFi.SSID());
  Serial.println("IP address: " + WiFi.localIP().toString());
  WiFi.softAP(gateway, "<notused>", 7, 0, 0);
}
#endif

////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
#if MQTTBROKER
void mqttPublish()    {
/*
 
 Predefined sensor type table is below:
 volatage = 16, temperature = 26, humidity= 36, pressure= 46, light= 56, OpenClose = 66,
 level = 76, presence = 86, motion = 96 etc.
 */ 

 char topic1[100];
 sprintf(topic1,"%s%i%s%i%s", "Sensordata/", device, "/", device, "/");
 char topic2[100];
 sprintf(topic2,"%s%i%s%i%s", "Sensordata/", device, "/", sensorType1, "/");
  char topic3[100];
 sprintf(topic3,"%s%i%s%i%s", "Sensordata/", device, "/", sensorType2, "/");
 char topic4[100];
 sprintf(topic4,"%s%i%s%i%s", "Sensordata/", device, "/", sensorType3, "/");
  char topic5[100];
 sprintf(topic5,"%s%i%s%i%s", "Sensordata/", device, "/", sensorType4, "/");
 char topic6[100];
 sprintf(topic6,"%s%i%s%i%s", "Sensordata/", device, "/", sensorType5, "/");
 
 // myBroker.publish(topic1, (String)device);
 // myBroker.publish("SensorData/device/", (String)device);
  myBroker.publish(topic2, (String)voltage);
  myBroker.publish(topic3, (String)temperature);
  myBroker.publish(topic4, (String)humidity);
  myBroker.publish(topic5, (String)pressure);
  myBroker.publish(topic6, (String)light);

  // wait a second

  //delay(1000);
}

void mqttPublish1()    {

  
  
  
 // myBroker.publish("SensorData/device/", (String)device);
  myBroker.publish("SensorType/", (String)sensorType1);
  myBroker.publish("SensorType/", (String)sensorType2);
  myBroker.publish("SensorType/", (String)sensorType3);
  myBroker.publish("SensorType/", (String)sensorType4);
  myBroker.publish("SensorType/", (String)sensorType5);

  // wait a second

  //delay(1000);
}

#endif

//////////////////////////////////////////////////////////////////////////////
 
  
void onProbeRequest(const WiFiEventSoftAPModeProbeRequestReceived& dataReceived) {

  if (dataReceived.mac[1] == 16) // only accept data from device with voltage as a sensor type at byte 1.
  {
     
    Serial.print("Probe Request:- ");
    Serial.print(" Device ID:  ");
    Serial.print(dataReceived.mac[0]);
    //device = (dataReceived.mac[0]);
    Serial.print(" Voltage:  ");
    Serial.print(dataReceived.mac[1]);
    sensorType1 = (dataReceived.mac[1]);
    Serial.print(" Temperature:  ");
    Serial.print(dataReceived.mac[2]);
    sensorType2 = (dataReceived.mac[2]);
    Serial.print(" Humidity:  ");
    Serial.print(dataReceived.mac[3]);
    sensorType3 = (dataReceived.mac[3]);
    Serial.print(" Pressure:  ");
    Serial.print(dataReceived.mac[4]);
    sensorType4 = (dataReceived.mac[4]);
    Serial.print(" Light:  ");
    Serial.println(dataReceived.mac[5]);
    sensorType5 = (dataReceived.mac[5]);
    
  }
  
  if (dataReceived.mac[0] == 6 || dataReceived.mac[0] == 16 || dataReceived.mac[0] == 26 || dataReceived.mac[0] == 36 || dataReceived.mac[0] == 46 || dataReceived.mac[0] == 56 || dataReceived.mac[0] == 66 || dataReceived.mac[0] == 76 || dataReceived.mac[0] == 86 || dataReceived.mac[0] == 96 || dataReceived.mac[0] == 106 || dataReceived.mac[0] == 116 || dataReceived.mac[0] == 126 || dataReceived.mac[0] == 136 || dataReceived.mac[0] == 146 || dataReceived.mac[0] == 156 || dataReceived.mac[0] == 166 || dataReceived.mac[0] == 176 || dataReceived.mac[0] == 186 || dataReceived.mac[0] == 196 || dataReceived.mac[0] == 206 || dataReceived.mac[0] == 216 || dataReceived.mac[0] == 226 || dataReceived.mac[0] == 236 || dataReceived.mac[0] == 246) // only accept data from certain devices.
  {
    
    Serial.print("Probe Request:- ");
    Serial.print(" Device ID:  ");
    Serial.print(dataReceived.mac[0], DEC);
    device = dataReceived.mac[0];
    Serial.print(" Voltage:  ");
    Serial.print(dataReceived.mac[1], DEC);
    voltage = dataReceived.mac[1];
    voltage = voltage * 2;
    Serial.print(" Temperature:  ");
    Serial.print(dataReceived.mac[2], DEC);
    temperature = dataReceived.mac[2];
    Serial.print(" Humidity:  ");
    Serial.print(dataReceived.mac[3], DEC);
    humidity = dataReceived.mac[3];
    Serial.print(" Pressure:  ");
    Serial.print(dataReceived.mac[4], DEC);
    pressure = dataReceived.mac[4];
    pressure = pressure * 4;
    Serial.print(" Light:  ");
    Serial.println(dataReceived.mac[5], DEC);
    light = dataReceived.mac[5];

      if (voltage < 295)      // if voltage of battery gets to low, print the warning below.
  {
    myBroker.publish("SensorData/warning", "Battery Low");
    Serial.println("**************Warning :- Battery Voltage low please change batteries********************" );
    Serial.println();
  }  
   if (dataReceived.mac[1] > 115 && dataReceived.mac[1] < 180)  {
    mqttPublish();
   }

  } else {

    //Serial.println("Waiting for Data............");

  }
  
}
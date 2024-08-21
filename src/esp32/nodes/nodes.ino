// time : 21:14
// date : 2024-08-20
#include <ArduinoJson.h>
#include "painlessMesh.h"

#include "./lib/sensors.h"
#define   MESH_PREFIX     "GCEK_ESP_MESH"
#define   MESH_PASSWORD   "linuxisnotunix"
#define   MESH_PORT       9090


// to Identify The node
int nodeNumber = 1;
const int echoPin = 5 ;
const int trigerPin = 18 ;

String readings;

String getReadings();
void sendMessage(); 

 
Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;
uint32_t gatewayId = 4193071321;

// User stub

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  // String msg = "Hello from node ";
  // msg += mesh.getNodeId();

  // mesh.sendBroadcast( msg );
  String msg = getReadings();
  mesh.sendSingle(gatewayId, msg);
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
 
  Serial.printf("from %u msg=%s\n", from, msg.c_str()); 
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

String getReadings(){
    
  StaticJsonDocument<200> doc; // Adjust size as needed
  doc["node"] = nodeNumber;
  doc["distance"] = getUltrasonicReadings(echoPin,trigerPin);
  
  String readings;
  Serial.println(readings);
  serializeJson(doc, readings);
  return readings;
}

void setup() {
  Serial.begin(9600);

  pinMode(trigerPin, OUTPUT); 
  pinMode(echoPin, INPUT); 

//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes(   ERROR | STARTUP);  // set before init() so that you can see startup messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}



void loop() {
  mesh.update();
  

delay(1000);


}

// time : 21:14
// date : 2024-08-20
#include <LiquidCrystal_I2C.h>

#include "painlessMesh.h"
#define   MESH_PREFIX     "GCEK_ESP_MESH"
#define   MESH_PASSWORD   "linuxisnotunix"
#define   MESH_PORT       9090
LiquidCrystal_I2C lcd(0x27,16,2);

// INFO: All the objects should be created here
Scheduler userScheduler; 
painlessMesh  mesh;

// INFO: Functions for painlessMesh other functions should go above

void sendMessage() ; 
Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  String msg = "Hello from Gateway ";
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, msg);
    // Check if deserialization was successful
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }
    int nodeId = doc["node"];         
    float distance = doc["distance"]; 

  lcd.setCursor(0,0);
  lcd.print("Node ID  " +String(nodeId));
  lcd.setCursor(0,1);
  lcd.print("Distance " + String(distance));
  Serial.printf("from %u msg=%s\n", from, msg.c_str()); 
  Serial.printf("form Node %d distance = %f",nodeId,distance);
  
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

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.clear();         
  lcd.backlight();
  Serial.println("This ID " + String(mesh.getNodeId()));
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
  // it will run the user scheduler as well
  mesh.update();
  // lcd.setCursor(0,1);
  // lcd.scrollDisplayLeft();  
  // delay(700);  
  
}

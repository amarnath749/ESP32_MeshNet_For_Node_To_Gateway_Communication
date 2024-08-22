// time : 21:14
// date : 2024-08-20
#include <LiquidCrystal_I2C.h>

#include "painlessMesh.h"
#include <WiFi.h>
#define   MESH_PREFIX     "GCEK_ESP_MESH"
#define   MESH_PASSWORD   "linuxisnotunix"
#define   MESH_PORT       9090

const char* ssid = "802.11";
const char* password = "12345678p";

// Set portnumber to 80
WiFiServer server(80);

LiquidCrystal_I2C lcd(0x27,16,2);

String header;
// Initialized it as global variable so that it will be accessible from any part of the system
float distance ; 


unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;


// INFO: All the objects should be created here
Scheduler userScheduler; 
painlessMesh  mesh;

// INFO: Functions for painlessMesh other functions should go above
void sendMessage() ; 
// Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  String msg = "Hello from Gateway ";
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  // taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
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

  
  // lcd.setCursor(0,1);
  // lcd.print("Node ID "+ String(nodeId) + "Distance " + String(distance));
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

  
  // WiFi.mode(WIFI_AP);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    lcd.setCursor(0,0);
    lcd.print("#");
    lcd.print(WiFi.localIP());

    Serial.println("Connected to Wi-Fi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

  Serial.println("This ID " + String(mesh.getNodeId()));
  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes(   ERROR | STARTUP);  // set before init() so that you can see startup messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT,WIFI_AP ); // 6 -> wifi channel
  mesh.onReceive(&receivedCallback);
  // mesh.onNewConnection(&newConnectionCallback);
  // mesh.onChangedConnections(&changedConnectionCallback);
  // mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  // userScheduler.addTask( taskSendMessage );
  // taskSendMessage.enable();
  server.begin();
}

void loop() {

  WiFiClient client = server.available();
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  lcd.setCursor(0,1);
  lcd.print(WiFi.RSSI());
  lcd.print(" dBm");

if (client) {
 currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          
    String currentLine = "";                
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  
      currentTime = millis();
      if (client.available()) {             
      char c = client.read();             
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                    
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // if (header.indexOf("GET /02/on") >= 0) {
            //   Serial.println("GPIO 02 on");
            //   current_led_status = "on";
            //   digitalWrite(LED_BUILTIN, HIGH);
            // } else if(header.indexOf("GET /02/off") >= 0) {
            //   Serial.println("GPIO 02 off");
            //   current_led_status = "off";
            //   digitalWrite(LED_BUILTIN, LOW);
            // }             
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            client.println("<body><h1>BUILTIN Led Control</h1>");
            
            client.println("<p>BUILTIN LED  - State " + String(distance) + "</p>");
            // if (current_led_status=="off") {
            //   client.println("<p><a href=\"/02/on\"><button class=\"button\">ON</button></a></p>");
            // } else {
            //   client.println("<p><a href=\"/02/off\"><button class=\"button button2\">OFF</button></a></p>");
            // } 
               
            client.println("</body></html>");
            
            client.println();
            break;
          } else {
          currentLine = "";
          }
        } else if (c != '\r') { 
          currentLine += c;     
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

  // it will run the user scheduler as well
  // lcd.setCursor(0,1);
  // lcd.scrollDisplayLeft();  
  // delay(700);  
  int numberAvailable = (int)  WiFi.scanNetworks() ;
  delay(140);
   lcd.clear();
  Serial.println("Available Networks " + numberAvailable);
  for(int i=0; i< numberAvailable ; ++i){
    lcd.setCursor(0,0);
    lcd.print("id ");
    lcd.print(WiFi.SSID(i));
    lcd.setCursor(0, 1);
    lcd.print("Str ");
    lcd.print(WiFi.RSSI(i));
    lcd.print(" dBm");
    delay(1000);
  }
  7
}

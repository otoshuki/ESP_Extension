//Guining Pertin
//WiFi Controller extension board

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

//WiFi Access Point details
const char *ssid = "ESP_Extension";
const char *password = "extension";

//Other variables
int control1 = 13;
int control3 = 12;
int control4 = 14;
bool state1 = HIGH;
bool state3 = HIGH;
bool state4 = HIGH;

//Server port
WiFiServer server(80);

void setup() {
  pinMode(control1, OUTPUT);
  pinMode(control3, OUTPUT);
  pinMode(control4, OUTPUT);

  digitalWrite(control1, HIGH);
  digitalWrite(control3, HIGH);
  digitalWrite(control4, HIGH);
  
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();  

  if (client) {                            
    Serial.println("New Client.");          
    String currentLine = "";               
    while (client.connected())             
    {            
      if (client.available())              
      {            
        char c = client.read();             
        Serial.write(c);                    
        if (c == '\n')                      
        { 
  
          if (currentLine.length() == 0) {
            
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            //HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><title>EXTENSION CONTROLLER</title>");
            client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/>");
            
            client.println("<link rel=\"icon\" href=\"data:,\">");
            //CSS to style buttons
            client.println("<style>html {font-family: Montserrat; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button1 {color: #D0D0D0; background-color: #10BC10; font-size: 30px; font-weight: bold; border: 5px solid #B4B4B4;");
            client.println(" padding: 30px 50px; cursor: pointer; margin: 2px; text-decoration: none; border-radius: 20px;}");
            
            client.println(".button2 {color: black; background-color: #F44336; font-size: 30px; font-weight: bold; border: 5px solid #494646;");
            client.println(" padding: 30px 50px; cursor: pointer; margin: 2px; text-decoration: none; border-radius: 20px;}");
            
            client.println("</style></head>");
            //Start body
            client.println("<body style=\"background-color: black;\"><h1 style=\"color: #D0D0D0;\">EXTENSION CONTROLLER MK.II</h1>");
            
            if(!state1) client.println("<p><a href=\"/C1\"><button class=\"button1\">EXT Port1<br>ON</button></a></p>");
            else client.println("<p><a href=\"/C1\"><button class=\"button2\">EXT Port1<br>OFF</button></a></p>");
            
            if(!state3) client.println("<p><a href=\"/C3\"><button class=\"button1\">EXT Port3<br>ON</button></a></p>");
            else client.println("<p><a href=\"/C3\"><button class=\"button2\">EXT Port3<br>OFF</button></a></p>");
            
            if(!state4) client.println("<p><a href=\"/C4\"><button class=\"button1\">EXT Port4<br>ON</button></a></p>");
            else client.println("<p><a href=\"/C4\"><button class=\"button2\">EXT Port4<br>OFF</button></a></p>");
            
            // The HTTP response ends with another blank line:
            client.println();
            
            // break out of the while loop:
            break;
          } 
          else 
          { 
    
            currentLine = "";
          }
        } 
        else if (c != '\r') { 
          currentLine += c;      
        }
      
        //Check to set the states
        if (currentLine.endsWith("GET /C1"))
        {
          state1 = !state1;
          digitalWrite(control1, state1);
        }
        if (currentLine.endsWith("GET /C3"))
        {
          state3 = !state3;
          digitalWrite(control3, state3);
        }
        if (currentLine.endsWith("GET /C4"))
        {
          state4 = !state4;
          digitalWrite(control4, state4);
        }
        
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

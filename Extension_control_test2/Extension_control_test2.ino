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
//int control2 = 17;
int control3 = 12;
int control4 = 14;
bool state1 = HIGH;
//bool state2 = HIGH;
bool state3 = HIGH;
bool state4 = HIGH;

//Server port
WiFiServer server(80);

void setup() {
  pinMode(control1, OUTPUT);
//  pinMode(control2, OUTPUT);
  pinMode(control3, OUTPUT);
  pinMode(control4, OUTPUT);

  digitalWrite(control1, HIGH);
//  digitalWrite(control2, HIGH);
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
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");          // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected())              // loop while the client's connected
    {            
      if (client.available())               // if there's bytes to read from the client,
      {            
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n')                      // if the byte is a newline character
        { 
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
      
            client.print("<a href=\"/C1\"><button type=\"button\">EXT Port1</button></a>");
            if(state1) client.print(" --> OFF<br>");
            else client.print(" --> ON<br>");
//            client.print("<a href=\"/C2\"><button type=\"button\">EXT Port2</button></a>");
//            if(state2) client.print(" --> OFF<br>");
//            else client.print(" --> ON<br>");
            client.print("<a href=\"/C3\"><button type=\"button\">EXT Port3</button></a>");
            if(state3) client.print(" --> OFF<br>");
            else client.print(" --> ON<br>");
            client.print("<a href=\"/C4\"><button type=\"button\">EXT Port4</button></a>");
            if(state4) client.print(" --> OFF<br>");
            else client.print(" --> ON<br>");
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } 
          else 
          { 
            // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } 
        else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      
        //Check to set the states
        if (currentLine.endsWith("GET /C1"))
        {
          state1 = !state1;
          digitalWrite(control1, state1);
        }
//        if (currentLine.endsWith("GET /C2"))
//        {
//          state2 = !state2;
//          digitalWrite(control2, state2);
//        } 
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

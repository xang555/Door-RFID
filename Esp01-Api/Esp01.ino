#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <conf.h>

#define COMMAND_PIN 2

/* Set these to your desired credentials. */
const char *ssid = "Laoitdev-Door-RFID-03";
const char *password = "LaoItDeV555";

ESP8266WebServer server(80);


//Check if header is present and correct
bool is_authentified(String verifyfrom){
  Serial.println("Enter is_authentified");
  if (server.hasHeader(verifyfrom)){   
    Serial.print("Found Token: ");
    String cookie = server.header(verifyfrom);
    Serial.println(cookie);
    if (cookie.indexOf(verify_cookie) != -1) {
      Serial.println("Authentification Successful");
      return true;
    }
  }
  Serial.println("Authentification Failed");
  return false;  
}


void handleLogin(){

  if (server.hasArg("uname") && server.hasArg("passwd")){
    if (server.arg("uname") == Uname &&  server.arg("passwd") == Passwd ){
      String header = "HTTP/1.1 301 OK\r\nSet-Cookie: "+verify_cookie+"\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
      server.sendContent(header);
      Serial.println("Log in Successful");
      return;
    }
    String header = "HTTP/1.1 301 OK\r\nLocation: /error\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    Serial.println("Log in Failed");
    return;
  }
  sendFile(200,"text/html",data_loginHTML,sizeof(data_loginHTML));
}

void handleErrorPage(){
    
}

void handleSendCommand() {

  if (!is_authentified("token")){
    server.send(401,"application/json","{\"err\" : \"401\",\"msg\":\"authentication fail\"}");
    return;
  }
  
  if(server.hasArg("cmd") && server.hasArg("submit")) {
    if(server.arg("cmd") == "1") {
         digitalWrite(COMMAND_PIN,HIGH); 
         delay(1000);
         digitalWrite(COMMAND_PIN,LOW);
        server.send(200,"application/json","{\"err\" : \"0\",\"msg\":\"send command success\"}");
        return; 
    }
  }

  server.send(301,"application/json","{\"err\" : \"301\",\"msg\":\"no command\"}");
  
}

/* handle API */


void setup() {
	Serial.begin(115200);
	Serial.println();
	Serial.print("Configuring access point...");
  pinMode(COMMAND_PIN,OUTPUT);
  digitalWrite(COMMAND_PIN,LOW);
  
	/* You can remove the password parameter if you want the AP to be open. */
	WiFi.softAP(ssid, password);

	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
  Serial.println(myIP);
  
  server.on("/login",handleLogin);
  server.on("/cmd",HTTP_POST,handleSendCommand);
  server.onNotFound(handleErrorPage);
  
  //here the list of headers to be recorded
  const char * headerkeys[] = {"User-Agent","token"};
  size_t headerkeyssize = sizeof(headerkeys)/sizeof(char*);
  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize );
 
	server.begin();
	Serial.println("HTTP server started");
}

void loop() {
	server.handleClient();
}

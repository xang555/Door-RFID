#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include "data.h"

/* Set these to your desired credentials. */
const char *ssid = "Laoitdev-Door-RFID-01";
const char *password = "12345678";

ESP8266WebServer server(80);


/* authentication */

//Check if header is present and correct
bool is_authentified(){
  Serial.println("Enter is_authentified");
  if (server.hasHeader("Cookie")){   
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
    if (cookie.indexOf(verify_cookie) != -1) {
      Serial.println("Authentification Successful");
      return true;
    }
  }
  Serial.println("Authentification Failed");
  return false;  
}


/* send response to client */

void sendHeader(int code, String type, size_t _size) {
  server.setContentLength(_size);
  server.send(code, type, "");
}

void sendFile(int code, String type, const char* adr, size_t len) {
  sendHeader(code,type,len);
  server.sendContent_P(adr,len);
}

/* handle load ui page */
void handleRoot() {
   if (!is_authentified()){
    String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  sendFile(200,"text/html",data_indexHTML,sizeof(data_indexHTML)); 
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
    sendFile(200,"text/html",data_errorHTML,sizeof(data_errorHTML));
}

void handleSuccess() {
   sendFile(200,"text/html",data_successHTML,sizeof(data_successHTML));
}

void handleLoadBootstrap(){
	sendFile(200,"text/css;charset=UTF-8",data_css_bootstrap_minCSS,sizeof(data_css_bootstrap_minCSS));
}

void handleLoadBootStrapTheme() {
	sendFile(200,"text/css;charset=UTF-8",data_css_bootstrap_theme_minCSS,sizeof(data_css_bootstrap_theme_minCSS));
}

void handleLoadStyleCss(){
	sendFile(200,"text/css;charset=UTF-8",data_styleCSS,sizeof(data_styleCSS));
}

void handleSendCommand() {

  if (!is_authentified()){
    String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  
  if(server.hasArg("cmd") && server.hasArg("submit")) {
    if(server.arg("cmd") == "1") {
        Serial.write("open");
        String header = "HTTP/1.1 301 OK\r\nLocation: /success\r\nCache-Control: no-cache\r\n\r\n";
        server.sendContent(header);
        return; 
    }
  }

   String header = "HTTP/1.1 301 OK\r\nLocation: /error\r\nCache-Control: no-cache\r\n\r\n";
   server.sendContent(header);
  
}

/* handle API */


void setup() {
	delay(1000);
	Serial.begin(115200);
	Serial.println();
	Serial.print("Configuring access point...");
	/* You can remove the password parameter if you want the AP to be open. */
	WiFi.softAP(ssid, password);

	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);
	server.on("/", handleRoot);
  server.on("/login",handleLogin);
  server.on("/error",handleErrorPage);
  server.on("/success",handleSuccess);
  server.on("/cmd",HTTP_POST,handleSendCommand);
  server.onNotFound(handleErrorPage);
  
	server.on("/css/bootstrap.min.css",handleLoadBootstrap);
	server.on("/css/bootstrap-theme.min.css",handleLoadBootStrapTheme);
	server.on("/style.css",handleLoadStyleCss);

  //here the list of headers to be recorded
  const char * headerkeys[] = {"User-Agent","Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys)/sizeof(char*);
  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize );
 
	server.begin();
	Serial.println("HTTP server started");
}

void loop() {
	server.handleClient();
}

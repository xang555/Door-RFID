#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include "data.h"

#define bufSize 6000
int bufc = 0; //buffer counter;

char data_websiteBuffer[bufSize];

/* Set these to your desired credentials. */
const char *ssid = "Laoitdev-Door-RFID-01";
const char *password = "12345678";

ESP8266WebServer server(80);


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
	sendFile(200,"text/html",data_indexHTML,sizeof(data_indexHTML));
}

void handleLogin(){
  sendFile(200,"text/html",data_loginHTML,sizeof(data_loginHTML));
}

void handleErrorPage(){
    sendFile(200,"text/html",data_errorHTML,sizeof(data_errorHTML));
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
	server.on("/css/bootstrap.min.css",handleLoadBootstrap);
	server.on("/css/bootstrap-theme.min.css",handleLoadBootStrapTheme);
	server.on("/style.css",handleLoadStyleCss);
	server.begin();
	Serial.println("HTTP server started");
}

void loop() {
	server.handleClient();
}

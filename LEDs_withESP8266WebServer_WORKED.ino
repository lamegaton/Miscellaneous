///********************************************************/
// credits to https://github.com/MajicDesigns/MD_MAX72XX
// Son Pham 10-17-2018
// Project: Signal Board for Bikers
/********************************************************/


#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

//Declare hardware
// I use NodeMCU ESP12E with Espressif NONOS
// 4 8x8 LED with MAX7219 
// You need to power the LED board with 5V, if not you will
// fry the 1117 on the NodeMCU

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CLK_PIN   14 // or SCK  D5
#define DATA_PIN  13 // or MOSI D7
#define CS_PIN    15 // or SS   D8

// Global message buffers shared by Wifi and Scrolling functions
#define CHAR_SPACING  1 // pixels between characters

// Global message buffers shared by Serial and Scrolling functions
#define BUF_SIZE  75
char dir = 'l';
char curMessage[BUF_SIZE];
char newMessage[BUF_SIZE];
bool newMessageAvailable = false;

uint16_t  scrollDelay = 25;  // in milliseconds
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
ESP8266WebServer server(80); //Initialize the server on Port 80

char WebResponse[] = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n";
char WebPage[] =
"<!DOCTYPE html> \n"
"<html> \n"
"<head> \n"
"<title>Signal Board</title> \n"
"\t<style>\n"
"\tbody{\n"
"\t\twidth:100%;\n"
"\t\theight:100%;\n"
"\t}\n"
"\t.button {\n"
"\t\tbackground-color: #4CAF50;\n"
"\t\tborder: none;\n"
"\t\tcolor: white;\n"
"\t\tpadding: 30px 60px;\n"
"\t\ttext-align: center;\n"
"\t\ttext-decoration: none;\n"
"\t\tdisplay: inline-block;\n"
"\t\tfont-size: 30px;\n"
"\t\tmargin: 4px 2px;\n"
"\t\tcursor: pointer;\n"
"\t}\n"
"\t.content{\n"
"\t\tmax-width: 500px;\n"
"\t\tmargin: auto;\n"
"\t\tbackground: white;\n"
"\t\tpadding: 10px;\n"
"\t}\n"
"\t</style>\n"
"</head> \n"
"</body>\n"
"<div class=\"content\">\n"
"<h1>Signal Board</h1>\n"
"<br><input type=\"button\" class=\"button\" name=\"b1\" value=\"Turn LEFT <--\" onclick=location.href='/left'> \n"
"<input type=\"button\" class=\"button\" name=\"b2\" value=\"Turn RIGHT -->\" onclick=location.href='/right'> \n"
"<input type=\"button\" class=\"button\" name=\"b3\" value=\"Turn EMERGENCY (SOS)\" onclick=location.href='/em'> \n"
"</div>\n"
"</body>\n"
"</html>";

void scrollDataSink(uint8_t dev, MD_MAX72XX::transformType_t t, uint8_t col)
// Callback function for data that is being scrolled off the display
{
#if PRINT_CALLBACK
  Serial.print("\n cb ");
  Serial.print(dev);
  Serial.print(' ');
  Serial.print(t);
  Serial.print(' ');
  Serial.println(col);
#endif
}

uint8_t scrollDataSource(uint8_t dev, MD_MAX72XX::transformType_t t)
// Callback function for data that is required for scrolling into the display
{
  static char   *p = curMessage;
  static uint8_t  state = 0;
  static uint8_t  curLen, showLen;
  static uint8_t  cBuf[8];
  uint8_t colData;

  // finite state machine to control what we do on the callback
  switch(state)
  {
    case 0: // Load the next character from the font table
      showLen = mx.getChar(*p++, sizeof(cBuf)/sizeof(cBuf[0]), cBuf);
      curLen = 0;
      state++;

      // if we reached end of message, reset the message pointer
      if (*p == '\0')
      {
        p = curMessage;     // reset the pointer to start of message
        if (newMessageAvailable)  // there is a new message waiting
        {
          strcpy(curMessage, newMessage);  // copy it in
          newMessageAvailable = false;
        }
      }
      // !! deliberately fall through to next state to start displaying

    case 1: // display the next part of the character
      colData = cBuf[curLen++];
      if (curLen == showLen)
      {
        showLen = CHAR_SPACING;
        curLen = 0;
        state = 2;
      }
      break;

    case 2: // display inter-character spacing (blank column)
      colData = 0;
      if (curLen == showLen)
        state = 0;
      curLen++;
      break;

    default:
      state = 0;
  }

  return(colData);
}

 void scrollText(char *d)
{
  static uint32_t prevTime = 0;

  // Is it time to scroll the text?
  if (millis()-prevTime >= scrollDelay)
  {
    if(*d == 'l'){
      mx.transform(MD_MAX72XX::TSL);  // scroll along - the callback will load all the data
    }
    else if(*d == 'r'){
      mx.transform(MD_MAX72XX::TSR);
    }
    prevTime = millis();      // starting point for next time
  }
}

void setup() {

  WiFi.mode(WIFI_AP); //Our ESP8266-12E is an AccessPoint 
  WiFi.softAP("O", "55086780"); // Provide the (SSID, password);
  server.on("/", handleRoot);
  server.on("/left", handleLeft);
  server.on("/right", handleRight);
  server.on("/em", handleEm);
  server.begin(); // Start the HTTP Server

  
  Serial.begin(115200);
  mx.begin();
  mx.setShiftDataInCallback(scrollDataSource);
  mx.setShiftDataOutCallback(scrollDataSink);
  strcpy(curMessage, "Be Kind! ");
  newMessage[0] = '\0';
}
void loop(){
  server.handleClient();
  scrollText(&dir);
}

void handleLeft(){
  char *direct;
  direct = &dir;
  *direct = 'l';
  server.send(200,"text/html",WebPage);
  strcpy(curMessage, "<<<<");
  //scrollText(&d);
  }
void handleRoot(){
  server.send(200,"text/html",WebPage);
  strcpy(curMessage, "1010");
  //scrollText(&d);
  }
void handleRight(){
  char *direct;
  direct = &dir;
  *direct = 'r';
  server.send(200,"text/html",WebPage);
  strcpy(curMessage, "<<<<");
  //scrollText(&d);
  }
void handleEm(){
  char *direct;
  direct = &dir;
  *direct = 'l';
  server.send(200,"text/html",WebPage);
  strcpy(curMessage, "SOS ");
  //scrollText(&d);
  }

#include <Boards.h>
#include <Firmata.h>
#include <FirmataConstants.h>
#include <FirmataDefines.h>
#include <FirmataMarshaller.h>
#include <FirmataParser.h>

#include <SPI.h>
#include <Wire.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Adafruit_PN532.h>

#define LEFT 8
#define RIGHT 7
#define NONE 9
#define BTH 10

#define PN532_IRQ   (2)
#define PN532_RESET (3)

uint32_t lastread = 0;
uint32_t timeout = 500;
bool readerDisabled = true;

// SCK = 13, MOSI = 11, MISO = 12.  The SS line can be any digital IO pin.
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);


#define DEBUG true  //set to true for debug output, false for no debug ouput
#define SerialInfo if(DEBUG)Serial
#define DEBUG_ERROR true
#define SerialError if(DEBUG_ERROR)Serial

// Enter a MAC address and IP address for your controller below.
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

byte ip[] = {192, 168, 6, 131};
IPAddress localIP(192, 168, 1, 102);
unsigned int localPort = 8080;      // local port to listen on
// IPAddress remoteIP(192, 168, 1, 103); 192.168.1.70
IPAddress remoteIP(192, 168, 1, 42);
unsigned int remotePort = 11000;     // local port to listen on
EthernetUDP Udp;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  // buffer to hold incoming packet,
char ReplyBuffer[] = "ACK";        // a string to send back
char CardBuffer[UDP_TX_PACKET_MAX_SIZE];        // a string to send back

void setup() {
  Serial.begin(115200);  
  
  nfc.begin();
  nfc.SAMConfig(); 
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    SerialError.print("Didn't find PN53x board");
    while (true); // halt
  }  
  pinMode(LEFT, OUTPUT);
  pinMode(RIGHT, OUTPUT);
  //pinMode(NONE, OUTPUT);
  digitalWrite(LEFT, HIGH);
  digitalWrite(RIGHT, HIGH);
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, localIP);
    // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    SerialError.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true); // halt
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    SerialError.println("Ethernet cable is not connected.");
  }
  // start UDP
  Udp.begin(localPort);
  SerialInfo.println("TORNO HABILITADO");
  
  nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A);
  Serial.println(Ethernet.localIP());
  // prueba();
}


void loop() {
  lectorNFC();
  delay(10);  
  receiveUDP();
 
}

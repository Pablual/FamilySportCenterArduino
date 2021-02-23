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

#define LEFT 7
#define RIGHT 8
#define NONE 9

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
IPAddress localIP(192, 168, 1, 102);
unsigned int localPort = 8080;      // local port to listen on
IPAddress remoteIP(192, 168, 1, 103);
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

  // prueba();
}


void loop() {
  if (readerDisabled) {
    nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A);
    if (millis() - lastread > timeout) {
      readerDisabled = false;
      //SerialInfo.println("Waiting for Card...");
    }
  }
  else{
    uint8_t success = false;
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
    uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    String cardidstr;
    success = nfc.readDetectedPassiveTargetID(uid, &uidLength);
    if (success) {
      // Display some basic information about the card
      SerialInfo.println("Found an ISO14443A card");
      SerialInfo.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
      SerialInfo.print("  UID Value: ");
      nfc.PrintHex(uid, uidLength);

      SerialInfo.print("Length: ");SerialInfo.println(uidLength);
      
      if (uidLength == 4)
      {
        uint32_t cardid = uid[0];
        cardid <<= 8;
        cardid |= uid[1];
        cardid <<= 8;
        cardid |= uid[2];  
        cardid <<= 8;
        cardid |= uid[3]; 
        SerialInfo.print("Seems to be a Mifare Classic card #");

        // harcodeamos el codigo a 1001 para probar.
        cardid = 1006;
        SerialInfo.println(cardid);
        SerialInfo.print(remoteIP);
        Udp.beginPacket(remoteIP,remotePort);
        
        cardidstr = String(cardid);
        cardidstr.toCharArray(CardBuffer,UDP_TX_PACKET_MAX_SIZE);
        Udp.write(CardBuffer);
        Udp.endPacket();
      }
      Serial.println("");
    }
    lastread = millis();
  }
  delay(10);  
  receiveUDP();
 
}

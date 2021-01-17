void receiveUDP(void){
  int dir = 0;
  int packetSize = Udp.parsePacket();
  if (packetSize == 3) {
    // read the packet into packetBufffer
    Udp.read(packetBuffer, packetSize);
    remoteIP = Udp.remoteIP();
    remotePort = Udp.remotePort();
    SerialInfo.print("Received ");
    SerialInfo.print(packetBuffer);
    SerialInfo.print(" from:");
    for (int i=0; i < 4; i++) {
      SerialInfo.print(remoteIP[i], DEC);
      if (i < 3) {
        SerialInfo.print(".");
      }
    }
    SerialInfo.print(", port ");
    SerialInfo.println(remotePort);
    if(strcmp(packetBuffer,"LFT")==0){
      dir = LEFT;
    }
    if(strcmp(packetBuffer,"RGT")==0){
      dir = RIGHT;
    }
    if(strcmp(packetBuffer,"NON")==0){
      dir = NONE;
    }
  if( dir == LEFT || dir == RIGHT)
  {
    SerialInfo.println("OPEN");
    digitalWrite(dir, LOW); 
    delay(3000);
    digitalWrite(dir, HIGH);     
  }
  if( dir == NONE)
  {
    for(int i = 0; i < 3; i++)
    {
        SerialInfo.println("INVALID CARD");
        digitalWrite(NONE, LOW);
        delay(500);
        digitalWrite(NONE, HIGH);
        delay(500);
    }
  }
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();
  }
}

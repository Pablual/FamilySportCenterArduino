void receiveUDP(void){
  int dir = 0;
  
  int packetSize = Udp.parsePacket();
  if (packetSize == 3) {
    readerDisabled = true;
    // read the packet into packetBufffer
    Udp.read(packetBuffer, packetSize);
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
      SerialInfo.println("Invalida card");
      dir = NONE;
    }
    if(strcmp(packetBuffer,"BTH")==0){
      dir = BTH;
    }
  if( dir == LEFT || dir == RIGHT)
  {
    SerialInfo.println("OPEN");
    digitalWrite(dir, LOW); 
    delay(3000);
  }
  if ( dir == BTH)
  {
      SerialInfo.println("OPEN CARD");
      digitalWrite(LEFT, LOW);    
      digitalWrite(RIGHT, LOW); 
      delay(3000);
  }
     digitalWrite(LEFT, HIGH);    
     digitalWrite(RIGHT, HIGH); 
    //Udp.beginPacket(remoteIP, remotePort);
    //Udp.write(ReplyBuffer);
    //Udp.endPacket();
   //readerDisabled = false;
  }
}

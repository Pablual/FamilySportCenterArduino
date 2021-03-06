// key a por defecto FFFFFFFFFFFF


void lectorNFC(){
    if (readerDisabled) {
      //nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A);
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
      
      // codigoAntiguo()
      // lector del bloque 4
       success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);  
      if (success) {
          Serial.println("Intentando autentificar bloque 4 con clave KEYA");
          uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
          Serial.println("UID a verificar: ");
          nfc.PrintHex(uid, uidLength);
         // Serial.println(uid);
          success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);   
          if (success)
          {
            Serial.println("Sector 1 (Bloques 4 a 7) autentificados");
            uint8_t data[16];
            
           // memcpy(data, (const uint8_t[]){ 'l', 'u', 'i', 's', 'l', 'l', 'a', 'm', 'a', 's', '.', 'e', 's', 0, 0, 0 }, sizeof data);
           // OJO, para escribir success = nfc.mifareclassic_WriteDataBlock (4, data);

            nfc.mifareclassic_ReadDataBlock(4, data);
            nfc.PrintHex(data, 8);

            String stringData = "";

            // numero de iteraciones menos uno
            for (int i = 0; i < 4; i++) {
                stringData += String(data[i], HEX);
            }
            Serial.println(stringData);

            Udp.beginPacket(remoteIP,remotePort);
          
            cardidstr = String(stringData);
            cardidstr.toCharArray(CardBuffer,UDP_TX_PACKET_MAX_SIZE);
            Udp.write(CardBuffer);
            Udp.endPacket();
        
            if (success)
            {          
              Serial.println("Datos leidos en bloque 4");
              delay(500);           
            }
            else
            {
              Serial.println("Fallo al escribir tarjeta");
              delay(1000);   
            }
          }
          else
          {
            Serial.println("Fallo autentificar tarjeta");
            delay(1000); 
          }
        }
      lastread = millis();
      readerDisabled = true;
    }
  }

  // codigo antiguo de lectura de uid etc, puede que este codigo asi tal cual no funcione.
  void codigoAntiguo() {
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
    
    }

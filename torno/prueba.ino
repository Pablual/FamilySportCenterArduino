void prueba(void){
   for(int i = 0; i < 3; i++)
    {
        SerialInfo.println("INVALID CARD");
        digitalWrite(LEFT, HIGH);    
        digitalWrite(RIGHT, HIGH); 
        delay(100);
        digitalWrite(LEFT, LOW);    
        digitalWrite(RIGHT, LOW); 
        delay(100);
    }
     digitalWrite(LEFT, HIGH);    
     digitalWrite(RIGHT, HIGH);      
  
  }

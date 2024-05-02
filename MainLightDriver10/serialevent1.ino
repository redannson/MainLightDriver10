 /*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent1()
{
  while (Serial1.available()) 
  {
    char inChar = Serial1.read();
    #ifdef DEBUG
    // Serial.print(inChar);
    #endif
     Serial.print(inChar);
    if(!msgStarted)
    {
      if(inChar == MSG_START_FLAG)
      {
        msgStarted =1;
        ui8RecvChecksum = 0;
        ui8ChkSumCount = 0;
        inputString += inChar;
        Receivedcount++;
      }
      else
      {
        msgStarted =0;
        Receivedcount =0;    
      }
    }
    else 
    { 
      // add it to the inputString:
      inputString += inChar;
      Receivedcount++;
      // if the incoming character is a newline, set a flag so the main loop can
      // do something about it:
      if(inChar == '*') 
      {
       // ui8RecvChecksum = 1;
        //ui8ChkSumCount = 0;
          stringComplete = true;
         Serial.println();
      }
     /* if(ui8RecvChecksum == 1)
      {
        ui8ChkSumCount++;
       /* if(ui8ChkSumCount == 3)
          {
           stringComplete = true;
          }
          }
          */
    }
  }
}

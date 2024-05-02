

/******************************************************************************************/
// Check Start string,compare if it is valid string.
//if yes return 1
//if no return 0

/*___________________________________________________________________________________________________
      int CheckStartString();
____________________________________________________________________________________________________*/
/*
 *  Function Name   : int CheckStartString();
 *  Author          : 
 *  Company         : Elcome Integrated Systems Pvt. Ltd.
 *  Date            : 17/10/2023
 *  Pass Value      : xxx
 *  Return Value    : xxx
 *  Effected Var    : xxx
 *    Register      : xxx
 *    Global Var    : xxx
 *  Stack use       : xxxByte
 *  Code Size       : xxxByte
 *  Machine  Cycle  : xxxMC Minimum
 *  Other function  :
 *  req.   
 *  function Process: Function is used to check the command type if $BCMD or $STQ
 *  Remarks         :
 */
 
int CheckStartString()
{
  String BCMDString = "$BCMD";
  String QueryString = "$STQ";
  StartIndex = inputString.indexOf("$");
  String ReceivedBCMD = inputString.substring(StartIndex, StartIndex + 5);
  String ReceivedCMDS = inputString.substring(StartIndex, StartIndex + 4);
  // Serial.print("query");
    #ifdef DEBUG
    // Serial.println(ReceivedCMDS);
    #endif
  if (ReceivedBCMD.equals(BCMDString))
  {
//     Serial.print("BCMD");
    #ifdef DEBUG
     Serial.println("Valid BCMD");
    #endif
    return 1;
  }
  else if (ReceivedCMDS.equals(QueryString))
  {
    #ifdef DEBUG
      //Serial.println("Query START");  
    #endif
    return 3;
  }
  else
    {
      #ifdef DEBUG
        Serial.println("Invalid START");  
      #endif
     return 0;
    }
}
/******************************************************************************************/
//-----------------------------------------------------------------------------------------
//Function to check valid LampID 
//Return 1 if Valid Lamp ID

/*___________________________________________________________________________________________________
      int CheckQueryLAMPID();
____________________________________________________________________________________________________*/
/*
 *  Function Name   : int CheckQueryLAMPID();
 *  Author          : 
 *  Company         : Elcome Integrated Systems Pvt. Ltd.
 *  Date            : 17/10/2023
 *  Pass Value      : xxx
 *  Return Value    : xxx
 *  Effected Var    : xxx
 *    Register      : xxx
 *    Global Var    : xxx
 *  Stack use       : xxxByte
 *  Code Size       : xxxByte
 *  Machine  Cycle  : xxxMC Minimum
 *  Other function  :
 *  req.   
 *  function Process: Function is used to check the Lamp ID in the query
 *  Remarks         :
 */
 
int CheckQueryLAMPID()
{
  uint8_t ui8IdOk = 0;
  String ReceivedID ;
  ReceivedID = inputString.substring((StartIndex + 5), StartIndex + 7);
  #ifdef DEBUG
    Serial.print("ReceivedID =");  Serial.println(ReceivedID);
    Serial.print("LampId =");   Serial.println(ui8LampID);
  #endif
  int ui8ReceivedID = ReceivedID.toInt();
  if (ui8ReceivedID == ui8LampID)
  {
    ui8IdOk = 1;
  }
  return ui8IdOk;
}
/******************************************************************************************/
//-----------------------------------------------------------------------------------------
//Function to check Group if in $BCMD Command

/*___________________________________________________________________________________________________
      uint8_t CheckGroupID();
____________________________________________________________________________________________________*/
/*
 *  Function Name   : uint8_t CheckGroupID();
 *  Author          : 
 *  Company         : Elcome Integrated Systems Pvt. Ltd.
 *  Date            : 17/10/2023
 *  Pass Value      : xxx
 *  Return Value    : xxx
 *  Effected Var    : xxx
 *    Register      : xxx
 *    Global Var    : xxx
 *  Stack use       : xxxByte
 *  Code Size       : xxxByte
 *  Machine  Cycle  : xxxMC Minimum
 *  Other function  :
 *  req.   
 *  function Process: Function is used to check if the Group ID mentioned in the command matches with specified Lamp ID
 *  Remarks         :
 */
 
uint8_t CheckGroupID()
{
  uint8_t ui8GrpChkOk = 0;
  String GroupID = inputString.substring((StartIndex + 8), (StartIndex + 9));
  if (GroupID == StrGroupIDNo)
  {
    #ifdef Debug
    Serial.print(GroupID);
    Serial.println( " GroupID Matches");
    #endif
    ui8GrpChkOk = 1;
  }
 if (ui8LampID == CENTER_LIGHT)
  {
    if((GroupID == "B") ||(GroupID == "C") ||(GroupID == "N"))
    {
       ui8GrpChkOk = 1;
    }
  }
 return ui8GrpChkOk;
}

//-----------------------------------------------------------------------------------------
//Set the Brightness For  Group of Lights 

/*___________________________________________________________________________________________________
      void CheckBrightnessBCMD();
____________________________________________________________________________________________________*/
/*
 *  Function Name   : void CheckBrightnessBCMD();
 *  Author          : 
 *  Company         : Elcome Integrated Systems Pvt. Ltd.
 *  Date            : 17/10/2023
 *  Pass Value      : xxx
 *  Return Value    : xxx
 *  Effected Var    : xxx
 *    Register      : xxx
 *    Global Var    : xxx
 *  Stack use       : xxxByte
 *  Code Size       : xxxByte
 *  Machine  Cycle  : xxxMC Minimum
 *  Other function  :
 *  req.   
 *  function Process: Function is used to check the brightness requested in BCMD command
 *  Remarks         :
 */
void CheckBrightnessBCMD()
{
   ReceivedBrightnessBCMD = inputString.substring((StartIndex + 12), StartIndex + 14);
   ui8brightnessBCMD = ReceivedBrightnessBCMD.toInt();

  //String ReceivedPwrBCmd = inputString.substring((StartIndex + 6), StartIndex + 7);
  //int PwrBCMD = ReceivedPwrBCmd.toInt();
  //  Serial.print(",");
  //  Serial.print(ReceivedBrightnessBCMD);

  #ifdef DEBUG
    Serial.print("ReceivedBrightnessBCMD=");
    Serial.println(ReceivedBrightnessBCMD);
  #endif
/*
  if (ui8LampID == 1) //Amber Light
  {
     uiMaxBrightness = 99;
     uiMinBrighness = 0;
  }
  else if (ui8LampID == 2) //Green Light
  {
     uiMaxBrightness = 75;
     uiMinBrighness = 0;
  }*/
  int s = map(ui8brightnessBCMD, uiMinBrighness, uiMaxBrightness, 0, 255);
  ui8brightnessBCMD = s;
  analogWrite(LampPower, s); //PWM out at pin no 13
}

//-----------------------------------------------------------------------------------------
//Function To Turn ON or OFF the Group OF Lights

/*___________________________________________________________________________________________________
      void CheckLampPowerBCMD();
____________________________________________________________________________________________________*/
/*
 *  Function Name   : void CheckLampPowerBCMD();
 *  Author          : 
 *  Company         : Elcome Integrated Systems Pvt. Ltd.
 *  Date            : 17/10/2023
 *  Pass Value      : xxx
 *  Return Value    : xxx
 *  Effected Var    : xxx
 *    Register      : xxx
 *    Global Var    : xxx
 *  Stack use       : xxxByte
 *  Code Size       : xxxByte
 *  Machine  Cycle  : xxxMC Minimum
 *  Other function  :
 *  req.   
 *  function Process: Function is used to check the Lamp Power mentioned in BCMD command
 *  Remarks         :
 */
 
void CheckLampPowerBCMD()
{
  String ReceivedStopGoParaBCMD = inputString.substring((StartIndex + 15), StartIndex + 16);
  StopGoFlag = ReceivedStopGoParaBCMD.toInt();
  String ReceivedNVGMode = inputString.substring((StartIndex + 10), StartIndex + 11);
  int NVGModeBCMD = ReceivedNVGMode.toInt();
  ReceivedPwrBCmd = inputString.substring((StartIndex + 6), StartIndex + 7);
  PwrBCMD = ReceivedPwrBCmd.toInt();
  pilotInfoFlag = inputString.substring((StartIndex + 17), (StartIndex + 18)); //PID Type Info From $BCMD COMMAND
  
  #ifdef DEBUG
    Serial.print("ReceivedPwrBCmd=");
    Serial.println(ReceivedPwrBCmd);
    Serial.print("PwrBCMD=");
    Serial.println(PwrBCMD);
    Serial.print("ReceivedNVGMode=");
    Serial.println(ReceivedNVGMode);
    Serial.print("NVGMode in int=");
    Serial.println(NVGModeBCMD);
    Serial.println();
    Serial.print("PID TYPE = "); Serial.println(pilotInfoFlag);
    Serial.print("EEPROM DATA = "); Serial.println(readEEPROMPIDType);
  #endif

  if(NVGModeBCMD == 0)  // Normal Mode
  {
    if((ui8LampID == GO_LIGHT1 || ui8LampID == GO_LIGHT2) && StopGoFlag == 0) //Go Light
    {
      PwrBCMD = 0;    // Turn OFF Light
    }
    if ((ui8LampID == STOP_LIGHT1 || ui8LampID == STOP_LIGHT2) && StopGoFlag == 1) //Stop Light
    {
      PwrBCMD = 0;    // Turn OFF Light
    }
    if(ui8LampID == PID_LIGHT)  
    { 
      pidAction(); // Call to piAction() Function
    }
      NVGLightBlinking = 0;
      Serial.println("Disable Timer");
      TIMSK1 = 0;
    if((ui8LampID == WAIVE_OFF)||(ui8LampID == HOMIMG_BEACON1) ||(ui8LampID == HOMIMG_BEACON2))
    {
        Serial.println("Enable Timer");
        TIMSK1 = (1 << OCIE1A);
     }
    
  }
  else    // NVG Mode
  {
      if( ui8LampID == STOP_LIGHT1 || ui8LampID == STOP_LIGHT2) //Stop Light( Red color light) will be off in NVG Mode
      {
        PwrBCMD = 0;    // Turn OFF Stop Light in NVG Mode
      }
      if(ui8LampID == GO_LIGHT1 || ui8LampID == GO_LIGHT2) //GO Light (Green color light) will blink in NVG Mode
      {
        if(StopGoFlag == 0)   // if Stop Mode Selected
        { // Start Go Blinking
          NVGLightBlinking = 1;
          Serial.println("Enable Timer");
          TIMSK1 = (1 << OCIE1A);
        }
        else
        {
          NVGLightBlinking = 0;
          Serial.println("Disable Timer");
          TIMSK1 = 0;
        }
      }
      if(ui8LampID == PID_LIGHT)//PID Light action in NVG Mode 02-04-24
      {
        if(PwrBCMD == 0)
        {
          
          
        }
        else
        {

          PwrBCMD = 0;
          Serial.println("PID NVG Mode");
          if(pilotInfoFlag == readEEPROMPIDType) //Comparing the EEPROM DATA and DATA FROM COMMAND
          {
            if(pilotInfoFlag == "A")//SAFE TO LAND
            {
              Serial.println("pilotInfoFlag in Safe To Land");
              PwrBCMD = 1;
              NVGLightBlinking = 0;
              Serial.println("Disable Timer");
              TIMSK1 = 0;
            }        
            else if(pilotInfoFlag == "B")//CAUTION
            {
              Serial.println("pilotInfoFlag in Caution");
              PwrBCMD = 1;
              NVGLightBlinking = 0;
              Serial.println("Disable Timer");
              TIMSK1 = 0;
            }
            else if(pilotInfoFlag == "C")//UNSAFE
            {
              Serial.println("pilotInfoFlag in UNSAFE");
              PwrBCMD = 0;
            }
          }
          if(readEEPROMPIDType == "A")
          {
            if(pilotInfoFlag == "C")//UNSAFE
             {
              PwrBCMD = 1;
              NVGLightBlinking = 1;
              Serial.println("Enable Timer");
              TIMSK1 = (1 << OCIE1A);
             }
          }
        }   
      }
  }

  if(PwrBCMD == 1) 
  {
    LedONAction(); //If PwrBCMD Bit is  1 TURN ON Lights
  }
  else   
  {
    LedOFFAction();//OR ELSE TURN OFF THE Lights 
  }
}

//-----------------------------------------------------------------------------------------
//Turn OFF The Light OR Group Of Lights

/*___________________________________________________________________________________________________
      void LedOFFAction(void);
____________________________________________________________________________________________________*/
/*
 *  Function Name   : void LedOFFAction(void);
 *  Author          : 
 *  Company         : Elcome Integrated Systems Pvt. Ltd.
 *  Date            : 17/10/2023
 *  Pass Value      : xxx
 *  Return Value    : xxx
 *  Effected Var    : xxx
 *    Register      : xxx
 *    Global Var    : xxx
 *  Stack use       : xxxByte
 *  Code Size       : xxxByte
 *  Machine  Cycle  : xxxMC Minimum
 *  Other function  :
 *  req.   
 *  function Process: Function is used to turn OFF the lights
 *  Remarks         :
 */
 
void LedOFFAction(void)
{
  Serial.println("LED OFF");
  digitalWrite(LightEnable, LOW);
  analogWrite(LampPower, LOW); //PWM out at pin no3 //take action of NVG OFF
  digitalWrite(LampPowerLED, LOW);
  digitalWrite(LampNVG, 0);
  LampOn = 0;
  NVGMode = 0;
  BlinkON = 0;
}
//-----------------------------------------------------------------------------------------
//Turn On The Light OR Group Of Lights

/*___________________________________________________________________________________________________
      void LedONAction(void);
____________________________________________________________________________________________________*/
/*
 *  Function Name   : void LedONAction(void);
 *  Author          : 
 *  Company         : Elcome Integrated Systems Pvt. Ltd.
 *  Date            : 17/10/2023
 *  Pass Value      : xxx
 *  Return Value    : xxx
 *  Effected Var    : xxx
 *    Register      : xxx
 *    Global Var    : xxx
 *  Stack use       : xxxByte
 *  Code Size       : xxxByte
 *  Machine  Cycle  : xxxMC Minimum
 *  Other function  :
 *  req.   
 *  function Process: Function is used to turn ON the lights
 *  Remarks         :
 */
void LedONAction(void)
{
  digitalWrite(LightEnable, HIGH);
  //analogWrite(LampPower, HIGH);   // turn the LED on (HIGH is the voltage level)
  analogWrite(LampPower, ui8brightnessBCMD);
  digitalWrite(LampPowerLED, HIGH);
  LampOn = 1;
  BlinkON = 1;
  Serial.print("LED ON ui8brightness =");
  Serial.println(ui8brightnessBCMD);
}
//----------------------------------------------------

/*___________________________________________________________________________________________________
      void pidAction();
____________________________________________________________________________________________________*/
/*
 *  Function Name   : void pidAction();
 *  Author          : 
 *  Company         : Elcome Integrated Systems Pvt. Ltd.
 *  Date            : 17/10/2023
 *  Pass Value      : xxx
 *  Return Value    : xxx
 *  Effected Var    : xxx
 *    Register      : xxx
 *    Global Var    : xxx
 *  Stack use       : xxxByte
 *  Code Size       : xxxByte
 *  Machine  Cycle  : xxxMC Minimum
 *  Other function  :
 *  req.   
 *  function Process: Function is used to find the PID Light related action- if it needs to switch ON or OFF
 *  Remarks         :
 */
 
void pidAction() 
{
 // String ReceivedPwrBCmd = inputString.substring((StartIndex + 6), StartIndex + 7);
 // int PwrBCMD = ReceivedPwrBCmd.toInt();
//   pilotInfoFlag = inputString.substring((StartIndex + 17), (StartIndex + 18)); //PID Type Info From $BCMD COMMAND
//    #ifdef DEBUG
//      Serial.println();
//      Serial.print("PID TYPE = "); Serial.println(pilotInfoFlag);
//      Serial.print(" EEPROM DATA "); Serial.println(readEEPROMPIDType);
//    #endif
    if(pilotInfoFlag == readEEPROMPIDType) //Comparing the EEPROM DATA and DATA FROM COMMAND
    {
      if(PwrBCMD == 1)//LedONAction(); // TURN ON THE PID LIGHT IF DATA FROM EEPROM AND $BCMD COMMAND MATCHES
      {
        Serial.println("LED ON ACTION");
        PwrBCMD = 1;
      }
      else
      {//LedOFFAction //TURN OFF THE LIGHT IF PwrBCMD =0
        PwrBCMD = 0;
        Serial.println("LED OFF ACTION");
      }
    }
    else //LedOFFAction
    {
      PwrBCMD = 0;
      Serial.println("Invalid PID TYPE"); //TURN OFF THE LIGHT IF PID TYPE is Different
    }
}
//-----------------------------------------------------------------------------------------
//Turn ON or OFF The NVG MODE for Group Of Lights

/*___________________________________________________________________________________________________
      void CheckNVGModeBCMD();
____________________________________________________________________________________________________*/
/*
 *  Function Name   : void CheckNVGModeBCMD();
 *  Author          : 
 *  Company         : Elcome Integrated Systems Pvt. Ltd.
 *  Date            : 17/10/2023
 *  Pass Value      : xxx
 *  Return Value    : xxx
 *  Effected Var    : xxx
 *    Register      : xxx
 *    Global Var    : xxx
 *  Stack use       : xxxByte
 *  Code Size       : xxxByte
 *  Machine  Cycle  : xxxMC Minimum
 *  Other function  :
 *  req.   
 *  function Process: Function is used to find the NVG mode of Light related action- if it needs to switch ON or OFF
 *  Remarks         :
 */
 
void CheckNVGModeBCMD()
{
  //  String ReceivedNVGModeI = inputString.substring((StartIndex+11),StartIndex+12);
  //  int   NVGModeI = ReceivedNVGModeI.toInt();
  String ReceivedNVGMode = inputString.substring((StartIndex + 10), StartIndex + 11);
  int NVGModeBCMD = ReceivedNVGMode.toInt();

  #ifdef DEBUG
    Serial.print("ReceivedNVGMode=");
    Serial.println(ReceivedNVGMode);
    Serial.print("NVGMode in int=");
    Serial.println(NVGModeBCMD);
  #endif

  if (NVGModeBCMD == 1)
  {
    digitalWrite(LampNVG, 255); //PWM out at pin no
    //  Serial.println("NVG MODE ON");
    NVGMode = 1;
    if (ui8LampID == GO_LIGHT1 || ui8LampID == GO_LIGHT2) //GO Light
    {
      if(StopGoFlag == 0)//Stop Light
      {
        NVGLightBlinking = 1;
        Serial.println("Enable Timer");
        TIMSK1 = (1 << OCIE1A);
      }else
      {
         LedONAction();     
      }
      
      
    }
    if (ui8LampID == STOP_LIGHT1 || ui8LampID == STOP_LIGHT2) //STOP Light
    {
      LedOFFAction();
    }
  }
  else
  {
    digitalWrite(LampNVG, 0); //PWM out at pin no
    // Serial.println("NVG MODE OFF");
    NVGMode = 0;
    if(ui8LampID == GO_LIGHT1 || ui8LampID == GO_LIGHT2)
    {
      NVGLightBlinking = 0;
      Serial.println("Disable Timer");
      TIMSK1 = 0;
    }

  }
}

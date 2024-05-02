/*
    Author        : Vaishali Khandave
    Company       : Elcome Integrated Systems Pvt. Ltd.
    File Name     : MainLightDriver.ino
    Date          : 16 Oct 2023
    File Version  : MainLightDriver9
    Other file    : None
    required

    Tools Used
    Arduino IDE   : Arduino V1.8.16
    Complier      : Arduino V1.8.16
    Assembler     : Arduino Vx.x.xx
    Linker        : Arduino Vx.x.xx
    Description   : This "MainLightDriver.ino" file contains setup and loop routine.
*/

/*=======================================================================================================================*/
/* These are the commands send between CPDC to PCM and PCM to Light Driver Card. Feedback/ response is collected 
 * from the particular lamp
   1) Broadcast Command $BCMD:This is sent from CPDC to individual PCM. Dataframe structure for same is mentioned below 
      $BCMD,PwrCmd,GrpID,NvgCmd,Intensity,StopGo,PilotInfo,Termination,Checksum
      Details of each databyte in dataframe is mentioned below: 
        Data byte_0: $BCMD- $- Start bit character, BCMD: Broadcast Command    
        Data byte_1: PwrCmd:0  Lights OFF 
                     PwrCmd:1  Lights ON  
        Data byte_2: GrpID: Select the Group of Lights from A/B/C/D/E/F/G/H/I/J/K/L/M/N
        Data byte_3: NvgCmd:0  NVG Mode OFF 
                     NvgCmd:1  NVG Mode ON  
        Data byte_4: Intensity:0 to 99 (in terms of percentage) 
        Data byte_5: StopGo:0  Stop Indication ON
                     StopGo:1  Go Indication ON 
        Data byte_6: PilotInfo:A  PID shows Green Light i.e Deck- Safe to land the Helicopter
                     PilotInfo:B  PID shows Amber Light i.e Deck- Cautionally land the Helicopter
                     PilotInfo:C  PID shows Red Light i.e Deck- Unsafe to land the Helicopter
        Data byte_7: *Checksum- *-Command End bit character, Checksum-
    Eg. $BCMD,1,A,1,10,0,A,*0a - For Lights in Group A
        $BCMD,1,B,0,10,0,A,*AA - For Lights in Group B
        $BCMD,1,J,0,10,0,A,*0a - For Lights in Group J
        $BCMD,1,M,0,10,0,A,*0a - For Lights in Group M
        $BCMD,1,I,1,10,0,A,*0a - For Lights in Group I
        $BCMD,1,I,0,10,0,A,*0a - For Lights in Group M
        
   2) Status Query Command $STQ:This is sent from PCM to connected individual lights in the group. Dataframe structure for
      same is mentioned below 
      $STQ,<Light ID decimal value>,*Checksum
      Data byte_0: $STQ- $- Start bit character, STQ: Status Query Command
      Data byte_1: Light ID: Select the Light ID from 
                              01 to 16 for Group A Lights/
                              17 to 24 for Group B Lights/
                              25 to 31 for Group C Lights/
                              32 to 48 for Group D Lights/
                              51 to 54 for Group E Lights/
                              55 to 58 for Group F Lights/
                              59 to 60 for Group G Lights/
                              61 to 63 for Group H Lights/
                              64 for Group I Light/
                              65 to 66 for Group J Lights/
                              67 to 68 for Group K Lights/
                              49, 50 and 69,70 for Group L Lights/
                              71 for Group M Light/
                              72 to 78 for Group N Lights 
      Data byte_2: *Checksum- *-Command End bit character, Checksum-
      Eg. $STQ,01,*0A

   3) Status Response Command $STR:This is sent from particular Light to connected PCM. Dataframe structure for same is 
      mentioned below    
      $STR,<Light ID decimal value>,OFF/ON (0/1), NVG OFF/ON (0/1), FAILURE (0/1),*Checksum
      Data byte_0: $STR- $- Start bit character, STR: Status Response Command
      Data byte_1: Light ID: Select the Light ID from details mention in description above.
      Data byte_2: OFF/ON : 0- Selected Light is OFF
                            1- Selected Light is ON
      Data byte_3: NvgCmd:0  NVG Mode OFF for Light
                   NvgCmd:1  NVG Mode ON for Light
      Data byte_4: FAILURE: 0- Selected Light not failed
                            1- Selected Light has failed
      Data byte_5: *Checksum- *-Command End bit character, Checksum-
      Eg. $STR,01,0,0,0,*AA
*/

/*===========================================================================================================================*/
//_______________________________________Project Details____________________________________________
/*
    Description       : This Project is used for switching ON and OFF of lights of Marine Ship Deck.
                        This code helps in selection of lamp from the given group.
                        UART Protocol used for Communication with PCM.
    Hardware Version  :
    Controller        : Atmel make ATMEGA2560-16AU
    Crystal Frequency : Internal Oscillator (16 MHz crystal oscillator )
    Debug Tool        :
    Programming  Tool :
    Configuration     :
    Setting
*/
#include <EEPROM.h>
#include "SystemConfig.h"
#define GO_LIGHT1 69                        // Defined decimal value of 69 to Go Light1- Repeated declaration
#define STOP_LIGHT1 70                      // Defined decimal value of 70 to Stop Light1- Repeated declaration
#define GO_LIGHT2 49                        // Defined decimal value of 49 to Go Light2- Repeated declaration
#define STOP_LIGHT2 50                      // Defined decimal value of 50 to Stop Light2- Repeated declaration
#define PID_LIGHT  64                       // Defined decimal value of 64 to LPD or PID Light- Repeated declaration
#define CENTER_LIGHT 19                     // Defined decimal value of 19 to Common or Center Light- Repeated declaration
                           
#define MASTER 1                            // Defined decimal value of 1 to Master
#define SLAVE 0                             // Defined decimal value of 0 to Slave
#define MSG_START_FLAG 0x24                 // Defined hex value of 24 to MSG_START_FLAG
#define SERIAL_TX_BUFFER_SIZE 17;           // Defined Serial Transmitter Buffer Size to 17
#define WAIVE_OFF 71                        // Defined Light ID 71 to Waive Off Light
#define HOMIMG_BEACON1 65                   // Defined Light ID 71 to Waive Off Light
#define HOMIMG_BEACON2 66                   // Defined Light ID 71 to Waive Off Light
/*=============================================================================================================*/

/*Port Pin assignment of Atmega 2560*/
int pincount = 8;                           // Variable to indicate the number of pins used in ATMEGA2560
int pins[] = {74, 9, 8, 7, 6, 73, 16, 17};  // Refer Arduino ATmega2560 Pin map(Mega core, do not refer AVR; pins used are
                                            // Pin No. 74(PH7),9(PH6),8(PH5),7(PH4),6(PH3),73(PH2),16(PH1),17(PH0) of MegaCore 
int LightEnable = 85; //PJ7                 // Pin 85(PJ7) is used for signal LightEnable
int LampPower = 10;                         // Pin 10(PB4) is used for signal LampPower
int LampPowerLED = 42;                      // Pin 42(PL7) is used for signal LampPowerLED
int LampNVG = 43;                           // Pin 43(PL6) is used for signal LampNVG
int Direction422 = 47;                      // Pin 47(PL2) is used for signal Direction422
int Error = A0;                             // Pin 54/A0(PF0) is used for signal Error
int SlaveMonitorPL5 = 44;//PL5 output B     // Pin 44(PL5) is used for signal SlaveMonitorPL5
int ErrorSlavePL4 = 45;//PL4 input A        // Pin 45(PL4) is used for signal ErrorSlavePL4
int ErrorSlavePL3 = 46;//PL3 input C        // Pin 46(PL3) is used for signal ErrorSlavePL3

/*=============================================================================================================*/
String pilotInfoFlag = "";                      // Var to get received PID type set cmd from Touch Panel 02-04-24
int NVGMode = 0;                            // Variable declared 0 to indicate NVGMode OFF
int StopGoFlag = 0;                         // Variable declared 0 to indicate StopGoFlag OFF
char CmdTYPE = 0, IDOK = 0, CheckSumOK = 0; // Variables declared 0 to indicate CmdTYPE is Wrong Command, Lamp ID does not match, 
                                            // Checksum does not match
char GroupOK = 0;                           // Variables declared 0 to indicate Group ID does not match 
int CardType;                               // Variable for identifying Master or Slave Card
String inputString = "";                    // Variable to hold string entered
bool stringComplete = false;                // Boolean variable to check if the string is complete or not. Declared as false.
int ui8MsgLen;                              // Variable is used to store the lenght of response received from lamp  
int StartIndex, LampOn = 0;                 // Variables declared to store start character '$', and other variable indicates Lamp OFF
String strLampID;                           // Variable is used to store the Lamp ID which responded with $STR 
int ui8LightError = 0;                      // Variable is declared to indicate the Light Error is not present
int ui8RecvChecksum = 0;                    // Variable is declared to indicate the CheckSum is received
int ui8ChkSumCount = 0;                     // Variable is declared to indicate the CheckSum count is 0
volatile int count;                         // Variable is used in Interrupt Serive Routine to vary count

/*=============================================================================================================*/

//DIP Switch Parameters
String decimalNumber, stringBit;            // Variables are used to store the converted decimal value, bit value of each DIP Switch 
String stringBinary, StrGroupIDNo;          // Variables are used to store the binary value shown by DIP Switch, Group ID received 
                                            // in $STR
long binaryNumber;                          // Variable is used to store the binary number by converting string to integer
int ui8LampID, bitVal;                      // Variable is used to store LampID, bit value of each DIP Switch (ui8: unsigned integer 8 bit data)
int ui8PIDType;                             // Variable is used to store 3 types of PID Light (ui8: unsigned integer 8 bit data)

/*=============================================================================================================*/

//Serial Event Parameters
int Receivedcount, msgStarted;               // Variable is used to indicate recived message count and starting of the message

/*=============================================================================================================*/
//EEPROM Parameters
int eeAddress, eeAddress1, eeAddress2;       // Variable is used to store PID Lamp details 
int eeAddress3, eeAddress4;                  // Variable is used to store PID Lamp details 
String readEEPROMPIDType;                    // Variable is used to store read PID Lamp type in EEPROM
int cReadInputPID;                           // Variable is used to store read input for PID Lamp
unsigned long WaitStart;                     // Variable is used to store time value in milliseconds since the program has started

int8_t ui8GroupLightBlinking = 0;            // Variable is declared to indicate Group Lights are not blinking
String stringOne ;                           // Variable is used to store the Lamp ID received during $STR response
String LightOnOFF ;                          // Variable is used to store the status of Lamp if ON or OFF
String NVGOnOFF;                             // Variable is used to store the NVG Mode of Lamp if ON or OFF
String LightError;                           // Variable is used to store the status of Lamp Error if ON or OFF
String cmd, ChecksumstringOne;               // Variables are used to store the $STR command response, Checksum string
int NVGLightBlinking = 0;                    // Variable is declared to indicate that Light in NVG Mode blinking OFF

unsigned int reload = 0x5024;                // 92 flashes 

//Variables for Brightness percentage details
uint8_t uiMaxBrightness = 99;                // Variable is used to store the maximum brightness percentage 99%
uint8_t uiMinBrighness = 0;                  // Variable is used to store the minimum brightness percentage 0%
uint8_t ui8brightness = 0;                   // Variable is initiated to indicate the present brightness of Light/ Lamp =0
String ReceivedBrightnessBCMD;               // Variable is used to store the brightness value received from $BCMD command string
uint8_t ui8brightnessBCMD;                   // Variable is used to store the string to integer converted brightness value in $BCMD 
String ReceivedPwrBCmd;                      // Variable is used to store the power flag status received from $BCMD command string
int PwrBCMD;                                 // Variable is used to store string to integer converted power flag status value in $BCMD 
int BlinkON = 0;                             // Variable is initiated to indicate the blinking of Light/ Lamp is OFF
bool bSwitchStatus = 0;                      // Variable is initiated to 0 for all the DIP switches from 7 to 0
/*=============================================================================================================*/

#define DEBUG 1                            // Defined Debug value of 1 to debug and program the LED Driver Card using UART0
/*=============================================================================================================*/
void InitGPIO();                             // Function to initialize GPIO of ATMEGA 2560

int CheckStartString();                      // Function to verify the start of instruction and verify if $BCMD or $STQ

void CheckLampPowerBCMD();                   // Function to check the lamp power in BCMD command
uint8_t CheckGroupID();                      // Function to check the Group of Lamp
void CheckNVGModeBCMD();                     // Function to check the NVG mode in BCMD command
void CheckBrightnessBCMD();                  // Function to check the brightness in BCMD command
void GetEEPROMPidData(void);                 // Function to receive data from EEPROM
void GetReadPIDData (void);                   // Function to read the PID Data and ask user if any change needed
void pidAction();                            // Function to execute the PID Light Indication Action

void ReadMasterSlaveSwitch(void);            // Function to read the Pin 8 DIP switch settings to identify Master or Slave Card
bool ReadDIPSwitch(void);                    // Function to read the DIP switch settings to identify the Lamp ID and Lamp Group ID
long convertBinaryToDecimal(long binary);    // Function to convert binary data to decimal for Lamp ID

void Init_Timer(void);                       // Function to initialize the timer
void BlinkLED(void);                         // Function to blink LED

int CheckQueryLAMPID();                      // Function to check the received Lamp ID in query and check with the Lamp ID data
void responseTOQuery();                      // Function to send response from particular light to the connected PCM
void ResetParameters();                      // Function to reset the parameters

void flash();                                // Function to turn ON or OFF the blinking of LED
void LedONAction(void);                      // Function to turn ON LED
void LedOFFAction(void);                     // Function to turn OFF LED

void serialEvent1();                         // Function to communicate serially between PCM and LED Driver using UART1

/******************************************************************************************/
/******************************************************************************************/
/*__________________________________________________________________________________________________
                                        void setup()
  Setting up of the parameters for serial communication
  ____________________________________________________________________________________________________*/

void setup()
{
  InitGPIO();                     // Called the InitGPIO function
  /*Defined value of EEPROM addresses*/
  eeAddress = 0;
  eeAddress1 = 1;
  eeAddress2 = 2;
  eeAddress3 = 3;
  eeAddress4 = 4;

  /*Starting Serial Communication with baud rate of 9600 with std config of 1 start bit, 8 data bits, 1 stop bit*/
  Serial.begin(9600, SERIAL_8N1);
  Serial1.begin(9600);            // Starting Serial Communication with baud rate of 9600 
  inputString.reserve(200);       // Reserving buffer space of 200 bytes for string manipulation
  Serial.println("Code start ");  // Printing the text "Code Start" on new line
  CardType = MASTER;              // Declaring the LED Driver card default as Master 
  bSwitchStatus = ReadDIPSwitch();// Store the read binary value based on the individual settings starting from 7 to 0 of DIP Switch
                                  // (in reverse order). This will give details of Lamp ID

  /*Timer Settings*/
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  OCR1A = reload;
  TCCR1B = (1 << WGM12) | (1 << CS12);
  sei();
  if (ui8GroupLightBlinking == 1)
  {
    TIMSK1 = (1 << OCIE1A);
  }
  //Init_Timer();
  bSwitchStatus = 1;                // DIP Switch on LED Driver card is set to 1
}

/*__________________________________________________________________________________________________
                                        void loop()
  Execution of the all the functions required for the Main Light Driver Card
  ____________________________________________________________________________________________________*/

void loop()
{
  if (bSwitchStatus == 1)                 // Check if DIP Switch status is ON
  {
    CmdTYPE = 0;                          // Assigned Cmd type as 0- Wrong Command
    if (ui8LightError == 1)               // Check if ui8LightError signal is turned ON then if yes
    {
      //Serial.println("Error SET");      
      BlinkLED();                         // LED4 (same used for LampNVG signal) is blinked ON and OFF to indicate the error     
      digitalWrite(SlaveMonitorPL5, LOW); // LED6 Output is switched to Low for Error indication
    }                                     // Command- if(ui8LightError == 1) completed
    if (stringComplete)                   // Check if stringComplete signal is received then if yes
    {
      #ifdef DEBUG1                        // Check if DEBUG1 signal is defined then if yes
      Serial.println(inputString);         // Printing the string stored in inputString variable on new line 
      #endif                               // End the process to check full/ complete string 
      if (inputString != "")               // Check if inputString Signal is not received then if not received  
      {
        //digitalWrite(Status, HIGH);      //status led
        #ifdef DEBUG                        // Check if DEBUG signal is defined then if yes
        Serial.print("Received Count=");    // Printing the text "Received Count"
        Serial.println(Receivedcount);      // Printing the string stored in Receivedcount variable on new line
        #endif                              // End the process of debug and print string count
        CmdTYPE = CheckStartString();       // Now call function CheckStartString() and store in the variable CmdTYPE ($BCMD or $STQ)
        // CheckCheckSum();
        CheckSumOK = 1;                     // Assigning 1 to CheckSumOK to indicate checksum matches
        if (CheckSumOK)                     // If CheckSumOK is 1 then
        {
          CheckSumOK = 0;                    // 0 is assigned to CheckSumOK variable
          switch (CmdTYPE)                   // With help of switch cases exceute the concerned commands for particular command type
          {
            case 0:                          // For case 0
            {
              #ifdef DEBUG                   // If DEBUG is defined then
              Serial.println("Wrong Command"); // Printing the text "Wrong Command" on new line  
              #endif                           // End the process of debug and printing
            }          
            break;                           // break the case in switch command
            case 1://$BCMD                   // For case 1
            {
              GroupOK = CheckGroupID();      // Assigning the returned value of function CheckGroupID() to variable GroupOK 
              if (GroupOK == 1)              // If GroupOK is 1 then
              {
                #ifdef DEBUG                 // Check if DEBUG signal is defined then if yes 
                Serial.println("Group ID Matches");// Printing the text "Group ID Matches" on new line  
                #endif                       // End the process of debug 
                CheckBrightnessBCMD();       // Call function CheckBrightnessBCMD() for specified brightness 
                CheckLampPowerBCMD();        // Call function CheckLampPowerBCMD() to switch ON or switch OFF lamp 
                // CheckNVGModeBCMD();
              }                              // Command- if (GroupOK==1) completed
            }                                // Command- case1 completed
            break;                           // break the case in switch command
            case 3://Query command $STQ      // For case 3
            {
              #ifdef DEBUG                   // Check if DEBUG signal is defined then if yes
              //Serial.println("Check Lamp ID");
              #endif                         // End the process of debug 
              IDOK = CheckQueryLAMPID();     // Assigning the returned value of function CheckQueryLAMPID() to variable IDOK 
              if (IDOK == 1)                 // If IDOK is 1 then
              {
                #ifdef DEBUG                 // Check if DEBUG signal is defined then if yes
                Serial.println("ID OK");     // Printing the text "ID OK" on new line 
                #endif                       // End the process of debug and printing 
                if (CardType != SLAVE)       // Check if Card is Master
                {
                  responseTOQuery();//STR    // Send response to Query by calling function responseTOQuery() 
                }                            // Command- if (CardType != SLAVE)completed
              }                              // Command- if (IDOK==1)completed
            }
            break;                           // break the case in switch command
            }                                // Command- switch (CmdTYPE) completed
          }                                  // Command- if (CheckSumOK) completed
        }                                    // Command- if (inputString != " ") completed
       else                                  //else command for if (inputString != " ") 
       {
          #ifdef DEBUG                       // Check if DEBUG signal is defined then if yes
          Serial.println("Checksum Error");  // Printing the text "Checksum Error" on new line
          #endif                             // End the process of debug and printing
       }                                     // Command- else completed
       ResetParameters();                    // Calling function ResetParameters() to clear the parameters set 
    }                                        // Command- if(StringComplete)completed
   //CheckErrorSwitch();
  }                                          // Command- if(bSwitchStatus == 1)completed 
  else
  {
    Serial.println("Wrong DIP switch settings");// Printing the text "Wrong DIP switch settings" on new line
  }                                          // Command- else for if(bSwitchStatus == 1)completed 
}                                            // Commnd- void loop() completed   

/******************************************************************************************/
/******************************************************************************************/
//Initializing the GPIOs for Master Slave Communication
//For Serial Communication

/*___________________________________________________________________________________________________
      void InitGPIO(void);
  ____________________________________________________________________________________________________*/
/*
    Function Name   : void InitGPIO(void);
    Author          :
    Company         : Elcome Integrated Systems Pvt. Ltd.
    Date            : 17/10/2023
    Pass Value      : xxx
    Return Value    : xxx
    Effected Var    : xxx
      Register      : xxx
      Global Var    : xxx
    Stack use       : xxxByte
    Code Size       : xxxByte
    Machine  Cycle  : xxxMC Minimum
    Other function  :
    req.
    function Process: Initiate the inputs and outputs
    Remarks         :
*/

void InitGPIO(void)
{
  pinMode(Direction422, OUTPUT);             //Initialize pin 47(PL2)as output named as DirectionRS422 
  pinMode(LightEnable, OUTPUT);              //Initialize pin 85(PJ7)as output named as LightEnable             
  pinMode(LampPower, OUTPUT);                //Initialize pin 10(PB4)as output named as LampPower
  pinMode(LampPowerLED, OUTPUT);             //Initialize pin 42(PL7)as output named as LampPowerLED
  pinMode(LampNVG, OUTPUT);                  //Initialize pin 43(PL6)as output named as LampNVG
  pinMode(Error, INPUT);                     //Initialize pin A0(PF0/54)as input named as Error
  pinMode(SlaveMonitorPL5, OUTPUT);          //Initialize pin 44(PL5)as output named as SlaveMonitorPL5
  pinMode(ErrorSlavePL4, INPUT);             //Initialize pin 45(PL4)as input named as ErrorSlavePL4
  pinMode(ErrorSlavePL3, INPUT);             //Initialize pin 46(PL3)as input named as ErrorSlavePL3
  
  for (int  x = 0 ; x == pincount  ; x++)    //Using for loop to initialize 8 pins (74,09,08,07,06,73,16,17) as input
  {                                          //    
    pinMode(pins[x], INPUT);                 //Initialize each pin (mentioned in above comment) as input 
  }
  digitalWrite(Direction422, LOW);           // Direction control for 422,always in receive mode
  digitalWrite(LightEnable, LOW);            // Light Enable signal is set low
  analogWrite(LampPower, 0);                 // POWER LED set OFF
  digitalWrite(LampPowerLED, LOW);           // LampPowerLED signal or pin is set low
  digitalWrite(LampNVG, LOW);                // NVG LED OFF
}

/******************************************************************************************/
//RESET THE PARAMETERS OF RS422 COMMAND

/*___________________________________________________________________________________________________
      void ResetParameters();
  ____________________________________________________________________________________________________*/
/*
    Function Name   : void ResetParameters();
    Author          :
    Company         : Elcome Integrated Systems Pvt. Ltd.
    Date            : 17/10/2023
    Pass Value      : xxx
    Return Value    : xxx
    Effected Var    : xxx
      Register      : xxx
      Global Var    : xxx
    Stack use       : xxxByte
    Code Size       : xxxByte
    Machine  Cycle  : xxxMC Minimum
    Other function  :
    req.
    function Process: Clear the Parameters entered through string
    Remarks         :
*/
void ResetParameters()
{
  inputString = "";                         // Clear the string
  stringComplete = false;                   // stringComplete varible is set as false
  Serial.flush();                           // Waits  for the transmission of outgoing serial data to complete. 
  delay(5);                                 // Delay of 5ms
  Receivedcount = 0;                        // Received count is set 0
}

/******************************************************************************************/
//FUNCTION TO SEND THE $STR FROM LIGHT DRIVER

/*___________________________________________________________________________________________________
      void responseTOQuery();
  ____________________________________________________________________________________________________*/
/*
    Function Name   : void responseTOQuery();
    Author          :
    Company         : Elcome Integrated Systems Pvt. Ltd.
    Date            : 17/10/2023
    Pass Value      : xxx
    Return Value    : xxx
    Effected Var    : xxx
      Register      : xxx
      Global Var    : xxx
    Stack use       : xxxByte
    Code Size       : xxxByte
    Machine  Cycle  : xxxMC Minimum
    Other function  :
    req.
    function Process: Respond to the query raised by Status response dataframe
    Remarks         :
*/

void responseTOQuery()
{
  // String stringone = ui8LampID
  strLampID = decimalNumber;                // Lamp ID is stored in the variable strLampID
  stringOne = strLampID;                    // strLampID is stored in variable stringOne
  LightOnOFF = String(LampOn);              // LightOnOFF stores the string format of LampOn
  NVGOnOFF = String(NVGMode);               // NVGOnOFF stores the string format of NVGMode
  LightError = String(ui8LightError);       // LightError stores the string format of ui8LightError
  //  cmd =LampId + "0"+","+"0"+","+"0"+",";
  //  cmd = "0"+","+"0"+","+"0"+",";
  if (strLampID.length() == 1)              // Check if the string length of strLampID is 1, 
  {
    cmd += "0" + strLampID;                 // if yes then cmd variable is stored with string 0 and strLampID
  }
  else
  {
    cmd += strLampID;                       // if not then cmd variable is stored with strLampID
  }
  cmd += ",";                               // Now add comma string to variable cmd
  cmd += LightOnOFF;                        // Next add LightOnOFF to the variable cmd
  cmd += ",";                               // Now add comma string to variable cmd
  cmd += NVGOnOFF;                          // Next add NVGOnOFF to the variable cmd
  cmd += ",";                               // Now add comma string to variable cmd
  cmd += LightError;                        // Next add LightError to the variable cmd
  cmd += ",";                               // Now add comma string to variable cmd
  cmd = "$STR," + cmd ;                     // Lastly add all the data in variable cmd , $STR to the variable cmd
  ui8MsgLen = cmd.length();
  //uint8_t icheckSum = CalculateChecksum(cmd, i8MsgLen); bypass checksum
  uint8_t icheckSum = 0;                    // assign 0 to variable icheckSum
  //ChecksumstringOne =  String(char(icheckSum));
  cmd = cmd + "*" + "A" + "A";              // Add the checksum as *AA to the variable cmd
  digitalWrite(Direction422, HIGH);         // Direction control for 422 is now transmitting the data
  delay(10);                                // Adding delay of 10ms
  Serial.println(cmd);                      // Print the data of variable cmd serially on new line, i.e response to the query 
  Serial1.println(cmd);                     // Print the data of variable cmd serially on new line, i.e response to the query 
  // wait for transmit buffer to empty
  while ((UCSR0A & _BV (TXC0)) == 0) {}     // Do nothing while (UCSR0A & _BV (TXC0) is 0
  delay(10);                                // Add delay of 10 ms
  digitalWrite(Direction422, LOW);          // Direction control for 422 as receiver
  cmd = "";                                 //reset string i.e. response to query
}
/******************************************************************************************/
//Function to convert Binary to Decimal Returns Long

/*___________________________________________________________________________________________________
      long convertBinaryToDecimal(long binary);
  ____________________________________________________________________________________________________*/
/*
    Function Name   : long convertBinaryToDecimal(long binary);
    Author          :
    Company         : Elcome Integrated Systems Pvt. Ltd.
    Date            : 17/10/2023
    Pass Value      : binary
    Return Value    : decimalVal
    Effected Var    : xxx
      Register      : xxx
      Global Var    : xxx
    Stack use       : xxxByte
    Code Size       : xxxByte
    Machine  Cycle  : xxxMC Minimum
    Other function  :
    req.
    function Process: Convert the data in binary to decimal
    Remarks         :
*/

long convertBinaryToDecimal(long binary)
{
  long number = binary;                   // storing varible number with value in variable binary 
  long decimalVal = 0;                    // storing 0 to decimalVal
  long baseVal = 1;                       // storing 1 to baseVal
  long tempVal = number;                  // Transferring value in variable number to variable tempVal
  long previousDigit;                     // Defined variable previousDigit

  // Converts Binary to Decimal
  while (tempVal)                         // while tempVal is true
  {
    previousDigit = tempVal % 10;         // Modulus of tempVal/10 is stored in variable previousDigit
    tempVal = tempVal / 10;               // Quotient of tempVal/10 is stored in tempVal again
    decimalVal += previousDigit * baseVal;// decimalVal variable stores addition of decimalVal and product of previousDigit, baseVal
    baseVal = baseVal * 2;                // baseVal variable now stores baseVal*2
  }
  // Serial.println("decimalVal:");
  //Serial.print(decimalVal);
  return decimalVal;                      //  Returns the Decimal number
}
/******************************************************************************************/
/******************************************************************************************/
/*__________________________________________________________________________________________________
                                        bool ReadDIPSwtich()
  ____________________________________________________________________________________________________
  /*
    Function Name   : bool ReadDIPSwtich();
    Author          :
    Company         : Elcome Integrated Systems Pvt. Ltd.
    Date            : 17/10/2023
    Pass Value      : xxx
    Return Value    : xxx
    Effected Var    : xxx
      Register      : xxx
      Global Var    : xxx
    Stack use       : xxxByte
    Code Size       : xxxByte
    Machine  Cycle  : xxxMC Minimum
    Other function  :
    req.
    function Process: The settings of DIP Switch are used to detect Lamp ID and Group ID
    Remarks         :

  This function read the settings of the DIP switch on the Square LED Driver Card Ver3.1. Please note
  the binary value should be in reverse order beginning with switch 8 to 1. Switch 8 is used to decide
  if the card will act as Master or Slave. Remaining 7 switches provide the binary value. This binary value is
  then converted to decimal value to find the LampID (from 1 to 78). The lamps are further grouped
  together as per their purpose in GroupID ('A' to 'O').

  Note: Lamp ID 19 is common or center light across Stern Approach, Oblique Approach (Port and Stbd)

  DIP SWITCH SETTING for Lamp ID
  SW8      |   SW7  SW6  SW5  SW4  SW3  SW2  SW1
  0=Master |Ex. 0    0    0    1    0    0    0   =  (08)in decimal
  1=Slave  |Ex. 1    1    1    1    0    1    1   =  (63)in decimal

  Lamps are divided in  groups and assigned Group ID
  1> LampID 01 to 16 fall under GroupID 'A'(Deck Periphery Light)
  2> LampID 17 to 24 fall under GroupID 'B'(Stern Approach Light)
  3> LampID 25 to 31 fall under GroupID 'C'(Oblique Approach Port Light)
  4> LampID 32 to 48 fall under GroupID 'D'(SHRS/Hangar Reference Light)
  5> LampID 51 to 54 fall under GroupID 'E'(Service Flood  Light)
  6> LampID 55 to 58 fall under GroupID 'F'(Deck Surface Illuminition Light)
  7> LampID 59 to 60 fall under GroupID 'G'(Hangar Wash Down Light)
  8> LampID 61 to 63 fall under GroupID 'H'(HIFR  Light)
  9> LampID 64 fall under GroupID 'I'(LPD)
  10> LampID 65 to 66 fall under GroupID 'J'(Homing Beacon Light)
  11> LampID 67 to 68 fall under GroupID 'K'(Obstruction  Light)
  12> LampID 49,50,69,70 fall under GroupID 'L'(Stop/Go Light)
  13> LampID 71 fall under GroupID 'M' (Waive off Light)
  14> LampID 72 to 78 fall under GroupID 'N'(Oblique Approach Stbd Light)
  ____________________________________________________________________________________________________*/
//Funcion to Read the Lamp ID and Assign The Group ID Accordingly
bool ReadDIPSwitch(void)                  // A boolean function ReadDIPSwitch is created to  read the Lamp ID set in binary 
{
  bool bDIPSWitchFlag = 1;                // Initially, bDIPSwitchFlag is set to 1

  ReadMasterSlaveSwitch();                //DIP SWITCH 8 decides whether LED Driver will respond as Master or Slave
                                          //If High(1):- The LED Driver will work as Master card
                                          //If Low (0):- The LED Driver will work as Slave,card will not respond               
 
  int ReadBitInt;                         // Declared integer variable ReadBitInt
  stringBinary = "";                      // stringBinary is kept blank
  for (int x = 1 ; x < pincount ; x++)    // using for loop to read pins of ATMEGA2560
  {
    ReadBitInt = digitalRead(pins[x]);    // The digital values read of pins are stored in variable ReadBitInt
    if (ReadBitInt == 0)                  // check if ReadBitInt is 0
    {
      bitVal = 1;                         // Assign 1 to variable bitVal 
    }
    else if (ReadBitInt == 1)             // else check if ReadBitInt is 1
    {
      bitVal = 0;                         // Assign 0 to variable bitVal
    }
    // bitVal = ~ReadBitInt;
    stringBit = String(bitVal);           // The string value of bitVal is stored in stringBit
    stringBinary = stringBinary + stringBit; // stringBinary stores the sum of previous value of stringBit and sttringBit
    binaryNumber = stringBinary.toInt();  // variable binaryNumber stores the integer value of stringBinary
    //Serial.print("bit x: "); Serial.println(x);
    //Serial.print("bit value: "); Serial.println(bitVal);
    //Serial.print("stringBinary: "); Serial.println(stringBinary);
  }
  decimalNumber = convertBinaryToDecimal(binaryNumber); // decimalNumber stores decimal value of binaryNumber
  ui8LampID = convertBinaryToDecimal(binaryNumber);     // ui8LampID stores decimal value of binaryNumber
  Serial.print("Decimal: "); Serial.println(ui8LampID); // Print Decimal and the value of Lamp ID on new line
  //Resets all the variables
  binaryNumber = 0;
  bitVal = 0;
  stringBit = "";
  stringBinary = "";

  unsigned char Flag = 0;                               // Flag is set to 0

  for ( unsigned char Index = 0; Index < MAX_NO_OF_HVLAS_LED; Index++) // Using for loop to check the GroupID of lamp
  {
    if (LED_ConfigArr[Index].ui8LED_ID == ui8LampID)    // check if array LED_ConfigArr element ui8LED_ID is ui8LampID
    {
      if (LED_ConfigArr[Index].ui8LED_GroupID == A)     // check if array LED_ConfigArr element ui8LED_GroupID is A 
      {
        StrGroupIDNo = "A";                             // GroupID of Lamp is A
      }
      if (LED_ConfigArr[Index].ui8LED_GroupID == B)     // check if array LED_ConfigArr element ui8LED_GroupID is B
      {
        StrGroupIDNo = "B";                             // GroupID of Lamp is B
      }
      if (LED_ConfigArr[Index].ui8LED_GroupID == C)     // check if array LED_ConfigArr element ui8LED_GroupID is C
      {
        StrGroupIDNo = "C";                             // GroupID of Lamp is C
      }
      if (LED_ConfigArr[Index].ui8LED_GroupID == D)     // check if array LED_ConfigArr element ui8LED_GroupID is D 
      {
        StrGroupIDNo = "D";                             // GroupID of Lamp is D
      }
      if (LED_ConfigArr[Index].ui8LED_GroupID == E)     // check if array LED_ConfigArr element ui8LED_GroupID is E 
      {
        StrGroupIDNo = "E";                             // GroupID of Lamp is E
      }
      if (LED_ConfigArr[Index].ui8LED_GroupID == F)     // check if array LED_ConfigArr element ui8LED_GroupID is F 
      {
        StrGroupIDNo = "F";                             // GroupID of Lamp is F
      }
      if (LED_ConfigArr[Index].ui8LED_GroupID == G)     // check if array LED_ConfigArr element ui8LED_GroupID is G 
      {
        StrGroupIDNo = "G";                             // GroupID of Lamp is G
      }
      if (LED_ConfigArr[Index].ui8LED_GroupID == H)     // check if array LED_ConfigArr element ui8LED_GroupID is H 
      {
        StrGroupIDNo = "H";                             // GroupID of Lamp is H
      }
      if (LED_ConfigArr[Index].ui8LED_GroupID == I)     // check if array LED_ConfigArr element ui8LED_GroupID is I 
      {
        StrGroupIDNo = "I";                             // GroupID of Lamp is I
        GetReadPIDData();                               // Calling function GetReadPIDData
        ui8GroupLightBlinking = 1;  
      }
      if (LED_ConfigArr[Index].ui8LED_GroupID == J)     // check if array LED_ConfigArr element ui8LED_GroupID is J 
      {
        StrGroupIDNo = "J";                             // GroupID of Lamp is J
        ui8GroupLightBlinking = 1;                      
      }
      if (LED_ConfigArr[Index].ui8LED_GroupID == K)     // check if array LED_ConfigArr element ui8LED_GroupID is K 
      {
        StrGroupIDNo = "K";                             // GroupID of Lamp is K
      }
      if (LED_ConfigArr[Index].ui8LED_GroupID == L)     // check if array LED_ConfigArr element ui8LED_GroupID is L 
      {
        StrGroupIDNo = "L";                             // GroupID of Lamp is L
      }
      if (LED_ConfigArr[Index].ui8LED_GroupID == M)     // check if array LED_ConfigArr element ui8LED_GroupID is M
      {
        StrGroupIDNo = "M";                             // GroupID of Lamp is M
         ui8GroupLightBlinking = 1;
      }
      if (LED_ConfigArr[Index].ui8LED_GroupID == N)     // check if array LED_ConfigArr element ui8LED_GroupID is N 
      {
        StrGroupIDNo = "N";                             // GroupID of Lamp is N
      }
      if (LED_ConfigArr[Index].ui8LED_GroupID == O)     // check if array LED_ConfigArr element ui8LED_GroupID is O
      {
        StrGroupIDNo = "O";                             // GroupID of Lamp is O
      }
      Flag++;
    }
  }

  if (Flag != 1)
  {
    Serial.println("Wrong ID");
    bDIPSWitchFlag = 0;
  }
  Serial.print("StrGroupID: ");
  Serial.println(StrGroupIDNo);


  //  if (ui8LampID < 17)
  //  {
  //    StrGroupIDNo = 'A'; //1,16
  //  }
  //  else if ((ui8LampID > 16) & (ui8LampID < 25))
  //  {
  //    StrGroupIDNo = 'B'; //17,24
  //  }
  //  else if ((ui8LampID > 24) & (ui8LampID < 32))
  //  {
  //    StrGroupIDNo = 'C'; //25,31
  //  }
  //  else if ((ui8LampID > 31) & (ui8LampID < 51))
  //  {
  //    StrGroupIDNo = 'D'; //32,50
  //  }
  //  else if ((ui8LampID > 50) & (ui8LampID < 55))
  //  {
  //    StrGroupIDNo = 'E'; //51,54
  //  }
  //  else if ((ui8LampID > 54) & (ui8LampID < 59))
  //  {
  //    StrGroupIDNo = 'F'; //55,58
  //  }
  //  else if ((ui8LampID > 58) & (ui8LampID < 61))
  //  {
  //    StrGroupIDNo = 'G'; //59,60
  //  }
  //  else if ((ui8LampID > 60) & (ui8LampID < 64))
  //  {
  //    StrGroupIDNo = 'H'; //61,63
  //  }
  //  else if (ui8LampID == 64)
  //  {
  //    StrGroupIDNo = 'I'; //64
  //    GetReadPIDData();
  //  }
  //  else if ((ui8LampID > 64) & (ui8LampID < 67))
  //  {
  //    StrGroupIDNo = 'J'; //65,66
  //    ui8GroupLightBlinking = 1;
  //  }
  //  else if ((ui8LampID > 66) & (ui8LampID < 69))
  //  {
  //    StrGroupIDNo = 'K'; //67,68
  //  }
  //  else if ((ui8LampID > 68) & (ui8LampID < 71) ||())
  //  {
  //    StrGroupIDNo = 'L'; //69,70
  //  }
  //  else if (ui8LampID == 71)
  //  {
  //    StrGroupIDNo = 'M'; //71
  //    ui8GroupLightBlinking = 1;
  //  }
  //  else if ((ui8LampID > 71) & (ui8LampID < 79))
  //  {
  //    StrGroupIDNo = 'N'; //72,78
  //  }
  //  else
  //  {
  //    Serial.println("Wrong ID");
  //    bDIPSWitchFlag = 0;
  //  }
  Serial.print("StrGroupIDNo = "); Serial.println(StrGroupIDNo);
  return bDIPSWitchFlag;
}

//---------------------------------------------------------------------------------------
//Function for User to Set PID Type For EACH PID Type
//MASTER GREEN LIGHT A 1
//SLAVE1 AMBER LIGHT B 2
//SLAVE2 RED LIGHT   C 3

/*___________________________________________________________________________________________________
      void GetEEPROMPidData(void);
  ____________________________________________________________________________________________________*/
/*
    Function Name   : void GetEEPROMPidData(void);
    Author          :
    Company         : Elcome Integrated Systems Pvt. Ltd.
    Date            : 17/10/2023
    Pass Value      : xxx
    Return Value    : xxx
    Effected Var    : xxx
      Register      : xxx
      Global Var    : xxx
    Stack use       : xxxByte
    Code Size       : xxxByte
    Machine  Cycle  : xxxMC Minimum
    Other function  :
    req.
    function Process: Collect (Fetch) the data of PID or LPD Light from EEPROM
    Remarks         :
*/

void GetEEPROMPidData(void)
{
  Serial.println("***********************************************************************");
  Serial.print("Set PID Type "); //Serial.println(fMaxCurrValue);
  Serial.println();

  Serial.println("Select Action 1/2/3/4");
  Serial.println("1)Green Deck(A)");
  Serial.println("2)Caution Deck(B)");
  Serial.println("3)Unsafe Deck(C)");
  Serial.println("4)Exit");
  WaitStart = millis();
  // while (Serial.available() > 0)

  while (Serial.available () == 0)
  {
    if ((millis() - WaitStart) > 20000)
      break;
  }
  Serial.print("Enter PID Type Input  ");
  cReadInputPID = Serial.parseInt();
  Serial.println(cReadInputPID);
  switch (cReadInputPID)
  {
    case 1 :
      ui8PIDType = 1;
      break;

    case 2 :
      ui8PIDType = 2;
      break;

    case 3 :
      ui8PIDType = 3;
      break;

    default:
      {
        ui8PIDType = EEPROM.read(eeAddress);
        EEPROM.update(eeAddress, ui8PIDType);
        ui8PIDType = EEPROM.read(eeAddress1);
        EEPROM.update(eeAddress1, ui8PIDType);
        ui8PIDType = EEPROM.read(eeAddress2);
        EEPROM.update(eeAddress2, ui8PIDType);
        Serial.println("EXIT");
      }
  }
  EEPROM.update(eeAddress, ui8PIDType);
  EEPROM.update(eeAddress1, ui8PIDType);
  EEPROM.update(eeAddress2, ui8PIDType);
}

/******************************************************************************************/
//Function to Decode Card is Master or Slave

/*___________________________________________________________________________________________________
      void ReadMasterSlaveSwitch(void);
  ____________________________________________________________________________________________________*/
/*
    Function Name   : void ReadMasterSlaveSwitch(void);
    Author          :
    Company         : Elcome Integrated Systems Pvt. Ltd.
    Date            : 17/10/2023
    Pass Value      : xxx
    Return Value    : xxx
    Effected Var    : xxx
      Register      : xxx
      Global Var    : xxx
    Stack use       : xxxByte
    Code Size       : xxxByte
    Machine  Cycle  : xxxMC Minimum
    Other function  :
    req.
    function Process: Function is used to read the position of switch 8 of DIP Switch
    Remarks         :
*/

void ReadMasterSlaveSwitch(void)
{
  bitVal = digitalRead(pins[0]);
  stringBit = String(bitVal);
  stringBinary = stringBinary + stringBit;
  binaryNumber = stringBinary.toInt();
  decimalNumber = convertBinaryToDecimal(binaryNumber);
  stringBinary = "";
  if (binaryNumber == 0)
  {
    CardType = SLAVE;
#ifdef DEBUG
    Serial.println("CardType:= SLAVE");
#endif
    Serial.println("CardType:= SLAVE");
  }
  else
  {
    CardType = MASTER;
#ifdef DEBUG
    //Serial.println("CardType:= MASTER");
#endif
    Serial.println("CardType:= MASTER");
  }
}
//===================================================

/*___________________________________________________________________________________________________
      void GetReadPIDData(void);
  ____________________________________________________________________________________________________*/
/*
    Function Name   : void GetReadPIDData(void);
    Author          :
    Company         : Elcome Integrated Systems Pvt. Ltd.
    Date            : 17/10/2023
    Pass Value      : xxx
    Return Value    : xxx
    Effected Var    : xxx
      Register      : xxx
      Global Var    : xxx
    Stack use       : xxxByte
    Code Size       : xxxByte
    Machine  Cycle  : xxxMC Minimum
    Other function  :
    req.
    function Process: Function is used to read the data of PID or LPD switch and print if deck is safe or unsafe
    Remarks         :
*/

void GetReadPIDData(void)
{
  //Reading The EEPROM DATA FROM PID LIGHT DRIVERs
  Serial.println("---------EEPROM Read------------");     // Print on new line ---------EEPROM Read------------
  ui8PIDType = EEPROM.read(eeAddress);                    // Transfer the value at eeAddress to variable ui8PIDType
  Serial.print("DATA ="); Serial.println(ui8PIDType);     // Print on new line DATA= value stored in ui8PIDType
  ui8PIDType = EEPROM.read(eeAddress1);                   // Transfer the value at eeAddress1 to variable ui8PIDType
  Serial.print("DATA ="); Serial.println(ui8PIDType);     // Print on new line DATA= value stored in ui8PIDType
  ui8PIDType = EEPROM.read(eeAddress2);                   // Transfer the value at eeAddress2 to variable ui8PIDType
  Serial.print("DATA ="); Serial.println(ui8PIDType);     // Print on new line DATA= value stored in ui8PIDType
  //Setting PID Type Accordingly
  if (ui8PIDType == 1)                                    // If value ui8PIDType is 1
  {
    readEEPROMPIDType = "A"; //GREEN DECK(GREEN LIGHT)    // Transfer the string A to variable readEEPROMIDType 
    Serial.println(" PID Type is Green Deck");            // Print on new line PID Type is Green Deck
  }
  else if (ui8PIDType == 2)                               // If value ui8PIDType is 2
  {
    readEEPROMPIDType = "B"; //CAUTION DECK(AMBER LIGHT)  // Transfer the string B to variable readEEPROMIDType
    Serial.println(" PID Type is Caution/Amber Deck");    // Print on new line PID Type is Caution/Amber Deck
  }
  else if (ui8PIDType == 3)                               // If value ui8PIDType is 3
  {
    readEEPROMPIDType = "C";//UNSAFE DECK(RED LIGHT)      // Transfer the string C to variable readEEPROMIDType
    Serial.println(" PID Type is Unsafe/Red Deck");       // Print on new line PID Type is Unsafe/Red Deck
  }
  cReadInputPID = ui8PIDType;                             // Transfer ui8PIDType variable value to variable cReadInputPID
  //Get EEPROM Data
  Serial.println("Do you wish to change?");               // Print on new line Do you wish to change?
  GetEEPROMPidData();//ASKING USER TO INPUT PID TYPE      // Calling function GetEEPROMPidData
  delay(500);                                             // Delay of 500 ms
}
//==============================================

/*___________________________________________________________________________________________________
      void Init_Timer(void);
  ____________________________________________________________________________________________________*/
/*
    Function Name   : void Init_Timer(void);
    Author          :
    Company         : Elcome Integrated Systems Pvt. Ltd.
    Date            : 17/10/2023
    Pass Value      : xxx
    Return Value    : xxx
    Effected Var    : xxx
      Register      : xxx
      Global Var    : xxx
    Stack use       : xxxByte
    Code Size       : xxxByte
    Machine  Cycle  : xxxMC Minimum
    Other function  :
    req.
    function Process: Function is used to initiate the timer
    Remarks         :
*/

void Init_Timer(void)                     // function Init_Timer is defined
{
  /*  Serial.println("Timer set start ");
    //----------Timer Settings---------//
    cli();
    TCCR1A = 0;
    TCCR1B = 0;
    OCR1A = reload;
    TCCR1B = (1 << WGM12) | (1 << CS12);
    sei();
    if (ui8GroupLightBlinking == 1)
    {
     Serial.println("Enable Timer");
     TIMSK1 = (1 << OCIE1A);
    }
    Serial.println("Timer set End ");*/
}
//==============================================
ISR(TIMER1_COMPA_vect)//27-09-2023        // Interrupt Service Routine is declared
{
  count++;                                // Count incremented
  flash();                                // function flash is declared
}
/******************************************************************************************/
//FUNCTION TO TURN ON & OFF BLINKING LIGHTS
/*___________________________________________________________________________________________________
      void flash();
  ____________________________________________________________________________________________________*/
/*
    Function Name   : void flash();
    Author          :
    Company         : Elcome Integrated Systems Pvt. Ltd.
    Date            : 17/10/2023
    Pass Value      : xxx
    Return Value    : xxx
    Effected Var    : xxx
      Register      : xxx
      Global Var    : xxx
    Stack use       : xxxByte
    Code Size       : xxxByte
    Machine  Cycle  : xxxMC Minimum
    Other function  :
    req.
    function Process: Function is used to turn Binking of Lamp ON & OFF
    Remarks         :
*/
void flash()//27-09-2023             // function flash is declared 
{
  if (LampOn == 1)                   // check if variable LampOn is 1
  {
    if (BlinkON == 0)                // check if variable BlinkOn is 0
    {
      analogWrite(LampPower, 1);     // TURN OFF THE BLINKING LIGHT (Reverse Logic)
      BlinkON = 1;                   // Assign 1 to variable BlinkOn
#ifdef DEBUG                         // Check if DEBUG signal is defined then if yes 
      Serial.println("BlinkON OFF"); // Printing the text "BlinkOn OFF"
#endif
    }
    else if (BlinkON == 1)           // check if variable LampOn is 1
    {
      analogWrite(LampPower, ui8brightnessBCMD);  //TURN ON BLINKING LIGHT AT BRIGHTNESS MENTIONED IN % IN BCMD (Reverse Logic)
      BlinkON = 0;                  // Assign 0 to variable BlinkOn
#ifdef DEBUG                        // Check if DEBUG signal is defined then if yes
      Serial.println("BlinkON ON"); // Printing the text "BlinkOn ON"
#endif
    }
  }
}

/******************************************************************************************/
/*___________________________________________________________________________________________________
      void BlinkLED(void);
  ____________________________________________________________________________________________________*/
/*
    Function Name   : void BlinkLED(void);
    Author          :
    Company         : Elcome Integrated Systems Pvt. Ltd.
    Date            : 17/10/2023
    Pass Value      : xxx
    Return Value    : xxx
    Effected Var    : xxx
      Register      : xxx
      Global Var    : xxx
    Stack use       : xxxByte
    Code Size       : xxxByte
    Machine  Cycle  : xxxMC Minimum
    Other function  :
    req.
    function Process: Function is used to initiate the timer
    Remarks         :
*/

void BlinkLED(void)
{
  digitalWrite(LampNVG, HIGH);  // turn the LED4 on by switching Pin43 (PL6) to high voltage(HIGH is the voltage level)
  delay(50);                    // delay of 50 ms
  digitalWrite(LampNVG, LOW);   // turn the LED4 off by switching Pin43 (PL6) to low voltage (LOW is the voltage level)
  delay(50);                    // delay of 50 ms
}
/******************************************************************************************/

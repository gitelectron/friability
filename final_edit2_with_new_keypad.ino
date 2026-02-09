#include <SoftwareSerial.h>
 
 //******************************************************************/
#include <EEPROM.h>
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 rtc;

#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h" 

#define pin4 4
#define pin5 5
#define pin6 6
#define pin7 7
// Library only supports hardware SPI at this time
// Connect SCLK to UNO Digital #13 (Hardware SPI clock)
// Connect MISO to UNO Digital #12 (Hardware SPI MISO)
// Connect MOSI to UNO Digital #11 (Hardware SPI MOSI)
#define RA8875_INT 3
#define RA8875_CS 10
#define RA8875_RESET 9

#define EEPROM_I2C_ADDRESS 0x50///24lc512

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
uint16_t tx, ty,txc,tyc;
int t=5,p2=0,    clr_count=0,rc=2,tc=0,maintenance_lockopen=0;
 int k=0,total_count=100,count_value[4]={1,0,0},cursorx=70,cursory=135,cursoryr=70,cursorxr=130,
   j=0,time_value[4]={0,0,4},total_time=4,page=0,i=0,i2=0,flag=0,
   select_mode=0,pass=0,
   pres=0,reenteredpass[4]={0,0,0,0},fixpassword[4]={1,2,3,4},
   y,m,d,h,mi,s,ay,am,ad,ah,ami,as,addr=0,eeprom=0,
   k1,k2,k3,k4,p,login=0,timebackflag=0,timenextflag=0,countnextflag=0,
  countbackflag=0,weightback_flag=0,weightnext_flag=0,settingsbackflag=0,settingsnextflag=0,
   NEW=0,  cursorxrpm=160,cursoryrpm=70,rpm_value[2]={2,5},total_rpm=25,rpm=0,
           nextflag=0,backflag=0,no_of_drums=2,drum_type=1,calculation=1,
           save_no_of_drums=2,save_drum_type=1,save_calculation=1,calculation_flag=0,
           home_flag=0,cursorxa=150,cursorya=55,capslock=1,wrongpass=0,
           madminbackflag=0,adminflag=0,maintainanceflag=0,
           adminback_flag=0,adminenter_flag=0,
           adminpage_flag=0,adminsettings_flag=0,adminconfig_flag=0,
           weight_unit=1,balance=2,printer=1,save_weight_unit=1,save_balance=2,save_printer=1,//1=yes,2=no
           insettingsnext_flag=0,insettingsback_flag=0,asettingsmenu2next_flag=0,asettingssetdandt_flag=0,
           asettingsinstrumentid_flag=0,asettingscompanyname_flag=0,companyback_flag=0,instrumentback_flag=0,
           sr_flag=0,rstpwd_flag=0,balpara_flag=0,printpara_flag=0,maintanancenext_flag=0,serialnonext_flag=0,
           resetpass_flag,/*adminconfig_flag=0*/configback_flag=0,adminandpass=0,adminoldback_flag=0,adminoldedit_flag=1,
           adminnewback_flag=0,adminnewname=0,adminnewpass=0,user1andpass=0,user2andpass=0,user3andpass=0,user4andpass=0,
           user5andpass=5,namebreak_flag=0,passbreak_flag=0,/*userandpasschangeback_flag=0,*/passchangesave=0,
           login_flag=0,loginshow=0,loginenter=0,loginback=0,loginnext=0,loginback2=0,loginenter2=0,rtcback_flag=0,rtctimenext_flag=0,rtctimeback_flag=0,rtcdatenext_flag=0,rtcdateback_flag=0,
           day_value[2],month_value[2],year_value[9],hour_value[7],min_value[2],sec_value[2],rtcdatesave_flag=0,rtctimesave_flag=0,rtctimejump_flag=0,rtcdatejump_flag=0,motornext_flag=0,motorstart_flag=0,motorstop_flag=0,motorstore_flag=0,
          /* weight_value[5],*//*total_weight,total_weight1,total_weight2,*/weightnext2_flag=0,weightback2_flag=0,maintenanceenter=0,maintenenceback=0,runsec=0,runhour=0,runmin=0,userentertime=2,dubrunsec=0,/*ftotal_weight2,ftotal_weight1,total_outweight1,total_outweight2,*/
           outweightnext1_flag=0,outweightback1_flag=0,outweightnext2_flag=0,outweightback2_flag=0,timeprocess=0,countprocess=0,dubcounts=0,runmilisec=0,process2=0,resultnext_flag=0,resultstore_flag=0,dubcountsc=0,c=0,productenter_flag=0,productback_flag=0,
           productname_flag=0,batchno_flag=0,lotno_flag=0,paratobeprintnext_flag=0,historyback_flag=0,historynext_flag=0,historyback2_flag=0,historyprint_flag=0,historydelete_flag=0,historypress=0,userandpasschangenext_flag=0,userandpasschangeback_flag=0,userandpasschangeenter_flag=0,
           userandpasschangenext2_flag=0,userandpasschangeback2_flag=0,userandpasschangeenter2_flag=0,n=0,x=0,caps=1,dubcapslock=1,key=1,pressenter=0,error_flag=0,a,ac=85,opto=0, encoder=0,
           historypage=0,n2=0,printd=0,printdl=0,hisweight1_value[7],hisweight2_value[7],hisweightout1_value[7],hisweightout2_value[7],processbreak=0,resume_flag=0,checkencoder,settingsclr=0,bb;
  
float percentage,percentage2;

unsigned long int total_weight,total_weight1,total_weight2,ftotal_weight2,ftotal_weight1,total_outweight1,total_outweight2, weight_value[5], percentagedub,percentagedub2;
unsigned long int checkt1,checkt2,currmilli=0,prevmilli=0,perw1,perw2,peroutw1,peroutw2,ti=0;

char defaultuser[]="admin",defaultpass[]="8655517";///maintenance
char admin[]="aadmin",adminpassword[]="1234";
char user1[]="user1",userpassword1[]="112345";
char user2[]="user2",userpassword2[]="212345";
char user3[]="user3",userpassword3[]="312345";
char user4[]="user4",userpassword4[]="412345";
char user5[]="user5",userpassword5[]="512345";

char hisprod[12],hisbatch[10],hislot[10],histime[15],hisdate[15];
char clr_button[2];

char adminuser[20],adminpass[30], newreenteredpass[20],newu1reenteredpass[20];

char save_product_name[10],save_lot_number[5],save_batch_number[10],product_name[10],show_history[40];

char defaultusercheck[20],loginuser[30],loginpass[30];//////defaultuser is maintenance///
char admincheck[20],adminpasswordcheck[10];
char user1check[20],userpassword1check[10];
char user2check[20],userpassword2check[10];
char user3check[20],userpassword3check[10];
char user4check[20],userpassword4check[10];
char user5check[20],userpassword5check[10];

char showusername[10];


char dubcounts_string[4];
char  ftotal_weight1_string[8], ftotal_weight2_string[8],per_string1[8],per_string2[8];
char runsec_string[5],runmin_string[5],runhour_string[5];
char maintainence_pass[10];
char rtc_string[20], rtc_string2[20],historytime[25];
char company_name[20],instrument_id[15],serial_no[15],save_company_name[20],save_instrument_id[15],save_serial_no[15];
char password[10],userenter[10];           
char stringset_rpm[3]="25";
char num1='1';
char num2='2';
char num3='3';
char num4='4';
  char string[11]="Enter Time";
  char string2[11]="in Minutes";
  char stringn1[2]="1";
  char stringn2[2]="2";
  char stringn3[2]="3";
  char stringn4[2]="4";
  char stringn5[2]="5";
  char stringn6[2]="6";
  char stringn7[2]="7";
  char stringn8[2]="8";
  char stringn9[2]="9";
  char stringn0[2]="0"; 
  char stringnext[8]="NEXT-->";
  char stringclear[5]="CLR";
  char stringmin[5]="min.";
  char stringcounts[13]="Enter Counts";

//Stepper myStepper(200, 4, 5, 6, 7);
/*
void writeAddress(int address, byte val)
{
  Wire.beginTransmission(EEPROM_I2C_ADDRESS);
  Wire.write((int)(address >> 8));   // MSB
  Wire.write((int)(address & 0xFF)); // LSB
  
   
  Wire.write(val);
  Wire.endTransmission();

  delay(5);
}

byte readAddress(int address)
{
  byte rData = 0xFF;
  
  Wire.beginTransmission(EEPROM_I2C_ADDRESS);
  
  Wire.write((int)(address >> 8));   // MSB
  Wire.write((int)(address & 0xF9)); // LSB
  Wire.endTransmission();  


  Wire.requestFrom(EEPROM_I2C_ADDRESS, 1);  

  rData =  Wire.read();

  return rData;
}
*/
void keypad()
{
  
 // tft.textSetCursor(60, 300);
tft.textMode();
 /* //vertical line///
  tft.drawFastVLine(280,00,272,RA8875_WHITE);
  tft.drawFastVLine(340,00,272,RA8875_WHITE);
  tft.drawFastVLine(400,00,200,RA8875_WHITE);

  //Horizontal line///
  tft.drawFastHLine(280,60,300,RA8875_WHITE);
  tft.drawFastHLine(280,130,300,RA8875_WHITE);
  tft.drawFastHLine(280,200,300,RA8875_WHITE);
*/
//tft.drawFastHLine(280,60,300,RA8875_WHITE);

 /////////////////////tft.drawRect(0, 144, 480, 272,0X3186);//0x032c,0X0A0F,0X3041
//tft.fillRoundRect(260, 10, 200, 245,20,0X9410 );
//upper //
////////////////////tft.fillRect(0, 0, 480, 11 , 0X0A0F);

//lower
//tft.fillRect(0,260, 480, 12 , 0X0A0F);

  //vertical line///
  //tft.drawFastVLine(280,00,272,RA8875_WHITE);
//  tft.drawFastVLine(340,02,257,RA8875_WHITE);
 // tft.drawFastVLine(400,00,200,RA8875_WHITE);

  //Horizontal line///
  //tft.drawFastHLine(0,12,480,RA8875_WHITE);
  //tft.drawFastHLine(0,260,480,RA8875_WHITE);
 // tft.drawFastHLine(280,200,180,RA8875_WHITE);
////dial pad//////
/*tft.fillRoundRect(10, 180, 40, 20,3 ,0x0a0f);//0XC618
 // tft.textSetCursor(55,170 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("1");*/

  
///number 1//////
  tft.fillRoundRect(9, 80, 35, 35,4,0x0a0f);//0XC618
  tft.textSetCursor(20,87); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("1");
  
 ///number 2//////
 // tft.fillRoundRect(355, 04, 50, 58, 10, RA8875_RED);
  tft.fillRoundRect(55, 80, 35, 35,4, 0x0a0f);
  tft.textSetCursor(66, 87); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("2");

  ///number 3//////
  //tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
  tft.fillRoundRect(101, 80, 35, 35,4, 0x0a0f);
  tft.textSetCursor(112, 87); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("3");

  ///number 4//////
 // tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
  tft.fillRoundRect(147, 80, 35, 35,4, 0x0a0f);
  tft.textSetCursor(158, 87); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("4");

  ///number 5//////
 // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(193, 80, 35, 35,4, 0x0a0f); 
  tft.textSetCursor(204, 87); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("5");

  ///number 6//////
 // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
//  tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
tft.fillRoundRect(239,80, 35, 35,4, 0x0a0f);
  tft.textSetCursor(250, 87); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("6");

  ///number 7//////
 // tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(285, 80, 35, 35,4, 0x0a0f);
  tft.textSetCursor(294, 87); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("7");

  ///number 8//////
 // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(331, 80, 35, 35,4, 0x0a0f);
  tft.textSetCursor(342, 87); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("8");

  ///number 9//////
  //tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
  //tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
  tft.fillRoundRect(377, 80, 35, 35,4, 0x0a0f);
  tft.textSetCursor(388, 87); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("9");

  ///number 0//////
  //tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(423, 80, 35, 35,4, 0x0a0f);
  tft.textSetCursor(434, 87); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("0");
 
  

  
// abcd.....//
//if(caps==1 && capslock==1)
//{
tft.fillRoundRect(20,130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(31,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("q");

  tft.fillRoundRect(66, 130, 35, 35,3 ,0x0a0f);//0XC618
 tft.textSetCursor(77,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("w");

  tft.fillRoundRect(112, 130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(123,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("e");

  tft.fillRoundRect(158, 130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(169,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("r");

  tft.fillRoundRect(204, 130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(215,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("t");

  tft.fillRoundRect(250, 130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(261,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("y");

  tft.fillRoundRect(296, 130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(307,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("u");

  tft.fillRoundRect(342, 130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(353,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("i");

  tft.fillRoundRect(388, 130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(399,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("o");

  tft.fillRoundRect(434, 130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(445,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("p");

  tft.fillRoundRect(9, 180, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(20,187); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("a");
  
 
  tft.fillRoundRect(55, 180, 35, 35,3, 0x0a0f);
  tft.textSetCursor(66, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("s");

  
  tft.fillRoundRect(101, 180, 35, 35,3, 0x0a0f);
  tft.textSetCursor(112, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("d");

  
 // tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
  tft.fillRoundRect(147, 180, 35, 35,3, 0x0a0f);
  tft.textSetCursor(158, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("f");

  
 // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(193, 180, 35, 35,3, 0x0a0f); 
  tft.textSetCursor(204, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("g");

  
 // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
//  tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
tft.fillRoundRect(239, 180, 35, 35,3, 0x0a0f);
  tft.textSetCursor(250, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("h");

  
 // tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(285, 180, 35, 35,3, 0x0a0f);
  tft.textSetCursor(296, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("j");

  
 // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(331, 180, 35, 35,3, 0x0a0f);
  tft.textSetCursor(342, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("k");

  
  //tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
  //tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
  tft.fillRoundRect(377, 180, 35, 35,3, 0x0a0f);
  tft.textSetCursor(388, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("l");


  //tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(423, 180, 35, 35,3, 0x0a0f);
  tft.textSetCursor(434, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("z");
 
  tft.fillRoundRect(9, 230, 45, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(24,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Dlt");

  tft.fillRoundRect(66, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(77,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("x");

  tft.fillRoundRect(112, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(123,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("c");

  tft.fillRoundRect(158, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(169,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("v");

  tft.fillRoundRect(204, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(206,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("spc");

  tft.fillRoundRect(250, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(261,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("b");

  tft.fillRoundRect(296, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(307,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("n");

  tft.fillRoundRect(342, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(353,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("m");

 tft.fillRoundRect(388, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(390,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("caps");

tft.fillRoundRect(432, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(433,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Entr");

//tft.graphicsMode();
/*
///ABCD...///
  if(caps==2 && capslock==1)
  {
//tft.textMode();
    
tft.fillRoundRect(20, 178, 36, 20,3 ,0x0a0f);//0XC618
  tft.textSetCursor(31,182 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Q");

  tft.fillRoundRect(66, 178, 36, 20,3 ,0x0a0f);//0XC618
 tft.textSetCursor(77,182 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("W");

  tft.fillRoundRect(112, 178, 36, 20,3 ,0x0a0f);//0XC618
  tft.textSetCursor(123,182 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("E");

  tft.fillRoundRect(158, 178, 36, 20,3 ,0x0a0f);//0XC618
  tft.textSetCursor(169,182 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("R");

  tft.fillRoundRect(204, 178, 36, 20,3 ,0x0a0f);//0XC618
  tft.textSetCursor(215,182 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("T");

  tft.fillRoundRect(250, 178, 36, 20,3 ,0x0a0f);//0XC618
  tft.textSetCursor(261,182 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Y");

  tft.fillRoundRect(296, 178, 36, 20,3 ,0x0a0f);//0XC618
  tft.textSetCursor(307,182 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("U");

  tft.fillRoundRect(342, 178, 36, 20,3 ,0x0a0f);//0XC618
  tft.textSetCursor(353,182 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("I");

  tft.fillRoundRect(388, 178, 36, 20,3 ,0x0a0f);//0XC618
  tft.textSetCursor(399,182 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("O");

  tft.fillRoundRect(434, 178, 36, 20,3 ,0x0a0f);//0XC618
  tft.textSetCursor(445,182 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("P");

  tft.fillRoundRect(9, 210, 36, 20,3 ,0x0a0f);//0XC618
  tft.textSetCursor(20,214 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("A");
  
 
  tft.fillRoundRect(55, 210, 36, 20,3, 0x0a0f);
  tft.textSetCursor(66, 214); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("S");

  
  tft.fillRoundRect(101, 210, 36, 20,3, 0x0a0f);
  tft.textSetCursor(112, 214); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("D");

  
 // tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
  tft.fillRoundRect(147, 210, 36, 20,3, 0x0a0f);
  tft.textSetCursor(158, 214); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("F");

  
 // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(193, 210, 36, 20,3, 0x0a0f); 
  tft.textSetCursor(204, 214); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("G");

  
 // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
//  tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
tft.fillRoundRect(239, 210, 36, 20,3, 0x0a0f);
  tft.textSetCursor(250, 214); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("H");

  
 // tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(285, 210, 36, 20,3, 0x0a0f);
  tft.textSetCursor(296, 214); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("J");

  
 // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(331, 210, 36, 20,3, 0x0a0f);
  tft.textSetCursor(342, 214); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("K");

  
  //tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
  //tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
  tft.fillRoundRect(377, 210, 36, 20,3, 0x0a0f);
  tft.textSetCursor(388, 214); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("L");


  //tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(423, 210, 36, 20,3, 0x0a0f);
  tft.textSetCursor(434, 214); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Z");
 
  tft.fillRoundRect(20, 240, 36, 20,3 ,0x0a0f);//0XC618
  tft.textSetCursor(31,244 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("X");

  tft.fillRoundRect(66, 240, 36, 20,3 ,0x0a0f);//0XC618
  tft.textSetCursor(77,244 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("C");

  tft.fillRoundRect(112, 240, 36, 20,3 ,0x0a0f);//0XC618
  tft.textSetCursor(123,244 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("V");

  tft.fillRoundRect(158, 240, 36, 20,3 ,0x0a0f);//0XC618
  tft.textSetCursor(169,244 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("B");

  tft.fillRoundRect(204, 240, 80, 20,3 ,0x0a0f);//0XC618
  tft.textSetCursor(220,244 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("space");

  tft.fillRoundRect(296, 240, 36, 20,3 ,0x0a0f);//0XC618
  tft.textSetCursor(307,244 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("N");

  tft.fillRoundRect(342, 240, 36, 20,3 ,0x0a0f);//0XC618
  tft.textSetCursor(353,244 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("M");

//capslock=0;
//caps=1;
 // tft.graphicsMode();
  }*/

//tft.textMode();
  //SIDE BAR////

  tft.fillRoundRect(9, 20, 45, 30,4 ,0x0a0f);//0XC618
  tft.textSetCursor(20,24 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("<--");

//tft.fillRoundRect(416,11,54,130,6,RA8875_WHITE);

  tft.fillRoundRect(420, 20, 45, 30,4 ,0x0a0f);//0XC618
  tft.textSetCursor(431,24 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("-->");

  /*tft.fillRoundRect(420, 60, 45, 30,3 ,0x0a0f);//0XC618
  tft.textSetCursor(424,68 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("CLR");

  tft.fillRoundRect(420, 100, 45, 30,3 ,0x0a0f);//0XC618
  tft.textSetCursor(424,108 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("ENTER");*/

   tft.graphicsMode();
  
}
void keypad2()
{
  if(capslock==1)
  {
 // tft.textSetCursor(60, 300);
tft.textMode();
 /* //vertical line///
  tft.drawFastVLine(280,00,272,RA8875_WHITE);
  tft.drawFastVLine(340,00,272,RA8875_WHITE);
  tft.drawFastVLine(400,00,200,RA8875_WHITE);

  //Horizontal line///
  tft.drawFastHLine(280,60,300,RA8875_WHITE);
  tft.drawFastHLine(280,130,300,RA8875_WHITE);
  tft.drawFastHLine(280,200,300,RA8875_WHITE);
*/
/*
//tft.drawFastHLine(280,60,300,RA8875_WHITE);

 tft.drawRect(0, 144, 480, 272,0X3186);//0x032c,0X0A0F,0X3041
//tft.fillRoundRect(260, 10, 200, 245,20,0X9410 );
//upper //
tft.fillRect(0, 0, 480, 11 , 0X0A0F);

//lower
//tft.fillRect(0,260, 480, 12 , 0X0A0F);

  //vertical line///
  //tft.drawFastVLine(280,00,272,RA8875_WHITE);
//  tft.drawFastVLine(340,02,257,RA8875_WHITE);
 // tft.drawFastVLine(400,00,200,RA8875_WHITE);

  //Horizontal line///
  //tft.drawFastHLine(0,12,480,RA8875_WHITE);
  //tft.drawFastHLine(0,260,480,RA8875_WHITE);
 // tft.drawFastHLine(280,200,180,RA8875_WHITE);
////dial pad//////
/*tft.fillRoundRect(10, 180, 40, 20,3 ,0x0a0f);//0XC618
 // tft.textSetCursor(55,170 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("1");*/

  
///number 1//////
  tft.fillRoundRect(9, 80, 35, 35,4,0x0a0f);//0XC618
  tft.textSetCursor(20,87); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("1");
  
 ///number 2//////
 // tft.fillRoundRect(355, 04, 50, 58, 10, RA8875_RED);
  tft.fillRoundRect(55, 80, 35, 35,4, 0x0a0f);
  tft.textSetCursor(66, 87); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("2");

  ///number 3//////
  //tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
  tft.fillRoundRect(101, 80, 35, 35,4, 0x0a0f);
  tft.textSetCursor(112, 87); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("3");

  ///number 4//////
 // tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
  tft.fillRoundRect(147, 80, 35, 35,4, 0x0a0f);
  tft.textSetCursor(158, 87); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("4");

  ///number 5//////
 // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(193, 80, 35, 35,4, 0x0a0f); 
  tft.textSetCursor(204, 87); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("5");

  ///number 6//////
 // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
//  tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
tft.fillRoundRect(239,80, 35, 35,4, 0x0a0f);
  tft.textSetCursor(250, 87); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("6");

  ///number 7//////
 // tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(285, 80, 35, 35,4, 0x0a0f);
  tft.textSetCursor(294, 87); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("7");

  ///number 8//////
 // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(331, 80, 35, 35,4, 0x0a0f);
  tft.textSetCursor(342, 87); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("8");

  ///number 9//////
  //tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
  //tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
  tft.fillRoundRect(377, 80, 35, 35,4, 0x0a0f);
  tft.textSetCursor(388, 87); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("9");

  ///number 0//////
  //tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(423, 80, 35, 35,4, 0x0a0f);
  tft.textSetCursor(434, 87); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("0");

  
  
// abcd.....//
if(caps==1 && capslock==1)
{
  key=1;
tft.fillRoundRect(20,130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(31,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("q");

  tft.fillRoundRect(66, 130, 35, 35,3 ,0x0a0f);//0XC618
 tft.textSetCursor(77,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("w");

  tft.fillRoundRect(112, 130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(123,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("e");

  tft.fillRoundRect(158, 130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(169,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("r");

  tft.fillRoundRect(204, 130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(215,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("t");

  tft.fillRoundRect(250, 130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(261,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("y");

  tft.fillRoundRect(296, 130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(307,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("u");

  tft.fillRoundRect(342, 130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(353,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("i");

  tft.fillRoundRect(388, 130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(399,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("o");

  tft.fillRoundRect(434, 130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(445,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("p");

  tft.fillRoundRect(9, 180, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(20,187); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("a");
  
 
  tft.fillRoundRect(55, 180, 35, 35,3, 0x0a0f);
  tft.textSetCursor(66, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("s");

  
  tft.fillRoundRect(101, 180, 35, 35,3, 0x0a0f);
  tft.textSetCursor(112, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("d");

  
 // tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
  tft.fillRoundRect(147, 180, 35, 35,3, 0x0a0f);
  tft.textSetCursor(158, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("f");

  
 // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(193, 180, 35, 35,3, 0x0a0f); 
  tft.textSetCursor(204, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("g");

  
 // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
//  tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
tft.fillRoundRect(239, 180, 35, 35,3, 0x0a0f);
  tft.textSetCursor(250, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("h");

  
 // tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(285, 180, 35, 35,3, 0x0a0f);
  tft.textSetCursor(296, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("j");

  
 // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(331, 180, 35, 35,3, 0x0a0f);
  tft.textSetCursor(342, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("k");

  
  //tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
  //tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
  tft.fillRoundRect(377, 180, 35, 35,3, 0x0a0f);
  tft.textSetCursor(388, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("l");


  //tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(423, 180, 35, 35,3, 0x0a0f);
  tft.textSetCursor(434, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("z");
 
  tft.fillRoundRect(9, 230, 45, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(24,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Dlt");

  tft.fillRoundRect(66, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(77,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("x");

  tft.fillRoundRect(112, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(123,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("c");

  tft.fillRoundRect(158, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(169,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("v");

  tft.fillRoundRect(204, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(206,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("spc");

  tft.fillRoundRect(250, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(261,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("b");

  tft.fillRoundRect(296, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(307,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("n");

  tft.fillRoundRect(342, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(353,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("m");

 tft.fillRoundRect(388, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(390,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("caps");

tft.fillRoundRect(432, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(433,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Entr");

capslock=0; 
caps=2;
}
  tft.fillRoundRect(388, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(390,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("caps");

//tft.graphicsMode();

///ABCD...///
  if(caps==2 && capslock==1)
  {
    key=2;
//tft.textMode();
    
tft.fillRoundRect(20,130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(31,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Q");

 tft.fillRoundRect(66, 130, 35, 35,3 ,0x0a0f);//0XC618
 tft.textSetCursor(77,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("W");

  tft.fillRoundRect(112, 130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(123,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("E");

  tft.fillRoundRect(158, 130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(169,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("R");

  tft.fillRoundRect(204, 130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(215,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("T");

  tft.fillRoundRect(250, 130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(261,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Y");

  tft.fillRoundRect(296, 130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(307,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("U");

  tft.fillRoundRect(342, 130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(353,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("I");

  tft.fillRoundRect(388, 130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(399,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("O");

   tft.fillRoundRect(434, 130, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(445,137 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("P");

  tft.fillRoundRect(9, 180, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(20,187); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("A");
  
 
  tft.fillRoundRect(55, 180, 35, 35,3, 0x0a0f);
  tft.textSetCursor(66, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("S");

  
  tft.fillRoundRect(101, 180, 35, 35,3, 0x0a0f);
  tft.textSetCursor(112, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("D");

  
 // tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
  tft.fillRoundRect(147, 180, 35, 35,3, 0x0a0f);
  tft.textSetCursor(158, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("F");

  
 // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(193, 180, 35, 35,3, 0x0a0f); 
  tft.textSetCursor(204, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("G");

  
 // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
//  tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
tft.fillRoundRect(239, 180, 35, 35,3, 0x0a0f);
  tft.textSetCursor(250, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("H");

  
 // tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(285, 180, 35, 35,3, 0x0a0f);
  tft.textSetCursor(296, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("J");

  
 // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(331, 180, 35, 35,3, 0x0a0f);
  tft.textSetCursor(342, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("K");

  
  //tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
  //tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
  tft.fillRoundRect(377, 180, 35, 35,3, 0x0a0f);
  tft.textSetCursor(388, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("L");


  //tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(423, 180, 35, 35,3, 0x0a0f);
  tft.textSetCursor(434, 187); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Z");

tft.fillRoundRect(9, 230, 45, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(24,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Dlt");
 
  tft.fillRoundRect(66, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(77,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("X");

  tft.fillRoundRect(112, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(123,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("C");

   tft.fillRoundRect(158, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(169,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("V");

  tft.fillRoundRect(250, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(261,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("B");

  tft.fillRoundRect(204, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(206,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("spc");

 tft.fillRoundRect(296, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(307,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("N");

  tft.fillRoundRect(342, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(353,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("M");

tft.fillRoundRect(388, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(390,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("caps");

tft.fillRoundRect(432, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(433,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Entr");

capslock=0;
caps=1;
 // tft.graphicsMode();
  }

//tft.textMode();
  //SIDE BAR////

  tft.fillRoundRect(9, 20, 45, 30,4 ,0x0a0f);//0XC618
  tft.textSetCursor(20,24 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("<--");

//tft.fillRoundRect(416,11,54,130,6,RA8875_WHITE);

  tft.fillRoundRect(420, 20, 45, 30,4 ,0x0a0f);//0XC618
  tft.textSetCursor(431,24 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("-->");
  
tft.fillRoundRect(9, 230, 45, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(24,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Dlt");
  
tft.fillRoundRect(432, 230, 35, 35,3 ,0x0a0f);//0XC618
  tft.textSetCursor(433,237 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Entr");
   tft.graphicsMode();
  }
}


void time_disp()
{
  tft.textMode();
  //tft.cursorBlink(32);
  
 // tft.drawRoundRect(7, 22, 240, 170, 20, 0xffff);//0x3041,0xc986
  /* Set the cursor location (in pixels) */
//tft.fillRect(464,12,30,251,RA8875_WHITE);

//tft.drawRect(7,19,241,171,RA8875_BLACK);
  
  tft.fillRoundRect(8, 30, 240, 170,10, 0Xffff);
  
  tft.fillRoundRect(8, 30, 240, 80,10,0X318C);
  tft.drawRoundRect(7,29,241,171,10,RA8875_BLACK);
  tft.textSetCursor(10, 25);
 
  /* Render some text! */
  
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1);
  tft.textWrite("Enter Time");
///// FOR string2////// 
  tft.textSetCursor(50, 50); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("in Minutes");

  ////entering reading value/////
  tft.fillRect(40, 120, 180, 60, 0x3f79);

  tft.textSetCursor(70,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
   
  tft.textSetCursor(85,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(100,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
 // tft.fillRect(50, 100, 180, 60, RA8875_WHITE);
 // tft.fillCircle(140,160,50,0X3186);
 // tft.textSetCursor(170, 110); ///text location////
//  tft.textTransparent(RA8875_BLACK);
 // tft.textEnlarge(1); ////text size////
   tft.textSetCursor(70,135); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("0");
   tft.textSetCursor(85,135); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("0");
   tft.textSetCursor(100,135); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("4");
 
tft.graphicsMode();
}

void counts_disp()
{

tft.textMode();
  //tft.cursorBlink(32);
  
 // tft.drawRoundRect(7, 22, 240, 170, 20, 0xffff);//0x3041,0xc986
  /* Set the cursor location (in pixels) */
//tft.fillRect(464,12,30,251,RA8875_WHITE);

//tft.drawRect(7,19,241,171,RA8875_BLACK);
  tft.textMode();
  tft.fillRoundRect(8, 30, 240, 170,10, 0Xffff);
  
  tft.fillRoundRect(8, 30, 240, 80,10,0X318C);
  tft.drawRoundRect(7,29,241,171,10,RA8875_BLACK);
  tft.textSetCursor(10, 25);
 
  /* Render some text! */
  
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1);
  tft.textWrite("Enter Counts");
///// FOR string2////// 
 /* tft.textSetCursor(50, 50); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("in Minutes");*/

  ////entering reading value/////
  tft.fillRect(40, 120, 180, 60, 0x3f79);

  tft.textSetCursor(70,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
   
  tft.textSetCursor(85,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(100,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
 // tft.fillRect(50, 100, 180, 60, RA8875_WHITE);
 // tft.fillCircle(140,160,50,0X3186);
 // tft.textSetCursor(170, 110); ///text location////
//  tft.textTransparent(RA8875_BLACK);
 // tft.textEnlarge(1); ////text size////
  
  tft.textSetCursor(70,135); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("1");
   tft.textSetCursor(85,135); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("0");
   tft.textSetCursor(100,135); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("0");
 // tft.textMode();
 // tft.cursorBlink(32);

//  tft.fillRoundRect(8, 10, 260, 252, 20, 0X0000);
  /* Set the cursor location (in pixels) */
//  tft.textSetCursor(10, 10);
 
  /* Render some text! */
 
tft.graphicsMode();
}

void pass_disp()
{
   tft.textMode();
  tft.cursorBlink(32);
  
  /* Set the cursor location (in pixels) */
  tft.textSetCursor(10, 10);
 
  /* Render some text! */
  
  tft.textTransparent(RA8875_RED);
  tft.textEnlarge(2);
  tft.textWrite("Enter");
///// FOR string2////// 
  tft.textSetCursor(30, 50); ///text location////
  tft.textTransparent(RA8875_RED);
  tft.textEnlarge(2); ////text size////
  tft.textWrite("Password");

  ////entering reading value/////
  tft.fillRoundRect(50, 120, 210, 80, 10, RA8875_RED);
  tft.textSetCursor(155, 150); ///text location////
  tft.textTransparent(RA8875_GREEN);
  tft.textEnlarge(1); ////text size////
}

void display_menu()
{

tft.textMode();
  //tft.cursorBlink(32);
  
  /* Set the cursor location (in pixels) */
  //tft.textSetCursor(10, 10);
 
  /* Render some text! */
  
  tft.textSetCursor(60, 150);

 /* //vertical line///
  tft.drawFastVLine(280,00,272,RA8875_WHITE);
  tft.drawFastVLine(340,00,272,RA8875_WHITE);
  tft.drawFastVLine(400,00,200,RA8875_WHITE);

  //Horizontal line///
  tft.drawFastHLine(280,60,300,RA8875_WHITE);
  tft.drawFastHLine(280,130,300,RA8875_WHITE);
  tft.drawFastHLine(280,200,300,RA8875_WHITE);
*/
 //tft.drawRoundRect(260, 10, 200, 245,20,0X3186);//0x032c,0X0A0F,0X3041
//tft.fillRoundRect(260, 10, 200, 245,20,0X9410 );
//upper //
//tft.fillRect(0, 0, 480, 11 , 0X0A0F);
tft.fillRect(0, 0, 480, 15 , 0X0A0F);

//lower
tft.fillRect(0,260, 480, 12 , 0X0A0F);

  //vertical line///
  //tft.drawFastVLine(280,00,272,RA8875_WHITE);
//  tft.drawFastVLine(340,02,257,RA8875_WHITE);
 // tft.drawFastVLine(400,00,200,RA8875_WHITE);

  //Horizontal line///
  tft.drawFastHLine(0,12,480,RA8875_WHITE);
  tft.drawFastHLine(0,260,480,RA8875_WHITE);
 // tft.drawFastHLine(280,200,180,RA8875_WHITE);
////dial pad//////

///number 1//////
  tft.fillRoundRect(280, 30, 50, 30,5 ,0x0a0f);//0XC618
  tft.textSetCursor(290, 25); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(1); ////text size////
  tft.textWrite("1");
  
 ///number 2//////
 // tft.fillRoundRect(355, 04, 50, 58, 10, RA8875_RED);
  tft.fillRoundRect(345, 30, 50, 30,5, 0x0a0f);
  tft.textSetCursor(355, 25); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("2");

  ///number 3//////
  //tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
  tft.fillRoundRect(415, 30, 50, 30,5, 0x0a0f);
  tft.textSetCursor(425, 25); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("3");

  ///number 4//////
 // tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
  tft.fillRoundRect(280, 75, 50, 30,5, 0x0a0f);
  tft.textSetCursor(285, 71); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("4");

  ///number 5//////
 // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(345, 75, 50, 30,5, 0x0a0f); 
  tft.textSetCursor(355, 71); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite(stringn5);

  ///number 6//////
 // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
//  tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
tft.fillRoundRect(415, 75, 50, 30,5, 0x0a0f);
  tft.textSetCursor(425, 71); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite(stringn6);

  ///number 7//////
 // tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(280, 120, 50, 30,5, 0x0a0f);
  tft.textSetCursor(290, 116); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite(stringn7);

  ///number 8//////
 // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(345, 120, 50, 30,5, 0x0a0f);
  tft.textSetCursor(355, 116); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite(stringn8);

  ///number 9//////
  //tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
  //tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
  tft.fillRoundRect(415, 120, 50, 30,5, 0x0a0f);
  tft.textSetCursor(425, 116); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite(stringn9);

  ///back <<- //////
 // tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(280, 165, 50, 30,5, 0x0a0f);
  tft.textSetCursor(280, 160); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("<--");

  ///number 0//////
  //tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
 // tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
 tft.fillRoundRect(345, 165, 50, 30,5, 0x0a0f);
  tft.textSetCursor(350, 161); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite(stringn0);

  ///next ->>//////
  //tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
  //tft.drawRect(290, 04, 50, 58, RA8875_WHITE);
  tft.fillRoundRect(415, 165, 50, 30,5, 0x0a0f);
  tft.textSetCursor(415, 160); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-->");


///save button////
 //tft.fillRoundRect(355, 215, 120, 50, 10,RA8875_BLUE);
 tft.fillRoundRect(385, 210, 80, 30,5, 0x0a0f);
  tft.textSetCursor(385, 210); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("SAVE");

// allclear button////
//tft.fillRoundRect(175, 215, 80, 50, 10, RA8875_BLUE);
tft.fillRoundRect(280, 210, 80, 30,5, 0x0a0f);
  tft.textSetCursor(285, 210); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("CLR");

//to write user data on display///
// tft.textSetCursor(70, 135); ///text location////
// tft.textEnlarge(2);  ////text size////

 //Serial.print("di");
  tft.graphicsMode();///to enable touch and disable textmode else touch willnot work///
 
  
}

void qls()
{
  tft.fillScreen(0x0000);
tft.fillRoundRect(130,100,40,50,20,RA8875_BLUE);//130
tft.fillRoundRect(210,100,70,50,20,RA8875_CYAN);//210
tft.fillRect(150,100,80,51,RA8875_BLUE);//150
tft.fillRect(205,100,50,51,RA8875_CYAN);//205

tft.textSetCursor(160, 87);//160
  tft.textTransparent(RA8875_WHITE);
  /* If necessary, enlarge the font */
  tft.textEnlarge(3);
  /* ... and render some more text! */
  delay(400);
  tft.textWrite("Q");
  delay(400);
  tft.textWrite("L");
  delay(400);
  tft.textWrite("S");
  delay(400);

  tft.textSetCursor(130, 150);//130
  tft.textTransparent(RA8875_WHITE);
  /* If necessary, enlarge the font */
  tft.textEnlarge(0);
  /* ... and render some more text! */
  tft.textWrite("Q");
   tft.textWrite("U");
    tft.textWrite("A");
     tft.textWrite("L");
      tft.textWrite("I");
       tft.textWrite("T");
        tft.textWrite("Y");
         tft.textWrite(" ");
         delay(400);
          tft.textWrite("L");
           tft.textWrite("A");
  tft.textWrite("B");
  tft.textWrite(" ");
  delay(400);
  tft.textWrite("S");
  tft.textWrite("O");
  tft.textWrite("L");
  tft.textWrite("U");
  tft.textWrite("T");
  tft.textWrite("I");
  tft.textWrite("O");
  tft.textWrite("N");
  tft.textWrite("S");
  delay(3000);
}

void setup()
{
  maintenance_lockopen=0;
  // Wire.begin();
  Serial.begin(9600);
   Wire.begin();
  pinMode(pin4,OUTPUT);
pinMode(pin5,OUTPUT);
pinMode(pin6,OUTPUT);
pinMode(23,INPUT);
//pinMode(pin7,INPUT);
pinMode(2,INPUT);
pinMode(13,OUTPUT);
pinMode(7,INPUT);
pinMode(22,OUTPUT);
 //EEPROM.write(3800,0);
if(EEPROM.read(3800)!=1)
{
  for(i=0;i<=4000;i++)
  {
    EEPROM.write(i,'\0');
  }
for(i=0;defaultuser[i]!='\0' && i<18;i++)
  {
EEPROM.write(i,defaultuser[i]);
  }
EEPROM.write(i,'\0');
  ///defaultpassword///
  for(i=19;defaultpass[i-19]!='\0' && i<29;i++)
  {
EEPROM.write(i,defaultpass[i-19]);
//Serial.print(defaultpass[i]);
  }
EEPROM.write(i,'\0');  
 ///admin///
 for(i=30;admin[i-30]!='\0' && i<49;i++)
  {
EEPROM.write(i,admin[i-30]);
  }
EEPROM.write(i,'\0');
  ///admin pass///
  for(i=50;adminpassword[i-50]!='\0' && i<59;i++)
  {
EEPROM.write(i,adminpassword[i-50]);
//Serial.print(defaultpass[i]);
  }
EEPROM.write(i,'\0');
  ///user1///
 for(i=60;user1[i-60]!='\0' && i<79;i++)
  {
EEPROM.write(i,user1[i-60]);
  }
EEPROM.write(i,'\0');
  ///pass1///
  for(i=80;userpassword1[i-80]!='\0' && i<89;i++)
  {
EEPROM.write(i,userpassword1[i-80]);
//Serial.print(defaultpass[i]);
  }
EEPROM.write(i,'\0');
  ///username2///
 for(i=90;user2[i-90]!='\0' && i<109;i++)
  {
EEPROM.write(i,user2[i-90]);
  }
EEPROM.write(i,'\0');
  ///pass2///
  for(i=110;userpassword2[i-110]!='\0' && i<119;i++)
  {
EEPROM.write(i,userpassword2[i-110]);
//Serial.print(defaultpass[i]);
  }
EEPROM.write(i,'\0');
  ///username3///
 for(i=120;user3[i-120]!='\0' && i<129;i++)
  {
EEPROM.write(i,user3[i-120]);
  }
EEPROM.write(i,'\0');
  ///pass3///
  for(i=130;userpassword3[i-130]!='\0' && i<139;i++)
  {
EEPROM.write(i,userpassword3[i-130]);
//Serial.print(defaultpass[i]);
  }
EEPROM.write(i,'\0');
  ///username4///
 for(i=140;user4[i-140]!='\0' && i<159;i++)
  {
EEPROM.write(i,user4[i-140]);
  }
EEPROM.write(i,'\0');
  ///pass4///
  for(i=160;userpassword4[i-160]!='\0' && i<169;i++)
  {
EEPROM.write(i,userpassword4[i-160]);
//Serial.print(defaultpass[i]);
  }
EEPROM.write(i,'\0');
  ///username5///
 for(i=170;user5[i-170]!='\0' && i<189;i++)
  {
EEPROM.write(i,user5[i-170]);
  }
EEPROM.write(i,'\0');
  ///pass5///
  for(i=190;userpassword5[i-190]!='\0' && i<199;i++)
  {
EEPROM.write(i,userpassword5[i-190]);
  }
  EEPROM.write(i,'\0');
EEPROM.write(3800,1);

digitalWrite(13,HIGH);
  delay(300);
  digitalWrite(13,LOW);
  delay(300);
   digitalWrite(13,HIGH);
  delay(300);
  digitalWrite(13,LOW);
  delay(300);

  i=0;
}
if(EEPROM.read(19)=='8' && EEPROM.read(20)=='6' && EEPROM.read(21)=='5' && EEPROM.read(22)=='5')
{
 digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  delay(100);
   digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  delay(100);
}
digitalWrite(13,HIGH);
  
//Serial.println("RA8875 start");

  /* Initialize the display using 'RA8875_480x80', 'RA8875_480x128', 'RA8875_480x272' or 'RA8875_800x480' */
  if (!tft.begin(RA8875_480x272)) {
    //Serial.println("RA8875 Not Found!");

   // while (1);
    
  }
 // Serial.println("Found RA8875");

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
  //  while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
  }
   
  // rtc.adjust(DateTime(ay, am, ad, ah, ami , as));
  tft.displayOn(true);
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);
  //tft.fillScreen(0X379f);//0XF799

  /* Switch to text mode */
  tft.textMode();
 // tft.cursorBlink(32);
 qls();
 
  //tft.fillScreen(0X379f);//0XF799
  tft.fillScreen(0XFFFF);
home_menu();
tft.textMode();
tft.fillRect(20, 30, 52,30,RA8875_RED);
 tft.drawRect(20, 30, 52,30,0x0000);
  tft.textSetCursor(22, 35); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("LOGIN");
  tft.graphicsMode();
  
 //login_page();
 
 
  pinMode(RA8875_INT, INPUT);
  digitalWrite(RA8875_INT, HIGH);

  tft.touchEnable(true);
  
 // Serial.print("Status: "); Serial.println(tft.readStatus(), HEX);
 // Serial.println("Waiting for touch events ..."); 
digitalWrite(pin5,HIGH);
digitalWrite(pin6,LOW);
/*******************if(digitalRead(7)==0)
  { 
    while(digitalRead(7)==0)
    {
digitalWrite(pin4,LOW);
//delayMicroseconds(1666);
delayMicroseconds(170);
digitalWrite(pin4,HIGH);
delayMicroseconds(170);
if(digitalRead(7)==1)
  {
   // digitalWrite(pin5,0);
    break;
  }
}
//digitalWrite(pin5,HIGH);
//digitalWrite(pin6,HIGH);
  }*///////////////////////////////
  //digitalWrite(pin5,HIGH);
  //digitalWrite(pin6,HIGH);
//digitalWrite(13,HIGH);
//digitalWrite(22,HIGH);
//digitalWrite(9,LOW);
//delay(700);
//digitalWrite(22,LOW);
}



void loop()
{
 /* if(home_flag==1)
  {
    tft.fillScreen(0X379f);
    home_menu();
    home_flag=0;
  }*/
 //void drawRAW(
//tft.drawRGBBitmap(0, 0, name, 300, 300);
  //tft.drawRGBBitmap(0,0,name,100,100);
//  drawBitmap(0,0,Temp,100,100,0x0000);

//rtc_time();

if(loginshow==1 || loginback==1)
{
  tx=ty=0;
  delay(500);
  //tft.fillScreen(0X379f);
  tft.fillScreen(0XFFFF);
  home_menu();
tft.textMode();
tft.fillRect(20, 30, 52,30,RA8875_RED);
 tft.drawRect(20, 30, 52,30,0x0000);
  tft.textSetCursor(22, 35); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("LOGIN");
  tft.graphicsMode();
  loginshow=0;
  loginback=0;
  maintenance_lockopen=0;
}
delay(400);
  /* Wait around for touch events */
  if (! digitalRead(RA8875_INT))
  {
    //delay(300);   
    if (tft.touched())
    {
     // Serial.print("Touch: ");
      tft.touchRead(&tx, &ty);
     // Serial.print(tx); Serial.print(", "); Serial.println(ty);
    }

    if(tx>833 && tx<973 && ty>712 && ty<861)
    {
      for(j=0;j<=10;j++)
      {
      showusername[j]=0;
      }
      j=0;
     tft.fillScreen(0xffff);
     login_page();
     keypad();
     while(loginback==0)
     {
      if(loginback2==1)
    {
      loginback2=0;
      tft.fillScreen(0xffff);
     login_page();
     keypad();
    }
       //delay(400);   
       if (! digitalRead(RA8875_INT))
  {
   // if (tft.touched())
   // {
     // Serial.print("Touch: ");
      tft.touchRead(&tx, &ty);
      login_function();
      keypad2();
   // }
   
  }
  
      if(loginenter==1)
      {
        loginenter=0;
 for(j=0;login!='\0';j++)
{/*
  Serial.print("\n");
Serial.print(loginuser[j]);
 Serial.print("\n");*/
}
//////maintenence////////////
        for(i=19,j=0;EEPROM.read(i)!='\0' && i<29;i++,j++)
        {
        defaultusercheck[j]=EEPROM.read(i);
        //login_flag=1;
        }
        defaultusercheck[j]=0;
        for(i=0;loginuser[i]!='\0';i++)
        {
         /* Serial.print("loginu=");
          Serial.print(i);
          Serial.print(loginuser[i]);
          Serial.print("\n");*/
        }
        
  ////////////admin////////////      
        for(i=30,j=0;EEPROM.read(i)!='\0' && i<49;i++,j++)
        {
        admincheck[j]=EEPROM.read(i);
        
       
        }
        admincheck[j]=0;
//////////////user1//////////////////////   
        for(i=60,j=0;EEPROM.read(i)!='\0' && i<79;i++,j++)
        {
        user1check[j]=EEPROM.read(i);
        
        
        }
       user1check[j]=0;
//////////////user2//////////////////////
         for(i=90,j=0;EEPROM.read(i)!='\0' && i<109;i++,j++)
        {
        user2check[j]=EEPROM.read(i);
        
       
        }
        user2check[j]=0;
       //user3//////////////////////
         for(i=120,j=0;EEPROM.read(i)!='\0' && i<129;i++,j++)
        {
        user3check[j]=EEPROM.read(i);
         
        
        }
        user3check[j]=0;
 //////////////user4//////////////////////
         for(i=140,j=0;EEPROM.read(i)!='\0' && i<159;i++,j++)
        {
        user4check[j]=EEPROM.read(i);
         
        
        }
        user4check[j]=0;
 //////////////user5//////////////////////
         for(i=170,j=0;EEPROM.read(i)!='\0' && i<189;i++,j++)
        {
        user5check[j]=EEPROM.read(i);
         
        
        }
      user5check[j]=0;
       i=j=k=0;
       
 /***************************************************************************************/

///////////////////maintenance////////////////////
if(strlen(loginuser)==strlen(defaultusercheck))
{ //login_flag=1;
  error_flag=1;
  if(strncmp(loginuser,defaultusercheck,strlen(defaultusercheck))==0)
  {
   maintenance_lockopen=1;
    login_flag=1;
    loginuser[0]='\0';
    loginpass[0]='\0';
    defaultusercheck[0]='\0';
  }

 else// if(strncmp(loginuser,defaultusercheck,strlen(defaultusercheck))!=0)
  {
    tft.textMode();
      tft.textSetCursor(90, 55); ///text location////
  tft.textTransparent(RA8875_RED);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Error");
  tft.graphicsMode();
   //login_flag=1;
   loginuser[0]='\0';
   loginpass[0]='\0';
 // loginenter=0;
  }
}
//////////////////////admin/////////////////////////
if(strlen(loginuser)==strlen(admincheck))
{
  if(strncmp(loginuser,admincheck,strlen(admincheck))==0)
  {
   error_flag=1;
    //login_flag=1;
    loginuser[0]='\0';
     loginpass[0]='\0';
     admincheck[0]='\0';
     tft.fillScreen(0xffff);
   login_page2();
   keypad();
   while(loginback2==0)
   {
    
    //delay(400);   
       if (! digitalRead(RA8875_INT))
  {
   
      tft.touchRead(&tx, &ty);
      login_function2();
      keypad2();
   
  }
  if(loginenter2==1)
  {
  loginenter2=0;
  for(j=0;login!='\0';j++)
{
  Serial.print("\n");
  Serial.print("loginp==");
Serial.print(loginpass[j]);
 //Serial.print("\n");
}
     for(i=50,j=0;EEPROM.read(i)!='\0' && i<59;i++,j++)
        {
        adminpasswordcheck[j]=EEPROM.read(i);
        
        }
        for(i=0;adminpasswordcheck[i]!='\0';i++)
        {
          Serial.print("adminp==");
          Serial.print(i);
          Serial.print(adminpasswordcheck[i]);
          Serial.print("\n");
        }
     if(strlen(loginpass)==strlen(adminpasswordcheck))
    {   
  
    if(strncmp(loginpass,adminpasswordcheck,strlen(adminpasswordcheck))==0)
    {

  /*tft.textMode();
   tft.textSetCursor(400,10); ///text location////
    tft.textTransparent(RA8875_BLACK);
   tft.textEnlarge(1);  ////text size////
    tft.textWrite(showusername);*/
     for(i=30,j=0;EEPROM.read(i)!='\0' && i<49;i++,j++)
        {
        
        showusername[j]=EEPROM.read(i);
       
        }
    login_flag=1;
    loginuser[0]='\0';
    loginpass[0]='\0';
    adminpasswordcheck[0]='\0';
  }
  else
  {
    tft.textMode();
      tft.textSetCursor(90, 55); ///text location////
  tft.textTransparent(RA8875_RED);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Error");
  tft.graphicsMode();
  
   loginuser[0]='\0';
   loginpass[0]='\0';
 // loginenter=0;
  }
}
else
  {
    tft.textMode();
      tft.textSetCursor(90, 55); ///text location////
  tft.textTransparent(RA8875_RED);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Error");
  tft.graphicsMode();
   loginuser[0]='\0';
   loginpass[0]='\0';
 // loginenter=0;
  }
 i=j=0;
  }
if(loginback2==1)
{
  break;
}
 if(login_flag==1)
      {
        home_flag=1;
        break;
      }
   }
 }
}
///////////////////user1/////////////////////////////
if(strlen(loginuser)==strlen(user1check))
{
  if(strncmp(loginuser,user1check,strlen(user1check))==0)
  {
   i=0;
     error_flag=1;
    //login_flag=1;
    loginuser[0]='\0';
     loginpass[0]='\0';
     user1check[0]='\0';
     tft.fillScreen(0xffff);
   login_page2();
   keypad();
   while(loginback2==0)
   {
    
   // delay(400);   
       if (! digitalRead(RA8875_INT))
  {
   
      tft.touchRead(&tx, &ty);
      login_function2();
      keypad2();
   
  }
  if(loginenter2==1)
  {
  loginenter2=0;
     for(i=80,j=0;EEPROM.read(i)!='\0' && i<89;i++,j++)
        {
        userpassword1check[j]=EEPROM.read(i);
        
        }
        for(i=0,j=0;loginpass[i]!='\0';i++,j++)
        {
          
     Serial.print(loginpass[i]);
        
        }
       
     if(strlen(loginpass)==strlen(userpassword1check))
    {   
  
    if(strncmp(loginpass,userpassword1check,strlen(userpassword1check))==0)
    {

  /*tft.textMode();
   tft.textSetCursor(400,10); ///text location////
    tft.textTransparent(RA8875_BLACK);
   tft.textEnlarge(1);  ////text size////
    tft.textWrite(showusername);*/
    for(i=60,j=0;EEPROM.read(i)!='\0' && i<79;i++,j++)
        {
        
        showusername[j]=EEPROM.read(i);
       
        }
    login_flag=1;
    loginuser[0]='\0';
    loginpass[0]='\0';
    userpassword1check[0]='\0';
  }
  else
  {
    tft.textMode();
      tft.textSetCursor(90, 55); ///text location////
  tft.textTransparent(RA8875_RED);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Error");
  tft.graphicsMode();
   loginuser[0]='\0';
   loginpass[0]='\0';
 // loginenter=0;
  }
}
else
  {
    tft.textMode();
      tft.textSetCursor(90, 55); ///text location////
  tft.textTransparent(RA8875_RED);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Error");
  tft.graphicsMode();
   loginuser[0]='\0';
   loginpass[0]='\0';
 // loginenter=0;
  }
 i=j=0;
  }
if(loginback2==1)
{
  break;
}
 if(login_flag==1)
      {
        home_flag=1;
        break;
      }
   }
  }
}
///////////////////user2/////////////////////////////
if(strlen(loginuser)==strlen(user2check))
{
  if(strncmp(loginuser,user2check,strlen(user2check))==0)
  {
   
     error_flag=1;
    //login_flag=1;
    loginuser[0]='\0';
     loginpass[0]='\0';
     user2check[0]='\0';
     tft.fillScreen(0xffff);
   login_page2();
   keypad();
   while(loginback2==0)
   {
    
   // delay(400);   
       if (! digitalRead(RA8875_INT))
  {
   
      tft.touchRead(&tx, &ty);
      login_function2();
      keypad2();
   
  }
  if(loginenter2==1)
  {
  loginenter2=0;
  
     for(i=110,j=0;EEPROM.read(i)!='\0' && i<119;i++,j++)
        {
        userpassword2check[j]=EEPROM.read(i);
        
        }
      
     if(strlen(loginpass)==strlen(userpassword2check))
    {   
  
    if(strncmp(loginpass,userpassword2check,strlen(userpassword2check))==0)
    {

  /*tft.textMode();
   tft.textSetCursor(400,10); ///text location////
    tft.textTransparent(RA8875_BLACK);
   tft.textEnlarge(1);  ////text size////
    tft.textWrite(showusername);*/
    for(i=90,j=0;EEPROM.read(i)!='\0' && i<109;i++,j++)
        {
        
        showusername[j]=EEPROM.read(i);
       
        }
    login_flag=1;
    loginuser[0]='\0';
    loginpass[0]='\0';
    userpassword2check[0]='\0';
  }
  else
  {
    tft.textMode();
      tft.textSetCursor(90, 55); ///text location////
  tft.textTransparent(RA8875_RED);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Error");
  tft.graphicsMode();
   loginuser[0]='\0';
   loginpass[0]='\0';
 // loginenter=0;
  }
}
else
  {
    tft.textMode();
      tft.textSetCursor(90, 55); ///text location////
  tft.textTransparent(RA8875_RED);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Error");
  tft.graphicsMode();
   loginuser[0]='\0';
   loginpass[0]='\0';
 // loginenter=0;
  }
 i=j=0;
  }
if(loginback2==1)
{
  break;
}
 if(login_flag==1)
      {
        home_flag=1;
        break;
      }
   }
  }
}
///////////////////user3/////////////////////////////
if(strlen(loginuser)==strlen(user3check))
{
  if(strncmp(loginuser,user3check,strlen(user3check))==0)
  {
   
     error_flag=1;
    //login_flag=1;
    loginuser[0]='\0';
     loginpass[0]='\0';
     user3check[0]='\0';
     tft.fillScreen(0xffff);
   login_page2();
   keypad();
   while(loginback2==0)
   {
    
   // delay(400);   
       if (! digitalRead(RA8875_INT))
  {
   
      tft.touchRead(&tx, &ty);
      login_function2();
      keypad2();
   
  }
  if(loginenter2==1)
  {
  loginenter2=0;
  
     for(i=130,j=0;EEPROM.read(i)!='\0' && i<139;i++,j++)
        {
        userpassword3check[j]=EEPROM.read(i);
        
        }
        
     if(strlen(loginpass)==strlen(userpassword3check))
    {   
  
    if(strncmp(loginpass,userpassword3check,strlen(userpassword3check))==0)
    {

  /*tft.textMode();
   tft.textSetCursor(400,10); ///text location////
    tft.textTransparent(RA8875_BLACK);
   tft.textEnlarge(1);  ////text size////
    tft.textWrite(showusername);*/
    for(i=120,j=0;EEPROM.read(i)!='\0' && i<129;i++,j++)
        {
        
        showusername[j]=EEPROM.read(i);
       
        }
    login_flag=1;
    loginuser[0]='\0';
    loginpass[0]='\0';
    userpassword3check[0]='\0';
  }
  else
  {
    tft.textMode();
      tft.textSetCursor(90, 55); ///text location////
  tft.textTransparent(RA8875_RED);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Error");
  tft.graphicsMode();
   loginuser[0]='\0';
   loginpass[0]='\0';
 // loginenter=0;
  }
}
else
  {
    tft.textMode();
      tft.textSetCursor(90, 55); ///text location////
  tft.textTransparent(RA8875_RED);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Error");
  tft.graphicsMode();
   loginuser[0]='\0';
   loginpass[0]='\0';
 // loginenter=0;
  }
 i=j=0;
  }
if(loginback2==1)
{
  break;
}
 if(login_flag==1)
      {
        home_flag=1;
        break;
      }
   }
  }
}
///////////////////user4/////////////////////////////
if(strlen(loginuser)==strlen(user4check))
{
  if(strncmp(loginuser,user4check,strlen(user4check))==0)
  {
   
     error_flag=1;
    //login_flag=1;
    loginuser[0]='\0';
     loginpass[0]='\0';
     user4check[0]='\0';
     tft.fillScreen(0xffff);
   login_page2();
   keypad();
   while(loginback2==0)
   {
    
    //delay(400);   
       if (! digitalRead(RA8875_INT))
  {
   
      tft.touchRead(&tx, &ty);
      login_function2();
      keypad2();
   
  }
  if(loginenter2==1)
  {
  loginenter2=0;
  
     for(i=160,j=0;EEPROM.read(i)!='\0' && i<169;i++,j++)
        {
        userpassword4check[j]=EEPROM.read(i);
        
        }
       
     if(strlen(loginpass)==strlen( userpassword4check))
    {   
  
    if(strncmp(loginpass, userpassword4check,strlen( userpassword4check))==0)
    {

  /*tft.textMode();
   tft.textSetCursor(400,10); ///text location////
    tft.textTransparent(RA8875_BLACK);
   tft.textEnlarge(1);  ////text size////
    tft.textWrite(showusername);*/
    for(i=140,j=0;EEPROM.read(i)!='\0' && i<159;i++,j++)
        {
        
        showusername[j]=EEPROM.read(i);
       
        }
    login_flag=1;
    loginuser[0]='\0';
    loginpass[0]='\0';
    userpassword4check[0]='\0';
  }
  else
  {
    tft.textMode();
      tft.textSetCursor(90, 55); ///text location////
  tft.textTransparent(RA8875_RED);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Error");
  tft.graphicsMode();
   loginuser[0]='\0';
   loginpass[0]='\0';
 // loginenter=0;
  }
}
else
  {
    tft.textMode();
      tft.textSetCursor(90, 55); ///text location////
  tft.textTransparent(RA8875_RED);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Error");
  tft.graphicsMode();
   loginuser[0]='\0';
   loginpass[0]='\0';
 // loginenter=0;
  }
 i=j=0;
  }
if(loginback2==1)
{
  break;
}
 if(login_flag==1)
      {
        home_flag=1;
        break;
      }
   }
  }
}
///////////////////user5/////////////////////////////
if(strlen(loginuser)==strlen(user5check))
{
  if(strncmp(loginuser,user5check,strlen(user5check))==0)
  {
   
    error_flag=1;
    //login_flag=1;
    loginuser[0]='\0';
     loginpass[0]='\0';
     user5check[0]='\0';
     tft.fillScreen(0xffff);
   login_page2();
   keypad();
   while(loginback2==0)
   {
    
    //delay(400);   
       if (! digitalRead(RA8875_INT))
  {
   
      tft.touchRead(&tx, &ty);
      login_function2();
      keypad2();
   
  }
  if(loginenter2==1)
  {
  loginenter2=0;
 
     for(i=190,j=0;EEPROM.read(i)!='\0' && i<199;i++,j++)
        {
        userpassword5check[j]=EEPROM.read(i);
        
        }
        
     if(strlen(loginpass)==strlen(userpassword5check))
    {   
  
    if(strncmp(loginpass,userpassword5check,strlen(userpassword5check))==0)
    {

  /*tft.textMode();
   tft.textSetCursor(400,10); ///text location////
    tft.textTransparent(RA8875_BLACK);
   tft.textEnlarge(1);  ////text size////
    tft.textWrite(showusername);*/
    for(i=170,j=0;EEPROM.read(i)!='\0' && i<189;i++,j++)
        {
        
        showusername[j]=EEPROM.read(i);
       
        }
    login_flag=1;
    loginuser[0]='\0';
    loginpass[0]='\0';
    userpassword5check[0]='\0';
  }
  else
  {
    tft.textMode();
      tft.textSetCursor(90, 55); ///text location////
  tft.textTransparent(RA8875_RED);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Error");
  tft.graphicsMode();
   loginuser[0]='\0';
   loginpass[0]='\0';
 // loginenter=0;
  }
}
else
  {
    tft.textMode();
      tft.textSetCursor(90, 55); ///text location////
  tft.textTransparent(RA8875_RED);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Error");
  tft.graphicsMode();
   //login_flag=1;
   loginuser[0]='\0';
   loginpass[0]='\0';
 // loginenter=0;
  }
 i=j=0;
  }
if(loginback2==1)
{
  break;
}
 if(login_flag==1)
      {
        home_flag=1;
        break;
      }
   }
  }
}

if(error_flag==0)
{
   tft.textMode();
      tft.textSetCursor(90, 55); ///text location////
  tft.textTransparent(RA8875_RED);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Error");
  tft.graphicsMode();
  loginuser[0]='\0';
  loginpass[0]='\0';
 // loginenter=0;
}
      }
 
     // loginenter=0;
      if(loginback==1)
      {
        loginshow=1;
       // loginback=0;
        break;
      }
      if(login_flag==1)
      {
        home_flag=1;
        break;
      }
     }
    
    }
    
   if(tx<859 && ty<702 && login_flag!=1)
    {
      tft.textMode();
      tft.textSetCursor(35, 250); ///text location////
  tft.textTransparent(RA8875_RED);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Please Login to Enter");
  tft.graphicsMode();
    }

while(login_flag==1)
{
  rtc_time();
//tft.fillRect(361,1,170,18,RA8875_WHITE);
tft.textMode();
   tft.textSetCursor(363,3); ///text location////
    tft.textTransparent(RA8875_BLUE);
   tft.textEnlarge(0);  ////text size////
    tft.textWrite(showusername);
    tft.graphicsMode();
error_flag=0;
    
  if(home_flag==1)
  {
  //  tft.fillScreen(0X379f);
   tft.fillScreen(0XFFFF);
   // homestatus_menu();
    home_menu();
   // homestatus_menu();
    home_flag=0;
   // login_flag=0;
  }
  if (! digitalRead(RA8875_INT))
  {
    delay(300);   
    if (tft.touched())
    {
      Serial.print("Touch: ");
      tft.touchRead(&tx, &ty);
      Serial.print(tx); Serial.print(", "); Serial.println(ty);
    }
/*if(home_flag==1)
  {
    tft.fillScreen(0X379f);
   // homestatus_menu();
    home_menu();
   // homestatus_menu();
    home_flag=0;
   // login_flag=0;
  }
*/
//rtc_time();

 if(tx>833 && tx<973 && ty>712 && ty<861)
    {
      loginshow=1;
      login_flag=0;
      showusername[0]='\0';
    /*  for(i=0;i<=19;i++)
  {
  loginuser[i]='0';
  }*/
      // loginuser[0]='0';
       Serial.print(loginuser);
      break;
    }
    
    //login///
  /* while(login==0)
    {
    if (! digitalRead(RA8875_INT))
  {
    if (tft.touched())
    {
    tft.touchRead(&tx, &ty);
    }
    
     if(tx>859 && tx<945 && ty<781 && ty>725)
     {
      login=1;
      keypad();
      home_menu();
      //Serial.print("IF");

      
     }
     else
     {
      login_page();
     }
   
  }
  tft.graphicsMode();
//  Serial.print("inlogin");
    }

*/
////home_page////
// >> home_page {
  //time///
  if(tx>484 && tx<630 && ty>593 && ty<808)
  {
 tft.fillScreen(RA8875_WHITE);//to clear screen,9410,3410
 time_disp();
 display_menu();
 ty=tx=0;
 delay(700);
 total_time=4;
 tc=0;
 while(timebackflag==0)
  {
    //Serial.print("z");
    if(weightback_flag==1)
    {
      tft.fillScreen(RA8875_WHITE);//to clear screen,9410,3410
 time_disp();
 display_menu();
 weightback_flag=0;
 tx=ty=0;
 tc=0;
 delay(700);
    }
     delay(600);
      if (! digitalRead(RA8875_INT))
  {
   if (tft.touched())
    {
    tft.touchRead(&tx, &ty);
    Serial.print(tx); Serial.print(", "); Serial.println(ty);
 time_function(); 
  }
  }
 if(timebackflag==1)
 {
  timebackflag=0;
  break;
 }
 if(timenextflag==1)
 {
  timenextflag=0;
 // while(weightback_flag==0)
 // {
  if(save_no_of_drums==1)
  {
    if(save_balance==1)
    {//timecountweight1();
      while(weightback_flag==0)
      {
        ///load cell part///
      }
      weightback_flag=0;
      
    }
    if(save_balance==2)
    {
      timecountweight1_menu();
      display_menu();
      tx=ty=0;
      delay(700);
      while(weightback_flag==0)
      {
         delay(500);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
        timecountweight1_function();
  }
  if(weightnext_flag==1)
  {
    tx=ty=0;
    weightnext_flag=0;
    ///start,stop///
    tft.fillScreen(RA8875_WHITE);
    motor_menu();
     tft.textMode();
    tft.textSetCursor(220, 120);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite("Elapsed Time");

tft.fillRect(228,158,140,40,RA8875_WHITE);
  tft.textSetCursor(230, 160);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite("00");
   tft.textWrite(":");
   tft.textSetCursor(280, 160);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
 tft.textWrite("00");
 tft.textWrite(":");
  tft.textSetCursor(330, 160);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite("00");
  
  tft.graphicsMode();
         // motor_menu();
         tx=ty=0;
          delay(700);
  while(motornext_flag==0)
  {
    
    if(/*(motorstart_flag==1 && motorstop_flag==1 && processbreak==1&& process2==1) || */calculation_flag==1)
    {
      tx=ty=0;
      motorstart_flag=0,motorstop_flag=0,process2=0,calculation_flag=0;
       tft.fillScreen(RA8875_WHITE);
       // tft.fillScreen(RA8875_WHITE);
       motor_menu();
     tft.textMode();
    tft.textSetCursor(220, 120);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite("Elapsed Time");
  
  tft.fillRect(228,158,140,40,RA8875_WHITE);
  tft.textSetCursor(230, 160);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite("00");
   tft.textWrite(":");
   tft.textSetCursor(280, 160);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
 tft.textWrite("00");
 tft.textWrite(":");
  tft.textSetCursor(330, 160);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite("00");
 
  tft.graphicsMode();
  
     // motor_menu();
     dubrunsec=0;
     tx=ty=0;
      delay(700);
     // process2=1 
    }
    
    delay(150);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
    motor_function();
    if(motornext_flag==1)
    {
      break;
    }
  }
   if(motornext_flag==1)
    {
      break;
    }
    if(motorstart_flag==1)
    {
      Serial.print("y");
     // while(timeprocess==0)//(dubrunsec/60)!=total_time || motornext_flag==1)
    //  {
       // motorstart_flag=0;
      motor_timestart_function();
      delay(500);
     // tft.fillScreen(RA8875_WHITE);
      unloading_function();
      runmilisec=runsec=runmin=runhour=0;
    //  }
      
    }
 /*   if((motorstart_flag==1 && motorstop_flag==1)
    {
       tft.fillScreen(RA8875_WHITE);
      
     // process2=1;
      
    }*/
    
    if(/*(dubrunsec/60)==total_time && */process2==1 /*&& motorstop_flag!=1*/)
    {
      motorstop_flag=0;
       process2=0;
      motorstart_flag=0; 
      timeprocess=0;
      calculation_flag=1;
      if(save_calculation==1)
      {
        if(save_no_of_drums==1)
        {
          if(save_balance==1)
          {
            ///load cell///
          }
          
           if(save_balance==2)
          {
            tx=ty=0;
            tft.fillScreen(RA8875_WHITE);
            outtimecountweight_menu();
            display_menu();
            tx=ty=0;
            delay(700);
            while(outweightnext1_flag==0)
            {
              delay(600);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
            outtimecountweight_function();
  }
/*  if(motornext_flag==1)
  {
  //  motornext_flag=0;
    break;
   // goto home_menu;
  }*/
            }
            if(outweightnext1_flag==1)
            {
              tx=ty=0;
              tft.fillScreen(RA8875_WHITE);
            calculation_result();
            outweightnext1_flag=0;
             tx=ty=0;
             delay(700);
              while(1)//resultnext_flag==0)
              {
                delay(1200);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
             calculation_function();
  }
          //  outweightnext1_flag=0;
           //calculation_function();
          //  outweightnext1_flag=0;
          if(resultnext_flag==1)
          {
            tx=ty=0;
            resultnext_flag=0;
           // delay(300);
            tft.fillScreen(RA8875_WHITE);
            paratobe_print_menu();
             tx=ty=0;
             delay(700);
            while(paratobeprintnext_flag==0)
            {
              if(productback_flag==1)
              {
                tft.fillScreen(RA8875_WHITE);
              paratobe_print_menu();
              productback_flag=0;
              tx=ty=0;
              delay(700);
              }
              delay(1200);
         if (! digitalRead(RA8875_INT))
          {
            tft.touchRead(&tx, &ty);
               paratobe_print_function();
          }

          historysave_function();
              if(productname_flag==1)
              {
                productname_flag=0;
                 tft.fillScreen(RA8875_WHITE);
                product_name_menu();
                keypad();
             // }
                while(productback_flag==0)
                {
                   //delay(400);
         if (! digitalRead(RA8875_INT))
          {
            tft.touchRead(&tx, &ty);
                  product_name_function();
                  keypad2();
          }
                  if( productenter_flag==1)
                  {
                    productenter_flag=0;
                    for(i=0; product_name[i]!='\0';i++)
                    {
                    save_product_name[i]= product_name[i];
                    }
                    product_name[0]='\0';
                  }
                }
               // productback_flag=0;
              }
              if(batchno_flag==1)
              {
                batchno_flag=0;
                 tft.fillScreen(RA8875_WHITE);
                batch_number_menu();
                 keypad();
                while(productback_flag==0)
                {
                   //delay(400);
         if (! digitalRead(RA8875_INT))
          {
            tft.touchRead(&tx, &ty);
                  product_name_function();
                  keypad2();
          }
                  if( productenter_flag==1)
                  {
                    productenter_flag=0;
                    for(i=0; product_name[i]!='\0';i++)
                    {
                    save_batch_number[i]=product_name[i];
                    }
                    product_name[0]='\0';
                  }
                }
              //  productback_flag=0;
              }
              if(lotno_flag==1)
              {
                lotno_flag=0;
                 tft.fillScreen(RA8875_WHITE);
                lot_number_menu();
                keypad();
                while(productback_flag==0)
                {
                   //delay(400);
         if (! digitalRead(RA8875_INT))
          {
            tft.touchRead(&tx, &ty);
                  product_name_function();
                  keypad2();
          }
                  if( productenter_flag==1)
                  {
                    productenter_flag=0;
                    for(i=0; product_name[i]!='\0';i++)
                    {
                    save_lot_number[i]= product_name[i];
                    }
                    product_name[0]='\0';
                  }
                }
               // productback_flag=0;
              }
            }
          }
          if(paratobeprintnext_flag==1)
          {
            break;
          }
              }
            }
            outweightnext1_flag=0;
          }
        }
      }
     if(save_calculation==2)
    {
      calculation_flag=1;
    } 
     
    }
 // }
           if(paratobeprintnext_flag==1)//resultnext_flag==1)
              {
                break;
              }
               if(motornext_flag==1)
    {
      break;
    }
  }
   // motornext_flag=1;
  }
  //weightnext_flag=0;
  if(paratobeprintnext_flag==1)//resultnext_flag==1)
              {
                break;
              }
  if(motornext_flag==1)
  {
  //  motornext_flag=0;
    break;            
  }
      }
     weightnext_flag=0;
    }
 // }
  }
  
  if(save_no_of_drums==2)
  {
    delay(500);
    tx=0,ty=0;
    if(save_balance==1)
    {
      
      while(weightback_flag==0)
      {
        ////load cell///
      }
      weightback_flag=0;
      
    }
    if(save_balance==2)
    {
      //delay(700);
    tx=0,ty=0;
      tft.fillScreen(RA8875_WHITE);
      timecountweight1_menu();
      display_menu();
       tx=0,ty=0;
        delay(700);
      while(weightback_flag==0)
      { 
        if(weightback2_flag==1)
        {
          tft.fillScreen(RA8875_WHITE);
      timecountweight1_menu();
      display_menu();
      weightback2_flag=0;
      tx=ty=0;
      delay(700);
        }
         delay(400);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
        timecountweight1_function();
  }
        if(weightnext_flag==1)
        {
         // delay(500);
    tx=0,ty=0;
          weightnext_flag=0;
          tft.fillScreen(RA8875_WHITE);
          timecountweight2_menu();
          display_menu();
           tx=0,ty=0;
            delay(700);
          while(weightback2_flag==0)
          {
           delay(400);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
            timecountweight2_function();
  }
  if(weightback2_flag==1)
 {
  break;
 }
  if(weightnext2_flag==1)
        {
          weightnext2_flag=0;
          ///store,start///
          tft.fillScreen(RA8875_WHITE);
          motor_menu();
 tft.textMode();
    tft.textSetCursor(220, 120);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite("Elapsed Time");
  
  tft.fillRect(228,158,140,40,RA8875_WHITE);
  tft.textSetCursor(230, 160);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite("00");
   tft.textWrite(":");
   tft.textSetCursor(280, 160);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
 tft.textWrite("00");
 tft.textWrite(":");
  tft.textSetCursor(330, 160);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite("00");
 
  tft.graphicsMode();
        tx=ty=0;
        delay(700);  
        //  motor_menu();
  while(motornext_flag==0)
  {

     if(/*(motorstop_flag==1 && motorstart_flag==1 && process2==1) || */calculation_flag==1)
    {
      motorstop_flag=0,motorstart_flag=0,process2=0,calculation_flag=0;
      dubrunsec=0;
      tft.fillScreen(RA8875_WHITE);
      motor_menu();
       tft.textMode();
    tft.textSetCursor(220, 120);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite("Elapsed Time");
  
  tft.fillRect(228,158,140,40,RA8875_WHITE);
  tft.textSetCursor(230, 160);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite("00");
   tft.textWrite(":");
   tft.textSetCursor(280, 160);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
 tft.textWrite("00");
 tft.textWrite(":");
  tft.textSetCursor(330, 160);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite("00");
 
  tft.graphicsMode();
       //motor_menu();
   //   process2=0;
    //  motorstop_flag=0;
     dubrunsec=0;
      tx=ty=0;
      delay(700);
    }
    
    delay(150);
      if (! digitalRead(RA8875_INT))
  {
    
    tft.touchRead(&tx, &ty);
    motor_function();
  }
    if(motorstart_flag==1)
    {
     // motorstart_flag=0;
      motor_timestart_function();
      delay(500);
     //tft.fillScreen(RA8875_WHITE);
      unloading_function();
      runmilisec=runsec=runmin=runhour=0;
    }
  /*  if(motorstop_flag==1 && process2==1)
    {
      tft.fillScreen(RA8875_WHITE);
      unloading_function();
      process2=0;
      motorstop_flag=0;
      
    }*/
    if(/*motorstop_flag!=1 && (dubrunsec/60)==total_time && */process2==1)
    {
      dubrunsec=0;
      motorstop_flag=0;
       process2=0;
       motorstart_flag=0;
       timeprocess=0;
       calculation_flag=1;
      if(save_calculation==1)
      {
        /*
        if(save_no_of_drums==1)
        {
          if(save_balance==1)
          {
            ///load cell///
          }
          
          if(save_balance==2)
          {
            tft.fillScreen(RA8875_WHITE);
            outtimecountweight_menu();
            display_menu();
            delay(1000);
            while(outweightnext1_flag==0)
            {
              delay(150);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
            outtimecountweight_function();
  }
  if(motornext_flag==1)
  {
    motornext_flag=0;
    goto home_menu;
  }
            }
            if(outweightnext1_flag==1)
            {
              tft.fillScreen(RA8875_RED);
            calculation_result();
            outweightnext1_flag=0;
            }
           // calculation_result();
            outweightnext1_flag=0;
          
        }
        }*/
        if(save_no_of_drums==2)
        {
          runsec=runmin=runhour=0;
          if(save_balance==1)
          {
            ///load cell///
          }
          if(save_balance==2)
          {
            tft.fillScreen(RA8875_WHITE);
            outtimecountweight_menu();
            display_menu();
            tx=ty=0;
            delay(700);
            while(outweightback1_flag==0)
            {
              if(outweightnext2_flag==1)
              {
                tft.fillScreen(RA8875_WHITE);
            outtimecountweight_menu();
            display_menu();
            outweightnext2_flag=0;
            tx=ty=0;
            delay(700);
              }
              delay(600);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
            outtimecountweight_function();
  }
            
            if(outweightnext1_flag==1)
            {
              outweightnext1_flag=0;
              tft.fillScreen(RA8875_WHITE);
              outtimecountweight2_menu();
            display_menu();
            tx=ty=0;
            delay(1000);
            while(outweightback2_flag==0)
            { 
              
              delay(600);
         if (! digitalRead(RA8875_INT))
          {
            tft.touchRead(&tx, &ty);
            outtimecountweight2_function();
             }
        if(outweightback2_flag==1)
        {
          break;
        }
            
            //outweightnext2_flag=0;
           // outweightnext1_flag=0;
            
            if(outweightnext2_flag==1)
            {
              outweightnext2_flag=0;
               tft.fillScreen(RA8875_WHITE);
            calculation_result();
            tx=ty=0;
            delay(700);
              while(1)//resultnext_flag==0)
              {
                delay(1200);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
             calculation_function();
  }
          //  outweightnext1_flag=0;
           if(resultnext_flag==1)
          {
          //  delay(700);
            resultnext_flag=0;
            tft.fillScreen(RA8875_WHITE);
            paratobe_print_menu();
            tx=ty=0;
            delay(700);
            while(paratobeprintnext_flag==0)
            {
              if(productback_flag==1)
              {
                tft.fillScreen(RA8875_WHITE);
              paratobe_print_menu();
              productback_flag=0;
              tx=ty=0;
              delay(700);
              }
              delay(700);
         if (! digitalRead(RA8875_INT))
          {
            tft.touchRead(&tx, &ty);
               paratobe_print_function();
          }
           historysave_function();
              if(productname_flag==1)
              {
                productname_flag=0;
                 tft.fillScreen(RA8875_WHITE);
                product_name_menu();
                keypad();
             // }
                while(productback_flag==0)
                {
                   //delay(300);
         if (! digitalRead(RA8875_INT))
          {
            tft.touchRead(&tx, &ty);
                  product_name_function();
                  keypad2();
          }
                  if( productenter_flag==1)
                  {
                    productenter_flag=0;
                    for(i=0; product_name[i]!='\0';i++)
                    {
                    save_product_name[i]= product_name[i];
                    }
                    for(i=0;i<10;i++)
                    {
                    product_name[i]='\0';
                    }
                   
                  }
                }
              //  productback_flag=0;
              }
              if(batchno_flag==1)
              {
                batchno_flag=0;
                 tft.fillScreen(RA8875_WHITE);
                batch_number_menu();
                 keypad();
                while(productback_flag==0)
                {
                   //delay(400);
         if (! digitalRead(RA8875_INT))
          {
            tft.touchRead(&tx, &ty);
                  product_name_function();
                  keypad2();
          }
                  if( productenter_flag==1)
                  {
                    productenter_flag=0;
                    for(i=0; product_name[i]!='\0';i++)
                    {
                    save_batch_number[i]=product_name[i];
                    }
                    for(i=0;i<10;i++)
                    {
                    product_name[i]='\0';
                    }
                  }
                }
              //  productback_flag=0;
              }
              if(lotno_flag==1)
              {
                lotno_flag=0;
                 tft.fillScreen(RA8875_WHITE);
                lot_number_menu();
                keypad();
                while(productback_flag==0)
                {
                   //delay(400);
         if (! digitalRead(RA8875_INT))
          {
            tft.touchRead(&tx, &ty);
                  product_name_function();
                  keypad2();
          }
                  if( productenter_flag==1)
                  {
                    productenter_flag=0;
                    for(i=0; product_name[i]!='\0';i++)
                    {
                    save_lot_number[i]= product_name[i];
                    }
                    for(i=0;i<10;i++)
                    {
                    product_name[i]='\0';
                    }
                  }
                }
             //   productback_flag=0;
              }
            }
          }
          if(paratobeprintnext_flag==1)
          {
            break;
          }
              }
             /* if(resultnext_flag==1)
              {
                break;
              }*/
            }
             if(paratobeprintnext_flag==1)//resultnext_flag==1)
              {
                break;
              }
            }
            }
             if(paratobeprintnext_flag==1)//resultnext_flag==1)
              {
                break;
              }
            }
            
          }
          }
       // }
      }
      if(save_calculation==2)
    {
      calculation_flag=1;
    } 
     
    }
    if(motornext_flag==1)
          {
             break;
          }
           if(paratobeprintnext_flag==1)//resultnext_flag==1)
              {
                break;
              }
  }
  
/*  if(motornext_flag==1)
  {
    break;
  }
  */
 /////////////// 
 /// if(save_calculation==1)
 // {
    
 // }
/* if(motornext_flag==1)
          {
             break;
          }
  */
        }
         if(paratobeprintnext_flag==1)//resultnext_flag==1)
              {
                break;
              }
       // weightback2_flag=0;
       if(motornext_flag==1)
  {
    //motornext_flag=0;
    break;            
  }
          }
         
         
        }
       // weightback_flag=0;
       if(weightback_flag==1)
       {
        break;
       }
        if(paratobeprintnext_flag==1)//resultnext_flag==1)
              {
                break;
              }
   if(motornext_flag==1)
  {
    //motornext_flag=0;
    break;            
  }
      }
      weightback_flag=0;
    }
    
 }

  }
 // Serial.print("uplz");
 // timenextflag=0;
            if(paratobeprintnext_flag==1)//resultnext_flag==1)
              {
               paratobeprintnext_flag=0;// resultnext_flag=0;
                break;
              }
 if(motornext_flag==1)
  {
    //motornext_flag=0;
    break;            
  }
             // Serial.print("lz");
  }
  timenextflag=0;
//////////////////////////////////  
//home_menu:
  home_flag=1;
   paratobeprintnext_flag=0;//resultnext_flag=0;
  //timenextflag=0;
   motornext_flag=0;
  timebackflag=0;
  }

 ////////////// ////counts//////////////////////////
  if(tx>440 && tx<657 && ty>407 && ty<593)
  {
    tft.fillScreen(RA8875_WHITE);//to clear screen,9410,3410
    counts_disp();
    display_menu();
    total_count=100;
    tc=0;
    while(countbackflag==0)// || countnextflag==0)
      {
         if(weightback_flag==1)
    {
      tft.fillScreen(RA8875_WHITE);//to clear screen,9410,3410
     counts_disp();
      display_menu();
     weightback_flag=0;
     tc=0;
    }
        
         delay(600);
      if (! digitalRead(RA8875_INT))
  {
     if (tft.touched())
    {
    tft.touchRead(&tx, &ty);
        counts_function();
  }
  }
   if(countbackflag==1)// || countnextflag==1)
     {
      countbackflag=0;
       break;
    }
    if(countnextflag==1)
 {
 // while(weightback_flag==0)
 // {
  if(save_no_of_drums==1)
  {
    if(save_balance==1)
    {//timecountweight1();
      while(weightback_flag==0)
      {
        ///load cell part///
      }
      weightback_flag=0;
      
    }
    if(save_balance==2)
    {
      timecountweight1_menu();
      display_menu();
      tx=ty=0;
      delay(700);
      while(weightback_flag==0)
      {
         delay(600);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
        timecountweight1_function();
  }
  if(weightnext_flag==1)
  {
    weightnext_flag=0;
    ///start,stop///
    tft.fillScreen(RA8875_WHITE);
          motor_menu();
          tft.textMode();
     tft.textSetCursor(220, 120);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite("Total Counts");

tft.fillRect(228,158,140,40,RA8875_WHITE);
  tft.textSetCursor(230, 160);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite("00");
  
  tft.graphicsMode();
  tx=ty=0;
          delay(700);
  while(motornext_flag==0)
  {
    
if(/*(motorstart_flag==1 && motorstop_flag==1 && process2==1) ||*/ calculation_flag==1)
{
  motorstart_flag=0,motorstop_flag=0,process2=0,calculation_flag=0;
  dubcounts=0;
  tft.fillScreen(0xffff);
  motor_menu();
   tft.textMode();
     tft.textSetCursor(220, 120);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite("Total Counts");
  
tft.fillRect(228,158,100,40,RA8875_WHITE);
  tft.textSetCursor(230, 160);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite("00");
  
  tft.graphicsMode();
  tx=ty=0;
          delay(700);
}
   
    delay(150);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
    motor_function();
  }
    if(motorstart_flag==1)
    {
      Serial.print("y");
     // while(timeprocess==0)//(dubrunsec/60)!=total_time || motornext_flag==1)
     // {
        //motorstart_flag=0;
      motor_countstart_function();
       delay(500);
     // tft.fillScreen(RA8875_WHITE);
      unloading_function();
      
   //  runmilisec=runsec=runmin=runhour=0;
     // }
      
    }
    
    if((/*(dubcounts)==total_count && motorstop_flag!=1 &&*/ process2==1))
    {
    //  motorstart_flag=0; 
    //  timeprocess=0;
    dubcounts=0;
    calculation_flag=1;
      motorstart_flag=0;
      process2=0;
      if(save_calculation==1)
      {
        if(save_no_of_drums==1)
        {
          if(save_balance==1)
          {
            ///load cell///
          }
          
           if(save_balance==2)
          {
            tft.fillScreen(RA8875_WHITE);
            outtimecountweight_menu();
            display_menu();
            tx=ty=0;
            delay(700);
            while(outweightnext1_flag==0)
            {
              delay(400);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
            outtimecountweight_function();
  }
 
            }
            if(outweightnext1_flag==1)
            {
              tft.fillScreen(RA8875_WHITE);
            calculation_result();
            outweightnext1_flag=0;
             while(1)//resultnext_flag==0)
              {
                delay(1000);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
             calculation_function();
  }
          //  outweightnext1_flag=0;
          if(resultnext_flag==1)
          {
            tx=ty=0;
            //delay(500);
            resultnext_flag=0;
            tft.fillScreen(RA8875_WHITE);
            paratobe_print_menu();
            tx=ty=0;
            delay(700);
            while(paratobeprintnext_flag==0)
            {
              if(productback_flag==1)
              {
                tft.fillScreen(RA8875_WHITE);
              paratobe_print_menu();
              productback_flag=0;
              }
              delay(1000);
         if (! digitalRead(RA8875_INT))
          {
            tft.touchRead(&tx, &ty);
               paratobe_print_function();
          }
           historysave_function();
              if(productname_flag==1)
              {
                productname_flag=0;
                 tft.fillScreen(RA8875_WHITE);
                product_name_menu();
                keypad();
             // }
                while(productback_flag==0)
                {
                   //delay(300);
         if (! digitalRead(RA8875_INT))
          {
            tft.touchRead(&tx, &ty);
                  product_name_function();
                  keypad2();
          }
                  if( productenter_flag==1)
                  {
                    productenter_flag=0;
                    for(i=0; product_name[i]!='\0';i++)
                    {
                    save_product_name[i]= product_name[i];
                    }
                    product_name[0]='\0';
                  }
                }
              //  productback_flag=0;
              }
              if(batchno_flag==1)
              {
                batchno_flag=0;
                 tft.fillScreen(RA8875_WHITE);
                batch_number_menu();
                 keypad();
                while(productback_flag==0)
                {
                  // delay(300);
         if (! digitalRead(RA8875_INT))
          {
            tft.touchRead(&tx, &ty);
                  product_name_function();
                  keypad2();
          }
                  if( productenter_flag==1)
                  {
                    productenter_flag=0;
                    for(i=0; product_name[i]!='\0';i++)
                    {
                    save_batch_number[i]=product_name[i];
                    }
                    product_name[0]='\0';
                  }
                }
              //  productback_flag=0;
              }
              if(lotno_flag==1)
              {
                lotno_flag=0;
                 tft.fillScreen(RA8875_WHITE);
                lot_number_menu();
                keypad();
                while(productback_flag==0)
                {
                  // delay(300);
         if (! digitalRead(RA8875_INT))
          {
            tft.touchRead(&tx, &ty);
                  product_name_function();
                  keypad2();
          }
                  if( productenter_flag==1)
                  {
                    productenter_flag=0;
                    for(i=0; product_name[i]!='\0';i++)
                    {
                    save_lot_number[i]= product_name[i];
                    }
                    product_name[0]='\0';
                  }
                }
               // productback_flag=0;
              }
            }
          }
          if(paratobeprintnext_flag==1)
          {
            break;
          }
              }
              
            }
            outweightnext1_flag=0;
          }
        }
      }
      if(save_calculation==2)
      {
        calculation_flag=1;
      }
    }
 // }
           if(paratobeprintnext_flag==1)//resultnext_flag==1)
              {
                break;
              }
              if(motornext_flag==1)
              {
                 break;
              }
  }
    //motornext_flag=1;
  }
  //weightnext_flag=0;
          if(paratobeprintnext_flag==1)//resultnext_flag==1)
              {
                break;
              }
              if(motornext_flag==1)
              {
               // motornext_flag=0;
                 break;
              }
      }
     weightnext_flag=0;
    }
 // }
  }
  
  if(save_no_of_drums==2)
  {
    delay(500);
    tx=0,ty=0;
    if(save_balance==1)
    {
      
      while(weightback_flag==0)
      {
        ////load cell///
      }
      weightback_flag=0;
      
    }
    if(save_balance==2)
    {
      tx=ty=0;
     // delay(400);
      tft.fillScreen(RA8875_WHITE);
      timecountweight1_menu();
      display_menu();
      tx=ty=0;
      delay(700);
      while(weightback_flag==0)
      {
         if(weightback2_flag==1)
        {
          tft.fillScreen(RA8875_WHITE);
      timecountweight1_menu();
      display_menu();
      weightback2_flag=0;
        }
         delay(500);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
        timecountweight1_function();
  }
        if(weightnext_flag==1)
        {
          weightnext_flag=0;
          tft.fillScreen(RA8875_WHITE);
          timecountweight2_menu();
          display_menu();
          tx=ty=0;
          delay(700);
          while(weightback2_flag==0)
          {
           delay(500);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
            timecountweight2_function();
  }
  if(weightback2_flag==1)
 {
  break;
 }
 
  if(weightnext2_flag==1)
        {
          weightnext2_flag=0;
          ///store,start///
          tft.fillScreen(RA8875_WHITE);
            motor_menu();
       tft.textMode();
     tft.textSetCursor(220, 120);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite("Total Counts");

tft.fillRect(228,158,140,40,RA8875_WHITE);
  tft.textSetCursor(230, 160);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite("00");
  
  tft.graphicsMode();
  tx=ty=0;
  delay(700);
        //  motor_menu();
  while(motornext_flag==0)
  {
    if(/*motorstart_flag==1 && motorstop_flag==1 && process2==1*/calculation_flag==1)
    {
      motorstart_flag=0,motorstop_flag=0,process2=0,calculation_flag=0;
      dubcounts=0;
      tft.fillScreen(RA8875_WHITE); 
      motor_menu();
       tft.textMode();
     tft.textSetCursor(220, 120);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite("Total Counts");
  
tft.fillRect(228,158,140,40,RA8875_WHITE);
  tft.textSetCursor(230, 160);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite("00");
  
  tft.graphicsMode();
 // motor_menu();
 tx=ty=0;
          delay(700);
    }
    
    delay(150);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
    motor_function();
  }
    if(motorstart_flag==1)
    {
     //  motorstart_flag=0;
      motor_countstart_function();
       delay(500);
    //  tft.fillScreen(RA8875_WHITE);
      unloading_function();
    }
    if((/*motorstop_flag!=1 && dubcounts==total_count && */process2==1))
    {
      dubcounts=0,calculation_flag=1;
       motorstop_flag=0;
       process2=0;
       motorstart_flag=0;
     //  timeprocess=0;
      if(save_calculation==1)
      {
        /*if(save_no_of_drums==1)
        {
          if(save_balance==1)
          {
            ///load cell///
          }
          if(save_balance==2)
          {
            tft.fillScreen(RA8875_WHITE);
            outtimecountweight_menu();
            display_menu();
            while(outweightnext1_flag==0)
            {
              delay(150);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
            outtimecountweight_function();
  }
            }
            
           // calculation_result();
            outweightnext1_flag=0;
          }
        }
        */
        if(save_no_of_drums==2)
        {
          runsec=runmin=runhour=0;
          if(save_balance==1)
          {
            ///load cell///
          }
          if(save_balance==2)
          {
            tft.fillScreen(RA8875_WHITE);
            outtimecountweight_menu();
            display_menu();
            tx=ty=0;
            delay(700);
            while(outweightback1_flag==0)
            {
              if(outweightnext2_flag==1)
              {
                tft.fillScreen(RA8875_WHITE);
            outtimecountweight_menu();
            display_menu();
            outweightnext2_flag=0;
              }
              delay(500);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
            outtimecountweight_function();
  }

   if(outweightnext1_flag==1)
            {
              tx=ty=0;
              outweightnext1_flag=0;
              tft.fillScreen(RA8875_WHITE);
              outtimecountweight2_menu();
            display_menu();
            tx=ty=0;
            delay(700);
            while(outweightback2_flag==0)
            { 
              delay(500);
         if (! digitalRead(RA8875_INT))
          {
            tft.touchRead(&tx, &ty);
            outtimecountweight2_function();
             }
        if(outweightback2_flag==1)
        {
          break;
        }
            
            //outweightnext2_flag=0;
           // outweightnext1_flag=0;
            
            if(outweightnext2_flag==1)
            {
              outweightnext2_flag=0;
               tft.fillScreen(RA8875_RED);
            calculation_result();
            tx=ty=0;
            delay(700);
              while(1)//resultnext_flag==0)
              {
                delay(1000);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
             calculation_function();
  }
          //  outweightnext1_flag=0;
          if(resultnext_flag==1)
          {
            delay(700);
            tx=0,ty=0;
            resultnext_flag=0;
            tft.fillScreen(RA8875_WHITE);
            paratobe_print_menu();
            tx=ty=0;
            delay(700);
            while(paratobeprintnext_flag==0)
            {
              if(productback_flag==1)
              {
                tft.fillScreen(RA8875_WHITE);
              paratobe_print_menu();
              productback_flag=0;
              tx=ty=0;
              delay(700);
              }
              delay(400);
         if (! digitalRead(RA8875_INT))
          {
            tft.touchRead(&tx, &ty);
               paratobe_print_function();
          }
           historysave_function();
              if(productname_flag==1)
              {
                productname_flag=0;
                 tft.fillScreen(RA8875_WHITE);
                product_name_menu();
                keypad();
             // }
                while(productback_flag==0)
                {
                   //delay(400);
         if (! digitalRead(RA8875_INT))
          {
            tft.touchRead(&tx, &ty);
                  product_name_function();
                  keypad2();
          }
                  if( productenter_flag==1)
                  {
                    productenter_flag=0;
                    for(i=0; product_name[i]!='\0';i++)
                    {
                    save_product_name[i]= product_name[i];
                    }
                    product_name[0]='\0';
                  }
                }
               // productback_flag=0;
              }
              if(batchno_flag==1)
              {
                batchno_flag=0;
                 tft.fillScreen(RA8875_WHITE);
                batch_number_menu();
                 keypad();
                while(productback_flag==0)
                {
                   //delay(400);
         if (! digitalRead(RA8875_INT))
          {
            tft.touchRead(&tx, &ty);
                  product_name_function();
                  keypad2();
          }
                  if( productenter_flag==1)
                  {
                    productenter_flag=0;
                    for(i=0; product_name[i]!='\0';i++)
                    {
                    save_batch_number[i]=product_name[i];
                    }
                    product_name[0]='\0';
                  }
                }
              //  productback_flag=0;
              }
              if(lotno_flag==1)
              {
                lotno_flag=0;
                 tft.fillScreen(RA8875_WHITE);
                lot_number_menu();
                keypad();
                while(productback_flag==0)
                {
                  // delay(400);
         if (! digitalRead(RA8875_INT))
          {
            tft.touchRead(&tx, &ty);
                  product_name_function();
                  keypad2();
          }
                  if( productenter_flag==1)
                  {
                    productenter_flag=0;
                    for(i=0; product_name[i]!='\0';i++)
                    {
                    save_lot_number[i]= product_name[i];
                    }
                    product_name[0]='\0';
                  }
                }
              //  productback_flag=0;
              }
            }
          }
          if(paratobeprintnext_flag==1)
          {
            break;
          }
              }
             /* if(resultnext_flag==1)
              {
                break;
              }*/
            }
             if(paratobeprintnext_flag==1)//resultnext_flag==1)
              {
                break;
              }
            }
            }
             if(paratobeprintnext_flag==1)//resultnext_flag==1)
              {
                break;
              }
            }
          
          }
          }
          if(save_calculation==2)
          {
            calculation_flag=1;
          }
        }
     // }
      
  //  }
    if(motornext_flag==1)
          {
             break;
          }
  }
          if(paratobeprintnext_flag==1)//resultnext_flag==1)
              {
                break;
              }
   // motorstart_flag=0;
  }
  //weightnext_flag=0;
  
 /* if(motornext_flag==1)
  {
    break;
  }
  */
 /////////////// 
 /// if(save_calculation==1)
 // {
    
 // }

        }
        if(paratobeprintnext_flag==1)//resultnext_flag==1)
              {
                break;
              }
               if(motornext_flag==1)
              {
                 break;
              }
          }
         
          
        }
         if(weightback_flag==1)
       {
        break;
       }
        if(paratobeprintnext_flag==1)//resultnext_flag==1)
              {
                break;
              }
              if(motornext_flag==1)
              {
                 break;
              }
      }
      weightback_flag=0;
    }
 }
 
      }
       if(paratobeprintnext_flag==1)//resultnext_flag==1)
              {
                paratobeprintnext_flag=0;//resultnext_flag=0;
                break;
              }
              if(motornext_flag==1)
              {
                motornext_flag=0;
                 break;
              }
      }
    home_flag=1;
  countnextflag=0;
  countbackflag=0;
  paratobeprintnext_flag=0;//resultnext_flag=0;
  motornext_flag=0;
  }

  ///settings///
  if(tx>424 && tx<667 && ty>218 && ty<333)
  {
    
   tft.fillScreen(0xffff);
   // settings_menu();
    display_menu();
    settings_menu();
    while(settingsbackflag==0 || settingsnextflag==0)
    {
      delay(400);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
    settings_keyfunction();
   settings_menu_function();
   
   // settings_keyfunction();
  }
        if(settingsbackflag==1 || settingsnextflag==1)
        {
          break;
        }
    
  }
  home_flag=1;
  settingsnextflag=0;
  settingsbackflag=0;
  }

  //////////////////////////madmin//////////////////////////
 // if(tx>177 && tx<339 && ty>479 && ty<781)
 if(tx>177 && tx<339 && ty>593 && ty<808)
  { 
     madmin_menu();
     while(madminbackflag==0)
     {
      
      if(adminback_flag==1)
      {
      
       tft.fillScreen(0xffff);
        madmin_menu();
        adminback_flag=0;
        adminflag=0;
        madminbackflag=0;
        maintainanceflag=0;
      //  maintenanceenter=0;
      }
      if(maintenenceback==1)
      {
        //delay(500);
        tft.fillScreen(0xffff);
        madmin_menu();
        maintenenceback=0;
        sr_flag=0;
        rstpwd_flag=0;
        balpara_flag=0;
        printpara_flag=0;
        delay(900);
      }
      delay(150);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
    madmin_menu_function();
  }
    
  ///admin///
    if(adminflag==1 && madminbackflag==1 && maintainanceflag==0)// && tx>730 && tx<869 && ty>383 && ty<680)
    {
      tft.fillScreen(0xffff);
      adminpass_menu();
      keypad();
      while(adminback_flag==0)
      {
        if(adminpage_flag==1)
        {
          tft.fillScreen(0xffff);
      adminpass_menu();
      keypad();
          adminpage_flag=0;
         // insettingsback_flag==1
         adminback_flag=0;
         // adminsettings_flag==1
          adminenter_flag=0;
        }
       /* if(wrongpass==1)
        {tft.fillScreen(0xffff);
      adminpass_menu();
      keypad();
          
        }*/
        //delay(400);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
      adminpass_function();
      keypad2();
     // login_function();
  }

 if(adminenter_flag==1)
 {
  Serial.print("enter");
   for(i=50,j=0;EEPROM.read(i)!='\0' && i<59;i++,j++)
 {
 adminpasswordcheck[j]=EEPROM.read(i);
 
// Serial.print(password[j]);

 //Serial.print(userenter[j]);
 }
 adminenter_flag=0;
/* for(j=0;loginuser[j]!='\0';j++)
 {
  admincheck[j]=loginuser[j];
  Serial.print(admincheck[j]);
 }*/
 i=j=0;
// if(userenter[0]==password[0] && userenter[1]==password[1] && userenter[2]==password[2] && userenter[3]==password[3] && userenter[4]==fixpassword[4] && userenter[5]==fixpassword[5] && userenter[6]==fixpassword[6] && userenter[7]==fixpassword[7])
if(strlen(adminpass)==strlen(adminpasswordcheck))
{
 if(strncmp(adminpass,adminpasswordcheck,strlen(adminpasswordcheck))==0)
  {
    adminpass[0]='\0';
    adminuser[0]='\0';
    tft.fillScreen(0xffff);
 adminpage_menu();

while(adminpage_flag==0)
{ 
  ///reverse settings//
  if(insettingsback_flag==1)
  {
    tft.fillScreen(0xffff);
    adminpage_menu();
    insettingsback_flag=0;
    adminsettings_flag=0;
    adminconfig_flag=0;
  }
  
  ///reverse config//
   if(configback_flag==1)
  {
    tft.fillScreen(0xffff);
    adminpage_menu();
    configback_flag=0;
    adminsettings_flag=0;
    adminconfig_flag=0;
  }

  
  
  delay(150);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
  adminpage_menu_function();
  }

  //settings///
if(adminsettings_flag==1)
{
admin_settings_menu();
while(insettingsback_flag==0)
{
  if(asettingsmenu2next_flag==1)
  {
    tft.fillScreen(0xffff);
    admin_settings_menu();
    asettingsmenu2next_flag=0;
    insettingsback_flag=0;
    insettingsnext_flag=0;
   /* asettingssetdandt_flag=0;
    asettingsinstrumentid_flag=0;
    asettingscompanyname_flag=0;
    asettingsmenu2next_flag=0;*/
  }
   delay(150);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
admin_settings_menu_function();
  }
  if(insettingsnext_flag==1)
  {
    tft.fillScreen(0xffff);
    adminsettingsmenu2();
    
    while(asettingsmenu2next_flag==0)
    {
      if(companyback_flag==1 || rtcdateback_flag==1 || instrumentback_flag==1)
      {
        
        tft.fillScreen(0xffff);
        adminsettingsmenu2();
       companyback_flag=rtcdateback_flag=instrumentback_flag=0;
        
      }
      delay(400);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
      adminsettingsmenu2_function();
  }
      if(asettingssetdandt_flag==1)
      {
        asettingssetdandt_flag=0;
        tft.fillScreen(0xffff);
        rtc_settings_parameters1_menu();
        display_menu();
        while(rtcdateback_flag==0)
        {
          if(rtctimeback_flag==1)
          {
        rtctimeback_flag=0;
        tft.fillScreen(0xffff);
        rtc_settings_parameters1_menu();
        display_menu();
          }
          delay(500);
      if (! digitalRead(RA8875_INT))
  {
  if (tft.touched())
    {
    tft.touchRead(&tx, &ty);
     Serial.print(tx); Serial.print(", "); Serial.println(ty);
          rtc_settings_parameters1_menu_function();
    }   
  }
        
        if(rtcdatenext_flag==1)
        {
          rtcdatenext_flag=0;
          //rtc.adjust(DateTime(ay, am, ad, ah, ami , as));
           tft.fillScreen(0xffff);
        rtc_settings_parameters2_menu();
        display_menu();
        while(rtctimeback_flag==0)
        {
           delay(500);
      if (! digitalRead(RA8875_INT))
  {
    if (tft.touched())
    {
    tft.touchRead(&tx, &ty);
     Serial.print(tx); Serial.print(", "); Serial.println(ty);
          rtc_settings_parameters2_menu_function();
    }
  }
        }
        }
        }
        
      }
    /*  if(asettingsinstrumentid_flag==1)
      {
        asettingsinstrumentid_flag=0;
        tft.fillScreen(0xffff);
         settings_instrumentid_menu();
         keypad();
         while(instrumentback_flag==0)
         {
           delay(150);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
//          settings_insturmentid_menu_function();
  //        keypad2();
  }       
         }
         //   instrumentback_flag=0;
      }*/
      if(asettingscompanyname_flag==1)
      { 
        asettingscompanyname_flag=0;
        tft.fillScreen(0xffff);
         settings_companyname_menu();
        keypad();
        while(companyback_flag==0)
        {
           delay(150);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
          settings_companyname_menu_function();
          keypad2();
  }
        }
       // companyback_flag=0;
      } 
    }
    
      
    }
  }  
}

//configuration///
if(adminconfig_flag==1)
{
  tx=ty=0;
  adminconfig_flag=0;
  delay(400);
  configuration_menu();
  while(configback_flag==0)
  {
    if(userandpasschangeback_flag==1)
    {
      tx=ty=0;
      delay(300);
      userandpasschangeback_flag=0;
       configuration_menu();
       
    }
    
     delay(150);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
    configuration_menu_function();
  }
   ////////////////////////////admin user and passs//////////// 
    if(adminandpass==1)
    {
      adminandpass=0;
       tx=ty=0;
      tft.fillScreen(RA8875_WHITE);
      userandpasschange_menu();
      keypad();
      delay(300);
     
      while(userandpasschangeback_flag==0)
      {
        if(userandpasschangeback2_flag==1)
        {
          userandpasschangeback2_flag=0;
           tft.fillScreen(RA8875_WHITE);
      userandpasschange_menu();
      keypad();
        }
        delay(400);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
        userandpasschange_function();
        keypad2();
  }
        if(userandpasschangeenter_flag==1)
        {
          userandpasschangeenter_flag=0;
          Serial.print("inp");
            p2=p2+30;
          Serial.print(p2);
          EEPROM.write(p2,'\0');
       //   i=i+30;
         // EEPROM.write(i,'\0');
          ///user1///
 for(i=30;newu1reenteredpass[i-30]!='\0' && i<49;i++)
  {
EEPROM.write(i,newu1reenteredpass[i-30]);
Serial.print("eew=");
Serial.print(EEPROM.read(i));
Serial.print("\n");
  } 
  i=0;
  newu1reenteredpass[0]='\0';
        }
        if(userandpasschangenext_flag==1)
        {
          userandpasschangenext_flag=0;
          tft.fillScreen(RA8875_WHITE);
      userandpasschange2_menu();
     keypad();
      while(userandpasschangeback2_flag==0)
      {
        delay(400);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
        userandpasschange2_function();
         keypad2();
  }
        if(userandpasschangeenter2_flag==1)
        {
          userandpasschangeenter2_flag=0;
          Serial.print(i);
          p2=p2+50;
          Serial.print(p2);
          EEPROM.write(p2,'\0');
         
          ///pass1///
  for(i=50;newreenteredpass[i-50]!='\0' && i<59;i++)
  {
EEPROM.write(i,newreenteredpass[i-50]);/////////////////////////////adminpassword///////////////
//Serial.print(defaultpass[i]);
  }
  for(i=50;i<=59;i++)
  {
    Serial.print("\npass=");
Serial.print(EEPROM.read(i));/////////////////////////////adminpassword///////////////
//Serial.print(defaultpass[i]);
  }
  i=0;
  newreenteredpass[0]='\0';    
        }
      }
          
        }
      }
      //userandpasschangeback_flag=0;
    
     /* adminoldpass_edit_menu();
      while(adminoldback_flag==0)
      {
        delay(150);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
        adminoldpass_edit_function();
  }
        if(adminoldedit_flag==1)
        {
          adminoldedit_flag=0;
          adminnewpass_edit_menu();
          while(adminnewback_flag==0)
          {
            delay(150);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
    adminnewpass_edit_function(); 
  }
            if(adminnewname==1 || namebreak_flag==1)
            {
              while(userandpasschangeback_flag==0)
              {
              
                      userandpasschange_function();
                      if(tx>360 && tx<683 && ty>552 && ty<589)
                      {
                        passbreak_flag=1;
                        i=0;
                        break;
                      }
                      if(passchangesave==1)
                      {
                        
                      }
              }
              }

            if(adminnewpass==1 || passbreak_flag==1)
             {
               while(userandpasschangeback_flag==0)
              {
                      userandpasschange_function();

                      if(tx>352 && tx<682 && ty>652 && ty<696)
                      {
                        namebreak_flag=1;
                        i=0;
                        break;
                      }
                       if(passchangesave==1)
                      {
                        
                      }
                      
              }
             }
          }
        }
      }*/
    }
    if(user1andpass==1)
    { /////////user1//////
      Serial.print("inp2");
      tx=ty=0;
      user1andpass=0;
      tft.fillScreen(RA8875_WHITE);
      userandpasschange_menu();
      keypad();
      delay(400);
      while(userandpasschangeback_flag==0)
      {
        if(userandpasschangeback2_flag==1)
        {
          userandpasschangeback2_flag=0;
           tft.fillScreen(RA8875_WHITE);
      userandpasschange_menu();
      keypad();
        }
        delay(400);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
        userandpasschange_function();
        keypad2();
  }
        if(userandpasschangeenter_flag==1)
        {
          userandpasschangeenter_flag=0;
        //  Serial.print("inp");
            p2=p2+60;
          Serial.print(p2);
          EEPROM.write(p2,'\0');
          ///user1///
 for(i=60;newu1reenteredpass[i-60]!='\0' && i<79;i++)
  {
EEPROM.write(i,newu1reenteredpass[i-60]);
Serial.print("eew=");
Serial.print(EEPROM.read(i));
Serial.print("\n");
  } 
  i=0;
  newu1reenteredpass[0]='\0';
        }
        if(userandpasschangenext_flag==1)
        {
          userandpasschangenext_flag=0;
          tft.fillScreen(RA8875_WHITE);
      userandpasschange2_menu();
     keypad();
      while(userandpasschangeback2_flag==0)
      {
        delay(400);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
        userandpasschange2_function();
        keypad2();
  }
        if(userandpasschangeenter2_flag==1)
        {
          userandpasschangeenter2_flag=0;
            p2=p2+80;
          Serial.print(p2);
          EEPROM.write(p2,'\0');
          ///pass1///
  for(i=80;newreenteredpass[i-80]!='\0' && i<89;i++)
  {
EEPROM.write(i,newreenteredpass[i-80]);/////////////////////////////user1password///////////////
Serial.print(EEPROM.read(i));
  }
  i=0;
  newreenteredpass[0]='\0';    
        }
      }
          
        }
      }
      //userandpasschangeback_flag=0;
    }
    if(user2andpass==1)
    {////////////////user2////////////////
      user2andpass=0;
      tft.fillScreen(RA8875_WHITE);
      userandpasschange_menu();
      keypad();
      while(userandpasschangeback_flag==0)
      {
        if(userandpasschangeback2_flag==1)
        {
          userandpasschangeback2_flag=0;
           tft.fillScreen(RA8875_WHITE);
      userandpasschange_menu();
      keypad();
        }
        delay(400);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
        userandpasschange_function();
        keypad2();
  }
        if(userandpasschangeenter_flag==1)
        {
          userandpasschangeenter_flag=0;
          Serial.print("inp");
            p2=p2+90;
          Serial.print(p2);
          EEPROM.write(p2,'\0');
          ///user2///
 for(i=90;newu1reenteredpass[i-90]!='\0' && i<109;i++)
  {
EEPROM.write(i,newu1reenteredpass[i-90]);/////////////////////////////username2//////////////
Serial.print("eew=");
Serial.print(EEPROM.read(i));
Serial.print("\n");
  } 
  i=0;
  newu1reenteredpass[0]='\0';
        }
        if(userandpasschangenext_flag==1)
        {
          userandpasschangenext_flag=0;
          tft.fillScreen(RA8875_WHITE);
      userandpasschange2_menu();
     keypad();
      while(userandpasschangeback2_flag==0)
      {
        delay(400);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
        userandpasschange2_function();
        keypad2();
  }
        if(userandpasschangeenter2_flag==1)
        {
          userandpasschangeenter2_flag=0;
            p2=p2+110;
          Serial.print(p2);
          EEPROM.write(p2,'\0');
          ///pass2///
  for(i=110;newreenteredpass[i-110]!='\0' && i<119;i++)
  {
EEPROM.write(i,newreenteredpass[i-110]);///////////////////userpassword2///////////////
//Serial.print(defaultpass[i]);
  }
  i=0;
  newreenteredpass[0]='\0';    
        }
      }
          
        }
      }
     // userandpasschangeback_flag=0;
    }
    if(user3andpass==1)
    {//////////////////user3//////////////////
      user3andpass=0;
      tft.fillScreen(RA8875_WHITE);
      userandpasschange_menu();
      keypad();
      while(userandpasschangeback_flag==0)
      {
        if(userandpasschangeback2_flag==1)
        {
          userandpasschangeback2_flag=0;
           tft.fillScreen(RA8875_WHITE);
      userandpasschange_menu();
      keypad();
        }
        delay(400);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
        userandpasschange_function();
        keypad2();
  }
        if(userandpasschangeenter_flag==1)
        {
          userandpasschangeenter_flag=0;
          Serial.print("inp");
            p2=p2+120;
          Serial.print(p2);
          EEPROM.write(p2,'\0');
          ///user3///
 for(i=120;newu1reenteredpass[i-120]!='\0' && i<129;i++)
  {
EEPROM.write(i,newu1reenteredpass[i-120]);///////////////////////username3//////////////////
Serial.print("eew=");
Serial.print(EEPROM.read(i));
Serial.print("\n");
  } 
  i=0;
  newu1reenteredpass[0]='\0';
        }
        if(userandpasschangenext_flag==1)
        {
          userandpasschangenext_flag=0;
          tft.fillScreen(RA8875_WHITE);
      userandpasschange2_menu();
     keypad();
      while(userandpasschangeback2_flag==0)
      {
        delay(400);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
        userandpasschange2_function();
        keypad2();
  }
        if(userandpasschangeenter2_flag==1)
        {
          userandpasschangeenter2_flag=0;
            p2=p2+130;
          Serial.print(p2);
          EEPROM.write(p2,'\0');
          ///pass3///
  for(i=130;newreenteredpass[i-130]!='\0' && i<139;i++)
  {
EEPROM.write(i,newreenteredpass[i-130]);//////////////////userpassword3////////////////////
//Serial.print(defaultpass[i]);
  }
  i=0;
  newreenteredpass[0]='\0';    
        }
      }
          
        }
      }
     // userandpasschangeback_flag=0;
    }
    if(user4andpass==1)
    {////////////////user4/////////////////
      user4andpass=0;
      tft.fillScreen(RA8875_WHITE);
      userandpasschange_menu();
      keypad();
      while(userandpasschangeback_flag==0)
      {
        if(userandpasschangeback2_flag==1)
        {
          userandpasschangeback2_flag=0;
           tft.fillScreen(RA8875_WHITE);
      userandpasschange_menu();
      keypad();
        }
        delay(400);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
        userandpasschange_function();
        keypad2();
  }
        if(userandpasschangeenter_flag==1)
        {
          userandpasschangeenter_flag=0;
          Serial.print("inp");
            p2=p2+140;
          Serial.print(p2);
          EEPROM.write(p2,'\0');
          ///user4///
 for(i=140;newu1reenteredpass[i-140]!='\0' && i<159;i++)
  {
EEPROM.write(i,newu1reenteredpass[i-140]);/////////////////username4/////////////////
Serial.print("eew=");
Serial.print(EEPROM.read(i));
Serial.print("\n");
  } 
  i=0;
  newu1reenteredpass[0]='\0';
        }
        if(userandpasschangenext_flag==1)
        {
          userandpasschangenext_flag=0;
          tft.fillScreen(RA8875_WHITE);
      userandpasschange2_menu();
     keypad();
      while(userandpasschangeback2_flag==0)
      {
        delay(400);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
        userandpasschange2_function();
        keypad2();
  }
        if(userandpasschangeenter2_flag==1)
        {
          userandpasschangeenter2_flag=0;
            p2=p2+160;
          Serial.print(p2);
          EEPROM.write(p2,'\0');
          ///pass5///
  for(i=160;newreenteredpass[i-160]!='\0' && i<169;i++)
  {
EEPROM.write(i,newreenteredpass[i-160]);////////////////userpassword4////////////
//Serial.print(defaultpass[i]);
  }
  i=0;
  newreenteredpass[0]='\0';    
        }
      }
          
        }
      }
     // userandpasschangeback_flag=0;
    }
    if(user5andpass==1)
    {/////////////user5//////////////////
      user5andpass=0;
      tft.fillScreen(RA8875_WHITE);
      userandpasschange_menu();
      keypad();
      while(userandpasschangeback_flag==0)
      {
        if(userandpasschangeback2_flag==1)
        {
          userandpasschangeback2_flag=0;
           tft.fillScreen(RA8875_WHITE);
      userandpasschange_menu();
      keypad();
        }
        delay(400);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
        userandpasschange_function();
        keypad2();
  }
        if(userandpasschangeenter_flag==1)
        {
          userandpasschangeenter_flag=0;
          Serial.print("inp");
            p2=p2+170;
          Serial.print(p2);
          EEPROM.write(p2,'\0');
          ///user5///
 for(i=170;newu1reenteredpass[i-170]!='\0' && i<189;i++)
  {
EEPROM.write(i,newu1reenteredpass[i-170]);///////////////////username5/////////////
Serial.print("eew=");
Serial.print(EEPROM.read(i));
Serial.print("\n");
  } 
  i=0;
  newu1reenteredpass[0]='\0';
        }
        if(userandpasschangenext_flag==1)
        {
          userandpasschangenext_flag=0;
          tft.fillScreen(RA8875_WHITE);
      userandpasschange2_menu();
     keypad();
      while(userandpasschangeback2_flag==0)
      {
        delay(400);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
        userandpasschange2_function();
        keypad2();
  }
        if(userandpasschangeenter2_flag==1)
        {
          userandpasschangeenter2_flag=0;
            p2=p2+190;
          Serial.print(p2);
          EEPROM.write(p2,'\0');
          ///pass5///
  for(i=190;newreenteredpass[i-190]!='\0' && i<199;i++)
  {
EEPROM.write(i,newreenteredpass[i-190]);////////////////userpassword5///////////////
//Serial.print(defaultpass[i]);
  }
  i=0;
  newreenteredpass[0]='\0';    
        }
      }
          
        }
      }
     // userandpasschangeback_flag=0;
    }
  }
 // configback_flag=0;
}

}
//adminpage_flag=0;
 }
  
  else
  {
    tft.textMode();

  tft.textSetCursor(90,55);
   tft.textTransparent(RA8875_RED);
  tft.textEnlarge(0);
  tft.textWrite("Error");
  //tft.drawRect(140,100,180,25,RA8875_BLACK);
   wrongpass=1;
  tft.graphicsMode();
   adminpass[0]='\0';
    adminuser[0]='\0';
  }
  }
  else
  {
    tft.textMode();

  tft.textSetCursor(90,55);
   tft.textTransparent(RA8875_RED);
  tft.textEnlarge(0);
  tft.textWrite("Error");
  //tft.drawRect(140,100,180,25,RA8875_BLACK);
  // wrongpass=1;
  tft.graphicsMode();
   adminpass[0]='\0';
    adminuser[0]='\0';
  }
 }
 }
      maintainanceflag=0;
      adminflag=0;
      madminbackflag=0;
      
    }
     
    ///maintaninance//
    if(maintainanceflag==1 && adminflag==0 && madminbackflag==1)// && tx>263 && tx<416 && ty>411 && ty<665)
    {
      tft.fillScreen(RA8875_WHITE);
      maintenancepass_menu();
      keypad();
      while(maintenenceback==0)
        {
          if(maintanancenext_flag==1)
          {
            tft.fillScreen(RA8875_WHITE);
            maintenancepass_menu();
      keypad();
      maintanancenext_flag=0;
      maintenanceenter=0;
          }
         
        
         // delay(150);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
    maintanencepass_menu_function();
    keypad2();
  }
  delay(200);
  if(maintenanceenter==1)
  {
  for(i=19,j=0;EEPROM.read(i)!='\0' && i<29;i++,j++)
 {
 defaultusercheck[j]=EEPROM.read(i);
 }
 defaultusercheck[j]=0;
  i=j=0;
 if((strlen(maintainence_pass)==strlen(defaultusercheck)) || (maintenance_lockopen==1))
{
  if((strncmp(maintainence_pass,defaultusercheck,strlen(defaultusercheck))==0) || (maintenance_lockopen==1))
  {
    tft.fillScreen(RA8875_WHITE);
    maintanance_menu();
    while(maintanancenext_flag==0)
    {
       if(sr_flag==1 || rstpwd_flag==1 || printpara_flag==1)
           {
                    tft.fillScreen(RA8875_WHITE);
                    maintanance_menu();
                    
                    sr_flag=0;
                    rstpwd_flag=0;
                    printpara_flag=0;
             }
     // delay(400);
      if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);
    maintanance_menu_function();
  }
      
          if(sr_flag==1)
          {
            tft.fillScreen(RA8875_WHITE);
            maintanance_srno_menu();
            keypad();
             while(serialnonext_flag==0)
            {
              //delay(150);
          if(! digitalRead(RA8875_INT))
          {
            
          tft.touchRead(&tx, &ty);
              maintanance_srno_function();
              keypad2();
          }
            }
            serialnonext_flag=0;
          }
          if(rstpwd_flag==1)
          {

            maintanance_rstpass_menu_function();
            
          }
          if(balpara_flag==1)
          {

            
          }
          if(printpara_flag==1)
          {

            
          }
        }
        }
       else
  {
    tft.textMode();

  tft.textSetCursor(90,55);
   tft.textTransparent(RA8875_RED);
  tft.textEnlarge(0);
  tft.textWrite("Error");
  //tft.drawRect(140,100,180,25,RA8875_BLACK);
   wrongpass=1;
  tft.graphicsMode();
   defaultusercheck[0]='\0';
   maintainence_pass[0]='\0';
   
  }
   
  }
  else
  {
    tft.textMode();

  tft.textSetCursor(90,55);
   tft.textTransparent(RA8875_RED);
  tft.textEnlarge(0);
  tft.textWrite("Error");
  //tft.drawRect(140,100,180,25,RA8875_BLACK);
  // wrongpass=1;
  tft.graphicsMode();
  defaultusercheck[0]='\0';
   maintainence_pass[0]='\0'; 
  }
  }
        }
      //  maintenanceenter=0;
       }
      
      maintainanceflag=0;
      adminflag=0;
     // madminbackflag=0;
      
    }
    
 /*   ///back///
    if(maintainanceflag==0 && adminflag==0 && madminbackflag==1)// && tx>885 && tx<951 && ty>731 && ty<796)
    {
      Serial.print("back");
      
      home_flag=1;
      
      maintainanceflag=0;
      adminflag=0;
      madminbackflag=0;
      
    }*/
     
    // }
     home_flag=1;
      
      maintainanceflag=0;
      adminflag=0;
      madminbackflag=0;
      
  }

  ///history///
 // if(tx>159 && tx<316 && ty>225 && ty<393)
 if(tx>159 && tx<316 && ty>407 && ty<593)
  {
    tft.fillScreen(0X379F);
    history1_menu();
    historyshow_menu();
    while(historyback_flag==0)
    {
      if(historyback2_flag==1)
      {
        tft.fillScreen(0X379F);
    history1_menu();
    historyshow_menu();
    historyback2_flag=0;
      }
      delay(500); 
       if (tft.touched())
    {
      Serial.print("Touch: ");
      tft.touchRead(&tx, &ty);
      Serial.print(tx); Serial.print(", "); Serial.println(ty);
      history_function();
      printout();
    }
      if(historynext_flag==1)
      {
        historynext_flag=0;
         tft.fillScreen(0X379F);
          history2_menu();
          historyshow2_menu();
       while(historyback2_flag==0)
       {
           delay(500); 
       if (tft.touched())
    {
      Serial.print("Touch: ");
       Serial.print(tx); Serial.print(", "); Serial.println(ty);
      tft.touchRead(&tx, &ty);
      history2_function();
      printout();
    }
    
       }
        
      }
   /*   if(historyback_flag==1)
      {
        break;
      }
    */
    }
     home_flag=1;
     historyback_flag=0;
     
  }
// >> home_page }

  /*  

    /////settings////
   if(login==1 && tx>718 && tx<923 && ty>256 && ty<313)
   {
     
   }


   //////Admin/////
   if(login==1       )
   {
    
   }
    
   // login_page();
    //Serial.print("inloop");
/*
    ///select mode area///
  if(page==0 && tx>393 && tx<627 && ty>371 && ty<658)
  {
     tft.fillScreen(0X0A0F);//to clear screen
     
     tft.textMode();
    // tft.cursorBlink(32);
   
    ////SELECT TIME or////
  tft.drawRect(160, 60, 150, 50, RA8875_WHITE);
  tft.textSetCursor(180, 65); ///text location////
  tft.textTransparent(RA8875_GREEN);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("TIME");

   ///select COUNT or///
  tft.fillRoundRect(160, 150, 150, 50, 10, RA8875_RED);
  tft.textSetCursor(165, 155); ///text location////
  tft.textTransparent(RA8875_GREEN);
  tft.textEnlarge(2); ////text size////
  tft.textWrite("Counts");

   tft.graphicsMode();///to enable touch and disable textmode else touch willnot work///

   pass=3;
   page=1;
   pres=0;
   tx=0;
   ty=0;

while(pres==0)
{
  if (! digitalRead(RA8875_INT))
  {
   delay(300); 
    tft.touchRead(&tx, &ty);
    
    //selected mode is time///
 if(page==1 && pass==3 && tx>412 && tx<668 && ty>602 && ty<725)
 {
 select_mode=1;
 tft.fillScreen(RA8875_BLACK);//to clear screen,9410,3410
 time_disp();
 display_menu();
 time_function(); 
 pres=1;
 tx=0;
 ty=0;
 tft.graphicsMode();///to enable touch and disable textmode else touch willnot work///
 }

    //selected mode is count///
 if(page==1 && pass==3 && tx>414 && tx<674 && ty>325 && ty<437)
  {
  select_mode=2;
  tft.fillScreen(RA8875_WHITE);//to clear screen
  
  counts_disp();
  display_menu();
  counts_function();
  pres=1;
  tx=0;
  ty=0;
  tft.graphicsMode();///to enable touch and disable textmode else touch willnot work///
  }
  }
}
pres=0;
  }

   //admin area///
  if(page==0 && tx>90  && tx<350  && ty>720  && ty<930)
  {
    Serial.print("bw");
    page=1;
    for(j=0;j<=3;j++)
 {
 password[j]=EEPROM.read(j);
 }
    mainpass_page();
    Serial.print("bw2");
    while(flag==0)
    {
      //Serial.print("w");
     //if (! digitalRead(RA8875_INT))
 // {
  // delay(300); 
  //  tft.touchRead(&tx, &ty);
    password_function();
/*      if(page==1 && userenter[0]==fixpassword[0] && userenter[1]==fixpassword[1] && userenter[2]==fixpassword[2] && userenter[3]==fixpassword[3])
    {
      Serial.print("tr");
    admin_disp();
    pass=4;
    page=2;
    flag=1;
    tx=0;
    ty=0;
    }

    if(userenter[0]!=fixpassword[0] || userenter[1]!=fixpassword[1] || userenter[2]!=fixpassword[2] ||  userenter[3]!=fixpassword[3])
    {
      Serial.print("fa");
     mainpass_page();
     //tft.fillRect(57, 120, 150, 58, RA8875_RED);
     tft.textMode();
     //tft.cursorBlink(32);
     tft.textSetCursor(10, 200); ///text location////
     tft.textTransparent(RA8875_GREEN);
     tft.textEnlarge(2);  ////text size////
     tft.textWrite("Error");
     flag==0;
     tft.graphicsMode();///to enable touch and disable textmode else touch willnot work///
      
    }
    
 */

 /*    if(page==1 && userenter[0]==password[0] && userenter[1]==password[1] && userenter[2]==password[2] && userenter[3]==password[3])
    {
      Serial.print("tr");
    admin_disp();
    pass=4;
    page=2;
    flag=1;
    tx=0;
    ty=0;
    }
    else //if(page==1 && userenter[0]!=password[0] && userenter[1]!=password[1] && userenter[2]!=password[2] &&  userenter[3]!=password[3])
    {
      Serial.print("fa");
     mainpass_page();
     //tft.fillRect(57, 120, 150, 58, RA8875_RED);
     tft.textMode();
     //tft.cursorBlink(32);
     tft.textSetCursor(10, 200); ///text location////
     tft.textTransparent(RA8875_GREEN);
     tft.textEnlarge(2);  ////text size////
     tft.textWrite("Error");
     flag==0;
     tft.graphicsMode();///to enable touch and disable textmode else touch willnot work///
      
    }
   // tx=0;
   // ty=0;
// }
    }
   // tx=0;
   // ty=0;
  
  flag=0;

while(pres==0)
{
if (! digitalRead(RA8875_INT))
  {
   delay(300); 
    tft.touchRead(&tx, &ty);

  //to set date and time//
  
  if(page==2 && pass==4 && tx>240 && ty<779 && ty>587 && ty<694)
  {
    tft.fillScreen(RA8875_BLACK);//to clear screen
    password_function();
    pres=1;
    tx=0;
    ty=0;
  }

   //to set password//
  if(page==2 && pass==4 && tx>251 && ty<777 && ty>293 && ty<429)
  {
    tft.fillScreen(RA8875_BLACK);//to clear screen
    pass_disp();
    display_menu();
    password_function();
    j=0;
    while(pres==0)
    {
      for(j=0;j<=3;j++)
      {
        
    password[j]=reenteredpass[j]; 
    EEPROM.write(j, password[j]);
    //addr=addr+1;
    //Serial.print(password[j]);
    
    }
      pres=1;
    }
    tx=0;
    ty=0;
  }
}
}
  }
pres=0;
}
for(j=0;j<=3;j++)
      {
        
   // password[j]//=reenteredpass[j];
    Serial.print(password[j]);
*/
}
}
      }

}
/*
void next()
{
  i2=0;

  tft.graphicsMode();
  if (! digitalRead(RA8875_INT))
  {
    delay(150); 
    tft.touchRead(&tx, &ty);
  //next//
   if(tx>88 && tx<318 && ty>144 && ty<236)
    {
     // tft.fillRoundRect(355, 215, 120, 50, 10, RA8875_BLACK);
     // tft.fillRoundRect(355, 215, 120, 50, 10, RA8875_RED);
      //tft.textTransparent(RA8875_GREEN);
     /* tft.textMode();
      tft.cursorBlink(32);
      tft.textTransparent(RA8875_GREEN);
  
      tft.textEnlarge(2);  ////text size////
      tft.textWrite(stringnext);
      tft.graphicsMode();
      
    
    tft.graphicsMode();
    cursorx=70;
    i=0;
    i2=6;
    flag=1;  
    Serial.print("ne");
    }
  }
}

void mainpass_page()
{
   tft.fillScreen(RA8875_BLACK);//to clear screen
    
      tft.textMode();
  //tft.cursorBlink(32);
  
  ///* Set the cursor location (in pixels) 
  tft.textSetCursor(10, 10);
 
 // /* Render some text! 
  
  tft.textTransparent(RA8875_RED);
  tft.textEnlarge(2);
  tft.textWrite("Please Enter");
 
  tft.textSetCursor(30, 50); ///text location////
  tft.textTransparent(RA8875_RED);
  tft.textEnlarge(2); ////text size////
  tft.textWrite("Password");

  ////entering reading value/////
  tft.drawRoundRect(50, 120, 210, 80, 10, RA8875_CYAN);
  tft.textSetCursor(155, 150); ///text location////
  tft.textTransparent(RA8875_GREEN);
  tft.textEnlarge(1); ////text size////

  tft.graphicsMode();

display_menu();
Serial.print("adm");
//password_function();
//Serial.print("apf");

  
}*/

void homestatus_menu()
{
  tft.textMode();
   tft.fillRect(30,100,55,20,RA8875_YELLOW);
 tft.drawRect(30,100,55,20,RA8875_BLACK);
 tft.textSetCursor(34, 100);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0);
  tft.textWrite("STATUS");
   tft.fillRoundRect(30,125,55,20,3,RA8875_CYAN);
 tft.drawRoundRect(30,125,55,20,3,RA8875_BLACK);
 // weight_unit=1//gm ==2//mg
 tft.textSetCursor(34, 129);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0);
  tft.textWrite("Rpm=");
 tft.textWrite(stringset_rpm);
  if(no_of_drums==1)
  {
    tft.fillRoundRect(10,180,120,25,3,RA8875_CYAN);
     tft.drawRoundRect(10,180,120,25,3,RA8875_BLACK);
    tft.textSetCursor(14, 185);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0);
  tft.textWrite("No.of Drums= 1");
  }
  if(no_of_drums==2)
  {
    tft.fillRoundRect(10,180,120,25,3,RA8875_CYAN);
    tft.drawRoundRect(10,180,120,25,3,RA8875_BLACK);
    tft.textSetCursor(14, 185);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0);
  tft.textWrite("No.of Drums= 2");
  }
  if(drum_type==1)
  {
    tft.fillRoundRect(10,150,133,25,3,RA8875_CYAN);
    tft.drawRoundRect(10,150,133,25,3,RA8875_BLACK);
    tft.textSetCursor(14, 155);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0);
  tft.textWrite("Drum= Friability");
  }
  if(drum_type==2)
  {
    tft.fillRoundRect(10,150,133,25,3,RA8875_CYAN);
    tft.drawRoundRect(10,150,133,25,3,RA8875_BLUE);
    tft.textSetCursor(14, 155);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0);
  tft.textWrite("Drum= Abrasion");
  }

  delay(100);
  tft.graphicsMode();
}

void home_menu()
{
  tft.textMode();
 // tft.cursorBlink(32);
/*
//upper //
tft.fillRect(0, 0, 480, 20 , 0Xffff);

//lower
tft.fillRect(0,250, 480, 25 , 0Xffff);


  //vertical line///
  //tft.drawFastVLine(280,00,272,RA8875_WHITE);
//  tft.drawFastVLine(340,02,257,RA8875_WHITE);
 // tft.drawFastVLine(400,00,200,RA8875_WHITE);

  //Horizontal line///
  tft.drawFastHLine(0,20,480,RA8875_BLACK);
  tft.drawFastHLine(0,250,480,RA8875_BLACK);
 // tft.drawFastHLine(280,200,180,RA8875_WHITE);


//tft.fillRoundRect(18, 54, 146, 190,10,0X379f);
 // tft.drawFastHLine(18,58,142,RA8875_BLACK);
 // tft.drawFastVLine(18,58,56,RA8875_BLACK);

/////start//////
  ///tft.fillTriangle(40,70,40,110,90,90,0x0000);
  //tft.fillRect(20, 40, 90, 70,0XFFFF);
  //tft.fillCircle(65,70,40,0xc859);
  //tft.fillCircle(65,70,30,0xffff);
  //tft.fillRoundRect(42, 42, 70, 70,10,0X632c);
/*  tft.fillRoundRect(42, 42, 70, 70,10,0Xc985);
  tft.fillRoundRect(43, 43, 65, 65,10,0Xffff);
 // tft.drawCircle(x,y,r,t);
  tft.fillTriangle(50,50,50,100,100,75,0x2fc2);
  tft.textSetCursor(50, 65); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Start");
*/
 // tft.fillRoundRect(20, 170, 130,58,10,0x333F);
// tft.fillCircle(65,200,40,0xc859);//cb2c
 
 /*tft.drawFastHLine(30, 170, 50, 0xffff);
// tft.drawFastHLine(, y, w, t);
 tft.drawLine(30, 170,20 , 190,0xffff);
 tft.drawLine(80, 170,90 ,190,0xffff);
 
 tft.drawFastVLine(20, 190, 20,0xffff );
 tft.drawFastVLine(90, 190, 20,0xffff );

 tft.drawLine(20, 210,30 , 230,0xffff);
 tft.drawLine(90, 210,80 ,230,0xffff);
 
 tft.drawFastHLine(30, 230, 50, 0xffff);
 */
/* tft.fillCircle(70,200,45,0xc985);
 tft.fillCircle(70,200,30,0xffff);
 tft.drawCircle(70,200,30,0x0000);
  tft.textSetCursor(40, 180); ///text location////
  tft.textTransparent(RA8875_RED);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("Stop");
*/
  /*//tft.fillRect(22, 177, 132,60,0xffff);
  tft.fillRoundRect(170, 185, 130,54,7,0xffff);//0x632c
  tft.fillRoundRect(180, 199, 115,25,5,0XF790);
  tft.fillCircle(167, 212,20,0XF790);
  tft.fillCircle(156, 212,15,0X379f);
  tft.textSetCursor(188, 200); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Settings");

  //tft.fillTriangle(40,30,40,70,100,50,0x2fc2);
  //tft.drawTriangle(40,30,40,70,100,50,0x0000);
 tft.fillRect(20, 30, 50,30,0x2fc2);
 tft.drawRect(20, 30, 50,30,0x0000);
  tft.textSetCursor(22, 35); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("logout");

 // tft.fillTriangle(10,268,50,100,100,75,0x2fc2);
 /* tft.textSetCursor(10, 250); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("login to start");
*/
/*
 // tft.fillRoundRect(170, 50, 130, 50,10,0xffff);
// tft.drawFastHLine(230,50,50,RA8875_BLACK);
 //tft.drawFastVLine(230,50,50,RA8875_BLACK);
 tft.fillCircle(230,65,40,0xffff);
 tft.drawFastHLine(230,65,30,RA8875_BLACK);
 tft.drawFastVLine(230,65,30,RA8875_BLACK);
 tft.drawCircle(230,65,40,0x0000);
 //for(j=0,;j<=3;j++)
 //{
 // tft.drawFastVLine(230,65,30,RA8875_BLACK);
 //}
  tft.textSetCursor(200, 60); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Time");

 tft.fillRoundRect(170, 120, 130, 50,5,0X632C );//0x632c
 tft.fillRect(210, 120, 50, 50,0X37f9 );//0x632c
  tft.textSetCursor(175, 135); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("  Counts ");
  /*tft.textSetCursor(175, 135); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("     123...");*/
 /* for(j=0;j<=3;j++)
  {
    tft.fillRect(210, 120, 50, 50,0Xffff );//0x632c
    tft.textSetCursor(220, 125); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1); ////text size////
    tft.textWrite(num[j]);
    delay(1000);
    //tft.textSetCursor(220, 125);
   // tft.textTransparent(RA8875_WHITE);
   // tft.textEnlarge(1);
  }*/

  //tft.fillRoundRect(170, 190, 130, 58,10,0x0a0f);
 /* for (int p = 0; p < 400; p++) 
{  k1 = 120 * (sin(PI * p / 2000));
   k2 = 120 * (cos(PI * p / 2000));  
   k3 = 60 * (sin(PI * p / 2000));  
   k4 = 60 * (cos(PI * p / 2000));  
  tft.drawLine(, k3 + 160, k2 + 160, k1+ 160, 0x0000);
}*/
/*
//tft.drawCircle(230,2300,60,0x0000);
tft.fillEllipse(230, 220, 55, 25, 0xcb2c);
tft.fillEllipse(230, 220, 50, 20, 0x379f);
tft.drawRoundRect(170, 190, 130, 58,5,0x0a0f);
tft.fillCircle(230,220,5,0x0000);
  tft.fillRoundRect(175,230,110, 15,1,0x079f);
  tft.textSetCursor(200, 205); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("RPM");
 // tft.fillCircle(230,220,5,0x0000);
 // tft.fillRoundRect(175,220,105, 25,1,0x079f);
  tft.drawLine(230, 220, 190, 200, 0x0000);
  //tft.fillCircle(230,220,5,0x0000);
*//*
  tft.fillRect(330, 50, 130, 58,0X632C);
  tft.textSetCursor(340, 55); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("Admin");
*/
/*
 tft.fillEllipse(390, 160,50 , 80,0XF790);
  tft.fillCircle(390, 60, 29, 0XF799);//0632C
  tft.fillRect(305,144,165,97,0X379f);
 // tft.drawRect(40,130,100,40,RA8875_WHITE);
  //tft.fillCurve(100, 100, 78, 38, 2, RA8875_BLACK);
  tft.textSetCursor(350, 110);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite("Admin");
  
 // tft.fillRect(330, 170, 130, 58,0X632C);
 
  tft.fillCircle(390, 200, 33, 0X0000);//0632C
  tft.fillCircle(390, 200, 30, 0X379f);//0632C
  tft.fillRect(350,200,33,32,0X379f);
  tft.fillRoundRect(341,176,112,40,8,0X0000);
  tft.fillRoundRect(343,178,110,38,6,RA8875_WHITE);
  tft.textSetCursor(380, 211);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite("<");
   tft.textSetCursor(346, 178); ///text location////
  tft.textTransparent(RA8875_RED);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("History");
*/
//delay(2000);
  //tft.graphicsMode();
  // tft.drawCircle(390,200,40,0xffff);
  //Horizontal line///
  tft.drawFastHLine(0,20,480,RA8875_BLACK);
  tft.drawFastHLine(0,250,480,RA8875_BLACK);
 // tft.drawFastHLine(280,200,180,RA8875_WHITE);


//tft.fillRoundRect(18, 54, 146, 190,10,0X379f);
 // tft.drawFastHLine(18,58,142,RA8875_BLACK);
 // tft.drawFastVLine(18,58,56,RA8875_BLACK);

/////start//////
  ///tft.fillTriangle(40,70,40,110,90,90,0x0000);
  //tft.fillRect(20, 40, 90, 70,0XFFFF);
  //tft.fillCircle(65,70,40,0xc859);
  //tft.fillCircle(65,70,30,0xffff);
  //tft.fillRoundRect(42, 42, 70, 70,10,0X632c);
/*  tft.fillRoundRect(42, 42, 70, 70,10,0Xc985);
  tft.fillRoundRect(43, 43, 65, 65,10,0Xffff);
 // tft.drawCircle(x,y,r,t);
  tft.fillTriangle(50,50,50,100,100,75,0x2fc2);
  tft.textSetCursor(50, 65); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Start");
*/
 // tft.fillRoundRect(20, 170, 130,58,10,0x333F);
// tft.fillCircle(65,200,40,0xc859);//cb2c
 
 /*tft.drawFastHLine(30, 170, 50, 0xffff);
// tft.drawFastHLine(, y, w, t);
 tft.drawLine(30, 170,20 , 190,0xffff);
 tft.drawLine(80, 170,90 ,190,0xffff);
 
 tft.drawFastVLine(20, 190, 20,0xffff );
 tft.drawFastVLine(90, 190, 20,0xffff );

 tft.drawLine(20, 210,30 , 230,0xffff);
 tft.drawLine(90, 210,80 ,230,0xffff);
 
 tft.drawFastHLine(30, 230, 50, 0xffff);
 */
/* tft.fillCircle(70,200,45,0xc985);
 tft.fillCircle(70,200,30,0xffff);
 tft.drawCircle(70,200,30,0x0000);
  tft.textSetCursor(40, 180); ///text location////
  tft.textTransparent(RA8875_RED);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("Stop");
*/
 tft.fillRect(176,10,176,20,RA8875_WHITE);
     tft.textSetCursor(182, 12);
   tft.textTransparent(RA8875_BLUE);
  tft.textEnlarge(0);
  tft.textWrite("Quality Lab Solutions");
tft.textSetCursor(330,252);
   tft.textTransparent(RA8875_BLUE);
  tft.textEnlarge(0);
  tft.textWrite("Model:QFR_200");

  //tft.fillRect(22, 177, 132,60,0xffff);
  //tft.fillRoundRect(170, 185, 130,54,7,0xffff);//0x632c
  tft.fillRoundRect(170, 185, 130,50,5,RA8875_BLUE);
  tft.drawRoundRect(170, 185, 130,50,5,0x0000);
  //tft.fillRoundRect(180, 199, 115,25,5,0XF790);
  //tft.fillCircle(167, 212,20,0XF790);
 // tft.fillCircle(156, 212,15,0X379f);
//tft.fillCircle(156, 212,15,0XFFFF);
  tft.textSetCursor(173, 190); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("SETTINGS");

  //tft.fillTriangle(40,30,40,70,100,50,0x2fc2);
  //tft.drawTriangle(40,30,40,70,100,50,0x0000);
 //tft.fillRect(20, 30, 50,30,0x2fc2);
  tft.fillRect(20, 30, 52,30,RA8875_BLUE);
 tft.drawRect(20, 30, 52,30,0x0000);
  tft.textSetCursor(22, 35); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("LOGOUT");

 // tft.fillTriangle(10,268,50,100,100,75,0x2fc2);
 /* tft.textSetCursor(10, 250); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("login to start");
*/
 tft.fillRoundRect(170, 50, 130, 50,5,RA8875_BLUE);
 tft.drawRoundRect(170, 50, 130,50,5,0x0000);
// tft.drawFastHLine(230,50,50,RA8875_BLACK);
 //tft.drawFastVLine(230,50,50,RA8875_BLACK);
 //tft.fillRoundRect(170, 120, 130, 50,5,0X632C );
 //tft.fillCircle(230,65,40,0xffff);
 //tft.drawFastHLine(230,65,30,RA8875_BLACK);
 //tft.drawFastVLine(230,65,30,RA8875_BLACK);
 //tft.drawCircle(230,65,40,0x0000);
 //for(j=0,;j<=3;j++)
 //{
 // tft.drawFastVLine(230,65,30,RA8875_BLACK);
 //}
  tft.textSetCursor(200, 60); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("TIME");

// tft.fillRoundRect(170, 120, 130, 50,5,0X632C );//0x632c
// tft.fillRect(210, 120, 50, 50,0X37f9 );//0x632c
 tft.fillRoundRect(170, 120, 128, 48,5,RA8875_BLUE);
 tft.drawRoundRect(170, 120, 128,48,5,0x0000);
  tft.textSetCursor(187, 129); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("COUNTS");
 /* tft.textSetCursor(175, 135); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("     123...");*/
 /* for(j=0;j<=3;j++)
  {
    tft.fillRect(210, 120, 50, 50,0Xffff );//0x632c
    tft.textSetCursor(220, 125); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1); ////text size////
    tft.textWrite(num[j]);
    delay(1000);
    //tft.textSetCursor(220, 125);
   // tft.textTransparent(RA8875_WHITE);
   // tft.textEnlarge(1);
  }*/

  //tft.fillRoundRect(170, 190, 130, 58,10,0x0a0f);
 /* for (int p = 0; p < 400; p++) 
{  k1 = 120 * (sin(PI * p / 2000));
   k2 = 120 * (cos(PI * p / 2000));  
   k3 = 60 * (sin(PI * p / 2000));  
   k4 = 60 * (cos(PI * p / 2000));  
  tft.drawLine(, k3 + 160, k2 + 160, k1+ 160, 0x0000);
}*/
/*
//tft.drawCircle(230,2300,60,0x0000);
tft.fillEllipse(230, 220, 55, 25, 0xcb2c);
tft.fillEllipse(230, 220, 50, 20, 0x379f);
tft.drawRoundRect(170, 190, 130, 58,5,0x0a0f);
tft.fillCircle(230,220,5,0x0000);
  tft.fillRoundRect(175,230,110, 15,1,0x079f);
  tft.textSetCursor(200, 205); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("RPM");
 // tft.fillCircle(230,220,5,0x0000);
 // tft.fillRoundRect(175,220,105, 25,1,0x079f);
  tft.drawLine(230, 220, 190, 200, 0x0000);
  //tft.fillCircle(230,220,5,0x0000);
*//*
  tft.fillRect(330, 50, 130, 58,0X632C);
  tft.textSetCursor(340, 55); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("Admin");
*/
 //tft.fillEllipse(390, 160,50 , 80,0XF790);
 // tft.fillCircle(390, 60, 29, 0XF799);//0632C
 // tft.fillRect(305,144,165,97,0X379f);
 // tft.drawRect(40,130,100,40,RA8875_WHITE);
// tft.drawRect(40,130,100,40,RA8875_BLACK);
  //tft.fillCurve(100, 100, 78, 38, 2, RA8875_BLACK);
 /*  tft.fillRoundRect(341,100,115,50,5,0X0000);
  tft.textSetCursor(350, 110);
   tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1);
  tft.textWrite("Admin");*/

//tft.fillRoundRect(341,90,115,50,5,0X0000);
 tft.fillRoundRect(341, 50, 115, 50,5,RA8875_BLUE);
 tft.drawRoundRect(341, 50, 115,50,5,0x0000);
  tft.textSetCursor(356, 60);
   tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1);
  tft.textWrite("ADMIN");
  
 // tft.fillRect(330, 170, 130, 58,0X632C);
 
 // tft.fillCircle(390, 200, 33, 0X0000);//0632C
  //tft.fillCircle(390, 200, 30, 0X379f);//0632C
  //tft.fillRect(350,200,33,32,0X379f);
  //tft.fillRoundRect(341,176,112,40,8,0X0000);
  tft.fillRoundRect(341,120,115,50,5,RA8875_BLUE);
  tft.drawRoundRect(341,120, 115,50,5,0x0000);
  //tft.fillRoundRect(343,178,110,38,6,RA8875_WHITE);
  //tft.textSetCursor(380, 211);
  // tft.textTransparent(RA8875_BLACK);
 // tft.textEnlarge(1);
 // tft.textWrite("<");
   tft.textSetCursor(344, 125); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("HISTORY");

  
homestatus_menu();

  tft.graphicsMode();
  page=0;
}


void time_function()
{
if(tc==0)
  {
    tc=1;
    tft.textMode();
        tft.fillRect(40, 120, 180, 60, 0x3f79);
         tft.textSetCursor(70,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
   
  tft.textSetCursor(85,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(100,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
  tft.graphicsMode();
  }
   
 //1//
    if(tx>360 && tx<472 && ty>713 && ty<870)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
      //tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn1);
      time_value[i]=1;// rpm value 
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
      Serial.print("yes");
    }
   
//Serial.print("b");
    //2//
    if(tx>224 && tx<359 && ty>713 && ty<870)
    { 
      tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn2);
      time_value[i]=2;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

      
    //3//
    if(tx>106 && tx<223 && ty>713 && ty<870)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn3);
      time_value[i]=3;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //4//
    if(tx>360 && tx<472 && ty>579 && ty<712)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn4);
      time_value[i]=4;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //5//
    if(tx>224 && tx<359 && ty>579 && ty<712)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn5);
      time_value[i]=5;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //6//
    if(tx>106 && tx<223 && ty>579 && ty<712)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn6);
      time_value[i]=6;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //7//
    if(tx>360 && tx<472 && ty>386 && ty<578)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn7);
      time_value[i]=7;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
     i++;
    }

    //8//
    if(tx>224 && tx<359 && ty>386 && ty<578)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn8);
      time_value[i]=8;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //9//
    if(tx>106 && tx<223 && ty>386 && ty<578)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn9);
      time_value[i]=9;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

     //<--//
    if(tx>360 && tx<472 && ty>300 && ty<385)
    {
  
      timebackflag=1;
      cursorx=70;
      i=0;
     
    
    }

    //0//
    if(tx>224 && tx<359 && ty>300 && ty<385)
    {
   // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_BLACK);
    // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite("0");
      time_value[i]=0;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //-->//
    if(tx>106 && tx<223 && ty>300 && ty<385)
    {
  
     timenextflag=1;
     cursorx=70;
     i=0;
   
    }
    
     //save//
   if(tx>106 && tx<286 && ty>155 && ty<299)
    {  
      /*tft.textMode();
      tft.textSetCursor(50, 210); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("value is stored");*/
  tft.fillRect(0, 0, 480, 15 , RA8875_GREEN);
  //tft.fillRect(75, 210, 80, 30, 0xffff);
   // cursorx=70;
    if(i==1)
   {
    total_time=(time_value[0]*1)+(time_value[1]*0)+(time_value[2]*0);
   }
    if(i==2)
   {
    total_time=(time_value[0]*10)+(time_value[1]*1)+(time_value[2]*0);
   }
    if(i==3)
   {
    total_time=(time_value[0]*100)+(time_value[1]*10)+time_value[2];
   }
   // total_time=(time_value[0]*100)+(time_value[1]*10)+time_value[2];
    Serial.print(total_time);
    Serial.print(total_rpm);
    Serial.print(ac);
    select_mode=1;
    //i=0;
    ///save_no_of_drumps=no_of_drumps;
   // save_drum_type=drum_type;
   // save_calculation=calculation;
   // i2=6;
    //flag=1;  
    //Serial.print("ne");
   /* if(total_time<1 && total_time>999)
    {
       
     cursorx=70;
     time_value[0]=2;
     time_value[1]=5;
     total_time=4;
     
     tft.textMode();
     tft.textSetCursor(1, 1); ///text location////
      tft.textTransparent(RA8875_RED);
     tft.textEnlarge(0);  ////text size////
      tft.textWrite("please enter value in between 20 to 50");
      
    }*/
    
    tft.graphicsMode();
    }

 ///clrea///
    if(tx>287 && tx<472 && ty>155 && ty<299)
    {
      /* tft.textMode();
     tft.textSetCursor(1, 1); ///text location////
      tft.textTransparent(RA8875_RED);
     tft.textEnlarge(0);  ////text size////
      tft.textWrite("");*/
      tft.fillRect(0, 0, 480, 15 , 0X0A0F);
      tft.textMode();
        tft.fillRect(40, 120, 180, 60, 0x3f79);
         tft.textSetCursor(70,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
   
  tft.textSetCursor(85,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(100,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

      
    //i=i-1;
      time_value[0]=0;// user entered password value
       time_value[1]=0;
        time_value[2]=0;
        
        tft.fillRect(40, 210, 150, 30, 0xffff);
        
      i=0;
      cursorx=70;
      
    }
    if(cursorx>115)
    {
      tft.fillRect(0, 0, 480, 15 , 0X0A0F);
      tft.textMode();
      tft.fillRect(40, 120, 180, 60, 0x3f79);

       tft.textSetCursor(70,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
   
  tft.textSetCursor(85,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(100,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

 tft.fillRect(40, 210, 150, 30, 0xffff);
      i=0;
      cursorx=70;
      tft.graphicsMode();
    }
    
    tft.graphicsMode();
  }



void counts_function()
{
  if(tc==0)
  {
    tc=1;
    tft.textMode();
        tft.fillRect(40, 120, 180, 60, 0x3f79);
         tft.textSetCursor(70,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
   
  tft.textSetCursor(85,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(100,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
  tft.graphicsMode();
  }
 //1//
    if(tx>360 && tx<472 && ty>713 && ty<870)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
      //tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn1);
      count_value[i]=1;// rpm value 
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
      Serial.print("yes");
    }
   
//Serial.print("b");
    //2//
    if(tx>224 && tx<359 && ty>713 && ty<870)
    { 
      tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn2);
      count_value[i]=2;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

      
    //3//
    if(tx>106 && tx<223 && ty>713 && ty<870)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn3);
      count_value[i]=3;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //4//
    if(tx>360 && tx<472 && ty>579 && ty<712)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn4);
      count_value[i]=4;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //5//
    if(tx>224 && tx<359 && ty>579 && ty<712)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn5);
      count_value[i]=5;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //6//
    if(tx>106 && tx<223 && ty>579 && ty<712)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn6);
      count_value[i]=6;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //7//
    if(tx>360 && tx<472 && ty>386 && ty<578)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn7);
      count_value[i]=7;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
     i++;
    }

    //8//
    if(tx>224 && tx<359 && ty>386 && ty<578)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn8);
      count_value[i]=8;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //9//
    if(tx>106 && tx<223 && ty>386 && ty<578)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn9);
      count_value[i]=9;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

     //<--//
    if(tx>360 && tx<472 && ty>300 && ty<385)
    {
  
      countbackflag=1;
     cursorx=70;
     i=0;
    
    }

    //0//
    if(tx>224 && tx<359 && ty>300 && ty<385)
    {
   // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_BLACK);
    // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite("0");
      count_value[i]=0;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //-->//
    if(tx>106 && tx<223 && ty>300 && ty<385)
    {
  
     countnextflag=1;
   cursorx=70;
     i=0;
    }
    
     //save//
   if(tx>106 && tx<286 && ty>155 && ty<299)
    {  
      
   // cursorx=70;
   tft.fillRect(0, 0, 480, 15 , RA8875_GREEN);
  /* tft.fillRect(0, 0, 480, 15 , 0X0A0F);
   tft.textMode();
     tft.textSetCursor(2, 2); ///text location////
      tft.textTransparent(RA8875_RED);
     tft.textEnlarge(0);  ////text size////
      tft.textWrite("Successfully Stored");*/
      if(i==1)
   {
    total_count=(count_value[0]*1)+(count_value[1]*0)+(count_value[2]*0);
   }
    if(i==2)
   {
    total_count=(count_value[0]*10)+(count_value[1]*1)+(count_value[2]*0);
   }
    if(i==3)
   {
    total_count=(count_value[0]*100)+(count_value[1]*10)+count_value[2];
   }
   // total_count=(count_value[0]*100)+(count_value[1]*10)+count_value[2];
    Serial.print(total_count);
    select_mode=2;
    //i=0;
    ///save_no_of_drumps=no_of_drumps;
   // save_drum_type=drum_type;
   // save_calculation=calculation;
   // i2=6;
    //flag=1;  
    //Serial.print("ne");
   /* if(total_time<1 && total_time>999)
    {
       
     cursorx=70;
     time_value[0]=2;
     time_value[1]=5;
     total_time=4;
     
     tft.textMode();
     tft.textSetCursor(1, 1); ///text location////
      tft.textTransparent(RA8875_RED);
     tft.textEnlarge(0);  ////text size////
      tft.textWrite("please enter value in between 20 to 50");
      
    }*/
    
    tft.graphicsMode();
    }

 ///clrea///
    if(tx>287 && tx<472 && ty>155 && ty<299)
    {
      /* tft.textMode();
     tft.textSetCursor(1, 1); ///text location////
      tft.textTransparent(RA8875_RED);
     tft.textEnlarge(0);  ////text size////
      tft.textWrite("");*/
      tft.fillRect(0, 0, 480, 15 , 0X0A0F);
      tft.textMode();
        tft.fillRect(40, 120, 180, 60, 0x3f79);
         tft.textSetCursor(70,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
   
  tft.textSetCursor(85,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(100,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

      
    //i=i-1;
      count_value[0]=0;// user entered password value
       count_value[1]=0;
        count_value[2]=0;
        
         tft.fillRect(40, 210, 150, 30, 0xffff);
         
      i=0;
      cursorx=70;
      
    }
    if(cursorx>115)
    {
      tft.fillRect(0, 0, 480, 15 , 0X0A0F);
      tft.textMode();
      tft.fillRect(40, 120, 180, 60, 0x3f79);

       tft.textSetCursor(70,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
   
  tft.textSetCursor(85,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(100,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

 tft.fillRect(40, 210, 150, 30, 0xffff);
 
      i=0;
      cursorx=70;
    }
    tft.graphicsMode();
}

void timecountweight1_menu()
{
  tft.textMode();

  tft.fillRoundRect(8, 30, 240, 170,10, 0Xffff);
  
  tft.fillRoundRect(8, 30, 240, 80,10,0X318C);
  tft.drawRoundRect(7,29,241,171,10,RA8875_BLACK);
  tft.textSetCursor(30, 40);
 
  /* Render some text! */
  
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0);
  tft.textWrite("Weight the Tablets");
///// FOR string2////// 
  tft.textSetCursor(66, 60); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("to Load in Drum 1");

  ////entering reading value/////
  tft.fillRect(40, 120, 180, 60, 0x3f79);

  tft.textSetCursor(70,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
  
  tft.textSetCursor(85,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(100,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

   tft.textSetCursor(115,140); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("."); 

  tft.textSetCursor(130,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(145,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
  
 
tft.graphicsMode();
  
  }


void timecountweight2_menu()
{
  tft.textMode();
  //tft.cursorBlink(32);
  
 // tft.drawRoundRect(7, 22, 240, 170, 20, 0xffff);//0x3041,0xc986
  /* Set the cursor location (in pixels) */
//tft.fillRect(464,12,30,251,RA8875_WHITE);

//tft.drawRect(7,19,241,171,RA8875_BLACK);
  
  tft.fillRoundRect(8, 30, 240, 170,10, 0Xffff);
  
  tft.fillRoundRect(8, 30, 240, 80,10,0X318C);
  tft.drawRoundRect(7,29,241,171,10,RA8875_BLACK);
  tft.textSetCursor(30, 40);
 
  /* Render some text! */
  
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0);
  tft.textWrite("Weight the Tablets");
///// FOR string2////// 
  tft.textSetCursor(66, 60); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("to Load in Drum 2");

  ////entering reading value/////
  tft.fillRect(40, 120, 180, 60, 0x3f79);
  
  tft.textSetCursor(70,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
   
  tft.textSetCursor(85,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(100,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(115,140); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("."); 

  tft.textSetCursor(130,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(145,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
 // tft.fillRect(50, 100, 180, 60, RA8875_WHITE);
 // tft.fillCircle(140,160,50,0X3186);
 // tft.textSetCursor(170, 110); ///text location////
//  tft.textTransparent(RA8875_BLACK);
 // tft.textEnlarge(1); ////text size////
  
 // tft.textWrite(stringmin);

 /* //vertical line///
  tft.drawFastVLine(48,98,59,0X3186);
  tft.drawFastVLine(231,101,59,0X3186);
 // tft.drawFastVLine(400,00,200,RA8875_WHITE);

  //Horizontal line///
  tft.drawFastHLine(50,161,180,0X3186);
  tft.drawFastHLine(48,98,180,0X3186);
 // tft.drawFastHLine(280,200,180,RA8875_WHITE);
*/
tft.graphicsMode();
}

void timecountweight1_function()
{
   //1//
    if(tx>360 && tx<472 && ty>713 && ty<870)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
      //tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn1);
      weight_value[i]=1;// rpm value 
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
      Serial.print("yes");
    }
   
//Serial.print("b");
    //2//
    if(tx>224 && tx<359 && ty>713 && ty<870)
    { 
      tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn2);
      weight_value[i]=2;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

      
    //3//
    if(tx>106 && tx<223 && ty>713 && ty<870)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn3);
      weight_value[i]=3;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //4//
    if(tx>360 && tx<472 && ty>579 && ty<712)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn4);
      weight_value[i]=4;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //5//
    if(tx>224 && tx<359 && ty>579 && ty<712)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn5);
      weight_value[i]=5;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //6//
    if(tx>106 && tx<223 && ty>579 && ty<712)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn6);
      weight_value[i]=6;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //7//
    if(tx>360 && tx<472 && ty>386 && ty<578)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn7);
      weight_value[i]=7;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
     i++;
    }

    //8//
    if(tx>224 && tx<359 && ty>386 && ty<578)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn8);
      weight_value[i]=8;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //9//
    if(tx>106 && tx<223 && ty>386 && ty<578)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn9);
      weight_value[i]=9;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

     //<--//
    if(tx>360 && tx<472 && ty>300 && ty<385)
    {
  
      weightback_flag=1;
     cursorx=70;
     i=0;
    
    }

    //0//
    if(tx>224 && tx<359 && ty>300 && ty<385)
    {
   // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_BLACK);
    // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite("0");
      weight_value[i]=0;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //-->//
    if(tx>106 && tx<223 && ty>300 && ty<385)
    {
  
     weightnext_flag=1;
   cursorx=70;
     i=0;
    }
    
     //save//
   if(tx>106 && tx<286 && ty>155 && ty<299)
    {  
     tft.fillRect(0, 0, 480, 15 , RA8875_GREEN);
     /* tft.fillRect(0, 0, 480, 15 , 0X0A0F);
   tft.textMode();
     tft.textSetCursor(2, 2); ///text location////
      tft.textTransparent(RA8875_RED);
     tft.textEnlarge(0);  ////text size////
      tft.textWrite("Successfully Stored");*/
   // cursorx=70;
    total_weight1=(weight_value[0]*10000)+(weight_value[1]*1000)+(weight_value[2]*100)+(weight_value[3]*10)+weight_value[4];
   
    perw1=(weight_value[0]*10000)+(weight_value[1]*1000)+(weight_value[2]*100)+(weight_value[3]*10)+weight_value[4];
  for(i=0;i<=4;i++)
  {
  hisweight1_value[i]=weight_value[i];
  Serial.print("hisweight1=");
  Serial.print(hisweight1_value[i]);
  }
//  peroutw1=(hisweight1_value[0]*10000)+( hisweight1_value[1]*1000)+( hisweight1_value[2]*100)+( hisweight1_value[3]*10)+ hisweight1_value[4];
   /*Serial.print(weight_value[0]);
      Serial.print(weight_value[1]);
         Serial.print(weight_value[2]);
            Serial.print(weight_value[3]);
               Serial.print(weight_value[4]);*/
               Serial.print("\nTOTAL===");
                  Serial.print(perw1);
                 // Serial.print(weight_value[0);
    //i=0;
    ///save_no_of_drumps=no_of_drumps;
   // save_drum_type=drum_type;
   // save_calculation=calculation;
   // i2=6;
    //flag=1;  
    //Serial.print("ne");
   /* if(total_time<1 && total_time>999)
    {
       
     cursorx=70;
     time_value[0]=2;
     time_value[1]=5;
     total_time=4;
     
     tft.textMode();
     tft.textSetCursor(1, 1); ///text location////
      tft.textTransparent(RA8875_RED);
     tft.textEnlarge(0);  ////text size////
      tft.textWrite("please enter value in between 20 to 50");
      
    }*/
    
    tft.graphicsMode();
    }

 ///clrea///
    if(tx>287 && tx<472 && ty>155 && ty<299)
    {
      for(i=0;i<=4;i++)
      {
      weight_value[i]=hisweight1_value[i]=0;
      }
      total_weight1=0;
      perw1=0;
      tft.fillRect(0, 0, 480, 15 , 0X0A0F);
      /* tft.textMode();
     tft.textSetCursor(1, 1); ///text location////
      tft.textTransparent(RA8875_RED);
     tft.textEnlarge(0);  ////text size////
      tft.textWrite("");*/
       tft.textMode();
       
        tft.fillRect(40, 120, 180, 60, 0x3f79);

      tft.textSetCursor(70,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
   
  tft.textSetCursor(85,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(100,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(115,140); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("."); 

  tft.textSetCursor(130,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(145,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
  tft.graphicsMode();

   tft.fillRect(40, 210, 150, 30, 0xffff);
    //i=i-1;
      /*count_value[0]=0;// user entered password value
       count_value[1]=0;
        count_value[2]=0;*/
      i=0;
      cursorx=70;
      
    }
    if(cursorx==115)
    {
      cursorx=130;
    }
    if(cursorx>160)
    {
      tft.fillRect(0, 0, 480, 15 ,0X0A0F);
      tft.textMode();
      tft.fillRect(40, 120, 180, 60, 0x3f79);

      tft.textSetCursor(70,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
   
  tft.textSetCursor(85,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(100,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(115,140); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("."); 

  tft.textSetCursor(130,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(145,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.graphicsMode();

   tft.fillRect(40, 210, 150, 30, 0xffff);
      i=0;
      cursorx=70;
    }
}

void timecountweight2_function()
{
   //1//
    if(tx>360 && tx<472 && ty>713 && ty<870)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
      //tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn1);
      weight_value[i]=1;// rpm value 
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
      Serial.print("yes");
    }
   
//Serial.print("b");
    //2//
    if(tx>224 && tx<359 && ty>713 && ty<870)
    { 
      tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn2);
      weight_value[i]=2;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

      
    //3//
    if(tx>106 && tx<223 && ty>713 && ty<870)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn3);
      weight_value[i]=3;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //4//
    if(tx>360 && tx<472 && ty>579 && ty<712)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn4);
      weight_value[i]=4;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //5//
    if(tx>224 && tx<359 && ty>579 && ty<712)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn5);
      weight_value[i]=5;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //6//
    if(tx>105 && tx<223 && ty>579 && ty<712)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn6);
      weight_value[i]=6;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //7//
    if(tx>360 && tx<472 && ty>386 && ty<578)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn7);
      weight_value[i]=7;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
     i++;
    }

    //8//
    if(tx>224 && tx<359 && ty>386 && ty<578)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn8);
      weight_value[i]=8;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //9//
    if(tx>106 && tx<223 && ty>386 && ty<578)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn9);
      weight_value[i]=9;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

     //<--//
    if(tx>360 && tx<472 && ty>300 && ty<385)
    {
  
      weightback2_flag=1;
     cursorx=70;
     i=0;
    
    }

    //0//
    if(tx>224 && tx<359 && ty>300 && ty<385)
    {
   // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_BLACK);
    // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite("0");
      weight_value[i]=0;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //-->//
    if(tx>106 && tx<223 && ty>300 && ty<385)
    {
  
     weightnext2_flag=1;
   cursorx=70;
     i=0;
    }
    
     //save//
   if(tx>106 && tx<286 && ty>155 && ty<299)
    {  
     tft.fillRect(0, 0, 480, 15 , RA8875_GREEN);
       tft.textMode();
      /*tft.textSetCursor(50, 210); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("value is Stored");*/
   // cursorx=70;
    total_weight2=(weight_value[0]*10000)+(weight_value[1]*1000)+(weight_value[2]*100)+(weight_value[3]*10)+(weight_value[4]);
 perw2=total_weight2;
  for(i=0;i<=4;i++)
  {
    Serial.print("hiswe1=");
     Serial.print( hisweight1_value[i]);
    Serial.print("hiswe2=");
   Serial.print(hisweight2_value[i]=weight_value[i]);
  
  }
    Serial.print(total_weight2);
    //i=0;
    ///save_no_of_drumps=no_of_drumps;
   // save_drum_type=drum_type;
   // save_calculation=calculation;
   // i2=6;
    //flag=1;  
    //Serial.print("ne");
   /* if(total_time<1 && total_time>999)
    {
       
     cursorx=70;
     time_value[0]=2;
     time_value[1]=5;
     total_time=4;
     
     tft.textMode();
     tft.textSetCursor(1, 1); ///text location////
      tft.textTransparent(RA8875_RED);
     tft.textEnlarge(0);  ////text size////
      tft.textWrite("please enter value in between 20 to 50");
      
    }*/
    
    tft.graphicsMode();
    }

 ///clrea///
    if(tx>287 && tx<472 && ty>155 && ty<299)
    {
      for(i=0;i<=4;i++)
      {
      weight_value[i]=hisweight2_value[i]=0;
      }
       total_weight2= perw2=0;
      /* tft.textMode();
     tft.textSetCursor(1, 1); ///text location////
      tft.textTransparent(RA8875_RED);
     tft.textEnlarge(0);  ////text size////
      tft.textWrite("");*/
      tft.fillRect(0, 0, 480, 15 , 0X0A0F);
        tft.textMode();
      tft.fillRect(40, 120, 180, 60, 0x3f79);

      tft.textSetCursor(70,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
   
  tft.textSetCursor(85,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(100,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(115,140); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("."); 

  tft.textSetCursor(130,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(145,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.graphicsMode();

   tft.fillRect(40, 210, 150, 30, 0xffff);
      
    //i=i-1;
      /*count_value[0]=0;// user entered password value
       count_value[1]=0;
        count_value[2]=0;*/
      i=0;
      cursorx=70;
      
    }
    
    if(cursorx==115)
    {
      cursorx=130;
    }
    if(cursorx>160)
    {
      tft.fillRect(0, 0, 480, 15 , 0X0A0F);
      tft.textMode();
      tft.fillRect(40, 120, 180, 60, 0x3f79);

      tft.textSetCursor(70,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
   
  tft.textSetCursor(85,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(100,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(115,140); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("."); 

  tft.textSetCursor(130,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(145,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.graphicsMode();

   tft.fillRect(40, 210, 150, 30, 0xffff);
      i=0;
      cursorx=70;
    }
}






void admin_disp()
{
  tft.fillScreen(0x0000);//to clear screen
  
  tft.textMode();
  //tft.cursorBlink(32);
   
    ////change date & TIME////
  tft.drawRoundRect(100, 60, 300, 50, 10, 0xC618);
 // tft.textSetCursor(115, 65); ///text location////
  tft.textTransparent(0x94B2);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("Set Date and Time");

   ///change password ///
  tft.drawRoundRect(100, 150, 300, 50, 10, 0xC618);
 // tft.textSetCursor(150, 155); ///text location////
  tft.textTransparent(0x94B2);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("Set password");
  tft.textTransparent(0xC618);
  tft.textWrite(" ");

   tft.graphicsMode();///to enable touch and disable textmode else touch willnot work///

   
}

void password_function()
{
  while(flag == 0)
      {
        //delay(300);
        if(! digitalRead(RA8875_INT))
          {
            delay(150);
        tft.touchRead(&tx, &ty);
   
        while(i<4)
        {
          //delay(300);
          if(! digitalRead(RA8875_INT))
          {
            delay(150);
          // Serial.print(i);
          tft.touchRead(&tx, &ty);
   //1//
    if(tx>326 && tx<452 && ty>700 && ty<899)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_GREEN);
  
      tft.textEnlarge(2);  ////text size////
      tft.textWrite(stringn1);
      userenter[i]=1;// user entered password value 
      reenteredpass[i]=1; //to change password
      Serial.print(userenter[i]);
      cursorx=cursorx+25;
      tft.graphicsMode();
      i++;
    }
   
    //2//
    if(tx>200 && tx<321 && ty>666 && ty<899)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_GREEN);
      tft.textEnlarge(2);  ////text size////
      tft.textWrite(stringn2);
      userenter[i]=2;// user entered password value
      reenteredpass[i]=2; //to change password
      Serial.print(userenter[i]);
      cursorx=cursorx+25;
      tft.graphicsMode();
      i++;
    }

      
    //3//
    if(tx>78 && tx<192 && ty>665 && ty<899)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_GREEN);
  
      tft.textEnlarge(2);  ////text size////
      tft.textWrite(stringn3);
      userenter[i]=3;// user entered password value
      reenteredpass[i]=3; //to change password
      Serial.print(userenter[i]);
      cursorx=cursorx+25;
      tft.graphicsMode();
      i++;
    }

    //4//
    if(tx>326 && tx<452 && ty>505 && ty<677)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_GREEN);
  
      tft.textEnlarge(2);  ////text size////
      tft.textWrite(stringn4);
      userenter[i]=4;// user entered password value 
      reenteredpass[i]=4; //to change password
      Serial.print(userenter[i]);
      cursorx=cursorx+25;
      tft.graphicsMode();
      i++;
    }

    //5//
    if(tx>200 && tx<321 && ty>504 && ty<663)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_GREEN);
  
      tft.textEnlarge(2);  ////text size////
      tft.textWrite(stringn5);
      userenter[i]=5;// user entered password value
      reenteredpass[i]=5; //to change password
      Serial.print(userenter[i]);
      cursorx=cursorx+25;
      tft.graphicsMode();
      i++;
    }

    //6//
    if(tx>78 && tx<192 && ty>503 && ty<675)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_GREEN);
  
      tft.textEnlarge(2);  ////text size////
      tft.textWrite(stringn6);
      userenter[i]=6;// user entered password value
      reenteredpass[i]=6; //to change password
      cursorx=cursorx+25;
      tft.graphicsMode();
      i++;
    }

    //7//
    if(tx>326 && tx<452 && ty>275 && ty<502)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_GREEN);
  
      tft.textEnlarge(2);  ////text size////
      tft.textWrite(stringn7);
      userenter[i]=7;// user entered password value
      reenteredpass[i]=7; //to change password
      cursorx=cursorx+25;
      tft.graphicsMode();
     i++;
    }

    //8//
    if(tx>200 && tx<321 && ty>275 && ty<502)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_GREEN);
  
      tft.textEnlarge(2);  ////text size////
      tft.textWrite(stringn8);
      userenter[i]=8;// user entered password value
      reenteredpass[i]=8; //to change password
      cursorx=cursorx+25;
      tft.graphicsMode();
      i++;
    }

    //9//
    if(tx>78 && tx<192 && ty>275 && ty<502)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_GREEN);
  
      tft.textEnlarge(2);  ////text size////
      tft.textWrite(stringn9);
      userenter[i]=9;// user entered password value
      reenteredpass[i]=9; //to change password
      cursorx=cursorx+25;
      tft.graphicsMode();
      i++;
    }

    //0//
    if(tx>326 && tx<452 && ty>78 && ty<269)
    {
   // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_BLACK);
    // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_GREEN);
  
      tft.textEnlarge(2);  ////text size////
      tft.textWrite(stringn0);
      userenter[i]=0;// user entered password value
      reenteredpass[i]=0; //to change password
      cursorx=cursorx+25;
      tft.graphicsMode();
      i++;
      Serial.print("ipf0");
    }

     ///clear////
   if(tx>514 && tx<653 && ty>78 && ty<269 )
     { 
    // tft.touchEnable(true);
     tft.fillRect(57, 120, 150, 58, RA8875_RED);
     tft.textMode();
   //  tft.cursorBlink(32);
     tft.textSetCursor(70, 150); ///text location////
     tft.textEnlarge(2);  ////text size////
     cursorx=70;
     userenter[0]=0;// user entered password value
     userenter[1]=0;// user entered password value
     userenter[2]=0;// user entered password value
     userenter[3]=0;// user entered password value

     reenteredpass[i]=0; //to change password
     reenteredpass[i]=0; //to change password
     reenteredpass[i]=0; //to change password
     reenteredpass[i]=0; //to change password
     tft.graphicsMode();
     i=0;
  }

   }
     }
  
    while(i>3 && i2<5)
    {
     // delay(300);
    
      if(! digitalRead(RA8875_INT))
          {
            delay(150);
            tft.touchRead(&tx, &ty);
          
     //next//

    if(tx>88 && tx<318 && ty>144 && ty<236)
    {
     // tft.fillRoundRect(355, 215, 120, 50, 10, RA8875_BLACK);
     // tft.fillRoundRect(355, 215, 120, 50, 10, RA8875_RED);
      //tft.textTransparent(RA8875_GREEN);
     /* tft.textMode();
      tft.cursorBlink(32);
      tft.textTransparent(RA8875_GREEN);
  
      tft.textEnlarge(2);  ////text size////
      tft.textWrite(stringnext);
      tft.graphicsMode();
      
    */
    tft.graphicsMode();
    cursorx=70;
    i=0;
    i2=6;
    flag=1;  
    Serial.print("ne");
    }
    
  ///clear////
   if(tx>514 && tx<653 && ty>78 && ty<269 )
     { 
    // tft.touchEnable(true);
     tft.fillRect(54, 125, 150, 58, RA8875_RED);
     tft.textMode();
   //  tft.cursorBlink(32);
     tft.textSetCursor(70, 150); ///text location////
     tft.textEnlarge(2);  ////text size////
    userenter[0]=0;// user entered password value
    userenter[1]=0;// user entered password value
    userenter[2]=0;// user entered password value
    userenter[3]=0;// user entered password value

    reenteredpass[i]=0; //to change password
    reenteredpass[i]=0; //to change password
    reenteredpass[i]=0; //to change password
    reenteredpass[i]=0; //to change password
     cursorx=70;
     tft.graphicsMode();
     i=0;
     i2=6;
  }
    }
    }
      i2=0;
          }
  }
 i=0;
 j=0;
}
void motor_menu()
{
  tft.fillScreen(0x379f);
   tft.textMode();
  /* tft.drawRect(30, 40, 100, 40, RA8875_BLACK);
  tft.textSetCursor(35,45 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("Store");*/
//upper //
tft.fillRect(0, 0, 480, 20 , 0Xffff);

//lower
tft.fillRect(0,250, 480, 25 , 0Xffff);


  //vertical line///
  //tft.drawFastVLine(280,00,272,RA8875_WHITE);
//  tft.drawFastVLine(340,02,257,RA8875_WHITE);
 // tft.drawFastVLine(400,00,200,RA8875_WHITE);

  //Horizontal line///
  tft.drawFastHLine(0,20,480,RA8875_BLACK);
  tft.drawFastHLine(0,250,480,RA8875_BLACK);

  tft.fillRoundRect(30, 40, 100, 40,5, RA8875_GREEN);
  tft.drawRoundRect(30, 40, 100, 40,5, RA8875_BLACK);
  tft.textSetCursor(35,45 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("Start");

  tft.fillRoundRect(30, 100, 100, 40,5, RA8875_YELLOW);
  tft.drawRoundRect(30, 100, 100, 40,5, RA8875_BLACK);
  tft.textSetCursor(35,105 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("Pause");

   tft.fillRoundRect(30, 160, 100, 40,5, RA8875_RED);
   tft.drawRoundRect(30, 160, 100, 40,5, RA8875_BLACK);
  tft.textSetCursor(35,165 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("Stop");

  tft.fillRoundRect(420, 20, 45, 30,4 ,0x0a0f);//0XC618
  tft.textSetCursor(431,24 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("-->");

 /* tft.textSetCursor(220, 120);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite("Elapsed Time");
  */
  tft.graphicsMode();
}
 void motor_function ()
 {

 /* if(tx>750 && tx<918 && ty>669 && ty<741)
  {
    motorstore_flag=1;
  }*/
  if(tx>755 && tx<921 && ty>623 && ty<760)
  {
    motorstart_flag=1;
   // Serial.print("t");
  }
  /*if(tx>756 && tx<921 && ty>316 && ty<411)
  {
    motorstop_flag=1;
  }*/
  ////-->////
    if(tx>143 && tx<179 && ty>763 && ty<818)
    {
   motornext_flag=1;
     // cursorxa=150;
     // i=0;
    }
     
 }
 
 void motor_timestart_function()
 {
  int t2=5;
  unsigned long currmilli;
  digitalWrite(pin6,LOW);
  digitalWrite(pin5,HIGH);
//digitalWrite(pin6,LOW);
  tft.fillRect(228,158,140,40,RA8875_WHITE);
   tft.textMode();
  tft.textSetCursor(230, 160);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite("00");
   tft.textWrite(":");
   tft.textSetCursor(280, 160);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
 tft.textWrite("00");
 tft.textWrite(":");
  tft.textSetCursor(330, 160);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite("00");
  tft.graphicsMode();
  while(dubrunsec/60!=total_time)
  { 
 // currmilli=millis();

 // runmilisec++;
 
  
 // if(runmilisec>=1000)
 // {
  //  if(runmilisec==10||runmilisec==20||runmilisec==30||runmilisec==40)
   // {
   //   runmilisec=runmilisec+2;
   // }
    
   
//currmilli=millis();
/*if((millis-prevmilli)>t2)
{
  //currmilli=currmilli;
Serial.print("1");
Serial.print("\n");
}*/
//myStepper.step(200);
//delay(1000);
//checkt1++;
//checkt2;
//prevmilli=millis();
//unsigned long currmilli=millis();
 //if((millis()-prevmilli)>1000);
 //{
 // prevmilli=currmilli; //currmilli;
  //dubrunsec++;
 // delay(1);
 // runmilisec++;
 // Serial.print("\nsec");
  
//  if(runmilisec==1000/(t2*4))
//  {
  //  if(runmilisec==10||runmilisec==20||runmilisec==30||runmilisec==40)
   // {
   //   runmilisec=runmilisec+2;
   // }
  //  runsec++;
  //  dubrunsec++;
 //   runmilisec=0;
 // }
 // dubrunsec++;
  //runsec++;
  
  // prevmilli=currmilli;
 //}
 prevmilli=millis();
 while(millis()-prevmilli<1000)
  {
    digitalWrite(pin4,LOW);
//delayMicroseconds(1666);
delayMicroseconds(ac);
digitalWrite(pin4,HIGH);
delayMicroseconds(ac);
   // prevmilli=currmilli;
   // Serial.print("lll\n");
   /* digitalWrite(pin8,LOW);
digitalWrite(pin9,LOW);
digitalWrite(pin10,LOW);
digitalWrite(pin11,HIGH);
delay(t2);

digitalWrite(pin9,LOW);
digitalWrite(pin10,LOW);
digitalWrite(pin11,LOW);
digitalWrite(pin8,HIGH);
delay(t2);

digitalWrite(pin8,LOW);
digitalWrite(pin9,LOW);
digitalWrite(pin11,LOW);
digitalWrite(pin10,HIGH);
delay(t2);

digitalWrite(pin8,LOW);
digitalWrite(pin10,LOW);
digitalWrite(pin11,LOW);
digitalWrite(pin9,HIGH);
delay(t2);*/
  }
 runsec++;
    dubrunsec++;
   /* if(runmin>1)
    {
      
    }*/
  if(runsec==60)
  {
    
    runmin++;
    runsec=0;
  }
  if(runmin==60)
  {
    runhour++;
    runmin=0;
  }
  if(runhour==12)
  {
    runhour=0;
  }
  if(dubrunsec/60==total_time)
  {
    digitalWrite(pin5,HIGH);
  //  digitalWrite(pin6,HIGH);
  }
  runsec_string[0]=(runsec/10)+'0';
  runsec_string[1]=(runsec%10)+'0';
  runmin_string[0]=(runmin/10)+'0';
  runmin_string[1]=(runmin%10)+'0';
  runhour_string[0]=(runhour/10)+'0';
  runhour_string[1]=(runhour%10)+'0';
  tft.fillRect(228,158,140,40,RA8875_WHITE);
   tft.textMode();
  tft.textSetCursor(230, 160);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite(runhour_string);
   tft.textWrite(":");
   tft.textSetCursor(280, 160);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
 tft.textWrite(runmin_string);
 tft.textWrite(":");
  tft.textSetCursor(330, 160);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite(runsec_string);
  tft.graphicsMode();
  
if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);   
/*if(tx>143 && tx<179 && ty>763 && ty<818)
    {
   motornext_flag=1;
   break;
     // cursorxa=150;
     // i=0;
    }*/
    if(tx>750 && tx<910 && ty>498 && ty<592)
  {
    tx=ty=0;
    delayMicroseconds(110);
    delayMicroseconds(130);
    digitalWrite(pin5,HIGH);
  //  digitalWrite(pin6,HIGH);
    tft.textMode();
    tft.fillRoundRect(30, 100, 100, 40,5, RA8875_YELLOW);
  tft.drawRoundRect(30, 100, 100, 40,5, RA8875_BLACK);
  tft.textSetCursor(35,105 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("Resume");
  tft.graphicsMode();
  while(resume_flag==0)
  {
if(digitalRead(7)==0)
    {
       tft.textMode();
       digitalWrite(pin5,HIGH);
    //digitalWrite(pin6,LOW);
      digitalWrite(22,HIGH);
      while(digitalRead(7)==0)
      {
   digitalWrite(pin4,LOW);
//delayMicroseconds(1666);
delayMicroseconds(170);
digitalWrite(pin4,HIGH);
delayMicroseconds(170);
        //digitalWrite(pin5,HIGH);
      //digitalWrite(pin6,LOW);
      }
      digitalWrite(22,LOW);
      digitalWrite(pin5,HIGH);
    //digitalWrite(pin6,HIGH);
       tft.graphicsMode();
    }
    
    delay(600);
    if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);  
    if(tx>750 && tx<910 && ty>498 && ty<592)
    {
       tx=ty=0;
      digitalWrite(pin5,HIGH);
     // digitalWrite(pin6,LOW);
       resume_flag=1;
    }
    if(resume_flag==1)
    {
      break;
      resume_flag=0;
    }
  }
  }
   tft.textMode();
    tft.fillRoundRect(30, 100, 100, 40,5, RA8875_YELLOW);
  tft.drawRoundRect(30, 100, 100, 40,5, RA8875_BLACK);
  tft.textSetCursor(35,105 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("pause");
  tft.graphicsMode();
  resume_flag=0;
  }

    
   if(tx>756 && tx<921 && ty>316 && ty<411)
  {
   // Serial.print("stop ");
    motorstop_flag=1;
    break;
  }
/*if(motorstop_flag==1)
  {
    motorstop_flag=0;
    break;
  }*/
 // }
  }
  }
  //dubrunsec=0;
  if((dubrunsec/60)==total_time)
  {
   // motorstart_flag=0;
    timeprocess=1;
    //dubrunsec=0;
  }
// digitalWrite(pin6,HIGH);
 // myStepper.setSpeed(motorSpeed);
 // myStepper.step(stepsPerRevolution / 100);
 }
 
  void motor_countstart_function()
 {
  currmilli=0;
 //  digitalWrite(pin5,HIGH);
   digitalWrite(pin6,LOW);
    digitalWrite(pin5,HIGH);
  while(dubcounts!=total_count)
  {
    
if(millis()-currmilli>((ac/4)-1))
{
  currmilli=millis();
//opto=digitalRead(23);
 if(digitalRead(23)==1)//opto==1)
{
 //opto=0;
 encoder++;
 if(total_rpm==50)
 {
  checkencoder=50;
 }
 if(total_rpm==20)
 {
  checkencoder=47;
 }
  if(total_rpm==25)
 {
  checkencoder=48;
 }
 if(total_rpm!=50 && total_rpm!=20 && total_rpm!=25)
 {
  checkencoder=47;
 }
 if(encoder==checkencoder)
 {
  encoder=0;
dubcounts++;
 dubcounts_string[0]=(dubcounts/100)+'0';
    dubcountsc=dubcounts%100;
    dubcounts_string[1]=(dubcountsc/10)+'0';
    dubcounts_string[2]=(dubcountsc%10)+'0';
    tft.textMode();
  tft.fillRect(228,158,140,40,RA8875_WHITE);
  tft.textSetCursor(230, 160);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite(dubcounts_string);
  tft.graphicsMode();
 }
}
}
 
if(dubcounts==total_count)
  {
  //  Serial.print(motorstop_flag);
   
  //  Serial.print(motorstop_flag);
    //digitalWrite(pin5,HIGH);
   // digitalWrite(pin6,HIGH);
   // Serial.print(motorstop_flag);
  }
  if(dubcounts==total_count)
  {
  //  Serial.print(motorstop_flag);
   
  //  Serial.print(motorstop_flag);
    break;
   // Serial.print(motorstop_flag);
  }
 /* if(encoder==50)
{
 // checkt1=millis();
    dubcounts_string[0]=(dubcounts/100)+'0';
    dubcountsc=dubcounts%100;
    dubcounts_string[1]=(dubcountsc/10)+'0';
    dubcounts_string[2]=(dubcountsc%10)+'0';
    tft.textMode();
  tft.fillRect(228,158,140,40,RA8875_WHITE);
  tft.textSetCursor(230, 160);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite(dubcounts_string);
  tft.graphicsMode();
}
*/
//checkt1=millis();
//if(millis()-checkt1<900)
//{
//checkt1=millis();
digitalWrite(pin4,LOW);
//delayMicroseconds(1666);
delayMicroseconds(ac);
digitalWrite(pin4,HIGH);
delayMicroseconds(ac);
//}
/*
    if(ac>=80 && ac<=110)
    {
      for(i=0;i<6400*2;i++)
      {
digitalWrite(pin4,LOW);
//delayMicroseconds(1666);
delayMicroseconds(ac);
digitalWrite(pin4,HIGH);
delayMicroseconds(ac);
      }
      dubcounts++;
      i=0;
    }
    
    if(ac>=79 && ac<=70)
    {
      for(i=0;i<6500*2;i++)
      {
digitalWrite(pin4,LOW);
//delayMicroseconds(1666);
delayMicroseconds(ac);
digitalWrite(pin4,HIGH);
delayMicroseconds(ac);
      }
      dubcounts++;
      i=0;
    }
    if(ac>=69 && ac<=60)
    {
      for(i=0;i<6800*2;i++)
      {
digitalWrite(pin4,LOW);
//delayMicroseconds(1666);
delayMicroseconds(ac);
digitalWrite(pin4,HIGH);
delayMicroseconds(ac);
      }
      dubcounts++;
      i=0;
    }
    if(ac>=57 && ac<=59)
    {
      for(i=0;i<7000*2;i++)
      {
digitalWrite(pin4,LOW);
//delayMicroseconds(1666);
delayMicroseconds(ac);
digitalWrite(pin4,HIGH);
delayMicroseconds(ac);
      }
      dubcounts++;
      i=0;
    }
      if(ac==50)
    {
      for(i=0;i<7700*2;i++)
      {
digitalWrite(pin4,LOW);
//delayMicroseconds(1666);
delayMicroseconds(ac);
digitalWrite(pin4,HIGH);
delayMicroseconds(ac);
      }
      dubcounts++;
      i=0;
    }
      if(ac>=54 && ac<=56)
    {
      for(i=0;i<7100*2;i++)
      {
digitalWrite(pin4,LOW);
//delayMicroseconds(1666);
delayMicroseconds(ac);
digitalWrite(pin4,HIGH);
delayMicroseconds(ac);
      }
      dubcounts++;
      i=0;
    }
     if(ac>=52 && ac<=51)
    {
      for(i=0;i<7400*2;i++)
      {
digitalWrite(pin4,LOW);
//delayMicroseconds(1666);
delayMicroseconds(ac);
digitalWrite(pin4,HIGH);
delayMicroseconds(ac);
      }
      dubcounts++;
      i=0;
    }
     if(ac==53)
    {
      for(i=0;i<7300*2;i++)
      {
digitalWrite(pin4,LOW);
//delayMicroseconds(1666);
delayMicroseconds(ac);
digitalWrite(pin4,HIGH);
delayMicroseconds(ac);
      }
      dubcounts++;
      i=0;
    }
     if(ac>=48 && ac<=49)
    {
      for(i=0;i<7900*2;i++)
      {
digitalWrite(pin4,LOW);
//delayMicroseconds(1666);
delayMicroseconds(ac);
digitalWrite(pin4,HIGH);
delayMicroseconds(ac);
      }
      dubcounts++;
      i=0;
    }
     if(ac>=47 && ac<=45)
    {
      for(i=0;i<7800*2;i++)
      {
digitalWrite(pin4,LOW);
//delayMicroseconds(1666);
delayMicroseconds(ac);
digitalWrite(pin4,HIGH);
delayMicroseconds(ac);
      }
      dubcounts++;
      i=0;
    }
    if(ac>=44 && ac<=42)
    {
      for(i=0;i<8000*2;i++)
      {
digitalWrite(pin4,LOW);
//delayMicroseconds(1666);
delayMicroseconds(ac);
digitalWrite(pin4,HIGH);
delayMicroseconds(ac);
      }
      dubcounts++;
      i=0;
    }
     if(ac==41)
    {
      for(i=0;i<8200*2;i++)
      {
digitalWrite(pin4,LOW);
//delayMicroseconds(1666);
delayMicroseconds(ac);
digitalWrite(pin4,HIGH);
delayMicroseconds(ac);
      }
      dubcounts++;
      i=0;
    }
    if(ac==40)
    {
      for(i=0;i<8300*2;i++)
      {
digitalWrite(pin4,LOW);
//delayMicroseconds(1666);
delayMicroseconds(ac);
digitalWrite(pin4,HIGH);
delayMicroseconds(ac);
      }
      dubcounts++;
      i=0;
    }*/
  /*  for(c=0;c<=2;c++)
    {
    digitalWrite(pin9,LOW);
digitalWrite(pin11,LOW);
digitalWrite(pin8,LOW);
digitalWrite(pin10,HIGH);
delay(5);


//digitalWrite(pin9,HIGH);
digitalWrite(pin10,LOW);
digitalWrite(pin11,LOW);
digitalWrite(pin9,LOW);
digitalWrite(pin8,HIGH);
delay(5);


digitalWrite(pin8,LOW);
digitalWrite(pin9,LOW);
//digitalWrite(pin10,HIGH);
digitalWrite(pin10,LOW);
digitalWrite(pin11,HIGH);
delay(5);

digitalWrite(pin8,LOW);
digitalWrite(pin11,LOW);
digitalWrite(pin10,LOW);
digitalWrite(pin9,HIGH);
delay(5);

//dubcounts++;
    }
    */
   /* if(c==3)
    {
dubcounts++;
c=0;
    }*/
    

 if(millis()-checkt2>500)
{
  checkt2=millis();
  if (! digitalRead(RA8875_INT))
  {
    tft.touchRead(&tx, &ty);   
/*if(tx>143 && tx<179 && ty>763 && ty<818)
    {
   motornext_flag=1;
   break;
     // cursorxa=150;
     // i=0;
    }*/
    if(tx>750 && tx<910 && ty>498 && ty<592)
  {
    tx=ty=0;
    delayMicroseconds(110);
    delayMicroseconds(130);
    digitalWrite(pin5,HIGH);
   // digitalWrite(pin6,HIGH);
    tft.textMode();
    tft.fillRoundRect(30, 100, 100, 40,5, RA8875_YELLOW);
  tft.drawRoundRect(30, 100, 100, 40,5, RA8875_BLACK);
  tft.textSetCursor(35,105 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("Resume");
  tft.graphicsMode();
while(resume_flag==0)
  {
    if(digitalRead(7)==0)
    {
       tft.textMode();
       digitalWrite(pin5,HIGH);
  // digitalWrite(pin6,LOW);
      digitalWrite(22,HIGH);
      while(digitalRead(7)==0)
      {
   digitalWrite(pin4,LOW);
//delayMicroseconds(1666);
delayMicroseconds(170);
digitalWrite(pin4,HIGH);
delayMicroseconds(170);
        digitalWrite(pin5,HIGH);
      //digitalWrite(pin6,LOW);
      }
      digitalWrite(22,LOW);
      digitalWrite(pin5,HIGH);
    //digitalWrite(pin6,HIGH);
       tft.graphicsMode();
    }
    delay(600);
  // if(millis()-checkt2>500)
//{
//checkt2=millis();
    if (! digitalRead(RA8875_INT))
  {

  checkt2=millis();
    tft.touchRead(&tx, &ty);  
    if(tx>750 && tx<910 && ty>498 && ty<592)
    {
      tx=ty=0;
      digitalWrite(pin5,HIGH);
     // digitalWrite(pin6,LOW);
       resume_flag=1;
    }

  }
//}
  if(resume_flag==1)
    {
      break;
      //resume_flag=0;
    }
  
  }
  tft.textMode();
    tft.fillRoundRect(30, 100, 100, 40,5, RA8875_YELLOW);
  tft.drawRoundRect(30, 100, 100, 40,5, RA8875_BLACK);
  tft.textSetCursor(35,105 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("Pause");
  tft.graphicsMode();
  resume_flag=0;
  }
    
   if(tx>756 && tx<921 && ty>316 && ty<411)
  {
  //  Serial.print(motorstop_flag);
    motorstop_flag=1;
  //  Serial.print(motorstop_flag);
    break;
   // Serial.print(motorstop_flag);
  }
/*if(motorstop_flag==1)
  {
    motorstop_flag=0;
    break;
  }*/
 // }
  }
  }
  }
  //digitalWrite(pin6,HIGH);
  i=0;
 // myStepper.setSpeed(motorSpeed);
 // myStepper.step(stepsPerRevolution / 100);
 }

 void unloading_function()
 {
  dubcounts=0;
  dubrunsec=0;
  delay(900);
  /*if(digitalRead(2)==0)
  {
    motorstop_flag=1;
  }*/
  //if(motorstop_flag==1)
 // {
    if(digitalRead(7)==0)
  { 
   
   /* tft.fillScreen(RA8875_RED);
    tft.textMode();
     tft.textSetCursor(100, 50);
   tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0);
  tft.textWrite("process is Halted externally");
  tft.textSetCursor(50, 700);
   tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0);
  tft.textWrite("Don't touch anything,System is Reinitializing...");
  */
  digitalWrite(pin6,LOW);
  digitalWrite(pin5,HIGH);
 // digitalWrite(pin6,LOW);
    while(digitalRead(7)==0)
    {
digitalWrite(pin4,LOW);
//delayMicroseconds(1666);
delayMicroseconds(170);
digitalWrite(pin4,HIGH);
delayMicroseconds(170);
if(digitalRead(7)==1)
  {
    break;
  }
 // processbreak=1;
}
//digitalWrite(pin4,LOW);
//delayMicroseconds(1666);
//delayMicroseconds(ac);
//digitalWrite(pin4,LOW);
  }
  //}
 
 // else
 // {
//  if(save_drum_type==1)
//  {
    //tft.fillScreen(RA8875_GREEN);
   DateTime now = rtc.now();
    y=now.year();
   // y=2021;
    rtc_string[0]=(y/1000)+'0';
    y=y%1000;
     rtc_string[1]=(y/100)+'0';
    y=y%100;
 rtc_string[2]=(y/10)+'0';
    y=y%10;
     rtc_string[3]=y+'0';
  rtc_string[4]='/';
   m=now.month();
    //m=01;
    rtc_string[5]=(m/10)+'0';
    m=m%10;
     rtc_string[6]=m+'0';
     rtc_string[7]='/';
    d=now.day();
   // d=23;
    rtc_string[8]=(d/10)+'0';
    d=d%10;
     rtc_string[9]=d+'0';
//rtc_string[10]='/';

rtc_string[10]=' ';
rtc_string[11]=' ';
if(now.hour()>12)
{
  h=now.hour()-12;
}
else
{
    h=now.hour();
}
    //h=01;
    rtc_string[12]=(h/10)+'0';
    h=h%10;
     rtc_string[13]=h+'0';
     rtc_string[14]=':';
    mi=now.minute();
   // mi=16;
    rtc_string[15]=(mi/10)+'0';
    mi=mi%10;
     rtc_string[16]=mi+'0';
     rtc_string[17]=':';
     s=now.second();
      rtc_string[18]=(s/10)+'0';
    s=s%10;
     rtc_string[19]=s+'0';
      if(rtc_string[5]!='@' && rtc_string[8]!='@' && rtc_string[2]!='6' && rtc_string[2]!='8' && rtc_string[2]!='4' && rtc_string[1]!='1')
   {
    for(i=0;i<=19;i++)
    {
      rtc_string2[i]=rtc_string[i];
    }
     rtc_string2[i]=0;
     i=0;
 // tft.graphicsMode();
   }
     for(i=0;i<=19;i++)
     {
      historytime[i]=rtc_string2[i];
     }
     historytime[20]=0;
     i=0;
 // if(motorstart_flag==1)
 // {
 
 if(save_drum_type==1)
  {
    tft.fillScreen(RA8875_WHITE);
    tft.textMode();
     tft.textSetCursor(100, 100);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0);
  tft.textWrite("Test Completed!!!");
  tft.textSetCursor(50, 130);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0);
  tft.textWrite("Don't touch anything,System is Unloading...");
  
digitalWrite(pin5,HIGH);
//digitalWrite(pin6,LOW);
digitalWrite(22,HIGH);

  for(i=0;i<1400*2;i++)
  {
digitalWrite(pin4,HIGH);
delayMicroseconds(130);
digitalWrite(pin4,LOW);
delayMicroseconds(130);
  }
  delay(500);
  
  digitalWrite(22,LOW);
  delay(500);
  digitalWrite(22,HIGH);
  delay(500);
  for(i=0;i<3500*2;i++)
  {
digitalWrite(5,LOW);

digitalWrite(pin4,HIGH);
delayMicroseconds(130);
digitalWrite(pin4,LOW);
delayMicroseconds(130);
  }
 // delay(1500);
   delay(500);
  
  digitalWrite(22,LOW);
  delay(500);
  digitalWrite(22,HIGH);
  delay(500);
  digitalWrite(22,HIGH);
  for(i=0;i<1200*2;i++)
  {
digitalWrite(5,HIGH);

digitalWrite(pin4,HIGH);
delayMicroseconds(130);
digitalWrite(pin4,LOW);
delayMicroseconds(130);
  }
  //delay(1200);
   delay(500);
  
  digitalWrite(22,LOW);
  delay(500);
  digitalWrite(22,HIGH);
  delay(500);
  digitalWrite(22,LOW);
  for(i=0;i<1200*2;i++)
  {
digitalWrite(5,LOW);

digitalWrite(pin4,HIGH);
delayMicroseconds(130);
digitalWrite(pin4,LOW);
delayMicroseconds(130);
  }
   //delay(1500);
    delay(500);
  
  digitalWrite(22,LOW);
  delay(500);
  digitalWrite(22,HIGH);
  delay(500);
  digitalWrite(22,LOW);
  // digitalWrite(22,HIGH);
  for(i=0;i<4000*2;i++)
  {
digitalWrite(5,HIGH);

digitalWrite(pin4,HIGH);
delayMicroseconds(170);
digitalWrite(pin4,LOW);
delayMicroseconds(170);
  }
  while(digitalRead(7)==0)
  {
digitalWrite(pin4,HIGH);
delayMicroseconds(170);
digitalWrite(pin4,LOW);
delayMicroseconds(170);
  }
  digitalWrite(22,HIGH);
  delay(1000);
  digitalWrite(22,LOW);
 // }
 /* if(motorstop_flag==1 && motorstart_flag==1)
  {
    tft.textMode();
   tft.textSetCursor(100, 50);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0);
  tft.textWrite("Process is interrupted due to external events");
  tft.textSetCursor(50, 700);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0);
  tft.textWrite("Don't touch anything,System is Unloading...");
  }*/
/*  t=5;
  for(i=0;i<10;i++)
{
digitalWrite(pin9,LOW);
digitalWrite(pin11,LOW);
digitalWrite(pin8,LOW);
digitalWrite(pin10,HIGH);
delay(t);


//digitalWrite(pin9,HIGH);
digitalWrite(pin10,LOW);
digitalWrite(pin11,LOW);
digitalWrite(pin9,LOW);
digitalWrite(pin8,HIGH);
delay(t);


digitalWrite(pin8,LOW);
digitalWrite(pin9,LOW);
//digitalWrite(pin10,HIGH);
digitalWrite(pin10,LOW);
digitalWrite(pin11,HIGH);
delay(t);

digitalWrite(pin8,LOW);
digitalWrite(pin11,LOW);
digitalWrite(pin10,LOW);
digitalWrite(pin9,HIGH);
delay(t);
}
  for(i=0;i<135;i++)
{
digitalWrite(pin9,LOW);
digitalWrite(pin11,LOW);
digitalWrite(pin8,LOW);
digitalWrite(pin10,HIGH);
delay(t);


//digitalWrite(pin9,HIGH);
digitalWrite(pin10,LOW);
digitalWrite(pin11,LOW);
digitalWrite(pin9,LOW);
digitalWrite(pin8,HIGH);
delay(t);


digitalWrite(pin8,LOW);
digitalWrite(pin9,LOW);
//digitalWrite(pin10,HIGH);
digitalWrite(pin10,LOW);
digitalWrite(pin11,HIGH);
delay(t);

digitalWrite(pin8,LOW);
digitalWrite(pin11,LOW);
digitalWrite(pin10,LOW);
digitalWrite(pin9,HIGH);
delay(t);
}

delay(500);
for(i=0;i<80;i++)
{
  ////clockwise///
digitalWrite(pin8,LOW);
digitalWrite(pin9,LOW);
digitalWrite(pin10,LOW);
digitalWrite(pin11,HIGH);
delay(t);

digitalWrite(pin9,LOW);
digitalWrite(pin10,LOW);
digitalWrite(pin11,LOW);
digitalWrite(pin8,HIGH);
delay(t);

digitalWrite(pin8,LOW);
digitalWrite(pin9,LOW);
digitalWrite(pin11,LOW);
digitalWrite(pin10,HIGH);
delay(t);

digitalWrite(pin8,LOW);
digitalWrite(pin10,LOW);
digitalWrite(pin11,LOW);
digitalWrite(pin9,HIGH);
delay(t);
}
delay(500);
for(i=0;i<80;i++)
{
  digitalWrite(pin9,LOW);
digitalWrite(pin11,LOW);
digitalWrite(pin8,LOW);
digitalWrite(pin10,HIGH);
delay(t);


//digitalWrite(pin9,HIGH);
digitalWrite(pin10,LOW);
digitalWrite(pin11,LOW);
digitalWrite(pin9,LOW);
digitalWrite(pin8,HIGH);
delay(t);


digitalWrite(pin8,LOW);
digitalWrite(pin9,LOW);
//digitalWrite(pin10,HIGH);
digitalWrite(pin10,LOW);
digitalWrite(pin11,HIGH);
delay(t);

digitalWrite(pin8,LOW);
digitalWrite(pin11,LOW);
digitalWrite(pin10,LOW);
digitalWrite(pin9,HIGH);
delay(t);
}
delay(500);
for(i=0;i<80;i++)
{
  ////clockwise///
digitalWrite(pin8,LOW);
digitalWrite(pin9,LOW);
digitalWrite(pin10,LOW);
digitalWrite(pin11,HIGH);
delay(t);

digitalWrite(pin9,LOW);
digitalWrite(pin10,LOW);
digitalWrite(pin11,LOW);
digitalWrite(pin8,HIGH);
delay(t);

digitalWrite(pin8,LOW);
digitalWrite(pin9,LOW);
digitalWrite(pin11,LOW);
digitalWrite(pin10,HIGH);
delay(t);

digitalWrite(pin8,LOW);
digitalWrite(pin10,LOW);
digitalWrite(pin11,LOW);
digitalWrite(pin9,HIGH);
delay(t);
}
for(i=0;i<80;i++)
{
  digitalWrite(pin9,LOW);
digitalWrite(pin11,LOW);
digitalWrite(pin8,LOW);
digitalWrite(pin10,HIGH);
delay(t);


//digitalWrite(pin9,HIGH);
digitalWrite(pin10,LOW);
digitalWrite(pin11,LOW);
digitalWrite(pin9,LOW);
digitalWrite(pin8,HIGH);
delay(t);


digitalWrite(pin8,LOW);
digitalWrite(pin9,LOW);
//digitalWrite(pin10,HIGH);
digitalWrite(pin10,LOW);
digitalWrite(pin11,HIGH);
delay(t);

digitalWrite(pin8,LOW);
digitalWrite(pin11,LOW);
digitalWrite(pin10,LOW);
digitalWrite(pin9,HIGH);
delay(t);
}*/
  //}
  
  }
 digitalWrite(pin6,HIGH);
 delay(2000);
i=0;
  tft.graphicsMode();
  process2=1;
 }

void outtimecountweight_menu()
{
  tft.textMode();
  //tft.cursorBlink(32);
  
 // tft.drawRoundRect(7, 22, 240, 170, 20, 0xffff);//0x3041,0xc986
  /* Set the cursor location (in pixels) */
//tft.fillRect(464,12,30,251,RA8875_WHITE);

//tft.drawRect(7,19,241,171,RA8875_BLACK);
  
  tft.fillRoundRect(8, 30, 240, 170,10, 0Xffff);
  
  tft.fillRoundRect(8, 30, 240, 80,10,0X318C);
  tft.drawRoundRect(7,29,241,171,10,RA8875_BLACK);
  tft.textSetCursor(24, 40);
 
  /* Render some text! */
  
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0);
  tft.textWrite("Dedust and Weight Tablets");
///// FOR string2////// 
  tft.textSetCursor(60, 60); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("From Drum 1");

  ////entering reading value/////
  tft.fillRect(40, 120, 180, 60, 0x3f79);

   tft.textSetCursor(70,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
   
  tft.textSetCursor(85,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(100,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(115,140); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("."); 

  tft.textSetCursor(130,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(145,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
 // tft.fillRect(50, 100, 180, 60, RA8875_WHITE);
 // tft.fillCircle(140,160,50,0X3186);
 // tft.textSetCursor(170, 110); ///text location////
//  tft.textTransparent(RA8875_BLACK);
 // tft.textEnlarge(1); ////text size////
  
 // tft.textWrite(stringmin);

 /* //vertical line///
  tft.drawFastVLine(48,98,59,0X3186);
  tft.drawFastVLine(231,101,59,0X3186);
 // tft.drawFastVLine(400,00,200,RA8875_WHITE);

  //Horizontal line///
  tft.drawFastHLine(50,161,180,0X3186);
  tft.drawFastHLine(48,98,180,0X3186);
 // tft.drawFastHLine(280,200,180,RA8875_WHITE);
*/
tft.graphicsMode();
}

void outtimecountweight2_menu()
{
  tft.textMode();
  //tft.cursorBlink(32);
  
 // tft.drawRoundRect(7, 22, 240, 170, 20, 0xffff);//0x3041,0xc986
  /* Set the cursor location (in pixels) */
//tft.fillRect(464,12,30,251,RA8875_WHITE);

//tft.drawRect(7,19,241,171,RA8875_BLACK);
  
  tft.fillRoundRect(8, 30, 240, 170,10, 0Xffff);
  
  tft.fillRoundRect(8, 30, 240, 80,10,0X318C);
  tft.drawRoundRect(7,29,241,171,10,RA8875_BLACK);
  tft.textSetCursor(24, 40);
 
  /* Render some text! */
  
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0);
  tft.textWrite("Dedust and Weight Tablets");
///// FOR string2////// 
  tft.textSetCursor(60, 60); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("From Drum 2");

  ////entering reading value/////
  tft.fillRect(40, 120, 180, 60, 0x3f79);

  
         tft.textSetCursor(70,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
   
  tft.textSetCursor(85,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(100,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(115,140); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("."); 

  tft.textSetCursor(130,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(145,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

 // tft.fillRect(50, 100, 180, 60, RA8875_WHITE);
 // tft.fillCircle(140,160,50,0X3186);
 // tft.textSetCursor(170, 110); ///text location////
//  tft.textTransparent(RA8875_BLACK);
 // tft.textEnlarge(1); ////text size////
  
 // tft.textWrite(stringmin);

 /* //vertical line///
  tft.drawFastVLine(48,98,59,0X3186);
  tft.drawFastVLine(231,101,59,0X3186);
 // tft.drawFastVLine(400,00,200,RA8875_WHITE);

  //Horizontal line///
  tft.drawFastHLine(50,161,180,0X3186);
  tft.drawFastHLine(48,98,180,0X3186);
 // tft.drawFastHLine(280,200,180,RA8875_WHITE);
*/
tft.graphicsMode();
}

void outtimecountweight_function()
{
  //1//
    if(tx>360 && tx<472 && ty>713 && ty<870)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
      //tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn1);
      weight_value[i]=1;// rpm value 
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
      Serial.print("yes");
    }
   
//Serial.print("b");
    //2//
    if(tx>224 && tx<359 && ty>713 && ty<870)
    { 
      tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn2);
      weight_value[i]=2;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

      
    //3//
    if(tx>106 && tx<223 && ty>713 && ty<870)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn3);
      weight_value[i]=3;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //4//
    if(tx>360 && tx<472 && ty>579 && ty<712)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn4);
      weight_value[i]=4;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //5//
    if(tx>224 && tx<359 && ty>579 && ty<712)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn5);
      weight_value[i]=5;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //6//
    if(tx>106 && tx<223 && ty>579 && ty<712)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn6);
      weight_value[i]=6;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //7//
    if(tx>360 && tx<472 && ty>386 && ty<578)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn7);
      weight_value[i]=7;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
     i++;
    }

    //8//
    if(tx>224 && tx<359 && ty>386 && ty<578)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn8);
      weight_value[i]=8;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //9//
    if(tx>106 && tx<223 && ty>386 && ty<578)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn9);
      weight_value[i]=9;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

     //<--//
    if(tx>360 && tx<478 && ty>300 && ty<385)
    {
  
      outweightback1_flag=1;
     cursorx=70;
     i=0;
    
    }

    //0//
    if(tx>224 && tx<359 && ty>300 && ty<385)
    {
   // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_BLACK);
    // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite("0");
      weight_value[i]=0;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //-->//
    if(tx>106 && tx<223 && ty>300 && ty<385)
    {
  
     outweightnext1_flag=1;
   cursorx=70;
     i=0;
    }
    
     //save//
   if(tx>106 && tx<286 && ty>155 && ty<299)
    {  
      tft.fillRect(0, 0, 480, 15 ,RA8875_GREEN);
      tft.textMode();
   // cursorx=70;
    total_outweight1=(weight_value[0]*10000)+(weight_value[1]*1000)+(weight_value[2]*100)+(weight_value[3]*10)+weight_value[4];
  peroutw1=(weight_value[0]*10000)+(weight_value[1]*1000)+(weight_value[2]*100)+(weight_value[3]*10)+weight_value[4];
   for(i=0;i<=4;i++)
  {
  hisweightout1_value[i]=weight_value[i];
  }
    Serial.print(total_outweight1);
    //i=0;
    ///save_no_of_drumps=no_of_drumps;
   // save_drum_type=drum_type;
   // save_calculation=calculation;
   // i2=6;
    //flag=1;  
    //Serial.print("ne");
   /* if(total_time<1 && total_time>999)
    {
       
     cursorx=70;
     time_value[0]=2;
     time_value[1]=5;
     total_time=4;
     
     tft.textMode();
     tft.textSetCursor(1, 1); ///text location////
      tft.textTransparent(RA8875_RED);
     tft.textEnlarge(0);  ////text size////
      tft.textWrite("please enter value in between 20 to 50");
      
    }*/
    
    tft.graphicsMode();
    }

 ///clrea///
    if(tx>287 && tx<472 && ty>155 && ty<299)
    {
      for(i=0;i<=4;i++)
      {
      weight_value[i]=hisweightout1_value[i]=0;
      }
       total_outweight1=0;
      peroutw1=0;
      /* tft.textMode();
     tft.textSetCursor(1, 1); ///text location////
      tft.textTransparent(RA8875_RED);
     tft.textEnlarge(0);  ////text size////
      tft.textWrite("");*/
tft.fillRect(0, 0, 480, 15 , 0X0A0F);
      tft.textMode();
        tft.fillRect(40, 120, 180, 60, 0x3f79);

         tft.textSetCursor(70,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
   
  tft.textSetCursor(85,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(100,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(115,140); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("."); 

  tft.textSetCursor(130,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(145,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

      tft.graphicsMode();
    //i=i-1;
      /*count_value[0]=0;// user entered password value
       count_value[1]=0;
        count_value[2]=0;*/
      i=0;
      cursorx=70;
      
    }
     if(cursorx==115)
    {
      cursorx=130;
    }
    if(cursorx>160)
    {
       tft.fillRect(0, 0, 480, 15 ,0X0A0F);
      tft.textMode();
      tft.fillRect(40, 120, 180, 60, 0x3f79);

      tft.textSetCursor(70,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
   
  tft.textSetCursor(85,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(100,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(115,140); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("."); 

  tft.textSetCursor(130,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(145,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.graphicsMode();
      i=0;
      cursorx=70;
    }
}

void outtimecountweight2_function()
{
  //1//
    if(tx>360 && tx<472 && ty>713 && ty<870)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
      //tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn1);
      weight_value[i]=1;// rpm value 
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
      Serial.print("yes");
    }
   
//Serial.print("b");
    //2//
    if(tx>224 && tx<359 && ty>713 && ty<870)
    { 
      tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn2);
      weight_value[i]=2;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

      
    //3//
    if(tx>106 && tx<223 && ty>713 && ty<870)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn3);
      weight_value[i]=3;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //4//
    if(tx>360 && tx<472 && ty>579 && ty<712)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn4);
      weight_value[i]=4;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //5//
    if(tx>224 && tx<359 && ty>579 && ty<712)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn5);
      weight_value[i]=5;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //6//
    if(tx>106 && tx<223 && ty>579 && ty<712)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn6);
      weight_value[i]=6;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //7//
    if(tx>360 && tx<472 && ty>386 && ty<578)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn7);
      weight_value[i]=7;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
     i++;
    }

    //8//
    if(tx>224 && tx<359 && ty>386 && ty<578)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn8);
      weight_value[i]=8;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //9//
    if(tx>106 && tx<223 && ty>386 && ty<578)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn9);
      weight_value[i]=9;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

     //<--//
    if(tx>360 && tx<472 && ty>300 && ty<385)
    {
  
      outweightback2_flag=1;
     cursorx=70;
     i=0;
     ftotal_weight1_string[0]='\0';
    }

    //0//
    if(tx>224 && tx<359 && ty>300 && ty<385)
    {
   // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_BLACK);
    // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorx, cursory); ///text location////
      tft.textTransparent(RA8875_WHITE);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite("0");
      weight_value[i]=0;// time value
      cursorx=cursorx+15;
      tft.graphicsMode();
      i++;
    }

    //-->//
    if(tx>106 && tx<223 && ty>300 && ty<385)
    {
  
     outweightnext2_flag=1;
   cursorx=70;
     i=0;
     ftotal_weight1_string[0]='\0';
     
    }
    
     //save//
   if(tx>106 && tx<286 && ty>155 && ty<299)
    {  
       tft.fillRect(0, 0, 480, 15 ,RA8875_GREEN);
      tft.textMode();
   // cursorx=70;
    total_outweight2=(weight_value[0]*10000)+(weight_value[1]*1000)+(weight_value[2]*100)+(weight_value[3]*10)+weight_value[4];
 // peroutw2=total_outweight2;
  peroutw2=(weight_value[0]*10000)+(weight_value[1]*1000)+(weight_value[2]*100)+(weight_value[3]*10)+weight_value[4];
  for(i=0;i<=4;i++)
  {
  hisweightout2_value[i]=weight_value[i];
  }
   Serial.print(total_outweight2);
    //i=0;
    ///save_no_of_drumps=no_of_drumps;
   // save_drum_type=drum_type;
   // save_calculation=calculation;
   // i2=6;
    //flag=1;  
    //Serial.print("ne");
   /* if(total_time<1 && total_time>999)
    {
       
     cursorx=70;
     time_value[0]=2;
     time_value[1]=5;
     total_time=4;
     
     tft.textMode();
     tft.textSetCursor(1, 1); ///text location////
      tft.textTransparent(RA8875_RED);
     tft.textEnlarge(0);  ////text size////
      tft.textWrite("please enter value in between 20 to 50");
      
    }*/
    
    tft.graphicsMode();
    }

 ///clrea///
    if(tx>287 && tx<472 && ty>155 && ty<299)
    {
      for(i=0;i<=4;i++)
      {
      weight_value[i]=hisweightout2_value[i]=0;
      }
      total_outweight2=0;
      peroutw2=0;
      /* tft.textMode();
     tft.textSetCursor(1, 1); ///text location////
      tft.textTransparent(RA8875_RED);
     tft.textEnlarge(0);  ////text size////
      tft.textWrite("");*/
       tft.fillRect(0, 0, 480, 15 ,0X0A0F);
      tft.textMode();
        tft.fillRect(40, 120, 180, 60, 0x3f79);
         tft.textSetCursor(70,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
   
  tft.textSetCursor(85,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(100,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(115,140); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("."); 

  tft.textSetCursor(130,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(145,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
      tft.graphicsMode();
    //i=i-1;
      /*count_value[0]=0;// user entered password value
       count_value[1]=0;
        count_value[2]=0;*/
      i=0;
      cursorx=70;
      
    }
    if(cursorx==115)
    {
      cursorx=130;
    }
    if(cursorx>160)
    {
       tft.fillRect(0, 0, 480, 15 ,0X0A0F);
      tft.textMode();
      tft.fillRect(40, 120, 180, 60, 0x3f79);

      tft.textSetCursor(70,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
   
  tft.textSetCursor(85,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(100,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(115,140); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("."); 

  tft.textSetCursor(130,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.textSetCursor(145,150); ///text location////
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  tft.graphicsMode();
      i=0;
      cursorx=70;
    }
}

void calculation_result()
{
tft.fillScreen(0X379f);
//upper //
tft.fillRect(0, 0, 480, 20 , 0Xffff);

//lower
tft.fillRect(0,250, 480, 25 , 0Xffff);
 /* tft.drawRect(30, 100, 100, 40, RA8875_BLACK);
  tft.textSetCursor(35,105 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("Start");*/

  /* tft.drawRect(30, 160, 100, 40, RA8875_BLACK);
  tft.textSetCursor(35,165 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("Stop");*/

tft.textMode();
  tft.fillRoundRect(420, 20, 45, 30,4 ,0x0a0f);//0XC618
  tft.textSetCursor(431,24 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("-->");
  tft.graphicsMode();
  
/*if(tx>750 && tx<918 && ty>669 && ty<741)
  {
    resultstore_flag=1;
  }

if(tx>143 && tx<179 && ty>763 && ty<818)
    {
   resultnext_flag=1;
     // cursorxa=150;
     // i=0;
    }
  */
  if(save_no_of_drums==1)
  {
    Serial.print("TOTAL WEIGHT=");
    Serial.print(total_weight1);
    perw1=total_weight1;
    Serial.print("out==");
    Serial.print(total_outweight1);
    percentage=(float)total_outweight1/(float)total_weight1;

 //percentage= 10000.00-percentage;
    percentage=1.0-percentage;
    percentage= percentage*10000;
     percentagedub= percentage;
     per_string1[0]= (percentagedub/1000)+'0';
    percentagedub=percentagedub%1000;
     per_string1[1]= (percentagedub/100)+'0';
    percentagedub=percentagedub%100;
     per_string1[2]= (percentagedub/10)+'0';
    percentagedub=percentagedub%10;
     per_string1[3]=percentagedub+'0';
     
    ftotal_weight1=(total_weight1)-(total_outweight1);

   
    // ftotal_weight1=100000.0000-ftotal_weight1;
    Serial.print("percen=");
    Serial.print(percentage);
    Serial.print("percen2=");
    Serial.print(percentage2);
    Serial.print(total_weight1);
    Serial.print("\noutw=");
    Serial.print(total_outweight1);
    Serial.print("\nftw=");
    Serial.print(ftotal_weight1);
    ftotal_weight1_string[0]=(ftotal_weight1/10000)+'0';
    ftotal_weight1=ftotal_weight1%10000;
    ftotal_weight1_string[1]=(ftotal_weight1/1000)+'0';
    ftotal_weight1=ftotal_weight1%1000;
    ftotal_weight1_string[2]=(ftotal_weight1/100)+'0';
    ftotal_weight1=ftotal_weight1%100;
    ftotal_weight1_string[3]='.';
    ftotal_weight1_string[4]=(ftotal_weight1/10)+'0';
    ftotal_weight1=ftotal_weight1%10;
    ftotal_weight1_string[5]=ftotal_weight1+'0';


   
   // Serial.print();
    tft.textMode();
   tft.textSetCursor(50, 100); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Weight Loss in Drum1= ");
  tft.textWrite(ftotal_weight1_string);
  if(save_weight_unit==1)
  {
  tft.textWrite(" gm");
  }
  if(save_weight_unit==2)
  {
  tft.textWrite(" mg");
  }
  tft.graphicsMode();
  }
  if(save_no_of_drums==2)
  {
    ftotal_weight1=(total_weight1)-(total_outweight1);
    //ftotal_weight1=ftotal_weight1;
     percentage=(float)total_outweight1/(float)total_weight1;
   percentage=1.00-percentage;
    percentage= percentage*10000;
    
//percentage=10000.00-percentage;
 percentagedub=percentage;
    
    per_string1[0]= (percentagedub/1000)+'0';
    percentagedub=percentagedub%1000;
     per_string1[1]= (percentagedub/100)+'0';
    percentagedub=percentagedub%100;
     per_string1[2]= (percentagedub/10)+'0';
    percentagedub=percentagedub%10;
     per_string1[3]=percentagedub+'0';
     
    ftotal_weight1_string[0]=(ftotal_weight1/10000)+'0';
    ftotal_weight1=ftotal_weight1%10000;
    ftotal_weight1_string[1]=(ftotal_weight1/1000)+'0';
    ftotal_weight1=ftotal_weight1%1000;
    ftotal_weight1_string[2]=(ftotal_weight1/100)+'0';
    ftotal_weight1=ftotal_weight1%100;
    ftotal_weight1_string[3]='.';
    ftotal_weight1_string[4]=(ftotal_weight1/10)+'0';
    ftotal_weight1=ftotal_weight1%10;
    ftotal_weight1_string[5]=ftotal_weight1+'0';
   /* ftotal_weight1_string[6]=(ftotal_weight1/10);
    ftotal_weight1=ftotal_weight1%10;*/
    Serial.print("\nstr=");
     Serial.print(ftotal_weight1_string[0]);
      Serial.print(ftotal_weight1_string[1]);
      Serial.print(ftotal_weight1_string[2]);
      Serial.print(ftotal_weight1_string[3]);
      Serial.print(ftotal_weight1_string[4]);
      Serial.print(ftotal_weight1_string[5]);
      
    ftotal_weight2=(total_weight2)-(total_outweight2);
   // ftotal_weight2=ftotal_weight2-100;
     percentage2=(float)total_outweight2/(float)total_weight2;
  
     percentage2=1.00-percentage2;
    percentage2= percentage2*10000;
    // percentagedub= percentage2;

// percentage2=10000.00- percentage2;
  percentagedub2=percentage2;   
     per_string2[0]= (percentagedub2/1000)+'0';
    percentagedub2=percentagedub2%1000;
     per_string2[1]= (percentagedub2/100)+'0';
    percentagedub2=percentagedub2%100;
     per_string2[2]= (percentagedub2/10)+'0';
    percentagedub2=percentagedub2%10;
     per_string2[3]=percentagedub2+'0';
         
    ftotal_weight2_string[0]=(ftotal_weight2/10000)+'0';
    ftotal_weight2=ftotal_weight2%10000;
    ftotal_weight2_string[1]=(ftotal_weight2/1000)+'0';
    ftotal_weight2=ftotal_weight2%1000;
    ftotal_weight2_string[2]=(ftotal_weight2/100)+'0';
    ftotal_weight2=ftotal_weight2%100;
    ftotal_weight2_string[3]='.';
    ftotal_weight2_string[4]=(ftotal_weight2/10)+'0';
    ftotal_weight2=ftotal_weight2%10;
    ftotal_weight2_string[5]=ftotal_weight2+'0';
    
   /* ftotal_weight2_string[5]='.';
    ftotal_weight2_string[6]=(ftotal_weight2/10);
    ftotal_weight2=ftotal_weight1%10;*/

     tft.textMode();
   tft.textSetCursor(50, 100); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Weight Loss in Drum1= ");
  tft.textWrite(ftotal_weight1_string);
 if(save_weight_unit==1)
  {
  tft.textWrite(" gm");
  }
  if(save_weight_unit==2)
  {
  tft.textWrite(" mg");
  }
  
   tft.textSetCursor(50,120); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Weight Loss in Drum2= ");
  tft.textWrite(ftotal_weight2_string);
  if(save_weight_unit==1)
  {
  tft.textWrite(" gm");
  }
  if(save_weight_unit==2)
  {
  tft.textWrite(" mg");
  }
  tft.graphicsMode();
 
  }
}

void calculation_function()
{
  if(tx>750 && tx<918 && ty>669 && ty<741)
  {
    resultstore_flag=1;
   
  }
////////////-->/////////
if(tx>113 && tx<211 && ty>756 && ty<872)
    {
   resultnext_flag=1;
     // cursorxa=150;
     // i=0;
    }
}
  
void rtc_time()
   {
    
  if (millis() - ti >= 1000)
  {
    ti = millis();
  
    DateTime now = rtc.now();
    y=now.year();
   // y=2021;
    rtc_string[0]=(y/1000)+'0';
    y=y%1000;
     rtc_string[1]=(y/100)+'0';
    y=y%100;
 rtc_string[2]=(y/10)+'0';
    y=y%10;
     rtc_string[3]=y+'0';
  rtc_string[4]='/';
   m=now.month();
    //m=01;
    rtc_string[5]=(m/10)+'0';
    m=m%10;
     rtc_string[6]=m+'0';
     rtc_string[7]='/';
    d=now.day();
   // d=23;
    rtc_string[8]=(d/10)+'0';
    d=d%10;
     rtc_string[9]=d+'0';
//rtc_string[10]='/';

rtc_string[10]=' ';
rtc_string[11]=' ';
if(now.hour()>12)
{
  h=now.hour()-12;
}
else
{
    h=now.hour();
}
    //h=01;
    rtc_string[12]=(h/10)+'0';
    h=h%10;
     rtc_string[13]=h+'0';
     rtc_string[14]=':';
    mi=now.minute();
   // mi=16;
    rtc_string[15]=(mi/10)+'0';
    mi=mi%10;
     rtc_string[16]=mi+'0';
     rtc_string[17]=':';
   //  if(s!=now.second():
   //  {
    s=now.second();
    
 //   s=60;
    rtc_string[18]=(s/10)+'0';
    s=s%10;
     rtc_string[19]=s+'0';
   //  }
   tft.textMode();
   if(rtc_string[5]!='2' && rtc_string[5]!='@' && rtc_string[8]!='@'&& rtc_string[2]!='1' && rtc_string[2]!='6' && rtc_string[2]!='8' && rtc_string[2]!='4' && rtc_string[1]!='1')
   {
    tft.fillRect(13,1,170,18,RA8875_WHITE);
      //tft.textMode();
    tft.textSetCursor(15, 2); ///text location////
  tft.textTransparent(RA8875_BLUE);
  tft.textEnlarge(0); ////text size////
 // tft.fillRect(28,5,70,5,RA8875_WHITE);
 //tft.fillRect(28,5,155,30,RA8875_WHITE);
 //delay(1000);
  tft.textWrite(rtc_string);
    for(i=0;i<=19;i++)
    {
      rtc_string2[i]=rtc_string[i];
    }
     rtc_string2[i]=0;
     i=0;
 // tft.graphicsMode();
   }
   
   tft.graphicsMode();
///  delay(500);
 //tft.fillRect(28,5,155,30,RA8875_WHITE);
  /*  Serial.print(now.year());
    Serial.print('/');
    Serial.print(now.month());
    Serial.print('/');
    Serial.print(now.day());
   // Serial.print(" (");
   // Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print("   ");
    Serial.print(now.hour());
    Serial.print(':');
    Serial.print(now.minute());
    Serial.print(':');
    Serial.print(now.second());
    Serial.println();
    //delay(100);*/
   }
   }
   
   void login_page()
   {
    tft.textMode();

  tft.textSetCursor(150, 30);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0);
  tft.textWrite("Enter Username");
///// FOR string2////// 
  tft.drawRect(140,50,220,25,RA8875_BLACK);

  tft.graphicsMode();
   }

 void login_page2()
   {
    tft.textMode();

  tft.textSetCursor(150, 30);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0);
  tft.textWrite("Enter Password");
///// FOR string2////// 
  tft.drawRect(140,50,220,25,RA8875_BLACK);

  tft.graphicsMode();
   }

 void login_function()
 {
  int q=0;
 
          if(cursorxa>350)
          {
            tft.fillRect(140,50,220,25,RA8875_WHITE);
            tft.drawRect(140,50,220,25,RA8875_BLACK);
            cursorxa=150;
            i=0;
          }
         
   //1//
    if(tx>886 && tx<1000 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("1");
      //delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("1");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("1");
  
      loginuser[i]='1';// user entered password value 
      loginpass[i]='1'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //2//
    if(tx>792 && tx<886 && ty>528 && ty<700)
    {  
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("2");
    //  delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("2");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("2");
      loginuser[i]='2';// user entered password value
      loginpass[i]='2'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //3//
    if(tx>732 && tx<792 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("3");
      //delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("3");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("3");
      loginuser[i]='3';// user entered password value
      loginpass[i]='3'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //4//
    if(tx>641 && tx<732 && ty>528 && ty<700)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("4");
     /// delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("4");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("4");
      loginuser[i]='4';// user entered password value 
      loginpass[i]='4'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //5//
    if(tx>554 && tx<641 && ty>528 && ty<700)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("5");
     // delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("5");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("5");
      loginuser[i]='5';// user entered password value
      loginpass[i]='5'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //6//
     if(tx>467 && tx<554 && ty>528 && ty<700)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("6");
      //delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("6");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("6");
      loginuser[i]='6';// user entered password value
      loginpass[i]='6'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //7//
    if(tx>395 && tx<467 && ty>528 && ty<700)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("7");
      //delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("7");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("7");
      loginuser[i]='7';// user entered password value
      loginpass[i]='7'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //8//
     if(tx>308 && tx<395 && ty>528 && ty<700)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("8");
      //delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("8");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("8");
      loginuser[i]='8';// user entered password value
      loginpass[i]='8'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //9//
    if(tx>238 && tx<308 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("9");
      //delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("9");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("9");
      loginuser[i]='9';// user entered password value
      loginpass[i]='9'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //0//
    if(tx>138 && tx<238 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_BLACK);
    // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("0");
      //delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("0");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("0");
      loginuser[i]='0';// user entered password value
      loginpass[i]='0'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
      Serial.print("ipf0");
    }

////abcd////

 if(key==1)
          {
//q//
   if(tx>873 && tx<977 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("q");
      //elay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("q");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("q");
  
      loginuser[i]='q';// user entered password value 
      loginpass[i]='q'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //w//
   if(tx>787 && tx<872 && ty>374 && ty<528)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("w");
      //delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("w");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("w");
      loginuser[i]='w';// user entered password value
      loginpass[i]='w'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //e//
    if(tx>705 && tx<786 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("e");
     // delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("e");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("e");
      loginuser[i]='e';// user entered password value
      loginpass[i]='e'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //r//
     if(tx>614 && tx<704 && ty>374 && ty<528)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("r");
      //delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("r");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("r");
      loginuser[i]='r';// user entered password value 
      loginpass[i]='r'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //t//
     if(tx>524 && tx<613 && ty>374 && ty<528)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("t");
      //delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("t");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("t");
      loginuser[i]='t';// user entered password value
      loginpass[i]='t'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //y//
    if(tx>437 && tx<523 && ty>374 && ty<528)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("y");
     // delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("y");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("y");
      loginuser[i]='y';// user entered password value
      loginpass[i]='y'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //u//
    if(tx>368 && tx<436 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("u");
      //delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("u");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("u");
      loginuser[i]='u';// user entered password value
      loginpass[i]='u'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //i//
    if(tx>263 && tx<368 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("i");
     //delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("i");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("i");
      loginuser[i]='i';// user entered password value
      loginpass[i]='i'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //o//
    if(tx>178 && tx<262 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("o");
      //elay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("o");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("o");
      loginuser[i]='o';// user entered password value
      loginpass[i]='o'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//p//
    if(tx>113 && tx<177 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("p");
     //delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("p");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("p");
  
      loginuser[i]='p';// user entered password value 
      loginpass[i]='p'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //a//
     if(tx>846 && tx<1000 && ty>233 && ty<374)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("a");
      //elay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("a");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("a");
      loginuser[i]='a';// user entered password value
      loginpass[i]='a'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //s//
     if(tx>792 && tx<846 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("s");
     //delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("s");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("s");
      loginuser[i]='s';// user entered password value
      loginpass[i]='s'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //d//
    if(tx>732 && tx<792 && ty>233 && ty<374)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("d");
     //delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("d");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("d");
      loginuser[i]='d';// user entered password value 
      loginpass[i]='d'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //f//
     if(tx>641 && tx<732 && ty>233 && ty<374)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("f");
     // delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("f");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("f");
      loginuser[i]='f';// user entered password value
      loginpass[i]='f'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //g//
    if(tx>554 && tx<641 && ty>233 && ty<374)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("g");
      //delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("g");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("g");
      loginuser[i]='g';// user entered password value
      loginpass[i]='g'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //h//
    if(tx>467 && tx<554 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("h");
      //elay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("h");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("h");
      loginuser[i]='h';// user entered password value
      loginpass[i]='h'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //j//
   if(tx>395 && tx<467 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("j");
      //elay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("j");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("j");
      loginuser[i]='j';// user entered password value
      loginpass[i]='j'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //k//
    if(tx>308 && tx<395 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("k");
      //delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("k");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("k");
      loginuser[i]='k';// user entered password value
      loginpass[i]='k'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//l//
    if(tx>238 && tx<308 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("l");
      //delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("l");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("l");
  
      loginuser[i]='l';// user entered password value 
      loginpass[i]='l'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //z//
     if(tx>138 && tx<238 && ty>233 && ty<374)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("z");
      //delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("z");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("z");
      loginuser[i]='z';// user entered password value
      loginpass[i]='z'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //x//
    if(tx>787 && tx<872 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("x");
      //delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("x");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("x");
      loginuser[i]='x';// user entered password value
      loginpass[i]='x'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //c//
    if(tx>704 && tx<787 && ty>89 && ty<233)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("c");
      //delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("c");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("c");
      loginuser[i]='c';// user entered password value 
      loginpass[i]='c'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //v//
    if(tx>614 && tx<704 && ty>89 && ty<233)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("v");
     // delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("v");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("v");
      loginuser[i]='v';// user entered password value
      loginpass[i]='v'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //b//
    if(tx>437 && tx<523 && ty>89 && ty<233)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("b");
      //delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("b");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("b");
      loginuser[i]='b';// user entered password value
      loginpass[i]='b'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

  /*  //space//
    if(tx>460 && tx<600 && ty>152 && ty<253)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      userenter[i]=' ';// user entered password value
      reenteredpass[i]=7; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }
*/
    //n//
     if(tx>368 && tx<436 && ty>89 && ty<233)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("n");
      //delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("n");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("n");
      loginuser[i]='n';// user entered password value
      loginpass[i]='n'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //m//
    if(tx>263 && tx<367 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("m");
     // delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("m");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("m");
      loginuser[i]='m';// user entered password value
      loginpass[i]='m'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
          }
          ////ABCD////

 if(key==2)
          {
//Q//
    if(tx>873 && tx<977 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Q");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Q");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Q");
  
      loginuser[i]='Q';// user entered password value 
      loginpass[i]='Q'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //W//
    if(tx>787 && tx<872 && ty>374 && ty<528)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("W");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("W");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("W");
      loginuser[i]='W';// user entered password value
      loginpass[i]='W'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //E//
    if(tx>705 && tx<786 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("E");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("E");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("E");
      loginuser[i]='E';// user entered password value
      loginpass[i]='E'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //R//
    if(tx>614 && tx<704 && ty>374 && ty<528)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("R");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("R");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("R");
      loginuser[i]='R';// user entered password value 
      loginpass[i]='R'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //T//
    if(tx>524 && tx<613 && ty>374 && ty<528)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("T");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("T");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("T");
      loginuser[i]='T';// user entered password value
      loginpass[i]='T'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //Y//
    if(tx>437 && tx<523 && ty>374 && ty<528)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Y");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Y");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Y");
      loginuser[i]='Y';// user entered password value
      loginpass[i]='Y'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //U//
    if(tx>368 && tx<436 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("U");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("U");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("U");
      loginuser[i]='U';// user entered password value
      loginpass[i]='U'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //I//
   if(tx>263 && tx<367 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("I");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("I");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("I");
      loginuser[i]='I';// user entered password value
      loginpass[i]='I'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //O//
    if(tx>178 && tx<262 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("O");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("O");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("O");
      loginuser[i]='O';// user entered password value
      loginpass[i]='O'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//P//
    if(tx>113 && tx<177 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("P");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("P");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("P");
  
      loginuser[i]='P';// user entered password value 
      loginpass[i]='P'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //A//
    if(tx>846 && tx<1000 && ty>233 && ty<374)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("A");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("A");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("A");
      loginuser[i]='A';// user entered password value
      loginpass[i]='A'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //S//
    if(tx>792 && tx<846 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("S");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("S");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("S");
      loginuser[i]='S';// user entered password value
      loginpass[i]='S'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //D//
     if(tx>732 && tx<792 && ty>233 && ty<374)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("D");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("D");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("D");
      loginuser[i]='D';// user entered password value 
      loginpass[i]='D'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //F//
    if(tx>641 && tx<732 && ty>233 && ty<374)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("F");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("F");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("F");
      loginuser[i]='F';// user entered password value
      loginpass[i]='F'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //G//
    if(tx>554 && tx<641 && ty>233 && ty<374)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("G");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("G");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("G");
      loginuser[i]='G';// user entered password value
      loginpass[i]='G'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //H//
     if(tx>467 && tx<554 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("H");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("H");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("H");
      loginuser[i]='H';// user entered password value
      loginpass[i]='H'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //J//
    if(tx>395 && tx<467 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("j");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("J");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("J");
      loginuser[i]='J';// user entered password value
      loginpass[i]='J'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //K//
    if(tx>308 && tx<395 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("K");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("K");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("K");
      loginuser[i]='K';// user entered password value
      loginpass[i]='K'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//L//
    if(tx>238 && tx<308 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("L");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("L");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("L");
  
      loginuser[i]='L';// user entered password value 
      loginpass[i]='L'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //Z//
    if(tx>138 && tx<238 && ty>233 && ty<374)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Z");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Z");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Z");
      loginuser[i]='Z';// user entered password value
      loginpass[i]='Z'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //X//
    if(tx>787 && tx<872 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("X");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("X");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("X");
      loginuser[i]='X';// user entered password value
      loginpass[i]='X'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //C//
    if(tx>704 && tx<787 && ty>89 && ty<233)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("C");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("C");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("C");
      loginuser[i]='C';// user entered password value 
      loginpass[i]='C'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //V//
    if(tx>614 && tx<704 && ty>89 && ty<233)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("V");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("V");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("V");
      loginuser[i]='V';// user entered password value
      loginpass[i]='V'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //B//
    if(tx>437 && tx<523 && ty>89 && ty<233)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("B");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("B");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("B");
      loginuser[i]='B';// user entered password value
      loginpass[i]='B'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

  /*  //space//
    if(tx>460 && tx<600 && ty>152 && ty<253)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      userenter[i]=' ';// user entered password value
      reenteredpass[i]=7; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }
*/
    //N//
     if(tx>368 && tx<436 && ty>89 && ty<233)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("N");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("N");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("N");
      loginuser[i]='N';// user entered password value
      loginpass[i]='N'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //M//
    if(tx>263 && tx<367 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("M");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("M");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("M");
      loginuser[i]='M';// user entered password value
      loginpass[i]='M'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
          }
    
//capslock//
    if(tx>178 && tx<262 && ty>89 && ty<233)
    {
      capslock=1;
      delay(100);
    }

    ////<--////
     if(tx>881 && tx<953 && ty>780 && ty<864)
    {
     loginback=1;
     key=1;
     //for(q=0;q<=20;q++)
    // {
     loginuser[0]='\0';// user entered password value
    // }
    // for(q=0;q<=8;q++)
    // {
    loginpass[0]='\0';
    // }
    cursorxa=150;
    i=0;
    clr_button[0]='\0';
     Serial.print("adminback_flag=1 ");
    }
/*
    ////-->////
    if(tx>143 && tx<179 && ty>763 && ty<818)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
     // tft.textEnlarge(0);  ////text size////
    //  tft.textWrite("9");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
     // tft.textEnlarge(0);  ////text size////
     // tft.textWrite("9");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
    //  tft.textEnlarge(0);  ////text size////
     // tft.textWrite("*");
     // userenter[i]='9';// user entered password value
      //reenteredpass[i]=9; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     // i++;
    }

  // }
 /*  if(loginenter==1)
   {
    break;
   }
  /* if(adminback_flag==1)
   {
    break;
   }*/
    // }
     //capslock//
    if(tx>178 && tx<262 && ty>89 && ty<233)
    {
      capslock=1;
      delay(100);
    }
 /*   ////<--////
    if(tx>885 && tx<953 && ty>746 && ty<795)
    {
     loginback=1;
    }*/
     ///clear////
   if(tx>873 && tx<1000 && ty>89 && ty<233)
     { 
    // tft.touchEnable(true);
     //tft.fillRect(57, 120, 150, 58, RA8875_RED);
     tft.textMode();
   //  tft.cursorBlink(32);
     cursorxa=cursorxa-10;
     tft.textSetCursor(cursorxa,cursorya); ///text location////
     tft.textTransparent(RA8875_WHITE);
     tft.textEnlarge(0);  ////text size////
   //  tft.textWrite("*");
    // tft.textSetCursor(cursorxa, cursorya); ///text location////
     // i=i-1;
     i=i-1;
     clr_button[0]=loginuser[i];
    clr_button[1]='\0';
     tft.textWrite(clr_button);
      loginuser[i]='\0';// user entered password value
   //  userenter[1]=0;// user entered password value
    // userenter[2]=0;// user entered password value
    // userenter[3]=0;// user entered password value
      
     loginpass[i]='\0'; //to change password
     //reenteredpass[i]=0; //to change password
     //reenteredpass[i]=0; //to change password
     //reenteredpass[i]=0; //to change password
    // i=i-1;
     if(cursorxa<150)
     {
      cursorxa=150;
      i=0;
       loginpass[i]='\0';
        loginuser[i]='\0';
     }
     tft.graphicsMode();
     Serial.print("user=");
     Serial.print(loginuser[i]);
     Serial.print("\ni=");
     Serial.print(i);
  }
  
  
    //enter//
     if(tx>113 && tx<177 && ty>89 && ty<233)
    {
      tft.fillRect(140,50,220,25,RA8875_WHITE);
            tft.drawRect(140,50,220,25,RA8875_BLACK);
            loginuser[i]='\0';
            loginpass[i]='\0';
            Serial.print("Lp1=");
            Serial.print(i);
    cursorxa=150;
    i=0;
    i2=12;
    loginenter=1;  
    
    Serial.print("ne");
    }

    while(tft.touched())
    {
      tft.touchRead(&txc, &tyc);
      txc=tyc=0;
      Serial.print("2");
      delay(100);
    }
}

void login_function2()
 {
  int q=0;
 Serial.print(i);
          if(cursorxa>350)
          {
            tft.fillRect(140,50,220,25,RA8875_WHITE);
            tft.drawRect(140,50,220,25,RA8875_BLACK);
            cursorxa=150;
            i=0;
          }
         
   //1//
    if(tx>886 && tx<1000 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("1");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("1");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
  
      loginuser[i]='1';// user entered password value 
      loginpass[i]='1'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //2//
    if(tx>792 && tx<886 && ty>528 && ty<700)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("2");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("2");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='2';// user entered password value
      loginpass[i]='2'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //3//
    if(tx>732 && tx<792 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("3");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("3");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='3';// user entered password value
      loginpass[i]='3'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //4//
    if(tx>641 && tx<732 && ty>528 && ty<700)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("4");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("4");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='4';// user entered password value 
      loginpass[i]='4'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //5//
    if(tx>554 && tx<641 && ty>528 && ty<700)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("5");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("5");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='5';// user entered password value
      loginpass[i]='5'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //6//
    if(tx>467 && tx<554 && ty>528 && ty<700)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("6");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("6");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='6';// user entered password value
      loginpass[i]='6'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //7//
    if(tx>395 && tx<467 && ty>528 && ty<700)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("7");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("7");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='7';// user entered password value
      loginpass[i]='7'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //8//
    if(tx>308 && tx<395 && ty>528 && ty<700)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("8");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("8");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='8';// user entered password value
      loginpass[i]='8'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //9//
    if(tx>238 && tx<308 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("9");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("9");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='9';// user entered password value
      loginpass[i]='9'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //0//
    if(tx>138 && tx<238 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_BLACK);
    // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("0");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("0");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='0';// user entered password value
      loginpass[i]='0'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
      Serial.print("ipf0");
    }

////abcd////

 if(key==1)
          {
//q//
    if(tx>873 && tx<977 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("q");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("q");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
  
      loginuser[i]='q';// user entered password value 
      loginpass[i]='q'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //w//
    if(tx>787 && tx<872 && ty>374 && ty<528)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("w");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("w");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='w';// user entered password value
      loginpass[i]='w'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //e//
    if(tx>705 && tx<786 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("e");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("e");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='e';// user entered password value
      loginpass[i]='e'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //r//
    if(tx>614 && tx<704 && ty>374 && ty<528)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("r");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("r");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='r';// user entered password value 
      loginpass[i]='r'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //t//
    if(tx>524 && tx<613 && ty>374 && ty<528)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("t");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("t");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='t';// user entered password value
      loginpass[i]='t'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //y//
     if(tx>437 && tx<523 && ty>374 && ty<528)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("y");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("y");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='y';// user entered password value
      loginpass[i]='y'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //u//
    if(tx>368 && tx<436 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("u");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("u");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='u';// user entered password value
      loginpass[i]='u'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //i//
    if(tx>263 && tx<368 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("i");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("i");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='i';// user entered password value
      loginpass[i]='i'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //o//
    if(tx>178 && tx<262 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("o");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("o");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='o';// user entered password value
      loginpass[i]='o'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//p//
    if(tx>113 && tx<177 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("p");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("p");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
  
      loginuser[i]='p';// user entered password value 
      loginpass[i]='p'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //a//
    if(tx>846 && tx<1000 && ty>233 && ty<374)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("a");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("a");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='a';// user entered password value
      loginpass[i]='a'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //s//
    if(tx>792 && tx<846 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("s");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("s");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='s';// user entered password value
      loginpass[i]='s'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //d//
    if(tx>732 && tx<792 && ty>233 && ty<374)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("d");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("d");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='d';// user entered password value 
      loginpass[i]='d'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //f//
    if(tx>641 && tx<732 && ty>233 && ty<374)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("f");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("f");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='f';// user entered password value
      loginpass[i]='f'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //g//
    if(tx>554 && tx<641 && ty>233 && ty<374)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("g");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("g");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='g';// user entered password value
      loginpass[i]='g'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //h//
    if(tx>467 && tx<554 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("h");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("h");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='h';// user entered password value
      loginpass[i]='h'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //j//
    if(tx>395 && tx<467 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("j");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("j");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='j';// user entered password value
      loginpass[i]='j'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //k//
    if(tx>308 && tx<395 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("k");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("k");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='k';// user entered password value
      loginpass[i]='k'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//l//
     if(tx>238 && tx<308 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("l");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("l");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
  
      loginuser[i]='l';// user entered password value 
      loginpass[i]='l'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //z//
     if(tx>138 && tx<238 && ty>233 && ty<374)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("z");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("z");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='z';// user entered password value
      loginpass[i]='z'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //x//
    if(tx>787 && tx<872 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("x");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("x");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='x';// user entered password value
      loginpass[i]='x'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //c//
     if(tx>704 && tx<787 && ty>89 && ty<233)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("c");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("c");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='c';// user entered password value 
      loginpass[i]='c'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //v//
     if(tx>614 && tx<704 && ty>89 && ty<233)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("v");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("v");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='v';// user entered password value
      loginpass[i]='v'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //b//
    if(tx>437 && tx<523 && ty>89 && ty<233)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("b");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("b");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='b';// user entered password value
      loginpass[i]='b'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

  /*  //space//
    if(tx>460 && tx<600 && ty>152 && ty<253)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      userenter[i]=' ';// user entered password value
      reenteredpass[i]=7; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }
*/
    //n//
    if(tx>368 && tx<436 && ty>89 && ty<233)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("n");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("n");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='n';// user entered password value
      loginpass[i]='n'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //m//
      if(tx>263 && tx<367 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("m");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("m");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='m';// user entered password value
      loginpass[i]='m'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
          }
          ////ABCD////

 if(key==2)
          {
//Q//
     if(tx>873 && tx<977 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Q");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Q");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
  
      loginuser[i]='Q';// user entered password value 
      loginpass[i]='Q'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //W//
    if(tx>787 && tx<872 && ty>374 && ty<528)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("W");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("W");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='W';// user entered password value
      loginpass[i]='W'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //E//
     if(tx>705 && tx<786 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("E");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("E");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='E';// user entered password value
      loginpass[i]='E'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //R//
   if(tx>614 && tx<704 && ty>374 && ty<528)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("R");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("R");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='R';// user entered password value 
      loginpass[i]='R'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //T//
     if(tx>524 && tx<613 && ty>374 && ty<528)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("T");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("T");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='T';// user entered password value
      loginpass[i]='T'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //Y//
     if(tx>437 && tx<523 && ty>374 && ty<528)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Y");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Y");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='Y';// user entered password value
      loginpass[i]='Y'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //U//
    if(tx>368 && tx<436 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("U");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("U");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='U';// user entered password value
      loginpass[i]='U'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //I//
   if(tx>263 && tx<367 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("I");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("I");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='I';// user entered password value
      loginpass[i]='I'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //O//
    if(tx>178 && tx<262 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("O");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("O");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='O';// user entered password value
      loginpass[i]='O'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//P//
    if(tx>113 && tx<177 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("P");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("P");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
  
      loginuser[i]='P';// user entered password value 
      loginpass[i]='P'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //A//
    if(tx>846 && tx<1000 && ty>233 && ty<374)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("A");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("A");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='A';// user entered password value
      loginpass[i]='A'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //S//
    if(tx>792 && tx<846 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("S");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("S");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='S';// user entered password value
      loginpass[i]='S'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //D//
    if(tx>732 && tx<792 && ty>233 && ty<374)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("D");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("D");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='D';// user entered password value 
      loginpass[i]='D'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //F//
     if(tx>641 && tx<732 && ty>233 && ty<374)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("F");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("F");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='F';// user entered password value
      loginpass[i]='F'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //G//
    if(tx>554 && tx<641 && ty>233 && ty<374)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("G");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("G");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='G';// user entered password value
      loginpass[i]='G'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //H//
   if(tx>467 && tx<554 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("H");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("H");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='H';// user entered password value
      loginpass[i]='H'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //J//
    if(tx>395 && tx<467 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("j");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("J");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='J';// user entered password value
      loginpass[i]='J'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //K//
    if(tx>308 && tx<395 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("K");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("K");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='K';// user entered password value
      loginpass[i]='K'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//L//
    if(tx>238 && tx<308 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("L");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("L");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
  
      loginuser[i]='L';// user entered password value 
      loginpass[i]='L'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //Z//
    if(tx>138 && tx<238 && ty>233 && ty<374)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Z");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Z");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='Z';// user entered password value
      loginpass[i]='Z'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //X//
    if(tx>787 && tx<872 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("X");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("X");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='X';// user entered password value
      loginpass[i]='X'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //C//
     if(tx>704 && tx<787 && ty>89 && ty<233)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("C");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("C");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='C';// user entered password value 
      loginpass[i]='C'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //V//
     if(tx>614 && tx<704 && ty>89 && ty<233)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("V");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("V");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='V';// user entered password value
      loginpass[i]='V'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //B//
    if(tx>437 && tx<523 && ty>89 && ty<233)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("B");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("B");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='B';// user entered password value
      loginpass[i]='B'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

  /*  //space//
    if(tx>460 && tx<600 && ty>152 && ty<253)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      userenter[i]=' ';// user entered password value
      reenteredpass[i]=7; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }
*/
    //N//
    if(tx>368 && tx<436 && ty>89 && ty<233)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("N");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("N");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='N';// user entered password value
      loginpass[i]='N'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //M//
    if(tx>263 && tx<367 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("M");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("M");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      loginuser[i]='M';// user entered password value
      loginpass[i]='M'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
          }
    
//capslock//
    if(tx>178 && tx<262 && ty>89 && ty<233)
    {
      capslock=1;
      delay(100);
    }

    ////<--////
   if(tx>881 && tx<953 && ty>780 && ty<864)
    {
      cursorxa=150;
     loginback2=1;
     key=1;
     //for(q=0;q<=20;q++)
    // {
     loginuser[0]='\0';// user entered password value
    // }
    // for(q=0;q<=8;q++)
    // {
    loginpass[0]='\0';
    // }
    cursorxa=150;
    i=0;
     Serial.print("adminback_flag=1 ");
    }
/*
    ////-->////
    if(tx>143 && tx<179 && ty>763 && ty<818)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
     // tft.textEnlarge(0);  ////text size////
    //  tft.textWrite("9");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
     // tft.textEnlarge(0);  ////text size////
     // tft.textWrite("9");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
    //  tft.textEnlarge(0);  ////text size////
     // tft.textWrite("*");
     // userenter[i]='9';// user entered password value
      //reenteredpass[i]=9; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     // i++;
    }

  // }
 /*  if(loginenter==1)
   {
    break;
   }
  /* if(adminback_flag==1)
   {
    break;
   }*/
    // }
     //capslock//
   if(tx>178 && tx<262 && ty>89 && ty<233)
    {
      capslock=1;
      delay(100);
    }
 /*   ////<--////
    if(tx>885 && tx<953 && ty>746 && ty<795)
    {
     loginback=1;
    }*/
     ///clear////
   if(tx>873 && tx<1000 && ty>89 && ty<233)
     {
      
    // tft.touchEnable(true);
     //tft.fillRect(57, 120, 150, 58, RA8875_RED);
     tft.textMode();
   //  tft.cursorBlink(32);
     cursorxa=cursorxa-10;
     tft.textSetCursor(cursorxa,cursorya); ///text location////
     tft.textTransparent(RA8875_WHITE);
     tft.textEnlarge(0);  ////text size////
     tft.textWrite("*");
    // tft.textSetCursor(cursorxa, cursorya); ///text location////
     // i=i-1;
     i=i-1;
      loginuser[i]='\0';// user entered password value
   //  userenter[1]=0;// user entered password value
    // userenter[2]=0;// user entered password value
    // userenter[3]=0;// user entered password value
      
     loginpass[i]='\0'; //to change password
     //reenteredpass[i]=0; //to change password
     //reenteredpass[i]=0; //to change password
     //reenteredpass[i]=0; //to change password
    // i=i-1;
     if(cursorxa<150)
     {
      cursorxa=150;
      i=0;
       loginpass[i]='\0';
        loginuser[i]='\0';
     }
     tft.graphicsMode();
     Serial.print("user=");
     Serial.print(loginuser[i]);
     Serial.print("\ni=");
     Serial.print(i);
  }
  
  
    //enter//
   if(tx>113 && tx<177 && ty>89 && ty<233)
    {
      tft.fillRect(140,50,220,25,RA8875_WHITE);
            tft.drawRect(140,50,220,25,RA8875_BLACK);
            loginuser[i]='\0';
            loginpass[i]='\0';
             Serial.print("checku i=");
  Serial.print(i);
    cursorxa=150;
    i=0;
    i2=12;
    loginenter2=1;  
    
    Serial.print("ne");
    }
    while(tft.touched())
    {
      tft.touchRead(&txc, &tyc);
      txc=tyc=0;
      Serial.print("2");
      delay(100);
    }
}
 
 void settings_menu()
 {
  tft.textMode();
  //tft.cursorBlink(32);
  
 // tft.drawRoundRect(7, 22, 240, 170, 20, 0xffff);//0x3041,0xc986
  /* Set the cursor location (in pixels) */
//tft.fillRect(464,12,30,251,RA8875_WHITE);
tft.fillRect(0, 0, 480, 15 , 0X0A0F);
//tft.fillRect(252,12,5,251,RA8875_WHITE);
  tft.drawRoundRect(29, 29, 207, 224,10, 0X0000);
  tft.fillRoundRect(30, 30, 205, 223,10, 0X379f);
  
  tft.fillRoundRect(30, 30, 205, 30,10,0X318C);
  tft.textSetCursor(50, 35);
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0);
  tft.textWrite("Enter Parameters");


  tft.textSetCursor(40, 80);
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0);
  tft.textWrite("SET RPM:");
  ////entering reading value/////
  tft.fillRect(150, 75, 55, 30, RA8875_WHITE);
  //default value//
  stringset_rpm[0]=(total_rpm/10)+'0';
  stringset_rpm[1]=(total_rpm%10)+'0';
  tft.textSetCursor(160, 70);
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite(stringset_rpm);
 
  tft.textSetCursor(40,120 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("No.of Drums: ");
  ////entering reading value/////
  tft.fillRect(150, 115, 30, 30, RA8875_WHITE);
  tft.textSetCursor(153,115 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("1");
  
  tft.fillRect(185, 115, 30, 30, RA8875_WHITE);
  tft.textSetCursor(190,115 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("2");

  tft.textSetCursor(40,175 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Drum Type: ");
  ////entering reading value/////
  tft.fillRect(150, 155, 80, 25, RA8875_WHITE);
  tft.textSetCursor(150,155 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Friability");
  
  tft.fillRect(150, 185, 80, 25, RA8875_WHITE);
  tft.textSetCursor(151,185 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Abrasion");


  tft.textSetCursor(40,220 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Calculation: ");
  ////entering reading value/////
  tft.fillRect(150, 220, 30, 30, RA8875_WHITE);
  tft.textSetCursor(151,225 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Yes");
  
  tft.fillRect(185, 220, 30, 30, RA8875_WHITE);
  tft.textSetCursor(188,225 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("No");

tft.graphicsMode();
}

void settings_menu_function()
{
  
   
  //no.of drumps
  if(tx>649 && tx<707 && ty>443 && ty>508)
  {
    tft.textMode();
    
    tft.fillRect(150, 115, 30, 30, RA8875_GREEN);
  tft.textSetCursor(153,115 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("1");
   //no_of_drumps=1;

   if(no_of_drums==2)
   {
   tft.fillRect(185, 115, 30, 30, RA8875_WHITE);
  tft.textSetCursor(190,115 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("2");
  // no_of_drumps=1;
   }
   no_of_drums=1;
    tft.graphicsMode();
  }
  if(tx>593 && tx<635 && ty>447 && ty<511)
  {
    tft.textMode();
    
    tft.fillRect(185, 115, 30, 30, RA8875_GREEN);
  tft.textSetCursor(190,115 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("2");
 // no_of_drumps=2;

  if(no_of_drums==1)
  {
    tft.fillRect(150, 115, 30, 30, RA8875_WHITE);
  tft.textSetCursor(153,115 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("1");
 // no_of_drumps=2;

  }
  no_of_drums=2;
    tft.graphicsMode();
  }

  //drum type
  if(tx>559 && tx<698 && ty>386 && ty<433)
  {
    tft.textMode();
    
     tft.fillRect(150, 155, 80, 25, RA8875_GREEN);
  tft.textSetCursor(150,155 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Friability");

  if( drum_type==2)
  {
    tft.fillRect(150, 185, 80, 25, RA8875_WHITE);
  tft.textSetCursor(151,185 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Abrasion");

  }
  
  drum_type=1;
  
  tft.graphicsMode();
  }
  if(tx>565 && tx<703 && ty>296 && ty<342)
  {
    tft.textMode();
    
    tft.fillRect(150, 185, 80, 25, RA8875_GREEN);
  tft.textSetCursor(151,185 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Abrasion");

  if( drum_type==1)
  {
     tft.fillRect(150, 155, 80, 25, RA8875_WHITE);
  tft.textSetCursor(150,155 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Friability");
  }
  
  drum_type=2;
  
  tft.graphicsMode();
  }

  ///calculation
  if(tx>662 && ty<704 && ty>132 && ty<239)
  { 
    tft.textMode();
    
    tft.fillRect(150, 220, 30, 30, RA8875_GREEN);
  tft.textSetCursor(151,225 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Yes");

  if( calculation==2)
  {
    tft.fillRect(185, 220, 30, 30, RA8875_WHITE);
  tft.textSetCursor(188,225 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("No");
  }
  
  calculation=1;
  
  tft.graphicsMode();
  }
  if(tx>603 && tx<638 && ty>194 && ty<244)
  {
     tft.textMode();
     
    tft.fillRect(185, 220, 30, 30, RA8875_GREEN);
  tft.textSetCursor(188,225 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("No");

  if( calculation==1)
  {
     tft.fillRect(150, 220, 30, 30, RA8875_WHITE);
  tft.textSetCursor(151,225 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Yes");
  }
  
  calculation=2;
  
  tft.graphicsMode();
  }
  
}


 void settings_keyfunction()
{
  
 if(settingsclr==1)
 {         
   //1//
    if(tx>360 && tx<472 && ty>713 && ty<870)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
      //tft.cursorBlink(32);
      tft.textSetCursor(cursorxrpm, cursoryrpm); ///text location////
      tft.textTransparent(RA8875_GREEN);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn1);
      rpm_value[rpm]=1;// rpm value 
      cursorxrpm=cursorxrpm+15;
      tft.graphicsMode();
      rpm++;
      Serial.print(rpm_value[i]);
    }
   
//Serial.print("b");
    //2//
    if(tx>224 && tx<359 && ty>713 && ty<870)
    { 
      tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorxrpm, cursoryrpm); ///text location////
      tft.textTransparent(RA8875_GREEN);
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn2);
      rpm_value[rpm]=2;// time value
       Serial.print(rpm_value[i]);
      cursorxrpm=cursorxrpm+15;
      tft.graphicsMode();
      rpm++;
    }

      
    //3//
    if(tx>106 && tx<223 && ty>713 && ty<870)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorxrpm, cursoryrpm); ///text location////
      tft.textTransparent(RA8875_GREEN);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn3);
      rpm_value[rpm]=3;// time value
       Serial.print(rpm_value[i]);
      cursorxrpm=cursorxrpm+15;
      tft.graphicsMode();
      rpm++;
    }

    //4//
    if(tx>360 && tx<472 && ty>579 && ty<712)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorxrpm, cursoryrpm); ///text location////
      tft.textTransparent(RA8875_GREEN);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn4);
      rpm_value[rpm]=4;// time value
       Serial.print(rpm_value[i]);
      cursorxrpm=cursorxrpm+15;
      tft.graphicsMode();
      rpm++;
    }

    //5//
    if(tx>224 && tx<359 && ty>579 && ty<712)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxrpm, cursoryrpm); ///text location////
      tft.textTransparent(RA8875_GREEN);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn5);
      rpm_value[rpm]=5;// time value
       Serial.print(rpm_value[i]);
      cursorxrpm=cursorxrpm+15;
      tft.graphicsMode();
      rpm++;
    }

    //6//
    if(tx>106 && tx<223 && ty>579 && ty<712)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxrpm, cursoryrpm); ///text location////
      tft.textTransparent(RA8875_GREEN);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn6);
      rpm_value[rpm]=6;// time value
       Serial.print(rpm_value[i]);
      cursorxrpm=cursorxrpm+15;
      tft.graphicsMode();
      rpm++;
    }

    //7//
    if(tx>360 && tx<472 && ty>386 && ty<578)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxrpm, cursoryrpm); ///text location////
      tft.textTransparent(RA8875_GREEN);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn7);
      rpm_value[rpm]=7;// time value
       Serial.print(rpm_value[i]);
      cursorxrpm=cursorxrpm+15;
      tft.graphicsMode();
     rpm++;
    }

    //8//
    if(tx>224 && tx<359 && ty>386 && ty<578)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxrpm, cursoryrpm); ///text location////
      tft.textTransparent(RA8875_GREEN);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn8);
      rpm_value[rpm]=8;// time value
       Serial.print(rpm_value[i]);
      cursorxrpm=cursorxrpm+15;
      tft.graphicsMode();
      rpm++;
    }

    //9//
    if(tx>106 && tx<223 && ty>386 && ty<578)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxrpm, cursoryrpm); ///text location////
      tft.textTransparent(RA8875_GREEN);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn9);
      rpm_value[rpm]=9;// time value
       Serial.print(rpm_value[i]);
      cursorxrpm=cursorxrpm+15;
      tft.graphicsMode();
      rpm++;
    }

     //<--//
    if(tx>360 && tx<472 && ty>300 && ty<385)
    {
      
      settingsbackflag=1;
  settingsclr=0;
     
    }

    //0//
    if(tx>224 && tx<359 && ty>300 && ty<385)
    {
   // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_BLACK);
    // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxrpm, cursoryrpm); ///text location////
      tft.textTransparent(RA8875_GREEN);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite("0");
      rpm_value[rpm]=0;// time value
      Serial.print(rpm_value[i]);
      cursorxrpm=cursorxrpm+15;
      tft.graphicsMode();
      rpm++;
    }
}
     //-->//
    if(tx>106 && tx<223 && ty>300 && ty<385)
   {
     settingsnextflag=1;
     settingsclr=0;
   //  }
     
     // rpm++;
    }
          

    /*  //<--//
    if(tx>373 && tx<460 && ty>334 && ty<347)
    {
   
      settingsbackflag=1;
    // }
     
    }    */

   
            
            /* if(rpm==0)
             {
              tft.fillRect(150, 75, 50, 30, RA8875_WHITE);
              
             }*/
     ///clear////
   if(tx>287 && tx<472 && ty>155 && ty<299)
     { 
       tft.fillRect(0, 0, 480, 15 ,0X0A0F);
      tft.textMode();
    // tft.touchEnable(true);
    // tft.fillRect(57, 120, 100, 58, RA8875_RED);
     tft.fillRect(150, 75, 55, 30, RA8875_WHITE);
     //tft.textMode();
    // tft.cursorBlink(32);
    // tft.textSetCursor(160, 70); ///text location////
    // tft.textEnlarge(0);  ////text size////
     cursorxrpm=160;
     rpm_value[0]=2;
     rpm_value[1]=5;
   total_rpm=25;
     tft.graphicsMode();
     rpm=0;
     settingsclr=1;
  }

  
     // }/
        

     //save//
   if(tx>106 && tx<286 && ty>155 && ty<299)
    {
     //  tft.fillRect(0, 0, 480, 15 ,RA8875_GREEN);
     //  Serial.print(rpm_value);
     Serial.print(rpm_value[0]);
     Serial.print(rpm_value[1]);
    cursorxrpm=160;
   // total_rpm=00;
    total_rpm=(rpm_value[0]*10)+rpm_value[1];
    save_no_of_drums=no_of_drums;
    save_drum_type=drum_type;
    save_calculation=calculation;
    stringset_rpm[0]=(total_rpm/10)+'0';
  stringset_rpm[1]=(total_rpm%10)+'0';
    i2=6;
    flag=1;  
 //tft.fillRect(0, 0, 480, 15 ,RA8875_GREEN);
/*tft.fillRect(0, 0, 480, 15 , 0X0A0F);
 tft.textMode();
     tft.textSetCursor(2, 1); ///text location////
      tft.textTransparent(RA8875_RED);
     tft.textEnlarge(0);  ////text size////
      tft.textWrite("Successfully Stored");    
    Serial.print("rpm=");
    Serial.print(total_rpm);
   /* Serial.print("\ndrum=");
    Serial.print(save_no_of_drums);
    Serial.print("\ndrumtype=");
    Serial.print(save_drum_type);
    Serial.print("\ncal=");
    Serial.print(save_calculation);
     Serial.print("\n");*/
   // Serial.print("cal=");
    if(total_rpm<20 || total_rpm>50)
    {
       
     cursorxrpm=160;
     rpm_value[0]=2;
     rpm_value[1]=5;
     total_rpm=25;
     tft.fillRect(0, 0, 480, 15 , 0X0A0F);
     tft.textMode();
     tft.textSetCursor(2, 1); ///text location////
      tft.textTransparent(RA8875_RED);
     tft.textEnlarge(0);  ////text size////
      tft.textWrite("please enter value in between 20 to 50");
      ac=85;
      
    }
    else
    {
      tft.fillRect(0, 0, 480, 15 ,RA8875_GREEN);
    }
    if(total_rpm>=20 && total_rpm<=25)
{
a=total_rpm-20;
a=a*5;
ac=110-a;
}
if(total_rpm==20)
{
  ac=111;
}
if(total_rpm==25)
{
  ac=87;
}
if(total_rpm==27)
{
  ac=85;
}

if(total_rpm==26)
{
  ac=83;
}

//if(total_rpm>=28 && total_rpm<=50)
//{
  if(total_rpm>=30 && total_rpm<=34)
  {
/*a=total_rpm-30;
a=a*2;
ac=80-a;*/
ac=66;
  }
  if(total_rpm>=35 && total_rpm<=39)
  {
/*a=total_rpm-30;
a=a*2;
ac=80-a;*/
ac=56;
  }
  if(total_rpm>=40 && total_rpm<=44)
  {
/*a=total_rpm-30;
a=a*2;
ac=80-a;*/
ac=50;
  }
  if(total_rpm>=47 && total_rpm<=49)
  {
/*a=total_rpm-30;
a=a*2;
ac=80-a;*/
ac=42;
  }
  if(total_rpm>=45 && total_rpm<=46)
  {
/*a=total_rpm-30;
a=a*2;
ac=80-a;*/
ac=45;
  }
  if(total_rpm==28)
  {
    a=total_rpm-28;
a=a*2;
ac=80-a;
}
if(total_rpm==50)
{
  ac=42;
 // ac=33;
}
//}
Serial.print(ac);
    rpm=0;
    tft.graphicsMode();
    } 
    if(rpm>2)
    {
       tft.fillRect(0, 0, 480, 15 ,0X0A0F);
       tft.fillRect(150, 75, 55, 30, RA8875_WHITE);
     //tft.textMode();
    // tft.cursorBlink(32);
    // tft.textSetCursor(160, 70); ///text location////
    // tft.textEnlarge(0);  ////text size////
     cursorxrpm=160;
   //  rpm_value[0]=2;
   //  rpm_value[1]=5;
    // tft.graphicsMode();
     rpm=0;
    }
}

void madmin_menu()
{
  tft.textMode();
  
  tft.fillScreen(0X379f);//0x0a0f
//upper //
tft.fillRect(0, 0, 480, 18 , 0Xffff);

//lower
tft.fillRect(0,250, 480, 25 , 0Xffff);

tft.fillRoundRect(9, 20, 45, 30,4 ,0x0a0f);//0XC618
  tft.textSetCursor(20,24 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("<--");
  //vertical line///
  //tft.drawFastVLine(280,00,272,RA8875_WHITE);
//  tft.drawFastVLine(340,02,257,RA8875_WHITE);
 // tft.drawFastVLine(400,00,200,RA8875_WHITE);

  //Horizontal line///
  tft.drawFastHLine(0,18,480,RA8875_BLACK);
  tft.drawFastHLine(0,250,480,RA8875_BLACK);
 // tft.drawFastHLine(280,200,180,RA8875_WHITE);
  
 // tft.textMode();
  ///admin///
  //tft.drawRect(140,100,180,25,RA8875_BLACK);
 // tft.fillCircle(95, 90, 33, 0XF799);//0632C
  tft.fillEllipse(95, 190,50 , 80,0XF790);
  tft.fillCircle(95, 90, 33, 0XF799);//0632C
  tft.fillRect(25,175,160,75,0X379f);
 // tft.drawRect(40,130,100,40,RA8875_WHITE);
  //tft.fillCurve(100, 100, 78, 38, 2, RA8875_BLACK);
  tft.textSetCursor(50, 135);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1);
  tft.textWrite("Admin");
  //tft.drawRect(140,100,180,25,RA8875_BLACK);

//lower
tft.fillRect(0,250, 480, 25 , 0Xffff);
tft.drawFastHLine(0,250,480,RA8875_BLACK);

////maintainance///
 // tft.drawRect(140,100,180,25,RA8875_BLACK);
 // tft.drawLine(300,150,350, 50, 0X0000);
 // tft.drawLine(330,150,380, 50, 0X0000);
  tft.fillCircle(340, 125, 50, 0X0A0F);
  tft.fillCircle(340, 125, 30, 0XFFFF);
  tft.textSetCursor(300, 120);
  tft.textTransparent(RA8875_RED);
  tft.textEnlarge(0);
  tft.textWrite("Maintenance");
  
tft.graphicsMode();
}

void madmin_menu_function()
{
 // while(madminflag==0)
 // {
 //   delay(150);
   //   if (! digitalRead(RA8875_INT))
  //{
   // tft.touchRead(&tx, &ty);
    ////////<--//////
    if(tx>864 && tx<977 && ty>757 && ty<892)
    {
      madminbackflag=1;
      adminflag=0;
      maintainanceflag=0;
      //Serial.print("flagnext");
    }
    if(tx>730 && tx<869 && ty>383 && ty<680)
    {
      adminflag=1;
      maintainanceflag=0;
      madminbackflag=1;
    }
    if(tx>263 && tx<416 && ty>411 && ty<665)
    {
      maintainanceflag=1;
      adminflag=0;
      madminbackflag=1;
    }
 // }
 // }//
}

void adminpass_menu()
{
 tft.textMode();

  tft.textSetCursor(150, 30);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0);
  tft.textWrite("Enter admin password");
///// FOR string2////// 
  tft.drawRect(140,50,220,25,RA8875_BLACK);
  
  tft.graphicsMode();
}

void adminpass_function()
{
int q=0;
 
          if(cursorxa>350)
          {
            tft.fillRect(140,50,220,25,RA8875_WHITE);
            tft.drawRect(140,50,220,25,RA8875_BLACK);
            cursorxa=150;
            i=0;
          }
          
   //1//
    if(tx>886 && tx<1000 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("1");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("1");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
  
      adminuser[i]='1';// user entered password value 
      adminpass[i]='1'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //2//
    if(tx>792 && tx<886 && ty>528 && ty<700)
    {   
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("2");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("2");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='2';// user entered password value
      adminpass[i]='2'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //3//
   if(tx>732 && tx<792 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("3");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("3");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='3';// user entered password value
      adminpass[i]='3'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //4//
    if(tx>641 && tx<732 && ty>528 && ty<700)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("4");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("4");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='4';// user entered password value 
      adminpass[i]='4'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //5//
    if(tx>554 && tx<641 && ty>528 && ty<700)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("5");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("5");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='5';// user entered password value
      adminpass[i]='5'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //6//
    if(tx>467 && tx<554 && ty>528 && ty<700)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("6");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("6");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='6';// user entered password value
      adminpass[i]='6'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //7//
     if(tx>395 && tx<467 && ty>528 && ty<700)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("7");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("7");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='7';// user entered password value
      adminpass[i]='7'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //8//
    if(tx>308 && tx<395 && ty>528 && ty<700)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("8");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("8");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='8';// user entered password value
      adminpass[i]='8'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //9//
    if(tx>238 && tx<308 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("9");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("9");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='9';// user entered password value
      adminpass[i]='9'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //0//
    if(tx>138 && tx<238 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_BLACK);
    // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("0");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("0");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='0';// user entered password value
      adminpass[i]='0'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
      Serial.print("ipf0");
    }

////abcd////

if(key==1)
{
//q//
    if(tx>873 && tx<977 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("q");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("q");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
  
      adminuser[i]='q';// user entered password value 
      adminpass[i]='q'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //w//
    if(tx>787 && tx<872 && ty>374 && ty<528)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("w");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("w");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='w';// user entered password value
      adminpass[i]='w'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //e//
    if(tx>705 && tx<786 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("e");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("e");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='e';// user entered password value
      adminpass[i]='e'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //r//
     if(tx>614 && tx<704 && ty>374 && ty<528)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("r");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("r");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='r';// user entered password value 
      adminpass[i]='r'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //t//
    if(tx>524 && tx<613 && ty>374 && ty<528)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("t");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("t");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='t';// user entered password value
      adminpass[i]='t'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //y//
     if(tx>437 && tx<523 && ty>374 && ty<528)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("y");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("y");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='y';// user entered password value
      adminpass[i]='y'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //u//
    if(tx>368 && tx<436 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("u");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("u");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='u';// user entered password value
      adminpass[i]='u'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //i//
    if(tx>263 && tx<368 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("i");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("i");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='i';// user entered password value
      adminpass[i]='i'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //o//
     if(tx>178 && tx<262 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("o");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("o");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='o';// user entered password value
      adminpass[i]='o'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//p//
    if(tx>113 && tx<177 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("p");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("p");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
  
      adminuser[i]='p';// user entered password value 
      adminpass[i]='p'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //a//
   if(tx>846 && tx<1000 && ty>233 && ty<374)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("a");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("a");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='a';// user entered password value
      adminpass[i]='a'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //s//
    if(tx>792 && tx<846 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("s");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("s");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='s';// user entered password value
      adminpass[i]='s'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //d//
    if(tx>732 && tx<792 && ty>233 && ty<374)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("d");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("d");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='d';// user entered password value 
      adminpass[i]='d'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //f//
    if(tx>641 && tx<732 && ty>233 && ty<374)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("f");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("f");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='f';// user entered password value
      adminpass[i]='f'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //g//
     if(tx>554 && tx<641 && ty>233 && ty<374)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("g");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("g");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='g';// user entered password value
      adminpass[i]='g'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //h//
     if(tx>467 && tx<554 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("h");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("h");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='h';// user entered password value
      adminpass[i]='h'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //j//
     if(tx>395 && tx<467 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("j");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("j");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='j';// user entered password value
      adminpass[i]='j'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //k//
    if(tx>308 && tx<395 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("k");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("k");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='k';// user entered password value
      adminpass[i]='k'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//l//
    if(tx>238 && tx<308 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("l");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("l");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
  
      adminuser[i]='l';// user entered password value 
      adminpass[i]='l'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //z//
     if(tx>138 && tx<238 && ty>233 && ty<374)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("z");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("z");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='z';// user entered password value
      adminpass[i]='z'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //x//
     if(tx>787 && tx<872 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("x");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("x");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='x';// user entered password value
      adminpass[i]='x'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //c//
     if(tx>704 && tx<787 && ty>89 && ty<233)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("c");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("c");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='c';// user entered password value 
      adminpass[i]='c'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //v//
    if(tx>614 && tx<704 && ty>89 && ty<233)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("v");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("v");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='v';// user entered password value
      adminpass[i]='v'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //b//
    if(tx>437 && tx<523 && ty>89 && ty<233)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("b");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("b");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='b';// user entered password value
      adminpass[i]='b'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

  /*  //space//
    if(tx>460 && tx<600 && ty>152 && ty<253)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      userenter[i]=' ';// user entered password value
      reenteredpass[i]=7; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }
*/
    //n//
    if(tx>368 && tx<436 && ty>89 && ty<233)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("n");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("n");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='n';// user entered password value
      adminpass[i]='n'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //m//
    if(tx>263 && tx<367 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("m");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("m");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='m';// user entered password value
      adminpass[i]='m'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
}
////ABCD////

 if(key==2)
          {
//Q//
    if(tx>873 && tx<977 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Q");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Q");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
  
      adminuser[i]='Q';// user entered password value 
      adminpass[i]='Q'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //W//
    if(tx>787 && tx<872 && ty>374 && ty<528)
    {  
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("W");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("W");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='W';// user entered password value
      adminpass[i]='W'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //E//
    if(tx>705 && tx<786 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("E");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("E");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='E';// user entered password value
      adminpass[i]='E'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //R//
     if(tx>614 && tx<704 && ty>374 && ty<528)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("R");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("R");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='R';// user entered password value 
      adminpass[i]='R'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //T//
    if(tx>524 && tx<613 && ty>374 && ty<528)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("T");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("T");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='T';// user entered password value
      adminpass[i]='T'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //Y//
    if(tx>437 && tx<523 && ty>374 && ty<528)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Y");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Y");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='Y';// user entered password value
      adminpass[i]='Y'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //U//
    if(tx>368 && tx<436 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("U");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("U");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='U';// user entered password value
      adminpass[i]='U'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //I//
     if(tx>263 && tx<367 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("I");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("I");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='I';// user entered password value
      adminpass[i]='I'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //O//
     if(tx>178 && tx<262 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("O");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("O");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='O';// user entered password value
      adminpass[i]='O'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//P//
    if(tx>113 && tx<177 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("P");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("P");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
  
      adminuser[i]='P';// user entered password value 
      adminpass[i]='P'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //A//
     if(tx>846 && tx<1000 && ty>233 && ty<374)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("A");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("A");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='A';// user entered password value
      adminpass[i]='A'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //S//
    if(tx>792 && tx<846 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("S");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("S");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='S';// user entered password value
      adminpass[i]='S'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //D//
    if(tx>732 && tx<792 && ty>233 && ty<374)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("D");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("D");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='D';// user entered password value 
      adminpass[i]='D'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //F//
    if(tx>641 && tx<732 && ty>233 && ty<374)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("F");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("F");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='F';// user entered password value
      adminpass[i]='F'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //G//
   if(tx>554 && tx<641 && ty>233 && ty<374)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("G");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("G");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='G';// user entered password value
      adminpass[i]='G'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //H//
     if(tx>467 && tx<554 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("H");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("H");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='H';// user entered password value
      adminpass[i]='H'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //J//
    if(tx>395 && tx<467 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("j");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("J");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='J';// user entered password value
      adminpass[i]='J'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //K//
    if(tx>308 && tx<395 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("K");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("K");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='K';// user entered password value
      adminpass[i]='K'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//L//
    if(tx>238 && tx<308 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("L");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("L");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
  
      adminuser[i]='L';// user entered password value 
      adminpass[i]='L'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //Z//
     if(tx>138 && tx<238 && ty>233 && ty<374)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Z");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Z");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='Z';// user entered password value
      adminpass[i]='Z'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //X//
    if(tx>787 && tx<872 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("X");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("X");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='X';// user entered password value
      adminpass[i]='X'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //C//
    if(tx>704 && tx<787 && ty>89 && ty<233)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("C");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("C");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='C';// user entered password value 
      adminpass[i]='C'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //V//
    if(tx>614 && tx<704 && ty>89 && ty<233)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("V");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("V");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='V';// user entered password value
      adminpass[i]='V'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //B//
   if(tx>437 && tx<523 && ty>89 && ty<233)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("B");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("B");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='B';// user entered password value
      adminpass[i]='B'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

  /*  //space//
    if(tx>460 && tx<600 && ty>152 && ty<253)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      userenter[i]=' ';// user entered password value
      reenteredpass[i]=7; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }
*/
    //N//
     if(tx>368 && tx<436 && ty>89 && ty<233)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("N");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("N");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='N';// user entered password value
      adminpass[i]='N'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //M//
     if(tx>263 && tx<367 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("M");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("M");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      adminuser[i]='M';// user entered password value
      adminpass[i]='M'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
          }
    

    ////<--////
     if(tx>881 && tx<953 && ty>780 && ty<864)
    {
     //loginback=1;
     adminback_flag=1;
     key=1;
     //for(q=0;q<=20;q++)
    // {
     adminuser[0]='\0';// user entered password value
    // }
    // for(q=0;q<=8;q++)
    // {
    adminpass[0]='\0';
    // }
    i=0;
    cursorxa=150;
     Serial.print("adminback_flag=1 ");
    }
/*
    ////-->////
    if(tx>143 && tx<179 && ty>763 && ty<818)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
     // tft.textEnlarge(0);  ////text size////
    //  tft.textWrite("9");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
     // tft.textEnlarge(0);  ////text size////
     // tft.textWrite("9");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
    //  tft.textEnlarge(0);  ////text size////
     // tft.textWrite("*");
     // userenter[i]='9';// user entered password value
      //reenteredpass[i]=9; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     // i++;
    }

  // }
 /*  if(loginenter==1)
   {
    break;
   }
  /* if(adminback_flag==1)
   {
    break;
   }*/
    // }
     //capslock//
    if(tx>178 && tx<262 && ty>89 && ty<233)
    {
      capslock=1;
    }
 /*   ////<--////
    if(tx>885 && tx<953 && ty>746 && ty<795)
    {
     loginback=1;
    }*/
     ///clear////
    if(tx>873 && tx<1000 && ty>89 && ty<233)
     {
      
    // tft.touchEnable(true);
     //tft.fillRect(57, 120, 150, 58, RA8875_RED);
     tft.textMode();
   //  tft.cursorBlink(32);
     cursorxa=cursorxa-10;
     tft.textSetCursor(cursorxa, cursorya); ///text location////
     tft.textTransparent(RA8875_WHITE);
     tft.textEnlarge(0);  ////text size////
     tft.textWrite("*");
    // tft.textSetCursor(cursorxa, cursorya); ///text location////
     // i=i-1;
     i=i-1;
      adminuser[i]='\0';// user entered password value
   //  userenter[1]=0;// user entered password value
    // userenter[2]=0;// user entered password value
    // userenter[3]=0;// user entered password value
      
     adminpass[i]='\0'; //to change password
     //reenteredpass[i]=0; //to change password
     //reenteredpass[i]=0; //to change password
     //reenteredpass[i]=0; //to change password
    // i=i-1;
     if(cursorxa<150)
     {
      cursorxa=150;
      i=0;
       adminuser[i]='\0';
        adminpass[i]='\0';
     }
     tft.graphicsMode();
     Serial.print("user=");
     Serial.print(loginuser[i]);
     Serial.print("\ni=");
     Serial.print(i);
  }
  
  
    //enter//
    if(tx>113 && tx<177 && ty>89 && ty<233)
    {
      tft.fillRect(140,50,220,25,RA8875_WHITE);
            tft.drawRect(140,50,220,25,RA8875_BLACK);
            adminuser[i]='\0';
            adminpass[i]='\0';
    cursorxa=150;
    i=0;
    i2=12;
   // loginenter=1;  
     adminenter_flag=1;
    Serial.print("logne");
    }
          
         while(tft.touched())
    {
      tft.touchRead(&txc, &tyc);
      txc=tyc=0;
      Serial.print("2");
      delay(100);
    }
  
} 

void adminpage_menu()
{
   tft.textMode();
////admin==settings and configurations///

tft.drawRoundRect(130, 50, 220, 165,10, 0X0000);
  //tft.fillRoundRect(30, 30, 205, 223,10, 0X379f);
  
  tft.fillRoundRect(130, 50, 220, 40,10,0X318C);
  tft.textSetCursor(140, 55);
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0);
  tft.textWrite("Select an Option");

tft.fillRoundRect(130, 120, 220, 30,10 ,0x3f79);//0XC618
tft.textSetCursor(140,125 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Settings");

tft.fillRoundRect(130, 180, 220, 30,10 ,0x3f79);//0XC618
tft.textSetCursor(140,185 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Configurations");

 /* tft.fillRoundRect(420, 140, 45, 30,4 ,0x0a0f);//0XC618
  tft.textSetCursor(431,144 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("<--");
  */

  tft.fillRoundRect(9, 20, 45, 30,4 ,0x0a0f);//0XC618
  tft.textSetCursor(20,24 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("<--");

  tft.graphicsMode();
}

void adminpage_menu_function()
{
  //settings///
if(tx>354 && tx<734 && ty>459 && ty<529)
{

  adminsettings_flag=1;
}

//configuration///
if(tx>343 && tx<731 && ty>242 && ty<348)
{
  adminconfig_flag=1;
}
/////<--////////////
 if(tx>864 && tx<977 && ty>757 && ty<892)
    {
      adminpage_flag=1;
    }
}

void admin_settings_menu()
{
  tft.fillScreen(0xffff);
   tft.textMode();
  //tft.cursorBlink(32);
  
 // tft.drawRoundRect(7, 22, 240, 170, 20, 0xffff);//0x3041,0xc986
  /* Set the cursor location (in pixels) */
//tft.fillRect(464,12,30,251,RA8875_WHITE);

//tft.fillRect(252,12,5,251,RA8875_WHITE);
  tft.drawRoundRect(29, 29, 207, 224,10, 0X0000);
  tft.fillRoundRect(30, 30, 205, 223,10, 0X379f);

  tft.fillRoundRect(30, 30, 205, 30,10,0X318C);
  tft.textSetCursor(50, 35);
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(0);
  tft.textWrite("Selects Parameters");


  tft.textSetCursor(40, 80);
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0);
  tft.textWrite("Weight Unit:");
  ////entering reading value/////
  tft.fillRect(150, 75, 30, 30, RA8875_WHITE);
  if(weight_unit==1)
   {
   tft.fillRect(150, 75, 30, 30, RA8875_GREEN);
 /* tft.textSetCursor(187,77 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("gm");*/
   }
  tft.textSetCursor(152,77 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("gm");
  
  tft.fillRect(185, 75, 30, 30, RA8875_WHITE);
  if(weight_unit==2)
   {
   tft.fillRect(185, 75, 30, 30, RA8875_GREEN);
 /* tft.textSetCursor(152,77 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("mg");*/
   }
  tft.textSetCursor(187,77 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("mg");
  
 
  tft.textSetCursor(40,120 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Balance: ");
  ////entering reading value/////
  tft.fillRect(150, 115, 30, 30, RA8875_WHITE);
   if(balance==1)
  {
  tft.fillRect(150, 115, 30, 30, RA8875_GREEN);
  /*tft.textSetCursor(190,115 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("yes");*/
  }
  
  tft.textSetCursor(153,115 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Yes");
  
  tft.fillRect(185, 115, 30, 30, RA8875_WHITE);
  if(balance==2)
  {
  tft.fillRect(185, 115, 30, 30, RA8875_GREEN);
 /* tft.textSetCursor(190,115 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("No");*/
  }
  tft.textSetCursor(190,115 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("No");

  tft.textSetCursor(40,175 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Printer: ");
  ////entering reading value/////
  tft.fillRect(150, 155, 80, 25, RA8875_WHITE);
  if(printer==1)
  {
  tft.fillRect(150, 155, 80, 25, RA8875_GREEN);
  /*tft.textSetCursor(151,185 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Inbuilt");*/
  }
  tft.textSetCursor(150,155 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Inbuilt");
  
  tft.fillRect(150, 185, 80, 25, RA8875_WHITE);
   if(printer==2)
  {
  tft.fillRect(150, 185, 80, 25, RA8875_GREEN);
 /* tft.textSetCursor(151,185 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("External");*/
  }
  tft.textSetCursor(151,185 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("External");


 //SIDE BAR////

  tft.fillRoundRect(420, 140, 45, 30,4 ,0x0a0f);//0XC618
  tft.textSetCursor(431,144 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("<--");

//tft.fillRoundRect(416,11,54,130,6,RA8875_WHITE);

  tft.fillRoundRect(420, 20, 45, 30,4 ,0x0a0f);//0XC618
  tft.textSetCursor(431,24 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("-->");

  tft.fillRoundRect(420, 60, 45, 30,3 ,0x0a0f);//0XC618
  tft.textSetCursor(424,68 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("CLR");

  tft.fillRoundRect(420, 100, 45, 30,3 ,0x0a0f);//0XC618
  tft.textSetCursor(424,108 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("save");

  tft.graphicsMode();
}

void admin_settings_menu_function()
{
  
 ///weight unit///
  if(tx>659 && tx<697 && ty>590 && ty<659)
  {
   tft.textMode();
    
  tft.fillRect(150, 75, 30, 30, RA8875_GREEN);
  tft.textSetCursor(152,77 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("gm");

   if(weight_unit==2)
   {
   tft.fillRect(185, 75, 30, 30, RA8875_WHITE);
  tft.textSetCursor(187,77 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("mg");
   }
   weight_unit=1;
    tft.graphicsMode();
  }
  if(tx>590 && tx<633 && ty>591 && ty<659)
  {
    tft.textMode();
    
   tft.fillRect(185, 75, 30, 30, RA8875_GREEN);
  tft.textSetCursor(187,77 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("mg");

   if(weight_unit==1)
   {
   tft.fillRect(150, 75, 30, 30, RA8875_WHITE);
  tft.textSetCursor(152,77 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("gm");
   }
   weight_unit=2;
    tft.graphicsMode();
  }

  //balance///
  if(tx>654 && tx<700 && ty>476 && ty<628)
  {
    tft.textMode();
            
     tft.fillRect(150, 115, 30, 30, RA8875_GREEN);
  tft.textSetCursor(153,115 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Yes");

  if(balance==2)
  {
  tft.fillRect(185, 115, 30, 30, RA8875_WHITE);
  tft.textSetCursor(190,115 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("No");
  }
  balance=1;
  tft.graphicsMode();
  
  }
  
  if(tx>586 && tx<633 && ty>476 && ty<544)
  { 
    tft.textMode();  
  
    tft.fillRect(185, 115, 30, 30, RA8875_GREEN);
  tft.textSetCursor(190,115 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("No");
  if(balance==1)
  {
     tft.fillRect(150, 115, 30, 30, RA8875_WHITE);
  tft.textSetCursor(153,115 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Yes");
  }
  balance=2;
   tft.graphicsMode();
  }

  ///printer///
  if(tx>576 && tx<709 && ty>359 && ty<409)
  {
tft.textMode();
    
    tft.fillRect(150, 155, 80, 25, RA8875_GREEN);
  tft.textSetCursor(150,155 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Inbuilt");
  if(printer==2)
  {
  tft.fillRect(150, 185, 80, 25, RA8875_WHITE);
  tft.textSetCursor(151,185 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("External");
  }
  printer=1;
  tft.graphicsMode();
  }
  if(tx>566 && tx<698 && ty>289 && ty<335)
  {
tft.textMode();
    
tft.fillRect(150, 185, 80, 25, RA8875_GREEN);
  tft.textSetCursor(151,185 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("External");
  if(printer==1)
  {
     tft.fillRect(150, 155, 80, 25, RA8875_WHITE);
  tft.textSetCursor(150,155 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Inbuilt");
  }
  printer=2;
  tft.graphicsMode();
  }

  ////<--(this one is in different position)///
  if(tx>113 && tx<211 && ty>403 && ty<496)
  {
    insettingsback_flag=1;
  }

   ////-->////
    if(tx>113 && tx<211 && ty>756 && ty<872)
    {
   insettingsnext_flag=1;
    }
    
     ///clear////
   if(tx>113 && tx<211 && ty>631 && ty<755)
     { 
       tft.textMode();
     tft.textSetCursor(2, 2); ///text location////
      tft.textTransparent(RA8875_WHITE);
     tft.textEnlarge(0);  ////text size////
     // tft.textWrite("Successfully Stored");
   
      save_weight_unit=1;
      save_balance=1;
      save_printer=1;
      tft.textMode();
      tft.fillRect(150, 75, 30, 30, RA8875_WHITE);
  tft.textSetCursor(152,77 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("gm");
  
  tft.fillRect(185, 75, 30, 30, RA8875_WHITE);
  tft.textSetCursor(187,77 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("mg");
  
 
  tft.textSetCursor(40,120 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("balance: ");
  ////entering reading value/////
  tft.fillRect(150, 115, 30, 30, RA8875_WHITE);
  tft.textSetCursor(153,115 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Yes");
  
  tft.fillRect(185, 115, 30, 30, RA8875_WHITE);
  tft.textSetCursor(190,115 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("No");

  tft.textSetCursor(40,175 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Printer: ");
  ////entering reading value/////
  tft.fillRect(150, 155, 80, 25, RA8875_WHITE);
  tft.textSetCursor(150,155 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Inbuilt");
  
  tft.fillRect(150, 185, 80, 25, RA8875_WHITE);
  tft.textSetCursor(151,185 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("External");

tft.graphicsMode();
     }

  //save//
    if(tx>113 && tx<211 && ty>496 && ty<630)
    {
      tft.textMode();
     tft.textSetCursor(2, 2); ///text location////
      tft.textTransparent(RA8875_RED);
     tft.textEnlarge(0);  ////text size////
      tft.textWrite("Successfully Stored");
    tft.graphicsMode();
     save_weight_unit=weight_unit;
     save_balance=balance;
     save_printer=printer;
     tft.graphicsMode();
    }
}

void adminsettingsmenu2()
{
  tft.textMode();
   /////admin screen==date,time..../////
//tft.fillScreen(0x3f79);  
tft.fillRoundRect(130, 50, 220, 30,10 ,0x3f79);//0XC618
tft.textSetCursor(140,55 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Set Date and Time");

/*tft.fillRoundRect(130, 100, 220, 30,10 ,0x3f79);//0XC618
tft.textSetCursor(140,105 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Instrument Id");*/

tft.fillRoundRect(130, 150, 220, 30,10 ,0x3f79);//0XC618
tft.textSetCursor(140,155 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Company Name");

tft.fillRoundRect(9, 20, 45, 30,4 ,0x0a0f);//0XC618
  tft.textSetCursor(20,24 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("<--");

   /*tft.fillRoundRect(420, 20, 45, 30,4 ,0x0a0f);//0XC618
  tft.textSetCursor(431,24 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("-->");*/

  tft.graphicsMode();
}

void adminsettingsmenu2_function()
{
  ///set date & time/// 
   if(tx>341 && tx<727 && ty>676 && ty<725)
  {
    asettingssetdandt_flag=1;
  }
 /* ///instrument id///
  if(tx>339 && tx<727 && ty>524 && ty<580)
  {
    asettingsinstrumentid_flag=1;
  }*/
  if(tx>353 && tx<730 && ty>370 && ty<435)
  {
    asettingscompanyname_flag=1;
  }
  ////<--////
    if(tx>864 && tx<977 && ty>757 && ty<892)
    {
      asettingsmenu2next_flag=1;
    }
 /* ///next///
  if(tx>127 && tx<191 && ty>753 && ty<830)
  {
    asettingsmenu2next_flag=1;
  }*/
}

void rtc_settings_parameters1_menu()
{
   tft.textMode();
  /////rtc time and date////

tft.fillRoundRect(20, 60, 250, 70,10, 0X379f);
tft.textSetCursor(30, 80);
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0);
  tft.textWrite("Enter Date:");
  
  ////entering reading value/////
  tft.fillRect(130, 75, 138, 30, RA8875_WHITE);
  tft.textSetCursor(153,70 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
  
 // tft.fillRect(130, 75, 130, 30, RA8875_WHITE);
  tft.textSetCursor(195,70 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");

  // tft.fillRect(130, 75, 130, 30, RA8875_WHITE);
  tft.textSetCursor(132,105 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("dd");
  
 // tft.fillRect(165, 75, 30, 30, RA8875_WHITE);
  tft.textSetCursor(167,105 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("mm");

  //tft.fillRect(200, 75, 60, 30, RA8875_WHITE);
  tft.textSetCursor(202,105 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("yyyy");

  tft.graphicsMode();
}

void rtc_settings_parameters2_menu()
{
  tft.textMode();
  tft.fillRoundRect(20, 60, 250, 70,10, 0X379f);
  tft.textSetCursor(30, 80);
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0);
  tft.textWrite("Enter Time:");

 tft.fillRect(130, 75, 130, 30, RA8875_WHITE);
  tft.textSetCursor(200,70); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1); ////text size////
  tft.textWrite(":");

  //tft.fillRect(130, 170, 130, 30, RA8875_WHITE);
  tft.textSetCursor(151,70 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1); ////text size////
  tft.textWrite(":");
  
  
  ////entering reading value/////
 // tft.fillRect(130, 170, 130, 30, RA8875_WHITE);
  tft.textSetCursor(132,105 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("hh");
  
 // tft.fillRect(165, 170, 200, 30, RA8875_WHITE);
  tft.textSetCursor(167,105 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("min");
  
  //tft.fillRect(200, 170, 30, 30, RA8875_WHITE);
  tft.textSetCursor(215,105 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0); ////text size////
  tft.textWrite("ss");

  tft.graphicsMode();
}

void rtc_settings_parameters1_menu_function()
{
  //if(tx>506 && tx<735 && ty>595 && ty<654)
 // {
    //while(rtctimejump_flag==0)
   // {
     // delay(200);
     // if (tft.touched())
   // {
 //     Serial.print("Touch: ");
    //  tft.touchRead(&tx, &ty);
   //1//
    if(tx>360 && tx<472 && ty>713 && ty<870)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
      //tft.cursorBlink(32);
      tft.textSetCursor(cursorxr, cursoryr); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn1);
      day_value[i]=1;// rtc value 
      month_value[i]=1;// rtc value 
      year_value[i]=1;// rtc value 
      cursorxr=cursorxr+12;
      tft.graphicsMode();
      i++;
      Serial.print("yes");
    }
   
//Serial.print("b");
    //2//
    if(tx>224 && tx<359 && ty>713 && ty<870)
    { 
      tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorxr, cursoryr); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn2);
       day_value[i]=2;// rtc value 
      month_value[i]=2;// rtc value 
      year_value[i]=2;// rtc value 
      cursorxr=cursorxr+12;
      tft.graphicsMode();
      i++;
    }

      
    //3//
    if(tx>106 && tx<223 && ty>713 && ty<870)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorxr, cursoryr); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn3);
      day_value[i]=3;// rtc value 
      month_value[i]=3;// rtc value 
      year_value[i]=3;// rtc value 
      cursorxr=cursorxr+12;
      tft.graphicsMode();
      i++;
    }

    //4//
    if(tx>360 && tx<472 && ty>579 && ty<712)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorxr, cursoryr); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn4);
       day_value[i]=4;// rtc value 
      month_value[i]=4;// rtc value 
      year_value[i]=4;// rtc value 
      cursorxr=cursorxr+12;
      tft.graphicsMode();
      i++;
    }

    //5//
    if(tx>224 && tx<359 && ty>579 && ty<712)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxr, cursoryr); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn5);
       day_value[i]=5;// rtc value 
      month_value[i]=5;// rtc value 
      year_value[i]=5;// rtc value 
      cursorxr=cursorxr+12;
      tft.graphicsMode();
      i++;
    }

    //6//
    if(tx>106 && tx<223 && ty>579 && ty<712)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxr, cursoryr); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn6);
      day_value[i]=6;// rtc value 
      month_value[i]=6;// rtc value 
      year_value[i]=6;// rtc value 
      cursorxr=cursorxr+12;
      tft.graphicsMode();
      i++;
    }

    //7//
    if(tx>360 && tx<472 && ty>386 && ty<578)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxr, cursoryr); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn7);
       day_value[i]=7;// rtc value 
      month_value[i]=7;// rtc value 
      year_value[i]=7;// rtc value 
      cursorxr=cursorxr+12;
      tft.graphicsMode();
     i++;
    }

    //8//
    if(tx>224 && tx<359 && ty>386 && ty<578)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxr, cursoryr); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn8);
       day_value[i]=8;// rtc value 
      month_value[i]=8;// rtc value 
      year_value[i]=8;// rtc value 
      cursorxr=cursorxr+12;
      tft.graphicsMode();
      i++;
    }

    //9//
    if(tx>106 && tx<223 && ty>386 && ty<578)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxr, cursoryr); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn9);
       day_value[i]=9;// rtc value 
      month_value[i]=9;// rtc value 
      year_value[i]=9;// rtc value 
      cursorxr=cursorxr+12;
      tft.graphicsMode();
      i++;
    }

     //<--//
    if(tx>360 && tx<472 && ty>300 && ty<385)
    {
  
      rtcdateback_flag=1;
     cursorxr=130;
     i=0;
    rc=2;
    }

    //0//
    if(tx>224 && tx<359 && ty>300 && ty<385)
    {
   // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_BLACK);
    // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxr, cursoryr); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite("0");
       day_value[i]=0;// rtc value 
      month_value[i]=0;// rtc value 
      year_value[i]=0;// rtc value 
      cursorxr=cursorxr+12;
      tft.graphicsMode();
      i++;
    }

    //-->//
    if(tx>106 && tx<223 && ty>300 && ty<385)
    {
  
     rtcdatenext_flag=1;
   cursorxr=130;
     i=0;
     rc=2;
    }
    
     //save//
   if(tx>106 && tx<286 && ty>155 && ty<299)
    {  
      tft.textMode();
   // cursorx=70;

      ad=(year_value[0]*10)+year_value[1];
      am=(year_value[2]*10)+year_value[3];// rtc value 
      ay=(year_value[4]*1000)+(year_value[5]*100)+(year_value[6]*10)+year_value[7];// rtc value 
      i=0;
      rc=2;
    cursorxr=130;
    rtc.adjust(DateTime(ay, am, ad, ah, ami , as));
   /* Serial.print(ad);
    Serial.print(am);
    Serial.print(ay);*/
    ///save_no_of_drumps=no_of_drumps;
   // save_drum_type=drum_type;
   // save_calculation=calculation;
   // i2=6;
    //flag=1;  
    //Serial.print("ne");
   /* if(total_time<1 && total_time>999)
    {
       
     cursorx=70;
     time_value[0]=2;
     time_value[1]=5;
     total_time=4;
     
     tft.textMode();
     tft.textSetCursor(1, 1); ///text location////
      tft.textTransparent(RA8875_RED);
     tft.textEnlarge(0);  ////text size////
      tft.textWrite("please enter value in between 20 to 50");
      
    }*/
    tft.fillRect(0, 0, 480, 12 ,RA8875_GREEN);
    tft.graphicsMode();
    }

 ///clrea///
    if(tx>287 && tx<472 && ty>155 && ty<299)
    {
       tft.fillRect(0, 0, 480, 12 , 0X0A0F);
       tft.textMode();
   /*  tft.textSetCursor(1, 1); ///text location////
      tft.textTransparent(RA8875_RED);
     tft.textEnlarge(0);  ////text size////
      tft.textWrite("");*/
      
      //  tft.fillRect(40, 120, 180, 60, 0x3f79);
      
    //i=i-1;
    tft.fillRect(130, 75, 130, 30, RA8875_WHITE);
  tft.textSetCursor(153,70 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
  
 // tft.fillRect(130, 75, 130, 30, RA8875_WHITE);
  tft.textSetCursor(195,70 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
       day_value[0]=0;// rtc value 
      month_value[0]=0;// rtc value 
      for(i=0;i<=9;i++)
      {
      year_value[i]=0;// rtc value 
      }
       day_value[1]=0;// rtc value 
      month_value[1]=0;// rtc value 
      year_value[1]=0;// rtc value 
      i=0;
      rc=2;
      cursorxr=130;
      tft.graphicsMode();
    }
    
    if(i==2)
    {
      if(rc==2)
      {
      cursorxr=cursorxr+13;
      rc=rc+1;
      }
    }
     if(i==4)
    {
      
      if(rc==3)
      {
      cursorxr=cursorxr+13;
      rc=rc+1;
      }
    }
  /*  if(cursorx>115)
    {
      tft.fillRect(40, 120, 180, 60, 0x3f79);
      i=0;
      cursorx=70;
    }*/
  /*   if(tx>568 && tx<736 && ty>326 && ty<381)
  {
    i=0;
    cursorxr=130;
    cursoryr=165;
  rtctimejump_flag=1;
  }*/

  if(i>8)
  {
     tft.fillRect(0, 0, 480, 12 , 0X0A0F);
    tft.textMode();
    tft.fillRect(130, 75, 140, 30, RA8875_WHITE);
  tft.textSetCursor(153,70 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
  
 // tft.fillRect(130, 75, 130, 30, RA8875_WHITE);
  tft.textSetCursor(195,70 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1); ////text size////
  tft.textWrite("-");
    i=0;
    rc=2;
    cursorxr=130;
  //  cursoryr=165;
    tft.graphicsMode();
  }
  //  }
  //  }
  //  rtctimejump_flag=0;
 // }
}
void rtc_settings_parameters2_menu_function()
{
  
 // if(tx>568 && tx<736 && ty>326 && ty<381)
 // {//rtcdatejump_flag=0;
   // while(rtcdatejump_flag==0)
   // {
     // delay(200);
    //  if (tft.touched())
    //{
 //     Serial.print("Touch: ");
    //  tft.touchRead(&tx, &ty);
if(i>5)
{
   tft.fillRect(0, 0, 480, 12 , 0X0A0F);
  tft.textMode();
  tft.fillRect(130, 75, 130, 30, RA8875_WHITE);
  tft.textSetCursor(200,70); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1); ////text size////
  tft.textWrite(":");

  //tft.fillRect(130, 170, 130, 30, RA8875_WHITE);
  tft.textSetCursor(153,70 ); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1); ////text size////
  tft.textWrite(":");
  i=0;
  rc=2;
  tft.graphicsMode();
    cursorxr=130;
   // cursory=70;
}
      
   /*   if(tx>506 && tx<735 && ty>595 && ty<654)
  {
    i=0;
    cursorxr=130;
   // cursory=70;
    rtcdatejump_flag=1;
  }
    */
     
   //1//
    if(tx>360 && tx<472 && ty>713 && ty<870)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
      //tft.cursorBlink(32);
      tft.textSetCursor(cursorxr, cursoryr); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn1);
      hour_value[i]=1;// time value
      min_value[i]=1;// rtc value 
      sec_value[i]=1;//  rpm value 
      cursorxr=cursorxr+13;
      tft.graphicsMode();
      i++;
      Serial.print("yes");
    }
   
//Serial.print("b");
    //2//
    if(tx>224 && tx<359 && ty>713 && ty<870)
    { 
      tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorxr, cursoryr); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn2);
      hour_value[i]=2;// time value
      min_value[i]=2;// rtc value 
      sec_value[i]=2;// rtc value 
      
      cursorxr=cursorxr+13;
      tft.graphicsMode();
      i++;
    }

      
    //3//
    if(tx>106 && tx<223 && ty>713 && ty<870)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorxr, cursoryr); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn3);
      hour_value[i]=3;// time value
      min_value[i]=3;// rtc value 
      sec_value[i]=3;// rtc value
      cursorxr=cursorxr+13;
      tft.graphicsMode();
      i++;
    }

    //4//
    if(tx>360 && tx<472 && ty>579 && ty<712)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorxr, cursoryr); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn4);
      hour_value[i]=4;// time value
      min_value[i]=4;// rtc value 
      sec_value[i]=4;// rtc value
      cursorxr=cursorxr+13;
      tft.graphicsMode();
      i++;
    }

    //5//
    if(tx>224 && tx<359 && ty>579 && ty<712)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxr, cursoryr); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn5);
     hour_value[i]=5;// time value
      min_value[i]=5;// rtc value 
      sec_value[i]=5;// rtc value
      cursorxr=cursorxr+13;
      tft.graphicsMode();
      i++;
    }

    //6//
    if(tx>106 && tx<223 && ty>579 && ty<712)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxr, cursoryr); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn6);
      hour_value[i]=6;// time value
      min_value[i]=6;// rtc value 
      sec_value[i]=6;// rtc value
      cursorxr=cursorxr+13;
      tft.graphicsMode();
      i++;
    }

    //7//
    if(tx>360 && tx<472 && ty>386 && ty<578)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxr, cursoryr); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn7);
      hour_value[i]=7;// time value
      min_value[i]=7;// rtc value 
      sec_value[i]=7;// rtc value
      cursorxr=cursorxr+13;
      tft.graphicsMode();
     i++;
    }

    //8//
    if(tx>224 && tx<359 && ty>386 && ty<578)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxr, cursoryr); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn8);
      hour_value[i]=8;// time value
      min_value[i]=8;// rtc value 
      sec_value[i]=8;// rtc value
      cursorxr=cursorxr+13;
      tft.graphicsMode();
      i++;
    }

    //9//
    if(tx>106 && tx<223 && ty>386 && ty<578)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxr, cursoryr); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite(stringn9);
      hour_value[i]=9;// time value
      min_value[i]=9;// rtc value 
      sec_value[i]=9;// rtc value
      cursorxr=cursorxr+13;
      tft.graphicsMode();
      i++;
    }
    //0//
    if(tx>224 && tx<359 && ty>300 && ty<385)
    {
   // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_BLACK);
    // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxr, cursoryr); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite("0");
      hour_value[i]=0;// time value
      min_value[i]=0;// rtc value 
      sec_value[i]=0;// rtc value
      cursorxr=cursorxr+13;
      tft.graphicsMode();
      i++;
    }
   /* if(i==2 || i==4)
    {
      cursorxr=cursorxr+13;
    }*/
 if(i==2)
    {
      if(rc==2)
      {
      cursorxr=cursorxr+13;
      rc=rc+1;
      }
    }
     if(i==4)
    {
      
      if(rc==3)
      {
      cursorxr=cursorxr+13;
      rc=rc+1;
      }
    }
     //<--//
    if(tx>360 && tx<472 && ty>300 && ty<385)
    {
  
     rtctimeback_flag=1;
     cursorxr=130;
     i=0;
    rc=2;
    }

  /*  //0//
    if(tx>251 && tx<325 && ty>335 && ty<400)
    {
   // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_BLACK);
    // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxr, cursoryr); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(1);  ////text size////
      tft.textWrite("0");
      hour_value[i]=0;// time value
      min_value[i]=0;// rtc value 
      sec_value[i]=0;// rtc value
      cursorxr=cursorxr+13;
      tft.graphicsMode();
      i++;
    }
*/
    /*//-->//
    if(tx>122 && tx<196 && ty>334 && ty<400)
    {
  
     rtctimenext_flag=1;
   cursorxr=130;
     i=0;
     rc=2;
    }
    */
     //save//
   if(tx>106 && tx<286 && ty>155 && ty<299)
    {  
      
      tft.textMode();
   // cursorx=70;
    ah=(hour_value[0]*10)+hour_value[1];
    ami=(hour_value[2]*10)+hour_value[3];
    as=(hour_value[4]*10)+hour_value[5];

    rtc.adjust(DateTime(ay, am, ad, ah, ami , as));
    i=0;
    rc=2;
    cursorxr=130;
    /* Serial.print("ah=");
    Serial.print(ah);
     Serial.print("\nami=");
    Serial.print(ami);
     Serial.print("\nas=");
    Serial.print(as);*/
    ///save_no_of_drumps=no_of_drumps;
   // save_drum_type=drum_type;
   // save_calculation=calculation;
   // i2=6;
    //flag=1;  
    //Serial.print("ne");
   /* if(total_time<1 && total_time>999)
    {
       
     cursorx=70;
     time_value[0]=2;
     time_value[1]=5;
     total_time=4;
     
     tft.textMode();
     tft.textSetCursor(1, 1); ///text location////
      tft.textTransparent(RA8875_RED);
     tft.textEnlarge(0);  ////text size////
      tft.textWrite("please enter value in between 20 to 50");
      
    }*/
    tft.fillRect(0, 0, 480, 15 ,RA8875_GREEN);
    tft.graphicsMode();
    }

 ///clrea///
    if(tx>287 && tx<472 && ty>155 && ty<299)
    {
       tft.fillRect(0, 0, 480, 12 , 0X0A0F);
       tft.textMode();
  /*   tft.textSetCursor(1, 1); ///text location////
      tft.textTransparent(RA8875_RED);
     tft.textEnlarge(0);  ////text size////
      tft.textWrite("");*/
      
       // tft.fillRect(40, 120, 180, 60, 0x3f79);
      tft.fillRect(130, 75, 130, 30, RA8875_WHITE);
  tft.textSetCursor(200,70); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1); ////text size////
  tft.textWrite(":");
  

  //tft.fillRect(130, 170, 130, 30, RA8875_WHITE);
  tft.textSetCursor(151,70); ///text location////
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(1); ////text size////
  tft.textWrite(":");
    //i=i-1;
    for(i=0;i<=7;i++)
    {
      hour_value[i]=0;// time value
    }
      min_value[0]=0;// rtc value 
      sec_value[0]=0;// rtc value
      
      hour_value[1]=0;// time value
      min_value[1]=0;// rtc value 
      sec_value[1]=0;// rtc value
      i=0;
      rc=2;
      cursorxr=130;
      tft.graphicsMode();
      
    }
   /* if(cursorx>115)
    {
      tft.fillRect(40, 120, 180, 60, 0x3f79);
      i=0;
      cursorx=70;
    }*/
    
  //}
    //}
    //rtcdatejump_flag=0;
//  }
  }

void settings_companyname_menu()
{
   tft.textMode();

  tft.textSetCursor(150, 30);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0);
  tft.textWrite("Enter company name");
///// FOR string2////// 
  tft.drawRect(140,50,180,25,RA8875_BLACK);

  tft.graphicsMode();
}
void settings_companyname_menu_function()
{
  while(companyback_flag==0)
        {
          delay(150);
           if(cursorxa>350)
          {
            tft.fillRect(140,50,220,25,RA8875_WHITE);
            tft.drawRect(140,50,220,25,RA8875_BLACK);
            cursorxa=150;
            i=0;
           // clr_button[i]='\0';
          //  company_name[i]='\0';
          }
          if(! digitalRead(RA8875_INT))
          {
            
          tft.touchRead(&tx, &ty);

          
   //1//
    if(tx>886 && tx<1000 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("1");
      company_name[i]='1';// user entered password value 
      
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //2//
     if(tx>792 && tx<886 && ty>528 && ty<700)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("2");
     
      company_name[i]='2';// user entered password value
      
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //3//
    if(tx>732 && tx<792 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("3");
      
      company_name[i]='3';// user entered password value
      
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //4//
   if(tx>641 && tx<732 && ty>528 && ty<700)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("4");
      
      company_name[i]='4';// user entered password value 
      reenteredpass[i]=4; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //5//
     if(tx>554 && tx<641 && ty>528 && ty<700)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("5");
     
      company_name[i]='5';// user entered password value
      reenteredpass[i]=5; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //6//
    if(tx>467 && tx<554 && ty>528 && ty<700)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("6");
      
      company_name[i]='6';// user entered password value
      reenteredpass[i]=6; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //7//
     if(tx>395 && tx<467 && ty>528 && ty<700)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("7");
     
     company_name[i]='7';// user entered password value
      reenteredpass[i]=7; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //8//
     if(tx>308 && tx<395 && ty>528 && ty<700)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("8");
      
      company_name[i]='8';// user entered password value
      reenteredpass[i]=8; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //9//
    if(tx>238 && tx<308 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("9");
     
      company_name[i]='9';// user entered password value
      reenteredpass[i]=9; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //0//
    if(tx>138 && tx<238 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_BLACK);
    // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("0");
     
      company_name[i]='0';// user entered password value
      reenteredpass[i]=0; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
      Serial.print("ipf0");
    }

////abcd////
if(key==1)
{

//q//
   if(tx>873 && tx<977 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("q");
      
  
      company_name[i]='q';// user entered password value 
      
      
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //w//
    if(tx>787 && tx<872 && ty>374 && ty<528)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("w");
     
      company_name[i]='w';// user entered password value
      
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //e//
     if(tx>705 && tx<786 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("e");
     
      company_name[i]='e';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //r//
     if(tx>614 && tx<704 && ty>374 && ty<528)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("r");
      
      company_name[i]='r';// user entered password value 
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //t//
     if(tx>524 && tx<613 && ty>374 && ty<528)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("t");
     
      company_name[i]='t';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //y//
     if(tx>437 && tx<523 && ty>374 && ty<528)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("y");
     
      company_name[i]='y';// user entered password value
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //u//
     if(tx>368 && tx<436 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("u");
     
      company_name[i]='u';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //i//
    if(tx>263 && tx<368 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("i");
     
      company_name[i]='i';// user entered password value
      
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //o//
    if(tx>178 && tx<262 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("o");
     
     company_name[i]='o';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//p//
     if(tx>113 && tx<177 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("p");
     
  
      company_name[i]='p';// user entered password value 
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //a//
     if(tx>846 && tx<1000 && ty>233 && ty<374)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("a");
     
      company_name[i]='a';// user entered password value
   
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //s//
    if(tx>792 && tx<846 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("s");
      
      company_name[i]='s';// user entered password value
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //d//
     if(tx>732 && tx<792 && ty>233 && ty<374)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("d");
      
      company_name[i]='d';// user entered password value 
      
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //f//
     if(tx>641 && tx<732 && ty>233 && ty<374)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("f");
     
     company_name[i]='f';// user entered password value
      
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //g//
    if(tx>554 && tx<641 && ty>233 && ty<374)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("g");
     
      company_name[i]='g';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //h//
     if(tx>467 && tx<554 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("h");
      
     company_name[i]='h';// user entered password value
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //j//
     if(tx>395 && tx<467 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("j");
     
      company_name[i]='j';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //k//
     if(tx>308 && tx<395 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("k");
      
      company_name[i]='k';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//l//
    if(tx>238 && tx<308 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("l");
     
  
      company_name[i]='l';// user entered password value 
   
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //z//
    if(tx>138 && tx<238 && ty>233 && ty<374)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("z");
      
      company_name[i]='z';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //x//
    if(tx>787 && tx<872 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("x");
      
      company_name[i]='x';// user entered password value
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //c//
    if(tx>704 && tx<787 && ty>89 && ty<233)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("c");
     
      company_name[i]='c';// user entered password value 
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //v//
    if(tx>614 && tx<704 && ty>89 && ty<233)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("v");
     
      company_name[i]='v';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //b//
   if(tx>437 && tx<523 && ty>89 && ty<233)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("b");
      
      company_name[i]='b';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //space//
    if(tx>524 && tx<613 && ty>89 && ty<233)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      
      company_name[i]=' ';// user entered password value
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //n//
    if(tx>368 && tx<436 && ty>89 && ty<233)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("n");
     
      company_name[i]='n';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //m//
    if(tx>263 && tx<367 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("m");
     
      company_name[i]='m';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
}

////ABCD////

 if(key==2)
          {
//Q//
    if(tx>873 && tx<977 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Q");
  
      company_name[i]='Q';// user entered password value 
    
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //W//
    if(tx>787 && tx<872 && ty>374 && ty<528)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("W");
     
     company_name[i]='W';// user entered password value
     
   
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //E//
     if(tx>705 && tx<786 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("E");
     
      company_name[i]='E';// user entered password value
     
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //R//
    if(tx>614 && tx<704 && ty>374 && ty<528)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("R");
     
      company_name[i]='R';// user entered password value 
  
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //T//
    if(tx>524 && tx<613 && ty>374 && ty<528)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("T");
   
      company_name[i]='T';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //Y//
     if(tx>437 && tx<523 && ty>374 && ty<528)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Y");
     
      company_name[i]='Y';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //U//
    if(tx>368 && tx<436 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("U");
     
      company_name[i]='U';// user entered password value
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //I//
    if(tx>263 && tx<367 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("I");
     
      company_name[i]='I';// user entered password value
 
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //O//
    if(tx>178 && tx<262 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("O");
    
      company_name[i]='O';// user entered password value
  
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//P//
    if(tx>113 && tx<177 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("P");
     
  
      company_name[i]='P';// user entered password value 
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //A//
    if(tx>846 && tx<1000 && ty>233 && ty<374)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("A");
    
      company_name[i]='A';// user entered password value
   
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //S//
    if(tx>792 && tx<846 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("S");
     
      company_name[i]='S';// user entered password value
   
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //D//
   if(tx>732 && tx<792 && ty>233 && ty<374)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("D");
     
      company_name[i]='D';// user entered password value 
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //F//
    if(tx>641 && tx<732 && ty>233 && ty<374)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("F");
     
      company_name[i]='F';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //G//
     if(tx>554 && tx<641 && ty>233 && ty<374)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("G");
      
      company_name[i]='G';// user entered password value
   
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //H//
    if(tx>467 && tx<554 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("H");
     
      company_name[i]='H';// user entered password value
      
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //J//
    if(tx>395 && tx<467 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("j");
     
      company_name[i]='J';// user entered password value
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //K//
    if(tx>308 && tx<395 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("K");
      
      company_name[i]='K';// user entered password value
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//L//
    if(tx>238 && tx<308 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("L");
    
  
      company_name[i]='L';// user entered password value 
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //Z//
    if(tx>138 && tx<238 && ty>233 && ty<374)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Z");
   
      company_name[i]='Z';// user entered password value
   
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //X//
     if(tx>787 && tx<872 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("X");
    
      company_name[i]='X';// user entered password value
   
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //C//
    if(tx>704 && tx<787 && ty>89 && ty<233)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("C");
     
    company_name[i]='C';// user entered password value 
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //V//
    if(tx>614 && tx<704 && ty>89 && ty<233)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("V");
     
      company_name[i]='V';// user entered password value
  
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //B//
   if(tx>437 && tx<523 && ty>89 && ty<233)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("B");
     
      company_name[i]='B';// user entered password value
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //space//
    if(tx>524 && tx<613 && ty>89 && ty<233)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
    
      company_name[i]=' ';// user entered password value
   
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //N//
    if(tx>368 && tx<436 && ty>89 && ty<233)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("N");
      
      company_name[i]='N';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //M//
    if(tx>263 && tx<367 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("M");
     
      company_name[i]='M';// user entered password value
      
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
          }
    
//////capslock//
    if(tx>178 && tx<262 && ty>89 && ty<233)
    {
  
      capslock=1;
    }

    ////<--////
    if(tx>864 && tx<977 && ty>757 && ty<892)
    {
   
      cursorxa=150;
      i=0;
      companyback_flag=1;
       company_name[0]='\0';
       key=1;
    }

 /*   ////-->////
    if(tx>143 && tx<179 && ty>763 && ty<818)
    {
   
      cursorxa=150;
      i=0;
    }*/
    
     ///clear////
   if(tx>873 && tx<1000 && ty>89 && ty<233)
     { 
      
    // tft.touchEnable(true);
     //tft.fillRect(57, 120, 150, 58, RA8875_RED);
     tft.textMode();
   //  tft.cursorBlink(32);
     cursorxa=cursorxa-10;
     tft.textSetCursor(cursorxa,cursorya); ///text location////
     tft.textTransparent(RA8875_WHITE);
     tft.textEnlarge(0);  ////text size////
     tft.textWrite(company_name[i]);
     i=i-1;
      clr_button[0]=  company_name[i];
    clr_button[1]='\0';
     tft.textWrite(clr_button);
    // tft.textSetCursor(cursorxa, cursorya); ///text location////
      company_name[i]='\0';// user entered password value
   //  userenter[1]=0;// user entered password value
    // userenter[2]=0;// user entered password value
    // userenter[3]=0;// user entered password value
    //  i=i-1;
     if(cursorxa<150)
     {
      cursorxa=150;
      i=0;
     }
     tft.graphicsMode();
     Serial.print(userenter[i]);
     Serial.print(i);
  }
          
     //SAVE//

    if(tx>113 && tx<177 && ty>89 && ty<233)
    {
      tft.textMode();
    cursorxa=150;
    i=0;
    i2=12;
    for(i=0;i<=19;i++)
    {
    //save_company_name[i]=company_name[i];
     EEPROM.write((i+2000),0);
    }
    for(i=0;company_name[i]!='\0';i++)
    {
    save_company_name[i]=company_name[i];
     EEPROM.write((i+2000),save_company_name[i]);
    }
     if(company_name[i]=='\0')
    {
      EEPROM.write((i+2000),'\0');
    }
     for(i=0;i<=19;i++)
    {
    save_company_name[i]=company_name[i]=0;
    }
    i=0;
     tft.fillRect(140,50,220,25,RA8875_WHITE);
      tft.drawRect(140,50,220,25,RA8875_BLACK);
    Serial.print("ne");
    tft.graphicsMode();

  }
    
    }
      i2=0;      
 
}
}

void settings_instrumentid_menu()
{
   tft.textMode();

  tft.textSetCursor(150, 30);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0);
  tft.textWrite("Enter Instrument No.");
///// FOR string2////// 
  tft.drawRect(140,50,180,25,RA8875_BLACK);

  tft.graphicsMode();
}
/*
void settings_insturmentid_menu_function()
{
  while(instrumentback_flag==0)
        {
          delay(150);
          if(! digitalRead(RA8875_INT))
          {
            
          tft.touchRead(&tx, &ty);
   //1//
    if(tx>902 && tx<948 && ty>411 && ty<468)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("1");
      instrument_id[i]='1';// user entered password value 
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //2//
    if(tx>816 && tx<870 && ty>404 && ty<472)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("2");
     
      instrument_id[i]='2';// user entered password value
      
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //3//
    if(tx>734 && tx<791 && ty>411 && ty<461)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("3");
      
      instrument_id[i]='3';// user entered password value
      
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //4//
    if(tx>657 && tx<717 && ty>404 && ty<460)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("4");
      
      instrument_id[i]='4';// user entered password value 
      reenteredpass[i]=4; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //5//
    if(tx>565 && tx<625 && ty>379 && ty<448)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("5");
     
      instrument_id[i]='5';// user entered password value
      reenteredpass[i]=5; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //6//
    if(tx>479 && tx<549 && ty>378 && ty<451)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("6");
      
      instrument_id[i]='6';// user entered password value
      reenteredpass[i]=6; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //7//
    if(tx>386 && tx<447 && ty>380 && ty<457)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("7");
     
     instrument_id[i]='7';// user entered password value
      reenteredpass[i]=7; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //8//
    if(tx>305 && tx<365 && ty>375 && ty<454)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("8");
      
      instrument_id[i]='8';// user entered password value
      reenteredpass[i]=8; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //9//
    if(tx>222 && tx<285 && ty>377 && ty<454)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("9");
     
      instrument_id[i]='9';// user entered password value
      reenteredpass[i]=9; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //0//
    if(tx>126 && tx<196 && ty>266 && ty<456)
    {
   // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_BLACK);
    // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("0");
     
      instrument_id[i]='0';// user entered password value
      reenteredpass[i]=0; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
     
    }

////abcd////

if(key==1)
{
//q//
    if(tx>880 && tx<933 && ty>315 && ty<358)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("q");
      
  
      instrument_id[i]='q';// user entered password value 
      reenteredpass[i]=1; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //w//
    if(tx>798 && tx<851 && ty>321 && ty<359)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("w");
     
      instrument_id[i]='w';// user entered password value
      reenteredpass[i]=2; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //e//
    if(tx>712 && tx<789 && ty>299 && ty<349)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("e");
     
      instrument_id[i]='e';// user entered password value
      reenteredpass[i]=3; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //r//
    if(tx>621 && tx<680 && ty>323 && ty<363)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("r");
      
      instrument_id[i]='r';// user entered password value 
      reenteredpass[i]=4; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //t//
    if(tx>541 && tx<600 && ty>318 && ty<367)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("t");
     
      instrument_id[i]='t';// user entered password value
      reenteredpass[i]=5; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //y//
    if(tx>449 && tx<507 && ty>325 && ty<365)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("y");
     
      instrument_id[i]='y';// user entered password value
      reenteredpass[i]=6; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //u//
    if(tx>368 && tx<410 && ty>321 && ty<356)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("u");
     
      instrument_id[i]='u';// user entered password value
      reenteredpass[i]=7; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //i//
    if(tx>278 && tx<354 && ty>324 && ty<363)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("i");
     
      instrument_id[i]='i';// user entered password value
      reenteredpass[i]=8; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //o//
    if(tx>193 && tx<256 && ty>322 && ty<374)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("o");
     
     instrument_id[i]='o';// user entered password value
      reenteredpass[i]=9; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//p//
    if(tx>114 && tx<162 && ty>332 && ty<356)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("p");
     
  
      instrument_id[i]='p';// user entered password value 
      reenteredpass[i]=1; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //a//
    if(tx>900 && tx<950 && ty>229 && ty<264)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("a");
     
      instrument_id[i]='a';// user entered password value
      reenteredpass[i]=2; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //s//
    if(tx>819 && tx<873 && ty>221 && ty<268)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("s");
      
      instrument_id[i]='s';// user entered password value
      reenteredpass[i]=3; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //d//
    if(tx>743 && tx<800 && ty>210 && ty<258)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("d");
      
      instrument_id[i]='d';// user entered password value 
      reenteredpass[i]=4; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //f//
    if(tx>649 && tx<702 && ty>225 && ty<266)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("f");
     
     instrument_id[i]='f';// user entered password value
      reenteredpass[i]=5; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //g//
    if(tx>569 && tx<617 && ty>202 && ty<268)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("g");
     
      instrument_id[i]='g';// user entered password value
      reenteredpass[i]=6; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //h//
    if(tx>475 && tx<529 && ty>233 && ty<273)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("h");
      
     instrument_id[i]='h';// user entered password value
      reenteredpass[i]=7; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //j//
    if(tx>388 && tx<446 && ty>233 && ty<288)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("j");
     
      instrument_id[i]='j';// user entered password value
      reenteredpass[i]=8; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //k//
    if(tx>302 && tx<363 && ty>230 && ty<322)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("k");
      
      instrument_id[i]='k';// user entered password value
      reenteredpass[i]=9; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//i//
    if(tx>219 && tx<277 && ty>233 && ty<381)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("l");
     
  
      instrument_id[i]='l';// user entered password value 
      reenteredpass[i]=1; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //z//
    if(tx>143 && tx<177 && ty>229 && ty<273)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("z");
      
      instrument_id[i]='z';// user entered password value
      reenteredpass[i]=2; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //x//
    if(tx>882 && tx<933 && ty>149 && ty<187)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("x");
      
      instrument_id[i]='x';// user entered password value
      reenteredpass[i]=3; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //c//
    if(tx>808 && tx<855 && ty>144 && ty<189)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("c");
     
      instrument_id[i]='c';// user entered password value 
      reenteredpass[i]=4; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //v//
    if(tx>719 && tx<767 && ty>164 && ty<209)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("v");
     
      instrument_id[i]='v';// user entered password value
      reenteredpass[i]=5; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //b//
    if(tx>619 && tx<679 && ty>153 && ty<243)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("b");
      
      instrument_id[i]='b';// user entered password value
      reenteredpass[i]=6; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //space//
    if(tx>460 && tx<600 && ty>152 && ty<253)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      
      instrument_id[i]=' ';// user entered password value
      reenteredpass[i]=7; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //n//
    if(tx>366 && tx<421 && ty>158 && ty<191)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("n");
     
      instrument_id[i]='n';// user entered password value
      reenteredpass[i]=8; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //m//
    if(tx>281 && tx<340 && ty>149 && ty<173)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("m");
     
      instrument_id[i]='m';// user entered password value
      reenteredpass[i]=9; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
}

///ABCD////
if(key==2)
{
  //Q//

    if(tx>880 && tx<933 && ty>315 && ty<358)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Q");
      
  
      instrument_id[i]='Q';// user entered password value 
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //W//
    if(tx>798 && tx<851 && ty>321 && ty<359)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("W");
     
      instrument_id[i]='W';// user entered password valu
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //E//
    if(tx>712 && tx<789 && ty>299 && ty<349)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("E");
     
      instrument_id[i]='E';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //R//
    if(tx>621 && tx<680 && ty>323 && ty<363)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("R");
      
      instrument_id[i]='R';// user entered password value 
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //T//
    if(tx>541 && tx<600 && ty>318 && ty<367)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("T");
     
      instrument_id[i]='T';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //Y//
    if(tx>449 && tx<507 && ty>325 && ty<365)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Y");
     
      instrument_id[i]='Y';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //U//
    if(tx>368 && tx<410 && ty>321 && ty<356)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("U");
     
      instrument_id[i]='U';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //I//
    if(tx>278 && tx<354 && ty>324 && ty<363)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("I");
     
      instrument_id[i]='I';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //O//
    if(tx>193 && tx<256 && ty>322 && ty<374)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("O");
     
     instrument_id[i]='O';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//P//
    if(tx>114 && tx<162 && ty>332 && ty<356)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("P");
     
  
      instrument_id[i]='P';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //A//
    if(tx>900 && tx<950 && ty>229 && ty<264)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("A");
     
      instrument_id[i]='A';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //S//
    if(tx>819 && tx<873 && ty>221 && ty<268)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("S");
      
      instrument_id[i]='S';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //D//
    if(tx>743 && tx<800 && ty>210 && ty<258)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("D");
      
      instrument_id[i]='D';// user entered password value 
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //F//
    if(tx>649 && tx<702 && ty>225 && ty<266)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("F");
     
     instrument_id[i]='F';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //G//
    if(tx>569 && tx<617 && ty>202 && ty<268)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("G");
     
      instrument_id[i]='G';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //H//
    if(tx>475 && tx<529 && ty>233 && ty<273)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("H");
      
     instrument_id[i]='H';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //J//
    if(tx>388 && tx<446 && ty>233 && ty<288)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("J");
     
      instrument_id[i]='J';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //K//
    if(tx>302 && tx<363 && ty>230 && ty<322)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("K");
      
      instrument_id[i]='K';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//L//
    if(tx>219 && tx<277 && ty>233 && ty<381)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("L");
     
  
      instrument_id[i]='L';// user entered password value 
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //Z//
    if(tx>143 && tx<177 && ty>229 && ty<273)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Z");
      
      instrument_id[i]='Z';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //X//
    if(tx>882 && tx<933 && ty>149 && ty<187)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("X");
      
      instrument_id[i]='X';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //C//
    if(tx>808 && tx<858 && ty>144 && ty<189)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("C");
     
      instrument_id[i]='C';// user entered password value 
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //V//
    if(tx>719 && tx<767 && ty>164 && ty<209)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("V");
     
      instrument_id[i]='V';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //B//
    if(tx>619 && tx<679 && ty>153 && ty<243)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("B");
      
      instrument_id[i]='B';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //space//
    if(tx>460 && tx<600 && ty>152 && ty<253)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      
      instrument_id[i]=' ';// user entered password value
      reenteredpass[i]=7; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //N//
    if(tx>366 && tx<421 && ty>158 && ty<191)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("N");
     
      instrument_id[i]='N';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //M//
    if(tx>281 && tx<340 && ty>149 && ty<173)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("M");
     
      instrument_id[i]='M';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
}
    
//capslock//
    if(tx>116 && tx<248 && ty>145 && ty<184)
    {
  
      capslock=1;
    }

    ////<--////
    if(tx>885 && tx<953 && ty>746 && ty<795)
    {
   
      cursorxa=150;
      i=0;
      instrumentback_flag=1;
       clr_button[1]='\0';
       instrument_id[i];
       key=1;
    }

 /*   ////-->////
    if(tx>143 && tx<179 && ty>763 && ty<818)
    {
   
      cursorxa=150;
      i=0;
    }*/
  /*  
     ///clear////
   if(tx>138 && tx<185 && ty>640 && ty<708)
     { 
      
    // tft.touchEnable(true);
     //tft.fillRect(57, 120, 150, 58, RA8875_RED);
     tft.textMode();
   //  tft.cursorBlink(32);
     cursorxa=cursorxa-10;
     tft.textSetCursor(cursorxa, 105); ///text location////
     tft.textTransparent(RA8875_WHITE);
     tft.textEnlarge(0);  ////text size////
     
    // tft.textSetCursor(cursorxa, cursorya); ///text location////
      i=i-1;
       clr_button[0]=instrument_id[i];
      clr_button[1]='\0';
     tft.textWrite(clr_button);
      instrument_id[i]='\0';// user entered password value
   //  userenter[1]=0;// user entered password value
    // userenter[2]=0;// user entered password value
    // userenter[3]=0;// user entered password value

    
     if(cursorxa<150)
     {
      cursorxa=150;
      i=0;
       instrument_id[i]='\0';
        clr_button[0]='\0';
     }
     tft.graphicsMode();
     Serial.print(userenter[i]);
     Serial.print(i);
     i=0;
  }
          
     //Enter//

    if(tx>126 && tx<179 && ty>522 && ty<600)
    {
     tft.fillRect(140,100,220,25,RA8875_WHITE);
            tft.drawRect(140,100,220,25,RA8875_BLACK);
    cursorxa=150;
    i=0;
    i2=12;
    for(j=0;instrument_id[j]!='\0';j++)
    {
    save_instrument_id[j]=instrument_id[j];
     EEPROM.write(j+2031,save_instrument_id[j]);
    }
    if(instrument_id[j]=='\0')
    {
      EEPROM.write(j+2031,'\0');
    }
    j=0;
    instrument_id[0]='\0';
    clr_button[0]='\0';
    Serial.print("ne");
     tft.fillRect(140,100,220,25,RA8875_WHITE);
            tft.drawRect(140,100,220,25,RA8875_BLACK);
    }
   }
    }
      i2=0;      
// j=0;
}
*/
//////admin---->>>configurations///////
void configuration_menu()
{
  tft.textMode();
  tft.fillScreen(0xffff);  //0x3f79
tft.fillRoundRect(130, 27, 220, 27,10 ,0x3f79);//0XC618
tft.textSetCursor(140,32 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Admin and password");

tft.fillRoundRect(130, 67, 220, 27,10 ,0x3f79);//0XC618
tft.textSetCursor(140,72 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("username1 and password");

tft.fillRoundRect(130, 107, 220, 27,10 ,0x3f79);//0XC618
tft.textSetCursor(140,112 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("username2 and password");

  tft.fillRoundRect(130, 147, 220, 27,10 ,0x3f79);//0XC618
tft.textSetCursor(140,152 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("username3 and password");

  tft.fillRoundRect(130, 187, 220, 27,10 ,0x3f79);//0XC618
tft.textSetCursor(140,192 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("username4 and password");

  tft.fillRoundRect(130, 227, 220, 27,10 ,0x3f79);//0XC618
tft.textSetCursor(140,232 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("username5 and password");
  
  tft.fillRoundRect(9, 20, 45, 30,4 ,0x0a0f);//0XC618
  tft.textSetCursor(20,24 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("<--");
  
  tft.graphicsMode();
  
}

void configuration_menu_function()
{
  ///admin///
  if(tx>359 && tx<722 && ty>711 && ty<792)
  {
     i=0;
    adminandpass=1;
    Serial.print("adminf");
  }

///user1///
  if(tx>344 && tx<726 && ty>629 && ty<704)
  {
     i=0;
    user1andpass=1;
  }

////user2///
  if(tx>348 && tx<725 && ty>508 && ty<569)
  {
     i=0;
    user2andpass=1;
  }

///user3///
  if(tx>348 && tx<733 && ty>396 && ty<451)
  {
     i=0;
    user3andpass=1;
  }

////user4///
  if(tx>351 && tx<726 && ty>274 && ty<346)
  {
     i=0;
    user4andpass=1;
  }

////user5////
  if(tx>348 && tx<728 && ty>174 && ty<228)
  {
     i=0;
    user5andpass=1;
  }

////<--////
    if(tx>864 && tx<977 && ty>757 && ty<892)
    {
   i=0;
      configback_flag=1;
    }
  
}

/*void adminoldpass_edit_menu()
{
   tft.textMode();
  tft.fillScreen(0xffff);  //0x3f79
//tft.fillRoundRect(130, 27, 220, 27,10 ,0x3f79);//0XC618
tft.textSetCursor(140,45 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Old Admin name and password");

  tft.textSetCursor(50,80 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Admin name :");
  tft.drawRect(160, 80, 180, 25,0x0000);//0XC618

  tft.textSetCursor(50,150 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("password :");
  tft.drawRect(160, 150, 180, 25,0x0000);//0XC618

   tft.fillRoundRect(420, 20, 45, 30,4 ,0x0a0f);//0XC618
  tft.textSetCursor(431,24 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Edit");

   tft.fillRoundRect(9, 20, 45, 30,4 ,0x0a0f);//0XC618
  tft.textSetCursor(20,24 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("<--");
  
  tft.graphicsMode();
 
}
void adminoldpass_edit_function()
{
  ////-->////
 
    if(tx>143 && tx<179 && ty>763 && ty<818)
    {
      adminoldedit_flag=1;
    }

     ////<--////
    if(tx>885 && tx<953 && ty>746 && ty<795)
    {
      adminoldback_flag=1;
    }
    
    
}

void adminnewpass_edit_menu()
{
tft.textMode();
 
      tft.textSetCursor(50,60 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Admin name :");
  tft.drawRect(160, 60, 180, 25,0x0000);//0XC618

  tft.textSetCursor(50,100 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("password :");
  tft.drawRect(160, 100, 180, 25,0x0000);//0XC618

 
    tft.graphicsMode();
}

void adminnewpass_edit_function()
{
  if(tx>352 && tx<682 && ty>652 && ty<696)
  {
    adminnewname=1;
  }
  if(tx>360 && tx<683 && ty>552 && ty<589)
  {
    adminnewpass=1;
  }
/*  ////<--////
    if(tx>885 && tx<953 && ty>746 && ty<795)
    {
      adminnewdback_flag=1;
    }
*/
   /*  if(tx>143 && tx<179 && ty>763 && ty<818)
    {
      adminoldedit_flag=1;
    }
  
}
*/
void userandpasschange_menu()
{
 tft.textMode();

  tft.textSetCursor(150,30);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0);
  tft.textWrite("Enter User Name");
///// FOR string2////// 
  tft.drawRect(140,50,180,25,RA8875_BLACK);

  tft.graphicsMode();
}

void userandpasschange2_menu()
{
   tft.textMode();

  tft.textSetCursor(150, 30);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0);
  tft.textWrite("Enter Password");
///// FOR string2////// 
  tft.drawRect(140,50,180,25,RA8875_BLACK);

  tft.graphicsMode();
}

void userandpasschange_function()
{
  int q;

  if(cursorxa>350)
          {
            tft.fillRect(140,50,220,25,RA8875_WHITE);
            tft.drawRect(140,50,220,25,RA8875_BLACK);
            cursorxa=150;
            i=0;
            newu1reenteredpass[i]='\0';
          }

  
   //1//
    if(tx>886 && tx<1000 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("1");
      
      newu1reenteredpass[i]='1'; //to change password
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //2//
    if(tx>792 && tx<886 && ty>528 && ty<700)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("2");
     
      newu1reenteredpass[i]='2'; //to change password
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //3//
    if(tx>732 && tx<792 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("3");
      
      newu1reenteredpass[i]='3'; //to change password
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //4//
    if(tx>641 && tx<732 && ty>528 && ty<700)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
     tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("4");
      newu1reenteredpass[i]='4'; //to change password
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //5//
     if(tx>554 && tx<641 && ty>528 && ty<700)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("5");
     
      newu1reenteredpass[i]='5'; //to change password
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //6//
     if(tx>467 && tx<554 && ty>528 && ty<700)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("6");
     
      newu1reenteredpass[i]='6'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //7//
    if(tx>395 && tx<467 && ty>528 && ty<700)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("7");
      
     
      newu1reenteredpass[i]='7'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //8//
    if(tx>308 && tx<395 && ty>528 && ty<700)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("8");
      
      //userenter[i]='8';// user entered password value
      newu1reenteredpass[i]='8'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //9//
    if(tx>238 && tx<308 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("9");
     
      
      newu1reenteredpass[i]='9'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //0//
    if(tx>138 && tx<238 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_BLACK);
    // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("0");
     
     
      newu1reenteredpass[i]='0'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
      
    }

////abcd////
if(key==1)
{
//q//
    if(tx>873 && tx<977 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("q");
     
      newu1reenteredpass[i]='q'; //to change password
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //w//
    if(tx>787 && tx<872 && ty>374 && ty<528)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("w");
      
      newu1reenteredpass[i]='w'; //to change password
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //e//
    if(tx>705 && tx<786 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("e");
     
      newu1reenteredpass[i]='e'; //to change password
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //r//
     if(tx>614 && tx<704 && ty>374 && ty<528)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("r");
      
      newu1reenteredpass[i]='r'; //to change password
      
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //t//
    if(tx>524 && tx<613 && ty>374 && ty<528)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("t");
     
      newu1reenteredpass[i]='t'; //to change password
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //y//
     if(tx>437 && tx<523 && ty>374 && ty<528)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("y");
      
      newu1reenteredpass[i]='y'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //u//
     if(tx>368 && tx<436 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("u");
      
      newu1reenteredpass[i]='u'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //i//
    if(tx>263 && tx<368 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("i");
      
      newu1reenteredpass[i]='i'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //o//
    if(tx>178 && tx<262 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("o");
     
      newu1reenteredpass[i]='o'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//p//
    if(tx>113 && tx<177 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("p");
      
      newu1reenteredpass[i]='p'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //a//
    if(tx>846 && tx<1000 && ty>233 && ty<374)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("a");
     
      newu1reenteredpass[i]='a'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //s//
    if(tx>792 && tx<846 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("s");
     
      newu1reenteredpass[i]='s'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //d//
    if(tx>732 && tx<792 && ty>233 && ty<374)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("d");
      
      newu1reenteredpass[i]='d'; //to change passwor
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //f//
    if(tx>641 && tx<732 && ty>233 && ty<374)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("f");
      newu1reenteredpass[i]='f'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //g//
    if(tx>554 && tx<641 && ty>233 && ty<374)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("g");
     
      newu1reenteredpass[i]='g'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //h//
     if(tx>467 && tx<554 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("h");
      
      newu1reenteredpass[i]='h'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //j//
    if(tx>395 && tx<467 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("j");
     
      newu1reenteredpass[i]='j'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //k//
    if(tx>308 && tx<395 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("k");
     
      newu1reenteredpass[i]='k'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//i//
    if(tx>238 && tx<308 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("l");
      
      newu1reenteredpass[i]='l'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //z//
    if(tx>138 && tx<238 && ty>233 && ty<374)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("z");
      
      newu1reenteredpass[i]='z'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //x//
    if(tx>787 && tx<872 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("x");
      
      newu1reenteredpass[i]='z'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //c//
    if(tx>704 && tx<787 && ty>89 && ty<233)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("c");
     
      newu1reenteredpass[i]='c'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //v//
     if(tx>614 && tx<704 && ty>89 && ty<233)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("v");
     
      newu1reenteredpass[i]='v'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //b//
     if(tx>437 && tx<523 && ty>89 && ty<233)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("b");
     
      newu1reenteredpass[i]='b'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

  /*  //space//
    if(tx>460 && tx<600 && ty>152 && ty<253)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      userenter[i]=' ';// user entered password value
      reenteredpass[i]=7; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }
*/
    //n//
    if(tx>368 && tx<436 && ty>89 && ty<233)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("n");
     
      newu1reenteredpass[i]='n'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //m//
    if(tx>263 && tx<367 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("m");
      
      newu1reenteredpass[i]='m'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
}

if(key==2)
{
  //Q//

    if(tx>873 && tx<977 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Q");
      
  
      newu1reenteredpass[i]='Q';// user entered password value 
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //W//
    if(tx>787 && tx<872 && ty>374 && ty<528)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("W");
     
      newu1reenteredpass[i]='W';// user entered password valu
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //E//
    if(tx>705 && tx<786 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("E");
     
      newu1reenteredpass[i]='E';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //R//
    if(tx>614 && tx<704 && ty>374 && ty<528)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("R");
      
     newu1reenteredpass[i]='R';// user entered password value 
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //T//
    if(tx>524 && tx<613 && ty>374 && ty<528)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("T");
     
      newu1reenteredpass[i]='T';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //Y//
    if(tx>437 && tx<523 && ty>374 && ty<528)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Y");
     
      newu1reenteredpass[i]='Y';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //U//
    if(tx>368 && tx<436 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("U");
     
      newu1reenteredpass[i]='U';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //I//
    if(tx>263 && tx<367 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("I");
     
     newu1reenteredpass[i]='I';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //O//
    if(tx>178 && tx<262 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("O");
     
     newu1reenteredpass[i]='O';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//P//
    if(tx>113 && tx<177 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("P");
     
  
      newu1reenteredpass[i]='P';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //A//
    if(tx>846 && tx<1000 && ty>233 && ty<374)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("A");
     
      newu1reenteredpass[i]='A';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //S//
    if(tx>792 && tx<846 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("S");
      
     newu1reenteredpass[i]='S';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //D//
    if(tx>732 && tx<792 && ty>233 && ty<374)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("D");
      
     newu1reenteredpass[i]='D';// user entered password value 
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //F//
   if(tx>641 && tx<732 && ty>233 && ty<374)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("F");
     
    newu1reenteredpass[i]='F';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //G//
    if(tx>554 && tx<641 && ty>233 && ty<374)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("G");
     
     newu1reenteredpass[i]='G';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //H//
    if(tx>467 && tx<554 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("H");
      
     newu1reenteredpass[i]='H';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //J//
    if(tx>395 && tx<467 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("J");
     
      newu1reenteredpass[i]='J';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //K//
    if(tx>308 && tx<395 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("K");
      
     newu1reenteredpass[i]='K';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//L//
     if(tx>238 && tx<308 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("L");
     
  
     newu1reenteredpass[i]='L';// user entered password value 
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //Z//
    if(tx>138 && tx<238 && ty>233 && ty<374)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Z");
      
     newu1reenteredpass[i]='Z';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //X//
    if(tx>787 && tx<872 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("X");
      
      newu1reenteredpass[i]='X';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //C//
    if(tx>704 && tx<787 && ty>89 && ty<233)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("C");
     
     newu1reenteredpass[i]='C';// user entered password value 
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //V//
     if(tx>614 && tx<704 && ty>89 && ty<233)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("V");
     
      newu1reenteredpass[i]='V';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //B//
    if(tx>437 && tx<523 && ty>89 && ty<233)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("B");
      
     newu1reenteredpass[i]='B';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

  /*  //space//
    if(tx>437 && tx<613 && ty>105 && ty<201)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      
      newu1reenteredpass[i]=' ';// user entered password value
      reenteredpass[i]=7; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }*/

    //N//
    if(tx>368 && tx<436 && ty>89 && ty<233)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("N");
     
      newu1reenteredpass[i]='N';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //M//
    if(tx>263 && tx<367 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("M");
     
     newu1reenteredpass[i]='M';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
}
    
//capslock//
    if(tx>178 && tx<262 && ty>89 && ty<233)
    {
      capslock=1;
    }

    ////<--////
    if(tx>864 && tx<977 && ty>757 && ty<892)
    {
   userandpasschangeback_flag=1;
   newu1reenteredpass[0]='\0';
   i=0;
   cursorxa=150;
   key=1;
   pressenter=0;
    }

    ////-->////
    if(tx>113 && tx<211 && ty>756 && ty<872)
    {
   userandpasschangenext_flag=1;
    newu1reenteredpass[0]='\0';
   i=0;
   cursorxa=150;
   key=1;
   pressenter=0;
    }
    
     ///clear////
    if(tx>873 && tx<1000 && ty>89 && ty<233)
     {
      
    // tft.touchEnable(true);
     //tft.fillRect(57, 120, 150, 58, RA8875_RED);
     tft.textMode();
   //  tft.cursorBlink(32);
     cursorxa=cursorxa-10;
     tft.textSetCursor(cursorxa,cursorya); ///text location////
     tft.textTransparent(RA8875_WHITE);
     tft.textEnlarge(0);  ////text size////
     tft.textWrite(reenteredpass[i]);
    // tft.textSetCursor(cursorxa, cursorya); ///text location////
      i=i-1;
      
   //  userenter[1]=0;// user entered password value
    // userenter[2]=0;// user entered password value
    // userenter[3]=0;// user entered password value

     
       clr_button[0]= newu1reenteredpass[i];
      clr_button[1]='\0';
     tft.textWrite(clr_button);
     newu1reenteredpass[i]='\0'; //to change password
     //reenteredpass[i]=0; //to change password
     //reenteredpass[i]=0; //to change password
     //reenteredpass[i]=0; //to change password
   //  i=i-1;
     if(cursorxa<150)
     {
      cursorxa=150;
      i=0;
       newu1reenteredpass[i]='\0';
     }
     pressenter=0;
     tft.graphicsMode();
    
  }

  //enter//
    if(tx>113 && tx<177 && ty>89 && ty<233)
    {
      if(pressenter==0)
      {
      tft.textMode();
    cursorxa=150;
     //tft.textMode();
   //  tft.cursorBlink(32);
    // cursorxa=cursorxa-10;
     tft.textSetCursor(cursorxa,cursorya); ///text location////
     for(q=0;q<=7;q++)
     {
     // cursorxa=cursorxa+10;
     
    tft.textSetCursor(cursorxa,cursorya);
     tft.textTransparent(RA8875_WHITE);
     tft.textEnlarge(0); 
     ////text size////
    // for(q=0;q<=7;q++)
    // {
     tft.textWrite("*");
     cursorxa=cursorxa+10;
     }
      newu1reenteredpass[i]='\0';
      p2=i;
   // i=0;
   // i2=12;
    cursorxa=150;
 userandpasschangeenter_flag=1;
 pressenter=1;
  tft.fillRect(140,50,220,25,RA8875_WHITE);
            tft.drawRect(140,50,220,25,RA8875_BLACK);
    tft.graphicsMode();
    Serial.print("ne");
    }
    }
    
}

void userandpasschange2_function()
{
  int q;

  if(cursorxa>350)
          {
            tft.fillRect(140,50,220,25,RA8875_WHITE);
            tft.drawRect(140,50,220,25,RA8875_BLACK);
            cursorxa=150;
            i=0;
             newreenteredpass[i]='\0';
          }
  
   //1//
    if(tx>886 && tx<1000 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("1");
      
      newreenteredpass[i]='1'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //2//
    if(tx>792 && tx<886 && ty>528 && ty<700)
    {   
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("2");
     
      newreenteredpass[i]='2'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //3//
    if(tx>732 && tx<792 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("3");
      
      newreenteredpass[i]='3'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //4//
    if(tx>641 && tx<732 && ty>528 && ty<700)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("4");
      newreenteredpass[i]='4'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //5//
     if(tx>554 && tx<641 && ty>528 && ty<700)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("5");
     
      newreenteredpass[i]='5'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //6//
    if(tx>467 && tx<554 && ty>528 && ty<700)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("6");
     
      newreenteredpass[i]='6'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //7//
     if(tx>395 && tx<467 && ty>528 && ty<700)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("7");
      
     
      newreenteredpass[i]='7'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //8//
     if(tx>308 && tx<395 && ty>528 && ty<700)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("8");
      
     // userenter[i]='8';// user entered password value
      newreenteredpass[i]='8'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //9//
    if(tx>238 && tx<308 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("9");
     
      
      newreenteredpass[i]='9'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //0//
     if(tx>138 && tx<238 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_BLACK);
    // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("0");
     
     
      newreenteredpass[i]='0'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
      Serial.print("ipf0");
    }

////abcd////

if(key==1)
{
//q//
     if(tx>873 && tx<977 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("q");
     
      newreenteredpass[i]='q'; //to change password
      
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //w//
    if(tx>787 && tx<872 && ty>374 && ty<528)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("w");
      
      newreenteredpass[i]='w'; //to change password
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //e//
     if(tx>705 && tx<786 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("e");
     
      newreenteredpass[i]='e'; //to change password
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //r//
    if(tx>614 && tx<704 && ty>374 && ty<528)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("r");
      
      newreenteredpass[i]='r'; //to change password
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //t//
     if(tx>524 && tx<613 && ty>374 && ty<528)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("t");
     
      newreenteredpass[i]='t'; //to change password
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //y//
   if(tx>437 && tx<523 && ty>374 && ty<528)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("y");
      
      newreenteredpass[i]='y'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //u//
     if(tx>368 && tx<436 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("u");
      
      newreenteredpass[i]='u'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //i//
    if(tx>263 && tx<368 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("i");
      
     newreenteredpass[i]='i'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //o//
     if(tx>178 && tx<262 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("o");
     
      newreenteredpass[i]='o'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//p//
     if(tx>113 && tx<177 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("p");
      
      newreenteredpass[i]='p'; //to change password
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //a//
    if(tx>846 && tx<1000 && ty>233 && ty<374)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("a");
     
      newreenteredpass[i]='a'; //to change password
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //s//
    if(tx>792 && tx<846 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("s");
     
     newreenteredpass[i]='s'; //to change password
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //d//
    if(tx>732 && tx<792 && ty>233 && ty<374)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("d");
      
      newreenteredpass[i]='d'; //to change password
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //f//
    if(tx>641 && tx<732 && ty>233 && ty<374)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("f");
      newreenteredpass[i]='f'; //to change password
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //g//
    if(tx>554 && tx<641 && ty>233 && ty<374)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("g");
     
      newreenteredpass[i]='g'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //h//
    if(tx>467 && tx<554 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("h");
      
      newreenteredpass[i]='h'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //j//
    if(tx>395 && tx<467 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("j");
     
      newreenteredpass[i]='j'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //k//
    if(tx>308 && tx<395 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("k");
     
      newreenteredpass[i]='k'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//l//
    if(tx>238 && tx<308 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("l");
      
      newreenteredpass[i]='l'; //to change password
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //z//
     if(tx>138 && tx<238 && ty>233 && ty<374)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("z");
      
      newreenteredpass[i]='z'; //to change password
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //x//
   if(tx>787 && tx<872 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("x");
      
      newreenteredpass[i]='z'; //to change password
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //c//
    if(tx>704 && tx<787 && ty>89 && ty<233)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("c");
     
      newreenteredpass[i]='c'; //to change password
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //v//
    if(tx>614 && tx<704 && ty>89 && ty<233)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("v");
     
      newreenteredpass[i]='v'; //to change password
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //b//
     if(tx>437 && tx<523 && ty>89 && ty<233)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("b");
     
      newreenteredpass[i]='b'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

  /*  //space//
    if(tx>460 && tx<600 && ty>152 && ty<253)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      userenter[i]=' ';// user entered password value
      reenteredpass[i]=7; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }
*/
    //n//
   if(tx>368 && tx<436 && ty>89 && ty<233)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("n");
     
      newreenteredpass[i]='n'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //m//
     if(tx>263 && tx<367 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("m");
      
      newreenteredpass[i]='m'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
}

////ABCD////

 if(key==2)
          {
//Q//
    if(tx>873 && tx<977 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Q");
  
      newreenteredpass[i]='Q';// user entered password value 
    
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //W//
     if(tx>787 && tx<872 && ty>374 && ty<528)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("W");
     
    newreenteredpass[i]='W';// user entered password value
     
   
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //E//
    if(tx>705 && tx<786 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("E");
     
     newreenteredpass[i]='E';// user entered password value
     
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //R//
    if(tx>614 && tx<704 && ty>374 && ty<528)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("R");
     
      newreenteredpass[i]='R';// user entered password value 
  
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //T//
     if(tx>524 && tx<613 && ty>374 && ty<528)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("T");
   
      newreenteredpass[i]='T';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //Y//
     if(tx>437 && tx<523 && ty>374 && ty<528)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Y");
     
     newreenteredpass[i]='Y';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //U//
    if(tx>368 && tx<436 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("U");
     
      newreenteredpass[i]='U';// user entered password value
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //I//
     if(tx>263 && tx<367 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("I");
     
      newreenteredpass[i]='I';// user entered password value
 
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //O//
    if(tx>178 && tx<262 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("O");
    
     newreenteredpass[i]='O';// user entered password value
  
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//P//
    if(tx>113 && tx<177 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("P");
     
  
      newreenteredpass[i]='P';// user entered password value 
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //A//
    if(tx>846 && tx<1000 && ty>233 && ty<374)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("A");
    
      newreenteredpass[i]='A';// user entered password value
   
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //S//
     if(tx>792 && tx<846 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("S");
     
      newreenteredpass[i]='S';// user entered password value
   
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //D//
     if(tx>732 && tx<792 && ty>233 && ty<374)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("D");
     
      newreenteredpass[i]='D';// user entered password value 
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //F//
    if(tx>641 && tx<732 && ty>233 && ty<374)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("F");
     
     newreenteredpass[i]='F';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //G//
    if(tx>554 && tx<641 && ty>233 && ty<374)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("G");
      
     newreenteredpass[i]='G';// user entered password value
   
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //H//
    if(tx>467 && tx<554 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("H");
     
      newreenteredpass[i]='H';// user entered password value
      
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //J//
    if(tx>395 && tx<467 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("j");
     
     newreenteredpass[i]='J';// user entered password value
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //K//
    if(tx>308 && tx<395 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("K");
      
      newreenteredpass[i]='K';// user entered password value
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//L//
    if(tx>238 && tx<308 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("L");
    
  
     newreenteredpass[i]='L';// user entered password value 
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //Z//
    if(tx>138 && tx<238 && ty>233 && ty<374)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Z");
   
      newreenteredpass[i]='Z';// user entered password value
   
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //X//
    if(tx>787 && tx<872 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("X");
    
      newreenteredpass[i]='X';// user entered password value
   
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //C//
     if(tx>704 && tx<787 && ty>89 && ty<233)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("C");
     
    newreenteredpass[i]='C';// user entered password value 
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //V//
   if(tx>614 && tx<704 && ty>89 && ty<233)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("V");
     
     newreenteredpass[i]='V';// user entered password value
  
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //B//
    if(tx>437 && tx<523 && ty>89 && ty<233)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("B");
     
     newreenteredpass[i]='B';// user entered password value
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
/*
    //space//
    if(tx>460 && tx<600 && ty>105 && ty<201)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
    
      newreenteredpass[i]=' ';// user entered password value
   
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }
*/
    //N//
   if(tx>368 && tx<436 && ty>89 && ty<233)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("N");
      
      newreenteredpass[i]='N';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //M//
    if(tx>263 && tx<367 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("M");
     
      newreenteredpass[i]='M';// user entered password value
      
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
          }
    
//capslock//
    if(tx>178 && tx<262 && ty>89 && ty<233)
    {
  /* // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("9");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("9");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      userenter[i]='9';// user entered password value
      reenteredpass[i]=9; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;*/
      capslock=1;
    }

    ////<--////
    if(tx>864 && tx<977 && ty>757 && ty<892)
    {
   userandpasschangeback2_flag=1;
   newreenteredpass[0]='\0';
   i=0;
   cursorxa=150;
   key=1;
   pressenter=0;
    }

    ////-->////
    if(tx>113 && tx<211 && ty>756 && ty<872)
    {
   userandpasschangenext2_flag=1;
   newreenteredpass[0]='\0';
   i=0;
   cursorxa=150;
   key=1;
   pressenter=0;
    }
    
     ///clear////
   if(tx>113 && tx<211 && ty>631 && ty<755)
     { 
      
    // tft.touchEnable(true);
     //tft.fillRect(57, 120, 150, 58, RA8875_RED);
     tft.textMode();
   //  tft.cursorBlink(32);
     cursorxa=cursorxa-10;
     tft.textSetCursor(cursorxa, cursorya); ///text location////
     tft.textTransparent(RA8875_WHITE);
     tft.textEnlarge(0);  ////text size////
     tft.textWrite(reenteredpass[i]);
    // tft.textSetCursor(cursorxa, cursorya); ///text location////
    //  i=i-1;
      
   //  userenter[1]=0;// user entered password value
    // userenter[2]=0;// user entered password value
    // userenter[3]=0;// user entered password value
       i=i-1;
       clr_button[0]=newreenteredpass[i];
      clr_button[1]='\0';
     tft.textWrite(clr_button);
     newreenteredpass[i]='\0'; //to change password
     //reenteredpass[i]=0; //to change password
     //reenteredpass[i]=0; //to change password
     //reenteredpass[i]=0; //to change password
   //  i=i-1;
     if(cursorxa<150)
     {
      cursorxa=150;
      i=0;
       newreenteredpass[i]='\0';
     }
     pressenter=0;
     tft.graphicsMode();
     Serial.print(userenter[i]);
     Serial.print(i);
  }

  //enter//
    if(tx>113 && tx<211 && ty>496 && ty<630)
    {
      if(pressenter==0)
      {
      
      tft.textMode();
    cursorxa=150;
     //tft.textMode();
   //  tft.cursorBlink(32);
    // cursorxa=cursorxa-10;
     tft.textSetCursor(cursorxa,cursorya); ///text location////
     for(q=0;q<=7;q++)
     {
     // cursorxa=cursorxa+10;
     
    tft.textSetCursor(cursorxa,cursorya);
     tft.textTransparent(RA8875_WHITE);
     tft.textEnlarge(0); 
     ////text size////
    // for(q=0;q<=7;q++)
    // {
     tft.textWrite("*");
     cursorxa=cursorxa+10;
     }
     newreenteredpass[i]='\0';
    /* if(adminandpass==1)
     {
      EEPROM.write((i+50),'\0');
     }*/
     pressenter=1;
     p2=i;
      Serial.print(p2);
   // i=0;
   // i2=12;
    cursorxa=150;
 userandpasschangeenter2_flag=1;
  tft.fillRect(140,50,220,25,RA8875_WHITE);
            tft.drawRect(140,50,220,25,RA8875_BLACK);
    tft.graphicsMode();
    Serial.print("ne");
    }
    }
}
          





////////////maintanance/////////////////

void maintenancepass_menu()
{
  tft.textMode();

  tft.textSetCursor(150, 30);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0);
  tft.textWrite("Enter Maintanance pass.");
///// FOR string2////// 
  tft.drawRect(140,50,180,25,RA8875_BLACK);

  tft.graphicsMode();
}

void maintanencepass_menu_function()
{
  int q=0;
    if(cursorxa>350)
          {
            tft.fillRect(140,50,220,25,RA8875_WHITE);
            tft.drawRect(140,50,220,25,RA8875_BLACK);
            cursorxa=150;
            i=0;
             maintainence_pass[i]='\0';
             //clr_button[i]='\0';
          }
   //1//
    if(tx>886 && tx<1000 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("1");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("1");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
  
      maintainence_pass[i]='1';// user entered password value 
    //  loginpass[i]='1'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //2//
    if(tx>792 && tx<886 && ty>528 && ty<700)
    {  
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("2");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("2");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='2';// user entered password value
    //  loginpass[i]='2'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //3//
     if(tx>732 && tx<792 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("3");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("3");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='3';// user entered password value
    //  loginpass[i]='3'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //4//
    if(tx>641 && tx<732 && ty>528 && ty<700)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("4");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("4");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='4';// user entered password value 
    //  loginpass[i]='4'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //5//
   if(tx>554 && tx<641 && ty>528 && ty<700)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("5");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("5");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='5';// user entered password value
    //  loginpass[i]='5'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //6//
    if(tx>467 && tx<554 && ty>528 && ty<700)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("6");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("6");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='6';// user entered password value
    //  loginpass[i]='6'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //7//
    if(tx>395 && tx<467 && ty>528 && ty<700)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("7");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("7");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='7';// user entered password value
    //  loginpass[i]='7'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //8//
     if(tx>308 && tx<395 && ty>528 && ty<700)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("8");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("8");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='8';// user entered password value
     // loginpass[i]='8'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //9//
     if(tx>238 && tx<308 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("9");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("9");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='9';// user entered password value
     // loginpass[i]='9'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //0//
    if(tx>138 && tx<238 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_BLACK);
    // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("0");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("0");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='0';// user entered password value
   //   loginpass[i]='0'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
      Serial.print("ipf0");
    }

////abcd////

if(key==1)
{
//q//
     if(tx>873 && tx<977 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("q");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("q");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
  
       maintainence_pass[i]='q';// user entered password value 
    //  loginpass[i]='q'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //w//
     if(tx>787 && tx<872 && ty>374 && ty<528)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("w");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("w");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='w';// user entered password value
    //  loginpass[i]='w'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //e//
    if(tx>705 && tx<786 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("e");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("e");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='e';// user entered password value
     // loginpass[i]='e'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //r//
     if(tx>614 && tx<704 && ty>374 && ty<528)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("r");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("r");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='r';// user entered password value 
     // loginpass[i]='r'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //t//
     if(tx>524 && tx<613 && ty>374 && ty<528)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("t");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("t");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='t';// user entered password value
     // loginpass[i]='t'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //y//
    if(tx>437 && tx<523 && ty>374 && ty<528)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("y");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("y");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='y';// user entered password value
     // loginpass[i]='y'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //u//
    if(tx>368 && tx<436 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("u");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("u");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='u';// user entered password value
    //  loginpass[i]='u'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //i//
   if(tx>263 && tx<368 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("i");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("i");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='i';// user entered password value
     // loginpass[i]='i'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //o//
    if(tx>178 && tx<262 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("o");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("o");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='o';// user entered password value
    //  loginpass[i]='o'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//p//
    if(tx>113 && tx<177 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("p");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("p");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
  
       maintainence_pass[i]='p';// user entered password value 
    //  loginpass[i]='p'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //a//
   if(tx>846 && tx<1000 && ty>233 && ty<374)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("a");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("a");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='a';// user entered password value
    //  loginpass[i]='a'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //s//
    if(tx>792 && tx<846 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("s");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("s");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='s';// user entered password value
     // loginpass[i]='s'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //d//
    if(tx>732 && tx<792 && ty>233 && ty<374)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("d");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("d");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='d';// user entered password value 
   //   loginpass[i]='d'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //f//
   if(tx>641 && tx<732 && ty>233 && ty<374)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("f");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("f");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='f';// user entered password value
     // loginpass[i]='f'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //g//
    if(tx>554 && tx<641 && ty>233 && ty<374)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("g");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("g");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='g';// user entered password value
     // loginpass[i]='g'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //h//
    if(tx>467 && tx<554 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("h");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("h");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='h';// user entered password value
     // loginpass[i]='h'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //j//
    if(tx>395 && tx<467 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("j");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("j");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='j';// user entered password value
     // loginpass[i]='j'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //k//
     if(tx>308 && tx<395 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("k");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("k");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='k';// user entered password value
     // loginpass[i]='k'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//l//
    if(tx>238 && tx<308 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("l");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("l");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
  
       maintainence_pass[i]='l';// user entered password value 
     // loginpass[i]='l'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //z//
    if(tx>138 && tx<238 && ty>233 && ty<374)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("z");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("z");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='z';// user entered password value
     // loginpass[i]='z'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //x//
    if(tx>787 && tx<872 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("x");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("x");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='x';// user entered password value
      //loginpass[i]='x'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //c//
    if(tx>704 && tx<787 && ty>89 && ty<233)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("c");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("c");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='c';// user entered password value 
      //loginpass[i]='c'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //v//
   if(tx>614 && tx<704 && ty>89 && ty<233)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("v");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("v");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='v';// user entered password value
     // loginpass[i]='v'; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //b//
   if(tx>437 && tx<523 && ty>89 && ty<233)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("b");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("b");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='b';// user entered password value
     // loginpass[i]='b'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

  /*  //space//
    if(tx>460 && tx<600 && ty>152 && ty<253)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      userenter[i]=' ';// user entered password value
      reenteredpass[i]=7; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }
*/
    //n//
    if(tx>368 && tx<436 && ty>89 && ty<233)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("n");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("n");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='n';// user entered password value
     // loginpass[i]='n'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //m//
    if(tx>263 && tx<367 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("m");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("m");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='m';// user entered password value
      //loginpass[i]='m'; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
}

 if(key==2)
          {
//Q//
    if(tx>873 && tx<977 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Q");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Q");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
  
       maintainence_pass[i]='Q';// user entered password value 
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //W//
   if(tx>787 && tx<872 && ty>374 && ty<528)
    {  
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("W");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("W");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='W';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //E//
    if(tx>705 && tx<786 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("E");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("E");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='E';// user entered password value
      
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //R//
   if(tx>614 && tx<704 && ty>374 && ty<528)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("R");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("R");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='R';// user entered password value 
   
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //T//
     if(tx>524 && tx<613 && ty>374 && ty<528)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("T");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("T");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='T';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //Y//
     if(tx>437 && tx<523 && ty>374 && ty<528)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Y");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Y");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='Y';// user entered password value
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //U//
    if(tx>368 && tx<436 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("U");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("U");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='U';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //I//
    if(tx>263 && tx<367 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("I");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("I");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='I';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //O//
   if(tx>178 && tx<262 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("O");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("O");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='O';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//P//
    if(tx>113 && tx<177 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("P");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("P");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
  
       maintainence_pass[i]='P';// user entered password value 
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //A//
   if(tx>846 && tx<1000 && ty>233 && ty<374)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("A");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("A");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='A';// user entered password value
   
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //S//
    if(tx>792 && tx<846 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("S");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("S");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='S';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //D//
    if(tx>732 && tx<792 && ty>233 && ty<374)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("D");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("D");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='D';// user entered password value 
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //F//
    if(tx>641 && tx<732 && ty>233 && ty<374)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("F");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("F");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='F';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //G//
    if(tx>554 && tx<641 && ty>233 && ty<374)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("G");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("G");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='G';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //H//
    if(tx>467 && tx<554 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("H");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("H");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='H';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //J//
   if(tx>395 && tx<467 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("j");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("J");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='J';// user entered password value
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //K//
    if(tx>308 && tx<395 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("K");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("K");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='K';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//L//
    if(tx>238 && tx<308 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("L");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("L");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
  
       maintainence_pass[i]='L';// user entered password value 
      
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //Z//
     if(tx>138 && tx<238 && ty>233 && ty<374)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Z");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Z");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='Z';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //X//
    if(tx>787 && tx<872 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("X");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("X");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='X';// user entered password value
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //C//
   if(tx>704 && tx<787 && ty>89 && ty<233)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("C");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("C");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='C';// user entered password value 
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //V//
    if(tx>614 && tx<704 && ty>89 && ty<233)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("V");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("V");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='V';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //B//
     if(tx>437 && tx<523 && ty>89 && ty<233)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("B");
      delay(300);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("B");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      maintainence_pass[i]='B';// user entered password value
   
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

  /*  //space//
    if(tx>460 && tx<600 && ty>152 && ty<253)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      userenter[i]=' ';// user entered password value
      reenteredpass[i]=7; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }
*/
    //N//
    if(tx>368 && tx<436 && ty>89 && ty<233)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("N");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("N");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
      maintainence_pass[i]='N';// user entered password value
      
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //M//
    if(tx>263 && tx<367 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("M");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("M");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("*");
       maintainence_pass[i]='M';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
          }
    
//capslock//
    if(tx>178 && tx<262 && ty>89 && ty<233)
    {
      capslock=1;
    }

    ////<--////
    if(tx>864 && tx<977 && ty>757 && ty<892)
    {
     maintenenceback=1;
     key=1;
    // for(q=0;q<=20;q++)
    // {
      maintainence_pass[0]='\0';// user entered password value
    // }
    clr_button[0]='\0';
    i=0;
    cursorxa=150;
    
    // for(q=0;q<=8;q++)
    // {
    // maintainence_pass[i]='\0';
   //  }
   //  Serial.print("adminback_flag=1 ");
    }
/*
    ////-->////
    if(tx>143 && tx<179 && ty>763 && ty<818)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
     // tft.textEnlarge(0);  ////text size////
    //  tft.textWrite("9");
      delay(500);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_WHITE);
     // tft.textEnlarge(0);  ////text size////
     // tft.textWrite("9");
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
    //  tft.textEnlarge(0);  ////text size////
     // tft.textWrite("*");
     // userenter[i]='9';// user entered password value
      //reenteredpass[i]=9; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     // i++;
    }

  // }
 /*  if(loginenter==1)
   {
    break;
   }
  /* if(adminback_flag==1)
   {
    break;
   }*/
    // }
     //capslock//
   if(tx>178 && tx<262 && ty>89 && ty<233)
    {
      capslock=1;
    }
 /*   ////<--////
    if(tx>885 && tx<953 && ty>746 && ty<795)
    {
     loginback=1;
    }*/
     ///clear////
   if(tx>873 && tx<1000 && ty>89 && ty<233)
     {
      
    // tft.touchEnable(true);
     //tft.fillRect(57, 120, 150, 58, RA8875_RED);
     tft.textMode();
   //  tft.cursorBlink(32);
     cursorxa=cursorxa-10;
     tft.textSetCursor(cursorxa,cursorya); ///text location////
     tft.textTransparent(RA8875_WHITE);
     tft.textEnlarge(0);  ////text size////
     tft.textWrite("*");
    // tft.textSetCursor(cursorxa, cursorya); ///text location////
     // i=i-1;
     i=i-1;
       maintainence_pass[i]='\0';// user entered password value
   //  userenter[1]=0;// user entered password value
    // userenter[2]=0;// user entered password value
    // userenter[3]=0;// user entered password value
      
    // loginpass[i]='\0'; //to change password
     //reenteredpass[i]=0; //to change password
     //reenteredpass[i]=0; //to change password
     //reenteredpass[i]=0; //to change password
    // i=i-1;
     if(cursorxa<150)
     {
      cursorxa=150;
      i=0;
       maintainence_pass[i]='\0';
     }
     tft.graphicsMode();
     Serial.print("user=");
     Serial.print(loginuser[i]);
     Serial.print("\ni=");
     Serial.print(i);
  }
  
  
    //enter//
    if(tx>113 && tx<177 && ty>89 && ty<233)
    {
      tft.fillRect(140,50,220,25,RA8875_WHITE);
            tft.drawRect(140,50,220,25,RA8875_BLACK);
    cursorxa=150;
    i=0;
    i2=12;
    maintenanceenter=1;  
    
    Serial.print("ne");
    }
    while(tft.touched())
    {
      tft.touchRead(&txc, &tyc);
      txc=tyc=0;
      Serial.print("2");
      delay(100);
    }
  
}

void maintanance_menu()
{
  tft.textMode();
   /////admin screen==date,time..../////
//tft.fillScreen(0x3f79);  
tft.fillRoundRect(130, 50, 220, 30,10 ,0x3f79);//0XC618
tft.textSetCursor(140,55 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Serial.No.");

tft.fillRoundRect(130, 100, 220, 30,10 ,0x3f79);//0XC618
tft.textSetCursor(140,105 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Rst Admin Pwd");

tft.fillRoundRect(130, 150, 220, 30,10 ,0x3f79);//0XC618
tft.textSetCursor(140,155 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Balance Parameter");

  tft.fillRoundRect(130, 200, 220, 30,10 ,0x3f79);//0XC618
tft.textSetCursor(140,205 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Print Parameter");

   tft.fillRoundRect(9, 20, 45, 30,4 ,0x0a0f);//0XC618
  tft.textSetCursor(20,24 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("<--");

   /*tft.fillRoundRect(420, 20, 45, 30,4 ,0x0a0f);//0XC618
  tft.textSetCursor(431,24 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("-->");*/

  tft.graphicsMode();
}
void maintanance_menu_function()
{
  ///sr.no///
  if(tx>333 && tx<730 && ty>621 && ty<721)
  {
    sr_flag=1;
  }
  ///rst admn pwd////
   if(tx>336 && tx< 738 && ty>530 && ty<590)
  {
    rstpwd_flag=1;
  }
  ///bal parameter///
   if(tx>336 && tx<729 && ty>379 && ty<374)
  {
    balpara_flag=1;
  }
  ////print param////
   if(tx>359 && tx<734 && ty>229 && ty<293)
  {
    printpara_flag=1;
  }
  ////next///
  //if(tx>127 && tx<191 && ty>753 && ty<830)
  if(tx>864 && tx<977 && ty>757 && ty<892)
  {
    maintanancenext_flag=1;
  }
}

void maintanance_srno_menu()
{
  tft.textMode();

  tft.textSetCursor(150, 30);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0);
  tft.textWrite("Enter Serial No.");
///// FOR string2////// 
  tft.drawRect(140,50,180,25,RA8875_BLACK);

  tft.graphicsMode();
}

void maintanance_srno_function()
{
  //delay(400);
         // if(! digitalRead(RA8875_INT))
         // {
            
          //tft.touchRead(&tx, &ty);

if(cursorxa>350)
          {
            tft.fillRect(140,50,220,25,RA8875_WHITE);
            tft.drawRect(140,50,220,25,RA8875_BLACK);
            cursorxa=150;
            i=0;
            serial_no[i]='\0';
            clr_button[i]='\0';
            save_serial_no[i]='\0';
          }
  
          
   //1//
    if(tx>886 && tx<1000 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("1");
      serial_no[i]='1';// user entered password value 
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //2//
   if(tx>792 && tx<886 && ty>528 && ty<700)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("2");
     
      serial_no[i]='2';// user entered password value
      
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //3//
   if(tx>732 && tx<792 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("3");
      
      serial_no[i]='3';// user entered password value
      
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //4//
    if(tx>641 && tx<732 && ty>528 && ty<700)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("4");
      
      serial_no[i]='4';// user entered password value 
      reenteredpass[i]=4; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //5//
    if(tx>554 && tx<641 && ty>528 && ty<700)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("5");
     
      serial_no[i]='5';// user entered password value
      reenteredpass[i]=5; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //6//
     if(tx>467 && tx<554 && ty>528 && ty<700)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("6");
      
      serial_no[i]='6';// user entered password value
      reenteredpass[i]=6; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //7//
   if(tx>395 && tx<467 && ty>528 && ty<700)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("7");
     
     serial_no[i]='7';// user entered password value
      reenteredpass[i]=7; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //8//
    if(tx>308 && tx<395 && ty>528 && ty<700)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("8");
      
      serial_no[i]='8';// user entered password value
      reenteredpass[i]=8; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //9//
    if(tx>238 && tx<308 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("9");
     
      serial_no[i]='9';// user entered password value
      reenteredpass[i]=9; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //0//
   if(tx>138 && tx<238 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_BLACK);
    // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("0");
     
      serial_no[i]='0';// user entered password value
      reenteredpass[i]=0; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
      Serial.print("ipf0");
    }

////abcd////
if(key==1)
{
//q//
    if(tx>873 && tx<977 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("q");
      
  
      serial_no[i]='q';// user entered password value 
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //w//
     if(tx>787 && tx<872 && ty>374 && ty<528)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("w");
     
      serial_no[i]='w';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //e//
     if(tx>705 && tx<786 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("e");
     
      serial_no[i]='e';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //r//
    if(tx>614 && tx<704 && ty>374 && ty<528)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("r");
      
      serial_no[i]='r';// user entered password value 
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //t//
     if(tx>524 && tx<613 && ty>374 && ty<528)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("t");
     
      serial_no[i]='t';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //y//
     if(tx>437 && tx<523 && ty>374 && ty<528)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("y");
     
      serial_no[i]='y';// user entered password value
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //u//
     if(tx>368 && tx<436 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("u");
     
      serial_no[i]='u';// user entered password value
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //i//
    if(tx>263 && tx<368 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("i");
     
      serial_no[i]='i';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //o//
    if(tx>178 && tx<262 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("o");
     
     serial_no[i]='o';// user entered password value
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//p//
     if(tx>113 && tx<177 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("p");
     
  
      serial_no[i]='p';// user entered password value 
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //a//
     if(tx>846 && tx<1000 && ty>233 && ty<374)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("a");
     
      serial_no[i]='a';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //s//
    if(tx>792 && tx<846 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("s");
      
      serial_no[i]='s';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //d//
    if(tx>732 && tx<792 && ty>233 && ty<374)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("d");
      
      serial_no[i]='d';// user entered password value 
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //f//
   if(tx>641 && tx<732 && ty>233 && ty<374)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("f");
     
     serial_no[i]='f';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //g//
   if(tx>554 && tx<641 && ty>233 && ty<374)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("g");
     
      serial_no[i]='g';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //h//
    if(tx>467 && tx<554 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("h");
      
     serial_no[i]='h';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //j//
    if(tx>395 && tx<467 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("j");
     
      serial_no[i]='j';// user entered password value
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //k//
     if(tx>308 && tx<395 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("k");
      
      serial_no[i]='k';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//l//
    if(tx>238 && tx<308 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("l");
     
  
      serial_no[i]='l';// user entered password value 
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //z//
    if(tx>138 && tx<238 && ty>233 && ty<374)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("z");
      
      serial_no[i]='z';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //x//
    if(tx>787 && tx<872 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("x");
      
      serial_no[i]='x';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //c//
     if(tx>704 && tx<787 && ty>89 && ty<233)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("c");
     
      serial_no[i]='c';// user entered password value 
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //v//
   if(tx>614 && tx<704 && ty>89 && ty<233)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("v");
     
      serial_no[i]='v';// user entered password value
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //b//
     if(tx>437 && tx<523 && ty>89 && ty<233)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("b");
      
      serial_no[i]='b';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //space//
    if(tx>524 && tx<613 && ty>89 && ty<233)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      
      serial_no[i]=' ';// user entered password value
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //n//
    if(tx>368 && tx<436 && ty>89 && ty<233)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("n");
     
      serial_no[i]='n';// user entered password value
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //m//
     if(tx>263 && tx<367 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("m");
     
      serial_no[i]='m';// user entered password value
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
}

///ABCD////
if(key==2)
{
  //Q//

     if(tx>873 && tx<977 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Q");
      
  
     serial_no[i]='Q';// user entered password value 
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //W//
    if(tx>787 && tx<872 && ty>374 && ty<528)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("W");
     
      serial_no[i]='W';// user entered password valu
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //E//
    if(tx>705 && tx<786 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("E");
     
      serial_no[i]='E';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //R//
    if(tx>614 && tx<704 && ty>374 && ty<528)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("R");
      
      serial_no[i]='R';// user entered password value 
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //T//
   if(tx>524 && tx<613 && ty>374 && ty<528)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("T");
     
      serial_no[i]='T';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //Y//
    if(tx>437 && tx<523 && ty>374 && ty<528)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Y");
     
     serial_no[i]='Y';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //U//
     if(tx>368 && tx<436 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("U");
     
      serial_no[i]='U';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //I//
   if(tx>263 && tx<367 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("I");
     
      serial_no[i]='I';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //O//
     if(tx>178 && tx<262 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("O");
     
    serial_no[i]='O';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//P//
    if(tx>113 && tx<177 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("P");
     
  
      serial_no[i]='P';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //A//
   if(tx>846 && tx<1000 && ty>233 && ty<374)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("A");
     
     serial_no[i]='A';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //S//
     if(tx>792 && tx<846 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("S");
      
     serial_no[i]='S';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //D//
    if(tx>732 && tx<792 && ty>233 && ty<374)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("D");
      
      serial_no[i]='D';// user entered password value 
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //F//
    if(tx>641 && tx<732 && ty>233 && ty<374)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("F");
     
     serial_no[i]='F';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //G//
   if(tx>554 && tx<641 && ty>233 && ty<374)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("G");
     
      serial_no[i]='G';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //H//
    if(tx>467 && tx<554 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("H");
      
     serial_no[i]='H';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //J//
    if(tx>395 && tx<467 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("J");
     
     serial_no[i]='J';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //K//
    if(tx>308 && tx<395 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("K");
      
     serial_no[i]='K';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//L//
     if(tx>238 && tx<308 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("L");
     
  
     serial_no[i]='L';// user entered password value 
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //Z//
    if(tx>138 && tx<238 && ty>233 && ty<374)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Z");
      
      serial_no[i]='Z';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //X//
    if(tx>787 && tx<872 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("X");
      
      serial_no[i]='X';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //C//
    if(tx>704 && tx<787 && ty>89 && ty<233)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("C");
     
      serial_no[i]='C';// user entered password value 
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //V//
    if(tx>614 && tx<704 && ty>89 && ty<233)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("V");
     
     serial_no[i]='V';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //B//
    if(tx>437 && tx<523 && ty>89 && ty<233)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("B");
      
      serial_no[i]='B';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //space//
    if(tx>524 && tx<613 && ty>89 && ty<233)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      
      serial_no[i]=' ';// user entered password value
      reenteredpass[i]=7; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //N//
    if(tx>368 && tx<436 && ty>89 && ty<233)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("N");
     
      serial_no[i]='N';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //M//
     if(tx>263 && tx<367 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("M");
     
     serial_no[i]='M';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
}

    
///capslock//
    if(tx>178 && tx<262 && ty>89 && ty<233)
    {
  
      capslock=1;
    }

    ////<--////
    if(tx>864 && tx<977 && ty>757 && ty<892)
    {
   
      cursorxa=150;
      i=0;
      //next_flag=1;
       serialnonext_flag=1;
      clr_button[0]='\0';
      serial_no[0]='\0';
      save_serial_no[0]='\0';
     key=1; 
    }

   /* ////-->////       ////////////   ////////////   ///////////
    if(tx>143 && tx<179 && ty>763 && ty<818)
    {
   
      cursorxa=150;
      i=0;
      serialnonext_flag=1;
      clr_button[0]='\0';
      serial_no[0]='\0';
      save_serial_no[0]='\0';
     key=1; 
      
    }
    */
     ///clear////
  if(tx>873 && tx<1000 && ty>89 && ty<233)
     {
      
    // tft.touchEnable(true);
     //tft.fillRect(57, 120, 150, 58, RA8875_RED);
     tft.textMode();
   //  tft.cursorBlink(32);
     cursorxa=cursorxa-10;
     tft.textSetCursor(cursorxa,cursorya); ///text location////
     tft.textTransparent(RA8875_WHITE);
     tft.textEnlarge(0);  ////text size////
    // tft.textSetCursor(cursorxa, cursorya); ///text location////
      i=i-1;
      clr_button[0]=serial_no[i];
    clr_button[1]='\0';
    tft.textWrite(clr_button);
      serial_no[i]='\0';// user entered password value

     
     if(cursorxa<150)
     {
      cursorxa=150;
      i=0;
      serial_no[0]='\0';
      save_serial_no[0]='\0';
     }
     tft.graphicsMode();
     Serial.print(userenter[i]);
     Serial.print(i);
  }
          
     //SAVE//

     if(tx>113 && tx<177 && ty>89 && ty<233)
    {
    cursorxa=150;
    i=0;
    i2=12;
    for(j=0;serial_no[j]!='\0';j++)
    {
    save_serial_no[j]=serial_no[j];
    EEPROM.write(j+2061, save_serial_no[j]);
    }
     if(serial_no[j]=='\0')
      {
        EEPROM.write(j+2061,'\0');
      }
    Serial.print("ne");
    for(j=0;j<=14;j++)
    {
    serial_no[j]='\0';
    }
    j=0;
    clr_button[0]='\0';
     tft.fillRect(140,50,220,25,RA8875_WHITE);
            tft.drawRect(140,50,220,25,RA8875_BLACK);
    }
     while(tft.touched())
    {
      tft.touchRead(&txc, &tyc);
      txc=tyc=0;
      Serial.print("2");
      delay(100);
    }
  // }
}
void maintanance_rstpass_menu_function()
{
  tft.fillScreen(RA8875_WHITE);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(150, 120); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("password resetting is in process");
      delay(2000);
  ///admin///
 for(i=30;admin[i-30]!='\0' && i<49;i++)
  {
EEPROM.write(i,admin[i-30]);
  }
EEPROM.write(i,'\0');
  ///admin pass///
  for(i=50;adminpassword[i-50]!='\0' && i<59;i++)
  {
EEPROM.write(i,adminpassword[i-50]);
//Serial.print(defaultpass[i]);
  }
EEPROM.write(i,'\0');
  ///user1///
 for(i=60;user1[i-60]!='\0' && i<79;i++)
  {
EEPROM.write(i,user1[i-60]);
  }
EEPROM.write(i,'\0');
  ///pass1///
  for(i=80;userpassword1[i-80]!='\0' && i<89;i++)
  {
EEPROM.write(i,userpassword1[i-80]);
//Serial.print(defaultpass[i]);
  }
EEPROM.write(i,'\0');
  ///username2///
 for(i=90;user2[i-90]!='\0' && i<109;i++)
  {
EEPROM.write(i,user2[i-90]);
  }
EEPROM.write(i,'\0');
  ///pass2///
  for(i=110;userpassword2[i-110]!='\0' && i<119;i++)
  {
EEPROM.write(i,userpassword2[i-110]);
//Serial.print(defaultpass[i]);
  }
EEPROM.write(i,'\0');
  ///username3///
 for(i=120;user3[i-120]!='\0' && i<129;i++)
  {
EEPROM.write(i,user3[i-120]);
  }
EEPROM.write(i,'\0');
  ///pass3///
  for(i=130;userpassword3[i-130]!='\0' && i<139;i++)
  {
EEPROM.write(i,userpassword3[i-130]);
//Serial.print(defaultpass[i]);
  }
EEPROM.write(i,'\0');
  ///username4///
 for(i=140;user4[i-140]!='\0' && i<159;i++)
  {
EEPROM.write(i,user4[i-140]);
  }
EEPROM.write(i,'\0');
  ///pass4///
  for(i=160;userpassword4[i-160]!='\0' && i<169;i++)
  {
EEPROM.write(i,userpassword4[i-160]);
//Serial.print(defaultpass[i]);
  }
EEPROM.write(i,'\0');
  ///username5///
 for(i=170;user5[i-170]!='\0' && i<189;i++)
  {
EEPROM.write(i,user5[i-170]);
  }
EEPROM.write(i,'\0');
  ///pass5///
  for(i=190;userpassword5[i-190]!='\0' && i<199;i++)
  {
EEPROM.write(i,userpassword5[i-190]);
//Serial.print(defaultpass[i]);

    tft.fillScreen(RA8875_WHITE);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(170, 120); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("password resetting is done");
      tft.graphicsMode();
      delay(900);
  }
EEPROM.write(i,'\0');  
i=0;
}
/*
void maintanance_rstpass_function()
{
  resetpass_flag=1;///write in eeprom
  for(j=0;j<=3;j++)
  {
    EEPROM.write(add, j+1);
  }
  
}*/
void maintanance_printparam_menu()
{
  
}
void maintanance_printparam_function()
{
  
}

void paratobe_print_menu()
{
  tft.textMode();
   /////admin screen==date,time..../////
//tft.fillScreen(0x3f79);  
tft.fillRoundRect(130, 50, 220, 30,10 ,0x3f79);//0XC618
tft.textSetCursor(140,55 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Product Name");

tft.fillRoundRect(130, 100, 220, 30,10 ,0x3f79);//0XC618
tft.textSetCursor(140,105 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Batch Number");

/*tft.fillRoundRect(130, 150, 220, 30,10 ,0x3f79);//0XC618
tft.textSetCursor(140,155 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Lot Number");*/

  tft.fillRect(130, 200, 220, 30 ,RA8875_GREEN);//0XC618
  tft.drawRect(130, 200, 220, 30 ,RA8875_BLACK);//0XC618
tft.textSetCursor(140,205 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("   Save  ");

   tft.fillRoundRect(420, 20, 45, 30,4 ,0x0a0f);//0XC618
  tft.textSetCursor(431,24 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("-->");

  tft.graphicsMode();
}

void paratobe_print_function()
{
  ///product name/// 
   if(tx>341 && tx<727 && ty>676 && ty<725)
  {
    productname_flag=1;
  }
  ///batch no.///
  if(tx>339 && tx<727 && ty>524 && ty<580)
  {
    batchno_flag=1;
  }
  ////lot no.///////
 /* if(tx>353 && tx<730 && ty>370 && ty<435)
  {
    lotno_flag=1;
  }*/
  if(tx>345 && tx<742 && ty>237 && ty<300)
  {
    /*for(i=0;i<=4;i++)
    {
    product_name[i]=' ';
    save_lot_number[i]=product_name[i];
    }
    product_name[0]='\0';*/
     tft.textMode();
     tft.textSetCursor(2, 2); ///text location////
      tft.textTransparent(RA8875_RED);
     tft.textEnlarge(0);  ////text size////
      tft.textWrite("Successfully Stored");
    tft.graphicsMode();
    resultstore_flag=1;
    historypress=1+historypress;
    Serial.print("upq");
  }
  ///next///
  if(tx>113 && tx<211 && ty>756 && ty<872)
  {
    paratobeprintnext_flag=1;
    //timecountw1
    for(i=0;i<=4;i++)
      {
      weight_value[i]=hisweight1_value[i]=weight_value[i]=hisweight2_value[i]=0;
      }
      total_weight1=perw1=total_weight2=perw2=total_outweight1=peroutw1=total_outweight2=peroutw2=0;
     
  }
}

void lot_number_menu()
{
  tft.textMode();

  tft.textSetCursor(150, 30);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0);
  tft.textWrite("Enter Lot Nummber");
///// FOR string2////// 
  tft.drawRect(140,50,220,25,RA8875_BLACK);

  tft.graphicsMode();
}

void batch_number_menu()
{
  tft.textMode();

  tft.textSetCursor(150, 30);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0);
  tft.textWrite("Enter Batch Number");
///// FOR string2////// 
  tft.drawRect(140,50,220,25,RA8875_BLACK);

  tft.graphicsMode();
}

void product_name_menu()
{
  tft.textMode();

  tft.textSetCursor(150,30);
   tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(0);
  tft.textWrite("Enter Product name");
///// FOR string2////// 
  tft.drawRect(140,50,220,25,RA8875_BLACK);

  tft.graphicsMode();
}

void product_name_function()
{

 if(cursorxa>350)
          {
            tft.fillRect(140,50,220,25,RA8875_WHITE);
            tft.drawRect(140,50,220,25,RA8875_BLACK);
            cursorxa=150;
            i=0;
            clr_button[i]='\0';
            product_name[i]='\0';
          }
  
  //1//
    if(tx>886 && tx<1000 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("1");
      product_name[i]='1';// user entered password value 
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //2//
     if(tx>792 && tx<886 && ty>528 && ty<700)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("2");
     
      product_name[i]='2';// user entered password value
      
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //3//
    if(tx>732 && tx<792 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("3");
      
      product_name[i]='3';// user entered password value
      
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //4//
    if(tx>641 && tx<732 && ty>528 && ty<700)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("4");
      
      product_name[i]='4';// user entered password value 
      reenteredpass[i]=4; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //5//
     if(tx>554 && tx<641 && ty>528 && ty<700)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("5");
     
      product_name[i]='5';// user entered password value
      reenteredpass[i]=5; //to change password
      Serial.print(userenter[i]);
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //6//
    if(tx>467 && tx<554 && ty>528 && ty<700)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("6");
      
      product_name[i]='6';// user entered password value
      reenteredpass[i]=6; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //7//
    if(tx>395 && tx<467 && ty>528 && ty<700)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("7");
     
     product_name[i]='7';// user entered password value
      reenteredpass[i]=7; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //8//
    if(tx>308 && tx<395 && ty>528 && ty<700)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("8");
      
      product_name[i]='8';// user entered password value
      reenteredpass[i]=8; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //9//
    if(tx>238 && tx<308 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("9");
     
      product_name[i]='9';// user entered password value
      reenteredpass[i]=9; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //0//
    if(tx>138 && tx<238 && ty>528 && ty<700)
    {
   // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_BLACK);
    // tft.fillRoundRect(290, 218, 50, 50, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
     // tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("0");
     
      product_name[i]='0';// user entered password value
      reenteredpass[i]=0; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
      Serial.print("ipf0");
    }

////abcd////

if(key==1)
{
//q//
    if(tx>873 && tx<977 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("q");
      
  
      product_name[i]='q';// user entered password value 
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //w//
     if(tx>787 && tx<872 && ty>374 && ty<528)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("w");
     
      product_name[i]='w';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //e//
    if(tx>705 && tx<786 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("e");
     
      product_name[i]='e';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //r//
    if(tx>614 && tx<704 && ty>374 && ty<528)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("r");
      
      product_name[i]='r';// user entered password value 
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //t//
    if(tx>524 && tx<613 && ty>374 && ty<528)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("t");
     
      product_name[i]='t';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //y//
    if(tx>437 && tx<523 && ty>374 && ty<528)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("y");
     
      product_name[i]='y';// user entered password value
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //u//
    if(tx>368 && tx<436 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("u");
     
      product_name[i]='u';// user entered password value
      
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //i//
     if(tx>263 && tx<368 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("i");
     
      product_name[i]='i';// user entered password value
   
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //o//
     if(tx>178 && tx<262 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("o");
     
     product_name[i]='o';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//p//
    if(tx>113 && tx<177 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("p");
     
  
      product_name[i]='p';// user entered password value 
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //a//
    if(tx>846 && tx<1000 && ty>233 && ty<374)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("a");
     
      product_name[i]='a';// user entered password value
      
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //s//
     if(tx>792 && tx<846 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("s");
      
      product_name[i]='s';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //d//
    if(tx>732 && tx<792 && ty>233 && ty<374)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("d");
      
      product_name[i]='d';// user entered password value 
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //f//
     if(tx>641 && tx<732 && ty>233 && ty<374)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("f");
     
     product_name[i]='f';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //g//
     if(tx>554 && tx<641 && ty>233 && ty<374)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("g");
     
      product_name[i]='g';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //h//
    if(tx>467 && tx<554 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("h");
      
     product_name[i]='h';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //j//
     if(tx>395 && tx<467 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("j");
     
      product_name[i]='j';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //k//
    if(tx>308 && tx<395 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("k");
      
      product_name[i]='k';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//l//
    if(tx>238 && tx<308 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("l");
     
  
      product_name[i]='l';// user entered password value 
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //z//
    if(tx>138 && tx<238 && ty>233 && ty<374)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("z");
      
      product_name[i]='z';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //x//
     if(tx>787 && tx<872 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("x");
      
      product_name[i]='x';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //c//
     if(tx>704 && tx<787 && ty>89 && ty<233)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("c");
     
      product_name[i]='c';// user entered password value 
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //v//
    if(tx>614 && tx<704 && ty>89 && ty<233)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("v");
     
      product_name[i]='v';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //b//
    if(tx>437 && tx<523 && ty>89 && ty<233)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("b");
      
      product_name[i]='b';// user entered password value
    
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //space//
    if(tx>524 && tx<613 && ty>89 && ty<233)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      
      product_name[i]=' ';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //n//
    if(tx>368 && tx<436 && ty>89 && ty<233)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("n");
     
      product_name[i]='n';// user entered password value
      
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //m//
    if(tx>263 && tx<367 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("m");
     
      product_name[i]='m';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
}

    ///ABCD////
if(key==2)
{
  //Q//

     if(tx>873 && tx<977 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Q");
      
  
      product_name[i]='Q';// user entered password value 
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //W//
     if(tx>787 && tx<872 && ty>374 && ty<528)
    { 
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("W");
     
       product_name[i]='W';// user entered password valu
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //E//
    if(tx>705 && tx<786 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("E");
     
       product_name[i]='E';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //R//
    if(tx>614 && tx<704 && ty>374 && ty<528)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("R");
      
       product_name[i]='R';// user entered password value 
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //T//
    if(tx>524 && tx<613 && ty>374 && ty<528)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("T");
     
      product_name[i]='T';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //Y//
    if(tx>437 && tx<523 && ty>374 && ty<528)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Y");
     
       product_name[i]='Y';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //U//
   if(tx>368 && tx<436 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("U");
     
      product_name[i]='U';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //I//
    if(tx>263 && tx<367 && ty>374 && ty<528)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("I");
     
       product_name[i]='I';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //O//
    if(tx>178 && tx<262 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("O");
     
     product_name[i]='O';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//P//
    if(tx>113 && tx<177 && ty>374 && ty<528)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("P");
     
  
       product_name[i]='P';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //A//
    if(tx>846 && tx<1000 && ty>233 && ty<374)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("A");
     
       product_name[i]='A';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //S//
     if(tx>792 && tx<846 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("S");
      
      product_name[i]='S';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //D//
   if(tx>732 && tx<792 && ty>233 && ty<374)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("D");
      
       product_name[i]='D';// user entered password value 
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //F//
    if(tx>641 && tx<732 && ty>233 && ty<374)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("F");
     
      product_name[i]='F';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //G//
    if(tx>554 && tx<641 && ty>233 && ty<374)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("G");
     
      product_name[i]='G';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //H//
     if(tx>467 && tx<554 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("H");
      
    product_name[i]='H';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //J//
    if(tx>395 && tx<467 && ty>233 && ty<374)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("J");
     
       product_name[i]='J';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //K//
    if(tx>308 && tx<395 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("K");
      
      product_name[i]='K';// user entered password value
     
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
//L//
    if(tx>238 && tx<308 && ty>233 && ty<374)
    {
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_BLACK);
   // delay(3);
   // tft.fillRoundRect(290, 04, 50, 58, 10, RA8875_RED);
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("L");
     
  
      product_name[i]='L';// user entered password value 
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
   
    //Z//
   if(tx>138 && tx<238 && ty>233 && ty<374)
    {
      tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("Z");
      
       product_name[i]='Z';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

      
    //X//
   if(tx>787 && tx<872 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_BLACK);
   // tft.fillRoundRect(420, 04, 50, 58, 10, RA8875_RED);
    //tft.textTransparent(RA8875_GREEN);
      tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("X");
      
       product_name[i]='X';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //C//
    if(tx>704 && tx<787 && ty>89 && ty<233)
    {
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_BLACK);
     //tft.fillRoundRect(290, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
       tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("C");
     
      product_name[i]='C';// user entered password value 
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //V//
     if(tx>614 && tx<704 && ty>89 && ty<233)
    {
  //   tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 78, 50, 58, 10, RA8875_RED);
     //tft.textTransparent(RA8875_GREEN);
     tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("V");
     
      product_name[i]='V';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //B//
    if(tx>437 && tx<523 && ty>89 && ty<233)
    {
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_BLACK);
     // tft.fillRoundRect(420, 78, 50, 58, 10, RA8875_RED);
     // tft.textTransparent(RA8875_GREEN);
     tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("B");
      
      product_name[i]='B';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //space//
    if(tx>524 && tx<613 && ty>89 && ty<233)
    {
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_BLACK);
   //  tft.fillRoundRect(290, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(" ");
      
      product_name[i]=' ';// user entered password value
      reenteredpass[i]=7; //to change password
      cursorxa=cursorxa+10;
      tft.graphicsMode();
     i++;
    }

    //N//
    if(tx>368 && tx<436 && ty>89 && ty<233)
    {
   //  tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(355, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
    //  tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("N");
     
       product_name[i]='N';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }

    //M//
     if(tx>263 && tx<367 && ty>89 && ty<233)
    {
   // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_BLACK);
    // tft.fillRoundRect(420, 148, 50, 58, 10, RA8875_RED);
    // tft.textTransparent(RA8875_GREEN);
    tft.textMode();
   //   tft.cursorBlink(32);
      tft.textSetCursor(cursorxa, cursorya); ///text location////
      tft.textTransparent(RA8875_BLACK);
  
      tft.textEnlarge(0);  ////text size////
      tft.textWrite("M");
     
       product_name[i]='M';// user entered password value
      cursorxa=cursorxa+10;
      tft.graphicsMode();
      i++;
    }
}
    
///capslock//
     if(tx>178 && tx<262 && ty>89 && ty<233)
    {
  
      capslock=1;
    }

    ////<--////
    if(tx>864 && tx<977 && ty>757 && ty<892)
    {
for(i=0;i<=9;i++)
{
   product_name[i]='\0';
}
      cursorxa=150;
      i=0;
      productback_flag=1;
      clr_button[0]='\0';
      key=1;
    }

 /*   ////-->////
    if(tx>143 && tx<179 && ty>763 && ty<818)
    {
   
      cursorxa=150;
      i=0;
      serialnonext_flag=1;
    }*/
    
     ///clear////
  if(tx>873 && tx<1000 && ty>89 && ty<233)
     {
      
    // tft.touchEnable(true);
     //tft.fillRect(57, 120, 150, 58, RA8875_RED);
     tft.textMode();
   //  tft.cursorBlink(32);
     cursorxa=cursorxa-10;
     tft.textSetCursor(cursorxa,cursorya); ///text location////
     tft.textTransparent(RA8875_WHITE);
     tft.textEnlarge(0);  ////text size////
    // product_name[i]='\0';
    i=i-1;
    clr_button[0]=product_name[i];
    clr_button[1]='\0';
     tft.textWrite(clr_button);
     product_name[i]='\0';
     
     
    // tft.textSetCursor(cursorxa, cursorya); ///text location////
     // i=i+1;
     // product_name[i]='\0';// user entered password value
   //  userenter[1]=0;// user entered password value
    // userenter[2]=0;// user entered password value
    // userenter[3]=0;// user entered password value

     for(k=0;product_name[k]!='\0';k++)
     {
      // Serial.print("i\n");
       Serial.print(k);
       Serial.print("i=\n");
      Serial.print( product_name[k]);
      Serial.print("\n");
     }
     if(cursorxa<150)
     {
      cursorxa=150;
      i=0;
      product_name[i]='\0';
     }
     tft.graphicsMode();
    // Serial.print(userenter[i]);
    // Serial.print(i);
  }
          
     //SAVE//

     if(tx>113 && tx<177 && ty>89 && ty<233)
    {
      tft.fillRect(140,50,220,25,RA8875_WHITE);
      tft.drawRect(140,50,220,25,RA8875_BLACK);
       for(i=i;i<=9;i++)
       {
         product_name[i]=' ';    
       }
    cursorxa=150;
    i=0;
    i2=12;
    productenter_flag=1;
    historypress=0;
    /*
    for(j=0;product_name[j]!='\0';j++)
    {
    save_product_name[j]=product_name[j];
    }*/
    Serial.print("ne");
  //  j=0;
    }
   while(tft.touched())
    {
      tft.touchRead(&txc, &tyc);
      txc=tyc=0;
      Serial.print("2");
      delay(100);
    } 
}

//////////////////////HISTORY////////////////////

void history1_menu()
{
  tft.textMode();
//  tft.fillScreen(0X379f);
  //tft.fillRect(20, 30, 50,30,RA8875_RED);

   tft.fillRoundRect(9, 20, 45, 30,4 ,0x0a0f);//0XC618
  tft.textSetCursor(20,24 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("<--");

  tft.fillRoundRect(420, 20, 45, 30,4 ,0x0a0f);//0XC618
  tft.textSetCursor(431,24 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("-->");

 /* tft.fillRoundRect(420, 60, 45, 30,3 ,0x0a0f);//0XC618
  tft.textSetCursor(424,68 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("ACLR");

  tft.fillRoundRect(420, 100, 45, 30,3 ,0x0a0f);//0XC618
  tft.textSetCursor(424,108 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("ENTER");
*/
tft.textSetCursor(10,60 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(1); ////text size////
  tft.textWrite("0");
  tft.textSetCursor(10,100 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(1); ////text size////
  tft.textWrite("1");
  tft.textSetCursor(10,140 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(1); ////text size////
  tft.textWrite("2");
  tft.textSetCursor(10,180 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(1); ////text size////
  tft.textWrite("3");
  tft.textSetCursor(10,220 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(1); ////text size////
  tft.textWrite("4");
  
  tft.fillRect(30,60,300,30,0xffff);
  tft.drawRect(28,58,302,32,0x0000);
  
  tft.fillRect(30,100,300,30,0xffff);
  tft.drawRect(28,98,302,32,0x0000);
  
  tft.fillRect(30,140,300,30,0xffff);
  tft.drawRect(28,138,302,32,0x0000);
  
  tft.fillRect(30,180,300,30,0xffff);
  tft.drawRect(28,178,302,32,0x0000);
  
  tft.fillRect(30,220,300,30,0xffff);
  tft.drawRect(28,218,302,32,0x0000);
  

  tft.fillRect(350,60,50,30,RA8875_GREEN);
  tft.textSetCursor(355,65 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Print");
  tft.fillRect(410,60,60,30,RA8875_RED);
  tft.textSetCursor(415,65 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Delete");
  
  tft.fillRect(350,100,50,30,RA8875_GREEN);
  tft.textSetCursor(355,105 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Print");
  tft.fillRect(410,100,60,30,RA8875_RED);
  tft.textSetCursor(415,105 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Delete");
  
  tft.fillRect(350,140,50,30,RA8875_GREEN);
  tft.textSetCursor(355,145 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Print");
  tft.fillRect(410,140,60,30,RA8875_RED);
  tft.textSetCursor(415,145 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Delete");
  
  tft.fillRect(350,180,50,30,RA8875_GREEN);
  tft.textSetCursor(355,185 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Print");
  tft.fillRect(410,180,60,30,RA8875_RED);
  tft.textSetCursor(415,185 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Delete");
  
  tft.fillRect(350,220,50,30,RA8875_GREEN);
  tft.textSetCursor(355,225 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Print");
  tft.fillRect(410,220,60,30,RA8875_RED);
  tft.textSetCursor(415,225 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Delete");

  tft.graphicsMode();
}
void history_function()
{
  ////<--////
    if(tx>864 && tx<977 && ty>757 && ty<892)
    {
   
     // cursorxa=150;
      i=0;
      historyback_flag=1;
    }
     ////-->////
    if(tx>113 && tx<211 && ty>756 && ty<872)
    {

      historynext_flag=1;
      historypage=2;
      //cursorxa=150;
      i=0;
    }
    //////delete history0////  
   if(tx>110 && tx<219 && ty>646 && ty<756)
    {
      Serial.print("d0");
       tft.fillRect(30,60,300,30,0xffff);
  tft.drawRect(28,58,302,32,0x0000);
  
  for(i=250;i<340;i++)
  {
   EEPROM.write(i,'\0');
  }
    }
     //////delete history1////  
    if(tx>110 && tx<219 && ty>506 && ty<645)
    {
      Serial.print("d1");
      tft.fillRect(30,100,300,30,0xffff);
  tft.drawRect(28,98,302,32,0x0000);
 for(i=340;i<430;i++)
  {
   EEPROM.write(i,'\0');
  }
    }
     //////delete history2////  
    if(tx>110 && tx<219 && ty>384 && ty<505)
    {
      Serial.print("d2");
      tft.fillRect(30,140,300,30,0xffff);
  tft.drawRect(28,138,302,32,0x0000);
  for(i=430;i<520;i++)
  {
  EEPROM.write(i,'\0');
  }
    }
     //////delete history3////  
    if(tx>110 && tx<219 && ty>255 && ty<383)
    {
      Serial.print("d3");
      tft.fillRect(30,180,300,30,0xffff);
  tft.drawRect(28,178,302,32,0x0000);
  for(i=520;i<610;i++)
  {
  EEPROM.write(i,'\0');
  }
    }
     //////delete history4////  
    if(tx>110 && tx<219 && ty>121 && ty<245)
    {
      Serial.print("d4");
      tft.fillRect(30,220,300,30,0xffff);
  tft.drawRect(28,218,302,32,0x0000);
  for(i=610;i<700;i++)
  {
  EEPROM.write(i,'\0');
  }
    }
    
}
void history2_menu()
{
  tft.textMode();
   tft.fillRoundRect(9, 20, 45, 30,4 ,0x0a0f);//0XC618
  tft.textSetCursor(20,24 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("<--");
  
//tft.fillRoundRect(9, 60, 45, 30,4 ,0x0a0f);//0XC618
  tft.textSetCursor(10,60 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(1); ////text size////
  tft.textWrite("5");
  tft.textSetCursor(10,100 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(1); ////text size////
  tft.textWrite("6");
  tft.textSetCursor(10,140 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(1); ////text size////
  tft.textWrite("7");
  tft.textSetCursor(10,180 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(1); ////text size////
  tft.textWrite("8");
  tft.textSetCursor(05,220 ); ///text location////
  tft.textTransparent(RA8875_BLACK);//0X94B2
  tft.textEnlarge(1); ////text size////
  tft.textWrite("9");
  
  tft.fillRect(30,60,300,30,0xffff);
  tft.drawRect(28,58,302,32,0x0000);
  
  tft.fillRect(30,100,300,30,0xffff);
  tft.drawRect(28,98,302,32,0x0000);
  
  tft.fillRect(30,140,300,30,0xffff);
  tft.drawRect(28,138,302,32,0x0000);
  
  tft.fillRect(30,180,300,30,0xffff);
  tft.drawRect(28,178,302,32,0x0000);
  
  tft.fillRect(30,220,300,30,0xffff);
  tft.drawRect(28,218,302,32,0x0000);
  

  tft.fillRect(350,60,50,30,RA8875_GREEN);
  tft.textSetCursor(355,65 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Print");
  tft.fillRect(410,60,60,30,RA8875_RED);
  tft.textSetCursor(415,65 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Delete");
  
  tft.fillRect(350,100,50,30,RA8875_GREEN);
  tft.textSetCursor(355,105 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Print");
  tft.fillRect(410,100,60,30,RA8875_RED);
  tft.textSetCursor(415,105 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Delete");
  
  tft.fillRect(350,140,50,30,RA8875_GREEN);
  tft.textSetCursor(355,145 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Print");
  tft.fillRect(410,140,60,30,RA8875_RED);
  tft.textSetCursor(415,145 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Delete");
  
  tft.fillRect(350,180,50,30,RA8875_GREEN);
  tft.textSetCursor(355,185 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Print");
  tft.fillRect(410,180,60,30,RA8875_RED);
  tft.textSetCursor(415,185 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Delete");
  
  tft.fillRect(350,220,50,30,RA8875_GREEN);
  tft.textSetCursor(355,225 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Print");
  tft.fillRect(410,220,60,30,RA8875_RED);
  tft.textSetCursor(415,225 ); ///text location////
  tft.textTransparent(RA8875_WHITE);//0X94B2
  tft.textEnlarge(0); ////text size////
  tft.textWrite("Delete");

  

   tft.graphicsMode();
}

void history2_function()
{
  ////<--////
    if(tx>864 && tx<977 && ty>757 && ty<892)
    {
   
     // cursorxa=150;
     // i=0;
      historyback2_flag=1;
      historypage=0;
    }

   //////delete history5////  
   if(tx>110 && tx<219 && ty>646 && ty<756)
    {
       tft.fillRect(30,60,300,30,0xffff);
  tft.drawRect(28,58,302,32,0x0000);
  for(i=700;i<790;i++)
  {
   EEPROM.write(i,'\0');
  }
    }
     //////delete history6////  
    if(tx>110 && tx<219 && ty>506 && ty<645)
    {
      tft.fillRect(30,100,300,30,0xffff);
  tft.drawRect(28,98,302,32,0x0000);
  for(i=790;i<880;i++)
  {
  EEPROM.write(i,'\0');
  }
    }
     //////delete history7////  
    if(tx>110 && tx<219 && ty>384 && ty<505)
    {
      tft.fillRect(30,140,300,30,0xffff);
  tft.drawRect(28,138,302,32,0x0000);
  for(i=880;i<970;i++)
  {
  EEPROM.write(i,'\0');
  }
    }
     //////delete history8////  
    if(tx>110 && tx<219 && ty>255 && ty<383)
    {
      tft.fillRect(30,180,300,30,0xffff);
  tft.drawRect(28,178,302,32,0x0000);
  for(i=970;i<1060;i++)
  {
  EEPROM.write(i,'\0');
  }
    }
     //////delete history9////  
    if(tx>110 && tx<219 && ty>121 && ty<245)
    {
      tft.fillRect(30,220,300,30,0xffff);
  tft.drawRect(28,218,302,32,0x0000);
  for(i=1060;i<1150;i++)
  {
  EEPROM.write(i,'\0');
  }
    } 
}

void historyprint_function()
{
  
}

void historyshow_menu()
{
   if(EEPROM.read(250)!='\0')
      {
        for(n=250,x=0;n<=280;n++,x++)
        {
        if(x<30)
          {
        show_history[x]=EEPROM.read(n);
        /* tft.textMode();
        tft.textSetCursor(33, 63); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(show_history);
       tft.graphicsMode();*/
      // Serial.print("history");
          }
        /*  if(x>10 && x<=20)
          {
        EEPROM.write(n,save_batch_number[x]);
          }
          if(x>20 && x<=25)
          {
         EEPROM.write(n,save_lot_number[x]);    
          }*/
          
        }
         tft.textMode();
        tft.textSetCursor(33, 63); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(show_history);
       tft.graphicsMode();
        show_history[0]='\0';
        n=x=0;
      }
       ///////  history2///////////
      if(EEPROM.read(340)!='\0')
      {
        for(n=340,x=0;n<=370;n++,x++)
        {
       if(x<30)
          {
        show_history[x]=EEPROM.read(n);
       /*  tft.textMode();
        tft.textSetCursor(33, 103); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(show_history);
       tft.graphicsMode();*/
          }
       /*   if(x>10 && x<=20)
          {
        EEPROM.write(n,save_batch_number[x]);
          }
          if(x>20 && x<=25)
          {
         EEPROM.write(n,save_lot_number[x]);    
          }*/
        }
         tft.textMode();
        tft.textSetCursor(33, 103); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(show_history);
       tft.graphicsMode();
         show_history[0]='\0';
        n=x=0;
      }
       ///////  history3///////////
      if(EEPROM.read(430)!='\0')
      {
         for(n=430,x=0;n<=460;n++,x++)
        {
       if(x<30)
          {
        show_history[x]=EEPROM.read(n);
        /* tft.textMode();
        tft.textSetCursor(33, 143); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(show_history);
       tft.graphicsMode();*/
          }
        /*  if(x>10 && x<=20)
          {
        EEPROM.write(n,save_batch_number[x]);
          }
          if(x>20 && x<=25)
          {
         EEPROM.write(n,save_lot_number[x]);    
          }*/
        }
         tft.textMode();
        tft.textSetCursor(33, 143); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(show_history);
       tft.graphicsMode();
         show_history[0]='\0';
        n=x=0;
      }
       ///////  history4///////////
      if(EEPROM.read(520)!='\0')
      {
         for(n=520,x=0;n<=550;n++,x++)
        {
       if(x<30)
          {
        show_history[x]=EEPROM.read(n);
         /*tft.textMode();
        tft.textSetCursor(33, 183); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(show_history);
       tft.graphicsMode();*/
          }
         /* if(x>10 && x<=20)
          {
        EEPROM.write(n,save_batch_number[x]);
          }
          if(x>20 && x<=25)
          {
         EEPROM.write(n,save_lot_number[x]);    
          }*/
        }
         tft.textMode();
        tft.textSetCursor(33, 183); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(show_history);
       tft.graphicsMode();
         show_history[0]='\0';
        n=x=0;
      }
       ///////  history5///////////
      if(EEPROM.read(610)!='\0')
      {
         for(n=610,x=0;n<=640;n++,x++)
        {
        if(x<30)
          {
        show_history[x]=EEPROM.read(n);
        /* tft.textMode();
        tft.textSetCursor(33, 223); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(show_history);
       tft.graphicsMode();*/
          }
       /*   if(x>10 && x<=20)
          {
        EEPROM.write(n,save_batch_number[x]);
          }
          if(x>20 && x<=25)
          {
         EEPROM.write(n,save_lot_number[x]);    
          }*/
        }
         tft.textMode();
        tft.textSetCursor(33, 223); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(show_history);
       tft.graphicsMode();
         show_history[0]='\0';
        n=x=0;
      }
}

void historyshow2_menu()
{
       ///////  history6///////////
      if(EEPROM.read(700)!='\0')
      {
         for(n=700,x=0;n<=730;n++,x++)
        {
        if(x<30)
          {
        show_history[x]=EEPROM.read(n);
         /*tft.textMode();
        tft.textSetCursor(33, 63); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(show_history);
       tft.graphicsMode();*/
          }
         /* if(x>10 && x<=20)
          {
        EEPROM.write(n,save_batch_number[x]);
          }
          if(x>20 && x<=25)
          {
         EEPROM.write(n,save_lot_number[x]);    
          }*/
        }
         tft.textMode();
        tft.textSetCursor(33, 63); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(show_history);
       tft.graphicsMode();
         show_history[0]='\0';
        n=x=0;
      }
       ///////  history7///////////
      if(EEPROM.read(790)!='\0')
      {
         for(n=790,x=0;n<=820;n++,x++)
        {
        if(x<30)
          {
        show_history[x]=EEPROM.read(n);
       /*  tft.textMode();
        tft.textSetCursor(33, 103); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(show_history);
       tft.graphicsMode();*/
          }
         /* if(x>10 && x<=20)
          {
        EEPROM.write(n,save_batch_number[x]);
          }
          if(x>20 && x<=25)
          {
         EEPROM.write(n,save_lot_number[x]);    
          }*/
        }
         tft.textMode();
        tft.textSetCursor(33, 103); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(show_history);
       tft.graphicsMode();
         show_history[0]='\0';
        n=x=0;
      }
       ///////  history8///////////
      if(EEPROM.read(880)!='\0')
      {
         for(n=880,x=0;n<=910;n++,x++)
        {
       if(x<30)
          {
        show_history[x]=EEPROM.read(n);
        /* tft.textMode();
        tft.textSetCursor(33, 143); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(show_history);
       tft.graphicsMode();*/
          }
       /*   if(x>10 && x<=20)
          {
        EEPROM.write(n,save_batch_number[x]);
          }
          if(x>20 && x<=25)
          {
         EEPROM.write(n,save_lot_number[x]);    
          }*/
        }
         tft.textMode();
        tft.textSetCursor(33, 143); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(show_history);
       tft.graphicsMode();
         show_history[0]='\0';
        n=x=0;
      }
       ///////  history9///////////
      if(EEPROM.read(970)!='\0')
      {
         for(n=970,x=0;n<=1000;n++,x++)
        {
       if(x<30)
          {
        show_history[x]=EEPROM.read(n);
        /* tft.textMode();
        tft.textSetCursor(33, 183); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(show_history);
       tft.graphicsMode();*/
          }
       /*   if(x>10 && x<=20)
          {
        EEPROM.write(n,save_batch_number[x]);
          }
          if(x>20 && x<=25)
          {
         EEPROM.write(n,save_lot_number[x]);    
          }*/
        }
        tft.textMode();
        tft.textSetCursor(33, 183); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(show_history);
       tft.graphicsMode();
         show_history[0]='\0';
        n=x=0;
      } ///////  history10///////////
      if(EEPROM.read(1060)!='\0')
      {
         for(n=1060,x=0;n<=1090;n++,x++)
        {
       if(x<30)
          {
        show_history[x]=EEPROM.read(n);
        /* tft.textMode();
        tft.textSetCursor(33, 223); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(show_history);
       tft.graphicsMode();*/
          }
       /*   if(x>10 && x<=20)
          {
        EEPROM.write(n,save_batch_number[x]);
          }
          if(x>20 && x<=25)
          {
         EEPROM.write(n,save_lot_number[x]);    
          }*/
        }
         tft.textMode();
        tft.textSetCursor(33, 223); ///text location////
      tft.textTransparent(RA8875_BLACK);
      tft.textEnlarge(0);  ////text size////
      tft.textWrite(show_history);
       tft.graphicsMode();
         show_history[0]='\0';
        n=x=0;
      }
}

void historysave_function()
{
  if(historypress==1)
  {
  //  historypress=2;
  
  if(resultstore_flag==1)
    {
      resultstore_flag=0;
     // Serial.print("save his up");
    ///////  history10///////////
      if(EEPROM.read(1060)=='\0' && EEPROM.read(990)!='\0' && EEPROM.read(900)!='\0' && EEPROM.read(810)!='\0' && EEPROM.read(720)!='\0' && EEPROM.read(630)!='\0' && EEPROM.read(540)!='\0' && EEPROM.read(450)!='\0' && EEPROM.read(360)!='\0' && EEPROM.read(250)!='\0')
      {
         for(n=1060,n2=1060,x=0;n<1150;n++,x++)
        {
       if(x<20)
         {
        EEPROM.write(n,historytime[x]);
         }
        if(x>=20 && x<30)
          {
        EEPROM.write(n,save_product_name[x-20]);
        //Serial.print("save his");
          }
         // EEPROM.write(n,'\0');
          if(x>=30 && x<40)
          {
        EEPROM.write(n,save_batch_number[x-30]);
          }
       //   EEPROM.write(n,'\0');
          if(x>=40 && x<50)
          {
         EEPROM.write(n,save_lot_number[x-40]);    
          }
        //  EEPROM.write(n,'\0');
          if(x>=50 && x<90)
          {
            if(x==50)
            {
         EEPROM.write(n,save_no_of_drums);
            }
            if(x>=51 && x<52)
            {
         EEPROM.write(n,save_drum_type); 
            } 
            if(x>=52 && x<54)
            {
         EEPROM.write(n,rpm_value[x-52]+'0'); 
            } 
            if(x>=54 && x<55)
            {
         EEPROM.write(n,select_mode); 
            } 
            if(x>=55 && x<58)
            {
              if(EEPROM.read(n2+54)==2)
              {
         EEPROM.write(n,count_value[x-55]+'0');      
              }
              else
              {
         EEPROM.write(n,time_value[x-55]+'0');
              } 
            }
              if(x>=59 && x<85)
              {
              if(EEPROM.read(n2+50)==1)
              {
                if(x>=59 && x<64)
                {
           EEPROM.write(n,hisweight1_value[x-59]+'0');
                }
                if(x>=64 && x<69)
                {
           EEPROM.write(n,hisweightout1_value[x-64]+'0');
                }
                if(x>=69 && x<72)
                {
            EEPROM.write(n,per_string1[x-69]);
                }
              }
              else
                {
                  if(x>=59 && x<64)
                {
           EEPROM.write(n,hisweight1_value[x-59]+'0');
                }
                if(x>=64 && x<69)
                {
           EEPROM.write(n,hisweightout1_value[x-64]+'0');
                }
                if(x>=69 && x<74)
                {
           EEPROM.write(n,hisweight2_value[x-69]+'0');
                }
                if(x>=74 && x<79)
                {
           EEPROM.write(n,hisweightout2_value[x-74]+'0');
                }
                if(x>=79 && x<82)
                {
            EEPROM.write(n,per_string1[x-79]);
                }
                if(x>=82 && x<85)
                {
            EEPROM.write(n,per_string2[x-82]);
                }
                }
              }
            
                if(x==86)
              {
                EEPROM.write(n,save_weight_unit); 
              }
          }
        }
        n=x=0;
        save_product_name[x]='\0';
        save_batch_number[x]='\0';
        save_lot_number[x]='\0';
      }
      ///////  history9///////////
      if(EEPROM.read(970)=='\0' && EEPROM.read(880)!='\0' && EEPROM.read(790)!='\0' && EEPROM.read(700)!='\0' && EEPROM.read(610)!='\0' && EEPROM.read(520)!='\0' && EEPROM.read(430)!='\0' && EEPROM.read(340)!='\0' && EEPROM.read(250)!='\0')
      {
         for(n=970,n2=970,x=0;n<1060;n++,x++)
        {
       if(x<20)
         {
        EEPROM.write(n,historytime[x]);
         }
        if(x>=20 && x<30)
          {
        EEPROM.write(n,save_product_name[x-20]);
        //Serial.print("save his");
          }
         // EEPROM.write(n,'\0');
          if(x>=30 && x<40)
          {
        EEPROM.write(n,save_batch_number[x-30]);
          }
        //  EEPROM.write(n,'\0');
          if(x>=40 && x<50)
          {
         EEPROM.write(n,save_lot_number[x-40]);    
          }
        //  EEPROM.write(n,'\0');
          if(x>=50 && x<90)
          {
            if(x==50)
            {
         EEPROM.write(n,save_no_of_drums);
            }
            if(x>=51 && x<52)
            {
         EEPROM.write(n,save_drum_type); 
            } 
            if(x>=52 && x<54)
            {
         EEPROM.write(n,rpm_value[x-52]+'0'); 
            } 
            if(x>=54 && x<55)
            {
         EEPROM.write(n,select_mode); 
            } 
            if(x>=55 && x<58)
            {
              if(EEPROM.read(n2+54)==2)
              {
         EEPROM.write(n,count_value[x-55]+'0');      
              }
              else
              {
         EEPROM.write(n,time_value[x-55]+'0');
              } 
            }
              if(x>=59 && x<85)
              {
              if(EEPROM.read(n2+50)==1)
              {
                if(x>=59 && x<64)
                {
           EEPROM.write(n,hisweight1_value[x-59]+'0');
                }
                if(x>=64 && x<69)
                {
           EEPROM.write(n,hisweightout1_value[x-64]+'0');
                }
                if(x>=69 && x<72)
                {
            EEPROM.write(n,per_string1[x-69]);
                }
              }
              else
                {
                  if(x>=59 && x<64)
                {
           EEPROM.write(n,hisweight1_value[x-59]+'0');
                }
                if(x>=64 && x<69)
                {
           EEPROM.write(n,hisweightout1_value[x-64]+'0');
                }
                if(x>=69 && x<74)
                {
           EEPROM.write(n,hisweight2_value[x-69]+'0');
                }
                if(x>=74 && x<79)
                {
           EEPROM.write(n,hisweightout2_value[x-74]+'0');
                }
                if(x>=79 && x<82)
                {
            EEPROM.write(n,per_string1[x-79]);
                }
                if(x>=82 && x<85)
                {
            EEPROM.write(n,per_string2[x-82]);
                }
                }
              }
            
                if(x==86)
              {
                EEPROM.write(n,save_weight_unit); 
              }
          }
        }
        n=x=0;
        save_product_name[x]='\0';
        save_batch_number[x]='\0';
        save_lot_number[x]='\0';
      }
      ///////  history8///////////
      if(EEPROM.read(880)=='\0' && EEPROM.read(790)!='\0' && EEPROM.read(700)!='\0' && EEPROM.read(610)!='\0' && EEPROM.read(520)!='\0' && EEPROM.read(430)!='\0' && EEPROM.read(340)!='\0' && EEPROM.read(250)!='\0')
      {
         for(n=880,n2=880,x=0;n<970;n++,x++)
        {
       if(x<20)
         {
        EEPROM.write(n,historytime[x]);
         }
        if(x>=20 && x<30)
          {
        EEPROM.write(n,save_product_name[x-20]);
        //Serial.print("save his");
          }
        //  EEPROM.write(n,'\0');
          if(x>=30 && x<40)
          {
        EEPROM.write(n,save_batch_number[x-30]);
          }
        //  EEPROM.write(n,'\0');
          if(x>=40 && x<50)
          {
         EEPROM.write(n,save_lot_number[x-40]);    
          }
         // EEPROM.write(n,'\0');
          if(x>=50 && x<90)
          {
            if(x==50)
            {
         EEPROM.write(n,save_no_of_drums);
            }
            if(x>=51 && x<52)
            {
         EEPROM.write(n,save_drum_type); 
            } 
            if(x>=52 && x<54)
            {
         EEPROM.write(n,rpm_value[x-52]+'0'); 
            } 
            if(x>=54 && x<55)
            {
         EEPROM.write(n,select_mode); 
            } 
            if(x>=55 && x<58)
            {
              if(EEPROM.read(n2+54)==2)
              {
         EEPROM.write(n,count_value[x-55]+'0');      
              }
              else
              {
         EEPROM.write(n,time_value[x-55]+'0');
              } 
            }
              if(x>=59 && x<85)
              {
              if(EEPROM.read(n2+50)==1)
              {
                if(x>=59 && x<64)
                {
           EEPROM.write(n,hisweight1_value[x-59]+'0');
                }
                if(x>=64 && x<69)
                {
           EEPROM.write(n,hisweightout1_value[x-64]+'0');
                }
                if(x>=69 && x<72)
                {
            EEPROM.write(n,per_string1[x-69]);
                }
              }
              else
                {
                  if(x>=59 && x<64)
                {
           EEPROM.write(n,hisweight1_value[x-59]+'0');
                }
                if(x>=64 && x<69)
                {
           EEPROM.write(n,hisweightout1_value[x-64]+'0');
                }
                if(x>=69 && x<74)
                {
           EEPROM.write(n,hisweight2_value[x-69]+'0');
                }
                if(x>=74 && x<79)
                {
           EEPROM.write(n,hisweightout2_value[x-74]+'0');
                }
                if(x>=79 && x<82)
                {
            EEPROM.write(n,per_string1[x-79]);
                }
                if(x>=82 && x<85)
                {
            EEPROM.write(n,per_string2[x-82]);
                }
                }
              }
            
                if(x==86)
              {
                EEPROM.write(n,save_weight_unit); 
              }
          }
        }
        n=x=0;
        save_product_name[x]='\0';
        save_batch_number[x]='\0';
        save_lot_number[x]='\0';
      }
      ///////  history7///////////
      if(EEPROM.read(790)=='\0' && EEPROM.read(700)!='\0' && EEPROM.read(610)!='\0' && EEPROM.read(520)!='\0' && EEPROM.read(430)!='\0' && EEPROM.read(340)!='\0' && EEPROM.read(250)!='\0')
      {
         for(n=790,n2=790,x=0;n<880;n++,x++)
        {
        if(x<20)
         {
        EEPROM.write(n,historytime[x]);
         }
        if(x>=20 && x<30)
          {
        EEPROM.write(n,save_product_name[x-20]);
        //Serial.print("save his");
          }
        //  EEPROM.write(n,'\0');
          if(x>=30 && x<40)
          {
        EEPROM.write(n,save_batch_number[x-30]);
          }
        //  EEPROM.write(n,'\0');
          if(x>=40 && x<50)
          {
         EEPROM.write(n,save_lot_number[x-40]);    
          }
        //  EEPROM.write(n,'\0');
          if(x>=50 && x<90)
          {
            if(x==50)
            {
         EEPROM.write(n,save_no_of_drums);
            }
            if(x>=51 && x<52)
            {
         EEPROM.write(n,save_drum_type); 
            } 
            if(x>=52 && x<54)
            {
         EEPROM.write(n,rpm_value[x-52]+'0'); 
            } 
            if(x>=54 && x<55)
            {
         EEPROM.write(n,select_mode); 
            } 
            if(x>=55 && x<58)
            {
              if(EEPROM.read(n2+54)==2)
              {
         EEPROM.write(n,count_value[x-55]+'0');      
              }
              else
              {
         EEPROM.write(n,time_value[x-55]+'0');
              } 
            }
              if(x>=59 && x<85)
              {
              if(EEPROM.read(n2+50)==1)
              {
                if(x>=59 && x<64)
                {
           EEPROM.write(n,hisweight1_value[x-59]+'0');
                }
                if(x>=64 && x<69)
                {
           EEPROM.write(n,hisweightout1_value[x-64]+'0');
                }
                if(x>=69 && x<72)
                {
            EEPROM.write(n,per_string1[x-69]);
                }
              }
              else
                {
                  if(x>=59 && x<64)
                {
           EEPROM.write(n,hisweight1_value[x-59]+'0');
                }
                if(x>=64 && x<69)
                {
           EEPROM.write(n,hisweightout1_value[x-64]+'0');
                }
                if(x>=69 && x<74)
                {
           EEPROM.write(n,hisweight2_value[x-69]+'0');
                }
                if(x>=74 && x<79)
                {
           EEPROM.write(n,hisweightout2_value[x-74]+'0');
                }
                if(x>=79 && x<82)
                {
            EEPROM.write(n,per_string1[x-79]);
                }
                if(x>=82 && x<85)
                {
            EEPROM.write(n,per_string2[x-82]);
                }
                }
              }
            if(x==86)
              {
                EEPROM.write(n,save_weight_unit); 
              }
                
          }
        }
        n=x=0;
        save_product_name[x]='\0';
        save_batch_number[x]='\0';
        save_lot_number[x]='\0';
      }
      ///////  history6///////////
      if(EEPROM.read(700)=='\0' && EEPROM.read(610)!='\0' && EEPROM.read(520)!='\0' && EEPROM.read(430)!='\0' && EEPROM.read(340)!='\0' && EEPROM.read(250)!='\0')
      {
         for(n=700,n2=700,x=0;n<790;n++,x++)
        {
         if(x<20)
         {
        EEPROM.write(n,historytime[x]);
         }
        if(x>=20 && x<30)
          {
        EEPROM.write(n,save_product_name[x-20]);
        //Serial.print("save his");
          }
        //  EEPROM.write(n,'\0');
          if(x>=30 && x<40)
          {
        EEPROM.write(n,save_batch_number[x-30]);
          }
         // EEPROM.write(n,'\0');
          if(x>=40 && x<50)
          {
         EEPROM.write(n,save_lot_number[x-40]);    
          }
        //  EEPROM.write(n,'\0');
          if(x>=50 && x<90)
          {
            if(x==50)
            {
         EEPROM.write(n,save_no_of_drums);
            }
            if(x>=51 && x<52)
            {
         EEPROM.write(n,save_drum_type); 
            } 
            if(x>=52 && x<54)
            {
         EEPROM.write(n,rpm_value[x-52]+'0'); 
            } 
            if(x>=54 && x<55)
            {
         EEPROM.write(n,select_mode); 
            } 
            if(x>=55 && x<58)
            {
              if(EEPROM.read(n2+54)==2)
              {
         EEPROM.write(n,count_value[x-55]+'0');      
              }
              else
              {
         EEPROM.write(n,time_value[x-55]+'0');
              } 
            }
              if(x>=59 && x<85)
              {
              if(EEPROM.read(n2+50)==1)
              {
                if(x>=59 && x<64)
                {
           EEPROM.write(n,hisweight1_value[x-59]+'0');
                }
                if(x>=64 && x<69)
                {
           EEPROM.write(n,hisweightout1_value[x-64]+'0');
                }
                if(x>=69 && x<72)
                {
            EEPROM.write(n,per_string1[x-69]);
                }
              }
              else
                {
                  if(x>=59 && x<64)
                {
           EEPROM.write(n,hisweight1_value[x-59]+'0');
                }
                if(x>=64 && x<69)
                {
           EEPROM.write(n,hisweightout1_value[x-64]+'0');
                }
                if(x>=69 && x<74)
                {
           EEPROM.write(n,hisweight2_value[x-69]+'0');
                }
                if(x>=74 && x<79)
                {
           EEPROM.write(n,hisweightout2_value[x-74]+'0');
                }
                if(x>=79 && x<82)
                {
            EEPROM.write(n,per_string1[x-79]);
                }
                if(x>=82 && x<85)
                {
            EEPROM.write(n,per_string2[x-82]);
                }
                }
              } 
              if(x==86)
              {
                EEPROM.write(n,save_weight_unit); 
              }    
          }
        }
        n=x=0;
        save_product_name[x]='\0';
        save_batch_number[x]='\0';
        save_lot_number[x]='\0';
      }
      ///////  history5///////////
      if(EEPROM.read(610)=='\0' && EEPROM.read(520)!='\0' && EEPROM.read(430)!='\0' && EEPROM.read(340)!='\0' && EEPROM.read(250)!='\0')
      {
         for(n=610,n2=610,x=0;n<700;n++,x++)
        {
         if(x<20)
         {
        EEPROM.write(n,historytime[x]);
         }
        if(x>=20 && x<30)
          {
        EEPROM.write(n,save_product_name[x-20]);
        //Serial.print("save his");
          }
       //   EEPROM.write(n,'\0');
          if(x>=30 && x<40)
          {
        EEPROM.write(n,save_batch_number[x-30]);
          }
       //   EEPROM.write(n,'\0');
          if(x>=40 && x<50)
          {
         EEPROM.write(n,save_lot_number[x-40]);    
          }
        //  EEPROM.write(n,'\0');
          if(x>=50 && x<90)
          {
            if(x==50)
            {
         EEPROM.write(n,save_no_of_drums);
            }
            if(x>=51 && x<52)
            {
         EEPROM.write(n,save_drum_type); 
            } 
            if(x>=52 && x<54)
            {
         EEPROM.write(n,rpm_value[x-52]+'0'); 
            } 
            if(x>=54 && x<55)
            {
         EEPROM.write(n,select_mode); 
            } 
            if(x>=55 && x<58)
            {
              if(EEPROM.read(n2+54)==2)
              {
         EEPROM.write(n,count_value[x-55]+'0');      
              }
              else
              {
         EEPROM.write(n,time_value[x-55]+'0');
              } 
            }
              if(x>=59 && x<85)
              {
              if(EEPROM.read(n2+50)==1)
              {
                if(x>=59 && x<64)
                {
           EEPROM.write(n,hisweight1_value[x-59]+'0');
                }
                if(x>=64 && x<69)
                {
           EEPROM.write(n,hisweightout1_value[x-64]+'0');
                }
                if(x>=69 && x<72)
                {
            EEPROM.write(n,per_string1[x-69]);
                }
              }
              else
                {
                  if(x>=59 && x<64)
                {
           EEPROM.write(n,hisweight1_value[x-59]+'0');
                }
                if(x>=64 && x<69)
                {
           EEPROM.write(n,hisweightout1_value[x-64]+'0');
                }
                if(x>=69 && x<74)
                {
           EEPROM.write(n,hisweight2_value[x-69]+'0');
                }
                if(x>=74 && x<79)
                {
           EEPROM.write(n,hisweightout2_value[x-74]+'0');
                }
                if(x>=79 && x<82)
                {
            EEPROM.write(n,per_string1[x-79]);
                }
                if(x>=82 && x<85)
                {
            EEPROM.write(n,per_string2[x-82]);
                }
                }
              }  
              if(x==86)
              {
                EEPROM.write(n,save_weight_unit); 
              }  
          }
        }
        n=x=0;
        save_product_name[x]='\0';
        save_batch_number[x]='\0';
        save_lot_number[x]='\0';
      }
      ///////  history4///////////
      if(EEPROM.read(520)=='\0' && EEPROM.read(430)!='\0' && EEPROM.read(340)!='\0' && EEPROM.read(250)!='\0')
      {
         for(n=520,n2=520,x=0;n<610;n++,x++)
        {
       if(x<20)
         {
        EEPROM.write(n,historytime[x]);
         }
        if(x>=20 && x<30)
          {
        EEPROM.write(n,save_product_name[x-20]);
        //Serial.print("save his");
          }
        //  EEPROM.write(n,'\0');
          if(x>=30 && x<40)
          {
        EEPROM.write(n,save_batch_number[x-30]);
          }
         // EEPROM.write(n,'\0');
          if(x>=40 && x<50)
          {
         EEPROM.write(n,save_lot_number[x-40]);    
          }
        //  EEPROM.write(n,'\0');
          if(x>=50 && x<90)
          {
            if(x==50)
            {
         EEPROM.write(n,save_no_of_drums);
            }
            if(x>=51 && x<52)
            {
         EEPROM.write(n,save_drum_type); 
            } 
            if(x>=52 && x<54)
            {
         EEPROM.write(n,rpm_value[x-52]+'0'); 
            } 
            if(x>=54 && x<55)
            {
         EEPROM.write(n,select_mode); 
            } 
            if(x>=55 && x<58)
            {
              if(EEPROM.read(n2+54)==2)
              {
         EEPROM.write(n,count_value[x-55]+'0');      
              }
              else
              {
         EEPROM.write(n,time_value[x-55]+'0');
              } 
            }
              if(x>=59 && x<85)
              {
              if(EEPROM.read(n2+50)==1)
              {
                if(x>=59 && x<64)
                {
           EEPROM.write(n,hisweight1_value[x-59]+'0');
                }
                if(x>=64 && x<69)
                {
           EEPROM.write(n,hisweightout1_value[x-64]+'0');
                }
                if(x>=69 && x<72)
                {
            EEPROM.write(n,per_string1[x-69]);
                }
              }
              else
                {
                  if(x>=59 && x<64)
                {
           EEPROM.write(n,hisweight1_value[x-59]+'0');
                }
                if(x>=64 && x<69)
                {
           EEPROM.write(n,hisweightout1_value[x-64]+'0');
                }
                if(x>=69 && x<74)
                {
           EEPROM.write(n,hisweight2_value[x-69]+'0');
                }
                if(x>=74 && x<79)
                {
           EEPROM.write(n,hisweightout2_value[x-74]+'0');
                }
                if(x>=79 && x<82)
                {
            EEPROM.write(n,per_string1[x-79]);
                }
                if(x>=82 && x<85)
                {
            EEPROM.write(n,per_string2[x-82]);
                }
                }
              } 
              if(x==86)
              {
                EEPROM.write(n,save_weight_unit); 
              }   
          }
        }
        n=x=0;
        save_product_name[x]='\0';
        save_batch_number[x]='\0';
        save_lot_number[x]='\0';
      }
      ///////  history3///////////
      if(EEPROM.read(430)=='\0' && EEPROM.read(340)!='\0' && EEPROM.read(250)!='\0')
      {
         for(n=430,n2=430,x=0;n<520;n++,x++)
        {
       if(x<20)
         {   
        EEPROM.write(n,historytime[x]);
         }
        if(x>=20 && x<30)
          {
             
         EEPROM.write(n,save_product_name[x-20]);
          }
          
          // EEPROM.write(n,'\0');
          if(x>=30 && x<40)
          {
        EEPROM.write(n,save_batch_number[x-30]);
          }
          
       
      
          // EEPROM.write(n,'\0');
          if(x>=40 && x<50)
          {
         EEPROM.write(n,save_lot_number[x-40]);    
          }
         
           //EEPROM.write(n,'\0');
          if(x>=50 && x<90)
          {
            if(x==50)
            {
         EEPROM.write(n,save_no_of_drums);
            }
            if(x>=51 && x<52)
            {
         EEPROM.write(n,save_drum_type); 
            } 
            if(x>=52 && x<54)
            {
         EEPROM.write(n,rpm_value[x-52]+'0'); 
            } 
            if(x>=54 && x<55)
            {
         EEPROM.write(n,select_mode); 
            } 
            if(x>=55 && x<58)
            {
              if(EEPROM.read(n2+54)==2)
              {
         EEPROM.write(n,count_value[x-55]+'0');      
              }
              else
              {
         EEPROM.write(n,time_value[x-55]+'0');
              } 
            }
              if(x>=59 && x<85)
              {
              if(EEPROM.read(n2+50)==1)
              {
                if(x>=59 && x<64)
                {
           EEPROM.write(n,hisweight1_value[x-59]+'0');
                }
                if(x>=64 && x<69)
                {
           EEPROM.write(n,hisweightout1_value[x-64]+'0');
                }
                if(x>=69 && x<72)
                {
            EEPROM.write(n,per_string1[x-69]);
                }
              }
              else
                {
                  if(x>=59 && x<64)
                {
           EEPROM.write(n,hisweight1_value[x-59]+'0');
                }
                if(x>=64 && x<69)
                {
           EEPROM.write(n,hisweightout1_value[x-64]+'0');
                }
                if(x>=69 && x<74)
                {
           EEPROM.write(n,hisweight2_value[x-69]+'0');
                }
                if(x>=74 && x<79)
                {
           EEPROM.write(n,hisweightout2_value[x-74]+'0');
                }
                if(x>=79 && x<82)
                {
            EEPROM.write(n,per_string1[x-79]);
                }
                if(x>=82 && x<85)
                {
            EEPROM.write(n,per_string2[x-82]);
                }
                }
              }
            
             if(x==86)
              {
                EEPROM.write(n,save_weight_unit); 
              }
          }
        }
        n=x=0;
        save_product_name[x]='\0';
        save_batch_number[x]='\0';
        save_lot_number[x]='\0';
      }
       ///////  history2///////////
      if(EEPROM.read(340)=='\0' && EEPROM.read(250)!='\0')
      {
        for(n=340,n2=340,x=0;n<430;n++,x++)
        {
       if(x<20)
         {   
        EEPROM.write(n,historytime[x]);
         }
        if(x>=20 && x<30)
          {
             
         EEPROM.write(n,save_product_name[x-20]);
          }
          
          // EEPROM.write(n,'\0');
          if(x>=30 && x<40)
          {
        EEPROM.write(n,save_batch_number[x-30]);
          }
          
       
      
          // EEPROM.write(n,'\0');
          if(x>=40 && x<50)
          {
         EEPROM.write(n,save_lot_number[x-40]);    
          }
         
           //EEPROM.write(n,'\0');
          if(x>=50 && x<90)
          {
            if(x==50)
            {
         EEPROM.write(n,save_no_of_drums);
            }
            if(x>=51 && x<52)
            {
         EEPROM.write(n,save_drum_type); 
            } 
            if(x>=52 && x<54)
            {
         EEPROM.write(n,rpm_value[x-52]+'0'); 
            } 
            if(x>=54 && x<55)
            {
         EEPROM.write(n,select_mode); 
            } 
            if(x>=55 && x<58)
            {
              if(EEPROM.read(n2+54)==2)
              {
         EEPROM.write(n,count_value[x-55]+'0');      
              }
              else
              {
         EEPROM.write(n,time_value[x-55]+'0');
              } 
            }
              if(x>=59 && x<85)
              {
              if(EEPROM.read(n2+50)==1)
              {
                if(x>=59 && x<64)
                {
           EEPROM.write(n,hisweight1_value[x-59]+'0');
                }
                if(x>=64 && x<69)
                {
           EEPROM.write(n,hisweightout1_value[x-64]+'0');
                }
                if(x>=69 && x<72)
                {
            EEPROM.write(n,per_string1[x-69]);
                }
              }
              else
                {
                  if(x>=59 && x<64)
                {
           EEPROM.write(n,hisweight1_value[x-59]+'0');
                }
                if(x>=64 && x<69)
                {
           EEPROM.write(n,hisweightout1_value[x-64]+'0');
                }
                if(x>=69 && x<74)
                {
           EEPROM.write(n,hisweight2_value[x-69]+'0');
                }
                if(x>=74 && x<79)
                {
           EEPROM.write(n,hisweightout2_value[x-74]+'0');
                }
                if(x>=79 && x<82)
                {
            EEPROM.write(n,per_string1[x-79]);
                }
                if(x>=82 && x<85)
                {
            EEPROM.write(n,per_string2[x-82]);
                }
                }
              }
            
             if(x==86)
              {
                EEPROM.write(n,save_weight_unit); 
              }
          }
        }
        n=x=0;
        save_product_name[x]='\0';
        save_batch_number[x]='\0';
        save_lot_number[x]='\0';
      }
      ///////  history1///////////
      if(EEPROM.read(250)=='\0')
      {
        for(n=250,n2=250,x=0;n<340;n++,x++)
        {
          if(x<20)
         {   
        EEPROM.write(n,historytime[x]);
         }
        if(x>=20 && x<30)
          {
             
         EEPROM.write(n,save_product_name[x-20]);
          }
          
          // EEPROM.write(n,'\0');
          if(x>=30 && x<40)
          {
        EEPROM.write(n,save_batch_number[x-30]);
          }
          
       
      
          // EEPROM.write(n,'\0');
          if(x>=40 && x<50)
          {
         EEPROM.write(n,save_lot_number[x-40]);    
          }
         
           //EEPROM.write(n,'\0');
          if(x>=50 && x<90)
          {
            if(x==50)
            {
         EEPROM.write(n,save_no_of_drums);
            }
            if(x>=51 && x<52)
            {
         EEPROM.write(n,save_drum_type); 
            } 
            if(x>=52 && x<54)
            {
         EEPROM.write(n,rpm_value[x-52]+'0'); 
            } 
            if(x>=54 && x<55)
            {
         EEPROM.write(n,select_mode); 
            } 
            if(x>=55 && x<58)
            {
              if(EEPROM.read(n2+54)==2)
              {
         EEPROM.write(n,count_value[x-55]+'0');      
              }
              else
              {
         EEPROM.write(n,time_value[x-55]+'0');
              } 
            }
              if(x>=59 && x<85)
              {
              if(EEPROM.read(n2+50)==1)
              {
                if(x>=59 && x<64)
                {
           EEPROM.write(n,hisweight1_value[x-59]+'0');
                }
                if(x>=64 && x<69)
                {
           EEPROM.write(n,hisweightout1_value[x-64]+'0');
                }
                if(x>=69 && x<72)
                {
            EEPROM.write(n,per_string1[x-69]);
                }
              }
              else
                {
                  if(x>=59 && x<64)
                {
           EEPROM.write(n,hisweight1_value[x-59]+'0');
                }
                if(x>=64 && x<69)
                {
           EEPROM.write(n,hisweightout1_value[x-64]+'0');
                }
                if(x>=69 && x<74)
                {
           EEPROM.write(n,hisweight2_value[x-69]+'0');
                }
                if(x>=74 && x<79)
                {
           EEPROM.write(n,hisweightout2_value[x-74]+'0');
                }
                if(x>=79 && x<82)
                {
            EEPROM.write(n,per_string1[x-79]);
                }
                if(x>=82 && x<85)
                {
            EEPROM.write(n,per_string2[x-82]);
                }
                }
              }
            
             if(x==86)
              {
                EEPROM.write(n,save_weight_unit); 
              }
          }
     /*      Serial.print("initiava=");
         for(i=59;i<64;i++)
  {
Serial.print(EEPROM.read(250+i));

  }*/
     
        }
       
        n=n2=x=0;
        save_product_name[0]='\0';
        save_batch_number[0]='\0';
        save_lot_number[0]='\0';
      }
       
    }
  }
}

void printout()
{

if(tx>=219 && tx<=341 && ty>=646 && ty<=756 && historypage==0)
{
Serial.print("0");
 n2=250;
 printd=1;
 printdl=1;
}
if(tx>=219 && tx<=341 && ty>=506 && ty<=645 && historypage==0)
{
  Serial.print("1");
  n2=340;
  printd=1;
  printdl=1;
}
if(tx>=219 && tx<=341 && ty>=384 && ty<=505 && historypage==0)
{
  Serial.print("2");
  n2=430;
  printd=1;
  printdl=1;
}
if(tx>=219 && tx<=341 && ty>=255 && ty<=383 && historypage==0)
{
  Serial.print("3");
  n2=520;
  printd=1;
  printdl=1;
}
if(tx>=219 && tx<=341 && ty>=121 && ty<=254 && historypage==0)
{
  Serial.print("4");
  n2=610;
  printd=1;
  printdl=1;
}


  if(tx>=219 && tx<=341 && ty>=646 && ty<=756 && historypage==2)
{
  Serial.print("0");
 n2=700;
 printd=1;
 printdl=1;
}
if(tx>=219 && tx<=341 && ty>=506 && ty<=645 && historypage==2)
{
  Serial.print("1");
  n2=790;
  printd=1;
  printdl=1;
}
if(tx>=219 && tx<=341 && ty>=384 && ty<=505 && historypage==2)
{
  Serial.print("2");
  n2=880;
  printd=1;
  printdl=1;
}
if(tx>=219 && tx<=341 && ty>=255 && ty<=383 && historypage==2)
{
  Serial.print("3");
  n2=970;
  printd=1;
  printdl=1;
}
if(tx>=219 && tx<=341 && ty>=121 && ty<=254 && historypage==2)
{
  Serial.print("4");
  n2=1060;
  printd=1;
  printdl=1;
}
 DateTime now = rtc.now();
    y=now.year();
   // y=2021;
    rtc_string[0]=(y/1000)+'0';
    y=y%1000;
     rtc_string[1]=(y/100)+'0';
    y=y%100;
 rtc_string[2]=(y/10)+'0';
    y=y%10;
     rtc_string[3]=y+'0';
  rtc_string[4]='/';
   m=now.month();
    //m=01;
    rtc_string[5]=(m/10)+'0';
    m=m%10;
     rtc_string[6]=m+'0';
     rtc_string[7]='/';
    d=now.day();
   // d=23;
    rtc_string[8]=(d/10)+'0';
    d=d%10;
     rtc_string[9]=d+'0';
//rtc_string[10]='/';

rtc_string[10]=' ';
rtc_string[11]=' ';
if(now.hour()>12)
{
  h=now.hour()-12;
}
else
{
    h=now.hour();
}
    //h=01;
    rtc_string[12]=(h/10)+'0';
    h=h%10;
     rtc_string[13]=h+'0';
     rtc_string[14]=':';
    mi=now.minute();
   // mi=16;
    rtc_string[15]=(mi/10)+'0';
    mi=mi%10;
     rtc_string[16]=mi+'0';
     rtc_string[17]=':';
     s=now.second();
      rtc_string[18]=(s/10)+'0';
    s=s%10;
     rtc_string[19]=s+'0';
     if(rtc_string[5]!='@' && rtc_string[8]!='@' && rtc_string[2]!='6' && rtc_string[2]!='8' && rtc_string[2]!='4' && rtc_string[1]!='1')
   {
    for(i=0;i<=19;i++)
    {
      rtc_string2[i]=rtc_string[i];
    }
     rtc_string2[i]=0;
     i=0;
 // tft.graphicsMode();
   }
if(printdl==1)
{
  printdl=0;
for(n=n2,x=0;n<n2+44;n++,x++)
  {
    if(x<=11)
    {
    hisdate[x]=EEPROM.read(n);
    }
    if(x>=12 && x<20)
    {
    histime[x-12]=EEPROM.read(n);
    }
    if(x>=20 && x<30)
    {
    hisprod[x-20]=EEPROM.read(n);
    }
    if(x>=30 && x<40)
    {
    hisbatch[x-30]=EEPROM.read(n);
    }
    if(x>=40 && x<50)
    {
    hislot[x-40]=EEPROM.read(n);
    }
  }
  x=i=0;
}
if(EEPROM.read(3995)>=100)
{
  EEPROM.write(3995,20);
}
if(EEPROM.read(3995)<=15)
{
  if(save_printer==1 && printd==1)
  {
    printd=0;
    SoftwareSerial softSerial(19, 18,true);
    softSerial.begin(9600);

for(i=0;i<4;i++)
{
softSerial.write("\n");
delay(10);
}
softSerial.write("Company Name  :");
for(i=0;EEPROM.read(2000+i)!='\0';i++)
{
softSerial.write(EEPROM.read(2000+i));
}
softSerial.write("\n");
delay(10);
softSerial.write("Serial No.    :");
for(i=0;EEPROM.read(2061+i)!='\0';i++)
{
softSerial.write(EEPROM.read(2061+i));
}
softSerial.write("\n");
delay(10);
 softSerial.write("Print Date    :");
delay(50);
for(i=0;i<=11;i++)
{
softSerial.write(rtc_string2[i]);
delay(10);
}
softSerial.write("\n");
delay(10);
softSerial.write("Print Time    :");
delay(50);
for(i=12;i<=19;i++)
{
softSerial.write(rtc_string2[i]);
delay(10);
}
softSerial.write("\n");
delay(10);
for(i=0;i<=30;i++)
{
softSerial.write("-");
delay(10);
}
softSerial.write("\n");
delay(10);   

softSerial.write("Test Date    :");
delay(50);
for(i=0;i<=11;i++)
{
softSerial.write(hisdate[i]);
delay(10);
}
softSerial.write("\n");
delay(10);
softSerial.write("Test Time    :");
delay(50);
for(i=12;i<=19;i++)
{
softSerial.write(histime[i-12]);
delay(10);
}

softSerial.write("\n");
delay(10);
softSerial.write("Drum Type    :");
delay(50);
Serial.print(EEPROM.read(n2+51));
if(EEPROM.read(n2+51)==1)
{
softSerial.write("Friability");
delay(10);
}
if(EEPROM.read(n2+51)==2)
{
  softSerial.write("Abration");
delay(10);
}
softSerial.write("\n");
delay(10);
softSerial.write("No. of Drums :");
delay(50);
softSerial.write(EEPROM.read(n2+50)+'0');
delay(50);

softSerial.write("\n");
delay(10);
softSerial.write("Selected Mode:");
delay(50);
if(EEPROM.read(n2+54)==2)
{
softSerial.write("Counts");
delay(10);
softSerial.write("\n");
delay(10);
softSerial.write("Counts       = ");
delay(10);
for(i=55;i<58;i++)
{
softSerial.write( EEPROM.read(n2+i));
delay(10);
}
}
else
{
  softSerial.write("Time");
delay(10);
softSerial.write("\n");
delay(10);
softSerial.write("Time=");
delay(10);
for(i=55;i<58;i++)
{
softSerial.write( EEPROM.read(n2+i));
delay(10);
}
}
softSerial.write("\n");
delay(10);
for(i=0;i<=30;i++)
{
softSerial.write("-");
delay(10);
}
softSerial.write("\n");
delay(10);
 softSerial.write("   Product Name :");
  delay(50);
 for(i=n2+20;i<=n2+29 /*||EEPROM.read(i)!='\0'  hisprod[i-20]!='\0'*/;i++)
{
//softSerial.write(hisprod[i-20]);
softSerial.write(EEPROM.read(i));
delay(10);
}
softSerial.write("\n");
delay(10);
  softSerial.write("   Batch/Lot Number :");
  delay(50);
  for(i=n2+30;i<=n2+39 /*|| EEPROM.read(i)!='\0' hisbatch[i-30]!='\0'*/;i++)
{
//softSerial.write(hisbatch[i-30]);
softSerial.write(EEPROM.read(i));
delay(10);
}
softSerial.write("\n");
delay(10);
 /* softSerial.write("   Lot Number   :");
  delay(10);
  for(i=n2+39;i<=n2+44 ||  EEPROM.read(i)!='\0' /*hislot[i-30]!='\0'*///;i++)
//{
//softSerial.write(hislot[i-30]);
/*softSerial.write(EEPROM.read(i));
delay(10);
}
softSerial.write("\n");
delay(10);
*/
   for(i=0;i<=30;i++)
{
softSerial.write("-");
  delay(10);
}
softSerial.write("\n");
delay(10);
if(EEPROM.read(n2+50)==1)
{
  softSerial.write("Initial Weight= ");
delay(10);
  for(i=59;i<64;i++)
  {
    if(i==62)
    {
      softSerial.write(".");
    }
softSerial.write(EEPROM.read(n2+i));
delay(30);
  }
  if(EEPROM.read(n2+86)==1)
  {
   softSerial.write(" gm");
delay(10);
  }
  if(EEPROM.read(n2+86)==2)
  {
  softSerial.write(" mg");
delay(10);
  }
  softSerial.write("\n");
delay(10);
  softSerial.write("Final Weight = ");
delay(10);
  for(i=64;i<69;i++)
  {
    if(i==67)
    {
      softSerial.write(".");
    }
softSerial.write(EEPROM.read(n2+i));
delay(30);
  }
 if(EEPROM.read(n2+86)==1)
  {
   softSerial.write(" gm");
delay(10);
  }
  if(EEPROM.read(n2+86)==2)
  {
  softSerial.write(" mg");
delay(10);
  }
  softSerial.write("\n");
delay(10);
softSerial.write("Weight Loss    = ");
delay(10);
for(i=69;i<72;i++)
  {
    if(i==71)
    {
      softSerial.write(".");
delay(10);
    }
 softSerial.write(EEPROM.read(n2+i));
delay(10);
  }
  softSerial.write("%");
delay(10);
 softSerial.write("\n");
delay(10);
}
else
{
  softSerial.write("Initial Weight1   = ");
delay(10);
  for(i=59;i<64;i++)
  {
    if(i==62)
    {
      softSerial.write(".");
    }
softSerial.write(EEPROM.read(n2+i));
delay(30);
  }
 if(EEPROM.read(n2+86)==1)
  {
   softSerial.write(" gm");
delay(10);
  }
  if(EEPROM.read(n2+86)==2)
  {
  softSerial.write(" mg");
delay(10);
  }
  softSerial.write("\n");
delay(10);
  softSerial.write("Final Weight1     = ");
delay(10);
  for(i=64;i<69;i++)
  {
    if(i==67)
    {
      softSerial.write(".");
    }
softSerial.write(EEPROM.read(n2+i));
delay(30);
  }
 if(EEPROM.read(n2+86)==1)
  {
   softSerial.write(" gm");
delay(10);
  }
  if(EEPROM.read(n2+86)==2)
  {
  softSerial.write(" mg");
delay(10);
  }
softSerial.write("\n");
delay(10);
  softSerial.write("Initial Weight2   = ");
delay(10);
  for(i=69;i<74;i++)
  {
    if(i==72)
    {
      softSerial.write(".");
    }
softSerial.write(EEPROM.read(n2+i));
delay(30);
  }
 if(EEPROM.read(n2+86)==1)
  {
   softSerial.write(" gm");
delay(10);
  }
  if(EEPROM.read(n2+86)==2)
  {
  softSerial.write(" mg");
delay(10);
  }
  softSerial.write("\n");
delay(10);
  softSerial.write("Final Weight2     = ");
delay(10);
  for(i=74;i<79;i++)
  {
    if(i==77)
    {
      softSerial.write(".");
    }
softSerial.write(EEPROM.read(n2+i));
delay(30);
  }
  if(EEPROM.read(n2+86)==1)
  {
   softSerial.write(" gm");
delay(10);
  }
  if(EEPROM.read(n2+86)==2)
  {
  softSerial.write(" mg");
delay(10);
  }
  softSerial.write("\n");
delay(10);
 softSerial.write("Weight Loss in Drum1= ");
delay(10);
for(i=79;i<82;i++)
  {
    if(i==81)
    {
      softSerial.write(".");
delay(10);
    }
 softSerial.write(EEPROM.read(n2+i));
delay(10);
  }
   softSerial.write("%");
delay(10);
 softSerial.write("\n");
delay(10);
softSerial.write("Weight Loss in Drum2= ");
delay(10);
for(i=82;i<85;i++)
  {
    if(i==84)
    {
      softSerial.write(".");
delay(10);
    }
 softSerial.write(EEPROM.read(n2+i));
delay(10);
  }
   softSerial.write("%");
delay(10);
}
softSerial.write("\n");
delay(10);
for(i=0;i<=30;i++)
{
softSerial.write("-");
  delay(10);
}
softSerial.write("\n");
delay(10);
softSerial.write("Remark:____________________");
softSerial.write("\n");
delay(10);
softSerial.write("Test: Completed");
delay(10);
softSerial.write("\n");
delay(10);
softSerial.write("Tested by:_________________");
delay(10);
softSerial.write("\n");
delay(10);
softSerial.write("\n");
delay(10);
softSerial.write("Approved by:_______________");
delay(10);
softSerial.write("\n");
delay(10);
for(i=0;i<=4;i++)
{
softSerial.write("\n");
delay(10);
}
n2=n=x=i=0;
hisdate[0]='\0';
histime[0]='\0';
hisprod[0]='\0';
hisbatch[0]='\0';
hislot[0]='\0';
  }
  EEPROM.write(3995,EEPROM.read(3995)+1);
}
 /*
  if(save_printer==2 && printd==1)
  {
    printd=0;
    SoftwareSerial softSerial(17, 16);
    softSerial.begin(19200);
for(i=0;i<=5;i++)
{
 softSerial.write("\n");
 delay(10);
}
softSerial.write("Print Date    :");
delay(50);
for(i=0;i<=11;i++)
{
softSerial.write(rtc_string[i]);
delay(10);
}
softSerial.write("\n");
delay(10);
softSerial.write("Print Time    :");
delay(50);
for(i=12;i<=19;i++)
{
softSerial.write(rtc_string[i]);
delay(10);
}
softSerial.write("\n");
delay(10);
for(i=0;i<=60;i++)
{
softSerial.write("-");
  delay(10);
}
 softSerial.write("\n");
 delay(10);
softSerial.write("        Test Date  :");
  delay(50);
 for(i=0;i<=11;i++)
{
softSerial.write(hisdate[i]);
delay(10);
} 
  for(i=0;i<=22;i++)
{
softSerial.write(" ");
  delay(10);
}
  softSerial.write("Drum Type     :");
  delay(50);
  if(EEPROM.read(n2+51)==1)
{
softSerial.write("Friability");
delay(10);
}
if(EEPROM.read(n2+51)==2)
{
  softSerial.write("Abration");
delay(10);
}
  softSerial.write("\n");
 delay(10);
 softSerial.write("        Test Time :");
  delay(10);
  for(i=12;i<=19;i++)
{
softSerial.write(histime[i-12]);
delay(10);
}
   for(i=0;i<=23;i++)
{
softSerial.write(" ");
  delay(10);
}
  softSerial.write("No. of Drums  :");
  delay(50);
  softSerial.write("\n");
 delay(10);
 
softSerial.write(EEPROM.read(n2+50)+'0');
delay(50);

  softSerial.write("\n");
delay(10);
softSerial.write("Selected Mode:");
delay(50);
if(EEPROM.read(n2+54)==2)
{
softSerial.write("Counts");
delay(10);
softSerial.write("\n");
delay(10);
softSerial.write("Counts       = ");
delay(10);
for(i=55;i<58;i++)
{
softSerial.write( EEPROM.read(n2+i));
delay(10);
}
}
else
{
  softSerial.write("Time");
delay(10);
softSerial.write("\n");
delay(10);
softSerial.write("Time=");
delay(10);
for(i=55;i<58;i++)
{
softSerial.write( EEPROM.read(n2+i));
delay(10);
}
}
softSerial.write("\n");
delay(10);
for(i=0;i<=30;i++)
{
softSerial.write("-");
delay(10);
}

  softSerial.write("\n");
 delay(10);
  softSerial.write("        Product Name :");
  delay(50);
  for(i=20;i<=29 || hisprod[i-29]!='\0';i++)
{
softSerial.write(hisprod[i-29]);
delay(10);
}
softSerial.write("\n");
 delay(10);
  softSerial.write("        Batch Number :");
  delay(50);
  for(i=30;i<=34 || hisbatch[i-30]!='\0';i++)
{
softSerial.write(hisbatch[i-30]);
delay(10);
}
softSerial.write("\n");
 delay(10);
  softSerial.write("        Lot Number   :");
  delay(10);
  for(i=30;i<=34 || hislot[i-30]!='\0';i++)
{
softSerial.write(hislot[i-30]);
delay(10);
}
   for(i=0;i<=60;i++)
{
softSerial.write("-");
  delay(10);
}
softSerial.write("\n");
delay(10);
if(EEPROM.read(n2+50)==1)
{
  softSerial.write("         initial weight= ");
delay(10);
  for(i=59;i<64;i++)
  {
    if(i==62)
    {
      softSerial.write(".");
    }
softSerial.write(EEPROM.read(n2+i));
delay(30);
  }
   softSerial.write(" mg/gm");
delay(10);
  softSerial.write("\n");
delay(10);
  softSerial.write("         final weight = ");
delay(10);
  for(i=64;i<69;i++)
  {
    if(i==67)
    {
      softSerial.write(".");
    }
softSerial.write(EEPROM.read(n2+i));
delay(30);
  }
  softSerial.write(" mg/gm");
delay(10);
  softSerial.write("\n");
delay(10);
softSerial.write("            Weight Loss    = ");
delay(10);
for(i=69;i<72;i++)
  {
    if(i==71)
    {
      softSerial.write(".");
delay(10);
    }
 softSerial.write(EEPROM.read(n2+i));
delay(10);
  }
  softSerial.write("%");
delay(10);
 softSerial.write("\n");
delay(10);
}
else
{
  softSerial.write("             initial weight1   = ");
delay(10);
  for(i=59;i<64;i++)
  {
    if(i==62)
    {
      softSerial.write(".");
    }
softSerial.write(EEPROM.read(n2+i));
delay(30);
  }
  softSerial.write("mg/gm");
delay(10);
  softSerial.write("\n");
delay(10);
  softSerial.write("             initial weight2   = ");
delay(10);
  for(i=64;i<69;i++)
  {
    if(i==67)
    {
      softSerial.write(".");
    }
softSerial.write(EEPROM.read(n2+i));
delay(30);
  }
  softSerial.write(" mg/gm");
delay(10);
softSerial.write("\n");
delay(10);
  softSerial.write("               final weight1     = ");
delay(10);
  for(i=69;i<74;i++)
  {
    if(i==72)
    {
      softSerial.write(".");
    }
softSerial.write(EEPROM.read(n2+i));
delay(30);
  }
  softSerial.write(" mg/gm");
delay(10);
  softSerial.write("\n");
delay(10);
  softSerial.write("                final weight2     = ");
delay(10);
  for(i=74;i<79;i++)
  {
    if(i==77)
    {
      softSerial.write(".");
    }
softSerial.write(EEPROM.read(n2+i));
delay(30);
  }
  softSerial.write(" mg/gm");
delay(10);
  softSerial.write("\n");
delay(10);
 softSerial.write("                 Weight Loss in drum1= ");
delay(10);
for(i=79;i<82;i++)
  {
    if(i==81)
    {
      softSerial.write(".");
delay(10);
    }
 softSerial.write(EEPROM.read(n2+i));
delay(10);
  }
   softSerial.write("%");
delay(10);
 softSerial.write("\n");
delay(10);
softSerial.write("                   Weight Loss in drum2= ");
delay(10);
for(i=82;i<85;i++)
  {
    if(i==84)
    {
      softSerial.write(".");
delay(10);
    }
 softSerial.write(EEPROM.read(n2+i));
delay(10);
  }
   softSerial.write("%");
delay(10);
}
for(i=0;i<=4;i++)
{
softSerial.write("\n");
delay(10);
}
n2=n=x=i=0;
hisdate[0]='\0';
histime[0]='\0';
hisprod[0]='\0';
hisbatch[0]='\0';
hislot[0]='\0';
  }
  */
}

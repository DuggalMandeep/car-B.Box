#include <SoftwareSerial.h>



#include <TinyGPS.h>

TinyGPS gps;
SoftwareSerial ss(2,3);
SoftwareSerial GSM(7,8); // RX, TX
int als2;
float llat,llon,lspeed;
char *ll;
double x;
double y;
double z;
float ttx=0,tty=0,ttz=0,acc=0;
enum _parseState {
  PS_DETECT_MSG_TYPE,

  PS_IGNORING_COMMAND_ECHO,

  PS_HTTPACTION_TYPE,
  PS_HTTPACTION_RESULT,
  PS_HTTPACTION_LENGTH,

  PS_HTTPREAD_LENGTH,
  PS_HTTPREAD_CONTENT
};

enum _actionState {
int aaaaaaaaaaaaaaaaaaa;
  AS_IDLE,
  AS_WAITING_FOR_RESPONSE
};

byte actionState = AS_IDLE;
unsigned long lastActionTime = 0;

byte parseState = PS_DETECT_MSG_TYPE;
char buffer[380];
byte pos = 0;

int contentLength = 0;

void resetBuffer() {

  memset(buffer, 0, sizeof(buffer));
  pos = 0;
}

void sendGSM(const char* msg, int waitMs = 500) {
  
  GSM.println(msg);
  while(GSM.available()) {
    parseATText(GSM.read());
  }
  delay(waitMs);
}
void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println("Latitude  Longitude   Date   Speed   Coordinates");
  Serial.println(" (deg)     (deg)                     x    y    z");
  Serial.println("------------------------------------------------");
  ss.begin(9600);
  GSM.begin(9600);als2=0;
pinMode(5,OUTPUT);
pinMode(6,OUTPUT);
digitalWrite(5,HIGH);

}

void acid () {
  // put your main code here, to run repeatedly:
float ttx1=ttx,tty1=tty,ttz1=ttz;

int minVal = 265;
int maxVal = 402;

ttx=analogRead(A0);
tty=analogRead(A1);
ttz=analogRead(A2);

  int xAng = map(ttx, minVal, maxVal, -90, 90);
  int yAng = map(tty, minVal, maxVal, -90, 90);
  int zAng = map(ttz, minVal, maxVal, -90, 90);

  ttx = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  tty = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  ttz = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

if((ttx>ttx1+10 || ttx>ttx1+10 || ttx>ttx1+10 )&& (ttx1!=0&&tty1!=0&&ttz1!=0 ))
{//Serial.println(ttx);Serial.print(" ");Serial.print(tty);Serial.print(" ");Serial.print(ttz);
  Serial.println("accident");
acc++;
}
if((ttx>ttx1+30 || ttx>ttx1+30 || ttx>ttx1+30 )&& (ttx1!=0&&tty1!=0&&ttz1!=0 ))
{//Serial.println(ttx);Serial.print(" ");Serial.print(tty);Serial.print(" ");Serial.print(ttz);
  Serial.println("accident");
acc += 3;
}
else
{
  Serial.println("no accident");
}
}

void loop()
{
  ss.listen();
  l1();
  GSM.listen();
  l2();
  
}
void l2()
{   acid();
  unsigned long now = millis();
char a[10];
     String g="AT+HTTPPARA=\"URL\",\"http://www.epoque-kiet.com/blackbox/blackbox.php?latitude=";
    int als1=als2;
int alv=0;
als2=analogRead(A3);
Serial.print(als1);
Serial.print(" ");
Serial.print(als2);
Serial.print(" ");

if(als1-als2>100 || als2-als1>100 || als2 >= 300)
{
 digitalWrite(6,LOW);alv=1;
}
else
{
  digitalWrite(6,HIGH);alv=0;
}
Serial.println(alv);

    dtostrf(llat,2,6,a);
            g+=a;g+="&longitude=";
    dtostrf(llon,2,6,a);
            g+=a;g+="&speed=";
      //      Serial.println(g);
    dtostrf(lspeed,2,2,a);
    //Serial.println(a);
            g+=a;g+="&x_cord=";
    dtostrf(x,2,2,a);
            g+=a;g+="&y_cord=";
    dtostrf(y,2,2,a);
            g+=a;g+="&z_cord=";
    dtostrf(z,2,2,a);//g+="id=1";
        g+=a;g+="&alv=";
    dtostrf(alv,2,2,a);//g+="id=1";
        g+=a;g+="&id=1";
            if(acc>1)
            {
              g+="&acc=1";
              Serial.println("accident occur");
            }
            else
            {
              g+="&acc=0";
              Serial.println("accident not occur");
            }
        g+="\"";//g+='\n';
char *ll = const_cast<char*>(g.c_str());
acc=0;


    if ( now > lastActionTime + 5000 ) {
  //      sendGSM("AT+SAPBR=3,1,\"APN\",\"Tata.Docomo.Internet\"");  
  sendGSM("AT+SAPBR=3,1,\"APN\",\"wap.cingular\"");  acid();
  sendGSM("AT+SAPBR=1,1",3000);acid();
  sendGSM("AT+HTTPINIT");  
  sendGSM("AT+HTTPPARA=\"CID\",1");
  //Serial.println(ll); Serial.println(ll);
delay(500);
  sendGSM(ll,3000);
   //   sendGSM("AT+HTTPPARA=\"URL\",\"http://www.iforce2d.net/test.php\"");
      sendGSM("AT+HTTPACTION=0");
      lastActionTime = now;
      actionState = AS_WAITING_FOR_RESPONSE;
   // }
  }
  
  while(GSM.available()) {
    lastActionTime = now;
    parseATText(GSM.read());
  }
}
void l1()
{acid();
  smartdelay(1000);
  float flat, flon;
  acid();
  unsigned long age, date, time, chars = 0;
  unsigned short sentences = 0, failed = 0;
acid();
  gps.f_get_position(&flat, &flon, &age);
acid();
  if(flat>0.0&&flat!=1000)
  {
  llat=flat;
  Serial.print(llat,6);
  }
  if(flon>0.0&&flon!=1000)
  {
  llon=flon;
  Serial.print(llon,6);
  }print_date(gps);

  lspeed = gps.f_speed_kmph();
  Serial.print(lspeed,2);
acid();
int minVal = 265;
int maxVal = 402;

  int xRead = analogRead(A1);
  int yRead = analogRead(A2);
  int zRead = analogRead(A3);

  int xAng = map(xRead, minVal, maxVal, -90, 90);
  int yAng = map(yRead, minVal, maxVal, -90, 90);
  int zAng = map(zRead, minVal, maxVal, -90, 90);

  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

 Serial.print("    ");
  Serial.print(x);
  Serial.print("  ");
  Serial.print(y);
  Serial.print("  ");
  Serial.print(z);
  Serial.println();
acid();
}

static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void print_date(TinyGPS &gps)
{
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPS::GPS_INVALID_AGE)
    {Serial.print("*** *** ");
  //  ldate="*** ***";
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ",
        month, day, year);
    //Serial.print(sz);
    sz;  Serial.print(sz);
  }

  smartdelay(0);
}

void parseATText(byte b) {
  buffer[pos++] = b;

  if ( pos >= sizeof(buffer) )
    {

      resetBuffer(); // just to be safe
    }
  switch (parseState) {
  case PS_DETECT_MSG_TYPE: 
    {
            //Serial.println((char)b);
      if ( b == '\n' )
        resetBuffer();
      else {        
        if ( pos == 3 && strcmp(buffer, "AT+") == 0 ) {
          parseState = PS_IGNORING_COMMAND_ECHO;
        }
        else if ( b == ':' ) {
  //        Serial.print("Checking message type: ");
   //       Serial.println(buffer);

          if ( strcmp(buffer, "+HTTPACTION:") == 0 ) {
            Serial.println("Received HTTPACTION");
            parseState = PS_HTTPACTION_TYPE;
          }
          else if ( strcmp(buffer, "+HTTPREAD:") == 0 ) {
            Serial.println("Received HTTPREAD");            
            parseState = PS_HTTPREAD_LENGTH;
          }
          resetBuffer();
        }
      }
    }
    break;

  case PS_IGNORING_COMMAND_ECHO:
    {
      if ( b == '\n' ) {
        Serial.print("Ignoring echo: ");
        Serial.println(buffer);
        parseState = PS_DETECT_MSG_TYPE;
        resetBuffer();
      }
    }
    break;

  case PS_HTTPACTION_TYPE:
    {
      if ( b == ',' ) {
        Serial.print("HTTPACTION type is ");
        Serial.println(buffer);
        parseState = PS_HTTPACTION_RESULT;
        resetBuffer();
      }
    }
    break;

  case PS_HTTPACTION_RESULT:
    {
      if ( b == ',' ) {
        Serial.print("HTTPACTION result is ");
        Serial.println(buffer);
        parseState = PS_HTTPACTION_LENGTH;
        resetBuffer();
      }
    }
    break;

  case PS_HTTPACTION_LENGTH:
    {
      if ( b == '\n' ) {
        Serial.print("HTTPACTION length is ");
        Serial.println(buffer);
        
        // now request content
        GSM.print("AT+HTTPREAD=0,");
        GSM.println(buffer);
        
        parseState = PS_DETECT_MSG_TYPE;
        resetBuffer();
      }
    }
    break;

  case PS_HTTPREAD_LENGTH:
    {
      if ( b == '\n' ) {
        contentLength = atoi(buffer);
        Serial.print("HTTPREAD length is ");
        Serial.println(contentLength);
        
        Serial.print("HTTPREAD content: ");
        
        parseState = PS_HTTPREAD_CONTENT;
        resetBuffer();
      }
    }
    break;

  case PS_HTTPREAD_CONTENT:
    {
      // for this demo I'm just showing the content bytes in the serial monitor
      Serial.write(b);
      
      contentLength--;
      
      if ( contentLength <= 0 ) {

        // all content bytes have now been read

        parseState = PS_DETECT_MSG_TYPE;
        resetBuffer();
        
        Serial.print("\n\n\n");
        
        actionState = AS_IDLE;
        
      }
    }
    break;
  }
}

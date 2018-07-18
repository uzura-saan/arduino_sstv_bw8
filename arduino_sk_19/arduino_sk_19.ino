/********************************************************
  SSTV WIRELESS CAMERA @uzura-saan
  Robot B/W8 Format (160x120)
  2017.07.19 v2.2
  ARDUINO "DUE" + ETHERNET SHIELD R3 + AD9850 DDS
  ETHERNET SHIELD ACTS JUST AS A "SD CARD SHIELD"
  --- THIS CODE IS IN THE PUBLIC DOMAIN --- 
  
  JPEG DECODE SECTION IS BASED ON MAKOTO KURAUCHI'S WORK
  https://github.com/MakotoKurauchi/JPEGDecoder
/********************************************************/

#include <arduino.h>
#include <SPI.h>
#include <SD.h>
#include <DueTimer.h>
#include "JPEGDecoder.h"

File myFile;
#define chipSelect 4

#define  CLK   6
#define  FQ    7
#define  DATA  8
#define  RST   9

String string1 = String("S");
String string2 = String("");
String string3 = String(".JPG");
String string4 = String("");
char fileName[8];
byte fileNumber;

volatile byte  sTimes;
volatile byte  sSq;
volatile int   line;
volatile int   ti;

const boolean vox = 1; //VOX TONE ENABLE

char charId[10] = "SSTV CAM "; // ***** INFORMATION HEADER: MAX 9 CAHARCTERS *****

//FONTS
const uint8_t font_a[]   = {0x00, 0x18, 0x24, 0x62, 0x62, 0x62, 0x7E, 0x62, 0x62, 0x62, 0x00};
const uint8_t font_b[]   = {0x00, 0x7C, 0x32, 0x32, 0x32, 0x3C, 0x32, 0x32, 0x32, 0x7C, 0x00};
const uint8_t font_c[]   = {0x00, 0x3C, 0x62, 0x62, 0x60, 0x60, 0x60, 0x62, 0x62, 0x3C, 0x00};
const uint8_t font_d[]   = {0x00, 0x7C, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x7C, 0x00};
const uint8_t font_e[]   = {0x00, 0x7E, 0x60, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x60, 0x7E, 0x00};
const uint8_t font_f[]   = {0x00, 0x7E, 0x60, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x60, 0x60, 0x00};
const uint8_t font_g[]   = {0x00, 0x3C, 0x62, 0x62, 0x60, 0x60, 0x66, 0x62, 0x62, 0x3C, 0x00};
const uint8_t font_h[]   = {0x00, 0x62, 0x62, 0x62, 0x62, 0x7E, 0x62, 0x62, 0x62, 0x62, 0x00};
const uint8_t font_i[]   = {0x00, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00};
const uint8_t font_j[]   = {0x00, 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x4C, 0x4C, 0x4C, 0x38, 0x00};
const uint8_t font_k[]   = {0x00, 0x62, 0x64, 0x68, 0x70, 0x68, 0x64, 0x62, 0x62, 0x62, 0x00};
const uint8_t font_l[]   = {0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7E, 0x00};
const uint8_t font_m[]   = {0x00, 0x42, 0x62, 0x76, 0x6A, 0x62, 0x62, 0x62, 0x62, 0x62, 0x00};
const uint8_t font_n[]   = {0x00, 0x42, 0x62, 0x72, 0x6A, 0x66, 0x62, 0x62, 0x62, 0x62, 0x00};
const uint8_t font_o[]   = {0x00, 0x3C, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x3C, 0x00};
const uint8_t font_p[]   = {0x00, 0x7C, 0x62, 0x62, 0x62, 0x7C, 0x60, 0x60, 0x60, 0x60, 0x00};
const uint8_t font_q[]   = {0x00, 0x3C, 0x62, 0x62, 0x62, 0x62, 0x62, 0x6A, 0x6A, 0x3C, 0x08};
const uint8_t font_r[]   = {0x00, 0x7C, 0x62, 0x62, 0x62, 0x7C, 0x68, 0x64, 0x62, 0x62, 0x00};
const uint8_t font_s[]   = {0x00, 0x3C, 0x62, 0x60, 0x60, 0x3C, 0x06, 0x06, 0x46, 0x3C, 0x00};
const uint8_t font_t[]   = {0x00, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00};
const uint8_t font_u[]   = {0x00, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x3C, 0x00};
const uint8_t font_v[]   = {0x00, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x22, 0x14, 0x08, 0x00};
const uint8_t font_w[]   = {0x00, 0x62, 0x62, 0x62, 0x62, 0x62, 0x6A, 0x76, 0x62, 0x42, 0x00};
const uint8_t font_x[]   = {0x00, 0x42, 0x62, 0x74, 0x38, 0x1C, 0x2E, 0x46, 0x42, 0x42, 0x00};
const uint8_t font_y[]   = {0x00, 0x42, 0x62, 0x74, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00};
const uint8_t font_z[]   = {0x00, 0x7E, 0x06, 0x0E, 0x0C, 0x18, 0x30, 0x70, 0x60, 0x7E, 0x00};
const uint8_t font_0[]   = {0x00, 0x3C, 0x62, 0x62, 0x66, 0x6A, 0x72, 0x62, 0x62, 0x3C, 0x00};
const uint8_t font_1[]   = {0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00};
const uint8_t font_2[]   = {0x00, 0x3C, 0x46, 0x06, 0x06, 0x1C, 0x20, 0x60, 0x60, 0x7E, 0x00};
const uint8_t font_3[]   = {0x00, 0x3C, 0x46, 0x06, 0x06, 0x1C, 0x06, 0x06, 0x46, 0x3C, 0x00};
const uint8_t font_4[]   = {0x00, 0x0C, 0x1C, 0x2C, 0x4C, 0x4C, 0x7E, 0x0C, 0x0C, 0x0C, 0x00};
const uint8_t font_5[]   = {0x00, 0x7E, 0x60, 0x60, 0x60, 0x7C, 0x06, 0x06, 0x46, 0x3C, 0x00};
const uint8_t font_6[]   = {0x00, 0x3C, 0x62, 0x60, 0x60, 0x7C, 0x62, 0x62, 0x62, 0x3C, 0x00};
const uint8_t font_7[]   = {0x00, 0x7E, 0x06, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00};
const uint8_t font_8[]   = {0x00, 0x3C, 0x62, 0x62, 0x62, 0x3C, 0x62, 0x62, 0x62, 0x3C, 0x00};
const uint8_t font_9[]   = {0x00, 0x3C, 0x46, 0x46, 0x46, 0x3E, 0x06, 0x06, 0x46, 0x3C, 0x00};
const uint8_t font_slu[] = {0x00, 0x00, 0x02, 0x06, 0x0E, 0x1C, 0x38, 0x70, 0x60, 0x40, 0x00};
const uint8_t font_das[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x7E, 0x00, 0x00, 0x00, 0x00};
const uint8_t font_dot[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x00};
const uint8_t font_que[] = {0x00, 0x3C, 0x46, 0x06, 0x06, 0x0C, 0x10, 0x00, 0x30, 0x30, 0x00};
const uint8_t font_eks[] = {0x00, 0x18, 0x18, 0x18, 0x18, 0x10, 0x10, 0x00, 0x18, 0x18, 0x00};
const uint8_t font_col[] = {0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00};
const uint8_t font_spa[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

uint8_t frameBuf[19200]; //160*120

void shortPulse (char PIN)
{
        digitalWrite(PIN, 1);
        digitalWrite(PIN, 0);
}

void setFreq(double freq)
{
        //--calculate
        int32_t d_Phase = freq * pow(2, 32) / 125000000;
        //--send first 32bit
        for (int i=0; i<32; i++, d_Phase>>=1)
        {
                if(d_Phase & 1 == 1)
                {
                        digitalWrite(DATA, HIGH); //--data
                }
                else
                {
                        digitalWrite(DATA, LOW); //--data
                }
                shortPulse(CLK);
        }
        //--send rest 8bit
        digitalWrite(DATA, LOW); //--data
        for (int i=0; i<8; i++)
        {
                shortPulse(CLK);
        }
        //--finish
        shortPulse(FQ);
}

void setup()
{
        char ch;
        
        Serial.begin(9600);
        Serial1.begin(38400); //Camera Serial
        delay(300);
        while(Serial.available())
        {
                ch = Serial.read(); //flush
        }
        while(Serial1.available())
        {
                ch = Serial1.read(); //flush
        }
        Serial.println("SERIAL READY");
        
        pinMode(10, OUTPUT);
        pinMode(31, OUTPUT); //ERROR LAMP
        pinMode(33, OUTPUT); //SHOT LAMP
        digitalWrite(31, LOW);
        digitalWrite(33, LOW);
        
        if (!SD.begin(chipSelect))
        {
                Serial.println("ERROR: INSERT SD CARD AND RESTART");
                digitalWrite(31, HIGH);
                return ;
        }
        Serial.println("SD CARD INITIALIZED");
        Serial.println();
        
        pinMode(47, INPUT);
        pinMode(49, INPUT);
        digitalWrite(47, 1); //INTERNAL PULL UP
        digitalWrite(49, 1); //INTERNAL PULL UP
        
        pinMode(CLK, OUTPUT);
        pinMode(FQ, OUTPUT);
        pinMode(DATA, OUTPUT);
        pinMode(RST, OUTPUT);
        //--dds reset
        shortPulse(RST);
        shortPulse(CLK);
        //--change mode
        shortPulse(FQ);
        
        //--cam reset
        Serial1.print((char)0x56);
        Serial1.print((char)0x00);
        Serial1.print((char)0x26);
        Serial1.print((char)0x00);
        delay(2500);
        
        while(!Serial1.available()){ }
        while(Serial1.available())
        {
                Serial.print(Serial1.read(), HEX);
                Serial.print(" ");
        }
        Serial.println();
        
        //--cam setSize
        Serial1.print((char)0x56);
        Serial1.print((char)0x00);
        Serial1.print((char)0x54);
        Serial1.print((char)0x01);
        Serial1.print((char)0x22); //160*120
        delay(100);
        
        while(!Serial1.available()){ }
        while(Serial1.available())
        {
                Serial.print(Serial1.read(), HEX);
                Serial.print(" ");
        }
        Serial.println();
        
        //--cam compRatio
        Serial1.print((char)0x56);
        Serial1.print((char)0x00);
        Serial1.print((char)0x31);
        Serial1.print((char)0x05);
        Serial1.print((char)0x01);
        Serial1.print((char)0x01);
        Serial1.print((char)0x12);
        Serial1.print((char)0x04);
        Serial1.print((char)0x0F); //SMALLER VALUE: LESS COMPRESSED (DEFAULT: 0x36)
        delay(100);
        
        while(!Serial1.available()){ }
        while(Serial1.available())
        {
                Serial.print(Serial1.read(), HEX);
                Serial.print(" ");
        }
        Serial.println();
        
        //--cam motionDetectEnable
        Serial1.print((char)0x56);
        Serial1.print((char)0x00);
        Serial1.print((char)0x42);
        Serial1.print((char)0x03);
        Serial1.print((char)0x00);
        Serial1.print((char)0x01);
        Serial1.print((char)0x01);
        delay(100);
        
        while(!Serial1.available()){ }
        while(Serial1.available())
        {
                Serial.print(Serial1.read(), HEX);
                Serial.print(" ");
        }
        Serial.println();
        
        Serial.println("JPEG CAMERA INITIALIZED");
        Serial.println();
        
        Timer1.attachInterrupt(timer1_interrupt).start(352); // ***** 354(uS/px) +/- SLANT ADJUST *****
        delay(100);
}

void timer1_interrupt(void)
{
        if(sSq == 3)
        {
                if(ti < 160)
                {
                        int p = (160 * line) + ti;
                        setFreq(1500 + 3.13 * frameBuf[p]);
                }
                else if(ti == 160)
                {
                        line++;
                        sSq = 2;
                }
                if(line == 120)
                {
                        sSq = 4;
                }
                ti++;
        }
}

void jpegTake(void)
{
        byte start_addr_m;
        byte start_addr_l;
        boolean jpegEnd;
        int jpegEndPos;
        int byteCount;
        byte incomingByte;
        byte jpegBuf[25000];
        boolean err;
        int i;
        
        Serial.println("NEW FILE");
        
        //--cam takePhoto
        digitalWrite(33, HIGH);
        Serial1.print((char)0x56);
        Serial1.print((char)0x00);
        Serial1.print((char)0x36);
        Serial1.print((char)0x01);
        Serial1.print((char)0x00);
        delay(100);
        
        while(!Serial1.available()){ }
        digitalWrite(33, LOW);
        while(Serial1.available())
        {
                Serial.print(Serial1.read(), HEX);
                Serial.print(" ");
        }
        Serial.println();
        
        //--cam readSize
        Serial1.print((char)0x56);
        Serial1.print((char)0x00);
        Serial1.print((char)0x34);
        Serial1.print((char)0x01);
        Serial1.print((char)0x00);
        delay(100);
        
        while(!Serial1.available()){ }
        while(Serial1.available())
        {
                Serial.print(Serial1.read(), HEX);
                Serial.print(" ");
        }
        Serial.println();
        
        //--cam readContent
        start_addr_m = 0x00;
        start_addr_l = 0x00;
        jpegEnd = false;
        i = 0;
        
        while(!jpegEnd)
        {
                /*
                Serial.print("start_addr: ");
                Serial.print(start_addr_m, HEX);
                Serial.print(" ");
                Serial.print(start_addr_l, HEX);
                Serial.println();
                delay(10);
                */
                Serial1.print((char)0x56);
                Serial1.print((char)0x00);
                Serial1.print((char)0x32);
                Serial1.print((char)0x0C);
                Serial1.print((char)0x00);
                Serial1.print((char)0x0A);
                Serial1.print((char)0x00);
                Serial1.print((char)0x00);
                
                Serial1.print((char)start_addr_m);  //MM
                Serial1.print((char)start_addr_l);  //MM
                
                Serial1.print((char)0x00);
                Serial1.print((char)0x00);
                Serial1.print((char)0x00);  //KK
                Serial1.print((char)0x20);  //KK (DEC:32)
                Serial1.print((char)0x00);  //XX
                Serial1.print((char)0x0A);  //XX
                
                while(!Serial1.available()){ }
                delay(22); //10ms at least
                
                byteCount = 0;
                while(Serial1.available())
                {
                        incomingByte = Serial1.read();
                        /*
                        Serial.print(incomingByte, HEX);
                        Serial.print(" ");
                        */
                        if(byteCount >=5 && byteCount <=36)
                        {
                                jpegBuf[i] = incomingByte;
                                
                                if(jpegBuf[i - 1] == 0xFF && jpegBuf[i] == 0xD9)
                                {
                                        /*
                                        Serial.println("CAUGHT FFD9!");
                                        */
                                        jpegEndPos = i;
                                        jpegEnd = true;
                                }
                                i++;
                        }
                        byteCount++;
                }
                /*
                Serial.println();
                */
                if(start_addr_l == 0xE0)
                {
                        start_addr_l = 0x00;
                        start_addr_m += 0x01;
                }
                else
                {
                        start_addr_l += 0x20; //DEC:32
                }
        }
        
        //--cam stop
        Serial1.print((char)0x56);
        Serial1.print((char)0x00);
        Serial1.print((char)0x36);
        Serial1.print((char)0x01);
        Serial1.print((char)0x03);
        delay(100);
        
        while(!Serial1.available()){ }
        while(Serial1.available())
        {
                Serial.print(Serial1.read(), HEX);
                Serial.print(" ");
        }
        Serial.println();
        
        //--jpeg fileSave
        string2 = String(fileNumber);
        string4 = String(string1 + string2 + string3);
        string4.toCharArray(fileName, 8);
        
        err = false;
        myFile = SD.open(fileName, FILE_WRITE | O_TRUNC);
        if(myFile)
        {
                for(i = 0; i <= jpegEndPos; i++)
                {
                        myFile.write(jpegBuf[i]);
                }
        }
        else
        {
                Serial.println("CAN'T OPEN FILE");
                err = true;
        }
        myFile.close();
        
        if(err == true)
        {
                digitalWrite(31, HIGH);
                while(1);
        }
        else
        {
                Serial.println(fileName);
                Serial.println("JPEG FILE SAVED");
                Serial.println();
        }
}

void jpegDecode(void)
{
        char str[100];
        //char filename[] = "SSTV.JPG";
        uint8 *pImg;
        int x,y,bx,by;
        
        float   pxGr;
        uint8_t pxGr_;
        int i;
        
        // Decoding start
        JpegDec.decode(fileName,0);
        
        // Image Information
        Serial.print("Width     :");
        Serial.println(JpegDec.width);
        Serial.print("Height    :");
        Serial.println(JpegDec.height);
        Serial.print("Components:");
        Serial.println(JpegDec.comps);
        Serial.print("MCU / row :");
        Serial.println(JpegDec.MCUSPerRow);
        Serial.print("MCU / col :");
        Serial.println(JpegDec.MCUSPerCol);
        Serial.print("Scan type :");
        Serial.println(JpegDec.scanType);
        Serial.print("MCU width :");
        Serial.println(JpegDec.MCUWidth);
        Serial.print("MCU height:");
        Serial.println(JpegDec.MCUHeight);
        Serial.println("");
        
        // Output CSV
        sprintf(str,"#SIZE,%d,%d",JpegDec.width,JpegDec.height);
        Serial.println(str);
        
        // Image size error
        if(JpegDec.width != 160 || JpegDec.height != 120)
        {
                digitalWrite(31, HIGH);
                Serial.println("ERROR: PICTURE SIZE SHOULD BE 160*120") ;
                while(1);
        }
        
        // Clear buffer
        for(i = 0; i < 19200; i++)
        {
                frameBuf[i] = 0xFF;
        }
        
        while(JpegDec.read())
        {
                pImg = JpegDec.pImage ;
                
                for(by=0; by < JpegDec.MCUHeight; by++)
                {
                        for(bx=0; bx < JpegDec.MCUWidth; bx++)
                        {
                                x = JpegDec.MCUx * JpegDec.MCUWidth + bx;
                                y = JpegDec.MCUy * JpegDec.MCUHeight + by;
                                
                                if(x < JpegDec.width && y < JpegDec.height)
                                {
                                        if(JpegDec.comps == 1) // Grayscale
                                        {
                                                //sprintf(str,"#RGB,%d,%d,%u", x, y, pImg[0]);
                                                //Serial.println(str);
                                                
                                                frameBuf[(160 * y) + x] = pImg[0];
                                        }
                                        else // RGB
                                        {
                                                //sprintf(str,"#RGB,%d,%d,%u,%u,%u", x, y, pImg[0], pImg[1], pImg[2]);
                                                //Serial.println(str);
                                                
                                                pxGr = ((pImg[0] * 0.30) + (pImg[1] * 0.59) + (pImg[2] * 0.11));
                                                pxGr_ = (uint8_t)pxGr;
                                                frameBuf[(160 * y) + x] = pxGr_;
                                        }
                                }
                                pImg += JpegDec.comps ;
                        }
                }
        }
        //for(;;);
        Serial.println("JPEG FILE DECODED");
        Serial.println();
}

void loop()
{
        int x,y,i,p;
        
        if(sSq == 0)
        {
                byte incomingByte;
                
                setFreq(2);//Mute
                
                while(sSq == 0)
                {
                        if(digitalRead(47) == 0) //TX enabled
                        {
                                if(digitalRead(49) == 0) //Holded
                                {
                                        sTimes = 0;
                                        sSq = 1;
                                }
                                else if(sTimes > 0) //Send 3 times
                                {
                                        sSq = 1;
                                }
                                else
                                {
                                        delay(500);
                                        
                                        //Motion detection
                                        Serial1.print((char)0x56);
                                        Serial1.print((char)0x00);
                                        Serial1.print((char)0x37);
                                        Serial1.print((char)0x01);
                                        Serial1.print((char)0x01); //Start
                                        delay(100);
                                        
                                        while(!Serial1.available()){ }
                                        while(Serial1.available())
                                        {
                                                Serial.print(Serial1.read(), HEX);
                                                Serial.print(" ");
                                        }
                                        Serial.println();
                                        
                                        Serial.println("MOTION DETECTION STARTED");
                                        Serial.println();
                                        
                                        while(sSq == 0)
                                        {
                                                while(Serial1.available())
                                                {
                                                        incomingByte = Serial1.read();
                                                        if(incomingByte == 0x39)
                                                        {
                                                                Serial.println(incomingByte, HEX);
                                                                Serial.println("MOTION DETECTED!");
                                                                Serial.println();
                                                                sTimes = 3;
                                                                sSq = 1;
                                                        }
                                                }
                                                if(analogRead(3) > 850) //External trigger
                                                {
                                                        Serial.println("A3 TRIGGERED!");
                                                        Serial.println();
                                                        sTimes = 3;
                                                        sSq = 1;
                                                }
                                                
                                                delay(500);
                                                
                                                if(digitalRead(49) == 0) //Holded
                                                {
                                                        sTimes = 0;
                                                        sSq = 1;
                                                }
                                                else if(digitalRead(47) == 1) //TX disabled
                                                {
                                                        sTimes = 0;
                                                        sSq = 0;
                                                        break;
                                                }
                                        }
                                        
                                        //Motion detection
                                        Serial1.print((char)0x56);
                                        Serial1.print((char)0x00);
                                        Serial1.print((char)0x37);
                                        Serial1.print((char)0x01);
                                        Serial1.print((char)0x00); //Stop
                                        delay(100);
                                        
                                        while(!Serial1.available()){ }
                                        while(Serial1.available())
                                        {
                                                Serial.print(Serial1.read(), HEX);
                                                Serial.print(" ");
                                        }
                                        Serial.println();
                                        
                                        Serial.println("MOTION DETECTION STOPPED");
                                        Serial.println();
                                }
                        }
                        else //Disabled
                        {
                                sTimes = 0;
                                delay(5);
                        }
                }
        }
        if(sSq == 1)
        {
                jpegTake();
                jpegDecode();
                fileNumber++;
                if(fileNumber == 100)
                {
                        fileNumber = 0;
                }
                
                //Adjust brightness
                for(p = 0; p < 19200; p++)
                {
                        frameBuf[p] += 0x12;
                        if(frameBuf[p] < 0x12)
                        {
                                frameBuf[p] = 0xFF;
                        }
                }
                
                //Text overlay
                for(p = 0; p < 1760; p++)
                {
                        frameBuf[p] = 0xFF;
                }
                
                for(i = 0; i < 9; i++)
                {
                        char ch;
                        ch = charId[i];
                        
                        for(y = 0; y < 11; y++)
                        {
                                for(x = 0; x < 8; x++)
                                {
                                        p = 8 + (160 * y) + (2 * 8 * i) + (2 * x); //Width: x2
                                        
                                        uint8_t mask;
                                        mask = pow(2, 7 - x);
                                        
                                        switch(ch)
                                        {
                                                case 'A':
                                                        if((font_a[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case 'B':
                                                        if((font_b[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case 'C':
                                                        if((font_c[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case 'D':
                                                        if((font_d[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case 'E':
                                                        if((font_e[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case 'F':
                                                        if((font_f[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case 'G':
                                                        if((font_g[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case 'H':
                                                        if((font_h[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case 'I':
                                                        if((font_i[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case 'J':
                                                        if((font_j[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case 'K':
                                                        if((font_k[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case 'L':
                                                        if((font_l[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case 'M':
                                                        if((font_m[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case 'N':
                                                        if((font_n[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;  
                                                        }
                                                        break;
                                                case 'O':
                                                        if((font_o[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case 'P':
                                                        if((font_p[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case 'Q':
                                                        if((font_q[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case 'R':
                                                        if((font_r[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case 'S':
                                                        if((font_s[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case 'T':
                                                        if((font_t[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case 'U':
                                                        if((font_u[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case 'V':
                                                        if((font_v[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case 'W':
                                                        if((font_w[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case 'X':
                                                        if((font_x[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case 'Y':
                                                        if((font_y[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case 'Z':
                                                        if((font_z[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case '0':
                                                        if((font_0[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case '1':
                                                        if((font_1[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case '2':
                                                        if((font_2[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case '3':
                                                        if((font_3[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case '4':
                                                        if((font_4[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case '5':
                                                        if((font_5[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case '6':
                                                        if((font_6[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case '7':
                                                        if((font_7[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case '8':
                                                        if((font_8[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case '9':
                                                        if((font_9[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case '/':
                                                        if((font_slu[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case '-':
                                                        if((font_das[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case '.':
                                                        if((font_dot[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case '?':
                                                        if((font_que[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case '!':
                                                        if((font_eks[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case ':':
                                                        if((font_col[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                                case ' ':
                                                        if((font_spa[y] & mask) != 0)
                                                        {
                                                                frameBuf[p    ] = 0x00;
                                                                frameBuf[p + 1] = 0x00;
                                                        }
                                                        break;
                                        }
                                }
                        }
                }
                
                //--VOX TONE
                if(vox == 1)
                {
                        setFreq(1900);
                        delay(100);
                        setFreq(1500);
                        delay(100);
                        setFreq(1900);
                        delay(100);
                        setFreq(1500);
                        delay(100);
                        setFreq(2300);
                        delay(100);
                        setFreq(1500);
                        delay(100);
                        setFreq(2300);
                        delay(100);
                        setFreq(1500);
                        delay(100);
                }
                
                //--VIS CODE
                //VIS CODE for ROBOT B/W 8S is B0000010 (DECIMAL 2)
                setFreq(1900);
                delay(300);
                setFreq(1200); //BREAK
                delay(10);
                setFreq(1900);
                delay(300);
                setFreq(1200); //START BIT
                delay(30);
                setFreq(1300); //BIT 0 (LSB FIRST)
                delay(30);
                setFreq(1100); //BIT 1
                delay(30);
                setFreq(1300); //BIT 2, 3, 4, 5, 6
                delay(150);
                setFreq(1100); //EVEN PARITY
                delay(30);
                setFreq(1200); //STOP BIT
                delay(30);
                //--VIS DONE
                
                line = 0;
                sSq  = 2;
        }
        if(sSq == 2)
        {
                //--sync
                setFreq(1200);
                delayMicroseconds(10000);
                //--
                ti  = 0;
                sSq = 3;
        }
        if(sSq == 4)
        {
                Serial.println("PICTURE SENT");
                Serial.println(sTimes);
                Serial.println();
                setFreq(2);
                sSq = 0;
                
                if(sTimes > 0)
                {
                        sTimes--;
                        
                }
                else
                {
                        for(i = 0; i < 3; i++) // ***** INTERVAL (IF NECESSARY) *****
                        {
                                delay(1000);
                        }
                }
        }
}


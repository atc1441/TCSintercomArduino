const int analogInPin = A0;  

int sensorValue = 0;   
bool lastone;

volatile uint32_t CMD = 0;
volatile uint8_t lengthCMD = 0;
volatile bool cmdReady;
volatile bool sendingAktiv;

void setup() {
  Serial.begin(9600);
}

void loop() {
  sensorValue = analogRead(analogInPin);

if(sensorValue >= 400 && lastone == true){
  lastone = false; 
  analyzeSignal();
}else if (sensorValue <400 && lastone == false){
  lastone = true;
  analyzeSignal();
}
  
 if (cmdReady) {
    cmdReady = 0;
    printHEX(CMD);
    Serial.println("");
  }
}


void analyzeSignal() {
  if(sendingAktiv)return;
  static uint32_t curCMD;
  static uint32_t usLast;
  static byte curCRC;
  static byte calCRC;
  static byte curLength;
  static byte cmdIntReady;
  static byte curPos;
  uint32_t usNow = micros();
  uint32_t timeInUS = usNow - usLast;
  usLast = usNow;
  byte curBit = 4;
  if (timeInUS >= 1000 && timeInUS <= 2999) {
    curBit = 0;
  } else if (timeInUS >= 3000 && timeInUS <= 4999) {
    curBit = 1;
  } else if (timeInUS >= 5000 && timeInUS <= 6999) {
    curBit = 2;
    //curPos = 0;
  } else if (timeInUS >= 7000 && timeInUS <= 24000) {
    curBit = 3;
    curPos = 0;
  //Serial.println("");
  }

  //Serial.print(curBit);
  
  if(curPos == 1 && curBit == 2)curPos = 0;
  if (curPos == 0) {
    if (curBit == 2) {
      curPos++;
    }
    curCMD = 0;
    curCRC = 0;
    calCRC = 1;
    curLength = 0;
  } else if (curBit == 0 || curBit == 1) {
    if (curPos == 1) {
      curLength = curBit;
      curPos++;
    } else if (curPos >= 2 && curPos <= 17) {
      if (curBit)bitSet(curCMD, (curLength ? 33 : 17) - curPos);
      calCRC ^= curBit;
      curPos++;
    } else if (curPos == 18) {
      if (curLength) {
        if (curBit)bitSet(curCMD, 33 - curPos);
        calCRC ^= curBit;
        curPos++;
      } else {
        curCRC = curBit;
        cmdIntReady = 1;
      }
    } else if (curPos >= 19 && curPos <= 33) {
      if (curBit)bitSet(curCMD, 33 - curPos);
      calCRC ^= curBit;
      curPos++;
    } else if (curPos == 34) {
      curCRC = curBit;
      cmdIntReady = 1;
    }
  } else {
    curPos = 0;
  }
  if (cmdIntReady) {
    cmdIntReady = 0;
    curPos = 0;
    if (curCRC == calCRC) {
      cmdReady = 1;
      lengthCMD = curLength;
      CMD = curCMD;
     // sendeACK();
    }
    curCMD = 0;
  }
}
void printHEX(uint32_t DATA) {
  uint8_t numChars = lengthCMD ? 8 :4;
  uint32_t mask  = 0x0000000F;
  mask = mask << 4 * (numChars - 1);
  for (uint32_t i = numChars; i > 0; --i) {
    Serial.print(((DATA & mask) >> (i - 1) * 4), HEX);
    mask = mask >> 4;
  }
}

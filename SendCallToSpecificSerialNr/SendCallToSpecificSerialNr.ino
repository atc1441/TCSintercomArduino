#define outputPin 5

#define startBit 6//ms
#define oneBit 4//ms
#define zeroBit 2//ms

uint32_t serialNr = 221592;//Put here the serial number you wanna call.

void setup() {
  Serial.begin(9600);
  pinMode(outputPin, OUTPUT);
}

void loop()
{

  Serial.print("calling 221592 internaly, Command for this: ");
  printHEX(callToSerial(serialNr, 1), 1);
  sendeProtokollHEX(callToSerial(serialNr, 1), 1);
  Serial.println();
  delay(1000);
  
}

uint32_t callToSerial(uint32_t serial, byte intExtCall) {
  uint32_t curCMD = 0;
  if (serial > 0xFFFFF)serial = 0xFFFFF;
  curCMD = serial << 8;
  if (intExtCall)bitSet(curCMD, 6);
  return curCMD;

}

void sendeProtokollHEX(uint32_t protokoll, byte firstBit) {
  int length = 16;
  byte checksm = 1;
  if (firstBit) length = 32;
  digitalWrite(outputPin, HIGH);
  delay(startBit);
  digitalWrite(outputPin, !digitalRead(outputPin));
  delay(firstBit ? oneBit : zeroBit);
  int curBit = 0;
  for (byte i = length; i > 0; i--) {
    curBit = bitRead(protokoll, i - 1);
    digitalWrite(outputPin, !digitalRead(outputPin));
    delay(curBit ? oneBit : zeroBit);
    checksm ^= curBit;
  }
  digitalWrite(outputPin, !digitalRead(outputPin));
  delay(checksm ? oneBit : zeroBit);
  digitalWrite(outputPin, LOW);
}

void printHEX(uint32_t DATA, byte lengthCMD) {
  uint8_t numChars = lengthCMD ? 8 : 4;
  uint32_t mask  = 0x0000000F;
  mask = mask << 4 * (numChars - 1);
  for (uint32_t i = numChars; i > 0; --i) {
    Serial.print(((DATA & mask) >> (i - 1) * 4), HEX);
    mask = mask >> 4;
  }
}


const int analogIn = A0;

int sensorValue = 0;
float voltageDivider = 0;
float actualVoltage = 0;

bool zustand = false;

unsigned long microsFlanke = 0;

const float threshold = 15.0;

const int sequenzLaenge = 27;
const unsigned long sequenz[sequenzLaenge] = {
  5988,
1884,
1884,
1880,
2116,
3740,
2116,
1888,
3972,

1884,
2112,
1880,
1892,
1876,
2104,
1880,
2112,
1872,

3976,
2104,
6536,
2120,
2112,
2112,
2120,
4208,
2104
    };

    const byte Licht[27] = {2,2,2,2,4,2,2,4,2, 2,2,2,2,2,2,2,2, 4,2};
const unsigned long jitter = 400;
int sequenzZaehler = 0;
bool sequenzLaeuft = false;
void setup() {
    Serial.begin(115200);
        Serial.println("Start");
  pinMode(13, OUTPUT);
}

void loop() {
    sensorValue = analogRead(analogIn);
    voltageDivider = map(sensorValue, 0, 1023, 0, 320) / 100.0;
    actualVoltage = voltageDivider * 24.0 / 2.2;
    
   digitalWrite(13, HIGH);
    delay(6);
for (byte i = 0; i < 17; i = i + 1) {
    digitalWrite(13, !digitalRead(13));
    delay(Licht[i]);
}
    digitalWrite(13, LOW);

    delay(5000);
     Serial.print("Digits: ");
     Serial.println(sensorValue);
    // Serial.print(", Divider: ");
    // Serial.print(voltageDivider);
    // Serial.print(", Actual: ");
    // Serial.println(actualVoltage);
    unsigned long microsJetzt = micros();
    unsigned long microsSeitLetzterFlanke = microsJetzt - microsFlanke;
    if(actualVoltage < threshold && zustand == false)
    {
        //Spannung sinkt -> positive Flanke
        if(!sequenzLaeuft) {
            // Sequenz startet bei positiver Flanke
            // erste Zeit ist die darauf folgende negative Flanke
            sequenzLaeuft = true;
        }
        else {
            if( abs(microsSeitLetzterFlanke - sequenz[sequenzZaehler]) < jitter ) {
                sequenzZaehler++;
            }
            else {
                sequenzLaeuft = false;
                sequenzZaehler = 0;
            }
        }
        float thenew = microsSeitLetzterFlanke/1000.0;
        Serial.print("");
        Serial.print((int)thenew);
        Serial.println(",");
        zustand = true;
        microsFlanke = microsJetzt;
    }
    if(actualVoltage >= threshold && zustand == true)
    {
        //Spannung steigt -> negative Flanke
        if(sequenzLaeuft) {
            if( abs(microsSeitLetzterFlanke - sequenz[sequenzZaehler]) < jitter ) {
                sequenzZaehler++;
        Serial.println(sequenzZaehler);
            }
            else {
                sequenzLaeuft = false;
                sequenzZaehler = 0;
            }
        }
        float thenew = microsSeitLetzterFlanke/1000.0;
        Serial.print("");
        Serial.print((int)thenew);
        Serial.println(",");
        zustand = false;
        microsFlanke = microsJetzt;
    }
    if(sequenzLaeuft && sequenzZaehler >= sequenzLaenge) {
        Serial.println("Sequenz vollständig!");
        // client.publish(mqtt_topic, "triggered");
        sequenzLaeuft = false;
        sequenzZaehler = 0;

        
    }
} 


int sensorPin = A9;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor
int sensorValueOld = 0;

#define MIN_HIT_VALUE 20
#define HIT_DURATION 30
#define HIT_EXECUTE_TIME 2

#define MIDI

int maxValue = 0;
bool isHit = false;
int hitStart = 0;
bool showHitValue = false;

void setup() {
#ifndef MIDI
  Serial.begin(115200);
#endif
}

bool isFirstHit(int sensorValue)
{
  return(sensorValue >= MIN_HIT_VALUE && (!isHit));
}

void firstHit()
{
  isHit = true;
  hitStart = millis();
#ifndef MIDI
  Serial.printf("hitStart\n");
#endif
}

bool isMiddleHit()
{
  return(isHit);
}

void middleHit(int sensorValue)
{
  if (sensorValue > maxValue) maxValue = sensorValue;
  if ( (!showHitValue) && (millis() - hitStart) > HIT_EXECUTE_TIME )
  {
#ifndef MIDI
    Serial.printf("hitValue %d\n", maxValue);
#else
    int velocity = map(maxValue, 0, 600, 0, 127);
    usbMIDI.sendNoteOn(60, velocity, 1);
#endif
    showHitValue = true;
  }
}

bool isEndHit(int sensorValue)
{
  return(isHit && sensorValue < MIN_HIT_VALUE && (millis() - hitStart) >= HIT_DURATION);
}

void endHit()
{
  isHit = false;
#ifndef MIDI
  Serial.printf("maxValue %d\n", maxValue);
  Serial.printf("hitEnd\n");
#else
  usbMIDI.sendNoteOff(60, 0, 1);
#endif
  maxValue = 0;
  showHitValue = false;
}

void loop() {
  sensorValue = analogRead(sensorPin);
  if (isFirstHit(sensorValue)) firstHit();
  if (isMiddleHit()) middleHit(sensorValue);
  if (isEndHit(sensorValue)) endHit();
}

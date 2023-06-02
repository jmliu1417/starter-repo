#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

//this is just for the turn yellow github repo

AsyncDelay delay_1s;

//interrupt setup
int switchPin = 7;
int rightPin = 5;
int leftPin = 4;

volatile bool switchFlag = 0;
volatile bool rightFlag = 0;
volatile bool leftFlag = 0;

bool switchVal = 0;
bool rightVal = 0;
bool leftVal = 0;

//temp and moist variables
float moistVal;
float tempF;
float lightVal;

//map functions
int tempLED;


//MIDI setup

float midi[127];
int A_four = 440;  // a is 440 hz...

int c_major[8] = { 60, 62, 64, 65, 67, 69, 71, 72 };  // c_major scale on C4

int song[4][2] = {
  { 58, 100 },
  { 55, 100 },
  { 60, 200 },
  { 57, 200 },
};

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  generateMIDI();

  pinMode(switchPin, INPUT_PULLUP);
  pinMode(leftPin, INPUT_PULLDOWN);
  pinMode(rightPin, INPUT_PULLDOWN);

  attachInterrupt(digitalPinToInterrupt(switchPin), onOff7, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rightPin), onOff5, FALLING);
  attachInterrupt(digitalPinToInterrupt(leftPin), onOff4, FALLING);

  switchVal = digitalRead(switchPin);

  while (!Serial) {
    Serial.println("Starting timer");
  }

  delay_1s.start(2000, AsyncDelay::MILLIS);
}

void loop() {
  // put your main code here, to run repeatedly:
  moistVal = CircuitPlayground.readCap(6);
  tempF = CircuitPlayground.temperatureF();
  lightVal = CircuitPlayground.lightSensor();
  tempLED = map(tempF, 40, 100, 0, 9);

  // for (int i = 0; i < tempLED + 1; i++) {
  //   //CircuitPlayground.setPixelColor(i, 128,  128,   0);
  //   if (i % 3 == 0) {
  //     CircuitPlayground.setPixelColor(i, 255, 255, 255);
  //   } else if (i % 3 == 1) {
  //     CircuitPlayground.setPixelColor(i, 128, 0, 0);
  //   } else {
  //     CircuitPlayground.setPixelColor(i, 0, 0, 128);
  //   }
  //   delay(50);
  //   for (int i = tempLED; i >= 0; i--) {
  //     CircuitPlayground.setPixelColor(i, 0, 0, 0);
  //     //delay(50);
  //   }
  // }

  if(switchFlag){
    delay(5);
    switchVal = digitalRead(switchPin);
    switchFlag = 0;
  }
  if(switchVal){
    
    if(moistVal > 1000){
      CircuitPlayground.setPixelColor(0, 255, 0, 0);
    }else{
      for (int i = 0; i < tempLED + 1; i++) {
    //CircuitPlayground.setPixelColor(i, 128,  128,   0);
    if (i % 3 == 0) {
      CircuitPlayground.setPixelColor(i, 255, 255, 255);
    } else if (i % 3 == 1) {
      CircuitPlayground.setPixelColor(i, 128, 0, 0);
    } else {
      CircuitPlayground.setPixelColor(i, 0, 0, 128);
    }
    delay(50);
    for (int i = tempLED; i >= 0; i--) {
      CircuitPlayground.setPixelColor(i, 0, 0, 0);
      //delay(50);
    }
  }
    }
  }
  if(!switchVal){
    if (moistVal > 1000) {
    for (int i = 0; i < sizeof(song) / sizeof(song[0]); i++) {
      CircuitPlayground.playTone(midi[song[i][0]], song[i][1]);
      }
    }

    if(lightVal > 900){
      for (int i = 0; i < sizeof(song) / sizeof(song[0]); i++) {
      CircuitPlayground.playTone(midi[song[i][0]], song[i][1]);
      }
    }
  }


  if (delay_1s.isExpired() ) {
    Serial.print("TEMPERATURE = ");
    Serial.println(tempF);
    Serial.print("MOISTURE = ");
    Serial.println(moistVal);
    Serial.print("LIGHT = ");
    Serial.println(lightVal);
    Serial.println(switchFlag);
    Serial.println(switchVal);
    delay_1s.repeat();
  }
}



void onOff7() {
  switchFlag = 1;
}

void onOff5() {
  rightFlag = 1;
}

void onOff4() {
  leftFlag = 1;
}

void generateMIDI() {
  for (int x = 0; x < 127; ++x) {
    midi[x] = (A_four / 32.0) * pow(2.0, ((x - 9.0) / 12.0));
    Serial.println(midi[x]);
  }
}

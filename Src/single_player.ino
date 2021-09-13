#include <FastLED.h>
#define NUM_LEDS1 42
#define NUM_LEDS2 27
#define NUM_LEDS3 27
CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];
CRGB leds3[NUM_LEDS3];
const byte N0[12] = {2, 3, 4, 5, 6, 11, 15, 20, 21, 22, 23, 24};
const byte N1[5] = {2, 3, 4, 5, 6};
const byte N2[11] = {2, 4, 5, 6, 11, 13, 15, 20, 21, 22, 24};
const byte N3[11] = {2, 3, 4, 5, 6, 11, 13, 15, 20, 22, 24};
const byte N4[9] = {2, 3, 4, 5, 6, 13, 22, 23, 24};
const byte N5[11] = {2, 3, 4, 6, 11, 13, 15, 20, 22, 23, 24};
const byte N6[12] = {2, 3, 4, 6, 11, 13, 15, 20, 21, 22, 23, 24};
const byte N7[7] = {2, 3, 4, 5, 6, 11, 24};
const byte N8[13] = {2, 3, 4, 5, 6, 11, 13, 15, 20, 21, 22, 23, 24};
const byte N9[12] = {2, 3, 4, 5, 6, 11, 13, 15, 20, 22, 23, 24};
byte gamestate = 0;
byte start;
byte redscore = 0;
byte greenscore = 0;
byte computer_power = 0;
byte man_power = 0; 
char i = 2;
char m;
int x;
char ledposition;
bool redpush = 0;
bool greenpush = 0;
bool greenwin = 0;
bool redwin = 0;
bool dooncered = 0;
bool dooncegreen = 0;
unsigned long starttime;
unsigned long currenttime;
unsigned int waittime = 60;

void RED() {
  if (dooncered == 0) {
    switch (gamestate) {
      case 0:
        gamestate = 1;
        start = 0;
        dooncered = 1;
        break;
      case 1:
        if (start == 1) {
          redpush = 1;
        }
        dooncered = 1;
        break;
      case 2:
        redpush = 1;
        dooncered = 1;
        break;
    }
  }
}

void GREEN() {
  if (dooncegreen == 0) {
    switch (gamestate) {
      case 0:
        gamestate = 1;
        start = 1;
        dooncegreen = 1;
      case 1:
        if (start == 0) {
          greenpush = 1;
        }
        dooncegreen = 1;
        break;
      case 3:
        greenpush = 1;
        dooncegreen = 1;
        break;
    }
  }
}


void computer_play()    // computer to play...
{
  if(redscore>= 0 && redscore <= 2)
  {
    computer_power = 7;
  }
  else if(redscore>= 3 && redscore <= 4)
  {
    computer_power = 8;
  }
  else if(redscore>= 5 && redscore <= 6)
  {
    computer_power = 9;
  }
  else if(redscore>= 7 && redscore <= 8)
  {
    computer_power = 10;
  }
  else if(redscore== 9)
  {
    computer_power = 11;
  }
  else if(redscore> 9)
  {
    computer_power = 12;
  }
}



void setup() {
  FastLED.addLeds<WS2812B, 8, GRB>(leds1, NUM_LEDS1);
  FastLED.addLeds<WS2812B, 9, GRB>(leds2, NUM_LEDS2);
  FastLED.addLeds<WS2812B, 10, GRB>(leds3, NUM_LEDS3);
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(0, RED, FALLING);
}

void loop() {
  switch (gamestate) {
    case 0:
      leds1[0].setRGB(0, 0, 0);
      leds1[1].setRGB(0, 0, 0);
      
      leds1[2].setRGB(255, 0, 0);
      leds1[3].setRGB(255, 0, 0);
      leds1[4].setRGB(255, 0, 0);
      leds1[39].setRGB(255, 0, 0);
      leds1[40].setRGB(255, 0, 0);
      leds1[41].setRGB(255, 0, 0);
      FastLED.show();
      break;
    case 1:
      showscore();
      if (start == 0) {
        redtogreenstart();
      }
      else {
        greentoredstart();
      }
      break;
    case 2:
      if (ledposition > 36) {
        dooncered = 0;
        greentored();
      }
      else {
        redwin = 1;
        gamestate = 4;
      }
      break;
    case 3:
      if (ledposition < 7) {
        dooncegreen = 0;
        redtogreen();
      }
      else {
        greenwin = 1;
        gamestate = 4;
      }
      break;
    case 4:
      waittime = 60;
      if (redwin == 1) {
        redwin = 0;
        redscore++;
        
        man_power = 0; 
        
        if (redscore == 10) {
          redwinanimation();
          gamestate = 0;
          redscore = 0;
          greenscore = 0;
          dooncered = 0;
          dooncegreen = 0;
          
          
          man_power = 0;
          computer_power = 0;

          
          leds2off();
          leds3off();
        }
        else {
          leds1off();
          showscore();
          red_score();
          start = 1;
          gamestate = 1;
          dooncered = 0;
        }
      }
      if (greenwin == 1) {
        greenwin = 0;
        greenscore++;

        man_power = 0;
        
        if (greenscore == 10) {
          greenwinanimation();
          gamestate = 0;
          redscore = 0;
          greenscore = 0;
          dooncered = 0;
          dooncegreen = 0;

          man_power = 0;
          computer_power = 0;

          
          leds2off();
          leds3off();
        }
        else {
          leds1off();
          showscore();
          green_score();
          start = 0;
          gamestate = 1;
          dooncegreen = 0;
        }
      }
      break;
  }
}

void redtogreenstart() {
  dooncered = 0;
  for (i = 2; i < NUM_LEDS1; i++) {
    leds1off();
    if (i < 7) {
      leds1[i].setRGB(255, 0, 0);
    }
    if (i >= 7 && i < 12) {
      leds1[i].setRGB(255, 255, 0);
    }
    if (i >= 12 && i < 32) {
      leds1[i].setRGB(0, 255, 0);
    }
    if (i >= 32 && i < 37) {
      leds1[i].setRGB(255, 255, 0);
    }
    if (i > 36) {
      leds1[i].setRGB(255, 0, 0);
      
      computer_play(); // check for power......
    }
    if(i>38 && man_power <= computer_power)         // for computer to press button
    {
        GREEN();
    }
    FastLED.show();
    if (greenpush == 1) {
      ledposition = i;
      i = NUM_LEDS1;
      gamestate = 2;
      greenpush = 0;
    }
    pause();
    if (i == 41) {
      redwin = 1;
      gamestate = 4;
    }
  }
}

void greentoredstart() {
  dooncegreen = 0;
  for (i = 41; i > 1; i--) {
    leds1off();
    if (i < 7) {
      leds1[i].setRGB(255, 0, 0);
    }
    if (i >= 7 && i < 12) {
      leds1[i].setRGB(255, 255, 0);
    }
    if (i >= 12 && i < 32) {
      leds1[i].setRGB(0, 255, 0);
    }
    if (i >= 32 && i < 37) {
      leds1[i].setRGB(255, 255, 0);
    }
    if (i > 36) {
      leds1[i].setRGB(255, 0, 0);
    }
    FastLED.show();
    if (redpush == 1) {
      ledposition = i;
      i = 1;
      gamestate = 3;
      redpush = 0;
      
      man_power++;
    }
    pause();
    if (i == 2) {
      greenwin = 1;
      gamestate = 4;
    }
  }
}

void redtogreen() {
  if (waittime == 3) {
  }
  else {
    waittime = waittime - 3;
  }
  for (i = ledposition; i < NUM_LEDS1; i++) {
    leds1off();
    if (i < 7) {
      leds1[i].setRGB(255, 0, 0);
    }
    if (i >= 7 && i < 12) {
      leds1[i].setRGB(255, 255, 0);
    }
    if (i >= 12 && i < 32) {
      leds1[i].setRGB(0, 255, 0);
    }
    if (i >= 32 && i < 37) {
      leds1[i].setRGB(255, 255, 0);
    }
    if (i > 36) {
      leds1[i].setRGB(255, 0, 0);

      computer_play(); // check for power.......
     
    }

    
    if(i>38 && man_power <= computer_power)         // for computer to press button
    {
        GREEN();
    }

      
    FastLED.show();
    if (greenpush == 1) {
      ledposition = i;
      i = NUM_LEDS1;
      gamestate = 2;
      greenpush = 0;
    }
    pause();
    if (i == 41) {
      redwin = 1;
      gamestate = 4;
    }
  }
}

void greentored() {
  if (waittime == 3) {
  }
  else {
    waittime = waittime - 3;
  }
  for (i = ledposition; i > 1; i--) {
    leds1off();
    if (i < 7) {
      leds1[i].setRGB(255, 0, 0);
    }
    if (i >= 7 && i < 12) {
      leds1[i].setRGB(255, 255, 0);
    }
    if (i >= 12 && i < 32) {
      leds1[i].setRGB(0, 255, 0);
    }
    if (i >= 32 && i < 37) {
      leds1[i].setRGB(255, 255, 0);
    }
    if (i > 36) {
      leds1[i].setRGB(255, 0, 0);
    }
    FastLED.show();
    if (redpush == 1) {
      ledposition = i;
      i = 1;
      gamestate = 3;
      redpush = 0;

      man_power++;
    }
    pause();
    if (i == 2) {
      greenwin = 1;
      gamestate = 4;
    }
  }
}

void showscore() {
  leds2off();
  leds3off();
  switch (redscore) {
    case 0:
      for (i = 0; i < sizeof(N0); i++) {
        leds2[N0[i]].setRGB(255, 255, 255);
      }
      break;
    case 1:
      for (i = 0; i < sizeof(N1); i++) {
        leds2[N1[i]].setRGB(255, 255, 255);
      }
      break;
    case 2:
      for (i = 0; i < sizeof(N2); i++) {
        leds2[N2[i]].setRGB(255, 255, 255);
      }
      break;
    case 3:
      for (i = 0; i < sizeof(N3); i++) {
        leds2[N3[i]].setRGB(255, 255, 255);
      }
      break;
    case 4:
      for (i = 0; i < sizeof(N4); i++) {
        leds2[N4[i]].setRGB(255, 255, 255);
      }
      break;
    case 5:
      for (i = 0; i < sizeof(N5); i++) {
        leds2[N5[i]].setRGB(255, 255, 255);
      }
      break;
    case 6:
      for (i = 0; i < sizeof(N6); i++) {
        leds2[N6[i]].setRGB(255, 255, 255);
      }
      break;
    case 7:
      for (i = 0; i < sizeof(N7); i++) {
        leds2[N7[i]].setRGB(255, 255, 255);
      }
      break;
    case 8:
      for (i = 0; i < sizeof(N8); i++) {
        leds2[N8[i]].setRGB(255, 255, 255);
      }
      break;
    case 9:
      for (i = 0; i < sizeof(N9); i++) {
        leds2[N9[i]].setRGB(255, 255, 255);
      }
      break;
  }
  switch (greenscore) {
    case 0:
      for (i = 0; i < sizeof(N0); i++) {
        leds3[N0[i]].setRGB(255, 255, 255);
      }
      break;
    case 1:
      for (i = 0; i < sizeof(N1); i++) {
        leds3[N1[i]].setRGB(255, 255, 255);
      }
      break;
    case 2:
      for (i = 0; i < sizeof(N2); i++) {
        leds3[N2[i]].setRGB(255, 255, 255);
      }
      break;
    case 3:
      for (i = 0; i < sizeof(N3); i++) {
        leds3[N3[i]].setRGB(255, 255, 255);
      }
      break;
    case 4:
      for (i = 0; i < sizeof(N4); i++) {
        leds3[N4[i]].setRGB(255, 255, 255);
      }
      break;
    case 5:
      for (i = 0; i < sizeof(N5); i++) {
        leds3[N5[i]].setRGB(255, 255, 255);
      }
      break;
    case 6:
      for (i = 0; i < sizeof(N6); i++) {
        leds3[N6[i]].setRGB(255, 255, 255);
      }
      break;
    case 7:
      for (i = 0; i < sizeof(N7); i++) {
        leds3[N7[i]].setRGB(255, 255, 255);
      }
      break;
    case 8:
      for (i = 0; i < sizeof(N8); i++) {
        leds3[N8[i]].setRGB(255, 255, 255);
      }
      break;
    case 9:
      for (i = 0; i < sizeof(N9); i++) {
        leds3[N9[i]].setRGB(255, 255, 255);
      }
      break;
  }
  FastLED.show();
}

void redwinanimation() {
  leds1off();
  leds2off();
  leds3off();
  for (i = 0; i < 4; i++) {
    for (m = 0; m < NUM_LEDS2; m++) {
      if(m !=0 || m != 1 || m != 7 || m != 8 || m != 9 || m != 10 || m != 16 || m != 17 || m != 18 || m != 19 || m != 25 || m != 26 ){
        leds2[m].setRGB(255, 0, 0);
      }
    }
    for (m = 0; m < NUM_LEDS3; m++) {
      if(m !=0 || m != 1 || m != 7 || m != 8 || m != 9 || m != 10 || m != 16 || m != 17 || m != 18 || m != 19 || m != 25 || m != 26 ){
        leds3[m].setRGB(0, 0, 0);
      }
    }
    FastLED.show();
    delay(500);
    for (m = 0; m < NUM_LEDS2; m++) {
      leds2[m].setRGB(0, 0, 0);
    }
    for (m = 0; m < NUM_LEDS3; m++) {
      leds3[m].setRGB(0, 0, 0);
    }
    FastLED.show();
    delay(500);
  }
}

void greenwinanimation() {
  leds1off();
  leds2off();
  leds3off();
  for (i = 0; i < 4; i++) {
    for (m = 0; m < NUM_LEDS3; m++) {
      if(m !=0 || m != 1 || m != 7 || m != 8 || m != 9 || m != 10 || m != 16 || m != 17 || m != 18 || m != 19 || m != 25 || m != 26 ){
        leds3[m].setRGB(0, 255, 0);
      }
    }
    for (m = 0; m < NUM_LEDS2; m++) {
      if(m !=0 || m != 1 || m != 7 || m != 8 || m != 9 || m != 10 || m != 16 || m != 17 || m != 18 || m != 19 || m != 25 || m != 26 ){
        leds2[m].setRGB(0, 0, 0);
      }
    }
    FastLED.show();
    delay(500);
    for (m = 0; m < NUM_LEDS2; m++) {
      leds3[m].setRGB(0, 0, 0);
    }
    for (m = 0; m < NUM_LEDS3; m++) {
      leds2[m].setRGB(0, 0, 0);
    }
    FastLED.show();
    delay(500);
  }
}

void red_score()
{
  for(int k = 0; k < 2; k++){
    for(int j = 2; j<NUM_LEDS1 ; j++)
    {
      if(j<22){
        leds1[j].setRGB(255, 0, 0);
      }
      else
      {
        leds1[j].setRGB(0, 0, 0);
      }
    }
    FastLED.show();
    delay(500);
    for(int j = 2; j<NUM_LEDS1 ; j++)
    {
      leds1[j].setRGB(0, 0, 0);
    }
    FastLED.show();
    delay(500);
  }
}
void green_score()
{
  for(int k = 0; k< 2; k++){
    for(int j = 41; j>1 ; j--)
    {
      if(j>22){
        leds1[j].setRGB(0, 255, 0);
      }
      else
      {
        leds1[j].setRGB(0, 0, 0);
      }
    }
    FastLED.show();
    delay(500);
    for(int j = 2; j<NUM_LEDS1 ; j++)
    {
      leds1[j].setRGB(0, 0, 0);
    }
    FastLED.show();
    delay(500);
  }
  
}

void leds2off() {
  for (m = 0; m < NUM_LEDS2; m++) {
    leds2[m].setRGB(0, 0, 0);
  }
}

void leds3off() {
  for (m = 0; m < NUM_LEDS3; m++) {
    leds3[m].setRGB(0, 0, 0);
  }
}

void leds1off() {
  for (m = 0; m < NUM_LEDS1; m++) {
    leds1[m].setRGB(0, 0, 0);
  }
}

void pause() {
  starttime = millis();
  currenttime = millis();
  while (currenttime - starttime <= waittime) {
    currenttime = millis();
  }
}

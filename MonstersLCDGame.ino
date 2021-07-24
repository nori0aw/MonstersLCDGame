//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif
int gameSpeed = 400;
boolean isMonster = false ;
int address = 0;
long bestScore;
uint8_t bell[8]  = {0x4,0xe,0xe,0xe,0x1f,0x0,0x4};
uint8_t note[8]  = {0x2,0x3,0x2,0xe,0x1e,0xc,0x0};
uint8_t clock[8] = {0x0,0xe,0x15,0x17,0x11,0xe,0x0};
uint8_t heart[8] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0};
uint8_t duck[8]  = {0x0,0xc,0x1d,0xf,0xf,0x6,0x0};
uint8_t check[8] = {0x0,0x1,0x3,0x16,0x1c,0x8,0x0};
uint8_t cross[8] = {0x0,0x1b,0xe,0x4,0xe,0x1b,0x0};
uint8_t retarrow[8] = {	0x1,0x1,0x5,0x9,0x1f,0x8,0x4};
byte monster[] = {
  B01110,
  B11011,
  B10101,
  B11111,
  B10001,
  B10001,
  B10001,
  B10001
};
byte human1[8] = {
        B01110,
        B01110,
        B00100,
        B01110,
        B10101,
        B00100,
        B01010,
        B10001
};
byte human2[8] = {
        B01110,
        B01110,
        B00100,
        B01110,
        B10110,
        B00100,
        B01110,
        B01100
};
byte emptyBlock[8] = {
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000
};

int arr [32]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0};
const int buttonPin = 4; 
int buttonState = 0;   

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int score = 0;

void setup()
{
  Serial.begin(9600);
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  randomSeed(analogRead(0));

  pinMode(buttonPin, INPUT);
  
  lcd.createChar(0, emptyBlock);
  lcd.createChar(1, human1);
  lcd.createChar(2, heart);
  lcd.createChar(3, monster);

  lcd.home();
  bestScore = EEPROMReadlong(address);
  
}
boolean stopGame = false;
// display all keycodes
void loop(void) {  
  if(!stopGame ){
    Serial.println("playing game..");
    playGame();
  }
  buttonState = digitalRead(buttonPin);
  
  if (buttonState != HIGH){
    Serial.println(buttonState);
    resetGame();   
  }
  
}


void resetGame(){
  for(int i =0 ;i<32;i++){
    arr[i]=0;
    
  }
  gameSpeed = 400;
  arr[18]=1;
  score = 0 ;
  stopGame = false;
  bestScore = EEPROMReadlong(address);
}

void gameover(){
      lcd.setCursor(0,0);
      String s = String(score);
      String bs = String(bestScore);
      stopGame = true;
      if(score > bestScore){
        EEPROMWritelong(address, score);
        lcd.print("!!!Best Score!!!"+s);
        lcd.setCursor(0,1);
        lcd.print("Best score:"+s);
      }else{
        lcd.print("Your score:"+s);
        lcd.setCursor(0,1);
        lcd.print("Best score:"+bs+"  ");
      }
      
      delay(1000);
      return;  
}

void playGame(){

  boolean  TickTock  = true;
  long clkTime = millis();
  long clkObj = millis();
  while (!stopGame) {
    
   if(millis()-clkTime > 400) { // clock for 60s, then scrolls for about 30s
      
      
      clkTime = millis();
      
      if (TickTock == true){
        lcd.createChar(1, human2);
        
      }else{
        lcd.createChar(1, human1);
      }
      TickTock = !TickTock;
      //Serial.println(TickTock);
      //Serial.println(millis());
      
    }
    if(millis()-clkObj > gameSpeed) {
      clkObj = millis();
      
      for(int i =0 ;i<16;i++){
        if(arr[i] != 0 && arr[i] != 1){
          if(i == 0){arr[i] = 0;}
          else{arr[i-1] = arr[i]; arr[i] = 0; }
        
        }
      
      }
      for(int i =16 ;i<32;i++){
        if(arr[i] != 0 && arr[i] != 1){
          if(i == 16){arr[i] = 0;}
          else{arr[i-1] = arr[i]; arr[i] = 0; }
        
        }
      
      }
      generateThings();
    }
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
    // turn LED on:
      
      if(arr[18]==2){
         arr[18] = 1;
         score +=10;
         gameSpeed-=3;
      }else if(arr[18]==3){
        gameover();
        return;
      }else{
        arr[18] = 1;
        if(arr[2] != 1 ){arr[2] = arr[2];}
        else {arr[2] = 0;}
      }
      
      Serial.println(buttonState);
  } else {
    if(arr[2]==2){
         arr[2] = 1;
         score +=10;
         gameSpeed-=3;
      }else if(arr[2]==3){
        gameover();
        return;
      }else{
        arr[2] = 1;
        if(arr[18] != 1 ){arr[18] = arr[18];}
        else {arr[18] = 0;}

      }
    Serial.println(gameSpeed);
  }
  draw();
  
  }
}

void draw(){

  lcd.setCursor(0, 0);
  for(int i= 0;i<32;i++){
    if(i==16){
      lcd.setCursor(0, 1);
    }
 
    lcd.write(byte(arr[i])); 
  }

   
}

void generateThings(){
  int up;
  long randomNumber =   random(300);
  up = random(2);
  if(randomNumber > 250 && !isMonster)
  {
    isMonster = true;
    if (up == 1){
      arr[15] = 3;
    }else{
      arr[31]= 3 ;
    }
  }else{isMonster = false;}
  
  if(randomNumber < 80){
    if (up == 1){
      arr[15] = 2;
    }else{
      arr[31]= 2 ;
    }
  }
  Serial.println(randomNumber);
  
}


long EEPROMReadlong(int address) {
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);
 
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

void EEPROMWritelong(int address, long value) {
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);
 
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}

/*

Morse Code Translator

Will receive a message on the console and transmit as Morse code.

*/

int led = 13;

// all timing is derived from dit length value
int dit = 100;
int dah = 3 * dit;
int ditsep = dit;
int letsep = 3 * dit;
int wrdsep = 7 * dit;

// international morse code values
/*
byte imcA = B01000000;
byte imcB = B10001111;
byte imcC = B10101111;
byte imcD = B10011111;
byte imcE = B01111111;
byte imcF = B00101111;
byte imcG = B11011111;
byte imcH = B00001111;
byte imcI = B00111111;
byte imcJ = B01110000;
byte imcK = B10100000;
byte imcL = B01001111;
byte imcM = B11000000;
byte imcN = B10111111;
byte imcO = B11100000;
byte imcP = B01101111;
byte imcQ = B11010000;
byte imcR = B01011111;
byte imcS = B00011111;
byte imcT = B10000000;
byte imcU = B00100000;
byte imcV = B00010000;
byte imcW = B01100000;
byte imcX = B10010000;
byte imcY = B10110000;
byte imcZ = B11001111;
byte imc0 = B11111000;
byte imc1 = B01111000;
byte imc2 = B00111000;
byte imc3 = B00011000;
byte imc4 = B00001000;
byte imc5 = B00000111;
byte imc6 = B10000111;
byte imc7 = B11000111;
byte imc8 = B11100111;
byte imc9 = B11110111;
*/

String IMC[36] = {"dD","Dddd","DdDd","Ddd","d","ddDd","DDd","dddd","dd","dDDD","DdD",
                  "dDdd","DD","Dd","DDD","dDDd","DDdD","dDd","ddd","D","ddD","dddD","dDD",
                  "DddD","DdDD","DDdd","DDDDD","dDDDD","ddDDD","dddDD","ddddD","ddddd",
                  "Ddddd","DDddd","DDDdd","DDDDd"};


String message = "SOS";

void setup(){
  
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  
}

void loop(){
 
  message = "";
  while (Serial.available() > 0) {
    message += char(Serial.read());
  }
  message.toLowerCase();

  for (int x = 0; x < message.length(); x++){
    String pattern;
    int curr = int(message.charAt(x));
    
    if ((curr > 96) && (curr < 122)){
      pattern = IMC[curr - 97];
    }
    else if ((curr > 47) && (curr < 58)){
      pattern = IMC[26 + (curr - 48)];
    }
    else {//if (curr == 32){
      pattern = "X";
    }
    
    for (int y = 0; y < pattern.length(); y++){
      if (pattern.charAt(y) == 'd'){
        digitalWrite(led, HIGH);
        delay(dit);
        digitalWrite(led, LOW);
      }
      else if (pattern.charAt(y) == 'D'){
        digitalWrite(led, HIGH);
        delay(dah);
        digitalWrite(led, LOW);
      }
      else{
        delay(wrdsep);
      }
      delay(letsep);
    }
  }

  delay(2000);
}

// pins used
int led = 13;
int pr = 0;

// sensor readings
int newval = 0;
int oldval = 0;
int baseline = 0;
int threshold = 10;
int toggle = 0;

// timing info
int dit = 10;

// state of translation
boolean inletter = false;
boolean inword = false;
boolean insentence = false;
unsigned long tickcount = 0;
unsigned long tickdiff = 0;
String message = "";

/* International Morse Code values */
String IMC[36] = {"dD","Dddd","DdDd","Ddd","d","ddDd","DDd","dddd","dd","dDDD","DdD",
                  "dDdd","DD","Dd","DDD","dDDd","DDdD","dDd","ddd","D","ddD","dddD","dDD",
                  "DddD","DdDD","DDdd","DDDDD","dDDDD","ddDDD","dddDD","ddddD","ddddd",
                  "Ddddd","DDddd","DDDdd","DDDDd"};


void setup(){

    Serial.begin(9600);  
    pinMode(led, OUTPUT);
    delay(10);
    baseline = analogRead(pr);
    newval = baseline;
    tickcount = millis();
    
//    calibrate();  // work-in-progress

}

void calibrate(){
  int next = baseline;
  int last = baseline;
  int diff = 0;
  int dcount = 5;
  int tcount = 5;
  boolean on = false;
  unsigned long time = millis();
  boolean dit_calibrated = false;
  boolean thold_calibrated = false;
  boolean calibrated = false;
  
  while (!calibrated){
    next = analogRead(pr);
    diff = next - baseline;
    if (diff > threshold){
      if ((diff > (1.1 * threshold)) && (!thold_calibrated)){
        threshold += .5 * (diff - threshold);
        tcount = 5;
        //Serial.print("Threshold: ");
        //Serial.println(threshold);
      }
      else if (tcount-- < 1){
        thold_calibrated = true;
      }
      if (!on){
        on = true;
        time = millis();
      }
    }
    else if (on && (next < (1.1 * baseline))){
      on = false;
      time = millis() - time;
      if ((time > (1.1 * dit)) && (!dit_calibrated)){
        dit += .5 * (time - dit);
        dcount = 5;
        //Serial.print("Dit: ");
        //Serial.println(dit);
      }
      else if ((time >  dit) && (dcount-- < 1)){
        dit_calibrated = true;
        Serial.print("Dit: ");
        Serial.println(dit);
      }
    }
    if (thold_calibrated && dit_calibrated){
      calibrated = true;
    }
    last = next;
    delay(11);
  }
  Serial.println(dit);
  Serial.println(threshold);
}

char decode(String letter){
  int val = 0;
  int i = 0;
  for (i=0; i<36; i++){
    if (letter.compareTo(IMC[i]) == 0){
      if (i < 26){
        val = i + 65;
      }
      else{
        val = i + 48;
      }
      break;
    }
  }

  if (i < 36){
    return val;
  }
  else{
    return 42;
  }
}

void loop(){
  
  newval = analogRead(pr);
  tickdiff = millis() - tickcount;
  if (((newval - baseline) > threshold) && (!toggle)){
    if (!insentence){
//    Serial.println("In a sentence");
      insentence = true;
    }
    if (!inword){
//    Serial.println("In a word");
      inword = true;
    }
    if (!inletter){
//    Serial.println("In a letter");
      inletter = true;
    }
      toggle = 1;
      tickcount = millis();
  }
  else if ((newval - baseline) < threshold){
    if (insentence) {
      if (inword){
        if (inletter){
          if (toggle){
            toggle = 0;
            if (tickdiff > dit){
              if (tickdiff > (3 * dit)){
//              Serial.println("Dah");
                message += "D";
              }
              else {
//              Serial.println("Dit");
                message += "d";
              }
            }
            tickcount = millis();
          }
          else if (tickdiff > 3 * dit){
//          Serial.println("Leaving a letter");
            inletter = false;
            // decode and print letter here
            Serial.print(decode(message));

            if (tickdiff > 7 * dit){
//            Serial.println("Leaving a word");
              inword = false;
              // create a space on the console
              Serial.print(" ");

              if (tickdiff > 10 * dit){
//              Serial.println("Leaving a sentence");
                insentence = false;
                // create a newline on the console
                Serial.println();
              }
            }
            
            message = "";
          }
        }
        else if (tickdiff > 7 * dit){
//        Serial.println("Leaving a word");
          inword = false;
          // print a space on the console
          Serial.print(" ");

          if (tickdiff > 10 * dit){
//          Serial.println("Leaving a sentence");
            insentence = false;
            // create a newline on the console
            Serial.println();
          }
          
          message = "";
        }
      }
      else if (tickdiff > 10 * dit){
//      Serial.println("Leaving a sentence");
        insentence = false;
        // create a newline on the console
        Serial.println();
        message = "";
      }
    }  
  }


// Dynamically adjust baseline/threshold based on ambient conditions

// Add routine to set dit timing based on a test pattern from transmitter

// Figure out why newlines get printed instead of spaces between subsequent words
  
//  Try various schemes to neutralize background noise and stabilize signal
  
}

// pins the leds are attached to
int red_pin    = 11;
int yellow_pin = 10;
int green_pin  = 9;

// PWM output power
int full    = 255;
int dim     = 32;
int off     = 0;
int flash   = 256;
int fade    = 257;

int fade_step = 5; // the interval to fade at

int in_byte;      // stores the serial value
long last_tx = 0; // create a “long” variable type to hold the millisecond timer value

void setup()  {
  Serial.begin(9600);   // start Arduino serial monitor at 9600bps
  pinMode(red_pin, OUTPUT);
  pinMode(yellow_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
} 

void loop()  { 
  // check serial buffer
  if (Serial.available() > 0){
    in_byte = Serial.read();    // read serial byte
    Serial.println(in_byte);    // print serial byte
    last_tx = millis();         // set the last_tx timestamp variable to the current system timer value

    switch(in_byte) {
      case 49: // 1 is received as 49
        traffic_lights(full, off, off); // red only
        break;
      case 50: // 2 is received as 50
        traffic_lights(off, full, off); // yellow only
        break;
      case 51: // 3 is received as 51
        traffic_lights(off, off, full); // green only
        break; 
      case 52: // 4 is received as 52
        traffic_lights(flash, off, off); // red flash
        break;
      case 53: // 5 is received as 53
        traffic_lights(off, flash, off); // yellow flash
        break;
      case 54: // 6 is received as 54
        traffic_lights(off, off, flash); // green flash
        break;
      case 55: // 7 is received as 55
        traffic_lights(fade, off, off); // red fade
        break;
      case 56: // 8 is received as 56
        traffic_lights(off, fade, off); // yellow fade
        break;
      case 57: // 9 is received as 57
        traffic_lights(off, off, fade); // green fade
        break;
    }
  } else {
    if ((millis() - last_tx) > 10000) {
       // it has been more than 10 seconds (10000 milliseconds) since any serial information has been received
       // assume there is a break in the serial connection, and turn red and yellow lights on solid.
       traffic_lights(dim, dim, off);
    }
  }
}

void traffic_lights(int red_state, int yellow_state, int green_state) {
  
  // handle off states before anything else
  if(red_state == off)    analogWrite(red_pin, off);
  if(yellow_state == off) analogWrite(yellow_pin, off);
  if(green_state == off)  analogWrite(green_pin, off);

  switch(red_state) {
    case 256: // flash
      flash_pin(red_pin);
      break;
    case 257: // fade
      fade_pin(red_pin);
      break;
    default:
      analogWrite(red_pin, red_state);
      break;
  }
  
  switch(yellow_state) {
    case 256: // flash
      flash_pin(yellow_pin);
      break;
    case 257: // fade
      fade_pin(yellow_pin);
      break;
    default:
      analogWrite(yellow_pin, yellow_state);
      break;
  }
  
  switch(green_state) {
    case 256: // flash
      flash_pin(green_pin);
      break;
    case 257: // fade
      fade_pin(green_pin);
      break;
    default:
      analogWrite(green_pin, green_state);
      break;
  }
}

void flash_pin(int pin) {
  analogWrite(pin, dim);
  delay(250);
  analogWrite(pin, full);
  delay(250);
  analogWrite(pin, dim);
  delay(250);
  analogWrite(pin, off);
}


void fade_pin(int pin) {
  fade_in_pin(pin);
  delay(125);
  fade_out_pin(pin);
}


void fade_in_pin(int pin) {
  for (int i=0; i <= 255; i = i + fade_step) {
    analogWrite(pin, i);
    delay(10);
  }
  analogWrite(pin, 255); // ensure on
}

void fade_out_pin(int pin) {
  for (int i=255; i >= 0; i = i - fade_step) {
    analogWrite(pin, i);
    delay(10);
  }
  analogWrite(pin, 0); // ensure off
}

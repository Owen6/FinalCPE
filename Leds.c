volatile unsigned char *pinB =     (unsigned char *) 0x23;
volatile unsigned char *portDDRB = (unsigned char *) 0x24;
volatile unsigned char *portB =    (unsigned char *) 0x25;

void setup() {
  // put your setup code here, to run once:
  *portDDRB |= 0xF0;
}

void loop() {
  // put your main code here, to run repeatedly:
  //red 
  *portB |= 0b10000000;
  *portB &= 0b01111111;
  delay(2000);
  //Blue
  *portB |= 0b01000000;
  *portB &= 0b10111111;
  delay(2000);
  //Green
  *portB |= 0b00100000;
  *portB &= 0b11011111;
  delay(2000);
  //Yellow
  *portB |= 0b00010000;
  *portB &= 0b11101111;
  delay(2000);
}

void leds(int s){
  switch(s){
    case 0:
      //Disabled
      *portB |= 0b00010000;
      *portB &= 0b11101111;
      break;
    case 1:
      //Idle
      *portB |= 0b00100000;
      *portB &= 0b11011111;
      break;
    case 2:
      //Running
      *portB |= 0b01000000;
      *portB &= 0b10111111;
      break;
    case 3:
      //Error
      *portB |= 0b10000000;
      *portB &= 0b01111111;
      break;
    default:
      break;
  }
}
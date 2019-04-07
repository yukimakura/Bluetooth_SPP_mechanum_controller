#include <SoftwareSerial.h>
#include <Servo.h>
#define AMP 127
//#define DBGPRINT

SoftwareSerial mySerial(2, 3); // RX, TX

class mechanummover{
  private:
    typedef struct{
      int FR,FL,BR,BL;
    }WHEELPOS;
    
    WHEELPOS pwm;
    WHEELPOS dir;
    WHEELPOS pwm_pin;
    WHEELPOS dir_pin;
  public:
    mechanummover(int pfr,int pfl,int pbr,int pbl,int dfr,int dfl,int dbr,int dbl)
    {

      pwm_pin.FR = pfr;
      pwm_pin.BL = pbl;
      pwm_pin.FL = pfl;
      pwm_pin.BR = pbr;

      dir_pin.FR = dfr;
      dir_pin.BL = dbl;
      dir_pin.FL = dfl;
      dir_pin.BR = dbr;
      
      pinMode(this->dir_pin.FR,OUTPUT);
      pinMode(this->dir_pin.BL,OUTPUT);
      pinMode(this->dir_pin.FL,OUTPUT);
      pinMode(this->dir_pin.BR,OUTPUT);

      pinMode(this->pwm_pin.FR,OUTPUT);
      pinMode(this->pwm_pin.BL,OUTPUT);
      pinMode(this->pwm_pin.FL,OUTPUT);
      pinMode(this->pwm_pin.BR,OUTPUT);
    }
    void Move(float x,float y,float yaw);
};

void mechanummover::Move(float x,float y,float yaw){

  char ptr[128] = "";
  
  WHEELPOS m;
  float power = 0;
  float rad = 0.0;
  power = fabs(x)+fabs(y);
  rad = atan2(y, x);

  #ifdef DBGPRINT
  Serial.print("rad = ");
  Serial.println(rad);
  Serial.print("power = ");
  Serial.println(power);
  #endif
  
  m.FR = (char)((sin(rad)-cos(rad))*power*AMP-(yaw)*AMP);
  m.BL = (char)((sin(rad)-cos(rad))*power*AMP+(yaw)*AMP);
  m.FL = (char)((sin(rad)+cos(rad))*power*AMP+(yaw)*AMP);
  m.BR = (char)((sin(rad)+cos(rad))*power*AMP-(yaw)*AMP);

  #ifdef DBGPRINT
  sprintf(ptr,"mFR = %d ; mFL = %d ; mBR = %d ; mBL = %d ;",m.FR,m.FL,m.BR,m.BL);
  Serial.println(ptr);
  #endif
  
  this->pwm.FR = abs(m.FR);
  this->pwm.BL = abs(m.BL);
  this->pwm.FL = abs(m.FL);
  this->pwm.BR = abs(m.BR);

  this->dir.FR = m.FR > 0 ? 1 :0;
  this->dir.BL = m.BL > 0 ? 1 :0;
  this->dir.FL = m.FL > 0 ? 1 :0;
  this->dir.BR = m.BR > 0 ? 1 :0;

  digitalWrite(this->dir_pin.FR,this->dir.FR);
  digitalWrite(this->dir_pin.BL,this->dir.BL);
  digitalWrite(this->dir_pin.FL,this->dir.FL);
  digitalWrite(this->dir_pin.BR,this->dir.BR);

  analogWrite(this->pwm_pin.FR,this->pwm.FR);
  analogWrite(this->pwm_pin.BL,this->pwm.BL);
  analogWrite(this->pwm_pin.FL,this->pwm.FL);
  analogWrite(this->pwm_pin.BR,this->pwm.BR);
  
  #ifdef DBGPRINT
  sprintf(ptr,"FR = %d,%d ; FL = %d,%d ; BR = %d,%d ; BL = %d,%d ;",this->pwm.FR,this->dir.FR,this->pwm.FL,this->dir.FL,this->pwm.BR,this->dir.BR,this->pwm.BL,this->dir.BL);
  Serial.println(ptr);
  #endif
  
}int a = 0;

void setup() {
  // put your setup code here, to run once:
  mySerial.begin(9600); 
  Serial.begin(9600);
  pinMode(11,OUTPUT);
  TCCR2B &= B11111000;
  TCCR2B |= B00000011;
}

float param_x = 0.0,param_y = 0.0,param_yaw = 0.0,pwm;

void loop() {
  static char *endptr;
  double temp = 0.0;
  static mechanummover mn(5,6,9,10,4,7,8,12);
  if (mySerial.available()) {
    String str = mySerial.readStringUntil(';');
    temp = strtod(str.c_str(), &endptr);
    String tempstr = endptr;
    #ifdef DBG  
PRINT
    Serial.println(endptr);
    #endif

    if(tempstr == ",x"){
      param_x = temp;
    }else if(tempstr == ",y"){
      param_y = temp;
    }else if(tempstr == ",z"){
      param_yaw = temp;
    }else if(tempstr == ",p"){
      pwm = temp;
    }else{Serial.println("else");}

    #ifdef DBGPRINT
    Serial.print("x = ");
    Serial.print(param_x);
    Serial.print(" y = ");
    Serial.print(param_y);
    Serial.print(" z = ");
    Serial.println(param_yaw);
    #endif
    
    mn.Move(param_x,param_y,param_yaw);
    if(pwm==1.0){
        Serial.print("on");
        TCCR2B &= B11111000;
        TCCR2B |= B00000011;
        analogWrite(11,20);
        
    }else{
        TCCR2B &= B11111000;
        TCCR2B |= B00000100;
        analogWrite(11,20);
    }
    
  }

}


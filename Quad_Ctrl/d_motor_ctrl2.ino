// Individual motor writes
float motor_write2(float t, float y, float r, float p, float wT0){
  float wT = throttle(t,wT0);
  motor_LF2(y,r,p,wT);
  motor_RF2(y,r,p,wT);
  motor_LR2(y,r,p,wT);
  motor_RR2(y,r,p,wT);
  return wT;
}

void motor_LF2(float y, float r, float p, float wT){
  float wY = y*f;
  float wR = - g*r;
  float wP = - h*p;
  float w = wT + wY + wR + wP;

  if(w <= 0){
    w = 0;
  }
  motorBlink(R, w);
}

void motor_RF2(float y, float r, float p, float wT){
  float wY = - y*f;
  float wR = g*r;
  float wP = - h*p;
  float w = wT + wY + wR + wP;

  if(w <= 0){
    w = 0;
  }
  motorBlink(B, w);
}

void motor_LR2(float y, float r, float p, float wT){
  float wY = - y*f;
  float wR = - g*r;
  float wP = h*p;
  float w = wT + wY + wR + wP;

  if(w <= 0){
    w = 0;
  }
  motorBlink(Y, w);
}

void motor_RR2(float y, float r, float p, float wT){
  float wY = y*f;
  float wR = g*r;
  float wP = h*p;
  float w = wT + wY + wR + wP;

  if(w <= 0){
    w = 0;
  }  
  motorBlink(G, w);
}

float throttle2(float t, float wT0){
  float curve_mod = 1.5;
  float max_rate = 0.001;
  float dwT;
  
  if(t >= 0){
    dwT = (pow( t, curve_mod)) * max_rate;
  }
  else if(t < 0){
    dwT = -(pow( abs(t), curve_mod)) * max_rate;
  }
  float wT = wT0 + dwT;
  if(wT > e){
    wT = e;
  }
  else if(wT < 0){
    wT = 0;
  }
  return wT;
}

void motorBlink(int pin, float w){
  float interval;
  int timescale = 100;

  if(w == 0){
    digitalWrite(pin,LOW);
  }
  else if(w == 1){
    digitalWrite(pin,HIGH);
  }
  else{
    interval = (1 - w) * timescale;
    interval = (int) interval;

    digitalWrite(pin,HIGH);
    delay(interval);
    digitalWrite(pin,LOW);
  }
}


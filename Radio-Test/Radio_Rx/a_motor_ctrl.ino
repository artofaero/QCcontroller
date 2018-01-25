// Individual motor writes
float motor_write(float t, float y, float r, float p, float wT0){
  float wT = throttle(t,wT0);
  motor_LF(y,r,p,wT);
  motor_RF(y,r,p,wT);
  motor_LR(y,r,p,wT);
  motor_RR(y,r,p,wT);
  return wT;
}

void motor_LF(float y, float r, float p, float wT){
  float wY = y*f;
  float wR = - g*r;
  float wP = - h*p;
  float w = wT + wY + wR + wP;

  if(w <= 0){
    w = 0;
  }
  analogWrite(R,floatTOint(w));
}

void motor_RF(float y, float r, float p, float wT){
  float wY = - y*f;
  float wR = g*r;
  float wP = - h*p;
  float w = wT + wY + wR + wP;

  if(w <= 0){
    w = 0;
  }
  analogWrite(B,floatTOint(w));
}

void motor_LR(float y, float r, float p, float wT){
  float wY = - y*f;
  float wR = - g*r;
  float wP = h*p;
  float w = wT + wY + wR + wP;

  if(w <= 0){
    w = 0;
  }
  analogWrite(Y,floatTOint(w));
}

void motor_RR(float y, float r, float p, float wT){
  float wY = y*f;
  float wR = g*r;
  float wP = h*p;
  float w = wT + wY + wR + wP;

  if(w <= 0){
    w = 0;
  }  
  analogWrite(G,floatTOint(w));
}

float throttle(float t, float wT0){
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


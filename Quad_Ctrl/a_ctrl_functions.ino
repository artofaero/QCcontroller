// Functions
//
// List of functions:
// alt_ctrl
// yaw_ctrl
// roll_ctrl
// pitch_ctrl
 
float alt_ctrl(float t, float e){
  // Altitude Control Function
  // Function produces an acceleration along the vertical axis of the
  // quad-copter corresponding to control input from Throttle Joystick.
  //
  // Inputs:
  // t = throttle input. Number between 0 and 1.
  // e = thrust fraction. Number between 0 and 1 following these rules:
  //       e + f + g + h <= 1
  //       f + g + h <= e
  // Outputs:
  // omega = A 4x1 array of angular velocities of each motor respectively.
  //          Number is dimensionless between 0 and 1.
  
  float omega[4];

  
  for(int i = 0; i < 4; i++){
    omega[i] = e*t;
  }
  return omega[4];
}

float yaw_ctrl(float y, float f, float omega0[4]){
//   Yaw Control Function
//   
//   Function produces an angular acceleration along the quad-copter's yaw
//   axis corresponding to control input from Yaw Joystick.
//  
//   Inputs:
//   y = Yaw input. Number between 0 and 1.
//   f = Yaw Fraction. Number between 0 and 1 following these rules:
//         e + f + g + h <= 1
//         f + g + h <= e
//   omega0 = A 4x1 array of angular velocities of each motor respectively 
//             calculated from the altitude control function.
//             Number is dimensionless between 0 and 1.
//  
//   Outputs:
//   omega = A 4x1 array of angular velocities of each motor respectively.
//             Number is dimensionless between 0 and 1.
  
  float omega[4];
  
  omega[0] = omega0[0] + y*f;
  omega[1] = omega0[1] - y*f;
  omega[2] = omega0[2] - y*f;
  omega[3] = omega0[3] + y*f;

  return omega[4];
}

float roll_ctrl(float r, float g, float omega0[4]){
  // Yaw Control Function
  // 
  // Function produces an angle along the quad-copter's roll axis 
  // corresponding to control input from Roll Joystick.
  //
  // Inputs:
  // r = Roll input. Number between 0 and 1.
  // g = Roll Fraction. Number between 0 and 1 following these rules:
  //       e + f + g + h <= 1
  //       f + g + h <= e
  // omega0 = A 4x1 array of angular velocities of each motor respectively 
  //           calculated from the altitude control function.
  //           Number is dimensionless between 0 and 1.
  //
  // Outputs:
  // omega = A 4x1 array of angular velocities of each motor respectively.
  //           Number is dimensionless between 0 and 1.
  
  float omega[4];

  omega[0] = omega0[0] - g*r;
  omega[1] = omega0[1] + g*r;
  omega[2] = omega0[2] - g*r;
  omega[3] = omega0[3] + g*r;
  
  return omega[4];
}

float pitch_ctrl(float p,float h,float omega0[4]){
  // Pitch Control Function
  // 
  // Function produces an angle along the quad-copter's roll axis 
  // corresponding to control input from Roll Joystick.
  //
  // Inputs:
  // p = Pitch input. Number between 0 and 1.
  // h = Pitch Fraction. Number between 0 and 1 following these rules:
  //       e + f + g + h <= 1
  //       f + g + h <= e
  // omega0 = A 4x1 array of angular velocities of each motor respectively 
  //           calculated from the altitude control function.
  //           Number is dimensionless between 0 and 1.
  //
  // Outputs:
  // omega = A 4x1 array of angular velocities of each motor respectively.
  //           Number is dimensionless between 0 and 1.
  
  float omega[4];

  omega[0] = omega0[0] - h*p;
  omega[1] = omega0[1] - h*p;
  omega[2] = omega0[2] + h*p;
  omega[3] = omega0[3] + h*p;
  
  return omega[4];
}

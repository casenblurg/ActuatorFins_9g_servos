#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


Adafruit_PWMServoDriver pwm;

#define Servo_0 0
#define Servo_0_0 130
#define Servo_0_90 350
#define Servo_0_180 550

#define Servo_1 1
#define Servo_1_0 120
#define Servo_1_90 320
#define Servo_1_180 540

#define Servo_2 2
#define Servo_2_0 120
#define Servo_2_90 340
#define Servo_2_180 540

#define Servo_3 3
#define Servo_3_0 120
#define Servo_3_90 330
#define Servo_3_180 550



int test_pwm = 350;
int test_servo = 0;

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setPWMFreq(50);

  Serial.println("Servo Calibration");

}

void loop(){
  
  pwm.setPWM(test_servo, 0, test_pwm);

    if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    // Check if input is a single digit (0–4) to change servo channel
    if (input.length() == 1 && isDigit(input.charAt(0))) {
      int channel = input.toInt();
      if (channel >= 0 && channel <= 4) {
        test_servo = channel;
        Serial.print("test_servo is now using channel ");
        Serial.println(test_servo);
        return;
      }
    }

    // Otherwise, try to update the PWM value
    int newVal = input.toInt();
    if (newVal >= 80 && newVal <= 600) {
      test_pwm = newVal;
      Serial.print("Updated test_pwm to: ");
      Serial.println(test_pwm);
    } else {
      Serial.println("Invalid input. Use 0–4 to change servo channel or 80–600 for PWM value.");
    }
  }

  delay(20);
}
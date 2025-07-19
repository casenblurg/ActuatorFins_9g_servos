#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Servo PWM mappings
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

// Function prototype
int mapFloat(float t, float in_min, float in_max, int out_min, int out_max);

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setPWMFreq(50);

  Serial.println("Starting Sequence...");
  centerAllServos();
  delay(1000);

  Serial.println("Stage 1: Paired Sweep");
  stage1_pairedSweep();
  centerAllServos();
  delay(1000);

  Serial.println("Stage 2: Wiggle");
  stage2_wiggle();
  centerAllServos();
  delay(1000);

  Serial.println("Stage 3: All Together 0 → 180 → 0");
  stage3_allTogetherSweep();
  centerAllServos();
  delay(1000);

  Serial.println("Done.");
}

void loop() {
  // Do nothing
}

void centerAllServos() {
  pwm.setPWM(Servo_0, 0, Servo_0_90);
  pwm.setPWM(Servo_1, 0, Servo_1_90);
  pwm.setPWM(Servo_2, 0, Servo_2_90);
  pwm.setPWM(Servo_3, 0, Servo_3_90);
}

// --- Stage 1: Paired Sweep ---
void stage1_pairedSweep() {
  for (int i = 0; i <= 100; i++) {
    float t = i / 100.0;
    pwm.setPWM(Servo_0, 0, mapFloat(t, 0, 1, Servo_0_0, Servo_0_180));
    pwm.setPWM(Servo_2, 0, mapFloat(t, 0, 1, Servo_2_180, Servo_2_0)); // inverted
    delay(15);
  }
  delay(250);
  for (int i = 0; i <= 100; i++) {
    float t = i / 100.0;
    pwm.setPWM(Servo_1, 0, mapFloat(t, 0, 1, Servo_1_0, Servo_1_180));
    pwm.setPWM(Servo_3, 0, mapFloat(t, 0, 1, Servo_3_180, Servo_3_0)); // inverted
    delay(15);
  }
}

// --- Stage 2: Wiggle ---
void stage2_wiggle() {
  wiggleServos(Servo_0, Servo_0_90, Servo_0_0, Servo_0_180, Servo_2, Servo_2_90, Servo_2_180, Servo_2_0);
  delay(250);
  wiggleServos(Servo_1, Servo_1_90, Servo_1_0, Servo_1_180, Servo_3, Servo_3_90, Servo_3_180, Servo_3_0);
}

void wiggleServos(int s1, int s1_center, int s1_min, int s1_max, int s2, int s2_center, int s2_min, int s2_max) {
  int range1 = (s1_max - s1_min) / 6;  // Bigger wiggle
  int range2 = (s2_max - s2_min) / 6;

  for (int i = 0; i < 3; i++) {
    pwm.setPWM(s1, 0, s1_center + range1);
    pwm.setPWM(s2, 0, s2_center - range2);
    delay(150);

    pwm.setPWM(s1, 0, s1_center - range1);
    pwm.setPWM(s2, 0, s2_center + range2);
    delay(150);
  }

  pwm.setPWM(s1, 0, s1_center);
  pwm.setPWM(s2, 0, s2_center);
}


// --- Stage 3: All Together Sweep ---
void stage3_allTogetherSweep() {
  for (int i = 0; i <= 100; i++) {
    float t = i / 100.0;
    pwm.setPWM(Servo_0, 0, mapFloat(t, 0, 1, Servo_0_0, Servo_0_180));
    pwm.setPWM(Servo_1, 0, mapFloat(t, 0, 1, Servo_1_0, Servo_1_180));
    pwm.setPWM(Servo_2, 0, mapFloat(t, 0, 1, Servo_2_0, Servo_2_180));
    pwm.setPWM(Servo_3, 0, mapFloat(t, 0, 1, Servo_3_0, Servo_3_180));
    delay(10);
  }
  for (int i = 100; i >= 0; i--) {
    float t = i / 100.0;
    pwm.setPWM(Servo_0, 0, mapFloat(t, 0, 1, Servo_0_0, Servo_0_180));
    pwm.setPWM(Servo_1, 0, mapFloat(t, 0, 1, Servo_1_0, Servo_1_180));
    pwm.setPWM(Servo_2, 0, mapFloat(t, 0, 1, Servo_2_0, Servo_2_180));
    pwm.setPWM(Servo_3, 0, mapFloat(t, 0, 1, Servo_3_0, Servo_3_180));
    delay(10);
  }
}

// --- Float Mapping Helper ---
int mapFloat(float t, float in_min, float in_max, int out_min, int out_max) {
  return out_min + (out_max - out_min) * ((t - in_min) / (in_max - in_min));
}

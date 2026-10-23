/*
** EPITECH PROJECT, 2026
** main.hpp
** File description:
** ...
*/

#include "component/arm_manager.hpp"
#include "utils.hpp"

SMS_STS leaderBus;
SMS_STS followerBus;


Arm leaderArm;
Arm followerArm;

const int numServos = SERVO_COUNT;
byte servoIDs[numServos] = {1, 2, 3, 4, 5, 6};

void setup() {
  Serial.begin(115200);
  while (!Serial);

  pinMode(LEADER_LED, OUTPUT);
  pinMode(FOLLOWER_LED, OUTPUT);

  pinMode(LEADER_BTN, INPUT_PULLUP);
  pinMode(FOLLOWER_BTN, INPUT_PULLUP);

  digitalWrite(LEADER_LED, LOW);
  digitalWrite(FOLLOWER_LED, LOW);

  Serial1.begin(1000000);
  leaderBus.pSerial = &Serial1;

  Serial2.begin(1000000);
  followerBus.pSerial = &Serial2;

  for (int i = 0; i < numServos; i++) {
    int pos = leaderBus.ReadPos(servoIDs[i]);
    while (pos == -1) {
      Serial.println("Looking for Leader");
      pos = leaderBus.ReadPos(servoIDs[i]);
    }
    leaderArm.setServoMinMax(i, pos);
  }
  for (int i = 0; i < numServos; i++) {
    int pos = followerBus.ReadPos(servoIDs[i]);
    while (pos == -1) {
      Serial.println("Looking for Follower");
      pos = followerBus.ReadPos(servoIDs[i]);
    }
    followerArm.setServoMinMax(i, pos);
  }

  Serial.println("=================================================");
  Serial.println("  System Booted. Press Calibration Buttons:      ");
  Serial.println("=================================================");


}

#define DEBUG 1

bool leader_cal = false;
bool follower_cal = false;
byte state = LOW;

void loop() {
  leader_cal = leaderArm.isCalibrated();
  follower_cal = followerArm.isCalibrated();
  // while (DEBUG)
  // {
  //     int pos = leaderBus.ReadPos(servoIDs[4]);
  //     if (pos == -1) {
  //       Serial.print(" -> Missing Leader Servo ID: ");
  //     } else {
  //       Serial.print("\n\n-> Found Leader Servo ID: ");
  //       Serial.print(pos);
  //       leaderArm.setServoRange(4, pos);
  //       leaderArm.displayServoMinMax(4);
  //     }
  //   delay(200);
  // }



  if (!leader_cal) {
    state = !digitalRead(LEADER_LED);
    digitalWrite(LEADER_LED, state);
  } else if (!follower_cal && leader_cal) {
    state = !digitalRead(FOLLOWER_LED);
    digitalWrite(FOLLOWER_LED, state);
    
  }


  if (!leader_cal) {
    Serial.println("== Leader ==");
    for (int i = 0; i < numServos; i++) {
      int pos = leaderBus.ReadPos(servoIDs[i]);
      if (pos == -1) {
        Serial.print(" -> Missing Leader Servo ID: ");
        Serial.println(servoIDs[i]);
      } else {
        Serial.print(" -> Found Leader Servo ID: ");
        leaderArm.setServoRange(i, pos);
        leaderArm.displayServoMinMax(i);
      }
    }

    if (digitalRead(LEADER_BTN) == LOW) {
      digitalWrite(LEADER_LED, HIGH);
      digitalWrite(FOLLOWER_LED, LOW);
      leaderArm.setCalibrated();
      Serial.println("== Leader Arm Calibrated! ==");
    }
    delay(256);
  }
  else if (!follower_cal) {
    Serial.println("== Follower ==");
    for (int i = 0; i < numServos; i++) {
      int pos = followerBus.ReadPos(servoIDs[i]);
      if (pos == -1) {
        Serial.print(" -> Missing Follower Servo ID: ");
        Serial.println(servoIDs[i]);
      } else {
        Serial.print(" -> Found Follower Servo ID: ");
        followerArm.setServoRange(i, pos);
        leaderArm.displayServoMinMax(i);
      }
    }

    if (digitalRead(FOLLOWER_BTN) == LOW) {
      digitalWrite(FOLLOWER_LED, HIGH);
      followerArm.setCalibrated();
      Serial.println("== Follower Arm Calibrated! ==");
    }
    delay(250);
  }
  else if (leader_cal && follower_cal) {
    
    for (int i = 0; i < numServos; i++) {
      int leaderPos = leaderBus.ReadPos(servoIDs[i]);
      int followerPos = followerBus.ReadPos(servoIDs[i]);
      if (leaderPos != -1 || followerPos != -1) {
        int lMin = leaderArm.getServoMin(i);
        int lMax = leaderArm.getServoMax(i);
        int fMin = followerArm.getServoMin(i);
        int fMax = followerArm.getServoMax(i);

        float targetPos = fMin;
        if (lMax != lMin) {
          targetPos = fMin + (float)(leaderPos - lMin) * (fMax - fMin) / (lMax - lMin);
        }
        
        targetPos = followerArm.clampServo(i, targetPos);
        followerBus.WritePosEx(servoIDs[i], (int)targetPos, 1500, 50);
      }
    }
  }
}
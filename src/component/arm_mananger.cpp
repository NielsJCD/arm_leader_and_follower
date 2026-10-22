/*
** EPITECH PROJECT, 2026
** arm_manager.hpp
** File description:
** ...
*/

#include "component/arm_manager.hpp"

bool Arm::isValid(int id) const
{
    return id >= 0 && id < SERVO_COUNT;
}


float Arm::getServoMax(int id) const
{
    if (!isValid(id)) return 0.0f;
    return servo[id].max;
}

float Arm::getServoMin(int id) const
{
    if (!isValid(id)) return 0.0f;
    return servo[id].min;
}

bool Arm::isCalibrated() const
{
    return _isCalibrated == CALIBRATED;
}
int Arm::getServoCount() const
{
    return SERVO_COUNT;
}

void Arm::setServoMinMax(int id, int var)
{
    servo[id].min = var;
    servo[id].max = var;
}

bool Arm::setServoRange(int id, float min, float max)
{
    if (!isValid(id) || min > max) return false;
    servo[id].min = min;
    servo[id].max = max;
    return true;
}

bool Arm::setServoRange(int id, float var)
{
    if (!isValid(id)) return false;
    if (var > servo[id].max)
    servo[id].max = var;
    if (var < servo[id].min)
    servo[id].min = var;
    return true;
}

float Arm::clampServo(int id, float angle) const
{
    if (!isValid(id)) return angle;
    const servo_param_t& s = servo[id];
    if (angle < s.min) return s.min;
    if (angle > s.max) return s.max;
    return angle;
}

void Arm::setCalibrated()
{
    _isCalibrated = CALIBRATED;
}

void Arm::resetCalibration()
{
    _isCalibrated = NOTCALIBRATED;
}

void Arm::displayMinMax(void) const
{
    Serial.println("--- Servo Min / Max Ranges ---");
    for (int i = 0; i < SERVO_COUNT; i++) {
        Serial.print("  Servo ID [");
        Serial.print(i + 1);
        Serial.print("] -> Min: ");
        Serial.print(servo[i].min);
        Serial.print(" | Max: ");
        Serial.println(servo[i].max);
    }
}

void Arm::displayServoMinMax(int id) const
{
    Serial.println("--- Servo Min / Max Ranges ---");
    Serial.print("  Servo ID [");
    Serial.print(id + 1);
    Serial.print("] -> Min: ");
    Serial.print(servo[id].min);
    Serial.print(" | Max: ");
    Serial.println(servo[id].max);
}

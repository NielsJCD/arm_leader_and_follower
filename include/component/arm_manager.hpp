/*
** EPITECH PROJECT, 2026
** arm_manager.hpp
** File description:
** ...
*/

#ifndef ARM_MANAGER_HPP
    #define ARM_MANAGER_HPP

#include "utils.hpp"

struct servo_param_t {
    int max;
    int min;
};

class Arm
{
  private:
      bool _isCalibrated = NOTCALIBRATED;
      servo_param_t servo[SERVO_COUNT];
      
      bool isValid(int id) const;

  public:
      Arm() = default;
      ~Arm() = default;

      float getServoMax(int id) const;
      float getServoMin(int id) const;
      
      bool isCalibrated() const;
      
      int  getServoCount() const;

      void setServoMinMax(int id, int var);
      bool setServoRange(int id, float min, float max);
      
      bool setServoRange(int id, float var);
      
      float clampServo(int id, float angle) const;

      void setCalibrated();
      void resetCalibration();

      void displayMinMax(void) const;
      void displayServoMinMax(int id) const;
};

#endif // ARM_MANAGER_HPP

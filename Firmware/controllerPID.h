/**
Arquivo: controllerPID.h
Descrição: Arquivo das declarações da classe controllerPID. 
            -Classe que representa um controlador PID
*/
#ifndef TCC_CONTROLLER_PID_H
#define TCC_CONTROLLER_PID_H

#include "boardDefs.h"

class ControllerPID 
{
public:
  ControllerPID(){}
  
  void Init();
  
  void TurnOnOff(bool isOn);
  
  void Set_Kp(float newValue);
  void Set_Kd(float newValue);
  void Set_Ki(float newValue);
  void Set_Setpoint(float newValue);
  float Get_Kp(void);
  float Get_Ki(void);
  float Get_Kd(void);
  float Get_Setpoint(void);
  bool Get_ControllerState(void);

  float UpdateData(float newData);
  
private:
  bool IsOn;
  float Kp;
  float Ki;
  float Kd;
  float Error_Previous;
  float Error_sum;
  float SetPoint;
};

#endif

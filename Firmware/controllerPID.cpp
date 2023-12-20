/**
Arquivo: controllerPID.cpp
Descrição: Arquivo das definições da classe controllerPID. 
            -Classe que representa um controlador PID
*/

#include "controllerPID.h"

/******************************************************/
/* PUBLIC FUCTIONS                                    */
/******************************************************/

/**
* Initializes PID controller object
*/
void ControllerPID::Init(void)
{
  IsOn = false;
  Kp = 0;
  Ki = 0;
  Kd = 0;
  Error_Previous = 0;
  Error_sum = 0;
  SetPoint = 25.5;
}


/**
* Turns controller ON or OFF
* @param isOn: true = turns controller ON | false = turns controller OFF
*/
void ControllerPID::TurnOnOff(bool newState)
{
  Error_Previous = 0;
  Error_sum = 0;
  IsOn = newState;
}

/**
* Sets new Kp value
* @param newValue: new value to be set
*/
void ControllerPID::Set_Kp(float newValue)
{
  Kp = newValue;
}

/**
* Sets new Ki value
* @param newValue: new value to be set
*/
void ControllerPID::Set_Kd(float newValue)
{
  Kd = newValue;
}

/**
* Sets new Kd value
* @param newValue: new value to be set
*/
void ControllerPID::Set_Ki(float newValue)
{
  Ki = newValue;
}

/**
* Sets new setpoint value
* @param newValue: new value to be set
*/
void ControllerPID::Set_Setpoint(float newValue)
{
  SetPoint = newValue;
}

/**
* Reads current Kp value
* @return float: current value
*/ 
float ControllerPID::Get_Kp(void)
{
  return Kp;
}

/**
* Reads current Ki value
* @return float: current value
*/ 
float ControllerPID::Get_Ki(void)
{
  return Ki;
}

/**
* Reads current Kd value
* @return float: current value
*/ 
float ControllerPID::Get_Kd(void)
{
  return Kd;
}

/**
* Reads current setpoint value
* @return float: current value
*/ 
float ControllerPID::Get_Setpoint(void)
{
  return SetPoint;
}

/**
* Reads current controller state value
* @return bool: true = on | false = off
*/ 
bool ControllerPID::Get_ControllerState(void)
{
  return IsOn;
}

/**
* Controller iteration
* @param newData: input of this iteration
* @return float: output (controller effort). ranges from -255 to +255
*/ 
float ControllerPID::UpdateData(float newData)
{
	float ThisError, ThisDifference;
  float Result = 0;

  if(IsOn == true)
  {
    ThisError = SetPoint - newData;
    Error_sum += ThisError;
    ThisDifference = Error_Previous - ThisError;

    Result = Kp*ThisError
          + Ki*Error_sum
          + Kd*ThisDifference;

    Error_Previous = ThisError;

    if(Result >= 255.0 || Result <= -255.0)
    {
      /* Anti windup: remove this iteration error sum if controller effort is max value */
      Error_sum = Error_sum - ThisError;
    }

    //Make up diff because heating is faster than cooling
    if(Result > 0) {Result = Result*0.6;}

    if      (Result > 255.0) { Result = 255.0;  }
    else if (Result < -255.0){ Result = -255.0; }
  }

	return Result;
}


/**
Arquivo: commands.h
Descrição: Arquivo das definições da classe que controla os comandos enviados pelo usuário. 
*/

#include "commands.h"

/******************************************************/
/* PUBLIC FUCTIONS                                    */
/******************************************************/


#define CMD_SET_KEY_KP                    "setKp"           
#define CMD_SET_KEY_KI                    "setKi"
#define CMD_SET_KEY_KD                    "setKd"
#define CMD_SET_KEY_CONTROLLER_SETPOINT   "setControllerSetpoint"
#define CMD_SET_KEY_CONTROLLER_STATE      "setControllerState"
#define CMD_SET_KEY_MONITORING_STATE      "setMonitoringState"
#define CMD_SET_KEY_COOLER_STATE          "setCoolerState"
#define CMD_SET_KEY_COOLER_POWER          "setCoolerPower"
#define CMD_GET_KEY_KP                    "getKp"            
#define CMD_GET_KEY_KI                    "getKi"
#define CMD_GET_KEY_KD                    "getKd"
#define CMD_GET_KEY_CONTROLLER_SETPOINT   "getControllerSetpoint"
#define CMD_GET_KEY_CONTROLLER_STATE      "getControllerState"
#define CMD_GET_KEY_TEMPERATURE           "getTemperature"
#define CMD_GET_KEY_MONITORING_STATE      "getMonitoringState"
#define CMD_SEND_MONITORING_DATA          "sendMonitoringData"

/**
* Initialize commands module
* @param perifericos: pointer to global perifericos object
* @param bt: pointer to global bluetooth object
* @param controller: pointer to global controller object
*/
void Commands::Init(Perifericos* perifericos, TCCBluetooth* bt, ControllerPID* controller)
{
  pPerifericos = perifericos;
  pBluetooth = bt;
  pController = controller;
}

/**
* Handles commands send by user
* @param key: string identifying which command
* @param value: value for set commands
*/
void Commands::ReceivedCmdFromUser(std::string key, std::string value)
{
  if(key.compare(CMD_SET_KEY_KP) == 0)
  {
    pController->Set_Kp(std::stof(value));
  }
  else if(key.compare(CMD_SET_KEY_KI) == 0)
  {
    pController->Set_Ki(std::stof(value));
  }
  else if(key.compare(CMD_SET_KEY_KD) == 0)
  {
    pController->Set_Kd(std::stof(value));
  }
  else if(key.compare(CMD_SET_KEY_CONTROLLER_SETPOINT) == 0)
  {
    pController->Set_Setpoint(std::stof(value)); 
  }
  else if(key.compare(CMD_SET_KEY_CONTROLLER_STATE) == 0)
  {
    bool bValue = false;
    if(value.compare("true") == 0){ bValue = true; }
    if(bValue == false){ pPerifericos->Peltier_SetPowerLevel(0); } //Turns off Peltier is controller is gonna be turned off
    Serial.println("New controller state is:" + String(bValue));
    pController->TurnOnOff(bValue); 
  }
  else if(key.compare(CMD_SET_KEY_MONITORING_STATE) == 0)
  {
    if(value.compare("true") == 0){ isMonitoringOn = true; }
    else{ isMonitoringOn = false; }
  }
  else if(key.compare(CMD_SET_KEY_COOLER_STATE) == 0)
  {
    bool bValue = false;
    if(value.compare("true") == 0){ bValue = true; }
    pPerifericos->CoolerInternal_SetOnOff(bValue); 
  }
  else if(key.compare(CMD_SET_KEY_COOLER_POWER) == 0)
  {
    int newPower = std::stof(value);
    if(newPower > 255){ newPower = 255; }
    pPerifericos->CoolerInternal_SetPowerLevel(newPower);
  }
  else if(key.compare(CMD_GET_KEY_KP) == 0)
  {
    float kp = pController->Get_Kp();
    pBluetooth->WriteData(key, std::to_string(kp), millis());
  }
  else if(key.compare(CMD_GET_KEY_KI) == 0)
  {
    float ki = pController->Get_Ki();
    pBluetooth->WriteData(key, std::to_string(ki), millis());    
  }
  else if(key.compare(CMD_GET_KEY_KD) == 0)
  {
    float kd = pController->Get_Kd();
    pBluetooth->WriteData(key, std::to_string(kd), millis());    
  }
  else if(key.compare(CMD_GET_KEY_CONTROLLER_SETPOINT) == 0)
  {
    float setpoint = pController->Get_Setpoint();
    pBluetooth->WriteData(key, std::to_string(setpoint), millis());    
  }
  else if(key.compare(CMD_GET_KEY_CONTROLLER_STATE) == 0)
  {
    bool controllerState = pController->Get_ControllerState();
    pBluetooth->WriteData(key, std::to_string(controllerState), millis());       
  }
  else if(key.compare(CMD_GET_KEY_TEMPERATURE) == 0)
  {
    float temp = pPerifericos->TempSensor_ReadTemperature();
    pBluetooth->WriteData(key, std::to_string(temp), millis()); 
  } 
  else if(key.compare(CMD_GET_KEY_MONITORING_STATE) == 0)
  {
    pBluetooth->WriteData(key, std::to_string(isMonitoringOn), millis());
  }  
}

/**
* Sends current temperature to user
* @param newData: value that will be send to user
*/
void Commands::SendMonitoringTempData(float newData)
{
  if(isMonitoringOn == true){ pBluetooth->WriteData(CMD_SEND_MONITORING_DATA, std::to_string(newData), millis()); }
}
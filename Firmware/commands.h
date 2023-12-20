/**
Arquivo: commands.h
Descrição: Arquivo das declarações da classe que controla os comandos enviados pelo usuário. 
*/

#ifndef TCC_COMMANDS_PID_H
#define TCC_COMMANDS_PID_H

#include "boardDefs.h"
#include "controllerPID.h"
#include "TCCbluetooth.h"
#include "perifericos.h"

class Commands 
{
public:
  Commands(){}
  
  void Init(Perifericos* perifericos, TCCBluetooth* bt, ControllerPID* controller);
  
  void ReceivedCmdFromUser(std::string key, std::string value);
  
  void SendMonitoringTempData(float newData);
  
private:
  Perifericos* pPerifericos; 
  TCCBluetooth* pBluetooth; 
  ControllerPID* pController;
  bool isMonitoringOn = false;

};

#endif

/**
Arquivo: boardDefs.h
Descrição: Arquivo com a definição dos pinos
*/

#ifndef TCC_BOARD_H
#define TCC_BOARD_H

#include <Arduino.h>

//#define DEBUG_SERIAL

#define CoolerInternal_Pin   33 //channel 5
#define CoolerPeltier_Pin    26 //channel 9
  
#define Peltier_DirectPin    25//32 //channel 4
#define Peltier_ReversePin   32//25 //channel 8

#define TempSensor_Pin       19

#endif
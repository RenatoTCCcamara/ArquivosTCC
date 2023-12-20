/**
Arquivo: perifericos.h
Descrição: Arquivo das declarações da classe perifericos. 
            -Classe que faz interface com os elementos de hardware
*/

#ifndef TCC_PERIFERICOS_H
#define TCC_PERIFERICOS_H

#include "boardDefs.h"
#include <LiquidCrystal_I2C.h>
#include "OneWire.h"
#include "DallasTemperature.h"

class Perifericos 
{
public:
  Perifericos(){}

  /* FUNCTIONS */
  /* General functions */
  void Init(void);
  
  /* LCD functions */
  void LCD_WriteL1(String text);
  void LCD_WriteL2(String text);

  /* Cooler functions */
  void CoolerInternal_SetOnOff(bool isOn);
  void CoolerInternal_SetPowerLevel(int newPower);

  /* Peltier functions */
  void Peltier_SetPowerLevel(int newPowerLevel);

  /* Temperature sensor functions */ 
  float TempSensor_ReadTemperature(void);

private:
  /* VARIABLES */
  /* LCD variables */
  LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
  String LCD_TextLine1 = "";
  String LCD_TextLine2 = "";
  
  /* Cooler variables */
  #define Cooler_PWMFreq 2500
  #define Cooler_PWMResolution 8
  #define CoolerInternal_PWMChannel 5
  #define CoolerPeltier_PWMChannel 9
  int CoolerInternal_PowerLvl = 0;
  int CoolerPeltier_PowerLvl = 0;
  
  /* Peltier variables */
  #define Peltier_PWMFreq 2500
  #define Peltier_PWMResolution 8
  #define Peltier_DirectPWMChannel 4
  #define Peltier_ReversePWMChannel 8

  /* Temperature sensor variables */ 
  DeviceAddress TempSensor_Address;
  OneWire TempSensor_OneWire;
  DallasTemperature TempSensor_Sensor;

  /* FUNCTIONS */
  /* LCD functions */
  void LCD_UpdateScreen(void);

  /* Cooler functions */
  void CoolerPeltier_SetOnOff(bool isOn);
  void CoolerPeltier_SetPowerLevel(int newPower);
};

#endif

/**
Arquivo: perifericos.cpp
Descrição: Arquivo das definições da classe perifericos. 
            -Classe que faz interface com os elementos de hardware
*/

#include "perifericos.h"

/******************************************************/
/* PUBLIC FUCTIONS                                    */
/******************************************************/

/**
* Initializes Perifericos object
*/
void Perifericos::Init(void)
{
  /* LCD */
  lcd.init();
  lcd.backlight();
  
  /* Cooler */
  Serial.println("Init PWM cooler internal");
  ledcSetup(CoolerInternal_PWMChannel, Cooler_PWMFreq, Cooler_PWMResolution);
  ledcAttachPin(CoolerInternal_Pin, CoolerInternal_PWMChannel);
  Serial.println("Init PWM cooler peltier");
  ledcSetup(CoolerPeltier_PWMChannel, Cooler_PWMFreq, Cooler_PWMResolution);
  ledcAttachPin(CoolerPeltier_Pin, CoolerPeltier_PWMChannel);

  /* Peltier */
   Serial.println("Init PWM peltier direct");
  ledcSetup(Peltier_DirectPWMChannel, Peltier_PWMFreq, Peltier_PWMResolution);
  ledcAttachPin(Peltier_DirectPin, Peltier_DirectPWMChannel);
   Serial.println("Init PWM peltier reverse");
  ledcSetup(Peltier_ReversePWMChannel, Peltier_PWMFreq, Peltier_PWMResolution);
  ledcAttachPin(Peltier_ReversePin, Peltier_ReversePWMChannel);

  /* Temperature sensor */
  TempSensor_OneWire = OneWire(TempSensor_Pin);
  TempSensor_Sensor = DallasTemperature(&TempSensor_OneWire);
  TempSensor_Sensor.begin();

}

/* LCD functions */
/**
* Writes text on LCD line 1
* @param text: text to be written
*/
void Perifericos::LCD_WriteL1(String text)
{
  LCD_TextLine1 = text;
  LCD_UpdateScreen();
}

/**
* Writes text on LCD line 2
* @param text: text to be written
*/
void Perifericos::LCD_WriteL2(String text)
{
  LCD_TextLine2 = text;
  LCD_UpdateScreen();
}

/* Cooler functions */
/**
* Turns internal cooler on or off
* @param isOn: true = turns cooler on | false = turns cooler off
*/
void Perifericos::CoolerInternal_SetOnOff(bool isOn)
{
  if(isOn){ ledcWrite(CoolerInternal_PWMChannel, CoolerInternal_PowerLvl); }
  else{ ledcWrite(CoolerInternal_PWMChannel, 0); }

}

/**
* Changes internal cooler power lvl
* @param newPower: integer between 0 and 255
*/
void Perifericos::CoolerInternal_SetPowerLevel(int newPower)
{
  if(newPower >= 0 && newPower <= 255)
  {
    CoolerInternal_PowerLvl = newPower;
    ledcWrite(CoolerInternal_PWMChannel, CoolerInternal_PowerLvl); 
  }
}

/* Peltier functions */
/**
* Changes Peltier power lvl
* @param newPowerLevel: integer between -255 and 255. Negative values are for reverse current, and positive values for direct current 
*/
void Perifericos::Peltier_SetPowerLevel(int newPowerLevel)
{
  /* Peltier cooler is always On at max power, to avoid burning the cell */
  CoolerPeltier_SetOnOff(true);
  CoolerPeltier_SetPowerLevel(255);

  #ifdef DEBUG_SERIAL
  Serial.println("Setting peltier to:" + String(newPowerLevel));
  #endif
  
  if(newPowerLevel < 0 && newPowerLevel >= -255)
  {
    /* Negative values are for reverse current */
    ledcWrite(Peltier_DirectPWMChannel, 0);
    ledcWrite(Peltier_ReversePWMChannel, -newPowerLevel); 
  }
  else if(newPowerLevel >= 0 && newPowerLevel <= 255)
  {
    /* Positive values are for direct current */
    ledcWrite(Peltier_ReversePWMChannel, 0); 
    ledcWrite(Peltier_DirectPWMChannel, newPowerLevel); 
  }
}

/* Temperature sensor functions */
/**
* Reads current temperature from sensor
* @return float: read temperature value. If reading fails the default return value is 25.48
*/
float Perifericos::TempSensor_ReadTemperature(void) 
{
  /* Start temperature with a default room temp value, so actuactors won't be overloaded if sensor fails */
  float fT  = 25.48;

  TempSensor_Sensor.requestTemperatures();
  /* Only attempts to read temperature if sensor is working properly */
  if (TempSensor_Sensor.getAddress(TempSensor_Address, 0))
  {
    fT = TempSensor_Sensor.getTempC(TempSensor_Address);
  }

  return fT;
}

/******************************************************/
/* PRIVATE FUCTIONS                                   */
/******************************************************/
/* LCD functions */
/**
* Updates LCD screen with texts stored on line variables
*/
void Perifericos::LCD_UpdateScreen(void)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(LCD_TextLine1);
  lcd.setCursor(0, 1);
  lcd.print(LCD_TextLine2);  
}

/* Cooler functions */
/**
* Turns Peltier cooler on or off
* @param isOn: true = turns cooler on | false = turns cooler off
*/
void Perifericos::CoolerPeltier_SetOnOff(bool isOn)
{
  if(isOn){ ledcWrite(CoolerPeltier_PWMChannel, CoolerPeltier_PowerLvl); }
  else{ ledcWrite(CoolerPeltier_PWMChannel, 0); }

}

/**
* Changes Peltier cooler power lvl
* @param newPower: integer between 0 and 255
*/
void Perifericos::CoolerPeltier_SetPowerLevel(int newPower)
{
  if(newPower >= 0 && newPower <= 255)
  {
    CoolerPeltier_PowerLvl = newPower;
    ledcWrite(CoolerPeltier_PWMChannel, CoolerPeltier_PowerLvl); 
  }
}
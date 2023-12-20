/**
Arquivo: TCC_camara.ino
Descrição: arquivo Main do projeto
*/

#include "boardDefs.h"
#include "perifericos.h"
#include "TCCbluetooth.h"
#include "commands.h"
#include "controllerPID.h"

/* Defines */
#define MONIROTING_COUNTER_LIMIT  2  /* 2*500ms  = 1s */
#define LCD_COUNTER_LIMIT         10 /* 10*500ms = 5s */

/* Objects and variables*/
Perifericos perifericos;
TCCBluetooth bt;
ControllerPID controller;
Commands commands;
hw_timer_t *Timer0_Cfg = NULL;
bool timerTriggered = false;
int monirotingTimerCounter = 0;
int lcdTimerCounter = 0;
float lastReadTemperature = 25.68;

/**
* Timer interruption
*  Sets up flag and increment monitoring counter
*  Triggers every 500ms
*/
void IRAM_ATTR Timer0_ISR()
{
  timerTriggered = true;
  monirotingTimerCounter++;
  lcdTimerCounter++;
}

void setup()
{
  Serial.begin(115200);
  perifericos.Init();
  bt.Init();
  controller.Init();
  commands.Init(&perifericos, &bt, &controller);

  Timer0_Cfg = timerBegin(0, 80, true);
  timerAttachInterrupt(Timer0_Cfg, &Timer0_ISR, true);
  timerAlarmWrite(Timer0_Cfg, 500000, true); //500.000 us = 0,5s
  timerAlarmEnable(Timer0_Cfg);

  perifericos.LCD_WriteL1("Turning on...");
}

void loop()
{
  if(bt.ReadCmdFlag() == true)
  { 
    #ifdef DEBUG_SERIAL
      Serial.print("Received cmd: ");
      Serial.print(bt.ReadCmdKey().c_str());
      Serial.print(" | Value is: ");
      Serial.println(bt.ReadCmdValue().c_str());
    #endif   
    commands.ReceivedCmdFromUser(bt.ReadCmdKey(), bt.ReadCmdValue());
  }

  if(timerTriggered == true)
  {
    lastReadTemperature = perifericos.TempSensor_ReadTemperature();
    Serial.print(String(lastReadTemperature));
    
    float ctrlSignal = controller.UpdateData(lastReadTemperature);
    perifericos.Peltier_SetPowerLevel((int)ctrlSignal);

    if(monirotingTimerCounter >= MONIROTING_COUNTER_LIMIT)
    {
      commands.SendMonitoringTempData(lastReadTemperature);
      monirotingTimerCounter = 0;
    }

    if(lcdTimerCounter >= LCD_COUNTER_LIMIT)
    {
      float controllerSetpoint = controller.Get_Setpoint();
      String line1Text = "Temp= " + String(lastReadTemperature) + " C";
      String line2Text = "Goal= " + String(controllerSetpoint) + " C";
      perifericos.LCD_WriteL1(line1Text);
      perifericos.LCD_WriteL2(line2Text);   

      lcdTimerCounter = 0;   
    }

    timerTriggered = false;
  }
}




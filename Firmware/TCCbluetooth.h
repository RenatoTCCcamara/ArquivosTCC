/**
Arquivo: bluetooth.h
Descrição: Arquivo das declarações da classe Bluetooth. 
            -Classe que controla as funcionalidades do bluetooth, como conexão e troca de dados
*/
#ifndef TCC_BLUETOOTH_H
#define TCC_BLUETOOTH_H

#include "boardDefs.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define DATA_SERVICE_UUID                    "00000010-0000-1000-8000-00805f9b34fb"
#define CMD_SERVICE_UUID                     "00000020-0000-1000-8000-00805f9b34fb"
#define DATA_CHARACTERISTIC_KEY_UUID         "00000011-0000-1000-8000-00805f9b34fb"
#define DATA_CHARACTERISTIC_VALUE_UUID       "00000012-0000-1000-8000-00805f9b34fb"
#define DATA_CHARACTERISTIC_TIMESTAMP_UUID   "00000013-0000-1000-8000-00805f9b34fb"
#define CMD_CHARACTERISTIC_KEY_UUID          "00000021-0000-1000-8000-00805f9b34fb"
#define CMD_CHARACTERISTIC_VALUE_UUID        "00000022-0000-1000-8000-00805f9b34fb"

#define BLUETOOTH_SECURITY_PIN 123456
#define BLUETOOTH_ADVERTISING_NAME "Camara_TCC_test"

/* Class that handles bluetooth characteristics data exchange */
class CharactCallbacks: public BLECharacteristicCallbacks 
{
public:
  CharactCallbacks(void){}
  CharactCallbacks(std::string* cmdValue, std::string* cmdKey, bool* cmdFlag);
  
private:
  std::string *charactCbkCmdValue;
  std::string *charactCbkCmdKey;
  bool *charactCbkCmdFlag;
  void onWrite(BLECharacteristic *pCharacteristic); 
};

/* Class that handles bluetooth server connections */
class ServerCallback: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer);
    void onDisconnect(BLEServer* pServer);
};

/* Class that handles bluetooth security */
class SecurityCallback : public BLESecurityCallbacks {
  uint32_t onPassKeyRequest();
  void onPassKeyNotify(uint32_t pass_key){}
  bool onConfirmPIN(uint32_t pass_key);
  bool onSecurityRequest();
  void onAuthenticationComplete(esp_ble_auth_cmpl_t cmpl);
};

/* Class that represents a bluetooth object, used to interact with bluetooth */
class TCCBluetooth 
{
public:

  /* Constructor */
  TCCBluetooth(){}

  /* Variables */
  std::string cmdValue;
  std::string cmdKey;
  bool cmdFlag;

  /* Functions */
  void Init(void);
  void WriteData(std::string key, std::string value, int timestamp);
  std::string ReadCmdKey(void);
  std::string ReadCmdValue(void);
  bool ReadCmdFlag(void);

private:

  /* Variables */
  BLEServer *pServer;
  BLEService *dataService;
  BLEService *cmdService;
  BLECharacteristic *dataKeyCharacteristic;
  BLECharacteristic *dataValueCharacteristic;
  BLECharacteristic *dataTimestampCharacteristic;
  BLECharacteristic *cmdKeyCharacteristic;
  BLECharacteristic *cmdValueCharacteristic;
  BLEAdvertising *pAdvertising;

  /* Functions */
  void InitBLESecurity(void);
};

#endif

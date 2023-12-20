/**
Arquivo: bluetooth.c
Descrição: Arquivo das definições da classe Bluetooth. 
            -Classe que controla as funcionalidades do bluetooth, como conexão e troca de dados
*/

#include "TCCbluetooth.h"
#include "boardDefs.h"
/******************************************************/
/* PUBLIC FUCTIONS                                    */
/******************************************************/

/* Characteristic callbacks class */
/**
* Initializes characteristic callback with pointers to write store written data.
* @param cmdValue: pointer to string where CmdValue data characteristic will be stored
* @param cmdValue: pointer to string where CmdKey data characteristic will be stored
* @param cmdValue: pointer to boolean where CmdFlag will be stored
*/
CharactCallbacks::CharactCallbacks(std::string* cmdValue, std::string* cmdKey, bool* cmdFlag)
{
  charactCbkCmdValue = cmdValue;
  charactCbkCmdKey = cmdKey;
  charactCbkCmdFlag = cmdFlag;
}

/**
* Called when client writes data on characteristic. Stores data and sets up flag.
* @param pCharacteristic: pointer of the characteristic that received write
*/
void CharactCallbacks::onWrite(BLECharacteristic *pCharacteristic) 
{
  std::string uuid = pCharacteristic->getUUID().toString();
  std::string charactData = pCharacteristic->getValue();

  if(charactData.length() > 0) 
  {
    if(uuid.compare(CMD_CHARACTERISTIC_KEY_UUID) == 0)
    {
      #ifdef DEBUG_SERIAL
      Serial.println("Bluetooth> Received new KEY!");
      #endif

      if(charactCbkCmdKey != NULL && charactCbkCmdFlag != NULL)
      {
        *charactCbkCmdKey = charactData;
        *charactCbkCmdFlag = true;
      }
    }
    else if(uuid.compare(CMD_CHARACTERISTIC_VALUE_UUID) == 0)
    {
      #ifdef DEBUG_SERIAL
      Serial.println("Bluetooth> Received new Value!");
      #endif

      if(charactCbkCmdValue != NULL)
      { 
        *charactCbkCmdValue = charactData; 
      }
    }

    #ifdef DEBUG_SERIAL
    Serial.print("Bluetooth> Received new write: ");
    Serial.print(charactData.c_str());
    Serial.print(" | from UUID: ");
    Serial.println(uuid.c_str());
    #endif
  }
}

/* Server callbacks */
/**
* Called when client sucessfully connects to server
* @param pServer: pointer to the BLE server
*/
void ServerCallback::onConnect(BLEServer* pServer) 
{
  #ifdef DEBUG_SERIAL
  Serial.println("Bluetooth client connected!");
  #endif
};

/**
* Called when client disconnects from the server
* @param pServer: pointer to the BLE server
*/
void ServerCallback::onDisconnect(BLEServer* pServer) 
{
  #ifdef DEBUG_SERIAL
  Serial.println("Bluetooth client disconnected...");
  #endif
  BLEDevice::startAdvertising();
}

/* Security callbacks */
/**
* Called when client requests pass
* @return uint32_t: pin
*/
uint32_t SecurityCallback::onPassKeyRequest()
{
  return BLUETOOTH_SECURITY_PIN;
}

/**
* Called when client inputs correct pin
* @return bool: always returns true since it's only called when pin is correct
*/
bool SecurityCallback::onConfirmPIN(uint32_t pass_key)
{
  vTaskDelay(5000);
  return true;
}

/**
* Called when client requests security
* @return bool: always accept security requests
*/
bool SecurityCallback::onSecurityRequest(){
  return true;
}

/**
* Called when client authenticates connection. If it's successfull does nothing, if it fails starts advertising again
* @return cmpl: BLE security object
*/
void SecurityCallback::onAuthenticationComplete(esp_ble_auth_cmpl_t cmpl)
{
  if(cmpl.success)
  {
    #ifdef DEBUG_SERIAL
    Serial.println("Bluetooth> SecurityCallback - Authentication Success");   
    #endif
  }
  else
  {
    #ifdef DEBUG_SERIAL
    Serial.println("   - SecurityCallback - Authentication Failure*");
    #endif
    BLEDevice::startAdvertising();
  }
}

/* TCCBluetooth functions */
/**
* Initialize TCCBluetooth object, sets up server, GATT, security, and advertising
*/
void TCCBluetooth::Init(void)
{
  /* Configure device and server */
  BLEDevice::init(BLUETOOTH_ADVERTISING_NAME);
  BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);
  BLEDevice::setSecurityCallbacks(new SecurityCallback());
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallback());

  /* Configura GATT */
  dataService = pServer->createService(DATA_SERVICE_UUID);
  cmdService = pServer->createService(CMD_SERVICE_UUID);

  dataKeyCharacteristic = dataService->createCharacteristic(
                                                            DATA_CHARACTERISTIC_KEY_UUID,
                                                            BLECharacteristic::PROPERTY_READ |
                                                            BLECharacteristic::PROPERTY_WRITE |
                                                            BLECharacteristic::PROPERTY_NOTIFY |
                                                            BLECharacteristic::PROPERTY_INDICATE
                                                           );
  
  dataValueCharacteristic = dataService->createCharacteristic(
                                                              DATA_CHARACTERISTIC_VALUE_UUID,
                                                              BLECharacteristic::PROPERTY_READ |
                                                              BLECharacteristic::PROPERTY_WRITE
                                                             );

  dataTimestampCharacteristic = dataService->createCharacteristic(
                                                              DATA_CHARACTERISTIC_TIMESTAMP_UUID,
                                                              BLECharacteristic::PROPERTY_READ |
                                                              BLECharacteristic::PROPERTY_WRITE
                                                             );
                                                             
  cmdKeyCharacteristic = cmdService->createCharacteristic(
                                                            CMD_CHARACTERISTIC_KEY_UUID,
                                                            BLECharacteristic::PROPERTY_READ |
                                                            BLECharacteristic::PROPERTY_WRITE |
                                                            BLECharacteristic::PROPERTY_NOTIFY |
                                                            BLECharacteristic::PROPERTY_INDICATE
                                                           );
  
  cmdValueCharacteristic = cmdService->createCharacteristic(
                                                             CMD_CHARACTERISTIC_VALUE_UUID,
                                                             BLECharacteristic::PROPERTY_READ |
                                                             BLECharacteristic::PROPERTY_WRITE
                                                            );


  cmdKeyCharacteristic->setCallbacks(new CharactCallbacks(NULL, &cmdKey, &cmdFlag));
  cmdValueCharacteristic->setCallbacks(new CharactCallbacks(&cmdValue, NULL, NULL));
  dataKeyCharacteristic->setCallbacks(new CharactCallbacks());
  dataValueCharacteristic->setCallbacks(new CharactCallbacks());
  dataTimestampCharacteristic->setCallbacks(new CharactCallbacks());

  dataService->start();
  cmdService->start();
  
  /* Configure advertising and security */
  pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
  InitBLESecurity();
}

/**
* Sends data to client.
* @param key: string with info of which data is being sent
* @param value: data value
* @param timestamp: board timestamp
*/
void TCCBluetooth::WriteData(std::string key, std::string value, int timestamp)
{
  dataKeyCharacteristic->setValue(key);
  dataValueCharacteristic->setValue(value);
  dataTimestampCharacteristic->setValue(std::to_string(timestamp));
  dataKeyCharacteristic->notify();
}

/**
* Reads last received command key.
* @return std::string: stored command key
*/
std::string TCCBluetooth::ReadCmdKey(void)
{
  cmdFlag = false;
  return cmdKey;
}

/**
* Reads last received command value.
* @return std::string: stored command value
*/
std::string TCCBluetooth::ReadCmdValue(void)
{
  return cmdValue;
}

/**
* Reads command flag.
* @return bool: true if command was received | false if no command was received
*/
bool TCCBluetooth::ReadCmdFlag(void)
{
  return cmdFlag;
}
/******************************************************/
/* PRIVATE FUCTIONS                                   */
/******************************************************/

/* TCCBluetooth functions */
/**
* Configure BLE security
*/
void TCCBluetooth::InitBLESecurity(void){
  esp_ble_auth_req_t auth_req = ESP_LE_AUTH_REQ_SC_MITM_BOND;
  esp_ble_io_cap_t iocap = ESP_IO_CAP_OUT;          
  uint8_t key_size = 16;     
  uint8_t init_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
  uint8_t rsp_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
  uint32_t passkey = BLUETOOTH_SECURITY_PIN;
  uint8_t auth_option = ESP_BLE_ONLY_ACCEPT_SPECIFIED_AUTH_DISABLE;
  esp_ble_gap_set_security_param(ESP_BLE_SM_SET_STATIC_PASSKEY, &passkey, sizeof(uint32_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_AUTHEN_REQ_MODE, &auth_req, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_IOCAP_MODE, &iocap, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_MAX_KEY_SIZE, &key_size, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_ONLY_ACCEPT_SPECIFIED_SEC_AUTH, &auth_option, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_SET_INIT_KEY, &init_key, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_SET_RSP_KEY, &rsp_key, sizeof(uint8_t));
}
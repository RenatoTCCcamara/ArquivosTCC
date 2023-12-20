/**
 * \file Bluetooth.kt
 * Bluetooth related functions
 */

package com.tcc.camaraclimatica.model.bluetooth

import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothProfile
import android.content.IntentFilter
import android.util.Log
import androidx.lifecycle.viewModelScope
import com.tcc.camaraclimatica.viewModel.SharedViewModel
import kotlinx.coroutines.launch
import java.util.Timer
import java.util.UUID
import kotlin.concurrent.schedule

val baseUUID: String = "-0000-1000-8000-00805f9b34fb"
val commandServiceUUID = UUID.fromString("00000020$baseUUID")
val commandKeyCharacteristicUUID = UUID.fromString("00000021$baseUUID")
val commandValueCharacteristicUUID = UUID.fromString("00000022$baseUUID")
val dataServiceUUID = UUID.fromString("00000010$baseUUID")
val dataKeyCharacteristicUUID = UUID.fromString("00000011$baseUUID")
val dataValueCharacteristicUUID = UUID.fromString("00000012$baseUUID")
val dataTimestampCharacteristicUUID = UUID.fromString("00000013$baseUUID")
/**
 * Initializes bluetooth module
 *  Invokes module init function, setup service and characteristic UUIDs, and sets
 *  callbacks subscriptions
 */
fun SharedViewModel.bluetoothInitialization(){
  bluetooth.initService(context)

  try
  {
    context.unregisterReceiver(bluetooth.getBroadcastReceiver())
  }
  catch(e: Exception)
  {
    Log.d("Bluetooth", "Broadcast receiver was not registered")
  }

  val filterPairingRequest = IntentFilter(BluetoothDevice.ACTION_PAIRING_REQUEST)
  context.registerReceiver(bluetooth.getBroadcastReceiver(), filterPairingRequest)
  val filterBondStateChange = IntentFilter(BluetoothDevice.ACTION_BOND_STATE_CHANGED)
  context.registerReceiver(bluetooth.getBroadcastReceiver(), filterBondStateChange)

  bluetooth.setGattService(commandServiceUUID)
  bluetooth.setGattCharacteristic(commandKeyCharacteristicUUID)
  bluetooth.setGattCharacteristic(commandValueCharacteristicUUID)
  bluetooth.subscribeForCharacteristicNotification(dataServiceUUID, dataKeyCharacteristicUUID)
  bluetooth.subscribeForCharacteristicNotification(dataServiceUUID, dataValueCharacteristicUUID)
  bluetooth.subscribeForCharacteristicNotification(dataServiceUUID, dataTimestampCharacteristicUUID)

  bluetooth.subscribeForConnectionStateChanges(::bluetoothConnectionStatusCbk)
  bluetooth.subscribeForWriteCharactEvent(commands::SendCallback)
  bluetooth.subscribeForDataNotifEvent(commands::NotificationCallback)
}

/**
 * Tries to connect to the bluetooth device that matches parameter name
 * @param deviceName name of the device
 */
fun SharedViewModel.bluetoothConnectToDevice(){
  bluetooth.disconnectFromCurrentConnectedDevice()
  btScanTimeoutTimer.cancel()
  btScanTimeoutTimer.start()

  Timer("Bluetooth scan start", false).schedule(800){
    viewModelScope.launch{
      _homeView_accessText.value = "Procurando dispositivo..."
      bluetooth.startScan(scanCallback)}
  }
}

/**
 * Stops the bluetooth scan process
 */
fun SharedViewModel.stopBluetoothScan(){
  bluetooth.stopScan(scanCallback)
}

/**
 * Callback that will be called by the bluetooth module when the connection
 *  state changes
 * @param status informs the connection status according to the BluetoothProfile definitions
 * @param deviceName name of the affected device fo identification purposes
 */
fun SharedViewModel.bluetoothConnectionStatusCbk(status: Int, deviceName: String){
  when(status){
    BluetoothProfile.STATE_DISCONNECTED->{
      viewModelScope.launch{
        if(isConnectionTimerOn == true && connectionRetryTimes < 10)
        {
          btConnectionStabilityTimer.cancel()
          isConnectionTimerOn = false
          bluetoothConnectToDevice()
        }
        else{
          commands.informConnectionWasLost()
        }
      }
      Log.w("Bluetooth", "User knows device ${deviceName} was disconnected")
    }
    BluetoothProfile.STATE_CONNECTED->{
      viewModelScope.launch{
        currentConnectedDeviceName = deviceName
        _connectionView_navDirection.value = R.id.action_connectionScreen_to_commandsScreen
        _connectionView_navDirection.value = null
      }
      Log.w("Bluetooth", "User knows device ${deviceName} is now connected")
    }
    else->{
      viewModelScope.launch{
        if(isConnectionTimerOn == true && connectionRetryTimes < 10)
        {
          btConnectionStabilityTimer.cancel()
          isConnectionTimerOn = false
          bluetoothConnectToDevice()
        }
        else{
          currentConnectedDeviceName = null
          commands.informConnectionWasLost()
          _homeView_accessText.value = "Falhou conexão com o dispositivo"
        }
      }
      Log.w("Bluetooth", "User knows device ${deviceName} failed to connect")
    }
  }
}
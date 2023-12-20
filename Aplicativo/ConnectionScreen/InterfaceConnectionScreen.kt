
/**
 * \file InterfaceConnectionScreen.kt
 * This file contains the Kotlin interface for the connection screen
 *  It's declared here all the functions and variables that connects the connection screen
 *  module with a project
 */

package com.tcc.views.interfaceconnectionscreen

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData

/**
 * Interface of the ConnectionScreen fragment
 *  ViewModels using this screen need to implement these functions and variables:
 */
interface InterfaceConnectionScreen{
  /*******************************************************************************
   * VARIABLES
   ******************************************************************************/
  val _connectionView_tvAppName: MutableLiveData<String>
  val connectionView_tvAppName: LiveData<String>
    get() = _connectionView_tvAppName

  val _connectionView_accessButtonText: MutableLiveData<String>
  val connectionView_accessButtonText: LiveData<String>
    get() = _connectionView_accessButtonText

  val _connectionView_accessText:MutableLiveData<String>
  val connectionView_accessText: LiveData<String>
    get() = _connectionView_accessText

  val _connectionView_navDirection: MutableLiveData<Int?>
  val connectionView_navDirection: LiveData<Int?>
    get() = _connectionView_navDirection

  val _connectionView_concedePermission: MutableLiveData<String?>
  val connectionView_concedePermission: LiveData<String?>
    get() = _connectionView_concedePermission

  val _connectionView_enableIntent: MutableLiveData<String?>
  val connectionView_enableIntent: LiveData<String?>
    get() = _connectionView_enableIntent

  /*******************************************************************************
   * FUNCTIONS
   ******************************************************************************/
  /**
   * It's called when the user presses the connection button
   */
  fun connectionView_BtnAccessButtonPress()

  /**
   * It's called on fragment creation, can be used to initialize ViewModel modules
   */
  fun initViewModel()
}
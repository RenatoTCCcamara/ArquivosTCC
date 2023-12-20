/**
 * \file InterfaceCommandsScreen.kt
 * Interface of the commands screen fragment
 *  The fragment will utilize these variables and functions.
 *  A programmer using this screen has to implement a ViewModel that is a child
 *   of this interface and build its Hilt module
 */

package com.tcc.views.interfacecommandsscreen

import android.widget.AdapterView
import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData

/**
 * Interface of the commandsScreen fragment
 *  ViewModels using this screen need to implement these functions and variables:
 */
interface InterfaceCommandsScreen
{
  /*******************************************************************************
   * VARIABLES
   ******************************************************************************/
  val _cmdScreenView_tvAppName: MutableLiveData<String>
  val cmdScreenView_tvAppName: LiveData<String>
    get() = _cmdScreenView_tvAppName

  val _cmdScreenView_navDirection: MutableLiveData<Int?>
  val cmdScreenView_navDirection: LiveData<Int?>
    get() = _cmdScreenView_navDirection

  val cmdScreenView_dropDownList: MutableLiveData<MutableList<String>>
  val cmdScreenView_dropDownAdapter: AdapterView.OnItemSelectedListener

  val _cmdScreenView_tvDropdownTitle: MutableLiveData<String>
  val cmdScreenView_tvDropdownTitle: LiveData<String>
    get() = _cmdScreenView_tvDropdownTitle

  val _cmdScreenView_tvDropdownResult: MutableLiveData<String>
  val cmdScreenView_tvDropdownResult: LiveData<String>
    get() = _cmdScreenView_tvDropdownResult

  val _cmdScreenView_button1Text: MutableLiveData<String>
  val cmdScreenView_button1Text: LiveData<String>
    get() = _cmdScreenView_button1Text

  val _cmdScreenView_button1Enabled: MutableLiveData<Boolean?>
  val cmdScreenView_button1Enabled: LiveData<Boolean?>
    get () = _cmdScreenView_button1Enabled

  val _cmdScreenView_button2Text: MutableLiveData<String>
  val cmdScreenView_button2Text: LiveData<String>
    get() = _cmdScreenView_button2Text

  val _cmdScreenView_button2Enabled: MutableLiveData<Boolean?>
  val cmdScreenView_button2Enabled: LiveData<Boolean?>
    get () = _cmdScreenView_button2Enabled

  val _cmdScreenView_button3Text: MutableLiveData<String>
  val cmdScreenView_button3Text: LiveData<String>
    get() = _cmdScreenView_button3Text

  val _cmdScreenView_button3Enabled: MutableLiveData<Boolean?>
  val cmdScreenView_button3Enabled: LiveData<Boolean?>
    get () = _cmdScreenView_button3Enabled

  /*******************************************************************************
   * FUNCTIONS
   ******************************************************************************/
  /**
   * Function called when button 1 is pressed
   */
  fun cmdScreenView_Btn1Press()

  /**
   * Function called when button 2 is pressed
   * @param input1Text text present on the input text field 1
   */
  fun cmdScreenView_Btn2Press(input1Text: String)

  /**
   * Function called when button 3 is pressed
   */
  fun cmdScreenView_Btn3Press()

  /**
   * It's called on fragment creation, can be used to initialize ViewModel modules
   */
  fun initViewModel()
}
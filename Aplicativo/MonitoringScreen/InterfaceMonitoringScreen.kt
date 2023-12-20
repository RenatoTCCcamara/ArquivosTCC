/**
 * \file InterfaceMonitoringScreen.kt
 *
 * Interface of the monitoring screen fragment
 *  The fragment will utilize these variables and functions.
 *  A programmer using this screen has to implement a ViewModel that is a child
 *   of this interface and build its Hilt module
 */
package com.tcc.views.interfaceomonitoringscreen

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import com.jjoe64.graphview.series.DataPoint
import com.jjoe64.graphview.series.LineGraphSeries


/**
 * Interface of the MonitoringScreen fragment
 */
interface InterfaceMonitoringScreen{
    /*******************************************************************************
     * VARIABLES
     ******************************************************************************/
    val _monitoringScreenView_tvAppName: MutableLiveData<String>
    val monitoringScreenView_tvAppName: LiveData<String>
        get() = _monitoringScreenView_tvAppName

    val _monitoringScreenView_navDirection: MutableLiveData<Int?>
    val monitoringScreenView_navDirection: LiveData<Int?>
        get() = _monitoringScreenView_navDirection

    val _monitoringScreenView_button1Text: MutableLiveData<String>
    val monitoringScreenView_button1Text: LiveData<String>
        get() = _monitoringScreenView_button1Text

    val _monitoringScreenView_graphData: MutableLiveData<LineGraphSeries<DataPoint>>
    val monitoringScreenView_graphData: LiveData<LineGraphSeries<DataPoint>>
        get() = _monitoringScreenView_graphData
    /*******************************************************************************
     * FUNCTIONS
     ******************************************************************************/
    /**
     * It's called when the user presses the button at the center of the screen
     */
    fun opsScreenView_Btn1Press()

    /**
     * It's called on fragment creation, can be used to initialize ViewModel modules
     */
    fun initViewModel()
}
/**
 * \file monitoringScreen.kt
 *
 * Fragment of the screen where user can see a graph of the temperature inside the chamber
 */

package com.tcc.views.monitoringscreen

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.navigation.fragment.findNavController
import com.tcc.views.interfaceomonitoringscreen.InterfaceMonitoringScreen
import com.tcc.views.monitoringscreen.databinding.FragmentMonitoringScreenBinding
import dagger.hilt.android.AndroidEntryPoint
import javax.inject.Inject

@AndroidEntryPoint
class MonitoringScreen : Fragment() {
    private var _binding: FragmentMonitoringScreenBinding? = null
    private val binding get() = _binding!!
    @Inject
    lateinit var viewModel: InterfaceMonitoringScreen

    /**
     * Invoked when fragment is created
     *  initiates the ViewModel
     */
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
    }

    /**
     * Invoked after fragment is created
     *  Inflates the layout
     */
    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        _binding = FragmentMonitoringScreenBinding.inflate(inflater, container, false)
        return binding.root
    }

    /**
     * Invoked when fragment layout is created
     *  Setup of view elements
     */
    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        viewModel.initViewModel()

        /* Nav direction */
        viewModel.monitoringScreenView_navDirection.observe(viewLifecycleOwner) { newValue ->
            if (newValue != null) {
                findNavController().navigate(newValue)
            }
        }

        /* TextViews */
        viewModel.monitoringScreenView_tvAppName.observe(viewLifecycleOwner) { newWord ->
            binding.tvAppName.text = newWord
        }

        /* Buttons texts*/
        viewModel.monitoringScreenView_button1Text.observe(viewLifecycleOwner) { newValue ->
            binding.button1.text = newValue
        }

        /* Graph data */
        binding.idGraphView.animate()
        viewModel._monitoringScreenView_graphData.observe(viewLifecycleOwner){newValue->
            binding.idGraphView.addSeries(newValue)
        }

        /* Buttons click functions */
        binding.button1.setOnClickListener {
            binding.button1.isEnabled = false
            viewModel.opsScreenView_Btn1Press()
            binding.button1.postDelayed({
                binding.button1.isEnabled = true
            }, 1000)
        }

    }
}
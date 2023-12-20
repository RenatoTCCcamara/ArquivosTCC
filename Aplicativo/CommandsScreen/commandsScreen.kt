/**
 * \file CommandsScreen.kt
 *
 * Fragment of the screen used to send commands to the embedded system
 */

package com.tcc.views.commandsscreen

import android.content.Intent
import android.content.pm.PackageManager
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ArrayAdapter
import androidx.appcompat.app.AlertDialog
import androidx.core.content.ContextCompat
import androidx.fragment.app.Fragment
import androidx.navigation.fragment.findNavController
import com.tcc.views.commandsscreen.databinding.FragmentCommandsScreenBinding
import com.tcc.views.interfacecommandsscreen.InterfaceCommandsScreen
import dagger.hilt.android.AndroidEntryPoint
import javax.inject.Inject

/**
 * Fragment where user can send special commands to device
 */
@AndroidEntryPoint
class commandsScreen : Fragment() {

    private var _binding: FragmentCommandsScreenBinding? = null
    private val binding get() = _binding!!
    @Inject lateinit var viewModel: InterfaceCommandsScreen

    /**
     * Invoked when fragment is created
     */
    override fun onCreate(savedInstanceState: Bundle?)
    {
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
        _binding = FragmentCommandsScreenBinding.inflate(inflater, container, false)
        return binding.root
    }

    /**
     * Invoked when fragment layout is created
     *  Setup of Buttons, Adapters, and observers.
     */
    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        viewModel.initViewModel()

        /* Nav direction */
        viewModel.cmdScreenView_navDirection.observe(viewLifecycleOwner){ newValue ->
            if(newValue != null){findNavController().navigate(newValue)}
        }

        /* TextViews */
        viewModel.cmdScreenView_tvAppName.observe(viewLifecycleOwner){newWord->
            binding.tvAppName.text = newWord
        }

        viewModel.cmdScreenView_tvDropdownTitle.observe(viewLifecycleOwner){newWord->
            binding.tvTitleDropdown.text = newWord
        }

        viewModel.cmdScreenView_tvDropdownResult.observe(viewLifecycleOwner){newWord->
            binding.tvResultDropdown.text = newWord
        }

        /* Buttons */
        viewModel.cmdScreenView_button1Text.observe(viewLifecycleOwner){newWord->
            binding.button1.text = newWord
        }

        viewModel.cmdScreenView_button2Text.observe(viewLifecycleOwner){newWord->
            binding.button2.text = newWord
        }

        viewModel.cmdScreenView_button3Text.observe(viewLifecycleOwner){newWord->
            binding.button3.text = newWord
        }

        viewModel.cmdScreenView_button1Enabled.observe(viewLifecycleOwner){newValue ->
            if(newValue == true)
            {
                binding.button1.isEnabled = true
            }
            else if(newValue == false)
            {
                binding.button1.isEnabled = false
            }
        }

        viewModel.cmdScreenView_button2Enabled.observe(viewLifecycleOwner){newValue ->
            if(newValue == true)
            {
                binding.button2.isEnabled = true
            }
            else if(newValue == false)
            {
                binding.button2.isEnabled = false
            }
        }

        viewModel.cmdScreenView_button3Enabled.observe(viewLifecycleOwner){newValue ->
            if(newValue == true)
            {
                binding.button3.isEnabled = true
            }
            else if(newValue == false)
            {
                binding.button3.isEnabled = false
            }
        }

        binding.button1.setOnClickListener {
            viewModel.cmdScreenView_Btn1Press()
        }

        binding.button2.setOnClickListener {
            viewModel.cmdScreenView_Btn2Press(binding.textInput1.text.toString())
        }

        binding.button3.setOnClickListener {
            viewModel.cmdScreenView_Btn3Press()
        }

        /* Dropdown */
        if(viewModel.cmdScreenView_dropDownList.value != null)
        {
            val dropDownAdapter: ArrayAdapter<String> =
                ArrayAdapter<String>(requireContext(), android.R.layout.simple_spinner_item, viewModel.cmdScreenView_dropDownList.value!!)
            dropDownAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
            _binding?.dropdownList?.adapter = dropDownAdapter
            _binding?.dropdownList?.onItemSelectedListener = viewModel.cmdScreenView_dropDownAdapter

        }
    }

    /**
     * Invoked when view is destroyed
     *  Free binding
     */
    override fun onDestroyView() {
        _binding = null
        super.onDestroyView()
    }
}
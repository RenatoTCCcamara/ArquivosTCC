/**
 * \file ConnectionScreen.kt
 * This file contains the Kotlin fragment of the connection screen
 *  The connection screen is the initial screen of an app and it contains a button
 *  that connects to the climate chamber
 */

package com.tcc.views.connectionscreen

import android.content.Intent
import android.content.pm.PackageManager
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.appcompat.app.AlertDialog
import androidx.core.content.ContextCompat
import androidx.navigation.fragment.findNavController
import com.tcc.views.connectionscreen.databinding.FragmentConnectionScreenBinding
import com.tcc.views.interfaceconnectionscreen.InterfaceConnectionScreen
import dagger.hilt.android.AndroidEntryPoint
import javax.inject.Inject

const val LOCATION_PERMISSION_REQUEST_CODE = 2

@AndroidEntryPoint
class ConnectionScreen : Fragment() {

  private var _binding: FragmentConnectionScreenBinding? = null
  private val binding get() = _binding!!
  @Inject lateinit var viewModel: InterfaceConnectionScreen

    override fun onCreate(savedInstanceState: Bundle?) {
    super.onCreate(savedInstanceState)
  }

  override fun onCreateView(
    inflater: LayoutInflater, container: ViewGroup?,
    savedInstanceState: Bundle?
  ): View? {

    _binding = FragmentConnectionScreenBinding.inflate(inflater, container, false)
    return binding.root
  }

  override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
    super.onViewCreated(view, savedInstanceState)
    viewModel.initViewModel()

    viewModel.connectionView_tvAppName.observe(viewLifecycleOwner){newWord->
      binding.appName.text = newWord
    }

    viewModel.connectionView_accessText.observe(viewLifecycleOwner){newWord->
      binding.accessText.text = newWord
    }

    viewModel.connectionView_navDirection.observe(viewLifecycleOwner){newValue->
      if(newValue != null){findNavController().navigate(newValue)}
    }

    viewModel.connectionView_accessButtonText.observe(viewLifecycleOwner){ newWord->
      binding.accessButton.text = newWord
    }

    binding.accessButton.setOnClickListener {
      binding.accessButton.isEnabled = false
      binding.accessButton.postDelayed(
        {
          binding.accessButton.isEnabled = true
        }
      ,1000)

      viewModel.connectionView_BtnAccessButtonPress()
    }

    /* Intents and persmissions */
    viewModel.connectionView_enableIntent.observe(viewLifecycleOwner){newValue->
      if(newValue != null){enableIntent(newValue)}
    }
    viewModel.connectionView_concedePermission.observe(viewLifecycleOwner){ newValue ->
      if(newValue != null){requestPermission(newValue)}
    }
  }

  /**
   * Request user to concede a permission
   * @param permission String code of the permission being requested
   */
  private fun requestPermission(permission: String)
  {
    if (ContextCompat.checkSelfPermission(
        requireContext(),
        permission
      ) != PackageManager.PERMISSION_GRANTED
    )
    {
      val builder =
        AlertDialog.Builder(requireContext())
      builder.setTitle("Conceder permissão para o aplicativo")
      builder.setMessage("Por favor conceda a permissão a seguir para o aplicativo funcionar corretamente")
      builder.setPositiveButton("ok", null)
      builder.setOnDismissListener {
        requestPermissions(
          arrayOf(permission),
          LOCATION_PERMISSION_REQUEST_CODE
        )
      }
      builder.show()
    }
  }

  /**
   * Intent that asks for user to enable a function
   * @param intentRequest string code of the intent request
   */
  private fun enableIntent(intentRequest: String)
  {
    val enableIntent = Intent(intentRequest)
    startActivityForResult(enableIntent, 1)
  }

}
<?php
// $Id$

/**
 * @file
 * Holds all the block admin form alter functions.
 */

/**
 * Alter the block_admin_configure form for minimal display.
 */
function _simpleblocks_form_block_admin_configure_alter_minimal(&$form, &$form_state) {
  // Disable/hide certain form elements
  $disabled_elements = array(
    'user_vis_settings' => array(),
    'role_vis_settings' => array(),
    'page_vis_settings' => array(),
    'block_settings' => array('info'),
  );
  _simpleblocks_disable_elements($disabled_elements, $form);

  // Unset some form settings.
  unset($form['block_settings']['#type']);

  // Check to see if the name 'Block' should be changed throughout the form.
  $block_name = variable_get('simpleblocks_block_name', 'Block');
  $args = array('@block' => $block_name);
  if ($block_name != 'Block') {
    $form['block_settings']['title']['#title'] = t('@block title', $args);
  }

  $form['simpleblocks']['title_enabled'] = array(
    '#type' => 'checkbox',
    '#value' => ($form['block_settings']['title']['#default_value'] != '<none>'),
    '#attributes' => array('id' => 'test'),
  );
  $form['block_settings']['title']['#description'] = t('Specify a title for this @block.  Leave blank for the default title, or uncheck to disable completely.', $args);
}

/**
 * 
 */
function theme_simpleblocks_block_admin_configure(&$form) {
  $form['block_settings']['title']['#prefix'] = drupal_render($form['simpleblocks']['title_enabled']);
  $output = drupal_render($form['simpleblocks']['status']);
  $output .= drupal_render($form);
  return $output;
}

/**
 * Helper function to disable (hide) certain form elements from the 
 * configure form.
 */
function _simpleblocks_disable_elements($elements, &$form) {
  if (is_array($elements)) {
    foreach($elements as $key => $items) {
      if (is_array($items)) {
        if (empty($items)) {
          unset($form[$key]['#type']);
          foreach(element_children($form[$key]) as $child_key) {
            $form[$key][$child_key]['#type'] = 'value';
            $form[$key][$child_key]['#value'] = $form[$key][$child_key]['#default_value'];
          }
        }
        else {
          foreach($items as $child_key) {
            $form[$key][$child_key]['#type'] = 'value';
            $form[$key][$child_key]['#value'] = $form[$key][$child_key]['#default_value'];
          }
        }
      }
      else {
        $form[$item]['#type'] = 'value';
        $form[$item]['#value'] = $form[$item]['#default_value'];
      }
    }
  }
}
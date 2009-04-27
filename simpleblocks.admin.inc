<?php
// $Id$

/**
 * @file
 * Holds all administration settings forms and functions.
 */

/**
 * Permissions form
 */
function simpleblocks_permissions_form() {
  $form = array();

  $roles = user_roles();
  $blocks = _block_rehash();
  $block_roles = simpleblocks_roles_load_all();

  $form['perms']['#tree'] = TRUE;

  foreach($blocks as $block) {
    $key = $block['module'] .'_'. $block['delta'];
    //$all_key = $block['module'] .'_alldeltas';

    $form['perms'][$key]['#tree'] = TRUE;

    $form['block_names'][$key]['name']['#type'] = 'markup';
    $form['block_names'][$key]['name']['#value'] = $block['info'];

    /*
    $form['perms'][$all_key][$rid] = array(
      '#type' => 'checkbox',
      '#default_value' => in_array($rid, (array) $block_roles[$all_key]),
    );
    */

    foreach ($roles as $rid => $role) {
      $form['perms'][$key][$rid] = array(
        '#type' => 'checkbox',
        '#default_value' => !in_array($rid, (array) $block_roles[$key]),
      );
    }
  }

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save permissions'));

  return $form;
}


/**
 * Submit handler for simpleblocks_permissions_form
 */
function simpleblocks_permissions_form_submit($form, &$form_state) {
  $perms = $form_state['values']['perms'];

/*
  ob_start();
  print_r($perms);
  drupal_set_message(ob_get_contents());
  ob_end_clean();
*/

  foreach ($perms as $key => $roles) {
    list($module, $delta) = explode('_', $key);

    // Delete all for this module and delta.
    db_query("DELETE FROM {simpleblocks_roles} WHERE module = '%s' AND delta = '%s'", $module, $delta);
    foreach ($roles as $rid => $value) {
      // If the permission is set, save it into the table.
      if (!$value) {
        db_query("INSERT INTO {simpleblocks_roles} (module, delta, rid) VALUES ('%s', '%s', %d)", $module, $delta, $rid);
      }
    }
  }

  drupal_set_message(t('The changes have been saved.'));
}

/**
 * Theme the simpleblocks administer permissions page.
 *
 * @ingroup themeable
 */
function theme_simpleblocks_permissions_form($form) {
  $roles = user_roles();
  $rows = $header = array();
  $ouptut = '';

  $header[] = (t('Block'));
  foreach ($roles as $rid => $name) {
    $header[] = $name;
  }

  foreach(element_children($form['perms']) as $key) {
    $row = array();
    $row[] = drupal_render($form['block_names'][$key]['name']);
    foreach($roles as $rid => $name) {
      $row[] = drupal_render($form['perms'][$key][$rid]);
    }

    $module = array_shift(explode('_', $key));
    $modules[$module][] = $row;
  }

  $all_rows = array();
  $colspan = count($roles) + 1;

  foreach($modules as $module => $rows) {
    $name = t('all @module blocks', array('@module' => $module));
    $all_rows[] = array(array('data' => $name, 'colspan' => $colspan, 'class' => 'module'));
    foreach($rows as $row) {
      $all_rows[] = $row;
    }
  }

  $output .= theme('table', $header, $all_rows, array('id' => 'simpleblocks-permissions-'. $module));
  $output .= drupal_render($form);

  return $output;
}
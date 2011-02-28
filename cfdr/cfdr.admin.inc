<?php
/**
 * @file Contains the settings form
 */

/**
 * Admin form called from path 'admin/settings/cfdr'
 **/
function cfdr_admin_form() {
  $form['cfdr']['cfdr_scan_path'] = array(
    '#title' => t('Path where to look for .xml files'),
    '#type' => 'textfield',
    '#description' => t('It is recomended to use a restrictive ".htaccess" file in this directory'),
    '#default_value' => variable_get('cfdr_scan_path', 'sites/default/cfd'),
  );
  return system_settings_form($form);
}

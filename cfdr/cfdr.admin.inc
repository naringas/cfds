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
  $form['cfdr']['cfdr_move_path'] = array(
    '#title' => t('Path where to move scanned .xml files'),
    '#type' => 'textfield',
    '#description' => t('Must be different from <strong>Path where to look for .xml files</strong>'),
    '#default_value' => variable_get('cfdr_move_path', 'sites/default/cfd/scanned'),
  );
  return system_settings_form($form);
}

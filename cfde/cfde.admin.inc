<?php
/**
 * @file Contains the settings form
 */

/**
 * Admin form called from path 'admin/settings/cfde'
 **/
function cfde_admin_form() {
  // Cron job settings
  $form['cfde']['zip'] = array(
    '#type' => 'fieldset',
    '#title' => t('Automatic ZIP file import'),
    '#description' => t('Automatic ZIP file import with cron.'),
  );
  $form['cfde']['zip']['cfde_zip_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#description' => t('Enable automatic ZIP import'),
    '#default_value' => variable_get('cfde_zip_enable', FALSE),
  );
  $form['cfde']['zip']['cfde_zip_filename'] = array(
    '#title' => t('ZIP file name'),
    '#type' => 'textfield',
    '#description' => t('without ".zip"'),
    '#default_value' => variable_get('cfde_zip_filename', 'cfd'),
  );
  $form['cfde']['zip']['cfde_zip_path'] = array(
    '#title' => t('Path where to look for "ZIP file name"'),
    '#type' => 'textfield',
    '#description' => t('It is recomended to use a restrictive ".htaccess" file in this directory'),
    '#default_value' => variable_get('cfde_zip_path', 'sites/default/cfd'),
  );
  $form['cfde']['user'] = array(
    '#type' => 'fieldset',
    '#title' => t('User Registration'),
    '#description' => t('These restrictions are ignored if user has "administer users" permision')
  );
  $form['cfde']['user']['cfde_username_validate'] = array(
    '#type' => 'checkbox',
    '#title' => t('Validate RFC on user registration'),
    '#description' => t('Prevents malformed RFCs from being usernames. Only checks RFC plausibility.'),
    '#default_value' => variable_get('cfde_username_validate', TRUE),
  );
  $form['cfde']['user']['cfde_register_existing_rfc'] = array(
    '#type' => 'checkbox',
    '#title' => t('Prevent registration of users without assigned CFDs'),
    '#default_value' => variable_get('cfde_register_existing_rfc', FALSE),
  );
  return system_settings_form($form);
}

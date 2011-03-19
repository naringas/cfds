<?php
/**
 * @file Contains the settings form
 */

/**
 * Admin form called from path 'admin/settings/cfde'
 **/
function cfde_admin_form() {
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
  $form['cfde']['expire'] = array(
    '#type' => 'fieldset',
    '#title' => t('Emitted CFD node expiration'),
    '#description' => t('Automatic node deletion with cron.'),
  );
  $form['cfde']['expire']['cfde_expire_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum node age <em>in seconds</em>'),
    '#description' => t("Nodes older than these seconds will be deleted. Based on node creation date (not CFD emission date).<br /><em>0 seconds</em> disables node expiration."),
    '#default_value' => variable_get('cfde_expire_time', 3024000),
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

/**
 * Returns cfds for administration
 * menu path 'cfde/admin'
 *
 **/
function cfde_admin_cfds() {
  $page = isset($_GET['page']) ? $_GET['page'] : 0;

  // $basic_query = 'SELECT {cfde}.nid, {cfde}.fecha_cfd, {node}.title, {node}.created FROM {cfde} INNER JOIN {node} ON {cfde}.nid = {node}.nid WHERE {node}.created >= %d AND {node}.created <= %d ORDER BY {node}.title ASC';
  $start = date_format(date_make_date(date('Y-m-d')), DATE_FORMAT_UNIX);
  $start -= ($page * 86400);
  $end = $start + 86399; // 86399 = 1 day - 1 second

  $results = db_query('SELECT {cfde}.nid, {cfde}.fecha_cfd, {node}.title, {node}.created FROM {cfde} INNER JOIN {node} ON {cfde}.nid = {node}.nid  WHERE {node}.created >= %d AND {node}.created <= %d ORDER BY {node}.title ASC', $start, $end);

  // construct the list
  $list = array();
  while ($cfd = db_fetch_object($results)) {
    $list['title'][$cfd->nid] = array('#value' => l($cfd->title, "node/$cfd->nid"));
    $list['date'][$cfd->nid] = array('#value' => date_format_date(date_make_date($cfd->fecha_cfd), 'medium'));
    $list['links'][$cfd->nid] = array('#value' => cfde_get_dl_links($cfd->title, $cfd->nid));
  }
  // $list['pager'] = array('#value' => theme('pager', NULL, 35, 0));
  $form['list'] = $list;
  $form['#theme'] = 'cfde_view_cfds';

  // Custom Pager
  if ($page > 0) {
    $go_back = '
      <li class="pager-first first"><a class="pager-first" href="?page=0">« hoy</a></li>
      <li class="pager-previous"><a href="?page='. ($page-1) . '">‹ día después</a></li>';
  }
  $date_display = date_format_date(date_make_date($start, NULL, DATE_UNIX), 'custom', 'l, j M Y');
  $form['pagerito'] = array('#value' =>
    '<div class="item-list"><ul class="pager">' .
      $go_back .
      '<li class="pager-current">' . $date_display . '</li>' .
      '<li class="pager-next"><a href="?page='. ($page+1) . '">día antes ›</a></li>
    </ul></div>'
  );
  return $form;
}

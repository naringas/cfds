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
  // filters form: cfde_admin_filter_form()
  drupal_add_js(libraries_get_path('datepick') . '/jquery.datepick.min.js');
  drupal_add_js(libraries_get_path('datepick') . "/jquery.datepick-es.js");
  drupal_add_css(drupal_get_path('module', 'cfd') . '/cfd_datepick.css');
  drupal_add_js('$(function() {$("#edit-created, #edit-fecha-cfd").datepick({minDate: "-7w", maxDate: "+0", pickerClass: "noPrevNext", showOtherMonths: true, selectOtherMonths: true, dateFormat: $.datepick.ISO_8601, showAnim: "fadeIn", showSpeed: "fast", showOnFocus: true }); });', 'inline');

  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filters'),
    '#collapsible' => TRUE,
  );
  $form['filters']['cliente_rfc'] = array(
    '#type' => 'textfield',
    '#title' => t('Client RFC'),
    '#size' => 14,
    '#maxlength' => 13,
    '#default_value' => $_SESSION['admin_filter']['cliente_rfc'],
  );
  $form['filters']['title'] = array(
    '#type' => 'textfield',
    '#title' => 'Serie + Folio',
    '#size' => 15,
    '#default_value' => $_SESSION['admin_filter']['title'],
  );
  $form['filters']['created'] = array(
    '#type' => 'textfield',
    '#title' => 'Fecha del nodo',
    '#size' => 11,
    '#maxlength' => 10,
    '#description' => t('Format: yyyy-mm-dd'),
    '#default_value' => substr($_SESSION['admin_filter']['created'], 0, 10),
  );
  $form['filters']['fecha_cfd'] = array(
    '#type' => 'textfield',
    '#title' => 'Fecha del CFD',
    '#size' => 11,
    '#maxlength' => 10,
    '#description' => t('Format: yyyy-mm-dd'),
    '#default_value' => substr($_SESSION['admin_filter']['fecha_cfd'], 0, 10),
  );
  $form['filters']['filter'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );
  $form['filters']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
  );
  // $form['filters']['#theme'] = 'cfde_admin_filter_form';
  $form['#submit'][] = 'cfde_admin_filter_form_submit';


  //Construct Query
  $query = 'SELECT {cfde}.nid, {cfde}.cliente_rfc, {cfde}.fecha_cfd, {node}.title, {node}.created FROM {cfde} INNER JOIN {node} ON {cfde}.nid = {node}.nid';
  if (isset($_SESSION['admin_filter'])) {
    $join = ' WHERE ';
    foreach ($_SESSION['admin_filter'] as $filter => $value) {
      switch ($filter) {
        case 'cliente_rfc':
          $query .= $join . $filter . " = '$value'";
          $join = ' AND ';
          break;

        case 'title':
          $query .= $join . $filter . " LIKE '%$value%'";
          $join = ' AND ';
          break;

        case 'created':
          $value = date_make_date($value, DATE_DATETIME);
          $start = $value->format('U');
          $end = $value->modify('+23 hours 59 minutes 59 seconds')->format('U');

          $query .= $join . $filter . " >= $start AND " . $filter . " <= $end";
          $join = ' AND ';
          break;

        case 'fecha_cfd':
          $end = date_make_date($value, DATE_DATETIME);
          $end = substr($end->modify('+23 hours 59 minutes 59 seconds')->format('c'), 0, 19);

          $query .= $join . $filter . " >= '$value' AND " . $filter . " <= '$end'";
          $join = ' AND ';
          break;
      }
    }
  }

  // Is same as $header in theme funcion
  $header = array(
    array('data' => t('Uploaded'), 'field' => 'created', 'sort' => 'DESC'),
    array('data' => t('FE #'), 'field' => 'title'),
    array('data' => t('Date'), 'field' => 'fecha_cfd'),
    array('data' => t('Client RFC'), 'field' => 'cliente_rfc'),
  );
  $per_page = 60; // results per page (for pager)
  $results = pager_query($query . tablesort_sql($header), $per_page);

  // construct the list
  $list = array();
  while ($cfd = db_fetch_object($results)) {
    $list['created'][$cfd->nid] = array('#value' => date_format_date(date_make_date($cfd->created, NULL, DATE_UNIX), 'short'));
    $list['title'][$cfd->nid] = array('#value' => l($cfd->title, "node/$cfd->nid"));
    $list['date'][$cfd->nid] = array('#value' => date_format_date(date_make_date($cfd->fecha_cfd), 'short'));
    $list['cliente_rfc'][$cfd->nid] = array('#value' => $cfd->cliente_rfc);
  }
  $list['pager'] = array('#value' => theme('pager', NULL, $per_page, 0));
  $form['list'] = $list;
  $form['#theme'] = 'cfde_admin_cfds';

  return $form;
}

function cfde_admin_filter_form_submit($form, &$form_state) {
  $cliente_rfc = $form_state['values']['cliente_rfc'];
  $created = $form_state['values']['created'];
  $fecha_cfd = $form_state['values']['fecha_cfd'];
  $title = $form_state['values']['title'];
  if ($form_state['values']['op'] == t('Reset')) {
    unset($_SESSION['admin_filter']);
    return;
  }

  if ($cliente_rfc) $_SESSION['admin_filter']['cliente_rfc'] = $cliente_rfc;
  else unset($_SESSION['admin_filter']['cliente_rfc']);

  if ($created) $_SESSION['admin_filter']['created'] = date_format(date_make_date($created), DATE_FORMAT_ISO);
  else unset($_SESSION['admin_filter']['created']);

  if ($fecha_cfd) $_SESSION['admin_filter']['fecha_cfd'] = date_format(date_make_date($fecha_cfd), DATE_FORMAT_ISO);
  else unset($_SESSION['admin_filter']['fecha_cfd']);

  if ($title) $_SESSION['admin_filter']['title'] = $title;
  else unset($_SESSION['admin_filter']['title']);
}
/**
 * Themes cfde_admin_cfds() in content management style
 * based on theme_node_admin_nodes() in core - node.admin.inc
 **/
function theme_cfde_admin_cfds($form) {
  // If there are rows in this form, then $form['title'] contains a list of
  // the title form elements.
  $has_posts = isset($form['list']['title']) && is_array($form['list']['title']);

  $header = array(
    array('data' => t('Uploaded'), 'field' => 'created', 'sort' => 'DESC'),
    array('data' => t('FE #'), 'field' => 'title'),
    array('data' => t('Date'), 'field' => 'fecha_cfd'),
    array('data' => t('Client RFC'), 'field' => 'cliente_rfc'),
  );

  $output = drupal_render($form['filters']);
  // $output .= drupal_render($form['pagerito']);

  if ($has_posts) {
    foreach (element_children($form['list']['title']) as $key) {
      $row = array();
      $row[] = drupal_render($form['list']['created'][$key]);
      $row[] = drupal_render($form['list']['title'][$key]);
      $row[] = drupal_render($form['list']['date'][$key]);
      $row[] = drupal_render($form['list']['cliente_rfc'][$key]);
      $rows[] = $row;
    }
  }
  else {
    $rows[] = array(array('data' => t('No CFDs available.'), 'colspan' => '4'));
  }

  $output .= theme('table', $header, $rows);
  if ($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }

  $output .= drupal_render($form);

  return $output;
}

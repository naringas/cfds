<?php
/*
 * @file Provies functionality for importing CFDs as zipped XMLs files.
 */

function cfde_import_form() {
  $form['#attributes']['enctype'] = 'multipart/form-data';
  $form['zip_file'] = array(
    '#type' => 'file',
    '#title' => t('Select a @type file', array('@type' => 'ZIP')),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  $form['#redirect'] = 'cfde/admin';

  return $form;
}

function cfde_import_form_validate($form, &$form_state) {
  $upload_size = $_FILES['files']['size']['zip_file'];
  $file_type = $_FILES['files']['type']['zip_file'];
//  dvm($file_type);
  if ($upload_size <= 0 || $file_type != 'application/x-zip-compressed')
    form_set_error('zip_file', t('You must submit a @type file', array('@type' => 'ZIP')));
}

function cfde_import_form_submit($form, &$form_state) {
  $file_name = $_FILES['files']['tmp_name']['zip_file'];
  $zip_file_name = $_FILES['files']['name']['zip_file'];

  if ($count = cfde_import_zip($file_name)) {
    drupal_set_message(t('@num CFDs imported from @filename', array('@num' => $count, '@filename' => $zip_file_name)), 'status');
  }
  else {
    drupal_set_message(t('@file not imported succesfully', array('@file' => $zip_file_name)), 'error');
  }
}

/**
 * Opens a zip file and creates nodes for each .xml file
 * does not validate using the schema, only checks for file extension.
 * doesn not call watchdog.
 * @param string $file_name the zip file to open
 * @return Number of xml files imported, FALSE if the file couldn't be opened
 **/
function cfde_import_zip($file_name) {
  global $user;
  $zip = zip_open($file_name);
  if (is_resource($zip)) {
    $count = 0;
    while ($zip_entry = zip_read($zip)) {
      // check for .xml file extension
      if (substr(zip_entry_name($zip_entry), -4) == '.xml') {
        zip_entry_open($zip, $zip_entry);
        $xml = zip_entry_read($zip_entry, zip_entry_filesize($zip_entry));
        zip_entry_close($zip_entry);

        $node = new stdClass();
        $node->type = 'cfde';
        //Sets the creator of these nodes to something.
        $node->uid = $user->uid;
        $node->xml_file = $xml;
        node_save($node);
        $count++;
        // drupal_set_message(t('@node saved', array('@node' => $node->title)));
      }
    }
  zip_close($zip);
  return $count;
  }
  else {
    return FALSE;
  }
}

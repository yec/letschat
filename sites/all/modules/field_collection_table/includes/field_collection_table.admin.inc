<?php

/**
 * @file
 * Administrative page callbacks for the Field Collection Table module.
 */

/**
 * Builds and returns the book settings form.
 */
function field_collection_table_admin_settings() {
  $form['field_collection_table_format_form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Format forms as table'),
    '#description' => t('If checked, all Field Collection forms will be formatted as tables.'),
    '#default_value' => variable_get('field_collection_table_format_form', TRUE),
  );
  $form['field_collection_table_hide_empty'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide empty columns'),
    '#description' => t('If checked, hide empty Field Collection table columns.'),
    '#default_value' => variable_get('field_collection_table_hide_empty', FALSE),
  );

  return system_settings_form($form);
}

<?php

/**
 * Print a single row of multiple fields.
 */
function theme_field_collection_table_multiple_value_field($variables) {
  $element = $variables['element'];
  $header = array();
  $cells = array();
  foreach (field_info_instances($element['#entity_type'], $element['#bundle']) as $field_name => $instance) {
    $field = $element[$field_name];
    if (empty($field)) {
      continue;
    }
    if (isset($field['#language']) && isset($field[$field['#language']])) {
      $header[] = $field[$field['#language']]['#title'];
    }
    elseif (isset($field['#title'])) {
      $header[] = $field['#title'];
    }
    else {
      $header[] = '';
    }
    $cells[] = array('data' => $field);
    unset($element[$field_name]);
  }

  $element['field_collection_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => array($cells),
    '#weight' => 0,
  );

  $element['#sorted'] = FALSE;
  return drupal_render_children($element);
}

/**
 * Replacement for theme_field_multiple_value_form().
 *
 * Each field is printed in a separate cell.
 */
function theme_field_collection_table_multiple_value_fields($variables) {
  $element = $variables['element'];
  $output = '';

  if (isset($element['#cardinality']) && ($element['#cardinality'] > 1 || $element['#cardinality'] == FIELD_CARDINALITY_UNLIMITED)) {
    $table_id = drupal_html_id($element['#field_name'] . '_values');
    $order_class = $element['#field_name'] . '-delta-order';
    $required = !empty($element['#required']) ? '<span class="form-required" title="' . t('This field is required. ') . '">*</span>' : '';

    $rows = array();

    // Sort items according to '_weight' (needed when the form comes back after
    // preview or failed validation)
    $items = array();
    foreach (element_children($element) as $key) {
      if ($key === 'add_more') {
        $add_more_button = &$element[$key];
      }
      else {
        $items[] = &$element[$key];
      }
    }
    usort($items, '_field_sort_items_value_helper');

    $header = array(
      array(
        'data' => '<label>' . t('!title: !required', array('!title' => $element['#title'], '!required' => $required)) . "</label>",
        'class' => array('field-label'),
      ),
    );

    // Add the items as table rows.
    foreach ($items as $key => $item) {
      uasort($item, 'element_sort');
      $item['_weight']['#attributes']['class'] = array($order_class);
      $cells = array(
        array('data' => '', 'class' => array('field-multiple-drag')),
      );
      foreach (element_children($item) as $child) {
        if (isset($item[$child]['#language']) && $key == 0) {
          $header[] = array(
            'data' => '<label>' . t('!title', array('!title' => $item[$child][$item[$child]['#language']]['#title'])) . '</label>',
            'class' => array('field-label'),
          );
        }
        $cells[] = array('data' => $item[$child]);
      }
      $rows[] = array(
        'data' => $cells,
        'class' => array('draggable'),
      );
    }
    $header[] = t('Order');

    $output = array(
      '#prefix' => '<div class="form-item">',
      '#suffix' => '</div>',
    );
    $output[] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#attributes' => array(
        'id' => $table_id,
        'class' => array(
          'field-multiple-table',
        ),
      ),
    );
    if (!empty($element['#description'])) {
      $output[] = array(
        '#prefix' => '<div class="description">',
        '#suffix' => '</div>',
        '#markup' => $element['#description'],
      );
    }
    if (isset($add_more_button)) {
      $output[] = $add_more_button + array(
        '#prefix' => '<div class="clearfix">',
        '#suffix' => '</div>',
      );
    }

    $output = drupal_render($output);

    drupal_add_tabledrag($table_id, 'order', 'sibling', $order_class);
  }
  else {
    foreach (element_children($element) as $key) {
      $output .= drupal_render($element[$key]);
    }
  }

  return $output;
}

/**
 * Implements template_preprocess_HOOK__PATTERN().
 */
function template_preprocess_table__field_collection_table(&$variables) {
  if (empty($variables['settings'])) {
    return;
  }
  if (isset($variables['settings']['empty'])) {
    _field_collection_table_hide_empty($variables);
  }
}

/**
 * Remove columns that are entirely empty.
 */
function _field_collection_table_hide_empty(&$variables) {
  $rows = $variables['rows'];

  $count = array();
  foreach ($rows as $row_delta => $row) {
    foreach ($row['data'] as $column_delta => $column) {
      if (!isset($count[$column_delta])) {
        $count[$column_delta] = 0;
      }
      if (isset($column['data']['#empty'])) {
        $count[$column_delta]++;
      }
    }
  }
  foreach ($count as $column_delta => $column) {
    if ($column === count($rows)) {
      foreach ($rows as $row_delta => $row) {
        unset($variables['rows'][$row_delta]['data'][$column_delta]);
        unset($variables['header'][$column_delta]);
      }
    }
  }
}

/**
 * Derivative of theme_table() solely for the HOOK_preprocess_table__PATTERN().
 */
function theme_table__field_collection_table($variables) {
  return theme_table($variables);
}

<?php
function abtpre_letschat_print($user) {

  $qs = drupal_get_query_parameters();
  
  $yid = $qs['yid'];

  //return node_view(node_load(31));

  $query = new EntityFieldQuery();
  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', array('performance_objectives','development_plan', 'self_assessment'))
    ->fieldCondition('field_person', 'uid', $user->uid)
    ->fieldCondition('field_yid', 'value', $yid)
    ->execute();

  if (!empty($result)) {
    foreach ($result['node'] as $nid => $entity) {
      $nodes[$entity->type] = $nid;
    }
  }

  $output = array();
  if (isset($nodes['performance_objectives'])) {
    $node = node_load($nodes['performance_objectives']);
    $output[] = node_view($node);
    $output[] = _abtpre_letschat_node_comments($node);
  }
  if (isset($nodes['development_plan'])) {
    $node = node_load($nodes['development_plan']);
    $output[] =  node_view($node);
    $output[] = _abtpre_letschat_node_comments($node);
  }
  if (isset($nodes['self_assessment'])) {
    $node = node_load($nodes['self_assessment']);
    $output[] =  node_view($node);
    $output[] = _abtpre_letschat_node_comments($node);
  }
  if ($rating = _abtpre_letschat_rating($yid, $user->uid)) {
    $output[] = array('#markup' => t('Overall rating: @rating', array('@rating' => _abtpre_letschat_variable($rating))));
  }
  return $output;
}

function _abtpre_letschat_node_comments($node) {
  $query = new EntityFieldQuery();
  $result = $query
    ->entityCondition('entity_type', 'comment')
    ->propertyCondition('nid', $node->nid)
    ->fieldOrderBy('field_purpose', 'value', 'DESC')
    ->execute();

  if (!empty($result)) {
    $comments = comment_load_multiple(array_keys($result['comment']));
    return comment_view_multiple($comments, $node);
  } else {
    return false;
  }
}

function _abtpre_letschat_variable($code) {

  $variables[0] = t('Unable to assess');
  $variables[1] = t('Developing performer');
  $variables[2] = t('Solid performer');
  $variables[3] = t('Excelling performer');
  $variables[4] = t('Difference maker');
  return $variables[$code];
}

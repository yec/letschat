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
    $output[] = array('#markup' => '<h3 style="text-align:center">'.t('Overall rating: @rating', array('@rating' => _abtpre_letschat_variable($rating))).'<br />'._abtpre_letschat_detail_rating($rating).'</h3>');
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
  $variables[0] = '';
  $variables[1] = t('Unable to assess');
  $variables[2] = t('Developing performer');
  $variables[3] = t('Solid performer');
  $variables[4] = t('Excelling performer');
  $variables[5] = t('Difference maker');
  return $variables[$code];
}

function _abtpre_letschat_detail_rating($rating) {
  $variables[0] = '';
  $variables[1] = t('Too soon to say.');
  $variables[2] = t('Currently developing the essential skills and abilities necessary to fully perform all aspects of the role.');
  $variables[3] = t('Is a solid and reliable performer who possesses all the essential skills and abilities necessary for the role.');
  $variables[4] = t('A highly skilled performer who contributes above and beyond the expectations of the role.');
  $variables[5] = t('An outstanding performer who drives own development and who routinely makes a difference to business performance.');
  return $variables[$rating];
}

<?php
// $Id: nodehierarchy_workflow_ng.inc,v 1.1 2009/04/04 04:58:39 ronan Exp $

/**
 * @file
 * Some workflow-ng conditions/actions
 */

/**
 * Implementation of hook_condition_info()
 */
function nodehierarchy_condition_info() {
  return array(
    'nodehierarchy_has_parent' => array(
      '#label' => t('Content has parent'),
      '#arguments' => array(
        'node' => array('#entity' => 'node', '#label' => t('Content')),
      ),
      '#module' => t('Node Hierarchy'),
    ),
  );
}

function nodehierarchy_has_parent($node, $settings) {
  return (bool)nodehierarchy_get_node_parent_nids($node->nid);
}


/**
 * Implementation of hook_action_info()
 */
function nodehierarchy_action_info() {
  return array(
    'nodehierarchy_action_load' => array(
      '#label' => t('Load a content\'s parent content'),
      '#arguments' => array(
        'node' => array('#entity' => 'node', '#label' => t('Child content')),
      ),
      '#new arguments' => array(
        'node_parent' => array('#entity' => 'node', '#label' => t('Parent content')),
      ),
      '#module' => t('Node Hierarchy'),
    ),
  );
}

/**
 * Loads the node's parent.
 */
function nodehierarchy_action_load($node, $settings) {
  $parents = nodehierarchy_get_node_parent_nids($nid);
  if ($parent = node_load($pnid)) {
    return array('#new arguments' => array('node_parent' => $parent));
  }
}

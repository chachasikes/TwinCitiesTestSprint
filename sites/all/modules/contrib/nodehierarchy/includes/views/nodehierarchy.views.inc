<?php
// $Id: nodehierarchy.views.inc,v 1.2 2009/04/04 04:58:39 ronan Exp $

// Implementations of Views 2 Hooks

/**
 * Implementation of hook_views_handlers()
 */
function nodehierarchy_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'nodehierarchy') .'/includes/views',
    ),
    'handlers' => array(
      'views_handler_field_nodehierarchy_actions' => array(
        'parent' => 'views_handler_field',
      ),
      'views_handler_field_nodehierarchy_parent' => array(
        'parent' => 'views_handler_field',
      ),
      'views_handler_argument_nodehierarchy_ancestor' => array(
        'parent' => 'views_handler_argument',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_data()
 */
function nodehierarchy_views_data() {

  // Basic table information.
  $data['nodehierarchy_menu_links']['table']['group']  = t('Node Hierarchy');
  $data['nodehierarchy_menu_links']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  // An aliased table to connect {menu_links} to {nodes}
  $data['nh_menu_links']['table']['group'] = t('Node Hierarchy');
  $data['nh_menu_links']['table']['join'] = array(
    'node' => array(
      'table' => 'menu_links',
      'left_table' => 'nodehierarchy_menu_links',
      'left_field' => 'mlid',
      'field' => 'mlid',
    ),
  );

  $data['nh_menu_links']['weight'] = array(
    'title' => t('Child Weight'),
    'help' => t('The sort order of the child node.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Link back to the node table via plid to get the parent node.
  $data['nh_parent']['table']['group'] = t('Node Hierarchy');
  $data['nh_parent']['table']['join'] = array(
    'node' => array(
      'table' => 'nodehierarchy_menu_links',
      'left_table' => 'nh_menu_links',
      'left_field' => 'plid',
      'field' => 'mlid'
    ),
  );

  // Parent nid.
  $data['nh_parent']['nid'] = array(
    'title' => t('Parent Node ID'),
    'help' => t('The parent id of the node.'),
    // Information for accepting a parent as an argument.
    'argument' => array(
      'field' => 'nid',
      'handler' => 'views_handler_argument_node_nid',
      'click sortable' => TRUE,
    ),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Parent Node'),
    ),
  );

  // Bogus fields for aliasing purposes.
  // Link back to the node table via plid to get the parent node.
  $data['nh_ancestor']['table']['group'] = t('Node Hierarchy');
  $data['nh_ancestor']['table']['join'] = array(
    'node' => array(
      'handler' => 'views_handler_join_nodehierarchy_ancestor',
      'arguments' => array(
        'table' => 'nodehierarchy_menu_links',
        'left_table' => 'nh_menu_links',
        'left_field' => 'plid',
        'field' => 'mlid'
      ),
    ),
  );
  $data['nh_ancestor']['nid'] = array(
    'title' => t('Ancestor Node ID'),
    'help' => t('Use this if you want to display any descendant of the given node'),
    // Information for accepting a parent as an argument.
    'argument' => array(
      'field' => 'nid',
      'handler' => 'views_handler_argument_node_nid',
      'click sortable' => TRUE,
    ),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Ancestor Node'),
    ),
  );

  return $data;
}

class views_handler_join_nodehierarchy_ancestor extends views_join {
  // PHP 4 doesn't call constructors of the base class automatically from a
  // constructor of a derived class. It is your responsibility to propagate
  // the call to constructors upstream where appropriate.
  function construct($table, $left_table) {
    parent::construct($table, $left_table);
  }

  function join($table, &$query) {
    $left = $query->get_table_info($this->left_table);
    $on = array();
    for ($i = 1; $i < MENU_MAX_DEPTH; $i++) {
      $on[] = "$left[alias].p$i = $table[alias].mlid";
    }
    $output = " $this->type JOIN {" . $this->table . "} $table[alias] ON (". implode(' OR ', $on) .") AND $left[alias].mlid != $table[alias].mlid";
    return $output;
  }
}



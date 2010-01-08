<?php
// $Id: draggableviews.views.inc,v 1.1.2.10 2009/08/21 14:47:09 sevi Exp $

/**
 * @file
 * Derives the view style plugin
 */

/**
 * Implemening hook_views_plugins
 */
function draggableviews_views_plugins() {
  return array(
    'module' => 'draggableviews', // This just tells our themes are elsewhere.

    'style' => array(
      'draggabletable' => array(
        'path' => drupal_get_path('module', 'draggableviews') .'/views',
        'title' => t('Draggable Table'),
        'help' => t('Displays rows in a table and makes them draggable.'),
        'handler' => 'draggableviews_plugin_style_draggabletable',
        'theme' => 'draggableviews_view_draggabletable',
        'theme file' => 'draggableviews_theme.inc',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'uses grouping' => FALSE,
        'type' => 'normal',
        'parent' => 'table',
      ),
    ),
  );
}

/**
 * Implemening hook_views_handlers
 */
function draggableviews_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'draggableviews') .'/views/handlers',
    ),
    'handlers' => array(

      // field handlers
      'views_handler_field_draggableviews_structure' => array(
        'parent' => 'views_handler_field',
      ),

      // sort handlers
      'views_handler_sort_draggableviews_structure' => array(
        'parent' => 'views_handler_sort',
      ),
    ),
  );
}

/**
 * Implementing hook_views_data.
 */
function draggableviews_views_data() {
  $data = array();

  $base_tables = array(
    'nid' => 'node',
    'uid' => 'users',
    'cid' => 'comments',
    'vid' => 'node_revisions',
    'tid' => 'term_data',
  );

  foreach ($base_tables AS $base_field => $base_table) {
    for ($i = 0; $i < 2; $i++) {
      $data['draggableviews_structure_'. $base_table . $i]['table'] = array(
        'group' => t('DraggableViews'),
        'join' => array(
          $base_table => array(
            'table' => 'draggableviews_structure',
            'left_field' => $base_field,
            'field' => 'nid',
            'extra' => array(
              array(
                'field' => 'delta',
                'operator' => '=',
                'value' => $i,
                'numeric' => TRUE,
              ),
            ),
          ),
        ),
      );
      $data['draggableviews_structure_'. $base_table . $i]['value'. $i] = array(
        'real field' => 'value',
        'title' => t($i == 0 ? 'Order' : 'Parent'),
        'field' => array(
          'handler' => 'views_handler_field_draggableviews_structure',
          'help' => t('This value provided by draggableviews is unique for each view.'),
          'click sortable' => $i == 0 ? TRUE : FALSE,
          'draggableviews_delta' => $i,
        ),
      );
      if ($i == 0) {
        $data['draggableviews_structure_'. $base_table . $i]['value'. $i]['sort'] = array(
          'handler' => 'views_handler_sort_draggableviews_structure',
          'help' => t('Sort by value'),
        );
      }
    }
  }

  return $data;
}

function draggableviews_views_query_alter(&$view, &$query) {
  for ($i = 0; $i < 2; $i++) {
    if (isset($query->table_queue['draggableviews_structure_'. $view->base_table . $i])) {
      $query->table_queue['draggableviews_structure_'. $view->base_table . $i]['join']->extra[] = array(
        'field' => 'view_name',
        'operator' => '=',
        'value' => $view->name,
      );
      $args = NULL;
      if (!empty($view->style_plugin->options['draggableviews_arguments']['use_args'])) {
        if (!empty($view->args)) {
          $args = implode('/', $view->args);
        }
      }
      $query->table_queue['draggableviews_structure_'. $view->base_table . $i]['join']->extra[] = array(
        'field' => 'args',
        'operator' => '=',
        'value' =>  $args, 
      );
    }
  }
}

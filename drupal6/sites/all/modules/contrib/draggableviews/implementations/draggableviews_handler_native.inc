<?php
// $Id: draggableviews_handler_native.inc,v 1.1.2.9 2009/08/21 14:47:09 sevi Exp $

/**
 * @file
 * The default implementation of draggableviews handlers.
 */

/*
 * Default Implementation: storage done by draggableviews
 */
class draggableviews_handler_native {
  var $view;

  function init($field_name, &$view) {
    $this->field_name = $field_name;
    $this->view       = $view;
    $this->pager      = $view->pager;

    $this->delta = $this->view->field[$field_name]->definition['draggableviews_delta'];

    if ($field_name == $view->style_plugin->options['tabledrag_hierarchy']['field']) {
      $this->type = 'hierarchy';
    }
    else {
      $this->type = 'order';
    }
  }

  function save($nid, $value) {
    $args = NULL;

    if (!empty($this->view->style_plugin->options['draggableviews_arguments']['use_args'])) {
      $args = implode('/', $this->view->args);
    }

    db_query("DELETE FROM {draggableviews_structure} WHERE view_name='%s' AND nid=%d AND delta=%d AND args='%s'", $this->view->name, $nid, $this->delta, $args);
    db_query("INSERT INTO {draggableviews_structure}(view_name, nid, delta, value, args) VALUES('%s', %d, %d, %d, '%s')", $this->view->name, $nid, $this->delta, $value, $args);
  }

  function get_form_element($value, $attributes = array()) {
    if (!isset($attributes['class'])) $attributes['class'] = $field['field_name'];

    switch ($this->type) {
      default:
        $options = array();
        for ($i = $this->range_start; $i <= $this->range_end; $i++) $options[$i] = $i;

        return array(
          '#type'  => 'select',
          '#name'  => $attributes['field_name'],
          '#value' => $value,
          '#options' => $options,
          '#attributes' => array('class' => $attributes['class']),
        );
        break;

      case 'hierarchy':
        return array(
          '#type' => 'hidden',
          '#name' => $attributes['field_name'],
          '#value' => $value,
          '#attributes' => array('class' => $attributes['class']),
        );
    }
  }

  function set_range($start, $end) {
    $this->range_start = $start;
    $this->range_end   = $end;
  }
}

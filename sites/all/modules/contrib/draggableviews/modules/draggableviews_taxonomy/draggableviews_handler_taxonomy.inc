<?php
// $Id: draggableviews_handler_taxonomy.inc,v 1.1.2.1 2009/08/19 14:17:58 sevi Exp $

/**
 * @file
 * The taxonomy handler.
 */

/*
 * Taxonomy handler.
 */
class draggableviews_handler_taxonomy {
  var $view;

  function init($field_name, &$view) {
    $this->field_name = $field_name;
    $this->view       = $view;
    $this->pager      = $view->pager;

    if ($field_name == $view->style_plugin->options['tabledrag_hierarchy']['field']) {
      $this->type = 'hierarchy';
    }
    else {
      $this->type = 'order';
    }
  }

  function save($tid, $value) {
    switch ($this->type) {
      case 'order':
        db_query("UPDATE {term_data} SET weight=%d WHERE tid=%d", $value, $tid);
        break;
      case 'hierarchy':
        db_query("UPDATE {term_hierarchy} SET parent=%d WHERE tid=%d", $value, $tid);
        break;
    }
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

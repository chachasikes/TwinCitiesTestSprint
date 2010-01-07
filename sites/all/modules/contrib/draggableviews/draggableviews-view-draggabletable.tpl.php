<?php
// $Id: draggableviews-view-draggabletable.tpl.php,v 1.6.2.8 2009/08/19 12:28:38 sevi Exp $
/**
 * @file
 * Template to display a view as a draggable table.
 *
 * - $header: An array of header labels keyed by field id.
 * - $fields: An array of CSS IDs to use for each field id.
 * - $class: A class or classes to apply to the table, based on settings.
 * - $rows: An array of row items. Each row is an array of content
 *   keyed by field ID.
 *
 * - $tabledrag_tableId: The table id that drupal_add_tabledrag needs
 */
?>
<table class="<?php print $class; ?>" id="<?php print $tabledrag_table_id; ?>">
  <thead>
    <tr>
      <?php foreach ($header as $field => $label): ?>
        <th class="views-field views-field-<?php print $fields[$field]; ?>"<?php if ($style[$field]) print ' style="'. $style[$field] .'"'; ?>>
          <?php print $label; ?>
        </th>
      <?php endforeach ?>
    </tr>
  </thead>
  <tbody>
    <?php foreach ($rows as $count => $row): ?>
      <?php $zebra = ($count % 2 == 1) ? 'even' : 'odd'; ?>
      <tr class="draggable <?php print $zebra; ?><?php if ($draggableviews_extended[$count]) print ' draggableviews-extended'; ?><?php if ($tabledrag_type[$count]) print ' '. $tabledrag_type[$count]; ?>">
        <?php foreach ($row as $field => $content): ?>
          <td class="views-field views-field-<?php print $fields[$field]; ?>"<?php if ($style[$field]) print ' style="'. $style[$field] .'"'; ?>><?php
            print $content;
          ?></td>
        <?php endforeach; ?>
      </tr>
    <?php endforeach; ?>
  </tbody>
</table>

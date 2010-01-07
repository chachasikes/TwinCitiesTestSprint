// $Id: nodeorder.js,v 1.2.2.3 2009/11/15 14:19:44 pvanderspek Exp $

/**
 * Order nodes associated with a term.
 *
 * This behavior is dependent on the tableDrag behavior, since it uses the
 * objects initialized in that behavior to update the row.
 */
Drupal.behaviors.blockDrag = function(context) {
  var table = $('table#ordernodes');
  var tableDrag = Drupal.tableDrag.ordernodes; // Get the blocks tableDrag object.

  // Add a handler for when a row is swapped, enable the submit button.
  tableDrag.onDrop = function() {
    $('form#nodeorder-admin-display-form input#edit-submit').removeAttr('disabled');
  };
}

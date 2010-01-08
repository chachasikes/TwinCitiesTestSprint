// $Id: openlayers_presets_ui.ui.js,v 1.2.2.2 2009/10/06 03:10:13 zzolo Exp $

/**
 * @file
 * This file holds the javascript functions for the preset UI
 *
 * @ingroup openlayers
 */

/**
 * Drupal beahvior for OL UI form
 */
Drupal.behaviors.OLUI = function(context) {
  var $projectSelect = $("input[@name='projections[easy_projection]']:not(.projection-processed)");
  var $projectOther = $('#edit-projections-projection');
  var $projectOtherWrapper = $('#edit-projections-projection-wrapper');
  var $autoOptionsCheck = $('#edit-options-automatic-options');
  var $submitProjection = $('#edit-openlayers-projection-ahah');

  // Reproject center values when projection changes
  $projectSelect.change(OL.updateCenterFormOnProjectionChange);
  $projectOther.change(OL.updateCenterFormOnProjectionChange);

  // Hide submit button
  $submitProjection.hide();

  // Add class
  $projectSelect.addClass('projection-processed');

  // Hide the other projection_field
  if ($projectSelect.length > 0) {
    if ($projectSelect.val() != 'other') {
      // $projectOtherWrapper.hide();
    }
  }

  // Change event for select and add class
  $projectSelect.change(function(e) {
    var val = $(this).val();
    if (val == 'other') {
      $projectOtherWrapper.show();
    }
    else {
      $projectOther.val(val);
      // $projectOtherWrapper.hide();
    }
  });

  // Automatic options.  We do it here, instead of in Form API because
  // Form API enforces the disabled
  $autoOptionsCheck.change(function() {
    var $thisCheck = $(this);
    var $autoOptions = $thisCheck.parent().parent().parent().find('input:not("#edit-options-automatic-options")');
    if ($thisCheck.is(':checked')) {
      $autoOptions.attr('disabled', 'disabled');
    }
    else {
      $autoOptions.removeAttr('disabled');
    }
  });

  // When form is submitted, if diabled, FAPI does not read values
  jQuery($autoOptionsCheck.form).submit(function() {
    $autoOptionsCheck.attr('checked', false);
    $autoOptionsCheck.trigger('change');
  });

  // Trigger change
  $autoOptionsCheck.trigger('change');

  // Change event for helper map inputs
  $('#edit-center-lat').change(OL.updateHelpmapCenter);
  $('#edit-center-lon').change(OL.updateHelpmapCenter);
  $('#edit-center-zoom').change(OL.updateHelpmapCenter);

  // @@TODO: Reproject lat/lon values of center map. On the first load
  // this is redundent, but it is important for ahah updates so that when
  // the projection changes the lat/lon changes to fit the new units.

  // Initial trigger of updateHelpmapCenter
  OL.updateHelpmapCenter();
}

/**
 * Update the values of the centering form whent the user changes projections
 *
 */
OL.updateCenterFormOnProjectionChange = function(event) {
  var $formItem = $(event.target);
  var projection = $formItem.val();
  var helpmap = OL.maps['openlayers-center-helpmap'].map;
  var zoom = helpmap.getZoom();
  var center = helpmap.getCenter();

  // Transform center
  center.transform(new OpenLayers.Projection('EPSG:4326'),new OpenLayers.Projection('EPSG:' + projection));

  // Get new lat and lon
  var lat = center.lat;
  var lon = center.lon;

  // Set new values in the field
  $('#edit-center-lat').val(lat);
  $('#edit-center-lon').val(lon);
}

/**
 * Update the center of the helpmap using the values from the form
 *
 * Take the center lat, lon and zoom values from the form and update
 * the helper map.
 */
OL.updateHelpmapCenter = function() {
  var projection = $('#edit-projections-projection').val();
  var zoom = $('#edit-center-zoom').val();
  var lat = $('#edit-center-lat').val();
  var lon = $('#edit-center-lon').val();

  // Check for lat and lon
  if (lat != '' && lon != '') {
    // Create new center
    var center = new OpenLayers.LonLat(lon, lat);
    // Transform for projection
    center.transform(new OpenLayers.Projection('EPSG:' + projection), new OpenLayers.Projection('EPSG:4326'));
    // Set center of map.
    OL.maps['openlayers-center-helpmap'].map.setCenter(center, zoom);
  }
}

/**
 * Update the values from the form using center of the helpmap.
 *
 * When a user pans and zooms our helper map, update the form values.
 */
OL.EventHandlers.updateCenterFormValues = function() {
  var helpmap = OL.maps['openlayers-center-helpmap'].map;
  var projection = $('#edit-projections-projection').val();
  var zoom = helpmap.getZoom();
  var center = helpmap.getCenter();

  // Transform center
  center.transform(new OpenLayers.Projection('EPSG:4326'),new OpenLayers.Projection('EPSG:' + projection));

  // Get new lat and lon
  var lat = center.lat;
  var lon = center.lon;

  // Set new values
  $('#edit-center-zoom').val(zoom);
  $('#edit-center-lat').val(lat);
  $('#edit-center-lon').val(lon);
}

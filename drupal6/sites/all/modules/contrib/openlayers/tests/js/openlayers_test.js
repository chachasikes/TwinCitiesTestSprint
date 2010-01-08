// $Id: openlayers_test.js,v 1.1.2.1 2009/10/06 03:10:13 zzolo Exp $

/**
 * @file
 * JS file for testing JS stuff in OpenLayers
 */

/**
 * Global object for OL Tests
 */
var OL = OL || {};
OL.Testing = OL.Testing || {};

/**
 * Test Callback for Style Context
 */
OL.Testing.StyleContextCallback = function(mapid, layerid, render_intent) {
  alert(mapid);
  return {};
}
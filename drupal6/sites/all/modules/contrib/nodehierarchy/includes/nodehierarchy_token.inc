<?php
// $Id: nodehierarchy_token.inc,v 1.2 2009/05/25 20:43:10 ronan Exp $

/**
 * @file
 * token.module integration functions for nodehierarchy.module
 */

/**
 * Implementation of hook_token_values().
 */
function nodehierarchy_token_values($type, $object = NULL, $options = array()) {
  $tokens = array();
  if ($type == 'node') {
    $node = $object;

    $parents = nodehierarchy_get_node_parent_nids($node->nid);
    if ($parent = node_load(@$parents[0])) {
      $tokens['hierarchyparenttitle']        = check_plain(@$parent->title);
      $tokens['hierarchyparenttitle-raw']    = @$parent->title;
      $tokens['hierarchyparentnid']          = $parent->nid;
    }
    else {
      $tokens['hierarchyparenttitle']        = '';
      $tokens['hierarchyparenttitle-raw']    = '';
      $tokens['hierarchyparentnid']          = '';
    }
    $tokens['hierarchytitlepath']          = nodehierarchy_token_get_hierarchytitlepath($node->nid);
    $tokens['fullhierarchytitlepath']      = nodehierarchy_token_get_fullhierarchytitlepath($node);
    $tokens['hierarchytitlepath-raw']      = nodehierarchy_token_get_hierarchytitlepath($node->nid, TRUE);
    $tokens['fullhierarchytitlepath-raw']  = nodehierarchy_token_get_fullhierarchytitlepath($node, TRUE);
    $tokens['hierarchypath']               = nodehierarchy_token_get_hierarchypath($node->nid);
    $tokens['fullhierarchypath']           = nodehierarchy_token_get_fullhierarchypath($node);
  }

  return $tokens;
}

/**
 * Implementation of hook_token_list().
 */
function nodehierarchy_token_list($type = 'all') {
  if ($type == 'node' || $type == 'all') {
    $tokens['node']['fullhierarchypath']            = t('The URL of the parent of the given node with the node title itself. Equivalent of [hierarchyurl]/[title]. This is the recommended token for hierarchical URLs.');
    $tokens['node']['hierarchypath']                = t('The URL of the parent of the given node. Lile [fullhierarchypath] but without the node title.');
    $tokens['node']['hierarchyparenttitle']         = t("The node's parent's title.");
    $tokens['node']['hierarchyparenttitle-raw']     = t("The node's parent's title. WARNING - raw user input.");
    $tokens['node']['hierarchyparentnid']           = t("The node's parent's nid.");

    $tokens['node']['hierarchytitlepath']          = t("The node's ancestors' titles in order. Separated by /.");
    $tokens['node']['hierarchytitlepath-raw']      = t("The node's ancestors' titles in order. Separated by /. WARNING - raw user input.");
    $tokens['node']['fullhierarchytitlepath']      = t("The node's ancestors' titles in order the title of the node itself. Separated by /. Equivalent of [hierarchypath]/[title].");
    $tokens['node']['fullhierarchytitlepath-raw']  = t("The node's ancestors' titles in order the title of the node itself. Separated by /. Equivalent of [hierarchypath]/[title]. WARNING - raw user input.");

    return $tokens;
  }
}

function nodehierarchy_token_get_fullhierarchytitlepath($node, $raw = TRUE) {
  $title = $raw ? $node->title : check_plain($node->title);

  // return the hierarchy path with the converted title
  return trim(nodehierarchy_token_get_hierarchytitlepath($node->nid, $raw) ."/". $title, "/");
}

function nodehierarchy_token_get_hierarchytitlepath($nid, $raw = TRUE) {
  // the hierarchy path is the parent node's full hierarchy path
  $parents = nodehierarchy_get_node_parent_nids($nid);
  if ($pnid = @$parents[0]) {
    return nodehierarchy_token_get_fullhierarchytitlepath(node_load($pnid));
  }
  return "";
}

function nodehierarchy_token_get_fullhierarchypath($node) {
  return trim(nodehierarchy_token_get_hierarchypath($node->nid) ."/". $node->title, "/");
}

function nodehierarchy_token_get_hierarchypath($nid) {
  // the hierarchy path is the parent node's full hierarchy path
  $parents = nodehierarchy_get_node_parent_nids($nid);
  if ($pnid = @$parents[0]) {
    // if the parent already has an alias (generated or entered) use that
    $parent_path = drupal_get_path_alias("node/$pnid");
    if ($parent_path != "node/$pnid") {
      // replace the separator with a space, so that pathauto replaces it with the separator again.
      // a little hacky but prevents the separator from being stripped
      return str_replace(variable_get('pathauto_separator', '-'), " ", $parent_path);
    }
    else {
      // recurse
      return nodehierarchy_token_get_fullhierarchypath($pnid);
    }
  }
  return "";
}
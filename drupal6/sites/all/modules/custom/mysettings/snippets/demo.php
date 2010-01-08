<?php
// $Id$

/**
 * @file
 * Tests for MySettings module.
 * DrupalWebTestCase *CAN* access database & files directory
 */

// Rule: Test class names are camel case and end with TestCase. 
// For example, a test designed to check the node view functionality would be named: NodeViewTestCase.

class MySettingsDemoTest extends DrupalWebTestCase {
  public static function getInfo() {
    return array(
      'name' => t('MySettings Demo Test'),
      'description' => t('What does the test do'),
      'group' => t('MySettings'),
    );
  }

  function setUp() {
    parent::setUp('mysettings');
    
    // Create and log in our user
    $privileged_user = $this->drupalCreateUser(array('create mymodule'));
    $this->drupalLogin($privileged_user);
  }

 // Rule: All tests functions are prefixed with test.
  /**
   * Demonstrate the fail function.
   */
  function testFailingFunction() {
    // Make the function fail.
     $this->fail('Description of what failed', 'MySettings');
  }

  function testVariousMessages() {
   // $name = $this->randomName(10);
    $this->fail('pass', 'MySettings');
  }

  // Create a mymodule node using the node form
  public function testMymoduleCreate() {
    $langcode = FIELD_LANGUAGE_NONE;
    $body_key = "body[$langcode][0][value]";
    // Create node to edit.
    $edit = array();
    $edit['title'] = $this->randomName(8);
    $edit[$body_key] = $this->randomName(16);
   
    $this->drupalPost('node/add/mymodule', $edit, t('Save'));
    $this->assertText(t('mymodule page @title has been created.', array('@title' => $edit['title'])));

    // For debugging we can output the page so it can be opened with a browser
    // Remove this line when the test has been debugged
    $this->outputScreenContents('After page creation', 'testMymoduleCreate');
  }
  
    // Create a mymodule node and then see if our user can edit it
  public function testMymoduleEdit() {
    $settings = array(
      'type' => 'mymodule',
      'title' => $this->randomName(32),
      'body' => array(FIELD_LANGUAGE_NONE => array(array($this->randomName(64)))),
    );
    $node = $this->drupalCreateNode($settings);

    // For debugging, we might output the node structure with $this->pass()
    // $this->pass('Node created: ' . var_export($node,TRUE));

    $edit_path = "node/{$node->nid}/edit";
    $this->drupalGet($edit_path);
    $this->assertFieldById('edit-body-zxx-0-summary','', 'Looking for edit-body-zxx-0-summary field as indication that we got to the edit page');

    // For debugging we can output the page so it can be opened with a browser
    // Remove this line when the test has been debugged
    $this->outputScreenContents("After drupalGet($edit_path) in testMymoduleEdit", 'testMymoduleEdit');
  }

  private function outputScreenContents($description, $basename) {
    // This is a hack to get a directory that won't be cleaned up by simpletest
    $file_dir = file_directory_path().'/../simpletest_output_pages';
    if (!is_dir($file_dir)) {
      mkdir($file_dir, 0777, TRUE);
    }
    $output_path = "$file_dir/$basename." . $this->randomName(10) . '.html';
    $rv = file_put_contents($output_path, $this->drupalGetContent());
    $this->pass("$description: Contents of result page are ".l('here',$output_path));
  }
 
  /*
   * USEFUL SNIPPETS
   *
   * From SimpleTest Tutorial:  http://drupal.org/node/265762
   */
  private function testVariousTests() {
    // Print out Random Variables.
    $var = $this->doAwesomeStuff($settings);
    $this->pass(var_export($var, TRUE));
    
    // Print out the Content of the Current Page.
    file_put_contents('output.html', $this->drupalGetContent());
    
    // Create a new account.
    $name = $this->randomName();
    $mail = "$name@example.com";
    $edit = array(
      'name' => $name,
      'mail' => $mail,
      'status' => FALSE, // checkboxes must be set with TRUE/FALSE rather than 1/0
    );
    $this->drupalPost('user/register', $edit, 'Create new account'); 
    
    // Click a link.
    $this->clickLink(t('Log out')); 
    
    // Load a node
    // * Prepare a user to do the stuff
    $user = $this->drupalCreateUser(array('access content', 'create page content'));
    $this->drupalLogin($user);
    
    // * Now do something with the users
    $this->drupalGet('node/' . $node->nid));
     
    // Create a node of a certain type.
    $settings = array(
    'type' => 'my_special_node_type', // This replaces the default type
    'my_special_field' => 'glark', // This appends a new field.
    );
    $node = $this->drupalCreateNode($settings);
    
    $this->assertEqual($node->type, 'my_spcial_node_type'); // We set this.
    $this->assertEqual($node->comment, 2); // This is default.
    
    // Set the default settings for a new node.
    // * Populate defaults array
    $defaults = array(
      'body'      => $this->randomName(32),
      'title'     => $this->randomName(8),
      'comment'   => 2,
      'changed'   => time(),
      'format'    => FILTER_FORMAT_DEFAULT,
      'moderate'  => 0,
      'promote'   => 0,
      'revision'  => 1,
      'log'       => '',
      'status'    => 1,
      'sticky'    => 0,
      'type'      => 'page',
      'revisions' => NULL,
      'taxonomy'  => NULL,
    );
    $defaults['teaser'] = $defaults['body'];
    // If we already have a node, we use the original node's created time, and this
    if (isset($defaults['created'])) {
      $defaults['date'] = format_date($defaults['created'], 'custom', 'Y-m-d H:i:s O');
    }
    if (empty($settings['uid'])) {
      global $user;
      $defaults['uid'] = $user->uid;
    }
  
    // Assertions: http://drupal.org/node/265828
    // $this->_assert() in order to save the assertion to the database
  
    // Formatting Guidelines: http://drupal.org/node/325974
  }
}
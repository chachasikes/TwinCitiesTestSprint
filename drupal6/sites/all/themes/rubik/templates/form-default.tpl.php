<div class='form form-layout-default clear-block'>
  <div class='column-main'><div class='column-wrapper clear-block'>
    <?php print drupal_render($form); ?>
    <div class='buttons'><?php print rubik_render_clone($buttons); ?></div>
  </div></div>
  <div class='column-side'><div class='column-wrapper clear-block'>
    <?php print drupal_render($sidebar); ?>
    <div class='buttons'><?php print drupal_render($buttons); ?></div>
  </div></div>
</div>
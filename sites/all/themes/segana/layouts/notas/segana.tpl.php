<?php
/**
 * @file
 * Template for a 2 column panel layout.
 *
 * This template provides a two column panel display layout, with
 * each column roughly equal in width.
 *
 * Variables:
 * - $id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 *   panel of the layout. This layout supports the following sections:
 *   - $content['left']: Content in the left column.
 *   - $content['right']: Content in the right column.
 */
?>
<div class="panel-display clearfix panel-alertas" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>
  <div class="inside superior"><?php print $content['superior']; ?></div>
  <div class="inside medio"><?php print $content['medio']; ?></div>
  <div class="inside inferior"><?php print $content['inferior']; ?></div>
</div>

<?php
// $Id$

/**
 * @file
 * Custom print module page template for the signup application form.
 *
 * This is a stripped down copy of print.tpl.php which may be themed
 * independently from the print module template.
 *
 * @ingroup print
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $print['language']; ?>" xml:lang="<?php print $print['language']; ?>">
  <head>
    <?php print $print['head']; ?>
    <title><?php print $print['title']; ?></title>
    <?php print $print['scripts']; ?>
    <?php print $print['robots_meta']; ?>
    <?php print $print['base_href']; ?>
    <?php print $print['favicon']; ?>
    <?php print $print['css']; ?>
  </head>
  <body<?php print $print['sendtoprinter']; ?>>
    <div class="print-logo"><?php print $print['logo']; ?></div>
    <p />
    <h1 class="print-title"><?php print $print['title']; ?></h1>
    <div class="print-content"><?php print $print['content']; ?></div>
    <div class="print-footer"><?php print $print['footer_message']; ?></div>
  </body>
</html>

<?php
// $Id$

/**
 * @file
 * Signup application form template.
 *
 * Available variables:
 * - $signup: The signup object the user subscribed to.
 * - $node: The node the user subscribed to.
 * - $signup_data: Rendered custom signup data.
 *     @see theme_signup_application_custom_data().
 *
 * @see template_preprocess_signup_application_form()
 */
?>
<div class="signup-application">
  <?php print $signup_data; ?>

  <div class="signup-application-signature clear-block">
    <div class="signup-application-signature-date">
      <div class="signup-application-form-label"><?php print t('Date'); ?>:</div>
      <div class="signup-application-form-value">&nbsp;</div>
    </div>
    <div class="signup-application-signature-signature">
      <div class="signup-application-form-label"><?php print t('Signature'); ?>:</div>
      <div class="signup-application-form-value">&nbsp;</div>
    </div>
  </div>
</div>
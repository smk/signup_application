/* $Id$ */

-- SUMMARY --

Generates a printable (PDF) application form from signups for the user to sign
and send back.

For a full description visit the project page:
  http://drupal.org/project/signup_application
Bug reports, feature suggestions and latest developments:
  http://drupal.org/project/issues/signup_application


-- REQUIREMENTS --

- Signup (http://drupal.org/project/signup)

- Printer, e-mail and PDF versions (http://drupal.org/project/print)

  Note: From the suite only the main module "Printer-friendly pages" and
        "PDF versions" are required.

- Token (http://drupal.org/project/token)


-- INSTALLATION --

Install as usual, see http://drupal.org/node/70151 for further information.

Signup Application Form leverages PDF generation from the Printer, e-mail and
PDF versions suite of modules, so make sure you have a PDF library installed
and configured. See the print project page for details.


-- CONFIGURATION --

After installation of the module there will be a new token

  [signup-application-pdf-url]

which can be inserted into the confirmation e-mail a user receives after
signing up for a node. The default confirmation message can be changed on the
signup settings page (Site configuration > Signup) and for any existing signup-
supported node on the Signups > Settings tab.

The resulting PDF filename defaults to "application.pdf" and can be changed by
overriding the value of the variable "signup_application_pdf_filename". The
filename may include any tokens from the signup namespace.


-- THEMING --

Signup Application Forms can be themed by customizing the provided templates:

- signup_application-page.tpl.php: a stripped-down version of print.tpl.php,
  used for theming the page output.

- signup_application-form.tpl.php: used to render the custom user supplied form
  fields and signature field.

For more information on how to override templates see
http://drupal.org/node/341628.


-- CREDITS --

Author: Stefan M. Kudwien (smk-ka) - http://drupal.org/user/48898

Development sponsored by TOZEN Internetagentur - http://www.tozen.de


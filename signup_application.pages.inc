<?php
// $Id$

/**
 * @file
 * Page callbacks to generate the printable application form.
 */

/**
 * Page callback to generate an appliciation form.
 *
 * @param $type
 *   The type to generate, currently 'pdf' is the only supported option.
 * @param $signup
 *   The signup object.
 */
function signup_application_generate($type, $signup) {
  if (in_array($type, array('form', 'pdf', 'print'))) {
    // Never cache generated output containing personal information.
    $GLOBALS['conf']['cache'] = FALSE;

    $function = 'signup_application_generate_' . $type;
    return $function($signup);
  }
  return MENU_NOT_FOUND;
}

/**
 * Generate an HTML version of the application form.
 *
 * @param $signup
 *   A signup object.
 *
 * @todo Remove this almost 1:1 copy of print_controller_html() when print
 *   module has figured out how to properly use the theme layer.
 *   @see http://drupal.org/node/358838
 *
 * @see print_controller_html()
 */
function signup_application_generate_print($signup) {
  module_load_include('inc', 'print', 'print.pages');

  // Create path using 'form' as type to actually generate the output.
  $token = signup_get_token($signup->sid, 'application');
  $path = "signup/application/form/$signup->sid/$token";

  $print = print_controller($path, NULL, PRINT_HTML_FORMAT);
  if ($print === FALSE) {
    return MENU_NOT_FOUND;
  }

  $print['signup'] = $signup;

  ob_start();
  include_once(_signup_application_get_template(PRINT_HTML_FORMAT));
  $html = ob_get_contents();
  ob_end_clean();
  $html = drupal_final_markup($html);
  print $html;

  // Return nothing to skip theme_page().
}

/**
 * Generate a PDF version of the application form.
 *
 * @param $signup
 *   A signup object.
 * @return
 *   If the PDF generation was successful, this function transmits the generated
 *   file and will not return.
 *
 * @todo Remove this almost 1:1 copy of print_pdf_controller() when print module
 *   has figured out how to properly use the theme layer.
 *   @see http://drupal.org/node/358838
 *
 * @see print_pdf_controller()
 */
function signup_application_generate_pdf($signup) {
  global $base_url, $language;

  module_load_include('inc', 'print_pdf', 'print_pdf.pages');

  // Create path using 'form' as type to actually generate the output.
  $token = signup_get_token($signup->sid, 'application');
  $path = "signup/application/form/$signup->sid/$token";

  $print_pdf_pdf_tool = variable_get('print_pdf_pdf_tool', PRINT_PDF_PDF_TOOL_DEFAULT);

  $print = print_controller($path, NULL, PRINT_PDF_FORMAT);
  if ($print === FALSE) {
    return MENU_NOT_FOUND;
  }

  $print['signup'] = $signup;

  // Img elements must be set to absolute
  $pattern = '!<(img\s[^>]*?)>!is';
  $print['content'] = preg_replace_callback($pattern, '_print_rewrite_urls', $print['content']);
  $print['logo'] = preg_replace_callback($pattern, '_print_rewrite_urls', $print['logo']);
  $print['footer_message'] = preg_replace_callback($pattern, '_print_rewrite_urls', $print['footer_message']);
  // And converted from private to public paths
  $file_downloads = variable_get('file_downloads', FILE_DOWNLOADS_PUBLIC);
  if ($file_downloads == FILE_DOWNLOADS_PRIVATE) {
    switch (variable_get('language_negotiation', LANGUAGE_NEGOTIATION_NONE)) {
      case LANGUAGE_NEGOTIATION_PATH_DEFAULT:
      case LANGUAGE_NEGOTIATION_PATH:
        $lang = $language->language;
        break;
      default:
        $lang = '';
        break;
    }
    $pattern = "!(<img\s[^>]*?src\s*?=\s*?['\"]?)${base_url}/(?:(?:index.php)?\?q=)?(?:${lang}/)?system/files(/[^>]*?>)!is";
    $replacement = '$1file://'. realpath(file_directory_path()) .'$2';
    $print['content'] = preg_replace($pattern, $replacement, $print['content']);
    $print['logo'] = preg_replace($pattern, $replacement, $print['logo']);
    $print['footer_message'] = preg_replace($pattern, $replacement, $print['footer_message']);
  }

  ob_start();
  include_once(_signup_application_get_template(PRINT_PDF_FORMAT));
  $html = ob_get_contents();
  ob_end_clean();
  $html = drupal_final_markup($html);

  // Convert the a href elements, to make sure no relative links remain
  $pattern = '!<(a\s[^>]*?)>!is';
  $html = preg_replace_callback($pattern, '_print_rewrite_urls', $html);
  // And make anchor links relative again, to permit in-PDF navigation
  $html = preg_replace("!${base_url}/". PRINTPDF_PATH ."/.*?%2523!", '#', $html);

  $pdf_filename = variable_get('signup_application_pdf_filename', 'application.pdf');
  if (function_exists('token_replace')) {
    $pdf_filename = token_replace($pdf_filename, 'signup', $signup);
  }
  if (basename($print_pdf_pdf_tool) == 'dompdf_config.inc.php') {
    _print_pdf_dompdf($print, $html, $pdf_filename);
  }
  elseif (basename($print_pdf_pdf_tool) == 'tcpdf.php') {
    _print_pdf_tcpdf($print, $html, $pdf_filename);
  }
  elseif (substr(basename($print_pdf_pdf_tool, '.exe'), 0, 11) == 'wkhtmltopdf') {
    _print_pdf_wkhtmltopdf($print, $html, $pdf_filename);
  }
  else {
    return MENU_NOT_FOUND;
  }
}

/**
 * Choose most appropriate template.
 *
 * @param $format
 *   Format of the PF page being rendered (html, pdf, etc.).
 * @return
 *   Filename of the most suitable template.
 *
 * @see _print_get_template()
 */
function _signup_application_get_template($format) {
  global $theme_key;

  $filenames = array();
  $filenames[] = "signup_application-page-$format.tpl.php";
  $filenames[] = 'signup_application-page.tpl.php';

  foreach ($filenames as $value) {
    // First in the theme directory
    $file = drupal_get_path('theme', $theme_key) .'/'. $value;
    if (file_exists($file)) {
      return $file;
    }
    // Then in the module directory
    $file = drupal_get_path('module', 'signup_application') .'/'. $value;
    if (file_exists($file)) {
      return $file;
    }
  }
}

/**
 * Page callback to render an application form.
 *
 * @param $signup
 *   A signup object.
 */
function signup_application_generate_form($signup) {
  drupal_add_css(drupal_get_path('module', 'signup_application') .'/signup_application.css', 'module', 'all', FALSE);
  return theme('signup_application_form', $signup);
}

/**
 * Template preprocess function for signup_application-form.tpl.php.
 */
function template_preprocess_signup_application_form(&$vars) {
  $signup = $vars['signup'];

  // Load the node the user signed up to.
  $vars['node'] = node_load($signup->nid);

  // Render custom user signup data.
  if (is_array($signup->form_data)) {
    $signup_data = $signup->form_data;
  }
  else {
    $signup_data = unserialize($signup->form_data);
  }
  $vars['signup_data'] = theme('signup_application_custom_data', $signup_data);
}

/**
 * Renders custom signup user data into a human-readable format.
 *
 * WARNING: This theme function is recursive (it calls itself for
 * nested data), so if you override it, be sure not to change the part
 * where it does "call_user_func(__FUNCTION__)".
 *
 * @param $data
 *   Array of custom user signup data.
 *
 * @see theme_signup_user_form()
 * @ingroup themable
 */
function theme_signup_application_custom_data($data) {
  $output = '';
  // All of the possible array key values should already be translated as
  // string literals in theme_signup_user_form() via the #title attributes, so
  // passing a variable to t() is actually safe here.  However, to avoid
  // warnings when extracting strings, "hide" the call to t() by using a
  // variable to hold the function name.
  $t = 't';
  // Loop through each first level element.
  foreach ($data as $key => $value) {
    $output .= '<div class="signup-application-form-element signup-application-'. signup_id_safe($key) .'">';
    if (is_array($value)) {
      // Element is nested, render it recursively.
      // Instead of the overhead of theme(), just call ourself directly.
      $output .= call_user_func(__FUNCTION__, $value);
    }
    else {
      // Keep the &nbsp; otherwise empty values will not be shown.
      $output .= '<div class="signup-application-form-label">'. $t($key) .':</div><div class="signup-application-form-value">&nbsp;'. check_plain($value) .'</div>';
    }
    $output .= "</div>\n";
  }
  return $output;
}

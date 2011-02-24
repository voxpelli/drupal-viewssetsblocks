<?php
// $Id$

/**
 * @file
 * Contains the Sets Blocks display plugin.
 */

/**
 * The plugin that handles some sets blocks.
 */
class viewssetsblocks_plugin_display_setsblocks extends views_plugin_display_block {
  function option_definition() {
    $options = parent::option_definition();

    $options['block_sets'] = array('default' => '');

    return $options;
  }

  /**
   * Provide the summary for page options in the views UI.
   *
   * This output is returned as an array.
   */
  function options_summary(&$categories, &$options) {
    // It is very important to call the parent function here:
    parent::options_summary($categories, $options);

    $block_sets = strip_tags($this->get_option('block_sets'));
    if (empty($block_sets)) {
      $block_sets = t('None');
    }

    if (strlen($block_sets) > 16) {
      $block_sets = substr($block_sets, 0, 16) . '...';
    }

    $options['block_sets'] = array(
      'category' => 'block',
      'title' => t('Sets'),
      'value' => $block_sets,
    );
  }

  /**
   * Provide the default form for setting options.
   */
  function options_form(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_form($form, $form_state);

    switch ($form_state['section']) {
      case 'block_sets':
        $form['#title'] .= t('Block Sets Arguments');
        $form['block_sets'] = array(
          '#type' => 'textfield',
          '#description' => t('The arguments for the blocks. One block will be generated for each combination of values. Multiple arguments are separated by a "/".'),
          '#default_value' => $this->get_option('block_sets'),
        );
        $form['block_sets_help'] = array(
          '#type' => 'item',
          '#title' => t('Iterative sets'),
          '#description' => t("The following sets can be used in the argument to fetch many different variations of the feed - one for each combination of items in the sets used."),
          '#value' => theme('sets_help'),
        );
        break;
    }
  }

  /**
   * Perform any necessary changes to the form values prior to storage.
   * There is no need for this function to actually store the data.
   */
  function options_submit(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_submit($form, $form_state);
    switch ($form_state['section']) {
      case 'block_sets':
        $this->set_option('block_sets', $form_state['values']['block_sets']);
        break;
    }
  }

  /**
   * The default block handler doesn't support configurable items,
   * but extended block handlers might be able to do interesting
   * stuff with it.
   */
  function execute_hook_block($op = 'list', $delta = 0, $edit = array()) {
    if ($op == 'list') {
      $blocks = array();

      $desc = $this->get_option('block_description');

      if (empty($desc)) {
        $desc = check_plain($this->view->name) . '-' . check_plain($this->display->display_title) . ':';
      }

      $block_sets = $this->get_option('block_sets');
      $variants = sets_string_get($block_sets);

      foreach ($variants as $variant) {
        $delta = '-setsblocks_' . base64_encode($variant['string']) . '-' . $this->view->name . '-' . $this->display->id;
        $blocks[$delta] =  array(
          'info' => $desc . ' ' . check_plain($variant['string']),
          'cache' => $this->get_cache_type()
        );
      }

      return $blocks;
    }
  }

  /**
   * Render any special blocks provided for this display.
   */
  function view_special_blocks($type) {
    if (strpos($type, '-setsblocks') === 0) {
      // avoid interfering with the admin forms.
      if (arg(0) == 'admin' && arg(1) == 'build' && arg(2) == 'views') {
        return;
      }

      list($type, $variant) = explode('_', $type, 2);

      $variant = base64_decode($variant);
      $args = explode('/', $variant);

      $this->view->set_arguments($args);

      $info = $this->view->execute_display();
      if ($info) {
        $info['view'] = $this->view;
      }
      return $info;
    }
    else {
      return parent::view_special_blocks($type);
    }
  }
}

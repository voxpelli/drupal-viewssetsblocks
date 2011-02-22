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
   * The default block handler doesn't support configurable items,
   * but extended block handlers might be able to do interesting
   * stuff with it.
   */
  // function execute_hook_block($op = 'list', $delta = 0, $edit = array()) {
  //   if ($op == 'list') {
  //     $delta = $this->view->name . '-' . $this->display->id;
  //     $desc = $this->get_option('block_description');
  // 
  //     if (empty($desc)) {
  //       $desc = t('@view: @display', array('@view' => $this->view->name, '@display' => $this->display->display_title));
  //     }
  //     return array(
  //       $delta => array(
  //         'info' => $desc,
  //         'cache' => $this->get_cache_type()
  //       )
  //     );
  //   }
  // }

  /**
   * The display block handler returns the structure necessary for a block.
   */
  // function execute() {
  //   // Prior to this being called, the $view should already be set to this
  //   // display, and arguments should be set on the view.
  //   $info['content'] = $this->view->render();
  //   $info['subject'] = filter_xss_admin($this->view->get_title());
  //   if (!empty($this->view->result) || $this->get_option('empty') || !empty($this->view->style_plugin->definition['even empty'])) {
  //     return $info;
  //   }
  // }

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
          '#description' => t('This will generate the blocks and will be the arguments for those blocks.'),
          '#default_value' => $this->get_option('block_sets'),
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

  function pre_execute() {
    $block_sets = $this->get_option('block_sets');
    $this->view->set_arguments(array(2));
    parent::pre_execute();
  }
}

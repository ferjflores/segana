<?php

/**
 * Generates the informe type editing form.
 */
function informe_type_form($form, &$form_state, $informe_type, $op = 'edit') {

  if ($op == 'clone') {
    $informe_type->label .= ' (cloned)';
    $informe_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $informe_type->label,
    '#description' => t('The human-readable name of this informe type.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($informe_type->type) ? $informe_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $informe_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'informe_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this informe type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($informe_type->description) ? $informe_type->description : '',
    '#description' => t('Description about the informe type.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save informe type'),
    '#weight' => 40,
  );

  if (!$informe_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete informe type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('informe_type_form_submit_delete')
    );
  }
  return $form;
}

/**
 * Submit handler for creating/editing informe_type.
 */
function informe_type_form_submit(&$form, &$form_state) {
  $informe_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  informe_type_save($informe_type);

  // Redirect user back to list of informe types.
  $form_state['redirect'] = 'admin/structure/informe-types';
}

function informe_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/informe-types/' . $form_state['informe_type']->type . '/delete';
}

/**
 * Informe type delete form.
 */
function informe_type_form_delete_confirm($form, &$form_state, $informe_type) {
  $form_state['informe_type'] = $informe_type;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['informe_type_id'] = array('#type' => 'value', '#value' => entity_id('informe_type' ,$informe_type));
  return confirm_form($form,
    t('Are you sure you want to delete informe type %title?', array('%title' => entity_label('informe_type', $informe_type))),
    'informe/' . entity_id('informe_type' ,$informe_type),
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Informe type delete form submit handler.
 */
function informe_type_form_delete_confirm_submit($form, &$form_state) {
  $informe_type = $form_state['informe_type'];
  informe_type_delete($informe_type);

  watchdog('informe_type', '@type: deleted %title.', array('@type' => $informe_type->type, '%title' => $informe_type->label));
  drupal_set_message(t('@type %title has been deleted.', array('@type' => $informe_type->type, '%title' => $informe_type->label)));

  $form_state['redirect'] = 'admin/structure/informe-types';
}

/**
 * Page to select informe Type to add new informe.
 */
function informe_admin_add_page() {
  $items = array();
  foreach (informe_types() as $informe_type_key => $informe_type) {
    $items[] = l(entity_label('informe_type', $informe_type), 'informe/add/' . $informe_type_key);
  }
  return array('list' => array('#theme' => 'item_list', '#items' => $items, '#title' => t('Select type of informe to create.')));
}

/**
 * Add new informe page callback.
 */
function informe_add($type) {
  $informe_type = informe_types($type);

  $informe = entity_create('informe', array('type' => $type));
  drupal_set_title(t('Create @name', array('@name' => entity_label('informe_type', $informe_type))));

  $output = drupal_get_form('informe_form', $informe);

  return $output;
}

/**
 * Informe Form.
 */
function informe_form($form, &$form_state, $informe) {
  $form_state['informe'] = $informe;

  $form['title'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Name'),
    '#default_value' => $informe->title,
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $informe->description,
  );

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $informe->uid,
  );

  field_attach_form('informe', $informe, $form, $form_state);

  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions'] = array(
    '#weight' => 100,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save informe'),
    '#submit' => $submit + array('informe_form_submit'),
  );

  // Show Delete button if we edit informe.
  $informe_id = entity_id('informe' ,$informe);
  if (!empty($informe_id) && informe_access('edit', $informe)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('informe_form_submit_delete'),
    );
  }

  $form['#validate'][] = 'informe_form_validate';

  return $form;
}

function informe_form_validate($form, &$form_state) {

}

/**
 * Informe submit handler.
 */
function informe_form_submit($form, &$form_state) {
  $informe = $form_state['informe'];

  entity_form_submit_build_entity('informe', $informe, $form, $form_state);

  informe_save($informe);

  $informe_uri = entity_uri('informe', $informe);

  $form_state['redirect'] = $informe_uri['path'];

  drupal_set_message(t('Informe %title saved.', array('%title' => entity_label('informe', $informe))));
}

function informe_form_submit_delete($form, &$form_state) {
  $informe = $form_state['informe'];
  $informe_uri = entity_uri('informe', $informe);
  $form_state['redirect'] = $informe_uri['path'] . '/delete';
}

/**
 * Delete confirmation form.
 */
function informe_delete_form($form, &$form_state, $informe) {
  $form_state['informe'] = $informe;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['informe_type_id'] = array('#type' => 'value', '#value' => entity_id('informe' ,$informe));
  $informe_uri = entity_uri('informe', $informe);
  return confirm_form($form,
    t('Are you sure you want to delete informe %title?', array('%title' => entity_label('informe', $informe))),
    $informe_uri['path'],
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Delete form submit handler.
 */
function informe_delete_form_submit($form, &$form_state) {
  $informe = $form_state['informe'];
  informe_delete($informe);

  drupal_set_message(t('Informe %title deleted.', array('%title' => entity_label('informe', $informe))));

  $form_state['redirect'] = '<front>';
}

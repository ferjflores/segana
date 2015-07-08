<?php
/**
 * Implements hook_entity_info().
 */
function informe_entity_info() {
  $return = array(
    'informe' => array(
      'label' => t('Informe'),
      'entity class' => 'Informe',
      'controller class' => 'InformeController',
      'base table' => 'informe',
      'fieldable' => TRUE,
      'entity keys' => array(
        'id' => 'iid',
        'bundle' => 'type',
      ),
      'bundle keys' => array(
        'bundle' => 'type',
      ),
      'bundles' => array(),
      'load hook' => 'informe_load',
      'view modes' => array(
        'full' => array(
          'label' => t('Default'),
          'custom settings' => FALSE,
        ),
      ),
      'label callback' => 'entity_class_label',
      'uri callback' => 'entity_class_uri',
      'module' => 'informe',
      'access callback' => 'informe_access',
    ),
  );
  $return['informe_type'] = array(
    'label' => t('Informe Type'),
    'entity class' => 'InformeType',
    'controller class' => 'InformeTypeController',
    'base table' => 'informe_type',
    'fieldable' => FALSE,
    'bundle of' => 'informe',
    'exportable' => TRUE,
    'entity keys' => array(
      'id' => 'id',
      'name' => 'type',
      'label' => 'label',
    ),
    'module' => 'informe',
    // Enable the entity API's admin UI.
    'admin ui' => array(
      'path' => 'admin/structure/informe-types',
      'file' => 'informe.admin.inc',
      'controller class' => 'InformeTypeUIController',
    ),
    'access callback' => 'informe_type_access',
  );

  return $return;
}

/**
 * Implements hook_entity_info_alter().
 */
function informe_entity_info_alter(&$entity_info) {
  foreach (informe_types() as $type => $info) {
    $entity_info['informe']['bundles'][$type] = array(
      'label' => $info->label,
      'admin' => array(
        'path' => 'admin/structure/informe-types/manage/%informe_type',
        'real path' => 'admin/structure/informe-types/manage/' . $type,
        'bundle argument' => 4,
      ),
    );
  }
}


/**
 * Implements hook_entity_property_info_alter().
 */
function informe_entity_property_info_alter(&$info) {
  $properties = &$info['informe']['properties'];
  $properties['created'] = array(
    'label' => t("Date created"),
    'type' => 'date',
    'description' => t("The date the node was posted."),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer nodes',
    'schema field' => 'created',
  );
  $properties['changed'] = array(
    'label' => t("Date changed"),
    'type' => 'date',
    'schema field' => 'changed',
    'description' => t("The date the node was most recently updated."),
  );
  $properties['uid'] = array(
    'label' => t("Author"),
    'type' => 'user',
    'description' => t("The author of the informe."),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer informe entities',
    'required' => TRUE,
    'schema field' => 'uid',
  );
}


/*******************************************************************************
 ********************************* Informe API's **********************************
 ******************************************************************************/

/**
 * Access callback for informe.
 */
function informe_access($op, $informe, $account = NULL, $entity_type = NULL) {
  global $user;

  if (!isset($account)) {
    $account = $user;
  }
  switch ($op) {
    case 'create':
      return user_access('administer informe entities', $account)
          || user_access('create informe entities', $account);
    case 'view':
      return user_access('administer informe entities', $account)
          || user_access('view informe entities', $account);
    case 'edit':
      return user_access('administer informe entities')
          || user_access('edit any informe entities')
          || (user_access('edit own informe entities') && ($informe->uid == $account->uid));
  }
}

/**
 * Load a informe.
 */
function informe_load($iid, $conditions = array(), $reset = FALSE) {
  $informes = informe_load_multiple(array($iid), $conditions, $reset);
  return reset($informes);
}

/**
 * Load multiple informes based on certain conditions.
 */
function informe_load_multiple($iids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('informe', $iids, $conditions, $reset);
}

/**
 * Save informe.
 */
function informe_save($informe) {
  entity_save('informe', $informe);
}

/**
 * Delete single informe.
 */
function informe_delete($informe) {
  entity_delete('informe', entity_id('informe' ,$informe));
}

/**
 * Delete multiple informes.
 */
function informe_delete_multiple($informe_ids) {
  entity_delete_multiple('informe', $informe_ids);
}


/*******************************************************************************
 ****************************** Informe Type API's ********************************
 ******************************************************************************/

/**
 * Access callback for informe Type.
 */
function informe_type_access($op, $entity = NULL) {
  return user_access('administer informe types');
}

/**
 * Load informe Type.
 */
function informe_type_load($informe_type) {
  return informe_types($informe_type);
}

/**
 * List of informe Types.
 */
function informe_types($type_name = NULL) {
  $types = entity_load_multiple_by_name('informe_type', isset($type_name) ? array($type_name) : FALSE);
  return isset($type_name) ? reset($types) : $types;
}

/**
 * Save informe type entity.
 */
function informe_type_save($informe_type) {
  entity_save('informe_type', $informe_type);
}

/**
 * Delete single case type.
 */
function informe_type_delete($informe_type) {
  entity_delete('informe_type', entity_id('informe_type' ,$informe_type));
}

/**
 * Delete multiple case types.
 */
function informe_type_delete_multiple($informe_type_ids) {
  entity_delete_multiple('informe_type', $informe_type_ids);
}

/**
  * Implements hook_views_api().
  */
function informe_views_api() {
  return array(
    'api' => 3,
    'path' => drupal_get_path('module', 'informe'),
  );
}

?>

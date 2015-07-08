<?php
/**
 * @file
 * Hooks provided by this module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Acts on informe being loaded from the database.
 *
 * This hook is invoked during $informe loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $entities
 *   An array of $informe entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_informe_load(array $entities) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}


/**
 * Responds when a $informe is inserted.
 *
 * This hook is invoked after the $informe is inserted into the database.
 *
 * @param Informe $informe
 *   The $informe that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_informe_insert(Informe $informe) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('informe', $informe),
      'extra' => print_r($informe, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a $informe being inserted or updated.
 *
 * This hook is invoked before the $informe is saved to the database.
 *
 * @param Informe $informe
 *   The $informe that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_informe_presave(Informe $informe) {
  $informe->name = 'foo';
}

/**
 * Responds to a $informe being updated.
 *
 * This hook is invoked after the $informe has been updated in the database.
 *
 * @param Informe $informe
 *   The $informe that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_informe_update(Informe $informe) {
  db_update('mytable')
    ->fields(array('extra' => print_r($informe, TRUE)))
    ->condition('id', entity_id('informe', $informe))
    ->execute();
}

/**
 * Responds to $informe deletion.
 *
 * This hook is invoked after the $informe has been removed from the database.
 *
 * @param Informe $informe
 *   The $informe that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_informe_delete(Informe $informe) {
  db_delete('mytable')
    ->condition('pid', entity_id('informe', $informe))
    ->execute();
}

/**
 * Act on a informe that is being assembled before rendering.
 *
 * @param $informe
 *   The informe entity.
 * @param $view_mode
 *   The view mode the informe is rendered in.
 * @param $langcode
 *   The language code used for rendering.
 *
 * The module may add elements to $informe->content prior to rendering. The
 * structure of $informe->content is a renderable array as expected by
 * drupal_render().
 *
 * @see hook_entity_prepare_view()
 * @see hook_entity_view()
 */
function hook_informe_view($informe, $view_mode, $langcode) {
  $informe->content['my_additional_field'] = array(
    '#markup' => $additional_field,
    '#weight' => 10,
    '#theme' => 'mymodule_my_additional_field',
  );
}

/**
 * Alter the results of entity_view() for informes.
 *
 * @param $build
 *   A renderable array representing the informe content.
 *
 * This hook is called after the content has been assembled in a structured
 * array and may be used for doing processing which requires that the complete
 * informe content structure has been built.
 *
 * If the module wishes to act on the rendered HTML of the informe rather than
 * the structured content array, it may use this hook to add a #post_render
 * callback. Alternatively, it could also implement hook_preprocess_informe().
 * See drupal_render() and theme() documentation respectively for details.
 *
 * @see hook_entity_view_alter()
 */
function hook_informe_view_alter($build) {
  if ($build['#view_mode'] == 'full' && isset($build['an_additional_field'])) {
    // Change its weight.
    $build['an_additional_field']['#weight'] = -10;

    // Add a #post_render callback to act on the rendered HTML of the entity.
    $build['#post_render'][] = 'my_module_post_render';
  }
}

/**
 * Acts on informe_type being loaded from the database.
 *
 * This hook is invoked during informe_type loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $entities
 *   An array of informe_type entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_informe_type_load(array $entities) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
 * Responds when a informe_type is inserted.
 *
 * This hook is invoked after the informe_type is inserted into the database.
 *
 * @param InformeType $informe_type
 *   The informe_type that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_informe_type_insert(InformeType $informe_type) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('informe_type', $informe_type),
      'extra' => print_r($informe_type, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a informe_type being inserted or updated.
 *
 * This hook is invoked before the informe_type is saved to the database.
 *
 * @param InformeType $informe_type
 *   The informe_type that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_informe_type_presave(InformeType $informe_type) {
  $informe_type->name = 'foo';
}

/**
 * Responds to a informe_type being updated.
 *
 * This hook is invoked after the informe_type has been updated in the database.
 *
 * @param InformeType $informe_type
 *   The informe_type that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_informe_type_update(InformeType $informe_type) {
  db_update('mytable')
    ->fields(array('extra' => print_r($informe_type, TRUE)))
    ->condition('id', entity_id('informe_type', $informe_type))
    ->execute();
}

/**
 * Responds to informe_type deletion.
 *
 * This hook is invoked after the informe_type has been removed from the database.
 *
 * @param InformeType $informe_type
 *   The informe_type that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_informe_type_delete(InformeType $informe_type) {
  db_delete('mytable')
    ->condition('pid', entity_id('informe_type', $informe_type))
    ->execute();
}

/**
 * Define default informe_type configurations.
 *
 * @return
 *   An array of default informe_type, keyed by machine names.
 *
 * @see hook_default_informe_type_alter()
 */
function hook_default_informe_type() {
  $defaults['main'] = entity_create('informe_type', array(
    // …
  ));
  return $defaults;
}

/**
 * Alter default informe_type configurations.
 *
 * @param array $defaults
 *   An array of default informe_type, keyed by machine names.
 *
 * @see hook_default_informe_type()
 */
function hook_default_informe_type_alter(array &$defaults) {
  $defaults['main']->name = 'custom name';
}


/**
 * Determines whether a informe hook exists.
 * 
 * @param $informe
 *   A informe object or a string containing the informe type.
 * @param $hook
 *   A string containing the name of the hook.
 * 
 * @return
 *   TRUE if the $hook exists in the informe type of $informe.
 */
function informe_hook($informe, $hook) {
  $base = informe_type_get_base($informe);
  return module_hook($base, $hook);
}
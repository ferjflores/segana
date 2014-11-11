<?php
//// entidades
/**
 * Implements hook_entity_info().
 */
function informes_entity_info() {

  $info = array();

  $info['agenda_informativa'] = array(
    'label' => t('Agenda Informativa'),
    'base table' => 'agenda_informativa',
    'entity keys' => array(
      'id' => 'aiid',
      'label' => 'nombre',
    ),
    'entity class' => 'AgendaInformativaEntity',
    'controller class' => 'AgendaInformativaEntityController',
    'access callback' => 'informes_access_callback',
    'uri callback' => 'entity_class_uri',
    'admin ui' => array(
      'path' => 'admin/structure/informes/agenda_informativa',
      'controller class' => 'EntityDefaultUIController',
    ),
    'fieldable' => TRUE,
    'bundles' => array(
      'agenda_informativa' => array(
        'label' => t('Agenda Informativa'),
        'admin' => array(
          'path' => 'admin/structure/informes/agenda_informativa',
        ),
      ),
    ),
    'module' => 'informes',
  );

  return $info;
}

/**
 * Extending the EntityAPIController for the Agenda Informativa entity.
 */
class AgendaInformativaEntityController extends EntityAPIController {
  
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {

    $build = parent::buildContent($entity, $view_mode, $langcode, $content);
    
    
    return $build;
  
  }
  
}

/**
 * Agenda Informativa entity class extending the Entity class
 */
class AgendaInformativaEntity extends Entity {

  /**
   * Change the default URI from default/id to agenda/id
   */
  protected function defaultUri() {
    return array('path' => 'agenda/' . $this->identifier());
  }

}


/**
 * Access callback for Agenda Informativa entities.
 */
function informes_access_callback($op, $agenda_informativa = NULL, $account = NULL) {
  if ($op == 'view' || $op == 'update' || $op == 'create' || $op == 'delete') {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Form definition for adding / editing a agenda_informativa.
 */
function agenda_informativa_form($form, &$form_state, $agenda_informativa = NULL) {

  $form['nombre'] = array(
    '#title' => t('Nombre de la agenda'),
    '#type' => 'textfield',
    '#default_value' => isset($agenda_informativa->nombre) ? $agenda_informativa->nombre : '',
    '#required' => TRUE,
  );
  field_attach_form('agenda_informativa', $agenda_informativa, $form, $form_state);
  $form['submit'] = array(
    '#type' => 'submit', 
    '#value' => isset($agenda_informativa->id) ? t('Update agenda_informativa') : t('Guarda agenda informativa'),
    '#weight' => 50,
  );

  return $form;
}

/**
 * Submit handler for the agenda_informativa add/edit form.
 */
function agenda_informativa_form_submit($form, &$form_state) {
  $agenda_informativa = entity_ui_form_submit_build_entity($form, $form_state);
  $agenda_informativa->save();
  drupal_set_message(t('La agenda informativa: @name ha sido guardada.', array('@name' => $agenda_informativa->name)));
  $form_state['redirect'] = 'admin/structure/informes/agenda_informativa';
}
?>
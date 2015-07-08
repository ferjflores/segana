<?php

function vistas_borrar_form($form, &$form_state, $entity_id = NULL, $tipo_contenido)
{
  $form = array();

  $form['#prefix'] = '<div id="formwrapper">';
  $form['#suffix'] = '</div>';

  $form["wrapper"] = array("#markup" => "<div class='inline-messages'></div>");

  $form_state['entity_id'] = $entity_id;
  $form_state['tipo_contenido'] = $tipo_contenido;
  if (!isset($form_state['triggering_element']['#return_value'])) {
    $form['contenedor'] = array(
      '#type' => 'fieldset',
      '#attributes' => array('class' => 'contenedor'),
      '#prefix' => '<div id="contenedor">',
      '#suffix' => '</div>',
    );
    $form['contenedor']['mensaje'] = array(
      '#markup' => '¿Esta seguro de borrar est' . (in_array($tipo_contenido, array('area', 'matriz')) ? 'a '  : 'e ')  . ($tipo_contenido == 'area' ? 'área' : $tipo_contenido) . '?',
    );
    $form['contenedor']['botones'] = array(
      '#type' => 'fieldset',
      '#prefix' => '<div id="botones">',
      '#suffix' => '</div>',
    );

    $form['contenedor']['botones']['eliminar'] = array(
      '#type' => 'submit',
      '#value' => t('Eliminar'),
      '#return_value' => t('Eliminar'),
      '#name' => 'Eliminar',
      '#submit' => array('vistas_borrar_form_submit'),
      '#ajax' => array(
        'event' => 'click',
        'callback' => 'form_vistas_borrar_submit_callback',
        'wrapper' => 'contenedor',
        'method' => 'replace',
        'effect' => 'fade',
        'progress' => array('type' => 'throbber'),
      ),
    );

    $form['contenedor']['botones']['cancelar'] = array(
      '#type' => 'submit',
      '#value' => t('Cancelar'),
      '#prefix' => '<div id="guardar">',
      '#suffix' => '</div>',
      '#ajax' => array(
        'event' => 'click',
        'callback' => 'form_vistas_borrar_cancelar_callback',
        'wrapper' => 'formwrapper',
        'method' => 'replace',
        'effect' => 'fade',
        'progress' => array('type' => 'none'),
      ),
    );
  }


  return $form;
}

function vistas_borrar_form_submit($form, &$form_state) {
  $tipo_contenido = $form_state['tipo_contenido'];
  $entity_id = $form_state['entity_id'];
  $entity_type = ($tipo_contenido == 'nota') ? 'node' : 'taxonomy_term';

  if (entity_exists($entity_id, $entity_type)) {
    if ($tipo_contenido == 'nota') {
      node_delete($entity_id);
    }
    else {
      //chequear que no tenga notas asociadas
      $nids_count = taxonomy_field_nodes($entity_id, $tipo_contenido);
      if ($nids_count == 0) {
        taxonomy_term_delete($entity_id);
      }
      else {
        form_set_error('', 'No se puede eliminar est' . (in_array($tipo_contenido, array('area', 'matriz')) ? 'a ' : 'e ') . ($tipo_contenido == 'area' ? 'área' : $tipo_contenido) . ' con id ' . $entity_id . ' tiene notas vinculadas');
      }
    }
    if (!entity_exists($entity_id, $entity_type)) {
      drupal_set_message('Se ha eliminado ' . 'est' . (in_array($tipo_contenido, array('area', 'matriz')) ? 'a ' : 'e ') . ($tipo_contenido == 'area' ? 'área' : $tipo_contenido) . ' con id ' . $entity_id);
    }
  }
  else {
    form_set_error('', 'est' . (in_array($tipo_contenido, array('area', 'matriz')) ? 'a ' : 'e ') . ($tipo_contenido == 'area' ? 'área' : $tipo_contenido) . ' con id ' . $entity_id . ' no existe actualmente');
  }
  $form_state['rebuild'] = TRUE;
}


function form_vistas_borrar_submit_callback($form, $form_state) {
  $messages = theme('status_messages');
  if($messages){
    $form["wrapper"] = array("#markup" => "<div class='inline-messages'>$messages</div>");
  }
  return array(
    '#type' => 'ajax',
    '#commands' => array(
      ajax_command_replace("#formwrapper", render($form)),
      array(
        'command' => 'afterAjaxCallbackBorrarCerrar',
      )
    )
  );
}

?>
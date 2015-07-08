<?php
function vistas_discursivo_form($form, &$form_state, $tid = NULL, $elemento_discursivo = NULL) {
  $form = array();
  if ($form_state['triggering_element']['#return_value'] == 'Eliminar') {
    return $form;
  }
  //determinar si es un elemento nuevo o editar(cargar valores)
  $vocabulary = taxonomy_vocabulary_machine_name_load($elemento_discursivo);
  $nombre_elemento_discursivo = $vocabulary->name;
  $campos = campos('taxonomy_term', $elemento_discursivo);

  if ( ($tid) && empty($form_state['clicked_button']['#value']) ){
    $wrapper = entity_metadata_wrapper('taxonomy_term', taxonomy_term_load($tid));
    $name = $wrapper->label();
    //$description = isset($term->description) ? $term->description : NULL;
    $description = $wrapper->description->raw();

    if (count($campos) > 0) {
      foreach ($campos as $field_name => $values) {
        if ($values['type'] == 'taxonomy_term_reference' || $values['type'] == 'entity_reference') {
          $$field_name = $wrapper->$field_name->getIdentifier();
        }
        else {
          $$field_name = $wrapper->$field_name->value();
        }
      }
    }


  }

  $form['contenedor'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="contenedor">',
    '#suffix' => '</div>',
  );

  $form['contenedor']['tid'] = array(
    '#type' => 'token',
    '#value' => isset($form_state['values']['tid']) ? $form_state['values']['tid'] : (isset($tid) ? $tid : NULL),
  );
  $form_state['elemento_discursivo'] = isset($form_state['elemento_discursivo']) ? $form_state['elemento_discursivo'] : (isset($elemento_discursivo) ? $elemento_discursivo : NULL);

  $form['contenedor']['name'] = array(
    '#type' => 'textfield',
    '#prefix' => '<div class="nombre_discursivo">',
    '#suffix' => '</div>',
    '#title' => ucfirst($nombre_elemento_discursivo),
    '#attributes' => array('autocomplete' =>'off'),
    '#default_value' => !empty($form_state['values']['name']) ? $form_state['values']['name'] : (isset($name) ? $name : NULL),
    '#size' => 70,
    '#maxlength' => 256,
    '#required' => TRUE,
  );
  if ($tid || isset($form_state['values']['tid'])) {

    $nids_count = taxonomy_field_nodes($tid, $form_state['elemento_discursivo']);
    $plural = ($nids_count > 1 ? 's' : '');
    $link = l($nids_count,'notas', array('attributes' => array('class' => 'ver', 'target'=>'_blank', 'title' => 'Ver notas asociadas'), 'query' => array($elemento_discursivo => $name)));


    $link_image = l(theme_image(array('path' => '/sites/all/themes/segana/images/listado.png')),'notas', array('attributes' => array('class' => 'ver',  'target'=>'_blank', 'title' => 'Ver notas asociadas'), 'query' => array($elemento_discursivo => $name), 'html' => TRUE));

    $notas_asociadas = theme('html_tag', array(
      'element' => array(
        '#tag' => 'div',
        '#attributes' => array(
          'id' => 'notas_asociadas',
        ),
        '#value' => 'Hay '. $link . ' nota' . $plural . ' asociada' . $plural . (($elemento_discursivo == 'matriz') ? ' a la ' : ' al ') . $elemento_discursivo . $link_image,
      ),
    ));

    $form['contenedor']['notas_asociadas'] = array(
      '#markup' => $notas_asociadas,
    );
  }

  $form['contenedor']['description'] = array(
    '#type' => 'textarea',
    '#title' => 'Descripción',
    '#attributes' => array('autocomplete' =>'off'),
    '#prefix' => '<div class="description">',
    '#suffix' => '</div>',
    '#default_value' => !empty($form_state['values']['description']) ? $form_state['values']['description'] : $description,
    '#size' => 50,
    '#maxlength' => 2000,
    '#resizable' => FALSE,
    '#required' => ($elemento_discursivo == 'medio') ? FALSE : TRUE,
  );

  foreach($campos as $field_name => $values) {
    switch($values['type']) {
      case 'taxonomy_term_reference':
        $type = 'select';
        break;
      case 'text':
        $type = 'textfield';
        break;
      case 'list_boolean':
        $type = 'radios';
        break;
    }
    $form['contenedor'][$field_name] = array(
      '#type' => $type,
      '#title' => $values['label'],
      '#prefix' => '<div class="campos_adicionales">',
      '#suffix' => '</div>',
      '#default_value' => isset($form_state['values'][$field_name]) ? $form_state['values'][$field_name] : (isset($$field_name) ? $$field_name : NULL),
    );
    if ($values['type'] == 'taxonomy_term_reference') {
      $form['contenedor'][$field_name]['#options'] = $values['terms'];
      $form['contenedor'][$field_name]['#empty_option'] = 'Seleccionar';
    }
    elseif ($values['type'] == 'text' | $values['type'] == 'number'){
      $form['contenedor'][$field_name]['#size'] = 70;
      $form['contenedor'][$field_name]['#maxlength'] = 256;
    }
    elseif ($values['type'] == 'list_boolean') {
      $form['contenedor'][$field_name]['#options'] = $values['allowed_values'];
      $form['contenedor'][$field_name]['#default_value'] = isset($form_state['values'][$field_name]) ? $form_state['values'][$field_name] : (!empty($$field_name) ? $$field_name : '0');
    }
  }


  if ($tid || isset($form_state['values']['tid'])) {
    $form['contenedor']['fusion'] =array(
      '#type' => 'fieldset',
      '#prefix' => '<div class="fusionar" id="fusionar">',
      '#suffix' => '</div>',
    );
    $form['contenedor']['fusion']['fusionar'] = array(
      '#type' => 'textfield',
      '#title' => 'Fusionar dentro del ' . $nombre_elemento_discursivo . ' actual',
      '#attributes' => array('autocomplete' =>'off'),
      '#size' => 70,
      '#maxlength' => 256,
      '#autocomplete_path' => 'notas/autocomplete/' . $elemento_discursivo .'/0/0',
    );
    $form['contenedor']['fusion']['boton_fusionar'] = array(
      '#type' => 'image_button',
      '#description' => 'Fusionar',
      '#src' => 'sites/all/themes/segana/images/enter.png',
      '#return_value' => "fusionar",
      '#submit' => array('vistas_discursivo_form_submit'),
      '#prefix' => '<div id="boton_fusionar">',
      '#suffix' => '</div>',
      '#attributes' => array('title' => 'Ejecutar fusion con este elemento'),
      '#ajax' => array(
        'event' => 'click',
        'callback' => 'form_vistas_discursivo_fusionar_callback',
        'wrapper' => 'formwrapper',
        'method' => 'html',
        'effect' => 'fade',
        'progress' => array('type' => 'none'),
        //'prevent' => 'submit click mousedown',
      ),
      '#limit_validation_errors' => array(array('fusionar'), array('boton_fusionar'), array('tid')),
    );
  }

  if ($elemento_discursivo == 'medio' && ($tid) && empty($form_state['clicked_button']['#value'])) {
    $form_state['ultima_actualizacion'] = date('d/m/Y h:i a', $wrapper->field_ultima_actualizacion->value());
    $form['contenedor']['ultima_actualizacion'] = array(
      '#markup' => "<div class='campos_adicionales'> <label>Última Actualización: " . $form_state['ultima_actualizacion'] . "</label></div>",
    );

  }
  $form['contenedor']['admin'] = opciones_admin_medio($elemento_discursivo, $tid);


  $form['contenedor']['botones'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="botones">',
    '#suffix' => '</div>',
  );

  $form['contenedor']['botones']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#return_value' => t('Reset'),
//		'#attributes' => array('onclick' => 'this.form.reset(); return false;'),
  );
  if ($tid || isset($form_state['values']['tid'])) {
    $form['contenedor']['botones']['eliminar'] = array(
      '#type' => 'submit',
      '#value' => t('Eliminar'),
      '#return_value' => t('Eliminar'),
      '#submit' => array('vistas_discursivo_form_submit'),
      '#ajax' => array(
        'event' => 'click',
        'callback' => 'form_vistas_discursivo_eliminar_callback',
        'wrapper' => 'formwrapper',
        'method' => 'replace',
        'effect' => 'fade',
        'progress' => array('type' => 'none'),
      ),
      //'#limit_validation_errors' => array(array('tid')),
    );
  }
  $form['#prefix'] = '<div id="formwrapper">';
  $form['#suffix'] = '</div>';

  //$form_id = str_replace('_', '-', $form_state['build_info']['form_id']);
  $form['contenedor']['botones']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Guardar'),
    '#return_value' => 'guardar',
    '#submit' => array('vistas_discursivo_form_submit'),
    '#prefix' => '<div id="guardar">',
    '#suffix' => '</div>',
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'form_vistas_discursivo_submit_callback',
      'effect' => 'fade',
      'wrapper' => 'formwrapper',
      'method' => 'replace',
      'progress' => array('type' => 'throbber'),
      //'prevent' => 'submit click mousedown',
    ),
  );
  return $form;

}


function vistas_discursivo_form_submit($form, &$form_state) {
  $pressed_button = check_plain($form_state['triggering_element']['#return_value']);
  switch ($pressed_button) {

    case 'fusionar':
      $vocabulary = taxonomy_vocabulary_machine_name_load($form_state['elemento_discursivo']);
      $tid_fusionar = key(buscar_termino_por_vid($form_state['values']['fusionar'], $vocabulary->vid));
      $fusion = fusionar_elementos($form_state['elemento_discursivo'], array($tid_fusionar), $form_state['values']['tid']);
      drupal_set_message("Se alteraron " . $fusion['cantidad_notas'] . " notas");
      drupal_set_message("Se alteraron " . $fusion['filas_discursivas'] . " filas discursivas");
      $form_state['rebuild'] = TRUE;
      break;

    case 'Eliminar':
      $entity_id = $form_state['values']['tid'];
      $term = taxonomy_term_load($entity_id);
      $vocabulary = taxonomy_vocabulary_load($term->vid);
      $tipo_contenido = $vocabulary->machine_name;

      if (entity_exists($entity_id, 'taxonomy_term')) {
        if ($tipo_contenido == 'nota') {
          node_delete($entity_id);
        }
        else {
          taxonomy_term_delete($entity_id);
        }
        if (!entity_exists($entity_id, 'taxonomy_term')) {
          drupal_set_message('Se ha eliminado ' . 'est' . (in_array($tipo_contenido, array('area', 'matriz')) ? 'a ' : 'e ') . ($tipo_contenido == 'area' ? 'área' : $tipo_contenido) . ' con id ' . $entity_id);
          unset($form_state['values']['tid']);
        }
      }
      else {
        form_set_error('', 'est' . (in_array($tipo_contenido, array('area', 'matriz')) ? 'a ' : 'e ') . ($tipo_contenido == 'area' ? 'área' : $tipo_contenido) . ' con id ' . $entity_id . ' no existe actualmente');
      }
      $form_state['rebuild'] = TRUE;
      break;

    case 'Reiniciar':
      $form_state['rebuild'] = TRUE;
      unset($form_state['values']);
      break;
    case 'guardar':
      $fecha = time();
      global $user;

      if (isset($form_state['values']['tid'])) {
        $wrapper = entity_metadata_wrapper('taxonomy_term', taxonomy_term_load($form_state['values']['tid']));
        $wrapper->name->set($form_state['values']['name']);
        $wrapper->field_fecha_actualizacion->set($fecha);
        $wrapper->field_usuario_actualizacion->set($user->uid);

        //para reparar la fecha de creacion, si no existe se establece como la fecha de actualizacion
        $fecha_creacion = $wrapper->field_fecha->value();
        if (!isset($fecha_creacion)) {
          $wrapper->field_fecha->set($fecha);
          $wrapper->field_user->set($user->uid);
        }
      }
      else {
        $vocabulary = taxonomy_vocabulary_machine_name_load($form_state['elemento_discursivo']);
        $term = entity_create('taxonomy_term', array(
            'vid' => $vocabulary->vid,
            'name' => $form_state['values']['name'],
          )
        );
        $wrapper = entity_metadata_wrapper('taxonomy_term', $term);
        $wrapper->save();
        $wrapper->field_fecha->set($fecha);
        $wrapper->field_user->set($user->uid);
      }

      $campos = campos('taxonomy_term', $form_state['elemento_discursivo']);
      $wrapper->description->set($form_state['values']['description']);

      if (!empty($campos)) {
        foreach ($campos as $field_name => $values) {
          if (isset($form_state['values'][$field_name]) && $form_state['values'][$field_name] !== '') {
            if ($values['type'] == 'taxonomy_term_reference') {
              $wrapper->$field_name->set($form_state['values'][$field_name]);
            }
            else {
              $wrapper->$field_name->set($form_state['values'][$field_name]);
            }
          }
        }
      }

      $wrapper->save();

      //opciones admin
      if ($user->uid == '1') {
        $tid = $wrapper->getIdentifier();
        $term = taxonomy_term_load($tid);
        $variables = unserialize($term->field_variables_medio[LANGUAGE_NONE][0]['value']);
        $variables['canal_rss'] = $form_state['values']['admin']['canal_rss'];
        $variables['canal_seccion'] = $form_state['values']['admin']['canal_seccion'];

        $term->field_variables_medio[LANGUAGE_NONE][0]['value'] = serialize($variables);
        $term->field_actualizacion_automatica[LANGUAGE_NONE][0]['value'] = $form_state['values']['admin']['actualizacion_automatica'];
        taxonomy_term_save($term);
      }

      if (isset($form_state['values']['tid'])) {
        drupal_set_message(ucfirst($form_state['elemento_discursivo']) ." '" . $form_state['values']['name'] . "' actualizada con id: " . $form_state['values']['tid']);
      }
      else {
        $titulo = l($form_state['values']['name'], 'editar/discursivo/' . $wrapper->tid->value(), array('attributes' => array('class' => array('ver'), 'target' => '_blank')));
        drupal_set_message(ucfirst($form_state['elemento_discursivo']) ." '" . $titulo . "' insertada con tid: " . $term->tid);
        $form_state['input'] = array();
        $form_state['values'] = array();
      }

      $form_state['rebuild'] = TRUE;
      break;
  }
}



function vistas_discursivo_form_validate($form, &$form_state) {
  if ($form_state['triggering_element']['#return_value'] == 'Eliminar') {
    $nids = taxonomy_select_nodes($form_state['values']['tid'], FALSE);
    if (!empty($nids)){
      $nids_count = count($nids);
      $plural = ($nids_count > 1 ? 's' : '');
      $link = l($nids_count,'notas', array('attributes' => array('class' => 'ver',  'target'=>'_blank'), 'query' => array($form_state['elemento_discursivo'] => $form_state['input']['name'])));
      form_set_error("", "Hay $link nota$plural asociada$plural " . (($form_state['values']['elemento_discursivo'] == 'matriz') ? ' a la ' : ' al ') . " " . $form_state['elemento_discursivo']);
    }
  }
  if ($form_state['triggering_element']['#return_value'] == 'fusionar') {
    if (!user_access('acceso edicion variables')) {
      form_set_error('boton_fusionar', t('No dispone de permisos suficientes para realizar esta operación'));
    }
    else {
      //validar reemplazar por tema
      $entity_id = $form_state['values']['tid'];
      $term = taxonomy_term_load($entity_id);
      $tid_fusionar = buscar_termino_por_vid($form_state['values']['fusionar'], $term->vid);
      if (empty($tid_fusionar)) {
        form_set_error("fusionar", "El valor '" . $form_state['values']['fusionar'] . "' es invalido para el campo " . $form['contenedor']['fusion']['fusionar']['#title']);
      }
    }
  }
  if ($form_state['triggering_element']['#return_value'] == 'guardar' && !isset($form_state['values']['tid'])) {
    $tid = key(taxonomy_get_term_by_name($form_state['values']['name'], $form_state['elemento_discursivo']));
    if (!empty($tid)) {
      form_set_error('', "Ya existe este elemento");
    }
  }
}

function form_vistas_discursivo_submit_callback($form, $form_state) {
  $messages = theme('status_messages');
  $commands = array();
  //if($messages){
  $commands[] = ajax_command_after(NULL, '<div id="messages">' . $messages . '</div>');
  //}

  $commands[] = ajax_command_replace("#formwrapper", render($form));

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

function form_vistas_discursivo_eliminar_callback($form, &$form_state){
  $messages = theme('status_messages');
  $commands = array();
  if($messages){
    $commands[] =   ajax_command_after(NULL, '<div id="messages">' . $messages . '</div>');
  }

  $commands[] = ajax_command_replace("#formwrapper", render($form));

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

function form_vistas_discursivo_medio_admin_callback($form, $form_state) {
  return array(
    '#type' => 'ajax',
    '#commands' => array(
      array (
        // The command in my JS file
        'command' => 'afterAjaxCallbackMedioAdmin',
        // Pass the needed data to be processed and displayed for the page
        'selectedValue' => $form_state['values']['tid'],
        'selectedTipo' => $form_state['triggering_element']['#return_value'],
      ),
    )
  );
}



function opciones_admin_medio($elemento_discursivo, $tid = NULL) {
  global $user;
  if (($user->uid == 1) && ($elemento_discursivo == 'medio')) {
    if (isset($tid)) {
      $term = taxonomy_term_load($tid);
      $variables = unserialize($term->field_variables_medio[LANGUAGE_NONE][0]['value']);
      $actualizacion_automatica = $term->field_actualizacion_automatica[LANGUAGE_NONE][0]['value'];
    }
    $opciones_admin = array(
      '#type' => 'fieldset',
      '#prefix' => '<div id="opciones-admin">',
      '#suffix' => '</div>',
      '#tree' => TRUE,
    );
    $opciones_admin['actualizacion_automatica'] = array(
      '#title' => 'Actualización automática',
      '#type' => 'select',
      '#options' => array(0 => 'No', 1 => 'Sí'),
      '#default_value' => isset($actualizacion_automatica) ? $actualizacion_automatica : NULL,
    );
    $opciones_admin['canal_rss'] = array(
      '#title' => 'Canales RSS',
      '#type' => 'textarea',
      '#default_value' => isset($variables['canal_rss']) ? $variables['canal_rss'] : NULL,
      '#description' => 'Un canal RSS por linea y su área ej: "Economía|http://www.eluniversal.com/rss/eco_avances.xml"',
      '#resizable' => FALSE,
    );

    $opciones_admin['canal_seccion'] = array(
      '#title' => 'Canales Sección',
      '#type' => 'textarea',
      '#default_value' => isset($variables['canal_seccion']) ? $variables['canal_seccion'] : NULL,
      '#description' => 'Un canal Sección por linea y su área ej: "Justicia y Seguridad|http://www.el-nacional.com/sucesos/"',
      '#resizable' => FALSE,
    );

    $opciones_admin['opciones_contenido'] = array(
      '#type' => 'submit',
      '#value' => t('Opciones contenido'),
      '#return_value' => 'contenido',
      '#submit' => array(),
      '#ajax' => array(
        'event' => 'click',
        'callback' => 'form_vistas_discursivo_medio_admin_callback',
        'wrapper' => '',
        'method' => '',
        'effect' => 'fade',
        'progress' => array('type' => 'throbber'),
        //'prevent' => 'submit click mousedown',
      ),
    );
    $opciones_admin['opciones_seccion'] = array(
      '#type' => 'submit',
      '#value' => t('Opciones sección'),
      '#return_value' => 'seccion',
      '#submit' => array(),
      '#ajax' => array(
        'event' => 'click',
        'callback' => 'form_vistas_discursivo_medio_admin_callback',
        'wrapper' => '',
        'method' => '',
        'effect' => 'fade',
        'progress' => array('type' => 'throbber'),
        //'prevent' => 'submit click mousedown',
      ),
    );
    return $opciones_admin;
  }
}



function vistas_medio_admin_form($form, &$form_state, $tipo, $entity_id) {
  $form['#attributes'] = array('class' => 'vistas_medio_admin_form');
  $term = taxonomy_term_load($entity_id);
  $variables = unserialize($term->field_variables_medio[LANGUAGE_NONE][0]['value']);

  $form['#prefix'] = '<div id="formwrapper">';
  $form['#suffix'] = '</div>';

  $form['contenedor'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => 'contenedor'),
    '#prefix' => '<div id="contenedor">',
    '#suffix' => '</div>',
  );

  if ($tipo == "contenido") {
    $form['contenedor']['superior'] = form_contenido($variables);
  }
  else {
    $form['contenedor']['superior'] = form_seccion($variables);
  }

  $form['contenedor']['medio'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => 'medio'),
    '#prefix' => '<div id="medio">',
    '#suffix' => '</div>',
  );
  $form['contenedor']['medio']['guardar'] = array(
    '#type' => 'submit',
    '#value' => t('Guardar'),
    '#return_value' => 'guardar',
    '#submit' => array('vistas_medio_admin_form_submit'),
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'form_vistas_medio_guardar_callback',
      'wrapper' => 'formwrapper',
      'method' => 'replace',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber'),
    ),
  );


  $form['contenedor']['inferior'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="inferior">',
    '#suffix' => '</div>',
  );


  if (strpos($form_state['triggering_element']['#return_value'], 'probar') !== FALSE) {
    $element = array();
    $element['#attributes'] = array();
    $url = ($tipo == 'contenido') ? 'url_prueba' : 'url_prueba_seccion';
    switch ($form_state['triggering_element']['#return_value']) {
      case (strpos($form_state['triggering_element']['#return_value'], "probar_url")):
        $element['#value'] = "";
        $element['#attributes']['src'] =  url("feeder/url/" . base64_encode($form_state['values']['superior'][$url]) . "/1", array('absolute' => TRUE));
        $element['#tag'] = 'iframe';
        break;
      case ((strpos($form_state['triggering_element']['#return_value'], "probar_contenido") !== FALSE) || (strpos($form_state['triggering_element']['#return_value'], "probar_seccion") !== FALSE)):
        $element['#value'] = feeder_url($form_state['values']['superior'][$url], FALSE, $tipo);
        $element['#tag'] = 'div';
        break;
      case ((strpos($form_state['triggering_element']['#return_value'], "probar_elemento") !== FALSE)):
        $fieldset = ($tipo == 'seccion') ? 'elementos_seccion' : 'elementos';
        $contenido_html = feeder_url($form_state['values']['superior'][$url], FALSE, $tipo);
        $elemento = str_replace('probar_elemento_', '',  $form_state['triggering_element']['#return_value']);
        $selector = $form_state['values']['superior'][$fieldset][$elemento];
        $element['#value'] = feeder_extraer($contenido_html, $selector);
        $element['#tag'] = 'div';
        break;
    }
    $element['#attributes']['id'] = 'resultado';

    $resultado = theme('html_tag', array('element' => $element));

    $form['contenedor']['inferior']['resultado'] = array(
      '#markup' => $resultado
    );
  }


  return $form;
}

function vistas_medio_admin_form_submit($form, &$form_state) {
  $pressed_button = check_plain($form_state['triggering_element']['#return_value']);
  switch ($pressed_button) {
    case 'guardar':
      $tid =  $form_state['build_info']['args'][1];
      $term = taxonomy_term_load($tid);
      $variables = unserialize($term->field_variables_medio[LANGUAGE_NONE][0]['value']);
      //pasar nuevo valores al arreglo variables
      form_state_values_clean($form_state);
      $values_form = $form_state['values']['superior'];
      foreach ($values_form as $key => $values) {
        $variables[$key] = $values;
      }
      $term->field_variables_medio[LANGUAGE_NONE][0]['value'] = serialize($variables);
      taxonomy_term_save($term);
      $form_state['rebuild'] = TRUE;
      break;
    case 'probar_url':
    case 'probar_url_seccion':
    case 'probar_contenido':
    case 'probar_seccion':
      $tipo =  $form_state['build_info']['args'][0];
      $tid =  $form_state['build_info']['args'][1];
      feeder_rules($tid, $form_state, $tipo);
      $form_state['rebuild'] = TRUE;
      break;
    case (strpos($pressed_button, "probar_")):
      $form_state['rebuild'] = TRUE;
      break;
  }
}

function form_vistas_medio_guardar_callback($form, $form_state) {
  $messages = theme('status_messages');
  $commands = array();
  if($messages){
    $commands[] = ajax_command_after(NULL, '<div id="messages">' . $messages . '</div>');
  }

  $commands[] = ajax_command_replace("#formwrapper", render($form));

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

function form_vistas_medio_probar_callback($form, $form_state) {
  $messages = theme('status_messages');
  $commands = array();
  if($messages){
    $commands[] = ajax_command_after(NULL, '<div id="messages">' . $messages . '</div>');
  }

  $commands[] = ajax_command_replace("#inferior", render($form['contenedor']['inferior']));

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

function form_contenido($variables) {

  $superior = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="superior">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );
  $superior['url_prueba'] = array(
    '#type' => 'textfield',
    '#title' => 'Url de prueba',
    '#maxlength' => 256,
    '#default_value' => isset($variables['url_prueba']) ? $variables['url_prueba'] : NULL,
  );
  $superior['probar_url'] = array(
    '#type' => 'submit',
    '#value' => t('Probar url'),
    '#return_value' => 'probar_url',
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'form_vistas_medio_probar_callback',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber'),
    ),
  );

  $superior['contenido'] = array(
    '#type' => 'textfield',
    '#title' => 'Contenedor del categoria, antetitulo, sumario, imagen, cuerpo...',
    '#default_value' => isset($variables['contenido']) ? $variables['contenido'] : NULL,
    '#description' => 'ej: <ul><li>div[@id="cont_cen1"]/div[@class="pod_izq_gl"]</li><li>*[contains(@class, "fb-like") or contains(@class, "social")]</li><li>*[@class="shareToolsBox"]</li><li>div[contains(@class, "field-name-body")]</li><li>div[@id="bodyContent"]</li><li>div[contains(@class, "article")][1]</li><li>div[@class="copytext"]|ul[@id="media-list"] (si son dos contenedores o mas)</li><li>div[contains(@class, "row")/img</li><li>div[@role="article"]/div[contains(@class, "eE")]</li>',
  );
  $superior['probar_contenido'] = array(
    '#type' => 'submit',
    '#value' => t('Probar contenido'),
    '#return_value' => 'probar_contenido',
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'form_vistas_medio_probar_callback',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber'),
    ),
  );
  $superior['contenido_grabber'] = array(
    '#type' => 'textfield',
    '#title' => 'Grabber del contenido',
    '#maxlength' => 256,
    '#default_value' => isset($variables['contenido_grabber']) ? $variables['contenido_grabber'] : '%.*%',
    '#description' => 'ej: %/.*/%'
  );

  $superior['elementos'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => 'contenedor'),
    '#prefix' => '<div id="elementos">',
    '#suffix' => '</div>',
  );

  $superior['elementos']['fecha'] = array(
    '#type' => 'textfield',
    '#title' => 'Contenedor de la fecha',
    '#default_value' => isset($variables['elementos']['fecha']) ? $variables['elementos']['fecha'] : NULL,
    '#description' => "ej: 'div.clear span.txt60', 0  (el numero indica cual elemento si hay varios elementos con el mismo selector)",
    '#maxlength'=> 400,
  );
  $superior['elementos']['probar_fecha'] = array(
    '#type' => 'submit',
    '#value' => t('Probar fecha'),
    '#return_value' => 'probar_elemento_fecha',
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'form_vistas_medio_probar_callback',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber'),
    ),
  );

  $superior['elementos']['etiqueta'] = array(
    '#type' => 'textfield',
    '#title' => 'Contenedor de la etiqueta(s) o categoria(s)',
    '#default_value' => isset($variables['elementos']['etiqueta']) ? $variables['elementos']['etiqueta'] : NULL,
    '#description' => "ej: 'div.clear span.txt60', 0  (el numero indica cual elemento si hay varios elementos con el mismo selector)",
    '#maxlength'=> 400,
  );
  $superior['elementos']['probar_etiqueta'] = array(
    '#type' => 'submit',
    '#value' => t('Probar etiqueta'),
    '#return_value' => 'probar_elemento_etiqueta',
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'form_vistas_medio_probar_callback',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber'),
    ),
  );

  $superior['elementos']['etiqueta_otras'] = array(
    '#type' => 'textfield',
    '#title' => 'Contenedor de otras etiqueta(s) o categoria(s)',
    '#default_value' => isset($variables['elementos']['etiqueta_otras']) ? $variables['elementos']['etiqueta_otras'] : NULL,
    '#description' => "ej: 'div.clear span.txt60', 0  (el numero indica cual elemento si hay varios elementos con el mismo selector)",
    '#maxlength'=> 400,
  );
  $superior['elementos']['probar_etiqueta_otras'] = array(
    '#type' => 'submit',
    '#value' => t('Probar etiqueta otras'),
    '#return_value' => 'probar_elemento_etiqueta_otras',
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'form_vistas_medio_probar_callback',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber'),
    ),
  );

  $superior['elementos']['antetitulo'] = array(
    '#type' => 'textfield',
    '#title' => 'Contenedor del antetitulo',
    '#default_value' => isset($variables['elementos']['antetitulo']) ? $variables['elementos']['antetitulo'] : NULL,
    '#description' => "ej: 'div.pod_izq_gl h2', 0",
    '#maxlength'=> 400,
  );
  $superior['elementos']['probar_antetitulo'] = array(
    '#type' => 'submit',
    '#value' => t('Probar antetitulo'),
    '#return_value' => 'probar_elemento_antetitulo',
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'form_vistas_medio_probar_callback',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber'),
    ),
  );

  $superior['elementos']['sumario'] = array(
    '#type' => 'textfield',
    '#title' => 'Contenedor del sumario',
    '#default_value' => isset($variables['elementos']['sumario']) ? $variables['elementos']['sumario'] : NULL,
    '#description' => "ej: 'div.pod_izq_gl h2', 0",
    '#maxlength'=> 400,
  );
  $superior['elementos']['probar_sumario'] = array(
    '#type' => 'submit',
    '#value' => t('Probar sumario'),
    '#return_value' => 'probar_elemento_sumario',
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'form_vistas_medio_probar_callback',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber'),
    ),
  );

  $superior['elementos']['autor'] = array(
    '#type' => 'textfield',
    '#title' => 'Contenedor del autor',
    '#default_value' => isset($variables['elementos']['autor']) ? $variables['elementos']['autor'] : NULL,
    '#description' => "ej: 'div.MT10 div.color000 span.color0e4d', 0",
    '#maxlength'=> 400,
  );
  $superior['elementos']['probar_autor'] = array(
    '#type' => 'submit',
    '#value' => t('Probar autor'),
    '#return_value' => 'probar_elemento_autor',
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'form_vistas_medio_probar_callback',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber'),
    ),
  );

  $superior['elementos']['cuerpo'] = array(
    '#type' => 'textfield',
    '#title' => 'Contenedor del cuerpo',
    '#default_value' => isset($variables['elementos']['cuerpo']) ? $variables['elementos']['cuerpo'] : NULL,
    '#description' => "ej: 'div.PR10', 0",
    '#maxlength'=> 400,
  );
  $superior['elementos']['probar_cuerpo'] = array(
    '#type' => 'submit',
    '#value' => t('Probar cuerpo'),
    '#return_value' => 'probar_elemento_cuerpo',
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'form_vistas_medio_probar_callback',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber'),
    ),
  );

  $superior['elementos']['imagen'] = array(
    '#type' => 'textfield',
    '#title' => 'Contenedor de la imagen(es)',
    '#default_value' => isset($variables['elementos']['imagen']) ? $variables['elementos']['imagen'] : NULL,
    '#description' => "ej: 'img#imageRotate', 0, src",
    '#maxlength'=> 400,
  );
  $superior['elementos']['probar_imagen'] = array(
    '#type' => 'submit',
    '#value' => t('Probar imagen'),
    '#return_value' => 'probar_elemento_imagen',
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'form_vistas_medio_probar_callback',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber'),
    ),
  );

  $superior['elementos']['leyenda_imagen'] = array(
    '#type' => 'textfield',
    '#title' => 'Contenedor de la leyenda de la imagen(es)',
    '#default_value' => isset($variables['elementos']['leyenda_imagen']) ? $variables['elementos']['leyenda_imagen'] : NULL,
    '#description' => "ej: 'div#leyend0', 0",
    '#maxlength'=> 400,
  );
  $superior['elementos']['probar_leyenda_imagen'] = array(
    '#type' => 'submit',
    '#value' => t('Probar leyenda'),
    '#return_value' => 'probar_elemento_leyenda_imagen',
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'form_vistas_medio_probar_callback',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber'),
    ),
  );

  $superior['elementos']['imagen_otras'] = array(
    '#type' => 'textfield',
    '#title' => 'Contenedor de otras imagen(es)',
    '#default_value' => isset($variables['elementos']['imagen_otras']) ? $variables['elementos']['imagen_otras'] : NULL,
    '#description' => "ej: 'img#imageRotate', 0, src",
    '#maxlength'=> 400,
  );
  $superior['elementos']['probar_imagen_otras'] = array(
    '#type' => 'submit',
    '#value' => t('Probar imagen otras'),
    '#return_value' => 'probar_elemento_imagen_otras',
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'form_vistas_medio_probar_callback',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber'),
    ),
  );

  $superior['elementos']['leyenda_imagen_otras'] = array(
    '#type' => 'textfield',
    '#title' => 'Contenedor de la leyenda de otras imagen(es)',
    '#default_value' => isset($variables['elementos']['leyenda_imagen_otras']) ? $variables['elementos']['leyenda_imagen_otras'] : NULL,
    '#description' => "ej: 'div#leyend0', 0",
    '#maxlength'=> 400,
  );
  $superior['elementos']['probar_leyenda_imagen_otras'] = array(
    '#type' => 'submit',
    '#value' => t('Probar leyenda otras'),
    '#return_value' => 'probar_elemento_leyenda_imagen_otras',
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'form_vistas_medio_probar_callback',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber'),
    ),
  );

  $superior['elementos']['objeto_multimedia'] = array(
    '#type' => 'textfield',
    '#title' => 'Contenedor de objecto multimedia',
    '#default_value' => isset($variables['elementos']['objeto_multimedia']) ? $variables['elementos']['objeto_multimedia'] : NULL,
    '#description' => "ej: 'div#leyend0', 0",
    '#maxlength'=> 400,
  );
  $superior['elementos']['probar_objeto_multimedia'] = array(
    '#type' => 'submit',
    '#value' => t('Probar obj multimedia'),
    '#return_value' => 'probar_elemento_objeto_multimedia',
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'form_vistas_medio_probar_callback',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber'),
    ),
  );

  return $superior;
}

function form_seccion($variables) {
  $superior = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="superior">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );
  $superior['url_prueba_seccion'] = array(
    '#type' => 'textfield',
    '#title' => 'Url de prueba',
    '#maxlength' => 256,
    '#default_value' => isset($variables['url_prueba_seccion']) ? $variables['url_prueba_seccion'] : NULL,
  );
  $superior['probar_url_seccion'] = array(
    '#type' => 'submit',
    '#value' => t('Probar url seccion'),
    '#return_value' => 'probar_url_seccion',
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'form_vistas_medio_probar_callback',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber'),
    ),
  );
  $superior['seccion_grabber'] = array(
    '#type' => 'textfield',
    '#title' => 'Grabber de la seccion',
    '#maxlength' => 256,
    '#default_value' => isset($variables['seccion_grabber']) ? $variables['seccion_grabber'] : NULL,
    '#description' => 'ej: %/.*/%'
  );

  $superior['seccion'] = array(
    '#type' => 'textfield',
    '#title' => 'Contenedor de listado de notas de la seccion',
    '#default_value' => isset($variables['seccion']) ? $variables['seccion'] : NULL,
    '#description' => 'ej: <ul><li>div[@id="cont_cen1"]/div[@class="pod_izq_gl"]</li><li>*[contains(@class, "fb-like") or contains(@class, "social")]</li><li>*[@class="shareToolsBox"]</li><li>div[contains(@class, "field-name-body")]</li><li>div[@id="bodyContent"]</li><li>div[contains(@class, "article")][1]</li><li>div[@class="copytext"]|ul[@id="media-list"] (si son dos contenedores o mas)</li><li>div[contains(@class, "row")/img</li><li>div[@role="article"]/div[contains(@class, "eE")]</li>',
  );
  $superior['probar_seccion'] = array(
    '#type' => 'submit',
    '#value' => t('Probar seccion'),
    '#return_value' => 'probar_seccion',
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'form_vistas_medio_probar_callback',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber'),
    ),
  );


  $superior['elementos_seccion'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => 'contenedor'),
    '#prefix' => '<div id="seccion">',
    '#suffix' => '</div>',
  );

  $superior['elementos_seccion']['titulo'] = array(
    '#type' => 'textfield',
    '#title' => 'Contenedor del titulo',
    '#default_value' => isset($variables['elementos_seccion']['titulo']) ? $variables['elementos_seccion']['titulo'] : NULL,
    '#description' => "ej: 'div.clear span.txt60', 0  (el numero indica cual elemento si hay varios elementos con el mismo selector)"
  );
  $superior['elementos_seccion']['probar_titulo'] = array(
    '#type' => 'submit',
    '#value' => t('Probar titulo'),
    '#return_value' => 'probar_elemento_titulo',
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'form_vistas_medio_probar_callback',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber'),
    ),
  );
  $superior['elementos_seccion']['url'] = array(
    '#type' => 'textfield',
    '#title' => 'Contenedor del url',
    '#default_value' => isset($variables['elementos_seccion']['url']) ? $variables['elementos_seccion']['url'] : NULL,
    '#description' => "ej: 'div.clear span.txt60', 0  (el numero indica cual elemento si hay varios elementos con el mismo selector)"
  );
  $superior['elementos_seccion']['probar_url'] = array(
    '#type' => 'submit',
    '#value' => t('Probar url'),
    '#return_value' => 'probar_elemento_url',
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'form_vistas_medio_probar_callback',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber'),
    ),
  );
  $superior['elementos_seccion']['sumario_seccion'] = array(
    '#type' => 'textfield',
    '#title' => 'Contenedor del sumario',
    '#default_value' => isset($variables['elementos_seccion']['sumario_seccion']) ? $variables['elementos_seccion']['sumario_seccion'] : NULL,
    '#description' => "ej: 'div.mtflow div.teaser',*  (el numero indica cual elemento si hay varios elementos con el mismo selector)"
  );
  $superior['elementos_seccion']['probar_sumario_seccion'] = array(
    '#type' => 'submit',
    '#value' => t('Probar sumario seccion'),
    '#return_value' => 'probar_elemento_sumario_seccion',
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'form_vistas_medio_probar_callback',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber'),
    ),
  );

  $superior['elementos_seccion']['fecha_seccion'] = array(
    '#type' => 'textfield',
    '#title' => 'Contenedor de la fecha',
    '#default_value' => isset($variables['elementos_seccion']['fecha_seccion']) ? $variables['elementos_seccion']['fecha_seccion'] : NULL,
    '#description' => "ej: 'div.mtflow div.teaser',*  (el numero indica cual elemento si hay varios elementos con el mismo selector)"
  );
  $superior['elementos_seccion']['probar_fecha_seccion'] = array(
    '#type' => 'submit',
    '#value' => t('Probar fecha seccion'),
    '#return_value' => 'probar_elemento_fecha_seccion',
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'form_vistas_medio_probar_callback',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber'),
    ),
  );

  $superior['elementos_seccion']['autor_seccion'] = array(
    '#type' => 'textfield',
    '#title' => 'Contenedor del autor',
    '#default_value' => isset($variables['elementos_seccion']['autor_seccion']) ? $variables['elementos_seccion']['autor_seccion'] : NULL,
    '#description' => "ej: 'div.mtflow div.teaser',*  (el numero indica cual elemento si hay varios elementos con el mismo selector)"
  );
  $superior['elementos_seccion']['probar_autor_seccion'] = array(
    '#type' => 'submit',
    '#value' => t('Probar autor seccion'),
    '#return_value' => 'probar_elemento_autor_seccion',
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'form_vistas_medio_probar_callback',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber'),
    ),
  );
  $superior['orden_grabber'] = array(
    '#type' => 'select',
    '#title' => 'Orden del grabber',
    '#options' => ['contenido' => 'contenido', 'seccion' => 'seccion'],
    '#default_value' => isset($variables['orden_grabber']) ? $variables['orden_grabber'] : NULL,
    '#description' => "primer elemento del grabber"
  );


  return $superior;
}


?>
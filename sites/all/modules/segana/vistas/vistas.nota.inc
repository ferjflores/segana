<?php

function vistas_nota_form($form, &$form_state, $nid = NULL, $AnalizarNotas) {
  $form = array();
  if ($form_state['triggering_element']['#return_value'] == 'Eliminar') {
    return $form;
  }


  // tipo medio opciones
  $vocabulary = taxonomy_vocabulary_machine_name_load('tipo_medio');
  $vid = $vocabulary->vid;

  $query_tipo_medio = db_select('taxonomy_term_data', 'ttd')
    ->fields('ttd', array('name', 'tid'))
    ->condition('ttd.vid', $vid);

  $resultado_tipo_medio = $query_tipo_medio->execute();
  $tipo_medio_array = array();

  foreach ($resultado_tipo_medio as $row_tipo_medio) {
    $tid = $row_tipo_medio->tid;
    $tipo_medio_array[$tid] = $row_tipo_medio->name;
  }
  $tid_impreso = array_search('Impreso', $tipo_medio_array);
  $tid_digital = array_search('Digital', $tipo_medio_array);

  //tipo de carga opciones

  $vocabulary = taxonomy_vocabulary_machine_name_load('tipo_carga');
  $vid = $vocabulary->vid;

  $query_tipo_carga = db_select('taxonomy_term_data', 'ttd')
    ->fields('ttd', array('name', 'tid'))
    ->condition('ttd.vid', $vid);

  $resultado_tipo_carga = $query_tipo_carga->execute();
  $tipo_carga_array = array();

  foreach ($resultado_tipo_carga as $row_tipo_carga) {
    $tid = $row_tipo_carga->tid;
    $tipo_carga_array[$tid] = $row_tipo_carga->name;
  }
  $tid_primera_pagina = array_search('Primera Página', $tipo_carga_array);
  $tid_cuerpo = array_search('Cuerpo', $tipo_carga_array);


  // tipo de titulo opciones
  $vocabulary = taxonomy_vocabulary_machine_name_load('tipo_titulo');
  $vid = $vocabulary->vid;

  $query_tipo_titulo = db_select('taxonomy_term_data', 'ttd')
    ->fields('ttd', array('name', 'tid'))
    ->condition('ttd.vid', $vid);

  $resultado_tipo_titulo = $query_tipo_titulo->execute();
  $tipo_titulo_array = array();

  foreach ($resultado_tipo_titulo as $row_tipo_titulo) {
    $tid = $row_tipo_titulo->tid;
    $tipo_titulo_array[$tid] = $row_tipo_titulo->name;
  }
  $tid_compuesto = array_search('Compuesto', $tipo_titulo_array);
  $tid_simple = array_search('Simple', $tipo_titulo_array);

  // usuarios
  $vocabulary = taxonomy_vocabulary_machine_name_load('usuario');
  $vid = $vocabulary->vid;

  $query_usuario = db_select('taxonomy_term_data', 'ttd')
    ->fields('ttd', array('name', 'tid'))
    ->condition('ttd.vid', $vid);

  $resultado_usuario = $query_usuario->execute();
  $usuario_array = array();

  foreach ($resultado_usuario as $row_usuario) {
    $tid = $row_usuario->tid;
    $usuario_array[$tid] = $row_usuario->name;
  }

  //determinar si es una nota nueva o editar(cargar valores)

  if ( ($nid) && empty($form_state['clicked_button']['#value']) ){
    $node = node_load($nid);
    $language = LANGUAGE_NONE;
    $title = $node->title;
    $fecha = date('Y-m-d', $node->field_fecha[$language][0]['value']);
    $tipo_titulo = $node->field_tipo_titulo[$language][0]['tid'];
    $sumario = isset($node->field_sumario[$language][0]['value']) ? $node->field_sumario[$language][0]['value'] : NULL;
    if ($tipo_titulo == $tid_compuesto) {
      $antetitulo = isset($node->field_antetitulo[$language][0]['value']) ? $node->field_antetitulo[$language][0]['value'] : NULL;
      $subtitulo = isset($node->field_subtitulo[$language][0]['value']) ? $node->field_subtitulo[$language][0]['value'] : NULL;
    }
    $body = $node->body[$language][0]['value'];
    $tipo_medio = $node->field_tipo_medio[$language][0]['tid'];
    $medio_term = taxonomy_term_load($node->field_medio[$language][0]['tid']);
    $medio = $medio_term->name;
    $area_term = taxonomy_term_load($node->field_area[$language][0]['tid']);
    $area = $area_term->name;
    $usuario = array();
    foreach($node->field_usuario[$language] as $key => $value){
      $usuario[] = $value['tid'];
    }
    $autor = $node->field_autor[$language][0]['value'];
    $tipo_informacion = $node->field_tipo_informacion[$language][0]['tid'];
    $tendencia_nota_term = isset($node->field_tendencia_nota[$language][0]['tid']) ? taxonomy_term_load($node->field_tendencia_nota[$language][0]['tid']) : NULL;
    $tendencia_nota = isset($tendencia_nota_term) ? $tendencia_nota_term->name : NULL;


    // opciones para medios digitales, aunque los medio impresos puede tener la misma nota en digital
    $enlace = isset($node->field_enlace[$language][0]['value']) ? $node->field_enlace[$language][0]['value'] : NULL;


    //opciones para medios impresos
    if ($tipo_medio == $tid_impreso) {
      $tipo_carga = $node->field_tipo_carga[$language][0]['tid'];

      $cuerpo_seccion = isset($node->field_cuerpo_seccion[$language][0]['value']) ? $node->field_cuerpo_seccion[$language][0]['value'] : NULL;
      $pagina = isset($node->field_pagina[$language][0]['value']) ? $node->field_pagina[$language][0]['value'] : NULL;
      $cuadrante = isset($node->field_cuadrante[$language][0]['value']) ? $node->field_cuadrante[$language][0]['value'] : NULL;
      $horizontal_cm = isset($node->field_horizontal_cm[$language][0]['value']) ? $node->field_horizontal_cm[$language][0]['value'] : NULL;
      $vertical_cm = isset($node->field_vertical_cm[$language][0]['value']) ? $node->field_vertical_cm[$language][0]['value'] : NULL;
      $centimetros = isset($node->field_centimetros[$language][0]['value']) ? $node->field_centimetros[$language][0]['value'] : NULL;

      if ($tipo_carga == $tid_primera_pagina) {
        $titulo_primera_pagina = $node->field_titulo_primera_pagina[$language][0]['value'];
        $tipo_titulo_primera_pagina = $node->field_tipo_titulo_primera_pagina[$language][0]['tid'];
        if ($tipo_titulo_primera_pagina == $tid_compuesto) {
          $antetitulo_primera_pagina = isset($node->field_antetitulo_primera_pagina[$language][0]['value']) ? $node->field_antetitulo_primera_pagina[$language][0]['value'] : NULL;
          $subtitulo_primera_pagina = isset($node->field_subtitulo_primera_pagina[$language][0]['value']) ? $node->field_subtitulo_primera_pagina[$language][0]['value'] : NULL;
          $sumario_primera_pagina = isset($node->field_sumario_primera_pagina[$language][0]['value']) ? $node->field_sumario_primera_pagina[$language][0]['value'] : NULL;
        }
        $horizontal_primera_pagina = isset($node->field_horizontal_primera_pagina[$language][0]['value']) ? $node->field_horizontal_primera_pagina[$language][0]['value'] : NULL;
        $vertical_primera_pagina = isset($node->field_vertical_primera_pagina[$language][0]['value']) ? $node->field_vertical_primera_pagina[$language][0]['value'] : NULL;
        $centimetros_primera_pagina = isset($node->field_centimetros_primera_pagina[$language][0]['value']) ? $node->field_centimetros_primera_pagina[$language][0]['value'] : NULL;
      }

    }
  }
  else {
    $fecha = date('Y-m-d');
  }

  //presentar mensajes de la nota anterior despues de guardada
  if ($AnalizarNotas && !isset($form_state['triggering_element']['#return_value'])) {
    $messages = theme('status_messages');
    $form["wrapper"] = array("#markup" => '<div id="messages">' . $messages . '</div>');
  }

  $form['contenedor'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => 'contenedor'),
    '#prefix' => '<div id="contenedor">',
    '#suffix' => '</div>',
    '#value' => '<A NAME="top"> </A> ',
  );

  $form['contenedor']['nid'] = array(
    '#type' => 'token',
    '#value' => isset($form_state['values']['nid']) ? $form_state['values']['nid'] : (isset($nid) ? $nid : NULL),
  );

  $form['contenedor']['superior'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="superior">',
    '#suffix' => '</div>',
  );

  $form['contenedor']['superior']['title'] = array(
    '#type' => 'textfield',
    '#title' => 'Título',
    '#attributes' => array('autocomplete' =>'off'),
    '#default_value' => !empty($form_state['values']['title']) ? $form_state['values']['title'] : (isset($title) ? $title : NULL),
    '#size' => 50,
    '#maxlength' => 256,
    '#required' => TRUE,
  );
  $form['contenedor']['superior']['fecha'] = array(
    '#type' => 'date_popup',
    '#title' => t('Fecha'),
    '#attributes' => array('autocomplete' =>'off'),
    '#date_format' => 'd-m-Y',
    '#date_year_range' => '-6:+0',
    '#size' => 6,
    '#default_value' => !empty($form_state['values']['fecha']) ? $form_state['values']['fecha'] : (isset($fecha) ? $fecha : NULL),
    '#required' => TRUE,
  );


  $form['contenedor']['titular'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="titular">',
    '#suffix' => '</div>',
  );

  $form['contenedor']['titular']['tipo_titulo'] = array(
    '#type' => 'select',
    '#title' => 'Tipo de titulo',
    '#default_value' => !empty($form_state['values']['tipo_titulo']) ? $form_state['values']['tipo_titulo'] : (isset($tipo_titulo) ? $tipo_titulo : array($tid_simple)),
    '#options' => $tipo_titulo_array,
    '#ajax' => array(
      'callback' => 'form_vistas_nota_tipo_titulo_callback',
      'wrapper' => 'titular',
      'progress' => array('type' => 'throbber'),
      'effect' => 'fade',
      'method' => 'replace',
    ),
  );


  $tipo_titulo_opcion = isset($form_state['values']['tipo_titulo']) ? $form_state['values']['tipo_titulo'] : (isset($tipo_titulo) ? $tipo_titulo: $tid_simple);

  if ($tipo_titulo_opcion == $tid_compuesto) {
    $form['contenedor']['titular']['antetitulo'] = array(
      '#type' => 'textfield',
      '#title' => 'Antetítulo',
      '#attributes' => array('autocomplete' =>'off'),
      '#default_value' => !empty($form_state['values']['antetitulo']) ? $form_state['values']['antetitulo'] : (isset($antetitulo) ? $antetitulo : NULL),
      '#size' => 50,
      '#maxlength' => 256,
      //'#dependency' => array('edit-tipo-titulo' => array($tid_compuesto)),
    );
    $form['contenedor']['titular']['subtitulo'] = array(
      '#type' => 'textfield',
      '#title' => 'Subtítulo',
      '#attributes' => array('autocomplete' =>'off'),
      '#default_value' => !empty($form_state['values']['subtitulo']) ? $form_state['values']['subtitulo'] : (isset($subtitulo) ? $subtitulo : NULL),
      '#size' => 50,
      '#maxlength' => 256,
      //'#dependency' => array('edit-tipo-titulo' => array($tid_compuesto)),
    );
  }
  $form['contenedor']['medio'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="medio">',
    '#suffix' => '</div>',
  );


  $form['contenedor']['medio']['derecha'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="medio-derecha">',
    '#suffix' => '</div>',
  );

  $form['contenedor']['medio']['derecha']['sumario'] = array(
    '#type' => 'textarea',
    '#title' => 'Sumario',
    '#attributes' => array('autocomplete' =>'off'),
    '#default_value' => !empty($form_state['values']['sumario']) ? $form_state['values']['sumario'] : (isset($sumario) ? $sumario : NULL),
    '#rows' => 5,
    '#cols' => 3,
    '#maxlength' => 56000,
    '#resizable' => FALSE,
    //'#dependency' => array('edit-tipo-titulo' => array($tid_compuesto)),
  );

  $form['contenedor']['medio']['derecha']['body'] = array(
    '#type' => 'textarea',
    '#title' => t('Cuerpo de texto'),
    '#attributes' => array('autocomplete' =>'off'),
    '#default_value' => isset($form_state['values']['body']) ? $form_state['values']['body'] : (isset($body) ? $body : NULL),
    '#rows' => 10,
    '#cols' => 17,
    '#maxlength' => 56000,
    '#resizable' => FALSE,
  );
  $form['contenedor']['medio']['izquierda'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="medio-izquierda">',
    '#suffix' => '</div>',
  );




  $form['contenedor']['medio']['izquierda']['tipo_medio'] = array(
    '#type' => 'select',
    '#title' => 'Tipo de medio',
    '#attributes' => array('autocomplete' =>'off'),
    '#default_value' => !empty($form_state['values']['tipo_medio']) ? $form_state['values']['tipo_medio'] : (isset($tipo_medio) ? $tipo_medio : NULL),
    '#options' => $tipo_medio_array,
    '#ajax' => array(
      'callback' => 'form_vistas_nota_tipo_medio_callback',
      'wrapper' => 'medio',
      'progress' => array('type' => 'throbber'),
      'effect' => 'fade',
      'method' => 'replace',
    ),
  );

  $form['contenedor']['medio']['izquierda']['medio'] = array(
    '#type' => 'textfield',
    '#title' => 'Medio',
    '#attributes' => array('autocomplete' =>'off'),
    '#default_value' => !empty($form_state['values']['medio']) ? $form_state['values']['medio'] : (isset($medio) ? $medio : NULL),
    '#size' => 13,
    '#maxlength' => 128,
    '#autocomplete_path' => 'notas/autocomplete_medio',
    '#required' => TRUE,
  );
  $tipo_medio_opcion = isset($form_state['values']['tipo_medio']) ? $form_state['values']['tipo_medio'] : (isset($tipo_medio) ? $tipo_medio: $tid_digital);


  $form['contenedor']['medio']['izquierda']['area'] = array(
    '#type' => 'textfield',
    '#title' => 'Área',
    '#attributes' => array('autocomplete' =>'off'),
    '#default_value' => !empty($form_state['values']['area']) ? $form_state['values']['area'] : (isset($area) ? $area : NULL),
    '#size' => 13,
    '#maxlength' => 128,
    '#autocomplete_path' => 'busqueda/autocomplete_area',
    '#required' => TRUE,
  );
  $form['contenedor']['medio']['izquierda']['usuario'] = array(
    '#type' => 'select',
    '#title' => 'Usuario',
    //'#attributes' => array('autocomplete' =>'off'),
    '#default_value' => !empty($form_state['values']['usuario']) ? $form_state['values']['usuario'] : (isset($usuario) ? $usuario : NULL),
    '#size' => 13,
    '#maxlength' => 128,
    '#options' => $usuario_array,
    '#chosen' => TRUE,
    '#multiple' => TRUE,
  );

  $form['contenedor']['medio']['izquierda']['autor'] = array(
    '#type' => 'textfield',
    '#title' => 'Autor',
    '#attributes' => array('autocomplete' =>'off'),
    '#default_value' => !empty($form_state['values']['autor']) ? $form_state['values']['autor'] : (isset($autor) ? $autor : NULL),
    '#size' => 13,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $vocabulary = taxonomy_vocabulary_machine_name_load('tipo_informacion');
  $vid = $vocabulary->vid;

  $query_tipo_informacion = db_select('taxonomy_term_data', 'ttd')
    ->fields('ttd', array('name', 'tid'))
    ->condition('ttd.vid', $vid);

  $resultado_tipo_informacion = $query_tipo_informacion->execute();
  $tipo_informacion_array = array();

  foreach ($resultado_tipo_informacion as $row_tipo_informacion) {
    $tid = $row_tipo_informacion->tid;
    $tipo_informacion_array[$tid] = $row_tipo_informacion->name;
  }

  $form['contenedor']['medio']['izquierda']['tipo_informacion'] = array(
    '#type' => 'select',
    '#title' => 'Tipo de información',
    '#attributes' => array('autocomplete' =>'off'),
    '#default_value' => !empty($form_state['values']['tipo_informacion']) ? $form_state['values']['tipo_informacion'] : (isset($tipo_informacion) ? $tipo_informacion : 8),
    '#options' => $tipo_informacion_array,
  );


  if ($tipo_medio_opcion == $tid_impreso){

    $form['contenedor']['medio']['impreso'] = array(
      '#type' => 'fieldset',
      '#prefix' => '<div id="medio-impreso">',
      '#suffix' => '</div>',
    );

    $form['contenedor']['medio']['impreso']['tipo_carga'] = array(
      '#type' => 'select',
      '#title' => 'Tipo de carga',
      '#default_value' => !empty($form_state['values']['tipo_carga']) ? $form_state['values']['tipo_carga'] : (isset($tipo_carga) ? $tipo_carga : 3),
      '#options' => $tipo_carga_array,
      '#ajax' => array(
        'callback' => 'form_vistas_nota_tipo_carga_callback',
        'wrapper' => 'primera_pagina',
        'progress' => array('type' => 'throbber'),
        'effect' => 'fade',
        'method' => 'replace',
      ),
    );
    $tipo_carga_opcion = isset($form_state['values']['tipo_carga']) ? $form_state['values']['tipo_carga'] : (isset($tipo_carga) ? $tipo_carga: $tid_primera_pagina);

    $form['contenedor']['medio']['impreso']['primera_pagina'] = array(
      '#type' => 'fieldset',
      '#prefix' => '<div id="primera_pagina">',
      '#suffix' => '</div>',
    );

    if ($tipo_carga_opcion == $tid_primera_pagina){

      $form['contenedor']['medio']['impreso']['primera_pagina']['titulo_primera_pagina'] = array(
        '#type' => 'textfield',
        '#title' => 'Título primera página',
        '#attributes' => array('autocomplete' =>'off'),
        '#default_value' => !empty($form_state['values']['titulo_primera_pagina']) ? $form_state['values']['titulo_primera_pagina'] : (isset($titulo_primera_pagina) ? $titulo_primera_pagina : NULL),
        '#size' => 50,
        '#maxlength' => 256,
        '#required' => TRUE,
      );



      $form['contenedor']['medio']['impreso']['primera_pagina']['tipo_titulo_primera_pagina'] = array(
        '#type' => 'select',
        '#title' => 'Tipo de titulo primera página',
        '#default_value' => !empty($form_state['values']['tipo_titulo_primera_pagina']) ? $form_state['values']['tipo_titulo_primera_pagina'] : (isset($tipo_titulo_primera_pagina) ? $tipo_titulo_primera_pagina : array($tid_simple)),
        '#options' => $tipo_titulo_array,
        '#ajax' => array(
          'callback' => 'form_vistas_nota_tipo_titulo_primera_pagina_callback',
          'wrapper' => 'titular_primera_pagina',
          'progress' => array('type' => 'throbber'),
          'effect' => 'fade',
          'method' => 'replace',
        ),
      );

      $form['contenedor']['medio']['impreso']['primera_pagina']['titular_primera_pagina'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div id="titular_primera_pagina">',
        '#suffix' => '</div>',
      );

      $tipo_titulo_primera_pagina_opcion = isset($form_state['values']['tipo_titulo_primera_pagina']) ? $form_state['values']['tipo_titulo_primera_pagina'] : (isset($tipo_titulo_primera_pagina) ? $tipo_titulo_primera_pagina: $tid_simple);

      if ($tipo_titulo_primera_pagina_opcion ==  $tid_compuesto) {

        $form['contenedor']['medio']['impreso']['primera_pagina']['titular_primera_pagina']['antetitulo_primera_pagina'] = array(
          '#type' => 'textfield',
          '#title' => 'Antetítulo primera página',
          '#attributes' => array('autocomplete' =>'off'),
          '#default_value' => !empty($form_state['values']['antetitulo_primera_pagina']) ? $form_state['values']['antetitulo_primera_pagina'] : (isset($antetitulo_primera_pagina) ? $antetitulo_primera_pagina : NULL),
          '#size' => 50,
          '#maxlength' => 256,
        );
        $form['contenedor']['medio']['impreso']['primera_pagina']['titular_primera_pagina']['subtitulo_primera_pagina'] = array(
          '#type' => 'textfield',
          '#title' => 'Subtítulo primera página',
          '#attributes' => array('autocomplete' =>'off'),
          '#default_value' => !empty($form_state['values']['subtitulo_primera_pagina']) ? $form_state['values']['subtitulo_primera_pagina'] : (isset($subtitulo_primera_pagina) ? $subtitulo_primera_pagina : NULL),
          '#size' => 50,
          '#maxlength' => 256,
        );
        $form['contenedor']['medio']['impreso']['primera_pagina']['titular_primera_pagina']['sumario_primera_pagina'] = array(
          '#type' => 'textarea',
          '#title' => 'Sumario primera página',
          '#attributes' => array('autocomplete' =>'off'),
          '#default_value' => !empty($form_state['values']['titular_primera_pagina']['sumario_primera_pagina']) ? $form_state['values']['sumario_primera_pagina'] : (isset($sumario_primera_pagina) ? $sumario_primera_pagina : NULL),
          '#rows' => 5,
          '#cols' => 3,
          '#maxlength' => 56000,
          '#resizable' => FALSE,
        );
      }

      $form['contenedor']['medio']['impreso']['primera_pagina']['horizontal_primera_pagina'] = array(
        '#type' => 'textfield',
        '#title' => 'Centimetros horizontal primera página',
        '#attributes' => array('autocomplete' =>'off'),
        '#default_value' => !empty($form_state['values']['horizontal_primera_pagina']) ? $form_state['values']['horizontal_primera_pagina'] : (isset($horizontal_primera_pagina) ? $horizontal_primera_pagina : NULL),
        '#size' => 12,
        '#maxlength' => 128,
        '#ajax' => array(
          'callback' => 'form_vistas_nota_centimetros_primera_pagina_callback',
          'wrapper' => 'centimetros_primera_pagina',
          'progress' => 'none',
          'effect' => 'fade',
          'method' => 'replace',
          'even' => 'input', 'change', 'keyup', 'paste', 'mouseup',
        ),
      );

      $form['contenedor']['medio']['impreso']['primera_pagina']['vertical_primera_pagina'] = array(
        '#type' => 'textfield',
        '#title' => 'Centimetros vertical primera pagina',
        '#attributes' => array('autocomplete' =>'off'),
        '#default_value' => !empty($form_state['values']['vertical_primera_pagina']) ? $form_state['values']['vertical_primera_pagina'] : (isset($vertical_primera_pagina) ? $vertical_primera_pagina : NULL),
        '#size' => 12,
        '#maxlength' => 128,
        '#ajax' => array(
          'callback' => 'form_vistas_nota_centimetros_primera_pagina_callback',
          'wrapper' => 'centimetros_primera_pagina',
          'progress' => 'none',
          'effect' => 'fade',
          'method' => 'replace',
          'even' => 'input', 'change', 'keyup', 'paste', 'mouseup',
        ),
      );

      $form['contenedor']['medio']['impreso']['primera_pagina']['centimetros_primera_pagina'] = array(
        '#type' => 'textfield',
        '#title' => 'Centimetros² primera_pagina'	,
        '#attributes' => array('autocomplete' =>'off'),
        '#default_value' => !empty($form_state['values']['centimetros_primera_pagina']) ? $form_state['values']['centimetros_primera_pagina'] : (isset($centimetros_primera_pagina) ? $centimetros_primera_pagina : NULL),
        '#size' => 12,
        '#maxlength' => 128,
        '#disabled' => TRUE,
        '#prefix' => '<div id="centimetros_primera_pagina">',
        '#suffix' => '</div>',
      );
    }
  }
  $form['contenedor']['medio']['izquierda']['enlace'] = array(
    '#type' => 'textfield',
    '#title' => 'Enlace',
    '#attributes' => array('autocomplete' =>'off'),
    '#default_value' => !empty($form_state['values']['enlace']) ? $form_state['values']['enlace'] : (isset($enlace) ? $enlace : NULL),
    '#size' => 13,
    '#maxlength' => 256,
  );


  if (($tipo_medio_opcion == $tid_impreso) && ($tipo_carga_opcion == $tid_cuerpo)) {

    $form['contenedor']['medio']['impreso']['cuerpo_seccion'] = array(
      '#type' => 'textfield',
      '#title' => 'Cuerpo y/o Sección',
      '#attributes' => array('autocomplete' =>'off'),
      '#default_value' => !empty($form_state['values']['cuerpo_seccion']) ? $form_state['values']['cuerpo_seccion'] : (isset($cuerpo_seccion) ? $cuerpo_seccion : NULL),
      '#size' => 12,
      '#maxlength' => 128,
    );

    $form['contenedor']['medio']['impreso']['pagina'] = array(
      '#type' => 'textfield',
      '#title' => 'Página',
      '#attributes' => array('autocomplete' =>'off'),
      '#default_value' => !empty($form_state['values']['pagina']) ? $form_state['values']['pagina'] : (isset($pagina) ? $pagina : NULL),
      '#size' => 12,
      '#maxlength' => 128,
    );


    $vocabulary = taxonomy_vocabulary_machine_name_load('cuadrante');
    $vid = $vocabulary->vid;

    $query_cuadrante = db_select('taxonomy_term_data', 'ttd')
      ->fields('ttd', array('name', 'tid'))
      ->condition('ttd.vid', $vid);

    $resultado_cuadrante = $query_cuadrante->execute();
    $cuadrante_array = array();

    foreach ($resultado_cuadrante as $row_cuadrante) {
      $tid = $row_cuadrante->tid;
      $cuadrante_array[$tid] = $row_cuadrante->name;
    }

    $form['contenedor']['medio']['impreso']['cuadrante'] = array(
      '#type' => 'select',
      '#title' => 'Cuadrante',
      '#attributes' => array('autocomplete' =>'off'),
      '#default_value' => !empty($form_state['values']['cuadrante']) ? $form_state['values']['cuadrante'] : (isset($cuadrante) ? $cuadrante : NULL),
      '#options' => $cuadrante_array,
    );


    $form['contenedor']['medio']['impreso']['horizontal_cm'] = array(
      '#type' => 'textfield',
      '#title' => 'Centimetros horizontal',
      '#attributes' => array('autocomplete' =>'off'),
      '#default_value' => !empty($form_state['values']['horizontal_cm']) ? $form_state['values']['horizontal_cm'] : (isset($horizontal_cm) ? $horizontal_cm : NULL),
      '#size' => 12,
      '#maxlength' => 128,
      '#ajax' => array(
        'callback' => 'form_vistas_nota_centimetros_callback',
        'wrapper' => 'centimetros',
        'progress' => 'none',
        'effect' => 'fade',
        'method' => 'replace',
        'even' => 'input', 'change', 'keyup', 'paste', 'mouseup',
      ),
    );

    $form['contenedor']['medio']['impreso']['vertical_cm'] = array(
      '#type' => 'textfield',
      '#title' => 'Centimetros vertical',
      '#attributes' => array('autocomplete' =>'off'),
      '#default_value' => !empty($form_state['values']['vertical_cm']) ? $form_state['values']['vertical_cm'] : (isset($vertical_cm) ? $vertical_cm : NULL),
      '#size' => 12,
      '#maxlength' => 128,
      '#ajax' => array(
        'callback' => 'form_vistas_nota_centimetros_callback',
        'wrapper' => 'centimetros',
        'progress' => 'none',
        'effect' => 'fade',
        'method' => 'replace',
        'even' => 'input', 'change', 'keyup', 'paste', 'mouseup',
      ),
    );

    $form['contenedor']['medio']['impreso']['centimetros'] = array(
      '#type' => 'textfield',
      '#title' => 'Centimetros²',
      '#attributes' => array('autocomplete' =>'off'),
      '#prefix' => '<div id="centimetros">',
      '#suffix' => '</div>',
      '#default_value' => !empty($form_state['values']['centimetros']) ? $form_state['values']['centimetros'] : (isset($centimetros) ? $centimetros : NULL),
      '#size' => 12,
      '#maxlength' => 128,
      '#disabled' => TRUE,
    );
  }

  $form['contenedor']['inferior'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="inferior">',
    '#suffix' => '</div>',
    '#title' => 'ELEMENTOS DISCURSIVOS',
  );
  if (isset($nid) && !isset($form_state['triggering_element']['#return_value'])){
    $filas_discursivo = count($node->field_tema[$language]);


    for ($i = 0; $i < $filas_discursivo; $i++) {
      $tema_term = isset($node->field_tema[$language][$i]['tid']) ? taxonomy_term_load($node->field_tema[$language][$i]['tid']) : NULL;
      $tema[$i] = $tema_term->name;
      $matriz_term = isset($node->field_matriz[$language][$i]['tid']) ? taxonomy_term_load($node->field_matriz[$language][$i]['tid']) : NULL;
      $matriz[$i] = $matriz_term->name;
      $cuerpo_argumentativo[$i] = isset($node->field_cuerpo_argumentativo[$language][$i]['value']) ? $node->field_cuerpo_argumentativo[$language][$i]['value'] : NULL;
      $tendencia[$i] = isset($node->field_tendencia[$language][$i]['tid']) ? $node->field_tendencia[$language][$i]['tid'] : NULL;
      $actor_term = isset($node->field_actor[$language][$i]['tid']) ? taxonomy_term_load($node->field_actor[$language][$i]['tid']) : NULL;
      $actor[$i] = $actor_term->name;
    }


    // en caso de no poseer se establece $filas_discursivo = 1 para tener una fila en blanco
    $filas_discursivo = empty($filas_discursivo) ? 1 : $filas_discursivo;


  }
  else {
    $filas_discursivo = (isset($form_state['values']['filas_discursivo'])) ? $form_state['values']['filas_discursivo'] : 1;
  }
  $form['contenedor']['filas_discursivo'] = array(
    '#type' => 'token',
    '#value' => $filas_discursivo,
  );


  $vocabulary = taxonomy_vocabulary_machine_name_load('tendencia');
  $vid = $vocabulary->vid;

  $query_tendencia = db_select('taxonomy_term_data', 'ttd')
    ->fields('ttd', array('name', 'tid'))
    ->condition('ttd.vid', $vid);

  $resultado_tendencia = $query_tendencia->execute();
  $tendencia_array = array();

  foreach ($resultado_tendencia as $row_tendencia) {
    $tid = $row_tendencia->tid;
    $tendencia_array[$tid] = $row_tendencia->name;
  }
  $tendencia_array[0] = 'No definido';

  $tendencia_nota = (!empty($form_state['values']['tendencia_nota'])) ? $tendencia_array[$form_state['values']['tendencia_nota']] : (isset($tendencia_nota) ? $tendencia_nota : NULL);
  $form['contenedor']['inferior']['tendencia_nota'] = array(
    '#type' => 'token',
    '#value' =>  $tendencia_nota,
  );

  $form['contenedor']['inferior']['tendencia'] = array(
    '#type' => 'textfield',
    '#title' => 'Tendencia de la nota: ',
    '#value' => ($tendencia_nota == 'Positivo' ? "Positivo" : ($tendencia_nota == 'Negativo' ? "Negativo" : NULL) ),
    '#prefix' => '<div id="tendencia_nota">',
    '#suffix' => '</div>',
    '#attributes' => array('readonly' => '', 'class' => ($tendencia_nota == 'Positivo' ? array('favor') : ($tendencia_nota == 'Negativo' ? array('contra') : NULL) ) ),
    '#size' => '4',
  );



  //encabezado elementos discursivos
  $tema_div = theme('html_tag', array('element' => array('#tag' => 'div','#attributes' => array('id' => 'tema'),'#value' => 'Tema')));
  $matriz_div = theme('html_tag', array('element' => array('#tag' => 'div','#attributes' => array('id' => 'matriz'),'#value' => 'Matriz')));
  $tendencia_div = theme('html_tag', array('element' => array('#tag' => 'div','#attributes' => array('id' => 'tendencia'),'#value' => 'Tendencia')));
  $cuerpo_argumentativo_div = theme('html_tag', array('element' => array('#tag' => 'div','#attributes' => array('id' => 'cuerpo_argumentativo'),'#value' => 'Cuerpo Argumentativo')));
  $actor_div = theme('html_tag', array('element' => array('#tag' => 'div','#attributes' => array('id' => 'actor'),'#value' => 'Actor')));
  $encabezado = theme('html_tag', array('element' => array('#tag' => 'div', '#attributes' => array('id' => 'encabezado'), '#value' => $tema_div . $matriz_div . $tendencia_div . $cuerpo_argumentativo_div . $actor_div)));

  $form['contenedor']['inferior']['encabezado'] = array(
    '#markup' => $encabezado
  );






  $form['contenedor']['inferior']['elementos_discursivos'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="elementos_discursivos">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );

  for ($i = 0; $i < $filas_discursivo; $i++) {
    $form['contenedor']['inferior']['elementos_discursivos'][$i] = array(
      '#type' => 'fieldset',
      '#prefix' => '<div id="fila_discursivo_'. $i .'" class="fila_discursivo">',
      '#suffix' => '</div>',
    );
    $form['contenedor']['inferior']['elementos_discursivos'][$i]["tema"] = array(
      '#type' => 'textfield',
      '#attributes' => array('autocomplete' =>'off'),
      '#default_value' => !empty($form_state['values']["elementos_discursivos"][$i]['tema']) ? $form_state['values']["elementos_discursivos"][$i]['tema'] : (isset($tema[$i]) ? $tema[$i] : NULL),
      '#size' => 12,
      '#maxlength' => 128,
      '#autocomplete_path' => 'notas/autocomplete/tema/0/0',
    );
    $form['contenedor']['inferior']['elementos_discursivos'][$i]["matriz"] = array(
      '#type' => 'textfield',
      '#attributes' => array('autocomplete' =>'off'),
      '#default_value' => !empty($form_state['values']["elementos_discursivos"][$i]["matriz"]) ? $form_state['values']["elementos_discursivos"][$i]["matriz"] : $matriz[$i],
      '#size' => 12,
      '#maxlength' => 128,
      '#autocomplete_path' => "notas/autocomplete/matriz/0/0",
    );
    $form['contenedor']['inferior']['elementos_discursivos'][$i]["tendencia"] = array(
      '#type' => 'select',
      '#attributes' => array('autocomplete' =>'off'),
      '#default_value' => !empty($form_state['values']["elementos_discursivos"][$i]["tendencia"]) ? $form_state['values']["elementos_discursivos"][$i]["tendencia"] : (isset($tendencia[$i]) ? $tendencia[$i] : 0),
      '#options' => $tendencia_array,
    );
    $form['contenedor']['inferior']['elementos_discursivos'][$i]["cuerpo_argumentativo"] = array(
      '#type' => 'textarea',
      '#attributes' => array('autocomplete' =>'off'),
      '#default_value' => !empty($form_state['values']['elementos_discursivos'][$i]["cuerpo_argumentativo"]) ? $form_state['values']['elementos_discursivos'][$i]["cuerpo_argumentativo"] : $cuerpo_argumentativo[$i],
      '#size' => 12,
      '#maxlength' => 128,
      '#rows' => 1,
      '#cols' => 17,
      '#maxlength' => 1000,
      '#resizable' => FALSE,
    );
    $form['contenedor']['inferior']['elementos_discursivos'][$i]["actor"] = array(
      '#type' => 'textfield',
      '#attributes' => array('autocomplete' =>'off'),
      '#default_value' => !empty($form_state['values']["elementos_discursivos"][$i]["actor"]) ? $form_state['values']["elementos_discursivos"][$i]["actor"] : $actor[$i],
      '#size' => 12,
      '#maxlength' => 128,
      '#autocomplete_path' => "notas/autocomplete/actor/0/0",
    );

    $form['contenedor']['inferior']['elementos_discursivos'][$i]["borrar_fila_discursivo"] = array(
      '#type' => 'image_button',
      '#description' => 'Borrar fila discursiva',
      '#src' => 'sites/all/themes/segana/images/delete.png',
      '#return_value' => "borrar_fila_discursivo_$i",
      '#submit' => array('vistas_nota_form_submit'),
      '#prefix' => '<div id="borrar_fila_discursivo">',
      '#suffix' => '</div>',
      '#ajax' => array(
        'event' => 'click',
        'callback' => 'form_vistas_notas_fila_discursivo_callback',
        'wrapper' => 'elementos_discursivos',
        'method' => 'html',
        'effect' => 'fade',
        'progress' => array('type' => 'none'),
        //'prevent' => 'submit click mousedown',
      ),
      '#disabled' => ($filas_discursivo > 1) ? FALSE : TRUE,
      '#limit_validation_errors' => array(array('filas_discursivo'), array('elementos_discursivos')),
    );

  }



  $form['contenedor']['inferior']['botones'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="botones">',
    '#suffix' => '</div>',
  );

  $form['contenedor']['inferior']['botones']['agregar_fila_discursivo'] = array(
    '#type' => 'image_button',
    '#description' => 'Agregar otro tema',
    '#src' => 'sites/all/themes/segana/images/add.png',
    '#return_value' => 'agregar_fila_discursivo',
    '#submit' => array('vistas_nota_form_submit'),
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'form_vistas_notas_fila_discursivo_callback',
      'wrapper' => 'elementos_discursivos',
      'method' => 'html',
      'effect' => 'fade',
      'progress' => array('type' => 'none'),
      //'prevent' => 'submit click mousedown',
    ),
    '#limit_validation_errors' => array(array('filas_discursivo'), array('elementos_discursivos')),
  );

  $form['contenedor']['inferior']['botones']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#return_value' => t('Reset'),
//		'#attributes' => array('onclick' => 'this.form.reset(); return false;'),
  );
  $form['#prefix'] = '<div id="formwrapper">';
  $form['#suffix'] = '</div>';

  if ($nid || isset($form_state['values']['nid'])) {
    $form['contenedor']['inferior']['botones']['eliminar'] = array(
      '#type' => 'submit',
      '#value' => t('Eliminar'),
      '#return_value' => t('Eliminar'),
      '#submit' => array('vistas_nota_form_submit'),
      '#ajax' => array(
        'event' => 'click',
        'callback' => 'form_vistas_nota_eliminar_callback',
        'wrapper' => 'formwrapper',
        'method' => 'replace',
        'effect' => 'fade',
        'progress' => array('type' => 'none'),
      ),
      '#limit_validation_errors' => array(array('nid'), array('eliminar')),
    );
  }

  //$form_id = str_replace('_', '-', $form_state['build_info']['form_id']);
  $form['contenedor']['inferior']['botones']['submit'] = array(
    '#type' => 'submit',
    '#value' => ($AnalizarNotas) ? t('Guardar y Siguiente') : t('Guardar'),
    '#return_value' => t('Guardar'),
    '#submit' => array('vistas_nota_form_submit'),
    '#prefix' => '<div id="guardar">',
    '#suffix' => '</div>',
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'forma_vista_notas_submit_callback',
      'wrapper' => 'formwrapper',
      'method' => 'replace',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber'),
      //'prevent' => 'submit click mousedown',
    ),
  );
  //$form["wrapper2"] = array("#markup" => "<div class='inline-messages'></div>");
  return $form;
}


function vistas_nota_form_submit($form, &$form_state) {

  $pressed_button = check_plain($form_state['triggering_element']['#return_value']);
  switch ($pressed_button) {
    case 'Reiniciar':
      $form_state['rebuild'] = TRUE;
      unset($form_state['values']);
      break;
    case 'agregar_fila_discursivo':
      $form_state['values']['filas_discursivo']++;
      $form_state['rebuild'] = TRUE;
      break;
    case (strpos($pressed_button, "borrar_")):
      $variables = explode('_', $pressed_button);
      $fila_numero = $variables[3];
      $form_state['values'] = $form_state['values'];
      $form_state['values']['filas_discursivo']--;

      unset($form_state['input']['elementos_discursivos'][$fila_numero]);
      $form_state['input']['elementos_discursivos'] = array_values($form_state['input']['elementos_discursivos']);
      $form_state['rebuild'] = TRUE;
      break;

    case 'Eliminar':
      $entity_id = $form_state['values']['nid'];

      if (entity_exists($entity_id, 'node')) {
        node_delete($entity_id);
        if (!entity_exists($entity_id, 'node')) {
          drupal_set_message('Se ha eliminado ' . 'esta nota con id ' . $entity_id);
          unset($form_state['values']['nid']);
        }
      }
      else {
        form_set_error('', 'esta nota con id ' . $entity_id . ' no existe actualmente');
      }
      $form_state['rebuild'] = TRUE;
      break;

    case 'Guardar':

      //chequear filas discursivo
      for ($i=0; $i < $form_state['values']['filas_discursivo']; $i++) {
        if (empty($form_state['values']['elementos_discursivos'][$i]['tema'])) {
          unset($form_state['input']['elementos_discursivos'][$i]);
          $form_state['input']['elementos_discursivos'] = array_values($form_state['input']['elementos_discursivos']);

          unset($form_state['values']['elementos_discursivos'][$i]);
          $form_state['values']['elementos_discursivos'] = array_values($form_state['values']['elementos_discursivos']);
          $form_state['values']['filas_discursivo']--;
        }
      }

      // tendencia de la nota

      $tendencia_nota = 0;
      for ($i=0; $i < $form_state['values']['filas_discursivo']; $i++) {
        // Anular valores de tendencia que no tienen matriz definida
        if (empty($form_state['values']['elementos_discursivos'][$i]['matriz'])) {
          unset($form_state['values']['elementos_discursivos'][$i]['tendencia']);
          unset($form_state['input']['elementos_discursivos'][$i]['tendencia']);
        }

        if ($form_state['values']['elementos_discursivos'][$i]['tendencia'] != 0){
          $tendencia_term = taxonomy_term_load($form_state['values']['elementos_discursivos'][$i]['tendencia']);
          $tendencia = $tendencia_term->name;
          if ($tendencia == 'Positivo'){
            $tendencia_nota++;
          }
          else {
            $tendencia_nota--;
          }
        }
      }
      if ($tendencia_nota > 0){
        $form_state['values']['tendencia_nota'] = key(taxonomy_get_term_by_name('Positivo', 'tendencia'));
      }
      elseif($tendencia_nota < 0) {
        $form_state['values']['tendencia_nota'] = key(taxonomy_get_term_by_name('Negativo', 'tendencia'));
      }
      else {
        unset($form_state['values']['tendencia_nota']);
      }


      //calculo de centimetros
      if (!empty($form_state['values']['horizontal_cm']) && !empty($form_state['values']['vertical_cm'])) {
        $form_state['values']['centimetros'] = $form_state['values']['horizontal_cm'] * $form_state['values']['vertical_cm'];
      }

      if (!empty($form_state['values']['horizontal_primera_pagina']) && !empty($form_state['values']['vertical_primera_pagina'])) {
        $form_state['values']['centimetros_primera_pagina'] = $form_state['values']['horizontal_primera_pagina'] * $form_state['values']['vertical_primera_pagina'];
      }

      $language = LANGUAGE_NONE;

      if (isset($form_state['values']['nid'])) {
        $node = node_load($form_state['values']['nid']);
        $filas_discursivas_old = count($node->field_tema[$language]);
        if ($filas_discursivas_old > $form_state['values']['filas_discursivo']) {
          unset($node->field_tema[$language]);
          unset($node->field_matriz[$language]);
          unset($node->field_tendencia[$language]);
          unset($node->field_actor[$language]);
          unset($node->field_cuerpo_argumentativo[$language]);
        }
      }
      else {
        //Para evitar duplicidad se debe verificar que la nota no coincida con otra en fecha, titulo y medio
        $query = new EntityFieldQuery();
        $query->entityCondition('entity_type', 'node')
          ->entityCondition('bundle', 'nota')
          ->propertyCondition('title', $form_state['values']['title'])
          ->fieldCondition('field_fecha', 'value', strtotime($form_state['values']['fecha']))
          ->fieldCondition('field_medio', 'tid', key(taxonomy_get_term_by_name($form_state['values']['medio'])));
        $resultado = $query->execute();
        if (count($resultado['node'])) {
          $nid = key($resultado['node']);
          $node_existente = node_load($nid);
          $titulo = l($node_existente->title, 'editar/nota/' . $node_existente->nid, array('attributes' => array('class' => 'ver', 'target'=>'_blank')));
          drupal_set_message("Esta nota ya existe $titulo con nid: $nid", 'error');
          $form_state['rebuild'] = TRUE;
          return;
        }

        $node = new stdClass();
        $node->type = 'nota';
        node_object_prepare($node);

        //si la nota es agregada directamente o no mediante cron se coloca como fecha original la fecha actual
        $node->field_fecha_original[$language][0]['value'] = time();
      }

      //limpiar el titulo y el cuerpo
      $form_state['values']['title'] = trim($form_state['values']['title']);
      $form_state['values']['body'] = trim($form_state['values']['body']);

      $node->title = $form_state['values']['title'];

      $campos = field_info_instances("node",'nota');
      foreach ($campos as $campo => $value) {
        if($value['display']['default']['type'] == 'hidden') {
          continue;
        }
        if ($value['display']['default']['type'] == 'taxonomy_term_reference_link'){
          $tipo = 'tid';
        }
        else {
          $tipo = 'value';
        }
        $campo_forma = ($campo == 'body') ? $campo : substr($campo, 6);
        $elementos_discursivos = array('tema', 'matriz', 'tendencia', 'actor', 'cuerpo_argumentativo');
        if (in_array($campo_forma, $elementos_discursivos)) {
          for ($i=0; $i < $form_state['values']['filas_discursivo']; $i++) {
            if ((!empty($form_state['values']['elementos_discursivos'][$i][$campo_forma]))) {
              $node->{$campo}[$language][$i][$tipo] = ($campo_forma == 'tendencia' || $campo_forma == 'cuerpo_argumentativo') ? $form_state['values']['elementos_discursivos'][$i][$campo_forma] : key(taxonomy_get_term_by_name($form_state['values']['elementos_discursivos'][$i][$campo_forma], $campo_forma));
            }
            else {
              unset($node->{$campo}[$language][$i]);
            }
          }
        }
        elseif (!empty($form_state['values'][$campo_forma])) {
          if ( $campo_forma == 'area' || $campo_forma == 'medio' || $campo_forma == 'usuario') {
            if ($campo_forma == 'usuario') {
              $i = 0;
              foreach($form_state['values']['usuario'] as $tid_usuario) {
                $node->{$campo}[$language][$i][$tipo] = $tid_usuario;
                $i++;
              }
            }
            else {
              $node->{$campo}[$language][0][$tipo] = key(taxonomy_get_term_by_name($form_state['values'][$campo_forma], $campo_forma));
            }
          }
          elseif ($campo_forma == 'fecha') {
            $node->{$campo}[$language][0][$tipo] = strtotime($form_state['values'][$campo_forma]);
          }
          else {
            $node->{$campo}[$language][0][$tipo] = $form_state['values'][$campo_forma];
          }
        }
        elseif ($campo !== 'field_analista_asignado') {
          unset($node->{$campo}[$language]);
        }

      }

      // usuario que hizo la actualización
      if (isset($form_state['values']['nid'])) {
        global $user;
        $node->field_usuario_actualizacion[$language][0]['target_id'] = $user->uid;
      }
      node_save($node);
      if (isset($form_state['values']['nid'])) {
        drupal_set_message("Nota con titulo '" . $form_state['values']['title'] . "' actualizada");
        $form_state['rebuild'] = TRUE;
      }
      else {
        $titulo = l($form_state['values']['title'], 'notas/ver/' . $node->nid, array('attributes' => array('class' => array('ver'))));
        drupal_set_message("Nota con titulo '" . $titulo . "' insertada con nid: " . $node->nid);
        $form_state['values'] = array();
        $form_state['input'] = array();
        $form_state['rebuild'] = TRUE;
      }
      break;
  }
}

function vistas_nota_form_validate($form, &$form_state) {
  //verificar valores
  $elementos = array('medio', 'area');

  foreach ($elementos as $value) {
    if(!empty($form_state['values'][$value])) {
      $tid = buscar_termino($form_state['values'][$value], $value);
      if (empty($tid)) {
        form_set_error($value, "El valor '". $form_state['values'][$value] ."' es invalido para el campo $value");
      }
    }
  }

  $campos_numericos = array('horizontal_cm', 'vertical_cm', 'horizontal_primera_pagina', 'vertical_primera_pagina');

  foreach ($campos_numericos as $value) {
    if (!empty($form_state['values'][$value]) && !is_numeric($form_state['values'][$value])){
      $titulo = isset($form_state['complete form']['contenedor']['medio']['impreso'][$value]['#title']) ? $form_state['complete form']['contenedor']['medio']['impreso'][$value]['#title'] : $form_state['complete form']['contenedor']['medio']['impreso']['primera_pagina'][$value]['#title'];
      form_set_error($value, "El valor '". $form_state['values'][$value] ."' es invalido para el campo $titulo, use un punto(.) para decimales");
    }
  }


  $filas_discursivo = $form_state['values']['filas_discursivo'];

  $elementos_discursivos = array('tema', 'matriz', 'actor');
  for ($i=0; $i < $filas_discursivo; $i++) {
    foreach ($elementos_discursivos as $value) {
      if(!empty($form_state['values']['elementos_discursivos'][$i][$value])) {
        $tid = buscar_termino($form_state['values']['elementos_discursivos'][$i][$value], $value);
        if (empty($tid)) {
          form_set_error("elementos_discursivos][$i][" . $value, "El valor '". $form_state['values']['elementos_discursivos'][$i][$value] ."' es invalido para el campo $value");
        }
      }
    }

    //Si se define un elemento en una fila discursiva a parte de un tema se deben definir todos los demas
    if(!empty($form_state['values']['elementos_discursivos'][$i]['tema'])) {
      $elementos_fila_discursivo = array('matriz', 'tendencia', 'cuerpo_argumentativo', 'actor');
      $cantidad_elementos_definidos = 0;
      foreach ($elementos_fila_discursivo as $value) {
        if (!empty($form_state['values']['elementos_discursivos'][$i][$value]) || $form_state['values']['elementos_discursivos'][$i][$value] != 0){
          // si el unico elemento definido es la tendencia este es automaticamente descartado si no se ha definido la matriz
          if ($value != 'tendencia'){
            $cantidad_elementos_definidos++;
          }
        }
      }

      //si alguno de los elemento matriz, cuerpo_argumentativo o matriz estan definido, el resto tambien lo debe estar

      if (($cantidad_elementos_definidos < 3) && ($cantidad_elementos_definidos > 0)) {
        foreach ($elementos_fila_discursivo as $value) {
          if (!empty($form_state['values']['elementos_discursivos'][$i][$value]) || $form_state['values']['elementos_discursivos'][$i][$value] != 0){
            form_set_error("elementos_discursivos][$i][" . $value, '');
          }
        }
        $tema = $form_state['values']['elementos_discursivos'][$i]['tema'];
        form_set_error($tema, "Faltan elementos por definir en la fila $i ($tema)");
      }
      // la tendencia debe estar definida si todos los demas elementos en la fila discursiva lo estan
      elseif($cantidad_elementos_definidos == 3 && $form_state['values']['elementos_discursivos'][$i]['tendencia'] == 0) {
        $matriz = $form_state['values']['elementos_discursivos'][$i]['matriz'];
        form_set_error("elementos_discursivos][$i][tendencia", "La tendencia debe ser definida para la matriz '$matriz' en la fila $i");
      }

    }

  }


  //Verificar que los valores de tendencia no dan un resultado neutro
  $tendencia_nota = 0;
  $cantidad_tendencia = 0;
  for ($i=0; $i < $form_state['values']['filas_discursivo']; $i++) {
    if (($form_state['values']['elementos_discursivos'][$i]['tendencia'] != 0) && !empty($form_state['values']['elementos_discursivos'][$i]['matriz'])){
      $tendencia_term = taxonomy_term_load($form_state['values']['elementos_discursivos'][$i]['tendencia']);
      $tendencia = $tendencia_term->name;
      $cantidad_tendencia++;
      if ($tendencia == 'Positivo') {
        $tendencia_nota++;
      }
      else {
        $tendencia_nota--;
      }
    }
  }
  if ($tendencia_nota == 0 && $cantidad_tendencia > 0) {
    form_set_error('','La tendencia de la nota no puede ser neutra');
    form_set_value($form['contenedor']['inferior']['tendencia'], 'a', $form_state);
  }

  //verificar permisos para borrar
  if ( ($form_state['triggering_element']['#return_value'] == 'Eliminar') && !contenido_access('borrar', 'nota', $form_state['values']['nid'])) {
    form_set_error('eliminar', t('No tiene permisos para borrar esta nota'));
  }

}


function form_vistas_notas_fila_discursivo_callback($form, $form_state) {
  return array(
    '#type' => 'ajax',
    '#commands' => array(
      ajax_command_replace("#elementos_discursivos", render($form['contenedor']['inferior']['elementos_discursivos'])),
    )
  );
}

function forma_vista_notas_submit_callback($form, $form_state) {
  $messages = drupal_get_messages(NULL, FALSE);
  $commands = array();
  // si hay mensajes imprimirlos, excepto si el argumento $AnalizarNotas es verdadero, en este caso no se imprime para imprimirlo una vez que se cargue la siguiente nota.
  //En caso de haber errores detener el redirect
  if (($messages && !$form_state['build_info']['args'][1]) || (form_get_errors()) ){
    $messages = theme('status_messages');
    $commands[] = ajax_command_after(NULL, '<div id="messages">' . $messages . '</div>');

    //eliminar el markup del mensaje de la nota anterior
    unset($form['wrapper']);
    $commands[] = ajax_command_replace("#formwrapper", render($form));

  }

  //redirigir a la siguiente nota para analizar, sí verdadero el argumento de analizar notas
  elseif($form_state['build_info']['args'][1] && !form_get_errors()) {
    global $user;
    $query = db_select('field_data_field_analista_asignado', 'faa');
    $query->LeftJoin('field_data_field_matriz','fm', 'faa.entity_id = fm.entity_id');
    $query->LeftJoin('field_data_field_actor','fac', 'faa.entity_id = fac.entity_id');
    $query
      ->condition('faa.field_analista_asignado_target_id', $user->uid);
    $query->addExpression('MAX(faa.entity_id)');
    $query->isNull('fm.field_matriz_tid');
    $query->isNull('fac.field_actor_tid');

    $resultado = $query->execute();
    $nid = $resultado->fetchField();

    ctools_include('ajax');
    ctools_add_js('ajax-responder');
    $commands[] = ctools_ajax_command_redirect('editar/nota/' . $nid . '/1');
  }


  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );

}

function form_vistas_nota_eliminar_callback($form, $form_state) {
  $messages = theme('status_messages');
  /*//return theme('status_messages');
  $commands[] = ajax_command_before(NULL, '<div id="messages">' . theme('status_messages') . '</div>');
  return array('#type' => 'ajax', '#commands' => $commands);*/

  if($messages){
    $form["wrapper"] = array("#markup" => "<div class='inline-messages'>$messages</div>");
    // si hay un mensaje de error colocarlo tambien en la parte inferior
    /*if (strpos($messages, 'error') !== FALSE) {
      $form["wrapper2"] = array("#markup" => "<div class='inline-messages'>$messages</div>");
    }*/
  }
  return $form;

}

function form_vistas_nota_tipo_titulo_callback($form, $form_state) {
  return array(
    '#type' => 'ajax',
    '#commands' => array(
      ajax_command_replace("#titular", render($form['contenedor']['titular'])),
    )
  );
}

function form_vistas_nota_tipo_medio_callback($form, $form_state) {
  return array(
    '#type' => 'ajax',
    '#commands' => array(
      ajax_command_replace("#medio", render($form['contenedor']['medio'])),
    )
  );
}



function form_vistas_nota_tipo_carga_callback($form, $form_state) {
  return array(
    '#type' => 'ajax',
    '#commands' => array(
      ajax_command_replace("#primera_pagina", render($form['contenedor']['medio']['impreso']['primera_pagina'])),
    )
  );
}

function form_vistas_nota_tipo_titulo_primera_pagina_callback($form, $form_state) {
  return array(
    '#type' => 'ajax',
    '#commands' => array(
      ajax_command_replace("#titular_primera_pagina", render($form['contenedor']['medio']['impreso']['primera_pagina']['titular_primera_pagina'])),
    )
  );
}

function form_vistas_nota_centimetros_callback($form, $form_state) {
  if (!empty($form_state['values']['horizontal_cm']) && !empty($form_state['values']['vertical_cm'])) {
    $form['contenedor']['medio']['impreso']['centimetros']['#value'] = $form_state['values']['horizontal_cm'] * $form_state['values']['vertical_cm'];
  }
  return array(
    '#type' => 'ajax',
    '#commands' => array(
      ajax_command_replace("#centimetros", render($form['contenedor']['medio']['impreso']['centimetros'])),
    )
  );
}

function form_vistas_nota_centimetros_primera_pagina_callback($form, $form_state) {
  if (!empty($form_state['values']['horizontal_primera_pagina']) && !empty($form_state['values']['vertical_primera_pagina'])) {
    $form['contenedor']['medio']['impreso']['primera_pagina']['centimetros_primera_pagina']['#value'] = $form_state['values']['horizontal_primera_pagina'] * $form_state['values']['vertical_primera_pagina'];
  }
  return array(
    '#type' => 'ajax',
    '#commands' => array(
      ajax_command_replace("#centimetros_primera_pagina", render($form['contenedor']['medio']['impreso']['primera_pagina']['centimetros_primera_pagina'])),
    )
  );
}




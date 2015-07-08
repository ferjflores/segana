<?php

function distribucion_notas_form($form, &$form_state, $NotasAsignadas = NULL) {
  $form = array();
  global $user;
  $form['#prefix'] = '<div id="formwrapper">';
  $form['#suffix'] = '</div>';
  if (isset($NotasAsignadas)) {
    $form['#attributes']['class'][] = 'page-distribucion-notas';
  }

  $form['fecha'] = array(
    '#type' => 'fieldset',
    '#title' => 'FECHA',
    '#prefix' => '<div id="fecha">',
    '#suffix' => '</div>',
  );
  $rango_ano_inicio = 2008 - date('Y');
  $form['fecha']['fecha_desde'] = array(
    '#type' => 'date_popup',
    '#title' => t('desde el'),
    '#attributes' => array('autocomplete' =>'off'),
    '#date_format' => 'd-m-Y',
    '#date_year_range' => "$rango_ano_inicio:+0",
    '#size' => 6,
    '#default_value' => isset($form_state['values']['fecha_desde']) ? $form_state['values']['fecha_desde'] : date('Y-m-d'),
  );
  $form['fecha']['fecha_hasta'] = array(
    '#type' => 'date_popup',
    '#title' => t('hasta el'),
    '#attributes' => array('autocomplete' =>'off'),
    '#date_format' => 'd-m-Y',
    '#default_value' => isset($form_state['storage']['values']['fecha_hasta']) ? $form_state['storage']['values']['fecha_hasta'] : 0,
    '#date_year_range' => "$rango_ano_inicio:+0",
    '#size' => 6,
  );

  $form['contenedor'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="contenedor">',
    '#suffix' => '</div>',
  );

  $form['contenedor']['superior'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="superior">',
    '#suffix' => '</div>',
  );


  $terms = taxonomy_get_terms_by_name('medio');
  foreach($terms as $tid => $term) {
    $medio_terms[$tid] = $term->name;
  }
  $form['contenedor']['superior']['medio'] = array(
    '#type' => 'select',
    '#title' => 'Medio',
    '#options' => $medio_terms,
    '#empty_option' => 'Todos',
    '#default_value' => !empty($form_state['values']['medio']) ? $form_state['values']['medio'] : NULL,
    '#validated' => TRUE,
  );

  $form['contenedor']['superior']['area'] = array(
    '#type' => 'textfield',
    '#title' => 'Área',
    '#attributes' => array('autocomplete' =>'off'),
    '#default_value' => !empty($form_state['values']['area']) ? $form_state['values']['area'] : NULL,
    '#size' => 30,
    '#maxlength' => 256,
    '#autocomplete_path' => 'notas/autocomplete/area/0/0',
  );

  $form['contenedor']['superior']['tema'] = array(
    '#type' => 'textfield',
    '#title' => 'Tema',
    '#attributes' => array('autocomplete' =>'off'),
    '#default_value' => !empty($form_state['values']['tema']) ? $form_state['values']['tema'] : NULL,
    '#size' => 30,
    '#maxlength' => 256,
    '#autocomplete_path' => 'notas/autocomplete/tema/0/0',
  );

  $form['contenedor']['superior']['usuario'] = array(
    '#type' => 'textfield',
    '#title' => 'Usuario',
    '#attributes' => array('autocomplete' =>'off'),
    '#default_value' => !empty($form_state['values']['usuario']) ? $form_state['values']['usuario'] : NULL,
    '#size' => 30,
    '#maxlength' => 256,
    '#autocomplete_path' => 'notas/autocomplete/usuario/0/0',
  );

  $form['contenedor']['superior']['palabra_clave'] = array(
    '#type' => 'textfield',
    '#title' => 'Palabra clave',
    '#attributes' => array('autocomplete' =>'off'),
    '#default_value' => !empty($form_state['values']['palabra_clave']) ? $form_state['values']['palabra_clave'] : NULL,
    '#size' => 30,
    '#maxlength' => 256,
  );

  $form['contenedor']['superior']['analisis'] = array(
    '#type' => 'select',
    '#title' => 'Análisis',
    '#options' => array('No analizada', 'Analizada', 'Todas'),
    '#default_value' => !empty($form_state['values']['analisis']) ? $form_state['values']['analisis'] : 0,
  );

  $form['contenedor']['superior']['asignacion'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="asignacion">',
    '#suffix' => '</div>',
  );


  $analistas = (isset($form_state['triggering_element']['#return_value']) && $form_state['triggering_element']['#return_value'] == 'BuscarSeleccionarAnalista') ? analistas(TRUE) : analistas();

  $form['contenedor']['superior']['asignacion']['analista'] = array(
    '#type' => 'select',
    '#title' => 'Seleccionar Analista',
    '#options' => $analistas,
    '#empty_option' => 'Sin analista asignado',
    '#default_value' => (!empty($form_state['values']['analista'])) ? $form_state['values']['analista'] : NULL,
    '#validated' => TRUE,
  );

  if (isset($form_state['triggering_element']['#return_value']) && $form_state['triggering_element']['#return_value'] == 'BuscarSeleccionarAnalista') {

    $form['contenedor']['superior']['asignacion']['buscar'] = array(
      '#type' => 'submit',
      '#value' => 'Buscar ',
      '#return_value' => t('Buscar'),
      '#submit' => array('distribucion_notas_form_submit'),
      '#ajax' => array(
        'event' => 'click',
        'callback' => 'form_distribucion_notas_buscar_callback',
        'wrapper' => 'busqueda',
        'method' => 'replace',
        'effect' => 'fade',
        'progress' => array('type' => 'throbber'),
        //'prevent' => 'submit click mousedown',
      ),
    );
  }
  else {
    $form['contenedor']['superior']['asignacion']['confirmar'] = array(
      '#type' => 'submit',
      '#value' => t('Confimar analista'),
      '#return_value' => t('Confimar analista'),
      '#submit' => array('distribucion_notas_form_submit'),
      '#ajax' => array(
        'event' => 'click',
        'callback' => 'form_distribucion_notas_confirmar_callback',
        'wrapper' => 'inferior',
        'method' => 'replace',
        'effect' => 'fade',
        'progress' => array('type' => 'throbber'),
        //'prevent' => 'submit click mousedown',
      ),
    );
  }

  $form['contenedor']['botones'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="botones">',
    '#suffix' => '</div>',
  );

  $form['contenedor']['botones']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#return_value' => t('Reset'),
  );

  if (!isset($NotasAsignadas)) {
    $form['contenedor']['botones']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Asignar Notas'),
      '#return_value' => 'AsignarNotasSeleccionarAnalista',
      '#submit' => array(),
      '#validate' => array('distribucion_notas_form_validate'),
      '#ajax' => array(
        'event' => 'click',
        'callback' => 'form_distribucion_notas_submit_callback',
        'wrapper' => 'superior',
        'method' => 'replace',
        'effect' => 'fade',
        'progress' => array('type' => 'throbber'),
        //'prevent' => 'submit click mousedown',
      ),
    );
  }

  if (!isset($NotasAsignadas)) {
    $form['contenedor']['botones']['buscar_seleccionar_analista'] = array(
      '#type' => 'submit',
      '#value' => t('Buscar'),
      '#name' => 'BuscarSeleccionarAnalista',
      '#return_value' => 'BuscarSeleccionarAnalista',
      '#submit' => array('distribucion_notas_form_submit'),
      '#ajax' => array(
        'event' => 'click',
        'callback' => 'form_distribucion_notas_buscar_seleccionar_analista_callback',
        'wrapper' => 'superior',
        'method' => 'replace',
        'effect' => 'fade',
        'progress' => array('type' => 'throbber'),
      ),
    );
  }
  else {
    $form['contenedor']['botones']['buscar'] = array(
      '#type' => 'submit',
      '#value' => 'Buscar',
      '#return_value' => t('Buscar'),
      '#submit' => array('distribucion_notas_form_submit'),
      '#ajax' => array(
        'event' => 'click',
        'callback' => 'form_distribucion_notas_buscar_callback',
        'wrapper' => 'busqueda',
        'method' => 'replace',
        'effect' => 'fade',
        'progress' => array('type' => 'throbber'),
        //'prevent' => 'submit click mousedown',
      ),
    );
  }

  $form['contenedor']['inferior'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="inferior">',
    '#suffix' => '</div>',
  );

  $form['contenedor']['inferior']['busqueda'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="busqueda">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
    '#theme' => 'tabla_notas',
  );

  if (!isset($form_state['notas_busqueda'])) {
    $form_state['notas_busqueda'] = isset($NotasAsignadas) ? busqueda_distribucion_notas(array('uid' => $user->uid)) : busqueda_distribucion_notas();
  }

  // encabezado
  $titulo_busqueda	= theme('html_tag', array('element' => array('#tag' => 'div', '#attributes' => array('class' => 'titulo_busqueda'),'#value' => 'Noticias (' . count($form_state['notas_busqueda']) . ')')));

  $form['contenedor']['inferior']['busqueda']['titulo'] = array(
    '#markup' => theme('html_tag', array(
      'element' => array(
        '#tag' => 'div',
        '#attributes' => array('class' => 'encabezado'),
        '#value' => $titulo_busqueda
      )
    ))
  );

  $form['contenedor']['inferior']['busqueda']['marcar_todo'] = array(
    '#type' => 'checkbox',
    '#title' => ' ',
    '#attributes' => (isset($form_state['values']['marcar_todo'])) ? array('checked' => 'checked', 'class' => array('marcar_todo')) : array('class' => array('marcar_todo')),
  );


  foreach ($form_state['notas_busqueda'] as $nid => $values) {
    $imagen_editar = theme_image(array('path' => 'sites/all/themes/segana/images/edita_rojo.png', 'alt' => 'Editar',  'attributes' => array()));
    $link_editar = l($imagen_editar, 'editar/nota/' . $values['nid'] . '/1', array('html' => TRUE, 'attributes' => array('target'=>'_blank')));

    $form['contenedor']['inferior']['busqueda'][$nid] = [
      'checkbox' => [
        '#type' => 'checkbox',
        '#title' => ' ',
        '#attributes' => (isset($form_state['values']['busqueda'][$nid]['checkbox']) && ($form_state['values']['busqueda'][$nid]['checkbox'] !== 0)) ? array('checked' => 'checked') : NULL,
      ],
      'area' => ['#markup' => $values['area']],
      'title' => ['#markup' => $values['title']],
      'etiquetas' => ['#markup' => $values['etiquetas']],
      'medio' => ['#markup' => $values['medio']],
      'fecha' => ['#markup' => $values['fecha']],
      'temas' => ['#markup' => $values['temas']],
      'matrices' => ['#markup' => $values['matrices']],
      'actores' => ['#markup' => $values['actores']],
      'usuarios' => ['#markup' => $values['usuarios']],
      'analista_asignado' => ['#markup' => $values['analista_asignado'] . $link_editar]
    ];
  }


  $form['#attached'] = [
    'js' => [drupal_get_path('module', 'distribucion_notas') . '/js/distribucion_notas.js'],
    'css' => [libraries_get_path('datatables'). '/media/css/jquery.dataTables.css']
  ];

  return $form;
}


function distribucion_notas_form_submit($form, &$form_state) {
  $pressed_button = check_plain($form_state['triggering_element']['#return_value']);
  switch ($pressed_button) {
    /*case 'AsignarNotasSeleccionarAnalista':
      $form_state['rebuild'] = TRUE;
      break;*/
    case 'Reiniciar':
      $form_state['rebuild'] = TRUE;
      unset($form_state['values']);
      break;
    case 'BuscarSeleccionarAnalista':
      $form_state['rebuild'] = TRUE;
      break;
    case 'Buscar':
      $variables = array(
        'medio' => $form_state['values']['medio'],
        'area' => $form_state['values']['area'],
        'tema' => $form_state['values']['tema'],
        'usuario' => $form_state['values']['usuario'],
        'fecha_desde' => $form_state['values']['fecha_desde'],
        'fecha_hasta' => $form_state['values']['fecha_hasta'],
        'palabra_clave' => $form_state['values']['palabra_clave'],
        'busqueda_exacta' => $form_state['values']['busqueda_exacta'],
        'cualquiera' => $form_state['values']['cualquiera'],
        'uid' =>  ($form_state['values']['analista'] != -1) ? $form_state['values']['analista'] : NULL,
        'notas_asignadas' => $form_state['values']['analista'],
        'analisis' => $form_state['values']['analisis'],
      );

      if ($form_state['build_info']['args'][0]) {
        global $user;
        $variables['uid'] = $user->uid;
      }

      $form_state['notas_busqueda'] = busqueda_distribucion_notas($variables);
      $form_state['rebuild'] = TRUE;
      break;
    case 'Confimar analista':
      $nids = array();
      foreach ($form_state['notas_busqueda'] as $nid => $values) {
        if ($form_state['values']['busqueda'][$nid]['checkbox'] == 1) {
          $nids[] = $nid;
        }
      }
      if (count($nids) > 0) {
        $nodes = entity_load('node', $nids);
        foreach($nodes as $node) {
          if ($form_state['values']['analista']) {
            $node->field_analista_asignado[LANGUAGE_NONE][0]['target_id'] = $form_state['values']['analista'];
          }
          else {
            unset($node->field_analista_asignado[LANGUAGE_NONE][0]);
          }
          field_attach_presave('node', $node);
          field_attach_update('node', $node);
          entity_get_controller('node')->resetCache(array($node->nid));
        }
      }

      $variables = array(
        'area' => $form_state['values']['area'],
        'tema' => $form_state['values']['tema'],
        'usuario' => $form_state['values']['usuario'],
        'fecha_desde' => $form_state['values']['fecha_desde'],
        'fecha_hasta' => $form_state['values']['fecha_hasta'],
        'palabra_clave' => $form_state['values']['palabra_clave'],
        'busqueda_exacta' => $form_state['values']['busqueda_exacta'],
        'cualquiera' => $form_state['values']['cualquiera'],
      );
      $form_state['notas_busqueda'] = busqueda_distribucion_notas($variables);
      $form_state['rebuild'] = TRUE;
      break;
  }
}
function distribucion_notas_form_validate($form, &$form_state) {
  //eliminar espacios en blanco
  form_set_value($form['contenedor']['superior']['area'], trim($form_state['values']['area']), $form_state);
  form_set_value($form['contenedor']['superior']['tema'], trim($form_state['values']['tema']), $form_state);
  form_set_value($form['contenedor']['superior']['usuario'], trim($form_state['values']['usuario']), $form_state);


  if ($form_state['triggering_element']['#value'] == 'Buscar') {
    $elementos_validar = array('area', 'tema', 'usuario');
    foreach($elementos_validar as $elemento){
      if (isset($form_state['values'][$elemento]) && !empty($form_state['values'][$elemento])) {
        $tid = buscar_termino($form_state['values'][$elemento], $elemento);
        if (empty($tid)) {
          form_set_error($elemento, "El valor '" . $form_state['values'][$elemento] . "' es invalido para el campo " . $form['contenedor']['superior'][$elemento]['#title']);
        }
      }
    }
  }
  if ($form_state['triggering_element']['#value'] == 'Asignar Notas') {
    $NotasPorAsignar = 0;
    foreach ($form_state['notas_busqueda'] as $nid => $values) {
      if (isset($form_state['values']['busqueda'][$nid]['checkbox']) && !empty($form_state['values']['busqueda'][$nid]['checkbox'])) {
        $NotasPorAsignar++;
      }
    }
    if ($NotasPorAsignar == 0) {
      form_set_error('', 'Debe seleccionar las notas que desea asignar');
    }
  }
}




function form_distribucion_notas_buscar_callback($form, $form_state) {
  $messages = theme('status_messages');
  $commands = array();
  if($messages){
    $commands[] = ajax_command_after(NULL, '<div id="messages">' . $messages . '</div>');
  }

  $commands[] = ajax_command_replace("#busqueda", render($form['contenedor']['inferior']['busqueda']));
  $commands[] =  array('command' => 'afterAjaxCallbackBuscar');

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

function form_distribucion_notas_buscar_seleccionar_analista_callback($form, $form_state) {
  $messages = theme('status_messages');
  $commands = array();
  if($messages){
    $commands[] =   ajax_command_after(NULL, '<div id="messages">' . $messages . '</div>');
  }

  $commands[] = ajax_command_replace("#superior", render($form['contenedor']['superior']));
  if (!form_get_errors()) {
    $commands[] = array('command' => 'afterAjaxCallbackDialogoSeleccionarAnalista');
  }


  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

function form_distribucion_notas_submit_callback($form, $form_state) {
  $messages = theme('status_messages');
  $commands = array();
  if($messages){
    $commands[] =   ajax_command_after(NULL, '<div id="messages">' . $messages . '</div>');
  }

  $commands[] = ajax_command_replace("#superior", render($form['contenedor']['superior']));
  if (!form_get_errors()) {
    $commands[] = array('command' => 'afterAjaxCallbackDialogoSeleccionarAnalista');
  }


  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

function form_distribucion_notas_confirmar_callback($form, $form_state) {
  $messages = theme('status_messages');
  $commands = array();
  if($messages){
    $commands[] = ajax_command_after(NULL, '<div id="messages">' . $messages . '</div>');
  }

  $commands[] = ajax_command_replace("#inferior", render($form['contenedor']['inferior']));
  $commands[] =  array('command' => 'afterAjaxCallbackConfirmarNotas');
  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

 /* global $user;
  if (array_search('coordinador', $user->roles)) {*/




/**
 * @param array $variables = array(
 *  'uid' => 'uid',               // uid del user del cual se quieren las notas asignadas
 *  'notas_asignadas' => 'uid',   // bool que indica si las notas debe estar asignadas notas asignadas
 *  'medio' => 'tid,'             //tid del medio
 *  'area' => 'name,'             //nombre del area
 *  'tema' => 'name,'             //nombre del tema
 *  'usuario' => 'name,'          //nombre del usuario
 *  'fecha_desde' => 'd-m-Y',     //fecha desde string cualquier formato
 *  'fecha_hasta' => 'd-m-Y',     //fecha hasta string cualquier formato
 *  'palabra_clave' => 'palabra(s) clave(s)', //string de las palabras clave
 *  'busqueda_exacta' => 0,       // bool que indica si se debe hacer busqueda exacta
 *  'cualquiera' => 0,            // bool que indica si se debe buscar cualqueira de las balabras claves
 *  'analisis' => 0,              // variable de tres estados que indica si las notas deben estar analizadas. NULL cualquiera
 * );
 * @return array
 */
function busqueda_distribucion_notas($variables = array()) {
  $query = db_select('node', 'n');
  $query->leftJoin('field_data_field_area', 'fa', 'n.nid = fa.entity_id AND fa.bundle = :bundle', array(':bundle' => 'nota'));
  $query->join('field_data_field_fecha', 'ff', 'ff.entity_id = n.nid AND ff.bundle = :bundle', array(':bundle' => 'nota'));
  $query->leftJoin('field_data_field_fecha_original', 'ffo', 'ffo.entity_id = n.nid AND ffo.bundle = :bundle', array(':bundle' => 'nota'));
  $query->join('field_data_field_medio', 'fme', 'fme.entity_id = n.nid AND fme.bundle = :bundle', array(':bundle' => 'nota'));
  $query->leftJoin('field_data_field_etiqueta', 'fe', 'n.nid = fe.entity_id');
  $query->leftJoin('taxonomy_term_data', 'ttd_area', 'fa.field_area_tid = ttd_area.tid');
  $query->join('taxonomy_term_data', 'ttd_medio', 'fme.field_medio_tid = ttd_medio.tid');
  $query->leftJoin('taxonomy_term_data', 'ttd_etiqueta', 'fe.field_etiqueta_tid = ttd_etiqueta.tid');
  $query
    ->fields('ttd_area', array('name'))
    ->fields('n', array('nid', 'title'))
    ->fields('ttd_medio', array('name'))
    ->fields('ff', array('field_fecha_value'))
    ->fields('ffo', array('field_fecha_original_value'))
    ->fields('u', array('name'))
    ->orderBy('ttd_area.name', 'ASC')
    ->orderBy('ttd_tema.name', 'ASC')
    ->orderBy('ff.field_fecha_value', 'ASC')
    ->orderBy('n.title', 'ASC')
    ->groupBy('n.nid');
  $query->addExpression("GROUP_CONCAT(DISTINCT ttd_tema.name ORDER BY ft.delta separator ',<br>')", 'temas');
  $query->addExpression("GROUP_CONCAT(DISTINCT ttd_matriz.name ORDER BY fm.delta separator ',<br>')", 'matrices');
  $query->addExpression("GROUP_CONCAT(DISTINCT ttd_actor.name ORDER BY fac.delta separator ',<br>')", 'actores');
  $query->addExpression("GROUP_CONCAT(DISTINCT ttd_usuario.name ORDER BY fu.delta separator ',<br>')", 'usuarios');
  $query->addExpression("GROUP_CONCAT(DISTINCT ttd_etiqueta.name ORDER BY fe.delta separator ',<br>')", 'etiquetas');
  if (!empty($variables['area'])) {
    $query->condition('ttd_area.name', $variables['area']);
  }
  if (!empty($variables['medio'])) {
    $query->condition('fme.field_medio_tid', $variables['medio']);
  }

  //tema
  if (!empty($variables['tema'])) {
    $query->join('field_data_field_tema', 'ft', 'n.nid = ft.entity_id AND ft.bundle = :bundle', array(':bundle' => 'nota'));
    $query->join('taxonomy_term_data', 'ttd_tema', 'ft.field_tema_tid = ttd_tema.tid');
    $query->condition('ttd_tema.name', $variables['tema']);
  }
  else {
    $query->leftJoin('field_data_field_tema', 'ft', 'n.nid = ft.entity_id AND ft.bundle = :bundle', array(':bundle' => 'nota'));
    $query->leftJoin('taxonomy_term_data', 'ttd_tema', 'ft.field_tema_tid = ttd_tema.tid');
  }

  // resto elementos discursivos
  $query->leftJoin('field_data_field_matriz', 'fm', 'n.nid = fm.entity_id AND fm.delta = ft.delta');
  $query->leftJoin('field_data_field_actor', 'fac', 'n.nid = fac.entity_id AND fac.delta = ft.delta');
  $query->leftJoin('taxonomy_term_data', 'ttd_matriz', 'fm.field_matriz_tid = ttd_matriz.tid');
  $query->leftJoin('taxonomy_term_data', 'ttd_actor', 'fac.field_actor_tid = ttd_actor.tid');

  if (!empty($variables['usuario'])){
    $query->join('field_data_field_usuario', 'fu', 'n.nid = fu.entity_id AND fu.bundle = :bundle', array(':bundle' => 'nota'));
    $query->join('taxonomy_term_data', 'ttd_usuario', 'fu.field_usuario_tid = ttd_usuario.tid');
    $query->condition('ttd_usuario.name', $variables['usuario']);
  }
  else {
    $query->leftJoin('field_data_field_usuario', 'fu', 'n.nid = fu.entity_id AND fu.bundle = :bundle', array(':bundle' => 'nota'));
    $query->leftJoin('taxonomy_term_data', 'ttd_usuario', 'fu.field_usuario_tid = ttd_usuario.tid');
  }
  if (!empty($variables['palabra_clave'])){
    $query = query_palabra_clave($query, $variables);
  }

  if (!empty($variables['fecha_hasta'])) {
    $query->condition('ff.field_fecha_value', strtotime($variables['fecha_desde']), '>=')
      ->condition('ff.field_fecha_value', strtotime($variables['fecha_hasta']), '<=');
  }
  elseif(isset($variables['fecha_desde'])) {
    $query->condition('ff.field_fecha_value', strtotime($variables['fecha_desde']));
  }
  else {
    $query->condition('ff.field_fecha_value', strtotime(date('d-m-Y')));
  }
  if (!empty($variables['uid'])) {
    $query->join('field_data_field_analista_asignado', 'faa', 'faa.entity_id = n.nid');
    $query->join('users', 'u', 'faa.field_analista_asignado_target_id = u.uid');
    $query->condition('faa.field_analista_asignado_target_id', $variables['uid']);
  }
  elseif(!empty($variables['notas_asignadas'])) {
    $query->join('field_data_field_analista_asignado', 'faa', 'faa.entity_id = n.nid');
    $query->join('users', 'u', 'faa.field_analista_asignado_target_id = u.uid');
    $query->isNotNull('faa.field_analista_asignado_target_id');
  }
  else {
    $query->leftJoin('field_data_field_analista_asignado', 'faa', 'faa.entity_id = n.nid');
    $query->leftJoin('users', 'u', 'faa.field_analista_asignado_target_id = u.uid');
    $query->isNull('faa.field_analista_asignado_target_id');
  }
  if (empty($variables['analisis']) || $variables['analisis'] == 0) {
    $query->isNULL('fm.field_matriz_tid');
    $query->isNULL('fac.field_actor_tid');
  }
  elseif ($variables['analisis'] == 1) {
    $query->isNotNULL('fm.field_matriz_tid');
    $query->isNotNULL('fac.field_actor_tid');
  }
  $resultado = $query->execute();
  $count_query = $resultado->rowCount();
  if ($count_query > 0) {
    $notas_busqueda = array();
    foreach ($resultado as $row) {
      $notas_busqueda[$row->nid] = array(
        'nid' => $row->nid,
        'area' => $row->name,
        'title' => $row->title,
        'medio' => $row->ttd_medio_name,
        'fecha' => isset($row->field_fecha_original_value) ? date('d/m/Y h:i a', $row->field_fecha_original_value) : date('d/m/Y', $row->field_fecha_value),
        'temas' => !is_null($row->temas) ? $row->temas : '',
        'usuarios' => !is_null($row->usuarios) ? $row->usuarios : '',
        'matrices' => !is_null($row->matrices) ? $row->matrices : '',
        'actores' => !is_null($row->actores) ? $row->actores : '',
        'analista_asignado' => !is_null($row->u_name) ? $row->u_name : '',
        'etiquetas' => !is_null($row->etiquetas) ? $row->etiquetas : '',
      );
    }
    return $notas_busqueda;
  }
  else {
    return array();
  }
}


function theme_tabla_notas($variables) {
  $rows = [];

  $header = [
    ['data' => drupal_render($variables['element']['marcar_todo']), 'class' => 'checkbox', 'datatable_options' => ['bSortable' => FALSE]],
    ['data' => 'Área', 'class' => 'area', 'datatable_options' => ['bSortable' => TRUE]],
    ['data' => 'Titulo', 'class' => 'title', 'datatable_options' => ['bSortable' => TRUE]],
    ['data' => 'Etiqueta', 'class' => 'etiqueta', 'datatable_options' => ['bSortable' => TRUE]],
    ['data' => 'Medio', 'class' => 'medio', 'datatable_options' => ['bSortable' => TRUE]],
    ['data' => 'Fecha', 'class' => 'fecha', 'datatable_options' => ['bSortable' => TRUE]],
    ['data' => 'Tema', 'class' => 'tema', 'datatable_options' => ['bSortable' => TRUE]],
    ['data' => 'Matriz', 'class' => 'matriz', 'datatable_options' => ['bSortable' => TRUE]],
    ['data' => 'Actor', 'class' => 'actor', 'datatable_options' => ['bSortable' => TRUE]],
    ['data' => 'Usuario', 'class' => 'usuario', 'datatable_options' => ['bSortable' => TRUE]],
    ['data' => 'Analista', 'class' => 'analista_asignado', 'datatable_options' => ['bSortable' => TRUE]],
  ];

  $titulo = drupal_render($variables['element']['titulo']);
  unset($variables['element']['marcar_todo'], $variables['element']['titulo']);

  foreach (element_children($variables['element']) as $nid) {
    $rows[] = [
      ['data' => drupal_render($variables['element'][$nid]['checkbox']), 'class' => 'checkbox'],
      ['data' => drupal_render($variables['element'][$nid]['area']), 'class' => 'area'],
      ['data' => drupal_render($variables['element'][$nid]['title']), 'class' => 'title'],
      ['data' => drupal_render($variables['element'][$nid]['etiquetas']), 'class' => 'etiqueta'],
      ['data' => drupal_render($variables['element'][$nid]['medio']), 'class' => 'medio'],
      ['data' => drupal_render($variables['element'][$nid]['fecha']), 'class' => 'fecha'],
      ['data' => drupal_render($variables['element'][$nid]['temas']), 'class' => 'tema'],
      ['data' => drupal_render($variables['element'][$nid]['matrices']), 'class' => 'matriz'],
      ['data' => drupal_render($variables['element'][$nid]['actores']), 'class' => 'actor'],
      ['data' => drupal_render($variables['element'][$nid]['usuario']), 'class' => 'usuario'],
      ['data' => drupal_render($variables['element'][$nid]['analista_asignado']), 'class' => 'analista_asignado'],
    ];
  }

  $attributes = [
    'datatable_options' => [
      'bFilter'   => FALSE,
      'bPaginate' => FALSE,
      'order' => [ 1, "asc" ],
      'fixedheader' => TRUE,
      //'offsetTop' => 15,
    ]
  ];

  if (empty($rows)) {
    $table = theme_table([
      'header' => $header,
      'rows' => $rows,
      'sticky' => 0,
      'attributes' => $attributes
    ]);
  }
  else {
    $table = theme_datatable([
      'header' => $header,
      'rows' => $rows,
      'sticky' => 0,
      'attributes' => $attributes,
      'empty' => ' ',
    ]);
  }

  return $titulo . $table;
}


function analistas($todos = NULL) {
  $query = db_select('users', 'u');
  $query->join('users_roles', 'ur', 'ur.uid = u.uid');
  $query->join('role', 'r', 'r.rid = ur.rid');
  $query
    ->condition('r.name', 'segana')
    ->condition('u.status', 1)
    ->fields('u', array('name', 'uid'));
  $resultado = $query->execute();
  $analistas = array();
  //agregar la opcion todos los analistas
  if ($todos) {
    $analistas[-1] = 'Todos';
  }
  foreach($resultado as $row) {
    $analistas[$row->uid] = $row->name;
  }

  return $analistas;
}

?>
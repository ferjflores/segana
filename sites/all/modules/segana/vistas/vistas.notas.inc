<?php
function vistas_notas(){
  drupal_add_css(drupal_get_path('module', 'vistas') . '/css/seguimiento.css', array('group' => CSS_DEFAULT, 'basename' => 'seguimiento.css'));
  drupal_add_js(drupal_get_path('module', 'vistas') . '/js/vistas.notas.js', array('group' => JS_DEFAULT, 'basename' => 'vistas.notas.js'));
  drupal_add_library('system', 'ui.datepicker');

  $header = [
    ['data' => 'Área', 'class' => 'area', 'datatable_options' => ['bSortable' => TRUE]],
    ['data' => 'Título', 'class' => 'title', 'datatable_options' => ['bSortable' => TRUE]],
    ['data' => 'Etiqueta', 'class' => 'etiqueta', 'datatable_options' => ['bSortable' => TRUE]],
    ['data' => 'Medio', 'class' => 'medio', 'datatable_options' => ['bSortable' => TRUE]],
    ['data' => 'Tema', 'class' => 'tema', 'datatable_options' => ['bSortable' => TRUE]],
    ['data' => 'Matriz', 'class' => 'matriz', 'datatable_options' => ['bSortable' => TRUE]],
    ['data' => 'Actor', 'class' => 'actor', 'datatable_options' => ['bSortable' => TRUE]],
    ['data' => 'Usuario', 'class' => 'usuario', 'datatable_options' => ['bSortable' => TRUE]],
    ['data' => 'Fecha', 'class' => 'fecha', 'datatable_options' => ['bSortable' => TRUE]],
    ['data' => 'Fecha Actualización', 'class' => 'fecha_actualizacion', 'datatable_options' => ['bSortable' => FALSE]],
  ];

  $footer = $header;
  //quitar la columna fecha actualización del footer y expandir fecha a 2 columnas en el footer igualmente
  unset ($footer[9]);
  $footer[8]['colspan'] = 2;

  $columnDefs = [
    ['name' => 'ttd_area.name', 'targets' => 0],
    ['name' => 'n.title', 'targets' => 1],
    ['name' => 'ttd_etiqueta.name', 'targets' => 2],
    ['name' => 'ttd_medio.name', 'targets' => 3],
    ['name' => 'ttd_tema.name', 'targets' => 4],
    ['name' => 'ttd_matriz.name', 'targets' => 5],
    ['name' => 'ttd_actor.name', 'targets' => 6],
    ['name' => 'ttd_usuario.name', 'targets' => 7],
    ['name' => 'fecha', 'targets' => 8],
    ['name' => 'ffa.field_fecha_actualizacion_value', 'targets' => 9],
  ];
  $columns = [
    ["data" => "area"],
    ["data" => "titulo"],
    ["data" => "etiqueta"],
    ["data" => "medio"],
    ["data" => "tema"],
    ["data" => "matriz"],
    ["data" => "actor"],
    ["data" => "usuario"],
    ["data" => "fecha"],
    ["data" => "fecha_actualizacion"],

  ];

  //rows

  $variables = [
    'start' => 0,
    'length' => 10,
    'total' => TRUE,
    'total_filtrado' => TRUE,
    'order' => ['campo' => 'fecha', 'direccion' => 'desc'],
    'fecha' => date('d-m-Y'),
  ];


  $notas = vistas_notas_buscar($variables);
  $rows = $notas['data'];


  $attributes = [
    'datatable_options' => [
      'bFilter'   => TRUE,
      'order' => [ 8, "desc" ],
      'fixedheader' => FALSE,
      'processing' => TRUE,
      'serverSide' => TRUE,
      'ajax' => '/seguimiento/json',
      'columnDefs' => $columnDefs,
      'columns' => $columns,
      'deferLoading' => [$notas['recordsFiltered'], $notas['recordsTotal']],
      'individualSearch' => TRUE,
      'searchDelay' => 1000,
      "searchCols" => [null, null, null, null, null, null, null, null, 'search' => $hoy],
    ],
    'class' => ['display'],
    'cellspacing' => 0,
  ];

  $table = theme_datatable([
    'header' => $header,
    'rows' => $rows,
    'sticky' => 0,
    'attributes' => $attributes,
    'empty' => ' ',
    'footer' => $footer,
  ]);

  return $table;
}


function vistas_notas_json(){
  //ed($_GET);

  //order
  if (isset($_GET['order'][0][column])){
    $order_column = $_GET['order'][0][column];
    $order = [
      'campo' => $_GET['columns'][$order_column]['name'],
      'direccion' => $_GET['order'][0]['dir'],
    ];
  }

  $variables = [
    'start' => ($_GET['start']) ? $_GET['start'] : 0,
    'length' => ($_GET['length']) ? $_GET['length'] : 10,
    'total' => TRUE,
    'total_filtrado' => TRUE,
    'order' =>  isset($order) ? $order : NULL,
  ];

  //obtener la busqueda por columna
  foreach($_GET['columns'] as $key => $values) {
    if (!empty($values['search']['value'])) {
      $name = $values['name'];
      $variables[$name] = $values['search']['value'];
    }
  }


  $notas = vistas_notas_buscar($variables);
  //$notas['draw'] = 1;

  drupal_json_output($notas);
}

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
function vistas_notas_buscar($variables = array()) {
  $query = db_select('node', 'n');
  $query->join('users', 'u', 'n.uid = u.uid');
  $query->leftJoin('field_data_field_area', 'fa', 'n.nid = fa.entity_id AND fa.bundle = :bundle', array(':bundle' =>
      'nota'));
  $query->leftJoin('field_data_field_fecha', 'ff', 'ff.entity_id = n.nid AND ff.bundle = :bundle', array(':bundle' => 'nota'));
  $query->leftJoin('field_data_field_fecha_original', 'ffo', 'ffo.entity_id = n.nid AND ffo.bundle = :bundle', array(':bundle' => 'nota'));
  $query->leftJoin('field_data_field_fecha_actualizacion', 'ffa', 'ffa.entity_id = n.nid AND ffa.bundle = :bundle', array(':bundle' => 'nota'));
  $query->leftJoin('field_data_field_medio', 'fme', 'fme.entity_id = n.nid AND fme.bundle = :bundle', array(':bundle' => 'nota'));
  $query->leftJoin('field_data_field_etiqueta', 'fe', 'n.nid = fe.entity_id');
  $query->leftJoin('taxonomy_term_data', 'ttd_area', 'fa.field_area_tid = ttd_area.tid');
  $query->leftJoin('taxonomy_term_data', 'ttd_medio', 'fme.field_medio_tid = ttd_medio.tid');
  $query->leftJoin('taxonomy_term_data', 'ttd_etiqueta', 'fe.field_etiqueta_tid = ttd_etiqueta.tid');
  $query->leftJoin('field_data_field_usuario_actualizacion', 'fua', 'fua.entity_id = n.nid');
  $query->leftJoin('users', 'uua', 'fua.field_usuario_actualizacion_target_id = uua.uid');
  $query
    ->fields('ttd_area', array('name'))
    ->fields('n', array('nid', 'title'))
    ->fields('ttd_medio', array('name'))
    ->fields('u', array('name'))
    ->fields('ffa', array('field_fecha_actualizacion_value'))
    ->groupBy('n.nid');
  $query->addExpression("GROUP_CONCAT(DISTINCT ttd_tema.name ORDER BY ft.delta separator ',<br>')", 'temas');
  $query->addExpression("GROUP_CONCAT(DISTINCT ttd_matriz.name ORDER BY fm.delta separator ',<br>')", 'matrices');
  $query->addExpression("GROUP_CONCAT(DISTINCT ttd_actor.name ORDER BY fac.delta separator ',<br>')", 'actores');
  $query->addExpression("GROUP_CONCAT(DISTINCT ttd_usuario.name ORDER BY fu.delta separator ',<br>')", 'usuarios');
  $query->addExpression("GROUP_CONCAT(DISTINCT ttd_etiqueta.name ORDER BY fe.delta separator ',<br>')", 'etiquetas');
  $query->addExpression("COALESCE(ffo.field_fecha_original_value, ff.field_fecha_value)", 'fecha');
  if (!empty($variables['ttd_area.name'])) {
    $query->condition('ttd_area.name', '%'. db_like($variables['ttd_area.name']) .'%', 'LIKE');
  }
  if (!empty($variables['n.title'])) {
    $query->condition('n.title', '%'. db_like($variables['n.title']) .'%', 'LIKE');
  }
  if (!empty($variables['ttd_medio.name'])) {
    $query->condition('ttd_medio.name', $variables['ttd_medio.name']);
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
  elseif(isset($variables['fecha'])) {
    $query->condition('ff.field_fecha_value', strtotime($variables['fecha']));
  }
 else {
    //$query->condition('ff.field_fecha_value', strtotime(date('d-m-Y')));
  }

  if (!empty($variables['order'])) {
    $query->orderBy($variables['order']['campo'], $variables['order']['direccion']);
  }
  else {
    $query->orderBy('fecha', 'DESC');
  }

  $query->range($variables['start'], $variables['length']);
  $resultado = $query->execute();
  $count_query = $resultado->rowCount();

  $notas = [];

  if ($variables['total_filtrado']) {
    $query_total_filtrado = clone $query;
    //reiniciar limit
    $range = $query_total_filtrado->range();
    unset($range);
    $total_filtrado = $query_total_filtrado->execute()->rowCount();
    $notas['recordsFiltered'] = $total_filtrado;
  }

  if ($variables['total']) {
    $total = db_select('node', 'n')
      ->fields('n', array('nid'))
      ->execute()
      ->rowCount();
    $notas['recordsTotal'] = $total;
  }
  if ($count_query > 0) {
    foreach ($resultado as $row) {
      $user = ($row->u_name) ? $row->u_name : "Sin usuario";

      $notas['data'][] = [
        $row->name,
        $row->title,
        !is_null($row->etiquetas) ? $row->etiquetas : '',
        $row->ttd_medio_name,
        !is_null($row->temas) ? $row->temas : '',
        !is_null($row->matrices) ? $row->matrices : '',
        !is_null($row->actores) ? $row->actores : '',
        !is_null($row->usuarios) ? $row->usuarios : '',
        isset($row->fecha) ? date('d-m-Y h:i a', $row->fecha)."<br>(".$user.")" : NULL,
        !is_null($row->field_fecha_actualizacion_value) ?
            $row->field_fecha_actualizacion_value : '',
      ];
    }
    return $notas;
  }
  else {
    return $notas;
  }
}



?>
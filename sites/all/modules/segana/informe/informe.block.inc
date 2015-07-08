<?php

/**
 * Implements hook_block_info().
 *
 * This hook declares what blocks are provided by the module.
 */
function informe_block_info() {
  $blocks['ultimos_temas'] = array(
    'info' => t('Últimos temas'),
    'status' => TRUE,
    'region' => 'sidebar_second',
    'visibility' => BLOCK_VISIBILITY_LISTED,
    'pages' => "agenda_informativa\nagenda_informativa/*",
    'weight' => 1,
  );
  $blocks['agenda_informativa'] = array(
    'info' => t('Últimas informes'),
    'status' => TRUE,
    'region' => 'sidebar_second',
    'visibility' => BLOCK_VISIBILITY_LISTED,
    'pages' => "agenda_informativa\nagenda_informativa/*",
    'weight' => 2,
  );
  $blocks['tematica'] = array(
    'info' => t('Últimas informes'),
    'status' => TRUE,
    'region' => 'sidebar_second',
    'visibility' => BLOCK_VISIBILITY_LISTED,
    'pages' => "tematica\ntematica/*",
    'weight' => 2,
  );
  return $blocks;
}


/**
 * Implements hook_block_view().
 *
 * This hook generates the contents of the blocks themselves.
 */
function informe_block_view($delta = '') {
  // The $delta parameter tells us which block is being requested.
  switch ($delta) {
    case 'ultimos_temas':
      $block['subject'] = t('Últimos temas');
      $block['content'] = block_informe($delta);
      break;
    case 'agenda_informativa':
      $block['subject'] = t('Últimos informes');
      $block['content'] = block_informe($delta);
      break;
    case 'tematica':
      $block['subject'] = t('Últimos informes');
      $block['content'] = block_informe($delta);
      break;
  }
  return $block;
}



function block_informe($delta = '') {
  switch ($delta) {
    case 'ultimos_temas':
      $pagina = 1;
      $time_since = strtotime(date('Y-m-d'));
      //select count(field_tema_tid) as cnt, name from node join field_data_field_tema on (node.nid = field_data_field_tema.entity_id) JOIN taxonomy_term_data ON (field_tema_tid = tid) where created > 1417494600 AND bundle = 'nota' group by field_data_field_tema.field_tema_tid order by cnt DESC, created DESC;
      $query = db_select('node', 'n');
      $query->join('field_data_field_tema', 'ft', 'n.nid = ft.entity_id');
      $query->join('taxonomy_term_data', 'ttd', 'ft.field_tema_tid = ttd.tid');
      $query->join('field_data_field_fecha', 'ff', 'ff.entity_id = n.nid');
      $query->addExpression('COUNT(ft.field_tema_tid)', 'tid_count');
      $query
      ->fields('ttd', array('name', 'tid'))
      ->condition('ff.field_fecha_value', $time_since, '>=')
      ->condition('ft.bundle', 'nota')
      ->groupBy('ft.field_tema_tid')
      ->orderBy('tid_count', 'DESC')
      ->orderBy('n.created', 'DESC')
      ->range((($pagina*20) - 20),20);

      $resultado = $query->execute();
      $rows = array();
      foreach ($resultado as $row) {
        //temas[] = $row->name . "(" . $row->tid_count . ")";
        $rows[] = array($row->tid_count, l($row->name, 'elementos_discursivos/ver/' . $row->tid, array('html' => TRUE, 'attributes' => array('class' => 'colorbox-load'), 'query' => array('width' => '750')) ) );
      }
    
      $tabla = array();
      $tabla['rows'] = $rows;
      $tabla['attributes'] = array('id' => 'tabla_temas');

      $output = theme_table($tabla);

    //$output = theme_item_list(array('items' => $temas, 'type' => 'ul', 'attributes' => array('class' => 'ultimos_temas'), 'title' => ''));
      break;
    case 'agenda_informativa':
    case 'tematica':
      //$time_since = time() - 3600*24*30;
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'informe')
        ->entityCondition('bundle', $delta)
        //->propertyCondition('created', $time_since, '>')
        ->range(0,5);
      $query->propertyOrderBy('created', 'DESC');
      $resultado = $query->execute();

      if (isset($resultado['informe'])) {
        $informes_iids = array_keys($resultado['informe']);
        $informes = entity_load('informe', $informes_iids);
      }

      $row = 0;
      $output = NULL;
      foreach ($informes as $iid => $value) {

        $usuario = user_load($value->uid);
        $language = LANGUAGE_NONE;

        $temas = array();
        foreach ($value->field_tema[$language] as $delta_tema => $tema) {
          $tema_term = taxonomy_term_load($tema['tid']);

          $temas[$tema['tid']] = $tema_term->name;

        }
        $temas = theme_item_list(array('items' => $temas, 'type' => 'ol', 'attributes' => array('class' => 'lista_temas'), 'title' => ''));

        $output .= theme('html_tag', array(
          'element' => array(
            '#tag' => 'div',
            '#attributes' => array(
              'id' => 'informe',
              'class' => ($row & 1) ? 'odd' : 'even',
              ),
            '#value' => l($value->title . ' (' . $usuario->name . ')', $delta . '/' . $iid) . $temas,
          ),
        ));
        $row++;
      }
      break;
  }
return $output;
}

?>
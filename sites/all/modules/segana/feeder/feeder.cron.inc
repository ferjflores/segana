<?php
/*
 * Implements hook_cron().
 */
function feeder_cron(){
  $logger = klogger_log();
  $logger->info("Iniciando corrida del cron");
  if (drupal_is_cli() && function_exists('drush_main')) {
    //durante las 5am y 8am se reduce el tiempo entre actualizaciones
    if (date('H') >= 5 and date('H') <= 9) {
      $TiempoMinActualizacion = 500;
    } elseif ((date('H') >= 20 and date('H') <= 23) || (date('H') >= 0 and date('H') < 5)) {
      $TiempoMinActualizacion = 3600;
    } else {
      $TiempoMinActualizacion = 1800;
    }
    //$TiempoMinActualizacion = 40000;
    $query = db_select('field_data_field_actualizacion_automatica', 'faa');
    $query->leftJoin('field_data_field_ultima_actualizacion', 'fua', 'faa.entity_id = fua.entity_id');
    $query->join('field_data_field_variables_medio', 'fvm', 'faa.entity_id = fvm.entity_id');
    $query
        ->condition('faa.field_actualizacion_automatica_value', 1)
        ->fields('faa', array('entity_id'))
        ->fields('fua', array('field_ultima_actualizacion_value'))
        ->fields('fvm', array('field_variables_medio_value'))
        ->orderBy('fua.field_ultima_actualizacion_value', 'ASC');

    $resultado = $query->execute();

    $queue_rss = DrupalQueue::get('feed_rss');
    $queue_seccion = DrupalQueue::get('feed_seccion');
    foreach ($resultado as $row) {
      $ultima_actualizacion = isset($row->field_ultima_actualizacion_value) ? $row->field_ultima_actualizacion_value : NULL;
      if (($ultima_actualizacion + $TiempoMinActualizacion) <= time()) {
        //verificar que no este agregado al queue
        $resultado_queue = db_select('queue', 'q')
            ->condition('q.data', serialize($row->entity_id))
            ->fields('q', array('data'))
            ->execute();

        if ($resultado_queue->rowCount() > 0) {
          $logger->notice($row->entity_id . " ya se encuentra en el queue");
        } else {
          //determinar si es por rss o por seccion web
          $variables = unserialize($row->field_variables_medio_value);
          if (!empty($variables['canal_rss'])) {
            $queue_rss->createItem($row->entity_id);
          } else {
            $queue_seccion->createItem($row->entity_id);
          }
          $logger->info("agregado al queue " . $row->entity_id);
        }
      } else {
        $logger->info($row->entity_id . " actualizado a las " . date('d-m-Y H:i:s', $ultima_actualizacion));
      }
    }

    //limpiar imagenes huerfanaas
    //SELECT fm.* FROM file_managed AS fm LEFT OUTER JOIN field_data_field_imagen AS fi ON (fm.fid = fi.field_imagen_fid) WHERE fi.field_imagen_fid IS NULL;
  }
  $logger->info("Finalizada corrida del cron");
}

/*
 * Implements hook_cron_queue_info().
 */
function feeder_cron_queue_info(){
  $queues['feed_rss'] = array(
    'worker callback' => 'feeder_rss',
    'time' => 300,
    'reliable' => TRUE,
  );
  $queues['feed_seccion'] = array(
    'worker callback' => 'feeder_seccion',
    'time' => 300,
    'reliable' => TRUE,
  );
  return $queues;
}
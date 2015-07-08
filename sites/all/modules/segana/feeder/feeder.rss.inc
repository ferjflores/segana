<?php
use PicoFeed\Reader\Reader;

function feeder_rss($tid) {
  feeder($tid, 'rss');
}

function feeder_rss_medio($tid) {
  $term = taxonomy_term_load($tid);
  $variables = unserialize($term->field_variables_medio[LANGUAGE_NONE][0]['value']);
  $canales_rss = preg_split('/\r\n|[\r\n]/', $variables['canal_rss']);
  $tid_domain = str_ireplace('www.', '', parse_url($term->field_pagina_web[LANGUAGE_NONE][0]['value'], PHP_URL_HOST));
  if (empty($tid_domain)) {
    $tid_domain = str_ireplace('www.', '', trim($term->field_pagina_web[LANGUAGE_NONE][0]['value']));
  }

  foreach ($canales_rss as $key => $area_canal_rss) {
    $area_canal_rss = explode('|', $area_canal_rss);
    $canales_rss[$key] = array('area' => $area_canal_rss[0], 'rss_url' => $area_canal_rss[1]);
  }

  $nids = array();
  foreach ($canales_rss as $crID => $values) {
    /*if ($crID > 0) {
      break;
    }*/
    //evitar hacer tantas peticiones seguidas, esperar 30 segundos
    if ($crID > 0) {
      sleep(20);
    }

    if (isset($variables['cache_rss'][$crID])) {
      $last_modified = $variables['cache_rss'][$crID]['last_modified'];
      $etag = $variables['cache_rss'][$crID]['etag'];
    }
    else {
      $last_modified = NULL;
      $etag = NULL;
    }
    $logger = klogger_log();
    $logger->info("Medio:". $term->name ." ID:".$crID." URL:  ".$values['rss_url']);
    $logger->info("Medio:". $term->name ." ID:".$crID." Almacenado:  etag: " .$etag ."   lm: " .$last_modified);

    $config = feeder_config();
    try {
      $reader = new Reader($config);
      $resource = $reader->download($values['rss_url'], $last_modified, $etag);

      if ($resource->isModified()) {
        $parser = $reader->getParser(
          $resource->getUrl(),
          $resource->getContent(),
          $resource->getEncoding()
        );

        $parser->enableContentGrabber();
        $feed = $parser->execute(MAX_TIME_NOTA, 1);
        $etag = $resource->getEtag();
        $last_modified = $resource->getLastModified();
        $logger->info("Medio:". $term->name ." ID:".$crID." Nuevo:  etag: " .$etag ."   lm: " .$last_modified);

        //almacenar etag y last_modified
        $variables['cache_rss'][$crID]['last_modified'] = $last_modified;
        $variables['cache_rss'][$crID]['etag'] = $etag;

        $term->field_variables_medio[LANGUAGE_NONE][0]['value'] = serialize($variables);
        taxonomy_term_save($term);

        feeder_rss_canal($tid, $term->name, $feed, $values, $tid_domain, $variables, $nids);
      }
      else {
        $logger->notice("Medio:". $term->name ." RSS $crID no modificado");
      }
    }
    catch (Exception $e) {
      $logger->error("Medio:". $term->name ." ID:".$crID, $e->getMessage());
    }
  }
  $term->field_ultima_actualizacion[LANGUAGE_NONE][0]['value'] = time();
  taxonomy_term_save($term);
  $logger->info("Medio: ". $term->name ." Notas insertadas", $nids);
}


function feeder_rss_canal($tid, $medio_name, $feed, $canal_rss, $domain, $variables, &$nids) {
  $area_tid = key(taxonomy_get_term_by_name($canal_rss['area'], 'area'));
  $notas = array();
  $logger = klogger_log();

  foreach ($feed->items as $id => $items) {
    /*if (count($notas) > 0) {
      break;
    }*/

    // incluir en el calculo del id la fecha, para saber si han actualizado la fecha
    $ItemId = $feed->items[$id]->getId();
    $fecha_rss = $feed->items[$id]->getDate()->getTimestamp();

    $pubDate = strtotime(traducirFecha($feed->items[$id]->getTag('pubDate')));
    if ($fecha_rss != $pubDate) {
      $fecha_rss = $pubDate;
    }

    $nota = array(
      'category' => $feed->items[$id]->getTag('category'),
      'titulo' => $feed->items[$id]->getTitle(),
      'url' => $feed->items[$id]->getUrl(),
      'fecha_rss' => $fecha_rss,
      'sumario_rss' => $feed->items[$id]->getTag('description'),
    );

    //rechazar la nota si viene de otro dominio
    $url_domain = str_ireplace('www.', '', parse_url($nota['url'], PHP_URL_HOST));
    if (strpos($url_domain, $domain) === FALSE) {
      $logger->error("Medio:". $medio_name ." Nota de otro dominio $url_domain diferente al del medio $domain  ".$nota['url']);
      continue;
    }

    $contenido = $feed->items[$id]->getContent();
    if ($contenido) {
      foreach ($variables['elementos'] as $key => $elemento) {
        $nota[$key] = feeder_extraer($contenido, $elemento);
      }

      //extraer tweets embebidos
      $contenidoRaw = $feed->items[$id]->rawContent;
      $elemento = 'blockquote.twitter-tweet a,*,href,,,,,,,status';
      $nota['tweets'] = feeder_extraer($contenidoRaw, $elemento);

      $nota['fecha'] = strtotime(traducirFecha($nota['fecha']));

      if (!empty($nota['fecha_rss']) && ($nota['fecha_rss'] > $nota['fecha'])) {
        $fecha = $nota['fecha_rss'];
      }
      else {
        $fecha = $nota['fecha'];
      }
      $NotaId = crc32($ItemId . $fecha);
      //chequear que esta nota no ha sido introducida
      $query = db_select('field_data_field_autoid', 'fai')
        ->condition('field_autoid_value', $NotaId)
        ->fields('fai', array('field_autoid_value'));
      $resultado = $query->execute();
      $count_query = $resultado->rowCount();
      if ($count_query > 0) {
        $logger->notice("Medio:". $medio_name ." Esta nota ya existe ".$NotaId." ".$nota['url']);
        continue;
      }

      $logger->info("Medio:". $medio_name ." Nota enviada para insercción ".$NotaId." ".$nota['url']);

      $nota = array_map('sanitizar', $nota);
      $notas[$NotaId] = $nota;
    }
    else {
      $logger->error("Medio:". $medio_name ." Fallo la descarga del contenido ".$nota['url']);
    }
  }

  if (!empty($notas)) {
    $nids = array_merge($nids, feeder_notas($notas, $tid, $area_tid, $medio_name));
  }
}

?>
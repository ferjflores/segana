<?php

function feeder_seccion($tid) {
  feeder($tid, 'seccion');
}


function feeder_seccion_medio($tid) {
  $term = taxonomy_term_load($tid);
  $variables = unserialize($term->field_variables_medio[LANGUAGE_NONE][0]['value']);
  $canales_seccion = preg_split('/\r\n|[\r\n]/', $variables['canal_seccion']);
  $domain = str_ireplace('www.', '', parse_url($term->field_pagina_web[LANGUAGE_NONE][0]['value'], PHP_URL_HOST));
  if (empty($domain)) {
    $domain = str_ireplace('www.', '', trim($term->field_pagina_web[LANGUAGE_NONE][0]['value']));
  }

  foreach ($canales_seccion as $key => $area_canal_seccion) {
    $area_canal_seccion = explode('|', $area_canal_seccion);
    $canales_seccion[$key] = array('area' => $area_canal_seccion[0], 'seccion_url' => $area_canal_seccion[1]);
  }

  $notasLeidas = 0;
  $nids = array();

  foreach ($canales_seccion as $csID => $values) {
    /*if ($csID > 0) {
      break;
    }*/
    $logger = klogger_log();
    $logger->info("Medio:". $term->name ." ID:".$csID." url:  ".$values['seccion_url']);

    $contenido_seccion = feeder_url($values['seccion_url'], FALSE, 'seccion');

    if (!empty($contenido_seccion)) {
      feeder_seccion_canal($tid, $term->name, $contenido_seccion, $values, $domain, $variables, $nids, $notasLeidas);
    }
    else {
      $logger->error("Medio:". $term->name . " Fallo la descarga del contenido de id: $csID ". $values['seccion_url']);
    }
  }
  $term->field_ultima_actualizacion[LANGUAGE_NONE][0]['value'] = time();
  taxonomy_term_save($term);
  $logger->info("Medio: ". $term->name ." Notas insertadas", $nids);
}


function feeder_seccion_canal($tid, $medio_name, $contenido_seccion, $canal_seccion, $domain, $variables, &$nids,
                              &$notasLeidas) {
  $logger = klogger_log();
  $items_seccion = array();
  foreach ($variables['elementos_seccion'] as $key => $elemento) {
    $items_seccion[$key] = preg_split('/\r\n|[\r\n]/', feeder_extraer($contenido_seccion, $elemento));
  }

  $items = array();
  for ($i = 0; $i < count($items_seccion['titulo']); $i++) {
    foreach ($items_seccion as $key => $elemento) {
      $items[$i][$key] = $items_seccion[$key][$i];
    }
  }
  $area_tid = key(taxonomy_get_term_by_name($canal_seccion['area'], 'area'));
  $notas = array();

  if (!empty($items)) {
    foreach ($items as $id => $item) {
      /*if (count($notas) > 0) {
        break;
      }*/
      //pausar si el numero de notas leidas sobrepasa 20
      if ($notasLeidas > 20) {
        $logger->info("Medio:". $medio_name ." Proceso pausado por 60 segundos");
        sleep(60);
        $notasLeidas = 0;
      }

      $nota = array_map('trim', $items[$id]);
      $NotaId = crc32(serialize($nota));


      //tiempo maximo para la fecha de la nota
      if (isset($nota['fecha_seccion']) && !empty($nota['fecha_seccion'])) {
        $nota['fecha_seccion'] = strtotime(traducirFecha($nota['fecha_seccion']));
        if (is_int($nota['fecha_seccion']) && ($nota['fecha_seccion'] + MAX_TIME_NOTA < time())) {
          continue;
        }
      }

      //rechazar la nota si viene de otro dominio
      $url_domain = str_ireplace('www.', '', parse_url($nota['url'], PHP_URL_HOST));
      if (strpos($url_domain, $domain) === FALSE) {
        $logger->error("Medio:". $medio_name ." Nota de otro dominio $url_domain diferente al del medio $domain  ". $nota['url']);
        continue;
      }

      //chequear que esta nota no ha sido introducida
      $query = db_select('field_data_field_autoid', 'fai')
          ->condition('field_autoid_value', $NotaId)
          ->fields('fai', array('field_autoid_value'));
      $resultado = $query->execute();
      $count_query = $resultado->rowCount();
      if ($count_query > 0) {
        $logger->info("Medio:". $medio_name ." Esta nota ya existe " . $NotaId ." ". $nota['url']);
        continue;
      }

      //con el medio diariodelosandes el url viene con html entities, igualmente podria pasar con otros
      $nota['url'] = html_entity_decode($nota['url']);

      $contenidoRawAndFiltered = feeder_url($nota['url'], FALSE, 'contenido', TRUE);
      $contenido = $contenidoRawAndFiltered['filtered'];
      $notasLeidas++;

      if ($contenido) {
        foreach ($variables['elementos'] as $key => $elemento) {
          $nota[$key] = feeder_extraer($contenido, $elemento);
        }

        //extraer tweets embebidos
        $elemento = 'blockquote.twitter-tweet a,*,href,,,,,,,status';
        $nota['tweets'] = feeder_extraer($contenidoRawAndFiltered['raw'], $elemento);

        $nota['fecha'] = strtotime(traducirFecha($nota['fecha']));

        $logger->info("Medio:". $medio_name ." Nota enviada para inserción ".$NotaId." ".$nota['url']);

        $nota = array_map('sanitizar', $nota);
        $notas[$NotaId] = $nota;
      }
      else {
        $logger->error("Medio:". $medio_name ." Fallo la descarga del contenido ".$nota['url']);
      }
    }
  }

  if (!empty($notas)) {
    $nids = array_merge($nids, feeder_notas($notas, $tid, $area_tid, $medio_name));
  }
}

?>
<?php

function vista_notas($node) {
	$titulo = $node->title;
	$language = LANGUAGE_NONE;
	$cuerpo_texto = $node->body[$language][0]['value'];
	$uid = $node->uid;
	$tipo_informacion_tid = $node->field_tipo_informacion[$language][0]['tid'];
	$area_tid = $node->field_area[$language][0]['tid'];
	
	$medio_tid = $node->field_medio[$language][0]['tid'];
	$medio_term = taxonomy_term_load($medio_tid);
	$medio = $medio_term->name;
	$localizacion_term = taxonomy_term_load($medio_term->field_localizacion[$language][0]['tid']);
	$localizacion = $localizacion_term->name;
	
	$autor = $node->field_autor[$language][0]['value'];
	$fecha = $node->field_fecha[$language][0]['value'];

	$tema = array();
	foreach ($node->field_tema[$language] as $delta => $tid) {
		$term = taxonomy_term_load($tid['tid']);
		$name = $term->name;
		$tema[$delta] = $name;
	}

	$matriz = array();
	foreach ($node->field_matriz[$language] as $delta => $tid) {
		$term = taxonomy_term_load($tid['tid']);
		$name = $term->name;
		$matriz[$delta] = $name;
	}

	$actor = array();
	foreach ($node->field_actor[$language] as $delta => $tid) {
		$term = taxonomy_term_load($tid['tid']);
		$name = $term->name;
		$actor[$delta] = $name;
	}

	$cuerpo_argumentativo = array();
	foreach ($node->field_cuerpo_argumentativo[$language] as $delta => $value) {
		$cuerpo_argumentativo[$delta] = $value['value'];
	}

	$elementos_discursivos = array();
	for ($i=0; (isset($tema[$i]) || isset($matriz[$i]) || isset($actor[$i])) ; $i++) {
		$elementos_discursivos[$i] = array(
			'data' => array(
				'tema' => array('data' => (isset($tema[$i]) ? $tema[$i] : NULL), 'class' => 'tema'),
				'matriz' => array('data' => (isset($matriz[$i]) ? $matriz[$i] : NULL), 'class' => 'matriz'),
				'cuerpo_argumentativo' => array('data' => (isset($cuerpo_argumentativo[$i]) ? $cuerpo_argumentativo[$i] : NULL), 'class' => 'cuerpo_argumentativo'),
				'actor' => array('data' => (isset($actor[$i]) ? $actor[$i] : NULL), 'class' => 'actor')
			),
		);
	}

	$find[] = 'â€œ'; // left side double smart quote
	$find[] = 'â€'; // right side double smart quote
	$find[] = 'â€˜'; // left side single smart quote
	$find[] = 'â€™'; // right side single smart quote
	$find[] = 'â€¦'; // elipsis
	$find[] = 'â€”'; // em dash
	$find[] = 'â€“'; // en dash

	$replace[] = '"';
	$replace[] = '"';
	$replace[] = "'";
	$replace[] = "'";
	$replace[] = "...";
	$replace[] = "-";
	$replace[] = "-";

	$cuerpo_texto = str_replace($find, $replace, $cuerpo_texto);

	$titulo = theme('html_tag', array(
	  'element' => array(
	    '#tag' => 'h2',
	    '#attributes' => array(
	      'id' => 'titulo',
	     ),
	    '#value' => $titulo,
	  ),
	));
	$medio = theme('html_tag', array(
	  'element' => array(
	    '#tag' => 'span',
	    '#attributes' => array(
	      'id' => 'medio',
	     ),
	    '#value' => $medio . " (" . $localizacion . ")"
	  ),
	));
	$fecha_medio = theme('html_tag', array(
	  'element' => array(
	    '#tag' => 'div',
	    '#attributes' => array(
	      'id' => 'fecha_medio',
	     ),
	    '#value' => date('d/m/Y', $fecha) . $medio
	  ),
	));
	$cuerpo_texto = theme('html_tag', array(
	  'element' => array(
	    '#tag' => 'div',
	    '#attributes' => array(
	      'id' => 'cuerpo_texto',
	     ),
	    '#value' => $cuerpo_texto,
	  ),
	));
	$autor = theme('html_tag', array(
	  'element' => array(
	    '#tag' => 'div',
	    '#attributes' => array(
	      'id' => 'autor',
	     ),
	    '#value' => $autor,
	  ),
	));

	$header = array(
		'tema' => array('data' => 'Tema', 'class' => 'tema'),
		'matriz' => array('data' => 'Matriz', 'class' => 'matriz'),
		'cuerpo_argumentativo' => array('data' => 'Cuerpo Argumentativo', 'class' => 'cuerpo_argumentativo'),
		'actor' => array('data' => 'Actor', 'class' => 'actor')
	);

	$elementos_discursivos = theme('table', array('header' => $header, 'rows' => $elementos_discursivos, 'attributes' => array('id' => 'elementos_discursivos')));
	$vista_notas = theme('html_tag', array(
	  'element' => array(
	    '#tag' => 'div',
	    '#attributes' => array(
	      'id' => 'vista_notas',
	     ),
	    '#value' => $titulo . $fecha_medio . $cuerpo_texto . $autor . $elementos_discursivos
	  ),
	));


	print $vista_notas;
	/*print "<pre>";
	print_r($node);
	print "</pre>";*/
}


function vista_elementos_discursivos($tid){
	$term = taxonomy_term_load($tid);
	$nombre = $term->name;
	$vid = $term->vid;
	$vocabulary = taxonomy_vocabulary_load($vid);
	$descripcion = $term->description;
	$fecha = $term->field_fecha[LANGUAGE_NONE][0]['value'];
  $fecha_actualizacion = $term->field_fecha_actualizacion[LANGUAGE_NONE][0]['value'];
  $ultima_actualizacion = $term->field_ultima_actualizacion[LANGUAGE_NONE][0]['value'];

	$tipo = theme('html_tag', array(
	  'element' => array(
	    '#tag' => 'h2',
	    '#attributes' => array(
	      'id' => 'tipo',
	     ),
	    '#value' => $vocabulary->name,
	  ),
	));

	$nombre = theme('html_tag', array(
	  'element' => array(
	    '#tag' => 'div',
	    '#attributes' => array(
	      'id' => 'nombre',
	     ),
	    '#value' => $nombre,
	  ),
	));

	$fecha = theme('html_tag', array(
	  'element' => array(
	    '#tag' => 'div',
	    '#attributes' => array(
	      'id' => 'fecha',
	     ),
	    '#value' => 'Fecha creacion: ' . date('d/m/Y', $fecha),
	  ),
	));

	$fecha_actualizacion = theme('html_tag', array(
	  'element' => array(
	    '#tag' => 'div',
	    '#attributes' => array(
	      'id' => 'fecha',
	     ),
	    '#value' => 'Fecha actualización: ' . date('d/m/Y', $fecha_actualizacion),
	  ),
	));

  $ultima_actualizacion = theme('html_tag', array(
    'element' => array(
      '#tag' => 'div',
      '#attributes' => array(
        'id' => 'fecha',
      ),
      '#value' => 'Última actualización de las notas de este medio: ' . date('d/m/Y h:i a', $ultima_actualizacion),
    ),
  ));

	$descripcion = theme('html_tag', array(
	  'element' => array(
	    '#tag' => 'div',
	    '#attributes' => array(
	      'id' => 'descripcion',
	     ),
	    '#value' => $descripcion,
	  ),
	));

	$nids_count = taxonomy_field_nodes($tid, $vocabulary->name);
	$plural = ($nids_count > 1 ? 's' : '');
	$link = l($nids_count,'notas', array('attributes' => array('class' => 'ver', 'target'=>'_blank', 'title' => 'Ver notas asociadas'), 'query' => array($vocabulary->machine_name => $term->name)));


	$link_image = l(theme_image(array('path' => '/sites/all/themes/segana/images/listado.png')),'notas', array('attributes' => array('class' => 'ver',  'target'=>'_blank', 'title' => 'Ver notas asociadas'), 'query' => array($vocabulary->machine_name => $term->name), 'html' => TRUE));

	$notas_asociadas = theme('html_tag', array(
    'element' => array(
      '#tag' => 'div',
      '#attributes' => array(
        'id' => 'notas_asociadas',
       ),
      '#value' => 'Hay '. $link . ' nota' . $plural . ' asociada' . $plural . (($vocabulary->machine_name == 'matriz') ? ' a la ' : ' al ') . $vocabulary->machine_name . $link_image,
    ),
  ));

	$vista_elementos_discursivos = theme('html_tag', array(
	  'element' => array(
	    '#tag' => 'div',
	    '#attributes' => array(
	      'id' => 'vista_elementos_discursivos',
	     ),
	    '#value' => $tipo . $nombre . $notas_asociadas . $fecha . $fecha_actualizacion . $ultima_actualizacion . $descripcion
	  ),
	));

	print $vista_elementos_discursivos;
}

?>
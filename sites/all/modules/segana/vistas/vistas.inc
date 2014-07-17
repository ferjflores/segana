<?php

function vista_notas($nid) {
	$node = node_load($nid);
	$titulo = $node->title;
	$cuerpo_texto = $node->body[$node->language][0]['value'];
	$uid = $node->uid;
	$tipo_informacion_tid = $node->field_tipo_informacion[$node->language][0]['tid'];
	$area_tid = $node->field_area[$node->language][0]['tid'];
	
	$medio_tid = $node->field_medio[$node->language][0]['tid'];
	$medio_term = taxonomy_term_load($medio_tid);
	$medio = $medio_term->name;
	$localizacion_term = taxonomy_term_load($medio_term->field_localizacion[LANGUAGE_NONE][0]['tid']);
	$localizacion = $localizacion_term->name;
	
	$autor = $node->field_autor[$node->language][0]['value'];
	$fecha = $node->field_fecha[$node->language][0]['value'];

	$tema = array();
	foreach ($node->field_tema[$node->language] as $delta => $tid) {
		$term = taxonomy_term_load($tid['tid']);
		$name = $term->name;
		$tema[$delta] = $name;
	}

	$matriz = array();
	foreach ($node->field_matriz[$node->language] as $delta => $tid) {
		$term = taxonomy_term_load($tid['tid']);
		$name = $term->name;
		$matriz[$delta] = $name;
	}

	$actor = array();
	foreach ($node->field_actor[$node->language] as $delta => $tid) {
		$term = taxonomy_term_load($tid['tid']);
		$name = $term->name;
		$actor[$delta] = $name;
	}

	$cuerpo_argumentativo = array();
	foreach ($node->field_cuerpo_argumentativo[$node->language] as $delta => $value) {
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
	$tipo_vocabulary = taxonomy_vocabulary_load($vid);
	$tipo = $tipo_vocabulary->name;
	$descripcion = $term->description;
	$fecha = $term->field_fecha[LANGUAGE_NONE][0]['value'];
	$fecha_actualizacion = $term->field_fecha_actualizacion[LANGUAGE_NONE][0]['value'];

	$tipo = theme('html_tag', array(
	  'element' => array(
	    '#tag' => 'h2',
	    '#attributes' => array(
	      'id' => 'tipo',
	     ),
	    '#value' => $tipo,
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

	$descripcion = theme('html_tag', array(
	  'element' => array(
	    '#tag' => 'div',
	    '#attributes' => array(
	      'id' => 'descripcion',
	     ),
	    '#value' => $descripcion,
	  ),
	));

	$vista_elementos_discursivos = theme('html_tag', array(
	  'element' => array(
	    '#tag' => 'div',
	    '#attributes' => array(
	      'id' => 'vista_elementos_discursivos',
	     ),
	    '#value' => $tipo . $nombre . $fecha . $fecha_actualizacion .$descripcion
	  ),
	));

	print $vista_elementos_discursivos;
}

?>
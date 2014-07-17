<?php

function autocomplete_busqueda_area($busqueda='') {
	$matches = array();
	$tipo = 'area';

	if (trim($busqueda)) {
		$busqueda_elementos = str_word_count($busqueda, 2);
		$matches = array();
		$vocabulary = taxonomy_vocabulary_machine_name_load($tipo);
		$vid = $vocabulary->vid;


		// si no hay area(s) o tema(s) agregado(s) 
		$resultado = buscar_terminos_sin_filtros($busqueda_elementos, $vid);


		foreach ($resultado as $term) {
		    $matches[$term->name] = check_plain($term->name);
	  	}
	}
	drupal_json_output($matches);
}

function autocomplete_busqueda_tema($areas = NULL, $hash, $busqueda='') {

	$matches = array();
	$tipo = 'tema';
	if (trim($busqueda)) {
		$busqueda_elementos = str_word_count($busqueda, 2);
		$vocabulary = taxonomy_vocabulary_machine_name_load($tipo);
		$vid = $vocabulary->vid;

		// si hay area(s) agregada(s) a la busqueda filtrar por estas
		if ($areas) {
			$resultado = buscar_terminos_con_filtros($busqueda_elementos, $vid, $tipo, $areas);
		}

		// si no hay area(s) o tema(s) agregado(s) 
		else {
			$resultado = buscar_terminos_sin_filtros($busqueda_elementos, $vid);
		}


		foreach ($resultado as $term) {

			// In the simplest case (see user_autocomplete), the key and the value are
			// the same. Here we'll display the uid along with the username in the
			// dropdown.
			$matches[$term->name] = check_plain($term->name);
		}
	}
	drupal_json_output($matches);
}





function autocomplete_busqueda_matriz($areas = NULL, $temas = NULL, $hash, $busqueda='') {

	$matches = array();
	$tipo = 'matriz';
	if (trim($busqueda)) {
		$busqueda_elementos = str_word_count($busqueda, 2);
		$vocabulary = taxonomy_vocabulary_machine_name_load($tipo);
		$vid = $vocabulary->vid;



		// si hay area(s) o tema(s) agregado(s) a la busqueda filtrar por estos
		if ( ($areas) || ($temas) || ($actores)) {

			$resultado = buscar_terminos_con_filtros($busqueda_elementos, $vid, $tipo, $areas, $temas);
		}


		// si no hay area(s) o tema(s) agregado(s) 
		else {
			$resultado = buscar_terminos_sin_filtros($busqueda_elementos, $vid);
		}

		foreach ($resultado as $term) {

			// In the simplest case (see user_autocomplete), the key and the value are
			// the same. Here we'll display the uid along with the username in the
			// dropdown.
			$matches[$term->name] = check_plain($term->name);
		}
	}
	drupal_json_output($matches);
}








function autocomplete_busqueda_actor($areas = NULL, $temas = NULL, $matrices = NULL, $hash, $busqueda='') {

	$matches = array();
	$tipo = 'actor';

	if (trim($busqueda)) {
		$busqueda_elementos = str_word_count($busqueda, 2);
		$vocabulary = taxonomy_vocabulary_machine_name_load($tipo);
		$vid = $vocabulary->vid;

		// si hay area(s) o tema(s) agregado(s) a la busqueda filtrar por estos
		if ( ($areas) || ($temas) || ($matrices) ) {
			$resultado = buscar_terminos_con_filtros($busqueda_elementos, $vid, $tipo, $areas, $temas, $matrices);
		}


		// si no hay area(s) o tema(s) agregado(s) 
		else {
			$resultado = buscar_terminos_sin_filtros($busqueda_elementos, $vid);
		}


		foreach ($resultado as $term) {

			// In the simplest case (see user_autocomplete), the key and the value are
			// the same. Here we'll display the uid along with the username in the
			// dropdown.
			$matches[$term->name] = check_plain($term->name);
		}
	}
	drupal_json_output($matches);
}







function buscar_terminos_sin_filtros($busqueda_elementos, $vid) {

	$query = db_select('taxonomy_term_data', 'ttd')
		->fields('ttd', array('name'))
		->range(0, 10);
	$db_and = db_and();
	foreach($busqueda_elementos as $key => $elemento){
		if ($key == 0) {
			$db_and->condition('ttd.name', db_like($elemento) . '%', 'LIKE');
		}
		else {
			$db_and->condition('ttd.name', '%' . db_like($elemento) . '%', 'LIKE');
		}
	}
	$db_and->condition('ttd.vid', $vid, '=');
	$query->condition($db_and);
	
	$resultado = $query->execute();
	// si no hay resultados ampliar la busqueda, buscando la primera palabra no solo al comienzo
	
	if ($resultado->rowCount() == 0) {
		//reiniciar las condiciones(WHERE)
		$conditions =& $query->conditions();
		$conditions = array('#conjunction' => 'AND');

		$db_and = db_and();
		foreach($busqueda_elementos as $key => $elemento){
			$db_and->condition('ttd.name', '%' . db_like($elemento) . '%', 'LIKE');
		}
		$db_and->condition('ttd.vid', $vid, '=');
		$query->condition($db_and);
		$resultado = $query->execute();
	}

	// aumentar la cantidad de resultados
	elseif ($resultado->rowCount() < 10) {
		$query2 = db_select('taxonomy_term_data', 'ttd')
			->fields('ttd', array('name'))
			->range(0, (10 - $resultado->rowCount()));
		$db_and = db_and();
		foreach($busqueda_elementos as $key => $elemento){
			if ($key == 0) {
				$db_and->condition('ttd.name', '%' . db_like($elemento) . '%', 'LIKE');
				$db_and->condition('ttd.name', db_like($elemento) . '%', 'NOT LIKE');
			}
			else {
				$db_and->condition('ttd.name', '%' . db_like($elemento) . '%', 'LIKE');
			}
		}
		$db_and->condition('ttd.vid', $vid, '=');
		$query2->condition($db_and);
		$query->union($query2);
		$resultado = $query->execute();
	}

	return $resultado;

}




function buscar_terminos_con_filtros($busqueda_elementos, $vid, $tipo, $areas = 0, $temas = 0, $matrices = 0, $actores = 0){

	$query = db_select('field_data_field_' . $tipo, 'tipo_buscado');

	if ($areas) {
		$query->join('field_data_field_area', 'fa', 'tipo_buscado.entity_id = fa.entity_id');
	}
	if ($temas) {
		$query->join('field_data_field_tema', 'ft', 'tipo_buscado.entity_id = ft.entity_id');
	}
	if ($matrices) {
		$query->join('field_data_field_matriz', 'fm', 'tipo_buscado.entity_id = fm.entity_id');
	}
	if ($actores) {
		$query->join('field_data_field_actor', 'fac', 'tipo_buscado.entity_id = fac.entity_id');
	}


	$query->join('taxonomy_term_data', 'ttd', 'tipo_buscado.field_' . $tipo . '_tid = ttd.tid');

	$query
		->fields('ttd', array('name'))
		->distinct()
		->range(0, 10);



	if ($areas) {
		$areas = explode(',', $areas);
		$db_or_area = db_or();
		foreach ($areas as $tid) {
			$db_or_area->condition('fa.field_area_tid', $tid, '=');
		}
		$query->condition($db_or_area);
	}

	if ($temas) {
		$temas = explode(',', $temas);
		$db_or_tema = db_or();
		foreach ($temas as $tid) {
			$db_or_tema->condition('ft.field_tema_tid', $tid, '=');
		}
		$query->condition($db_or_tema);
	}

	if ($matrices) {
		$matrices = explode(',', $matrices);
		$db_or_matriz = db_or();
		foreach ($matrices as $tid) {
			$db_or_matriz->condition('fm.field_matriz_tid', $tid, '=');
		}
		$query->condition($db_or_matriz);
	}

	if ($actores) {
		$actores = explode(',', $actores);
		$db_or_actor = db_or();
		foreach ($actores as $tid) {
			$db_or_actor->condition('fac.field_actor_tid', $tid, '=');
		}
		$query->condition($db_or_tema);
	}


	$db_and = db_and();
	foreach($busqueda_elementos as $key => $elemento){
		if ($key == 0) {
			$db_and->condition('ttd.name', db_like($elemento) . '%', 'LIKE');
		}
		else {
			$db_and->condition('ttd.name', '%' . db_like($elemento) . '%', 'LIKE');
		}
	}
	$db_and->condition('ttd.vid', $vid, '=');

	$query->condition($db_and);

	$resultado = $query->execute();	


	// si no hay resultados ampliar la busqueda, buscando la primera palabra no solo al comienzo
	if ($resultado->rowCount() == 0) {
		//reiniciar las condiciones(WHERE)
		$conditions =& $query->conditions();
		$conditions = array('#conjunction' => 'AND');
		if ($areas) {
			$query->condition($db_or_area);
		}
		if ($temas) {
			$query->condition($db_or_tema);
		}
		if ($matrices) {
			$query->condition($db_or_matriz);
		}
		if ($actores) {
			$query->condition($db_or_actor);
		}

		$db_and = db_and();
		foreach($busqueda_elementos as $key => $elemento){
			$db_and->condition('ttd.name', '%' . db_like($elemento) . '%', 'LIKE');
		}
		$db_and->condition('ttd.vid', $vid, '=');
		$query->condition($db_and);

		$resultado = $query->execute();
	}


	// aumentar la cantidad de resultados
	elseif ($resultado->rowCount() < 10) {

		$query2 = db_select('field_data_field_' . $tipo, 'tipo_buscado');

		if ($areas) {
			$query2->join('field_data_field_area', 'fa', 'tipo_buscado.entity_id = fa.entity_id');
		}
		if ($temas) {
			$query2->join('field_data_field_tema', 'ft', 'tipo_buscado.entity_id = ft.entity_id');
		}
		if ($matrices) {
			$query2->join('field_data_field_matriz', 'fm', 'tipo_buscado.entity_id = fm.entity_id');
		}
		if ($actores) {
			$query2->join('field_data_field_actor', 'fac', 'tipo_buscado.entity_id = fac.entity_id');
		}


		$query2->join('taxonomy_term_data', 'ttd', 'tipo_buscado.field_' . $tipo . '_tid = ttd.tid');

		$query2
			->fields('ttd', array('name'))
			->distinct()
			->range(0, (10 - $resultado->rowCount()) );

		if ($areas) {
			$query2->condition($db_or_area);
		}
		if ($temas) {
			$query2->condition($db_or_tema);
		}
		if ($matrices) {
			$query2->condition($db_or_matriz);
		}
		if ($actores) {
			$query2->condition($db_or_actor);
		}

		$db_and = db_and();
		foreach($busqueda_elementos as $key => $elemento){
			if ($key == 0) {
				$db_and->condition('ttd.name', '%' . db_like($elemento) . '%', 'LIKE');
				$db_and->condition('ttd.name', db_like($elemento) . '%', 'NOT LIKE');
			}
			else {
				$db_and->condition('ttd.name', '%' . db_like($elemento) . '%', 'LIKE');
			}
		}

		$db_and->condition('ttd.vid', $vid, '=');
		$query2->condition($db_and);

		$query->union($query2);

		$resultado = $query->execute();
	}

	return $resultado;
}


?>
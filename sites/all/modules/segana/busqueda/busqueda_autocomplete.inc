<?php

function autocomplete_busqueda_area($busqueda='') {
	$matches = array();
	$tipo = 'area';

	if (trim($busqueda)) {
		$busqueda_elementos = preg_split('/\s+/', $busqueda);
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
		$busqueda_elementos = preg_split('/\s+/', $busqueda);
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
		$busqueda_elementos = preg_split('/\s+/', $busqueda);
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
			$matches[$term->name] = check_plain($term->name);
		}
	}
	drupal_json_output($matches);
}








function autocomplete_busqueda_actor($areas = NULL, $temas = NULL, $matrices = NULL, $hash, $busqueda='') {

	$matches = array();
	$tipo = 'actor';

	if (trim($busqueda)) {
		$busqueda_elementos = preg_split('/\s+/', $busqueda);
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
			if (!empty($term->field_rol_value)) {
				$name = $name_rol = check_plain($term->name) . '  <span>[' . $term->field_rol_value . ']</span>';
			}
			else {
				$name = check_plain($term->name);
			}

			$matches[$term->name] =$name;
		}
	}
	drupal_json_output($matches);
}

?>
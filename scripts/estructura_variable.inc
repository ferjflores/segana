<?

///////////localizacion//////////
//////////////////////////////////
	$tipo ='localizacion';

	$query = db_select('field_data_field_seganaid', 'fs')
		->condition('fs.bundle', $tipo, '=')
		->fields('fs', array('field_seganaid_value'));
	$resultado = $query->execute();
	$count_query = $resultado->rowCount();
	$array_id = array();
	if ($count_query > 0) {
		foreach ($resultado as $row) {
			$array_id[] = $row->field_seganaid_value;
		}
	}


	$query_viejo = "SELECT * FROM Localizacion";
	$resultado_viejo = $segana_viejo->query($query_viejo);
	$array_viejo = array();
	while ($row = $resultado_viejo->fetch_object()) {
		$seganaid = $row->id_localizacion;
		$tipo_localizacion = $row->id_tipo_localizacion;

		$query = db_select('field_data_field_seganaid', 'fs')
			->condition(db_and()->condition('fs.bundle', 'tipo_localizacion', '=')->condition('fs.field_seganaid_value', $tipo_localizacion, '='))
			->fields('fs', array('entity_id'));
		$resultado = $query->execute();
		$row_tipo_localizacion = $resultado->fetchObject();
		$tipo_localizacion_tid = $row_tipo_localizacion->entity_id;

		drush_print($seganaid ."  Tipo: ". $tipo ."   ". $row->nombre);
		drush_print();
		if (!in_array($seganaid, $array_id)) {
			$term = new stdClass();
			$vid = 5;
			$term->name = fixUTF8(forceUTF8(trim($row->nombre)));  
			$term->vid = $vid;
			$term->field_tipo_localizacion[LANGUAGE_NONE][0][tid]  = $tipo_localizacion_tid;
			$term->field_seganaid[LANGUAGE_NONE][0][value]  = $seganaid; 
			taxonomy_term_save($term);
			drush_print($term->tid ." ". $term->name . "             ". $term->description);
			drush_print();
		}
	}

///////////medio//////////
//////////////////////////////////
	$tipo ='medio';

	$query = db_select('field_data_field_seganaid', 'fs')
		->condition('fs.bundle', $tipo, '=')
		->fields('fs', array('field_seganaid_value'));
	$resultado = $query->execute();
	$count_query = $resultado->rowCount();
	$array_id = array();
	if ($count_query > 0) {
		foreach ($resultado as $row) {
			$array_id[] = $row->field_seganaid_value;
		}
	}

	$query_viejo = "SELECT * FROM Medio_Informacion $limit";
	$resultado_viejo = $segana_viejo->query($query_viejo);
	$array_viejo = array();

	while ($row = $resultado_viejo->fetch_object()) {
		$seganaid = $row->id_medio_informacion;


		drush_print($seganaid ."  Tipo: ". $tipo ."   ". $row->nombre);
		drush_print();
		if (!in_array($seganaid, $array_id)) {


			$localizacion = $row->localizacion;
			$query = db_select('field_data_field_seganaid', 'fs')
				->condition(db_and()->condition('fs.bundle', 'localizacion', '=')->condition('fs.field_seganaid_value', $localizacion, '='))
				->fields('fs', array('entity_id'));
			$resultado = $query->execute();
			$row_localizacion = $resultado->fetchObject();
			$localizacion_tid = $row_localizacion->entity_id;

			$query = db_select('field_data_field_tipo_localizacion', 'ftl');
			$query->join('taxonomy_term_data', 'ttd', 'ftl.field_tipo_localizacion_tid = ttd.tid');
			$query->join('taxonomy_term_data', 'ttd_alcance', 'ttd.name = ttd_alcance.name');
			$query
				->condition(db_and()->condition('ftl.bundle', 'localizacion', '=')->condition('ftl.entity_id', $localizacion_tid, '=')->condition('ttd_alcance.vid', 22, '='))
				->fields('ttd_alcance', array('tid'));
			$resultado = $query->execute();
			$row_alcance = $resultado->fetchObject();
			$alcance_tid = $row_alcance->tid;


			$tipo_periodico = $row->tipo_medio;
			$query = db_select('field_data_field_seganaid', 'fs')
				->condition(db_and()->condition('fs.bundle', 'tipo_periodico', '=')->condition('fs.field_seganaid_value', $tipo_periodico, '='))
				->fields('fs', array('entity_id'));
			$resultado = $query->execute();
			$row_tipo_periodico = $resultado->fetchObject();
			$tipo_periodico_tid = $row_tipo_periodico->entity_id;

			$periodicidad = $row->periodicidad;
			$query = db_select('field_data_field_seganaid', 'fs')
				->condition(db_and()->condition('fs.bundle', 'periodicidad', '=')->condition('fs.field_seganaid_value', $periodicidad, '='))
				->fields('fs', array('entity_id'));
			$resultado = $query->execute();
			$row_periodicidad = $resultado->fetchObject();
			$periodicidad_tid = $row_periodicidad->entity_id;

			$tipo_publicacion = $row->tipo_publicacion;
			$query = db_select('field_data_field_seganaid', 'fs')
				->condition(db_and()->condition('fs.bundle', 'tipo_publicacion', '=')->condition('fs.field_seganaid_value', $tipo_publicacion, '='))
				->fields('fs', array('entity_id'));
			$resultado = $query->execute();
			$row_tipo_publicacion = $resultado->fetchObject();
			$tipo_publicacion_tid = $row_tipo_publicacion->entity_id;




			$term = new stdClass();
			$vid = 6;
			$term->name = fixUTF8(forceUTF8(trim($row->nombre)));  
			$term->vid = $vid;
			$term->field_seganaid[LANGUAGE_NONE][0][value]  = $seganaid; 
			$term->field_localizacion[LANGUAGE_NONE][0][tid]  = $localizacion_tid;
			$term->field_alcance[LANGUAGE_NONE][0][tid]  = $alcance_tid;
			$term->field_tipo_periodico[LANGUAGE_NONE][0][tid]  = $tipo_periodico_tid;
			$term->field_periodicidad[LANGUAGE_NONE][0][tid]  = $periodicidad_tid;
			$term->field_tipo_publicacion[LANGUAGE_NONE][0][tid]  = $tipo_publicacion_tid;
			$term->field_pagina_web[LANGUAGE_NONE][0][value]  = fixUTF8(forceUTF8(trim($row->pagina_web)));
			taxonomy_term_save($term);
			drush_print($term->tid ." ". $term->name . "             ". $term->description);
			drush_print();
		}
	}

	?>

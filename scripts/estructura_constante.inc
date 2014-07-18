<?
///////////tendencia//////////
//////////////////////////////////
	$tipo ='tendencia';

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

	$row = array(1=>'A favor', 2=>'En contra');
	
	$array_viejo = array();
	foreach ($row as $id => $tendencia) {
		$seganaid = $id;
		drush_print($seganaid ."  Tipo: ". $tipo ."   ". $tendencia);
		drush_print();
		if (!in_array($seganaid, $array_id)) {
			$term = new stdClass();
			$vid = 16;
			$term->name = fixUTF8(forceUTF8(trim($tendencia)));  
			$term->vid = $vid;
			$term->field_seganaid[LANGUAGE_NONE][0][value]  = $seganaid; 
			taxonomy_term_save($term);
			drush_print($term->tid ." ". $term->name . "             ". $term->description);
			drush_print();
		}
	}


///////////tipo_carga//////////
//////////////////////////////////
	$tipo ='tipo_carga';

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

	$row = array(1=>'Primera Página', 2=>'Cuerpo');
	
	$array_viejo = array();
	foreach ($row as $id => $tipo_carga) {
		$seganaid = $id;
		drush_print($seganaid ."  Tipo: ". $tipo ."   ". $tipo_carga);
		drush_print();
		if (!in_array($seganaid, $array_id)) {
			$term = new stdClass();
			$vid = 11;
			$term->name = fixUTF8(forceUTF8(trim($tipo_carga)));  
			$term->vid = $vid;
			$term->field_seganaid[LANGUAGE_NONE][0][value]  = $seganaid; 
			taxonomy_term_save($term);
			drush_print($term->tid ." ". $term->name . "             ". $term->description);
			drush_print();
		}
	}


///////////tipo_informacion//////////
//////////////////////////////////
	$tipo ='tipo_informacion';

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

	$row = array(1=>'Artículo de opinión', 2=>'Editorial', 3=>'Entrevista', 4=>'Noticia', 5=>'Reportaje', 6=>'Reseña');
	
	$array_viejo = array();
	foreach ($row as $id => $tipo_informacion) {
		$seganaid = $id;
		drush_print($seganaid ."  Tipo: ". $tipo ."   ". $tipo_informacion);
		drush_print();
		if (!in_array($seganaid, $array_id)) {
			$term = new stdClass();
			$vid = 7;
			$term->name = fixUTF8(forceUTF8(trim($tipo_informacion)));  
			$term->vid = $vid;
			$term->field_seganaid[LANGUAGE_NONE][0][value]  = $seganaid; 
			taxonomy_term_save($term);
			drush_print($term->tid ." ". $term->name . "             ". $term->description);
			drush_print();
		}
	}


///////////tipo_medio//////////
//////////////////////////////////
	$tipo ='tipo_medio';

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

	$row = array(1=>'Digital', 2=>'Impreso');
	
	$array_viejo = array();
	foreach ($row as $id => $tipo_medio) {
		$seganaid = $id;
		drush_print($seganaid ."  Tipo: ". $tipo ."   ". $tipo_medio);
		drush_print();
		if (!in_array($seganaid, $array_id)) {
			$term = new stdClass();
			$vid = 3;
			$term->name = fixUTF8(forceUTF8(trim($tipo_medio)));  
			$term->vid = $vid;
			$term->field_seganaid[LANGUAGE_NONE][0][value]  = $seganaid; 
			taxonomy_term_save($term);
			drush_print($term->tid ." ". $term->name . "             ". $term->description);
			drush_print();
		}
	}



///////////tipo_titulo//////////
//////////////////////////////////
	$tipo ='tipo_titulo';

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

	$row = array(1=>'Simple', 2=>'Compuesto');
	
	$array_viejo = array();
	foreach ($row as $id => $tipo_titulo) {
		$seganaid = $id;
		drush_print($seganaid ."  Tipo: ". $tipo ."   ". $tipo_titulo);
		drush_print();
		if (!in_array($seganaid, $array_id)) {
			$term = new stdClass();
			$vid = 20;
			$term->name = fixUTF8(forceUTF8(trim($tipo_titulo)));  
			$term->vid = $vid;
			$term->field_seganaid[LANGUAGE_NONE][0][value]  = $seganaid; 
			taxonomy_term_save($term);
			drush_print($term->tid ." ". $term->name . "             ". $term->description);
			drush_print();
		}
	}

///////////tipo_localizacion//////////
//////////////////////////////////
	$tipo ='tipo_localizacion';

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

	$row = array(1=>'Estatal', 2=>'Regional', 3=>'Nacional', 4=>'Internacional');
	
	$array_viejo = array();
	foreach ($row as $id => $tipo_localizacion) {
		$seganaid = $id;
		drush_print($seganaid ."  Tipo: ". $tipo ."   ". $tipo_localizacion);
		drush_print();
		if (!in_array($seganaid, $array_id)) {
			$term = new stdClass();
			$vid = 4;
			$term->name = fixUTF8(forceUTF8(trim($tipo_localizacion)));  
			$term->vid = $vid;
			$term->field_seganaid[LANGUAGE_NONE][0][value]  = $seganaid; 
			taxonomy_term_save($term);
			drush_print($term->tid ." ". $term->name . "             ". $term->description);
			drush_print();
		}
	}

///////////alcance//////////
//////////////////////////////////
	$tipo ='alcance';

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

	$row = array(1=>'Estatal', 2=>'Regional', 3=>'Nacional', 4=>'Internacional');
	
	$array_viejo = array();
	foreach ($row as $id => $alcance) {
		$seganaid = $id;
		drush_print($seganaid ."  Tipo: ". $tipo ."   ". $alcance);
		drush_print();
		if (!in_array($seganaid, $array_id)) {
			$term = new stdClass();
			$vid = 22;
			$term->name = fixUTF8(forceUTF8(trim($alcance)));  
			$term->vid = $vid;
			$term->field_seganaid[LANGUAGE_NONE][0][value]  = $seganaid; 
			taxonomy_term_save($term);
			drush_print($term->tid ." ". $term->name . "             ". $term->description);
			drush_print();
		}
	}

///////////cuadrante//////////
//////////////////////////////////
	$tipo ='cuadrante';

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

	$row = array(1=>'Arriba Derecha', 2=>'Arriba Izquierda', 3=>'Abajo Derecha', 4=>'Abajo Izquierda');
	
	$array_viejo = array();
	foreach ($row as $id => $cuadrante) {
		$seganaid = $id;
		drush_print($seganaid ."  Tipo: ". $tipo ."   ". $cuadrante);
		drush_print();
		if (!in_array($seganaid, $array_id)) {
			$term = new stdClass();
			$vid = 12;
			$term->name = fixUTF8(forceUTF8(trim($cuadrante)));  
			$term->vid = $vid;
			$term->field_seganaid[LANGUAGE_NONE][0][value]  = $seganaid; 
			taxonomy_term_save($term);
			drush_print($term->tid ." ". $term->name . "             ". $term->description);
			drush_print();
		}
	}

///////////periodicidad//////////
//////////////////////////////////
	$tipo ='periodicidad';

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

	$row = array(1=>'Diario', 2=>'Vespertino', 3=>'Semanario', 4=>'Quincenal', 5=>'Mensual', 6=>'No Tiene');
	
	$array_viejo = array();
	foreach ($row as $id => $periodicidad) {
		$seganaid = $id;
		drush_print($seganaid ."  Tipo: ". $tipo ."   ". $periodicidad);
		drush_print();
		if (!in_array($seganaid, $array_id)) {
			$term = new stdClass();
			$vid = 13;
			$term->name = fixUTF8(forceUTF8(trim($periodicidad)));  
			$term->vid = $vid;
			$term->field_seganaid[LANGUAGE_NONE][0][value]  = $seganaid; 
			taxonomy_term_save($term);
			drush_print($term->tid ." ". $term->name . "             ". $term->description);
			drush_print();
		}
	}

///////////tipo_pagina//////////
//////////////////////////////////
	$tipo ='tipo_pagina';

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

	$row = array(1=>'Personal', 2=>'Página web de medio', 3=>'Portal', 4=>'Institucional');
	
	$array_viejo = array();
	foreach ($row as $id => $tipo_pagina) {
		$seganaid = $id;
		drush_print($seganaid ."  Tipo: ". $tipo ."   ". $tipo_pagina);
		drush_print();
		if (!in_array($seganaid, $array_id)) {
			$term = new stdClass();
			$vid = 18;
			$term->name = fixUTF8(forceUTF8(trim($tipo_pagina)));  
			$term->vid = $vid;
			$term->field_seganaid[LANGUAGE_NONE][0][value]  = $seganaid; 
			taxonomy_term_save($term);
			drush_print($term->tid ." ". $term->name . "             ". $term->description);
			drush_print();
		}
	}

///////////tipo_periodico//////////
//////////////////////////////////
	$tipo ='tipo_periodico';

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

	$row = array(1=>'Tabloide', 2=>'Estandar', 3=>'No Tiene');
	
	$array_viejo = array();
	foreach ($row as $id => $tipo_periodico) {
		$seganaid = $id;
		drush_print($seganaid ."  Tipo: ". $tipo ."   ". $tipo_periodico);
		drush_print();
		if (!in_array($seganaid, $array_id)) {
			$term = new stdClass();
			$vid = 17;
			$term->name = fixUTF8(forceUTF8(trim($tipo_periodico)));  
			$term->vid = $vid;
			$term->field_seganaid[LANGUAGE_NONE][0][value]  = $seganaid; 
			taxonomy_term_save($term);
			drush_print($term->tid ." ". $term->name . "             ". $term->description);
			drush_print();
		}
	}

///////////tipo_publicacion//////////
//////////////////////////////////
	$tipo ='tipo_publicacion';

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

	$row = array(1=>'General', 2=>'Especializado', 3=>'No Tiene');
	
	$array_viejo = array();
	foreach ($row as $id => $tipo_publicacion) {
		$seganaid = $id;
		drush_print($seganaid ."  Tipo: ". $tipo ."   ". $tipo_publicacion);
		drush_print();
		if (!in_array($seganaid, $array_id)) {
			$term = new stdClass();
			$vid = 19;
			$term->name = fixUTF8(forceUTF8(trim($tipo_publicacion)));  
			$term->vid = $vid;
			$term->field_seganaid[LANGUAGE_NONE][0][value]  = $seganaid; 
			taxonomy_term_save($term);
			drush_print($term->tid ." ". $term->name . "             ". $term->description);
			drush_print();
		}
	}
?>
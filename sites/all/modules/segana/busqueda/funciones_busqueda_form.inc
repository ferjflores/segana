<?php

function tipo_localizacion(){
	$tipo_localizacion = array();
	$tipo_localizacion[0] = 'Todos';
	$vocabulary = taxonomy_vocabulary_machine_name_load('tipo_localizacion');
	$vid = $vocabulary->vid;
	$tree = taxonomy_get_tree($vid);
	foreach($tree as $key => $val){
		$tipo_localizacion[$val->tid] = $val->name;
	}
	$tipo_localizacion[-1] = 'Impresos';
	return $tipo_localizacion;
}

function medios($tipo_localizacion){
	$vocabulary = taxonomy_vocabulary_machine_name_load('medio');
	$vid = $vocabulary->vid;
	$medios = array();
	switch ($tipo_localizacion) {
		case 0:
			$tree = taxonomy_get_tree($vid);
			foreach($tree as $key => $val){
				$medios[$val->tid] = $val->name;
			}
			return $medios;
			break;
		case -1:
			$query = db_select('taxonomy_term_data', 'ttd');
			$query->join('field_data_field_version_impresa', 'fvi', 'ttd.tid = fvi.entity_id');
			$query
				->condition(db_and()->condition('ttd.vid', $vid, '=')->condition('fvi.field_version_impresa_value', 1, '='))
				->fields('ttd', array('tid', 'name'))
				->orderBY('name', 'ASC');
			$resultado = $query->execute();
			$count_query = $resultado->rowCount();
			if ($count_query > 0){
				foreach ($resultado as $row) {
					$medios[$row->tid] = $row->name;
				}
			}
			return $medios;
			break;
		default:
			$query = db_select('taxonomy_term_data', 'ttd');
			$query->join('field_data_field_localizacion', 'fl', 'ttd.tid = fl.entity_id');
			$query->join('field_data_field_tipo_localizacion', 'ftl', 'fl.field_localizacion_tid = ftl.entity_id');
			$query
				->condition(db_and()->condition('ttd.vid', $vid, '=')->condition('ftl.field_tipo_localizacion_tid', $tipo_localizacion, '='))
				->fields('ttd', array('tid', 'name'))
				->orderBY('name', 'ASC');
			$resultado = $query->execute();
			$count_query = $resultado->rowCount();
			if ($count_query > 0){
				foreach ($resultado as $row) {
					$medios[$row->tid] = $row->name;
				}
			}
			return $medios;
			break;
	}
}

function tipo_informacion(){
	$tipo_informacion = array();
	$vocabulary = taxonomy_vocabulary_machine_name_load('tipo_informacion');
	$vid = $vocabulary->vid;
	$tree = taxonomy_get_tree($vid);
	foreach($tree as $key => $val){
		$tipo_informacion[$val->tid] = $val->name;
	}
	$tipo_informacion[0] = 'Todos';
	return $tipo_informacion;
}


function tendencia(){
	$tendencia = array();
	$vocabulary = taxonomy_vocabulary_machine_name_load('tendencia');
	$vid = $vocabulary->vid;
	$tree = taxonomy_get_tree($vid);
	foreach($tree as $key => $val){
		$tendencia[$val->tid] = $val->name;
	}
	$tendencia[0] = "Todas";
	return $tendencia;
}


/// esta funcion deberia estar con las funciones generales
function buscar_termino($elemento = NULL, $tipo = NULL) {
	if ( (!empty($elemento)) && (!empty($tipo)) ) {
		$vocabulary = taxonomy_vocabulary_machine_name_load($tipo);
		$vid = $vocabulary->vid;
		$query = db_select('taxonomy_term_data', 'ttd')
		->fields('ttd', array('name', 'tid'))
		->condition(db_and()->condition('ttd.vid', $vid, '=')->condition('ttd.name', $elemento, 'LIKE'));
		$resultado = $query->execute();
		if ($row = $resultado->fetchObject()) {
			$tid = $row->tid;
			$name = $row->name;
			return array($tid => $name);
		}
		else {
			return NULL;
		}
	}
	else {
		return NULL;
	}
}


function lista_elementos($lista_elementos,$tipo){
	if (!empty($lista_elementos)){
		$form = array(
			'#type' => 'fieldset',
			'#prefix' => '<div id="lista_elementos_'. $tipo .'">',
			'#suffix' => '</div>',
		);
		foreach($lista_elementos as $tid => $name){
			$form["borrar_". $tipo ."_$tid"] = array(
				'#type' => 'image_button',
				'#description' => 'Borrar '.$tid,
				'#src' => 'sites/all/themes/segana/images/delete.png',
				'#return_value' => 'borrar_'. $tipo .'_' . $tid,
				//'#submit' => array($submit),
				'#ajax' => array(
					'event' => 'click',
					'callback' => 'form_busqueda_borrar_elemento_callback',
					'wrapper' => 'lista_elementos_'. $tipo,
					'method' => 'html',
					'effect' => 'none',
					'progress' => array('type' => 'throbber'),
				)
			);
			$form[$name] = array(
				'#markup' => $name,
				'#prefix' => "<div id='elemento'>",
				'#suffix' => "</div>",
			);
		}

	}
	else {
		$form = NULL;
	}
	return $form;
}

?>
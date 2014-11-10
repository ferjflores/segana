#!/usr/bin/drush
<?php

if (drush_get_option('truncate')) {
	$tablas = array(
			'field_data_body',
			'field_data_field_actor',
			'field_data_field_alcance',
			'field_data_field_antetitulo',
			'field_data_field_antetitulo_primera_pagina',
			'field_data_field_area',
			'field_data_field_autor',
			'field_data_field_centimetros',
			'field_data_field_centimetros_primera_pagina',
			'field_data_field_cuadrante',
			'field_data_field_cuerpo_argumentativo',
			'field_data_field_cuerpo_seccion',
			'field_data_field_enlace',
			'field_data_field_fecha',
			'field_data_field_fecha_actualizacion',
			'field_data_field_horizontal_cm',
			'field_data_field_horizontal_primera_pagina',
			'field_data_field_localizacion',
			'field_data_field_matriz',
			'field_data_field_medio',
			'field_data_field_nombre',
			'field_data_field_pagina',
			'field_data_field_pagina_web',
			'field_data_field_periodicidad',
			'field_data_field_rol',
			'field_data_field_seganaid',
			'field_data_field_subtitulo',
			'field_data_field_subtitulo_primera_pagina',
			'field_data_field_sumario',
			'field_data_field_sumario_primera_pagina',
			'field_data_field_tema',
			'field_data_field_tendencia',
			'field_data_field_tipo_carga',
			'field_data_field_tipo_informacion',
			'field_data_field_tipo_localizacion',
			'field_data_field_tipo_medio',
			'field_data_field_tipo_pagina',
			'field_data_field_tipo_periodico',
			'field_data_field_tipo_publicacion',
			'field_data_field_tipo_titulo',
			'field_data_field_tipo_titulo_primera_pagina',
			'field_data_field_titulo_primera_pagina',
			'field_data_field_user',
			'field_data_field_usuario',
			'field_data_field_version_impresa',
			'field_data_field_vertical_cm',
			'field_data_field_vertical_primera_pagina',
			'field_data_ldap_user_current_dn'
		);
	$tablas2 = array(
			'node',
			'node_revision',
			'taxonomy_term_data',
			'taxonomy_index',
			'taxonomy_term_hierarchy',
			'users'
		);
	foreach ($tablas as $tabla) {
		$query = "TRUNCATE $tabla";
		db_query($query);
		$tabla_revision = str_replace('field_data', 'field_revision', $tabla);
		$query_revision = "TRUNCATE $tabla_revision";
		db_query($query_revision);
	}
	foreach ($tablas2 as $tabla) {
		$query = "TRUNCATE $tabla";
		db_query($query);
	}

	define('DRUPAL_ROOT', getcwd());
	require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	require_once DRUPAL_ROOT . '/includes/password.inc';
	$password = user_hash_password('nx560');

	//db_query("DELETE FROM field_data_field_seganaid WHERE entity_type != 'user'");
	//db_query("DELETE FROM field_revision_field_seganaid WHERE entity_type != 'user'");
	db_query("INSERT INTO `users` (`uid`, `name`, `pass`, `mail`, `theme`, `signature`, `signature_format`, `created`, `access`, `login`, `status`, `timezone`, `language`, `picture`, `init`, `data`) VALUES
(0, '', '', '', '', '', NULL, 0, 0, 0, 0, NULL, '', 0, '', NULL),
(1, 'admin', '". $password ."', 'admin@avn.info.ve', '', '', 'html', 1399995941, 1402506575, 1402335223, 1, 'America/Caracas', '', 0, 'admin@avn.info.ve', 0x613a313a7b733a373a226f7665726c6179223b693a313b7d)");
}
else {
	$vocabulary = taxonomy_get_vocabularies();
	foreach ($vocabulary as $item) {
		$vid= $item->vid;
		$tree = taxonomy_get_tree($vid);
		foreach($tree as $key => $val){
			taxonomy_term_delete($val->tid);
		}
	}


	$query = db_select('node', 'n')
		->fields('n', array('nid'));
	$resultado = $query->execute();
	foreach ($resultado as $row) {
		drush_print('borrando '. $row->nid);
		node_delete($row->nid);
	}

	db_query('TRUNCATE node');
	db_query('TRUNCATE node_revision');
	db_query('TRUNCATE taxonomy_term_data'); 
}

?>

<?php

function vistas_forms($form_id, $args) {
  $forms = array();
  if (strpos($form_id, 'vistas_medio_form_') === 0) {
    $forms[$form_id] = array(
      'callback' => 'vistas_medio_form',
    );
  }
  if (strpos($form_id, 'vistas_nota_form_') === 0) {
    $forms[$form_id] = array(
      'callback' => 'vistas_nota_form',
    );
  }
	$elementos_discursivos = array('area','tema','matriz','actor');
	foreach($elementos_discursivos as $elemento_discursivo){
		if (strpos($form_id, 'vistas_'. $elemento_discursivo .'_form') === 0) {
			$forms[$form_id] = array(
				'callback' => 'vistas_discursivo_form',
				'callback arguments' => array($args[0], $elemento_discursivo),
		  );
		}
	}

  return $forms;
}

function vistas_form_alter(&$form, $form_state, $form_id) {
	if (strpos($form_id, 'vistas_medio_form_') === 0) {
		$form['#attributes'] = array('class' => 'vistas-medio-form');
  }
	if (strpos($form_id, 'vistas_nota_form_') === 0) {
		$form['#attributes'] = array('class' => 'vistas-nota-form');
  }
	$elementos_discursivos = array('area','tema','matriz','actor');
	foreach($elementos_discursivos as $elemento_discursivo){
		if (strpos($form_id, 'vistas_'. $elemento_discursivo .'_form') === 0) {
			$form['#attributes'] = array('class' => 'vistas-discursivo-form');
		}
	}
}

function vistas_nota_form($form, &$form_state, $nid = NULL) {
	$form = array();



	// tipo medio opciones
	$vocabulary = taxonomy_vocabulary_machine_name_load('tipo_medio');
	$vid = $vocabulary->vid;

	$query_tipo_medio = db_select('taxonomy_term_data', 'ttd')
    ->fields('ttd', array('name', 'tid'))
    ->condition('ttd.vid', $vid);

  $resultado_tipo_medio = $query_tipo_medio->execute();
	$tipo_medio_array = array();

	foreach ($resultado_tipo_medio as $row_tipo_medio) {
		$tid = $row_tipo_medio->tid;
		$tipo_medio_array[$tid] = $row_tipo_medio->name;
	}
  $tid_impreso = array_search('Impreso', $tipo_medio_array);
  $tid_digital = array_search('Digital', $tipo_medio_array);

  //tipo de carga opciones

  $vocabulary = taxonomy_vocabulary_machine_name_load('tipo_carga');
	$vid = $vocabulary->vid;

	$query_tipo_carga = db_select('taxonomy_term_data', 'ttd')
    ->fields('ttd', array('name', 'tid'))
    ->condition('ttd.vid', $vid);

  $resultado_tipo_carga = $query_tipo_carga->execute();
	$tipo_carga_array = array();

  foreach ($resultado_tipo_carga as $row_tipo_carga) {
  	$tid = $row_tipo_carga->tid;
  	$tipo_carga_array[$tid] = $row_tipo_carga->name;
  }  
  $tid_primera_pagina = array_search('Primera Página', $tipo_carga_array);
  $tid_cuerpo = array_search('Cuerpo', $tipo_carga_array);


  // tipo de titulo opciones
	$vocabulary = taxonomy_vocabulary_machine_name_load('tipo_titulo');
	$vid = $vocabulary->vid;

	$query_tipo_titulo = db_select('taxonomy_term_data', 'ttd')
    ->fields('ttd', array('name', 'tid'))
    ->condition('ttd.vid', $vid);

  $resultado_tipo_titulo = $query_tipo_titulo->execute();
	$tipo_titulo_array = array();

  foreach ($resultado_tipo_titulo as $row_tipo_titulo) {
  	$tid = $row_tipo_titulo->tid;
  	$tipo_titulo_array[$tid] = $row_tipo_titulo->name;
  }  
	$tid_compuesto = array_search('Compuesto', $tipo_titulo_array);
	$tid_simple = array_search('Simple', $tipo_titulo_array);



  	//determinar si es una nota nueva o editar(cargar valores)

	if ( ($nid) && empty($form_state['clicked_button']['#value']) ){
		$node = node_load($nid);
		$language = LANGUAGE_NONE;
		$title = $node->title;
		$fecha = date('Y-m-d', $node->field_fecha[$language][0]['value']);
		$tipo_titulo = $node->field_tipo_titulo[$language][0]['tid'];
		if ($tipo_titulo == $tid_compuesto) {
			$antetitulo = isset($node->field_antetitulo[$language][0]['value']) ? $node->field_antetitulo[$language][0]['value'] : NULL;
			$subtitulo = isset($node->field_subtitulo[$language][0]['value']) ? $node->field_subtitulo[$language][0]['value'] : NULL;
			$sumario = isset($node->field_sumario[$language][0]['value']) ? $node->field_sumario[$language][0]['value'] : NULL;
		}
		$body = $node->body[$language][0]['value'];
		$tipo_medio = $node->field_tipo_medio[$language][0]['tid'];
		$medio_term = taxonomy_term_load($node->field_medio[$language][0]['tid']);
		$medio = $medio_term->name;
		$area_term = taxonomy_term_load($node->field_area[$language][0]['tid']);
		$area = $area_term->name;
		$usuario_term = isset($node->field_usuario[$language][0]['tid']) ? taxonomy_term_load($node->field_usuario[$language][0]['tid']) : NULL;
		$usuario = isset($usuario_term) ? $usuario_term->name : NULL;
		$autor = $node->field_autor[$language][0]['value'];
		$tipo_informacion = $node->field_tipo_informacion[$language][0]['tid'];
		$tendencia_nota_term = isset($node->field_tendencia_nota[$language][0]['tid']) ? taxonomy_term_load($node->field_tendencia_nota[$language][0]['tid']) : NULL;
		$tendencia_nota = isset($tendencia_nota_term) ? $tendencia_nota_term->name : NULL;


		// opciones para medios digitales
		if ($tipo_medio == $tid_digital) {
			$enlace = isset($node->field_enlace[$language][0]['value']) ? $node->field_enlace[$language][0]['value'] : NULL;
		}
		//opciones para medios impresos
		else {
			$tipo_carga = $node->field_tipo_carga[$language][0]['tid'];

			$cuerpo_seccion = isset($node->field_cuerpo_seccion[$language][0]['value']) ? $node->field_cuerpo_seccion[$language][0]['value'] : NULL;
			$pagina = isset($node->field_pagina[$language][0]['value']) ? $node->field_pagina[$language][0]['value'] : NULL;
			$cuadrante = isset($node->field_cuadrante[$language][0]['value']) ? $node->field_cuadrante[$language][0]['value'] : NULL;
			$horizontal_cm = isset($node->field_horizontal_cm[$language][0]['value']) ? $node->field_horizontal_cm[$language][0]['value'] : NULL;
			$vertical_cm = isset($node->field_vertical_cm[$language][0]['value']) ? $node->field_vertical_cm[$language][0]['value'] : NULL;
			$centimetros = isset($node->field_centimetros[$language][0]['value']) ? $node->field_centimetros[$language][0]['value'] : NULL;

			if ($tipo_carga == $tid_primera_pagina) {
				$titulo_primera_pagina = $node->field_titulo_primera_pagina[$language][0]['value'];
				$tipo_titulo_primera_pagina = $node->field_tipo_titulo_primera_pagina[$language][0]['tid'];
				if ($tipo_titulo_primera_pagina == $tid_compuesto) {
					$antetitulo_primera_pagina = isset($node->field_antetitulo_primera_pagina[$language][0]['value']) ? $node->field_antetitulo_primera_pagina[$language][0]['value'] : NULL;
					$subtitulo_primera_pagina = isset($node->field_subtitulo_primera_pagina[$language][0]['value']) ? $node->field_subtitulo_primera_pagina[$language][0]['value'] : NULL;
					$sumario_primera_pagina = isset($node->field_sumario_primera_pagina[$language][0]['value']) ? $node->field_sumario_primera_pagina[$language][0]['value'] : NULL;
				}
				$horizontal_primera_pagina = isset($node->field_horizontal_primera_pagina[$language][0]['value']) ? $node->field_horizontal_primera_pagina[$language][0]['value'] : NULL;
				$vertical_primera_pagina = isset($node->field_vertical_primera_pagina[$language][0]['value']) ? $node->field_vertical_primera_pagina[$language][0]['value'] : NULL;
				$centimetros_primera_pagina = isset($node->field_centimetros[$language][0]['value']) ? $node->field_centimetros[$language][0]['value'] : NULL;
			}

		}
	}
	else {
		$fecha = date('Y-m-d');
	}

	$form["wrapper"] = array("#markup" => "<div class='inline-messages'></div>");

	$form['contenedor'] = array(
		'#type' => 'fieldset',
		'#attributes' => array('class' => 'contenedor'),
		'#prefix' => '<div id="contenedor">',
		'#suffix' => '</div>',
		'#value' => '<A NAME="top"> </A> ',
	);

	$form['contenedor']['nid'] = array(
		'#type' => 'token',
		'#value' => isset($form_state['storage']['values']['nid']) ? $form_state['storage']['values']['nid'] : (isset($nid) ? $nid : NULL),
	);

	$form['contenedor']['superior'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="superior">',
		'#suffix' => '</div>',
	);

	$form['contenedor']['superior']['title'] = array(
		'#type' => 'textfield',
		'#title' => 'Título',
		'#attributes' => array('autocomplete' =>'off'),
		'#default_value' => !empty($form_state['storage']['values']['title']) ? $form_state['storage']['values']['title'] : (isset($title) ? $title : NULL),
		'#size' => 50,
		'#maxlength' => 256,
		'#required' => TRUE,
  	);
	$form['contenedor']['superior']['fecha'] = array(
		'#type' => 'date_popup',
		'#title' => t('Fecha'),
		'#attributes' => array('autocomplete' =>'off'),
		'#date_format' => 'd-m-Y',
		'#date_year_range' => '-6:+0',
		'#size' => 6,
		'#default_value' => !empty($form_state['storage']['values']['fecha']) ? $form_state['storage']['values']['fecha'] : (isset($fecha) ? $fecha : NULL),
		'#required' => TRUE,
	);


	$form['contenedor']['titular'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="titular">',
		'#suffix' => '</div>',
	);

  $form['contenedor']['titular']['tipo_titulo'] = array(
		'#type' => 'select',
		'#title' => 'Tipo de titulo',
		'#default_value' => !empty($form_state['storage']['values']['tipo_titulo']) ? $form_state['storage']['values']['tipo_titulo'] : (isset($tipo_titulo) ? $tipo_titulo : array($tid_simple)),
		'#options' => $tipo_titulo_array,
		'#ajax' => array(
			'callback' => 'form_vistas_nota_tipo_titulo_callback',
			'wrapper' => 'titular',
			'progress' => array('type' => 'throbber'),
			'effect' => 'fade',
			'method' => 'replace',
		),
  );


	$tipo_titulo_opcion = isset($form_state['values']['tipo_titulo']) ? $form_state['values']['tipo_titulo'] : (isset($tipo_titulo) ? $tipo_titulo: $tid_simple);

	if ($tipo_titulo_opcion ==  $tid_compuesto) {
	  $form['contenedor']['titular']['antetitulo'] = array(
			'#type' => 'textfield',
			'#title' => 'Antetítulo',
			'#attributes' => array('autocomplete' =>'off'),
			'#default_value' => !empty($form_state['storage']['values']['antetitulo']) ? $form_state['storage']['values']['antetitulo'] : (isset($antetitulo) ? $antetitulo : NULL),
			'#size' => 50,
			'#maxlength' => 256,
			//'#dependency' => array('edit-tipo-titulo' => array($tid_compuesto)),
	  );
	   $form['contenedor']['titular']['subtitulo'] = array(
			'#type' => 'textfield',
			'#title' => 'Subtítulo',
			'#attributes' => array('autocomplete' =>'off'),
			'#default_value' => !empty($form_state['storage']['values']['subtitulo']) ? $form_state['storage']['values']['subtitulo'] : (isset($subtitulo) ? $subtitulo : NULL),
			'#size' => 50,
			'#maxlength' => 256,
			//'#dependency' => array('edit-tipo-titulo' => array($tid_compuesto)),
	  );
	  $form['contenedor']['titular']['sumario'] = array(
			'#type' => 'textarea',
			'#title' => 'Sumario',
			'#attributes' => array('autocomplete' =>'off'),
			'#default_value' => !empty($form_state['storage']['values']['sumario']) ? $form_state['storage']['values']['sumario'] : (isset($sumario) ? $sumario : NULL),
			'#rows' => 5,
			'#cols' => 3,
			'#maxlength' => 56000,
			'#resizable' => FALSE,
			//'#dependency' => array('edit-tipo-titulo' => array($tid_compuesto)),
	  );
	}
	$form['contenedor']['medio'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="medio">',
		'#suffix' => '</div>',
	);


	$form['contenedor']['medio']['derecha'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="medio-derecha">',
		'#suffix' => '</div>',
	);

	$form['contenedor']['medio']['derecha']['body'] = array(
		'#type' => 'textarea',
		'#title' => t('Cuerpo de texto'),
		'#attributes' => array('autocomplete' =>'off'),
		'#default_value' => isset($form_state['storage']['values']['body']) ? $form_state['storage']['values']['body'] : (isset($body) ? $body : NULL),
		'#rows' => 10,
		'#cols' => 17,
		'#maxlength' => 56000,
		'#resizable' => FALSE,
	);
	$form['contenedor']['medio']['izquierda'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="medio-izquierda">',
		'#suffix' => '</div>',
	);




	$form['contenedor']['medio']['izquierda']['tipo_medio'] = array(
		'#type' => 'select',
		'#title' => 'Tipo de medio',
		'#attributes' => array('autocomplete' =>'off'),
		'#default_value' => !empty($form_state['storage']['values']['tipo_medio']) ? $form_state['storage']['values']['tipo_medio'] : (isset($tipo_medio) ? $tipo_medio : NULL),
		'#options' => $tipo_medio_array,
		'#ajax' => array(
			'callback' => 'form_vistas_nota_tipo_medio_callback',
			'wrapper' => 'medio',
			'progress' => array('type' => 'throbber'),
			'effect' => 'fade',
			'method' => 'replace',
		),
  );

  $form['contenedor']['medio']['izquierda']['medio'] = array(
		'#type' => 'textfield',
		'#title' => 'Medio',
		'#attributes' => array('autocomplete' =>'off'),
		'#default_value' => !empty($form_state['storage']['values']['medio']) ? $form_state['storage']['values']['medio'] : (isset($medio) ? $medio : NULL),
		'#size' => 13,
		'#maxlength' => 128,
		'#autocomplete_path' => 'notas/autocomplete_medio',
		'#required' => TRUE,
  );
	$tipo_medio_opcion = isset($form_state['values']['tipo_medio']) ? $form_state['values']['tipo_medio'] : (isset($tipo_medio) ? $tipo_medio: $tid_digital);


  $form['contenedor']['medio']['izquierda']['area'] = array(
		'#type' => 'textfield',
		'#title' => 'Área',
		'#attributes' => array('autocomplete' =>'off'),
		'#default_value' => !empty($form_state['storage']['values']['area']) ? $form_state['storage']['values']['area'] : (isset($area) ? $area : NULL),
		'#size' => 13,
		'#maxlength' => 128,
		'#autocomplete_path' => 'busqueda/autocomplete_area',
		'#required' => TRUE,
  );
  $form['contenedor']['medio']['izquierda']['usuario'] = array(
		'#type' => 'textfield',
		'#title' => 'Usuario',
		'#attributes' => array('autocomplete' =>'off'),
		'#default_value' => !empty($form_state['storage']['values']['usuario']) ? $form_state['storage']['values']['usuario'] : (isset($usuario) ? $usuario : NULL),
		'#size' => 13,
		'#maxlength' => 128,
		'#autocomplete_path' => 'nota/autocomplete/usuario/0/0',
  );

	$form['contenedor']['medio']['izquierda']['autor'] = array(
		'#type' => 'textfield',
		'#title' => 'Autor',
		'#attributes' => array('autocomplete' =>'off'),
		'#default_value' => !empty($form_state['storage']['values']['autor']) ? $form_state['storage']['values']['autor'] : (isset($autor) ? $autor : NULL),
		'#size' => 13,
		'#maxlength' => 128,
		'#required' => TRUE,
  );

	$vocabulary = taxonomy_vocabulary_machine_name_load('tipo_informacion');
	$vid = $vocabulary->vid;

	$query_tipo_informacion = db_select('taxonomy_term_data', 'ttd')
    ->fields('ttd', array('name', 'tid'))
    ->condition('ttd.vid', $vid);

  $resultado_tipo_informacion = $query_tipo_informacion->execute();
	$tipo_informacion_array = array();

  foreach ($resultado_tipo_informacion as $row_tipo_informacion) {
  	$tid = $row_tipo_informacion->tid;
  	$tipo_informacion_array[$tid] = $row_tipo_informacion->name;
  }

  $form['contenedor']['medio']['izquierda']['tipo_informacion'] = array(
		'#type' => 'select',
		'#title' => 'Tipo de información',
		'#attributes' => array('autocomplete' =>'off'),
		'#default_value' => !empty($form_state['storage']['values']['tipo_informacion']) ? $form_state['storage']['values']['tipo_informacion'] : (isset($tipo_informacion) ? $tipo_informacion : NULL),
		'#options' => $tipo_informacion_array,
  );


  if ($tipo_medio_opcion == $tid_impreso){
	
		$form['contenedor']['medio']['impreso'] = array(
			'#type' => 'fieldset',
			'#prefix' => '<div id="medio-impreso">',
			'#suffix' => '</div>',
		);

	  $form['contenedor']['medio']['impreso']['tipo_carga'] = array(
			'#type' => 'select',
			'#title' => 'Tipo de carga',
			'#default_value' => !empty($form_state['storage']['values']['tipo_carga']) ? $form_state['storage']['values']['tipo_carga'] : (isset($tipo_carga) ? $tipo_carga : NULL),
			'#options' => $tipo_carga_array,
			'#ajax' => array(
				'callback' => 'form_vistas_nota_tipo_carga_callback',
				'wrapper' => 'primera_pagina',
				'progress' => array('type' => 'throbber'),
				'effect' => 'fade',
				'method' => 'replace',
			),
	  );
		$tipo_carga_opcion = isset($form_state['values']['tipo_carga']) ? $form_state['values']['tipo_carga'] : (isset($tipo_carga) ? $tipo_carga: $tid_cuerpo);

		$form['contenedor']['medio']['impreso']['primera_pagina'] = array(
			'#type' => 'fieldset',
			'#prefix' => '<div id="primera_pagina">',
			'#suffix' => '</div>',
		);
		  
		if ($tipo_carga_opcion == $tid_primera_pagina){

		  $form['contenedor']['medio']['impreso']['primera_pagina']['titulo_primera_pagina'] = array(
				'#type' => 'textfield',
				'#title' => 'Título primera página',
				'#attributes' => array('autocomplete' =>'off'),
				'#default_value' => !empty($form_state['storage']['values']['titulo_primera_pagina']) ? $form_state['storage']['values']['titulo_primera_pagina'] : (isset($titulo_primera_pagina) ? $titulo_primera_pagina : NULL),
				'#size' => 50,
				'#maxlength' => 256,
				'#required' => TRUE,
		 	);



		  $form['contenedor']['medio']['impreso']['primera_pagina']['tipo_titulo_primera_pagina'] = array(
				'#type' => 'select',
				'#title' => 'Tipo de titulo primera página',
				'#default_value' => !empty($form_state['storage']['values']['tipo_titulo_primera_pagina']) ? $form_state['storage']['values']['tipo_titulo_primera_pagina'] : (isset($tipo_titulo_primera_pagina) ? $tipo_titulo_primera_pagina : array($tid_simple)),
				'#options' => $tipo_titulo_array,
				'#ajax' => array(
					'callback' => 'form_vistas_nota_tipo_titulo_primera_pagina_callback',
					'wrapper' => 'titular_primera_pagina',
					'progress' => array('type' => 'throbber'),
					'effect' => 'fade',
					'method' => 'replace',
				),
		  );

		  $form['contenedor']['medio']['impreso']['primera_pagina']['titular_primera_pagina'] = array(
				'#type' => 'fieldset',
				'#prefix' => '<div id="titular_primera_pagina">',
				'#suffix' => '</div>',
			);

			$tipo_titulo_primera_pagina_opcion = isset($form_state['values']['tipo_titulo_primera_pagina']) ? $form_state['values']['tipo_titulo_primera_pagina'] : (isset($tipo_titulo_primera_pagina) ? $tipo_titulo_primera_pagina: $tid_simple);

			if ($tipo_titulo_primera_pagina_opcion ==  $tid_compuesto) {

			  $form['contenedor']['medio']['impreso']['primera_pagina']['titular_primera_pagina']['antetitulo_primera_pagina'] = array(
					'#type' => 'textfield',
					'#title' => 'Antetítulo primera página',
					'#attributes' => array('autocomplete' =>'off'),
					'#default_value' => !empty($form_state['storage']['values']['antetitulo_primera_pagina']) ? $form_state['storage']['values']['antetitulo_primera_pagina'] : (isset($antetitulo_primera_pagina) ? $antetitulo_primera_pagina : NULL),
					'#size' => 50,
					'#maxlength' => 256,
			  );
			   $form['contenedor']['medio']['impreso']['primera_pagina']['titular_primera_pagina']['subtitulo_primera_pagina'] = array(
					'#type' => 'textfield',
					'#title' => 'Subtítulo primera página',
					'#attributes' => array('autocomplete' =>'off'),
					'#default_value' => !empty($form_state['storage']['values']['subtitulo_primera_pagina']) ? $form_state['storage']['values']['subtitulo_primera_pagina'] : (isset($subtitulo_primera_pagina) ? $subtitulo_primera_pagina : NULL),
					'#size' => 50,
					'#maxlength' => 256,
			  );
			  $form['contenedor']['medio']['impreso']['primera_pagina']['titular_primera_pagina']['sumario_primera_pagina'] = array(
					'#type' => 'textarea',
					'#title' => 'Sumario primera página',
					'#attributes' => array('autocomplete' =>'off'),
					'#default_value' => !empty($form_state['storage']['values']['titular_primera_pagina']['sumario_primera_pagina']) ? $form_state['storage']['values']['sumario_primera_pagina'] : (isset($sumario_primera_pagina) ? $sumario_primera_pagina : NULL),
					'#rows' => 5,
					'#cols' => 3,
					'#maxlength' => 56000,
					'#resizable' => FALSE,
			  );
			}

			$form['contenedor']['medio']['impreso']['primera_pagina']['horizontal_primera_pagina'] = array(
				'#type' => 'textfield',
				'#title' => 'Centimetros horizontal primera página',
				'#attributes' => array('autocomplete' =>'off'),
				'#default_value' => !empty($form_state['storage']['values']['horizontal_primera_pagina']) ? $form_state['storage']['values']['horizontal_primera_pagina'] : (isset($horizontal_primera_pagina) ? $horizontal_primera_pagina : NULL),
				'#size' => 12,
				'#maxlength' => 128,
				'#ajax' => array(
					'callback' => 'form_vistas_nota_centimetros_primera_pagina_callback',
					'wrapper' => 'centimetros_primera_pagina',
					'progress' => 'none',
					'effect' => 'fade',
					'method' => 'replace',
					'even' => 'blur',
				),
		  );

			$form['contenedor']['medio']['impreso']['primera_pagina']['vertical_primera_pagina'] = array(
				'#type' => 'textfield',
				'#title' => 'Centimetros vertical primera pagina',
				'#attributes' => array('autocomplete' =>'off'),
				'#default_value' => !empty($form_state['storage']['values']['vertical_primera_pagina']) ? $form_state['storage']['values']['vertical_primera_pagina'] : (isset($vertical_primera_pagina) ? $vertical_primera_pagina : NULL),
				'#size' => 12,
				'#maxlength' => 128,
				'#ajax' => array(
					'callback' => 'form_vistas_nota_centimetros_primera_pagina_callback',
					'wrapper' => 'centimetros_primera_pagina',
					'progress' => 'none',
					'effect' => 'fade',
					'method' => 'replace',
					'even' => 'blur',
				),
		  );

			$form['contenedor']['medio']['impreso']['primera_pagina']['centimetros_primera_pagina'] = array(
				'#type' => 'textfield',
				'#title' => 'Centimetros² primera_pagina'	,
				'#attributes' => array('autocomplete' =>'off'),
				'#default_value' => !empty($form_state['storage']['values']['centimetros_primera_pagina']) ? $form_state['storage']['values']['centimetros_primera_pagina'] : (isset($centimetros_primera_pagina) ? $centimetros_primera_pagina : NULL),
				'#size' => 12,
				'#maxlength' => 128,
				'#disabled' => TRUE,
				'#prefix' => '<div id="centimetros_primera_pagina">',
				'#suffix' => '</div>',
		  );
		}
	}
	if ($tipo_medio_opcion == $tid_digital) {
		$form['contenedor']['medio']['izquierda']['enlace'] = array(
			'#type' => 'textfield',
			'#title' => 'Enlace',
			'#attributes' => array('autocomplete' =>'off'),
			'#default_value' => !empty($form_state['storage']['values']['enlace']) ? $form_state['storage']['values']['enlace'] : (isset($enlace) ? $enlace : NULL),
			'#size' => 13,
			'#maxlength' => 128,
	  );
	}


	if ($tipo_medio_opcion == $tid_impreso) {

		$form['contenedor']['medio']['impreso']['cuerpo_seccion'] = array(
			'#type' => 'textfield',
			'#title' => 'Cuerpo y/o Sección',
			'#attributes' => array('autocomplete' =>'off'),
			'#default_value' => !empty($form_state['storage']['values']['cuerpo_seccion']) ? $form_state['storage']['values']['cuerpo_seccion'] : (isset($cuerpo_seccion) ? $cuerpo_seccion : NULL),
			'#size' => 12,
			'#maxlength' => 128,
	  );

		$form['contenedor']['medio']['impreso']['pagina'] = array(
			'#type' => 'textfield',
			'#title' => 'Página',
			'#attributes' => array('autocomplete' =>'off'),
			'#default_value' => !empty($form_state['storage']['values']['pagina']) ? $form_state['storage']['values']['pagina'] : (isset($pagina) ? $pagina : NULL),
			'#size' => 12,
			'#maxlength' => 128,
	 	);


		$vocabulary = taxonomy_vocabulary_machine_name_load('cuadrante');
		$vid = $vocabulary->vid;

		$query_cuadrante = db_select('taxonomy_term_data', 'ttd')
	    ->fields('ttd', array('name', 'tid'))
	    ->condition('ttd.vid', $vid);

	  $resultado_cuadrante = $query_cuadrante->execute();
		$cuadrante_array = array();

	  foreach ($resultado_cuadrante as $row_cuadrante) {
	  	$tid = $row_cuadrante->tid;
	  	$cuadrante_array[$tid] = $row_cuadrante->name;
	  }

	  $form['contenedor']['medio']['impreso']['cuadrante'] = array(
			'#type' => 'select',
			'#title' => 'Cuadrante',
			'#attributes' => array('autocomplete' =>'off'),
			'#default_value' => !empty($form_state['storage']['values']['cuadrante']) ? $form_state['storage']['values']['cuadrante'] : (isset($cuadrante) ? $cuadrante : NULL),
			'#options' => $cuadrante_array,
	  );


		$form['contenedor']['medio']['impreso']['horizontal_cm'] = array(
			'#type' => 'textfield',
			'#title' => 'Centimetros horizontal',
			'#attributes' => array('autocomplete' =>'off'),
			'#default_value' => !empty($form_state['storage']['values']['horizontal_cm']) ? $form_state['storage']['values']['horizontal_cm'] : (isset($horizontal_cm) ? $horizontal_cm : NULL),
			'#size' => 12,
			'#maxlength' => 128,
			'#ajax' => array(
				'callback' => 'form_vistas_nota_centimetros_callback',
				'wrapper' => 'centimetros',
				'progress' => 'none',
				'effect' => 'fade',
				'method' => 'replace',
				'even' => 'blur',
			),
	 	);

		$form['contenedor']['medio']['impreso']['vertical_cm'] = array(
			'#type' => 'textfield',
			'#title' => 'Centimetros vertical',
			'#attributes' => array('autocomplete' =>'off'),
			'#default_value' => !empty($form_state['storage']['values']['vertical_cm']) ? $form_state['storage']['values']['vertical_cm'] : (isset($vertical_cm) ? $vertical_cm : NULL),
			'#size' => 12,
			'#maxlength' => 128,
			'#ajax' => array(
				'callback' => 'form_vistas_nota_centimetros_callback',
				'wrapper' => 'centimetros',
				'progress' => 'none',
				'effect' => 'fade',
				'method' => 'replace',
				'even' => 'blur',
			),
	 	);

		$form['contenedor']['medio']['impreso']['centimetros'] = array(
			'#type' => 'textfield',
			'#title' => 'Centimetros²',
			'#attributes' => array('autocomplete' =>'off'),
			'#prefix' => '<div id="centimetros">',
			'#suffix' => '</div>',
			'#default_value' => !empty($form_state['storage']['values']['centimetros']) ? $form_state['storage']['values']['centimetros'] : (isset($centimetros) ? $centimetros : NULL),
			'#size' => 12,
			'#maxlength' => 128,
			'#disabled' => TRUE,
	 	);
	}

	$form['contenedor']['inferior'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="inferior">',
		'#suffix' => '</div>',
		'#title' => 'ELEMENTOS DISCURSIVOS',
	);
	if (isset($nid) && !isset($form_state['triggering_element']['#return_value'])){
		$query_filas_discursivo = db_select('field_data_field_tema', 'ft')
    	->condition('ft.entity_id', $nid);
    $filas_discursivo = $query_filas_discursivo->countQuery()->execute()->fetchField();


    for ($i = 0; $i < $filas_discursivo; $i++) {
    	$tema_term = isset($node->field_tema[$language][$i]['tid']) ? taxonomy_term_load($node->field_tema[$language][$i]['tid']) : NULL;
    	$tema[$i] = $tema_term->name;
    	$matriz_term = isset($node->field_matriz[$language][$i]['tid']) ? taxonomy_term_load($node->field_matriz[$language][$i]['tid']) : NULL;
    	$matriz[$i] = $matriz_term->name;
    	$cuerpo_argumentativo[$i] = isset($node->field_cuerpo_argumentativo[$language][$i]['value']) ? $node->field_cuerpo_argumentativo[$language][$i]['value'] : NULL;
    	$tendencia[$i] = isset($node->field_tendencia[$language][$i]['tid']) ? $node->field_tendencia[$language][$i]['tid'] : NULL;
    	$actor_term = isset($node->field_actor[$language][$i]['tid']) ? taxonomy_term_load($node->field_actor[$language][$i]['tid']) : NULL;
    	$actor[$i] = $actor_term->name;
		}
    

    // en caso de no poseer se establece $filas_discursivo = 1 para tener una fila en blanco
    $filas_discursivo = empty($filas_discursivo) ? 1 : $filas_discursivo;


	}
	else {
		$filas_discursivo = (isset($form_state['storage']['values']['filas_discursivo'])) ? $form_state['storage']['values']['filas_discursivo'] : 1;
	}
	$form['contenedor']['filas_discursivo'] = array(
		'#type' => 'token',
		'#value' => $filas_discursivo,
	);


	$vocabulary = taxonomy_vocabulary_machine_name_load('tendencia');
	$vid = $vocabulary->vid;

	$query_tendencia = db_select('taxonomy_term_data', 'ttd')
    ->fields('ttd', array('name', 'tid'))
    ->condition('ttd.vid', $vid);

  $resultado_tendencia = $query_tendencia->execute();
	$tendencia_array = array();

  foreach ($resultado_tendencia as $row_tendencia) {
  	$tid = $row_tendencia->tid;
  	$tendencia_array[$tid] = $row_tendencia->name;
  } 
  $tendencia_array[0] = 'No definido'; 

  $tendencia_nota = (!empty($form_state['storage']['values']['tendencia_nota'])) ? $tendencia_array[$form_state['storage']['values']['tendencia_nota']] : (isset($tendencia_nota) ? $tendencia_nota : NULL);
	$form['contenedor']['inferior']['tendencia_nota'] = array(
		'#type' => 'token',
		'#value' =>  $tendencia_nota,
	);

	$form['contenedor']['inferior']['tendencia'] = array(
		'#type' => 'textfield',
		'#title' => 'Tendencia de la nota: ',
		'#value' => ($tendencia_nota == 'A favor' ? "A favor" : ($tendencia_nota == 'En contra' ? "En contra" : NULL) ),
		'#prefix' => '<div id="tendencia_nota">',
		'#suffix' => '</div>',
		'#attributes' => array('readonly' => '', 'class' => ($tendencia_nota == 'A favor' ? array('favor') : ($tendencia_nota == 'En contra' ? array('contra') : NULL) ) ),
		'#size' => '4',
	);



	//encabezado elementos discursivos
	$tema_div = theme('html_tag', array('element' => array('#tag' => 'div','#attributes' => array('id' => 'tema'),'#value' => 'Tema')));
	$matriz_div = theme('html_tag', array('element' => array('#tag' => 'div','#attributes' => array('id' => 'matriz'),'#value' => 'Matriz')));
	$tendencia_div = theme('html_tag', array('element' => array('#tag' => 'div','#attributes' => array('id' => 'tendencia'),'#value' => 'Tendencia')));
	$cuerpo_argumentativo_div = theme('html_tag', array('element' => array('#tag' => 'div','#attributes' => array('id' => 'cuerpo_argumentativo'),'#value' => 'Cuerpo Argumentativo')));
	$actor_div = theme('html_tag', array('element' => array('#tag' => 'div','#attributes' => array('id' => 'actor'),'#value' => 'Actor')));
	$encabezado = theme('html_tag', array('element' => array('#tag' => 'div', '#attributes' => array('id' => 'encabezado'), '#value' => $tema_div . $matriz_div . $tendencia_div . $cuerpo_argumentativo_div . $actor_div)));

	$form['contenedor']['inferior']['encabezado'] = array(
		'#markup' => $encabezado
	);






  $form['contenedor']['inferior']['elementos_discursivos'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="elementos_discursivos">',
		'#suffix' => '</div>',
		'#tree' => TRUE,
	);

	for ($i = 0; $i < $filas_discursivo; $i++) {
		$form['contenedor']['inferior']['elementos_discursivos'][$i] = array(
			'#type' => 'fieldset',
			'#prefix' => '<div id="fila_discursivo_'. $i .'" class="fila_discursivo">',
			'#suffix' => '</div>',
		);
		$form['contenedor']['inferior']['elementos_discursivos'][$i]["tema"] = array(
			'#type' => 'textfield',
			'#attributes' => array('autocomplete' =>'off'),
			'#default_value' => !empty($form_state['storage']['values']["elementos_discursivos"][$i]['tema']) ? $form_state['storage']['values']["elementos_discursivos"][$i]['tema'] : (isset($tema[$i]) ? $tema[$i] : NULL),
			'#size' => 12,
			'#maxlength' => 128,
			'#autocomplete_path' => 'notas/autocomplete/tema/0/0',
		);
		$form['contenedor']['inferior']['elementos_discursivos'][$i]["matriz"] = array(
			'#type' => 'textfield',
			'#attributes' => array('autocomplete' =>'off'),
			'#default_value' => !empty($form_state['storage']['values']["elementos_discursivos"][$i]["matriz"]) ? $form_state['storage']['values']["elementos_discursivos"][$i]["matriz"] : $matriz[$i],
			'#size' => 12,
			'#maxlength' => 128,
			'#autocomplete_path' => "notas/autocomplete/matriz/0/0",
		);
		$form['contenedor']['inferior']['elementos_discursivos'][$i]["tendencia"] = array(
			'#type' => 'select',
			'#attributes' => array('autocomplete' =>'off'),
			'#default_value' => !empty($form_state['storage']['values']["elementos_discursivos"][$i]["tendencia"]) ? $form_state['storage']['values']["elementos_discursivos"][$i]["tendencia"] : (isset($tendencia[$i]) ? $tendencia[$i] : 0),
			'#options' => $tendencia_array,
		);
		$form['contenedor']['inferior']['elementos_discursivos'][$i]["cuerpo_argumentativo"] = array(
			'#type' => 'textarea',
			'#attributes' => array('autocomplete' =>'off'),
			'#default_value' => !empty($form_state['storage']['values']['elementos_discursivos'][$i]["cuerpo_argumentativo"]) ? $form_state['storage']['values']['elementos_discursivos'][$i]["cuerpo_argumentativo"] : $cuerpo_argumentativo[$i],
			'#size' => 12,
			'#maxlength' => 128,
			'#rows' => 1,
			'#cols' => 17,
			'#maxlength' => 1000,
			'#resizable' => FALSE,
		);
		$form['contenedor']['inferior']['elementos_discursivos'][$i]["actor"] = array(
			'#type' => 'textfield',
			'#attributes' => array('autocomplete' =>'off'),
			'#default_value' => !empty($form_state['storage']['values']["elementos_discursivos"][$i]["actor"]) ? $form_state['storage']['values']["elementos_discursivos"][$i]["actor"] : $actor[$i],
			'#size' => 12,
			'#maxlength' => 128,
			'#autocomplete_path' => "notas/autocomplete/actor/0/0",
		);

		$form['contenedor']['inferior']['elementos_discursivos'][$i]["borrar_fila_discursivo"] = array(
			'#type' => 'image_button',
			'#description' => 'Borrar fila discursiva',
			'#src' => 'sites/all/themes/segana/images/delete.png',
			'#return_value' => "borrar_fila_discursivo_$i",
			'#submit' => array('vistas_nota_form_submit'),
			'#prefix' => '<div id="borrar_fila_discursivo">',
			'#suffix' => '</div>',
			'#ajax' => array(
				'event' => 'click',
				'callback' => 'form_vistas_notas_fila_discursivo_callback',
				'wrapper' => 'elementos_discursivos',
				'method' => 'html',
				'effect' => 'fade',
				'progress' => array('type' => 'none'),
				//'prevent' => 'submit click mousedown',
			),
			'#disabled' => ($filas_discursivo > 1) ? FALSE : TRUE,
			'#limit_validation_errors' => array(array('filas_discursivo'), array('elementos_discursivos')),
		);

	}



	$form['contenedor']['inferior']['botones'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="botones">',
		'#suffix' => '</div>',
	);

	$form['contenedor']['inferior']['botones']['agregar_fila_discursivo'] = array(
		'#type' => 'image_button',
		'#description' => 'Agregar otro tema',
		'#src' => 'sites/all/themes/segana/images/add.png',
		'#return_value' => 'agregar_fila_discursivo',
		'#submit' => array('vistas_nota_form_submit'),
		'#ajax' => array(
			'event' => 'click',
			'callback' => 'form_vistas_notas_fila_discursivo_callback',
			'wrapper' => 'elementos_discursivos',
			'method' => 'html',
			'effect' => 'fade',
			'progress' => array('type' => 'none'),
			//'prevent' => 'submit click mousedown',
		),
			'#limit_validation_errors' => array(array('filas_discursivo'), array('elementos_discursivos')),
	);

	$form['contenedor']['inferior']['botones']['reset'] = array(
		'#type' => 'submit',
		'#value' => t('Reset'),
		'#return_value' => t('Reset'),
//		'#attributes' => array('onclick' => 'this.form.reset(); return false;'),
	);
  $form['#prefix'] = '<div id="formwrapper">';
  $form['#suffix'] = '</div>';

	//$form_id = str_replace('_', '-', $form_state['build_info']['form_id']);
	$form['contenedor']['inferior']['botones']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Guardar'),
		'#return_value' => t('Guardar'),
		'#submit' => array('vistas_nota_form_submit'),
		'#prefix' => '<div id="guardar">',
		'#suffix' => '</div>',
		'#ajax' => array(
			'event' => 'click',
			'callback' => 'forma_vista_notas_submit_callback',
			'wrapper' => 'formwrapper',
			'method' => 'replace',
			'effect' => 'fade',
			'progress' => array('type' => 'none'),
			//'prevent' => 'submit click mousedown',
		),
	);
	$form["wrapper2"] = array("#markup" => "<div class='inline-messages'></div>");
	return $form;
}


function vistas_nota_form_submit($form, &$form_state) {

	$pressed_button = check_plain($form_state['triggering_element']['#return_value']);
	switch ($pressed_button) {
		case 'Reiniciar':
			$form_state['rebuild'] = TRUE;
			unset($form_state['storage']);
			break;
		case 'agregar_fila_discursivo':
			$form_state['storage']['values'] = $form_state['values'];
			$form_state['storage']['values']['filas_discursivo']++;
			$form_state['rebuild'] = TRUE;
			break;
		case (strpos($pressed_button, "borrar_")):
			$variables = explode('_', $pressed_button);
			$fila_numero = $variables[3];
			$form_state['storage']['values'] = $form_state['values'];
			$form_state['storage']['values']['filas_discursivo']--;

			unset($form_state['input']['elementos_discursivos'][$fila_numero]);
			$form_state['input']['elementos_discursivos'] = array_values($form_state['input']['elementos_discursivos']);
			$form_state['rebuild'] = TRUE;
			break;
		case 'Guardar':

			//chequear filas discursivo 
			for ($i=0; $i < $form_state['values']['filas_discursivo']; $i++) { 
				if (empty($form_state['values']['elementos_discursivos'][$i]['tema'])) {
					unset($form_state['input']['elementos_discursivos'][$i]);
					$form_state['input']['elementos_discursivos'] = array_values($form_state['input']['elementos_discursivos']);
					
					unset($form_state['values']['elementos_discursivos'][$i]);
					$form_state['values']['elementos_discursivos'] = array_values($form_state['values']['elementos_discursivos']);
					$form_state['storage']['values']['filas_discursivo']--;
				}
			}

			// tendencia de la nota

			$tendencia_nota = 0;
			for ($i=0; $i < $form_state['values']['filas_discursivo']; $i++) {
				// Anular valores de tendencia que no tienen matriz definida
				if (empty($form_state['values']['elementos_discursivos'][$i]['matriz'])) {
					unset($form_state['values']['elementos_discursivos'][$i]['tendencia']);
					unset($form_state['input']['elementos_discursivos'][$i]['tendencia']);
				}

				if ($form_state['values']['elementos_discursivos'][$i]['tendencia'] != 0){
					$tendencia_term = taxonomy_term_load($form_state['values']['elementos_discursivos'][$i]['tendencia']);
					$tendencia = $tendencia_term->name;
					if ($tendencia == 'A favor'){
						$tendencia_nota++;
					}
					else {
						$tendencia_nota--;
					}
				}
			}
			if ($tendencia_nota > 0){
				$form_state['values']['tendencia_nota'] = key(taxonomy_get_term_by_name('A favor', 'tendencia'));
			}
			elseif($tendencia_nota < 0) {
				$form_state['values']['tendencia_nota'] = key( taxonomy_get_term_by_name('En contra', 'tendencia'));
			}
			else {
				unset($form_state['values']['tendencia_nota']);
			}


			//calculo de centimetros

			if (!empty($form_state['values']['horizontal_cm']) && !empty($form_state['values']['vertical_cm'])) {
				$form_state['values']['centimetros'] = $form_state['values']['horizontal_cm'] * $form_state['values']['vertical_cm'];
			}

			if (!empty($form_state['values']['horizontal_primera_pagina']) && !empty($form_state['values']['vertical_primera_pagina'])) {
				$form_state['values']['centimetros_primera_pagina'] = $form_state['values']['horizontal_primera_pagina'] * $form_state['values']['vertical_primera_pagina'];
			}

			if (isset($form_state['values']['nid'])) {
				$node = node_load($form_state['values']['nid']);
				$form_state['storage']['values'] = $form_state['values'];
			}
			else {
				$node = new stdClass();
				$node->type = 'nota';
				node_object_prepare($node);
				unset($form_state['input']);
				unset($form_state['storage']);
			}
			$language = LANGUAGE_NONE;
			$node->title = $form_state['values']['title'];

			$campos = field_info_instances("node",'nota');
			foreach ($campos as $campo => $value) {
				if ($value['display']['default']['type'] == 'taxonomy_term_reference_link'){
					$tipo = 'tid';
				}
				else {
					$tipo = 'value';
				}
				$campo_forma = ($campo == 'body') ? $campo : substr($campo, 6);
				$elementos_discursivos = array('tema', 'matriz', 'tendencia', 'actor', 'cuerpo_argumentativo');
				if (in_array($campo_forma, $elementos_discursivos)) {
					for ($i=0; $i < $form_state['values']['filas_discursivo']; $i++) {
						if ((!empty($form_state['values']['elementos_discursivos'][$i][$campo_forma]))) {
							$node->{$campo}[$language][$i][$tipo] = ($campo_forma == 'tendencia' || $campo_forma == 'cuerpo_argumentativo') ? $form_state['values']['elementos_discursivos'][$i][$campo_forma] : key(taxonomy_get_term_by_name($form_state['values']['elementos_discursivos'][$i][$campo_forma], $campo_forma));
						}
						else {
							unset($node->{$campo}[$language][$i]);
						}
					}
				}
				elseif (!empty($form_state['values'][$campo_forma])) {
					if ( $campo_forma == 'area' || $campo_forma == 'medio' || $campo_forma == 'usuario') {
						$node->{$campo}[$language][0][$tipo] = key(taxonomy_get_term_by_name($form_state['values'][$campo_forma], $campo_forma));
					}
					elseif ($campo_forma == 'fecha') {
						$node->{$campo}[$language][0][$tipo] = strtotime($form_state['values'][$campo_forma]);
					}
					else {
						$node->{$campo}[$language][0][$tipo] = $form_state['values'][$campo_forma];
					}
				}
				else {
					unset($node->{$campo}[$language]);
				}

			}
			node_save($node);
			if (isset($form_state['values']['nid'])) {
				drupal_set_message("Nota con titulo '" . $form_state['values']['title'] . "' actualizada");
			}
			else {
				$titulo = l($form_state['values']['title'], 'notas/ver/' . $node->nid, array('attributes' => array('class' => array('ver'))));
				drupal_set_message("Nota con titulo '" . $titulo . "' insertada con nid: " . $node->nid);
			}
			$form_state['rebuild'] = TRUE;
			break;
	}
}

function vistas_nota_form_validate($form, &$form_state) {
	//verificar valores 
	$elementos = array('medio', 'area', 'usuario');

	foreach ($elementos as $value) {
		if(!empty($form_state['values'][$value])) {
			$tid = buscar_termino($form_state['values'][$value], $value);
			if (empty($tid)) {
				form_set_error($value, "El valor '". $form_state['values'][$value] ."' es invalido para el campo $value");
			}
		}	
	}
	
	$campos_numericos = array('horizontal_cm', 'vertical_cm', 'horizontal_primera_pagina', 'vertical_primera_pagina');

	foreach ($campos_numericos as $value) {
		if (!empty($form_state['values'][$value]) && !is_numeric($form_state['values'][$value])){
			$titulo = isset($form_state['complete form']['contenedor']['medio']['impreso'][$value]['#title']) ? $form_state['complete form']['contenedor']['medio']['impreso'][$value]['#title'] : $form_state['complete form']['contenedor']['medio']['impreso']['primera_pagina'][$value]['#title'];
			form_set_error($value, "El valor '". $form_state['values'][$value] ."' es invalido para el campo $titulo, use un punto(.) para decimales");
		}
	}


	$filas_discursivo = $form_state['values']['filas_discursivo'];

	$elementos_discursivos = array('tema', 'matriz', 'actor');
	for ($i=0; $i < $filas_discursivo; $i++) { 
		foreach ($elementos_discursivos as $value) {
			if(!empty($form_state['values']['elementos_discursivos'][$i][$value])) {
				$tid = buscar_termino($form_state['values']['elementos_discursivos'][$i][$value], $value);
				if (empty($tid)) {
					form_set_error("elementos_discursivos][$i][" . $value, "El valor '". $form_state['values']['elementos_discursivos'][$i][$value] ."' es invalido para el campo $value");
				}
			}	
		}

		//Si se define un elemento en una fila discursiva a parte de un tema se deben definir todos los demas
		if(!empty($form_state['values']['elementos_discursivos'][$i]['tema'])) {
			$elementos_fila_discursivo = array('matriz', 'tendencia', 'cuerpo_argumentativo', 'actor');
			$cantidad_elementos_definidos = 0;
			foreach ($elementos_fila_discursivo as $value) {
				if (!empty($form_state['values']['elementos_discursivos'][$i][$value]) || $form_state['values']['elementos_discursivos'][$i][$value] != 0){
					// si el unico elemento definido es la tendencia este es automaticamente descartado si no se ha definido la matriz
					if ($value != 'tendencia'){
						$cantidad_elementos_definidos++;
					}
				}
			}

			//si alguno de los elemento matriz, cuerpo_argumentativo o matriz estan definido, el resto tambien lo debe estar
			
			if (($cantidad_elementos_definidos < 3) && ($cantidad_elementos_definidos > 0)) {
				foreach ($elementos_fila_discursivo as $value) {
					if (!empty($form_state['values']['elementos_discursivos'][$i][$value]) || $form_state['values']['elementos_discursivos'][$i][$value] != 0){
						form_set_error("elementos_discursivos][$i][" . $value, '');
					}
				}
				$tema = $form_state['values']['elementos_discursivos'][$i]['tema'];
				form_set_error($tema, "Faltan elementos por definir en la fila $i ($tema)");
			}
			// la tendencia debe estar definida si todos los demas elementos en la fila discursiva lo estan
			elseif($cantidad_elementos_definidos == 3 && $form_state['values']['elementos_discursivos'][$i]['tendencia'] == 0) {
				$matriz = $form_state['values']['elementos_discursivos'][$i]['matriz'];
				form_set_error("elementos_discursivos][$i][tendencia", "La tendencia debe ser definida para la matriz '$matriz' en la fila $i");
			}

		}

	}



	//Verificar que los valores de tendencia no dan un resultado neutro
	$tendencia_nota = 0;
	$cantidad_tendencia = 0;
	for ($i=0; $i < $form_state['values']['filas_discursivo']; $i++) {
		if (($form_state['values']['elementos_discursivos'][$i]['tendencia'] != 0) && !empty($form_state['values']['elementos_discursivos'][$i]['matriz'])){
			$tendencia_term = taxonomy_term_load($form_state['values']['elementos_discursivos'][$i]['tendencia']);
			$tendencia = $tendencia_term->name;
			$cantidad_tendencia++;
			if ($tendencia == 'A favor'){
				$tendencia_nota++;
			}
			else {
				$tendencia_nota--;
			}
		}
	}
	if ($tendencia_nota == 0 && $cantidad_tendencia > 0){
		form_set_error('','La tendencia de la nota no puede ser neutra');
		form_set_value($form['contenedor']['inferior']['tendencia'], 'a', $form_state);
	}



	//form_set_error('','mensaje de prueba');
}


function form_vistas_notas_fila_discursivo_callback($form, $form_state) {
	return array(
	    '#type' => 'ajax',
	    '#commands' => array(
	      ajax_command_replace("#elementos_discursivos", render($form['contenedor']['inferior']['elementos_discursivos'])),
    	)
  );
}

function forma_vista_notas_submit_callback($form, $form_state) {
	$messages = theme('status_messages');
	/*//return theme('status_messages'); 
	$commands[] = ajax_command_before(NULL, '<div id="messages">' . theme('status_messages') . '</div>');
	return array('#type' => 'ajax', '#commands' => $commands);*/
	if($messages){
    $form["wrapper"] = array("#markup" => "<div class='inline-messages'>$messages</div>");
    $form["wrapper2"] = array("#markup" => "<div class='inline-messages'>$messages</div>");
  }
  return $form;

}

function form_vistas_nota_tipo_titulo_callback($form, $form_state) {
	return array(
	    '#type' => 'ajax',
	    '#commands' => array(
	      ajax_command_replace("#titular", render($form['contenedor']['titular'])),
    	)
  );
}

function form_vistas_nota_tipo_medio_callback($form, $form_state) {
	return array(
	    '#type' => 'ajax',
	    '#commands' => array(
	      ajax_command_replace("#medio", render($form['contenedor']['medio'])),
    	)
  );
}



function form_vistas_nota_tipo_carga_callback($form, $form_state) {
	return array(
	    '#type' => 'ajax',
	    '#commands' => array(
	      ajax_command_replace("#primera_pagina", render($form['contenedor']['medio']['impreso']['primera_pagina'])),
    	)
  );
}

function form_vistas_nota_tipo_titulo_primera_pagina_callback($form, $form_state) {
	return array(
	    '#type' => 'ajax',
	    '#commands' => array(
	      ajax_command_replace("#titular_primera_pagina", render($form['contenedor']['medio']['impreso']['primera_pagina']['titular_primera_pagina'])),
    	)
  );
}

function form_vistas_nota_centimetros_callback($form, $form_state) {
	if (!empty($form_state['values']['horizontal_cm']) && !empty($form_state['values']['vertical_cm'])) {
		$form['contenedor']['medio']['impreso']['centimetros']['#value'] = $form_state['values']['horizontal_cm'] * $form_state['values']['vertical_cm'];
	}
	return array(
	    '#type' => 'ajax',
	    '#commands' => array(
	      ajax_command_replace("#centimetros", render($form['contenedor']['medio']['impreso']['centimetros'])),
    	)
  );
}

function form_vistas_nota_centimetros_primera_pagina_callback($form, $form_state) {
	if (!empty($form_state['values']['horizontal_primera_pagina']) && !empty($form_state['values']['vertical_primera_pagina'])) {
		$form['contenedor']['medio']['impreso']['primera_pagina']['centimetros_primera_pagina']['#value'] = $form_state['values']['horizontal_primera_pagina'] * $form_state['values']['vertical_primera_pagina'];
	}
	return array(
	    '#type' => 'ajax',
	    '#commands' => array(
	      ajax_command_replace("#centimetros_primera_pagina", render($form['contenedor']['medio']['impreso']['primera_pagina']['centimetros_primera_pagina'])),
    	)
  );
}



function vistas_discursivo_form($form, &$form_state, $tid = NULL, $elemento_discursivo = NULL) {
	$form = array();

  	//determinar si es un elemento nuevo o editar(cargar valores)
	$vocabulary = taxonomy_vocabulary_machine_name_load($elemento_discursivo);
	$nombre_elemento_discursivo = $vocabulary->name;

	if ( ($tid) && empty($form_state['clicked_button']['#value']) ){
		$term = taxonomy_term_load($tid);
		$name = $term->name;
		$description = isset($term->description) ? $term->description : NULL;

		if ($elemento_discursivo == 'actor'){
			$rol = isset($term->field_rol[LANGUAGE_NONE][0]['value']) ? $term->field_rol[LANGUAGE_NONE][0]['value'] : NULL;
		}


	}
	$form["wrapper"] = array("#markup" => "<div class='inline-messages'></div>");

	$form['contenedor'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="contenedor">',
		'#suffix' => '</div>',
	);

	$form['contenedor']['tid'] = array(
		'#type' => 'token',
		'#value' => isset($form_state['storage']['values']['tid']) ? $form_state['storage']['values']['tid'] : (isset($tid) ? $tid : NULL),
	);
		$form['contenedor']['elemento_discursivo'] = array(
		'#type' => 'token',
		'#value' => isset($form_state['storage']['values']['elemento_discursivo']) ? $form_state['storage']['values']['elemento_discursivo'] : (isset($elemento_discursivo) ? $elemento_discursivo : NULL),
	);

	$form['contenedor']['name'] = array(
		'#type' => 'textfield',
		'#prefix' => '<div class="nombre_discursivo">',
		'#suffix' => '</div>',
		'#title' => ucfirst($nombre_elemento_discursivo),
		'#attributes' => array('autocomplete' =>'off'),
		'#default_value' => !empty($form_state['storage']['values']['name']) ? $form_state['storage']['values']['name'] : (isset($name) ? $name : NULL),
		'#size' => 70,
		'#maxlength' => 256,
		'#required' => TRUE,
	);
	if ($tid || isset($form_state['storage']['values']['tid'])) {
		
		$nids_count = count(taxonomy_select_nodes($tid, FALSE));
		$plural = ($nids_count > 1 ? 's' : '');
		$link = l($nids_count,'notas', array('attributes' => array('class' => 'ver', 'target'=>'_blank', 'title' => 'Ver notas asociadas'), 'query' => array($elemento_discursivo => $name)));


		$link_image = l(theme_image(array('path' => '/sites/all/themes/segana/images/listado.png')),'notas', array('attributes' => array('class' => 'ver',  'target'=>'_blank', 'title' => 'Ver notas asociadas'), 'query' => array($elemento_discursivo => $name), 'html' => TRUE));

		$notas_asociadas = theme('html_tag', array(
	    'element' => array(
	      '#tag' => 'div',
	      '#attributes' => array(
	        'id' => 'notas_asociadas',
	       ),
	      '#value' => 'Hay '. $link . ' nota' . $plural . ' asociada' . $plural . (($elemento_discursivo == 'matriz') ? ' a la ' : ' al ') . $elemento_discursivo . $link_image,
	    ),
	  ));

		$form['contenedor']['notas_asociadas'] = array(
			'#markup' => $notas_asociadas,
		);
	}

	$form['contenedor']['description'] = array(
		'#type' => 'textarea',
		'#title' => 'Descripción',
		'#attributes' => array('autocomplete' =>'off'),
		'#prefix' => '<div class="description">',
		'#suffix' => '</div>',
		'#default_value' => !empty($form_state['storage']['values']['description']) ? $form_state['storage']['values']['description'] : $description,
		'#size' => 50,
		'#maxlength' => 2000,
		'#resizable' => FALSE,
		'#required' => TRUE,
	);

	if ($elemento_discursivo == 'actor') {
		$form['contenedor']['rol'] = array(
			'#type' => 'textfield',
			'#prefix' => '<div class="rol">',
			'#suffix' => '</div>',
			'#title' => Rol,
			'#attributes' => array('autocomplete' =>'off'),
			'#default_value' => !empty($form_state['storage']['values']['rol']) ? $form_state['storage']['values']['rol'] : (isset($rol) ? $rol : NULL),
			'#size' => 70,
			'#maxlength' => 256,
			'#required' => TRUE,
		);
	}

	if ($tid || isset($form_state['storage']['values']['tid'])) {
		$form['contenedor']['fusion'] =array(
			'#type' => 'fieldset',
			'#prefix' => '<div class="fusionar" id="fusionar">',
			'#suffix' => '</div>',
		);
		$form['contenedor']['fusion']['fusionar'] = array(
			'#type' => 'textfield',
			'#title' => 'Fusionar dentro del ' . $nombre_elemento_discursivo . ' actual',
			'#attributes' => array('autocomplete' =>'off'),
			'#size' => 70,
			'#maxlength' => 256,
			'#autocomplete_path' => 'notas/autocomplete/' . $elemento_discursivo .'/0/0',
		);
		$form['contenedor']['fusion']['boton_fusionar'] = array(
			'#type' => 'image_button',
			'#description' => 'Fusionar',
			'#src' => 'sites/all/themes/segana/images/enter.png',
			'#return_value' => "fusionar",
			'#submit' => array('vistas_discursivo_form_submit'),
			'#prefix' => '<div id="boton_fusionar">',
			'#suffix' => '</div>',
			'#attributes' => array('title' => 'Ejecutar fusion con este elemento'),
			'#ajax' => array(
				'event' => 'click',
				'callback' => 'form_vistas_discursivo_fusionar_callback',
				'wrapper' => 'fusionar',
				'method' => 'html',
				'effect' => 'fade',
				'progress' => array('type' => 'none'),
				//'prevent' => 'submit click mousedown',
			),
			'#limit_validation_errors' => array(array('fusionar')),
		);
	}


	$form['contenedor']['botones'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="botones">',
		'#suffix' => '</div>',
	);

	$form['contenedor']['botones']['reset'] = array(
		'#type' => 'submit',
		'#value' => t('Reset'),
		'#return_value' => t('Reset'),
//		'#attributes' => array('onclick' => 'this.form.reset(); return false;'),
	);
	$form['contenedor']['botones']['eliminar'] = array(
		'#type' => 'submit',
		'#value' => t('Eliminar'),
		'#return_value' => t('Eliminar'),
		'#submit' => array('vistas_discursivo_form_submit'),
		'#ajax' => array(
			'event' => 'click',
			'callback' => 'form_vistas_discursivo_submit_callback',
			'wrapper' => 'formwrapper',
			'method' => 'replace',
			'effect' => 'fade',
			'progress' => array('type' => 'none'),
		),
		'#limit_validation_errors' => array(array('tid')),
	);
  $form['#prefix'] = '<div id="formwrapper">';
  $form['#suffix'] = '</div>';

	//$form_id = str_replace('_', '-', $form_state['build_info']['form_id']);
	$form['contenedor']['botones']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Guardar'),
		'#return_value' => t('Guardar'),
		'#submit' => array('vistas_discursivo_form_submit'),
		'#prefix' => '<div id="guardar">',
		'#suffix' => '</div>',
		'#ajax' => array(
			'event' => 'click',
			'callback' => 'form_vistas_discursivo_submit_callback',
			'wrapper' => 'formwrapper',
			'method' => 'replace',
			'effect' => 'fade',
			'progress' => array('type' => 'none'),
			//'prevent' => 'submit click mousedown',
		),
	);
	return $form;

}


function vistas_discursivo_form_submit($form, &$form_state) {
	$pressed_button = check_plain($form_state['triggering_element']['#return_value']);
	switch ($pressed_button) {
		case 'Fusionar':
			$form_state['storage']['values'] = $form_state['values'];
			$form_state['rebuild'] = TRUE;
			break;
		case 'Eliminar':
			$form_state['storage']['values'] = $form_state['values'];
			$form_state['rebuild'] = TRUE;
			break;
		case 'Reiniciar':
			$form_state['rebuild'] = TRUE;
			unset($form_state['storage']);
			break;
		case 'Guardar':
			$language = LANGUAGE_NONE;
			$fecha = time();
			if (isset($form_state['values']['tid'])) {
				$term = taxonomy_term_load($form_state['values']['tid']);
				$form_state['storage']['values'] = $form_state['values'];
			}
			else {
				$term = new stdClass();
				$vocabulary = taxonomy_vocabulary_machine_name_load($form_state['values']['elemento_discursivo']);
				$term->vid = $vocabulary->vid;
				$term->field_fecha[$language][0][value] = $fecha;
			}

			global $user;

			$term->name = $form_state['values']['name'];
			$term->description = $form_state['values']['description'];
			$term->field_fecha_actualizacion[$language][0]['value'] = $fecha;
			$term->field_user[$language][0]['target_id'] = $user->uid;
			$term->field_user[$language][0]['target_type'] = 'user';
			if ($form_state['values']['elemento_discursivo'] == 'actor'){
				$term->field_rol[$language][0]['value'] = $form_state['values']['rol'];
			}
			taxonomy_term_save($term);
			
			if (isset($form_state['values']['tid'])) {
				drupal_set_message(ucfirst($form_state['values']['elemento_discursivo']) ." '" . $form_state['values']['title'] . "' actualizada");
			}
			else {
				$titulo = l($form_state['values']['name'], 'elementos_discursivos/ver/' . $term->tid, array('attributes' => array('class' => array('ver'))));
				drupal_set_message(ucfirst($form_state['values']['elemento_discursivo']) ." '" . $titulo . "' insertada con tid: " . $term->tid);
			}

			$form_state['rebuild'] = TRUE;
			break;
	}
}



function vistas_discursivo_form_validate($form, &$form_state) {
	if ($form_state['triggering_element']['#return_value'] == 'Eliminar') {
		$nids = taxonomy_select_nodes($form_state['values']['tid'], FALSE);
		if (!empty($nids)){
			$nids_count = count($nids);
			$plural = ($nids_count > 1 ? 's' : '');
			$link = l($nids_count,'notas', array('attributes' => array('class' => 'ver',  'target'=>'_blank'), 'query' => array($form_state['values']['elemento_discursivo'] => $form_state['input']['name'])));
			drupal_set_message("Hay $link nota$plural asociada$plural " . (($form_state['values']['elemento_discursivo'] == 'matriz') ? ' a la ' : ' al ') . " " . $form_state['values']['elemento_discursivo'], 'error');
		}
	}
}

function form_vistas_discursivo_submit_callback($form, $form_state) {
	$messages = theme('status_messages');
	/*//return theme('status_messages'); 
	$commands[] = ajax_command_before(NULL, '<div id="messages">' . theme('status_messages') . '</div>');
	return array('#type' => 'ajax', '#commands' => $commands);*/
	if($messages){
    $form["wrapper"] = array("#markup" => "<div class='inline-messages'>$messages</div>");
  }
  return $form;

}
function form_vistas_discursivo_fusionar_callback($form, $form_state) {
	return array(
	    '#type' => 'ajax',
	    '#commands' => array(
	      ajax_command_replace("#fusionar", render($form['contenedor']['fusion'])),
    	)
  );
}
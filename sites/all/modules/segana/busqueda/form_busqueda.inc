<?php

function busqueda_form($form, &$form_state) {
  $form = array();
  $default_value = array();
  ctools_include('dependent');

  $form['busqueda'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="busqueda">',
		'#suffix' => '</div>',
	);

	$form['fecha'] = array(
		'#type' => 'fieldset',
		'#title' => 'FECHA',
		'#prefix' => '<div id="fecha">',
		'#suffix' => '</div>',
	);
	$rango_ano_inicio = 2008 - date('Y');
	$form['fecha']['fecha_desde'] = array(
		'#type' => 'date_popup',
		'#title' => t('desde el'),
		'#attributes' => array('autocomplete' =>'off'),
		'#date_format' => 'd-m-Y',
		'#date_year_range' => "$rango_ano_inicio:+0",
		'#size' => 6,
		'#default_value' => isset($form_state['storage']['values']['fecha_desde']) ? $form_state['storage']['values']['fecha_desde'] : date('Y-m-d'),
	);
	$form['fecha']['fecha_hasta'] = array(
		'#type' => 'date_popup',
		'#title' => t('hasta el'),
		'#attributes' => array('autocomplete' =>'off'),
		'#date_format' => 'd-m-Y',
		'#default_value' => isset($form_state['storage']['values']['fecha_hasta']) ? $form_state['storage']['values']['fecha_hasta'] : 0,
		'#date_year_range' => "$rango_ano_inicio:+0",
		'#size' => 6,
	);
	$form['busqueda']['opciones'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="edit-opciones">',
		'#suffix' => '</div>',
	);

	$form['busqueda']['opciones']['izquierda'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="edit-opciones-izquierda">',
		'#suffix' => '</div>',
	);

	$tipo_localizacion = tipo_localizacion();
	$tipo_localizacion_opcion = isset($form_state['values']['tipo_localizacion']) ? $form_state['values']['tipo_localizacion'] : key($tipo_localizacion);
	$form['busqueda']['opciones']['izquierda']['tipo_localizacion'] = array(
		'#type' => 'select',
		'#title' => t('Medios'),
		'#default_value' => $tipo_localizacion_opcion,
		'#options' => $tipo_localizacion,
		'#ajax' => array(
			'callback' => 'form_busqueda_tipo_localizacion_callback',
			'wrapper' => '',
			'progress' => array('type' => 'throbber'),
			'effect' => 'fade',
		),
	);
	if ($tipo_localizacion_opcion == -1){
		$form['busqueda']['opciones']['izquierda']['primera_pagina'] = array(
			'#type' => 'checkbox',
			'#title' => t('Primera Página'),
			'#prefix' => '<div id="primera_pagina">',
			'#suffix' => '</div>',
		);
	}

	// Si se cambia el tipo de localizacion reiniciar los valores de medios
	if (isset($form_state['triggering_element']['#name']) && $form_state['triggering_element']['#name'] == 'tipo_localizacion') {
		unset($form_state['input']['medios']);
		unset($form_state['values']['medios']);
	}
	$form['busqueda']['medios'] = array(
		'#type' => 'checkboxes',
		'#attributes' => array('class' => array('lista_medios'), 'id' => array('lista_medios')),
		'#title' => t('Lista de Medios '. (($tipo_localizacion_opcion == 0) ? '' : $tipo_localizacion[$tipo_localizacion_opcion]) ),
		'#default_value' => isset($form_state['values']['medios']) ? $form_state['values']['medios'] : $default_value,
		'#size' => 10,
		'#options' => medios($tipo_localizacion_opcion),
		'#prefix' => '<div id="medios-wrapper">',
		'#suffix' => '</div>',
	 );

	$tipo_informacion =tipo_informacion();
	$tipo_informacion_default = array_search('Noticia', $tipo_informacion);
	$form['busqueda']['opciones']['izquierda']['tipo_informacion'] = array(
		'#type' => 'select',
		'#title' => t('Tipo de Información'),
		'#default_value' => isset($form_state['storage']['values']['tipo_informacion']) ? $form_state['storage']['values']['tipo_informacion'] : $tipo_informacion_default,
		'#options' => $tipo_informacion,
	);
	$form['busqueda']['opciones']['izquierda']['opciones_graficos'] = array(
		'#type' => 'fieldset',
		'#title' => 'Opciones gráficos',
		'#collapsible' => TRUE, 
	  	'#collapsed' => TRUE,
	  	'#attributes' => array('id' => 'edit-opciones-graficos'),
		'#prefix' => '<div id="opciones_graficos">',
		'#suffix' => '</div>',
	);
	$form['busqueda']['opciones']['izquierda']['opciones_graficos']['opciones_simple'] = array(
		'#type' => 'fieldset',
		'#title' => 'Gráficos simples',
		'#collapsible' => TRUE, 
	  	'#collapsed' => TRUE,
	  	'#attributes' => array('id' => 'edit-opciones-simple'),
		'#prefix' => '<div id="opciones_simple">',
		'#suffix' => '</div>',
	);
	$form['busqueda']['opciones']['izquierda']['opciones_graficos']['opciones_simple']['conectores'] = array(
			'#type' => 'checkbox',
			'#title' => t('Usar conectores'),
			'#attributes' => (isset($form_state['storage']['values']['conectores']) && $form_state['storage']['values']['conectores'] == 1 ) ? array('checked' => 'checked') : NULL,
	);
	$form['busqueda']['opciones']['izquierda']['opciones_graficos']['opciones_simple']['otros'] = array(
			'#type' => 'checkbox',
			'#title' => t('Agrupar resto en otros'),
			'#attributes' => (isset($form_state['storage']['values']['otros']) && $form_state['storage']['values']['otros'] == 1 ) ? array('checked' => 'checked') : NULL,
	);

	$form['busqueda']['opciones']['izquierda']['opciones_graficos']['opciones_simple']['leyenda'] = array(
		'#type' => 'select',
		'#attributes' => array('id' => 'edit-leyenda'),
		'#options' => array('Sin leyenda', 'Leyenda en 1 columna', 'Leyenda en 2 columnas'),
		'#default_value' => isset($form_state['storage']['values']['leyenda']) ? $form_state['storage']['values']['leyenda'] : array(1),
	);
	$form['busqueda']['opciones']['izquierda']['opciones_graficos']['opciones_simple']['tipo-simple'] = array(
		'#type' => 'select',
		'#attributes' => array('id' => 'edit-tipo-simple'),
		'#options' => array(
			'column' => 'Columnas',
			'bar' => 'Barras',
		),
	);
	$form['busqueda']['opciones']['izquierda']['opciones_graficos']['opciones_lineales'] = array(
		'#type' => 'fieldset',
		'#title' => 'Gráficos lineales',
		'#collapsible' => TRUE, 
	  	'#collapsed' => FALSE,
	  	'#attributes' => array('id' => 'edit-opciones-lineales'),
		'#prefix' => '<div id="opciones_lineales">',
		'#suffix' => '</div>',
	);

	$form['busqueda']['opciones']['izquierda']['opciones_graficos']['opciones_lineales']['cantidad_lineal'] = array(
			'#type' => 'select',
			'#title' => t('Cantidad de elementos'),
			'#options' => array(2 => 2, 3 => 3, 4 => 4, 5 => 5, 6 => 6, 7 => 7, 8 => 8),
			'#default_value' => isset($form_state['values']['cantidad_lineal']) ? $form_state['values']['cantidad_lineal'] : 3,
	);

	$form['busqueda']['opciones']['izquierda']['opciones_graficos']['opciones_combinados'] = array(
		'#type' => 'fieldset',
		'#title' => 'Gráficos combinados',
		'#collapsible' => TRUE, 
	  	'#collapsed' => FALSE,
	  	'#attributes' => array('id' => 'edit-opciones-combinados'),
		'#prefix' => '<div id="opciones_combinados">',
		'#suffix' => '</div>',
	);

	$form['busqueda']['opciones']['izquierda']['opciones_graficos']['opciones_combinados']['tipo-combinado'] = array(
		'#type' => 'select',
		'#attributes' => array('id' => 'edit-tipo-combinado'),
		'#options' => array(
			'ninguno' => 'Ninguno',
			'bar' => 'Barras',
			'column' => 'Columnas',
		),
	);

	$form['busqueda']['opciones']['izquierda']['opciones_graficos']['opciones_combinados']['apiladas'] = array(
			'#type' => 'checkbox',
			'#title' => t('Apiladas'),
			'#default_value' => array(0),
			'#attributes' => (isset($form_state['storage']['values']['apiladas']) && $form_state['storage']['values']['apiladas'] == 1 ) ? array('checked' => 'checked') : NULL,
			'#process' => array('ctools_dependent_process'),
			'#dependency' => array('edit-tipo-combinado' => array('bar', 'column')),
	);
	$form['busqueda']['opciones']['izquierda']['opciones_graficos']['opciones_combinados']['cantidad_combinado'] = array(
			'#type' => 'select',
			'#title' => t('Cantidad de elementos'),
			'#options' => array(2 => 2, 3 => 3, 4 => 4, 5 => 5, 6 => 6, 7 => 7, 8 => 8),
			'#default_value' => isset($form_state['values']['cantidad_combinado']) ? $form_state['values']['cantidad_combinado'] : 5,
			'#process' => array('ctools_dependent_process'),
			'#dependency' => array('edit-tipo-combinado' => array('bar', 'column')),
	);

	$form['busqueda']['opciones']['izquierda']['opciones_graficos']['paleta_colores'] = array(
		'#type' => 'fieldset',
		'#title' => 'Paleta de colores',
		'#collapsible' => TRUE, 
	  	'#collapsed' => (isset($form_state['triggering_element']['#return_value']) && $form_state['triggering_element']['#return_value'] == 'reiniciar_colores') ? FALSE : TRUE,
	  	'#prefix' => '<div id="paleta_colores">',
	  	'#suffix' => '</div>',
	  	'#tree' => TRUE,
	);

	foreach (paleta() as $key => $color) {
		$form['busqueda']['opciones']['izquierda']['opciones_graficos']['paleta_colores'][$key] = array(
			'#type' => 'jquery_colorpicker',
			'#title' => t('Color ' . ($key+1) ),
			'#default_value' => substr($color, 1),
		);
	}

	$form['busqueda']['opciones']['izquierda']['opciones_graficos']['paleta_colores']['reiniciar_colores'] = array(
		'#type' => 'image_button',
		'#description' => 'Reiniciar colores',
		'#title' => 'Reiniciar colores',
		'#src' => 'sites/all/themes/segana/images/reset-button.png',
		'#return_value' => 'reiniciar_colores',
		'#submit' => array('busqueda_form_submit'),
		'#ajax' => array(
			'event' => 'click',
			'callback' => 'form_busqueda_reiniciar_colores_callback',
			'wrapper' => 'paleta_colores',
			'method' => 'html',
			'effect' => 'fade',
			'progress' => array('type' => 'none'),
		)
	);
	$form['busqueda']['opciones']['derecha'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="edit-opciones-derecha">',
		'#suffix' => '</div>',
	);

	$form['busqueda']['opciones']['derecha']['palabra_clave'] = array(
		'#type' => 'textarea',
		'#title' => t('Palabras Clave'),
		'#attributes' => array('autocomplete' =>'off'),
		'#default_value' => isset($form_state['storage']['values']['palabra_clave']) ? $form_state['storage']['values']['palabra_clave'] : NULL,
		'#rows' => 1,
		'#cols' => 17,
		'#maxlength' => 64,
		'#resizable' => FALSE,
	);

	if (isset($form_state['triggering_element']['#name'])) {
		switch ($form_state['triggering_element']['#name']) {
			case 'busqueda_exacta':
				unset($form_state['input']['cualquiera']);
				break;
			case 'cualquiera':
				unset($form_state['input']['busqueda_exacta']);
				break;
		}
	}
	$form['busqueda']['opciones']['derecha']['busqueda_exacta'] = array(
		'#type' => 'checkbox',
		'#title' => t('Búsqueda exacta'),
		'#default_value' => isset($form_state['storage']['values']['busqueda_exacta']) ? $form_state['storage']['values']['busqueda_exacta'] : NULL,
		'#prefix' => '<div id="busqueda_exacta">',
		'#suffix' => '</div>',
		'#ajax' => array(
			'callback' => 'form_busqueda_palabra_clave_callback',
			'wrapper' => 'cualquiera',
			'progress' => array('type' => 'none'),
			'effect' => 'none',
		),
	 );	
	$form['busqueda']['opciones']['derecha']['cualquiera'] = array(
		'#type' => 'checkbox',
		'#title' => t('Cualquiera de las palabras'),
		'#default_value' => isset($form_state['storage']['values']['cualquiera']) ? $form_state['storage']['values']['cualquiera'] : NULL,
		'#prefix' => '<div id="cualquiera">',
		'#suffix' => '</div>',
		'#ajax' => array(
			'callback' => 'form_busqueda_palabra_clave_callback',
			'wrapper' => 'busqueda_exacta',
			'progress' => array('type' => 'none'),
			'effect' => 'none',
			'method' => 'replace',
		),
	 );

	//elementos para el autocomplete
	if (isset($form_state['storage']['values']['datos_area'])) {
		$areas = implode(',', array_keys(unserialize(descifrar($form_state['storage']['values']['datos_area']))));
	}
	if (isset($form_state['storage']['values']['datos_tema'])) {
		$temas = implode(',', array_keys(unserialize(descifrar($form_state['storage']['values']['datos_tema']))));
	}
	if (isset($form_state['storage']['values']['datos_matriz'])) {
		$matrices = implode(',', array_keys(unserialize(descifrar($form_state['storage']['values']['datos_matriz']))));
	}
	if (isset($form_state['storage']['values']['datos_actor'])) {
		$actores = implode(',', array_keys(unserialize(descifrar($form_state['storage']['values']['datos_actor']))));
	}

	// hash aleatorio para evitar el microcache de la lista del autocomplete cuando las condiciones han cambiado
	$hash = generateHash();

	$form['busqueda']['opciones']['derecha']['wrapper_area'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="edit-opciones-area">',
		'#suffix' => '</div>',
	);
	$form['busqueda']['opciones']['derecha']['wrapper_area']['area'] = array(
		'#type' => 'textfield',
		'#title' => 'Área',
		'#attributes' => array('autocomplete' =>'off'),
		'#size' => 25,
		'#maxlength' => 128,
		'#autocomplete_path' => 'busqueda/autocomplete_area',
  );
	$form['busqueda']['opciones']['derecha']['wrapper_area']['agregar_area'] = array(
		'#type' => 'image_button',
		'#description' => 'Agregar otra area',
		'#src' => 'sites/all/themes/segana/images/add.png',
		'#return_value' => 'agregar_area',
		'#submit' => array('busqueda_form_submit'),
		'#ajax' => array(
			'event' => 'click',
			'callback' => 'form_busqueda_elemento_callback',
			'wrapper' => 'edit-opciones-area',
			'method' => 'html',
			'effect' => 'none',
			'progress' => array('type' => 'none'),
		)
	);
	$form['busqueda']['opciones']['derecha']['wrapper_area']['datos_area'] = array(
		'#type' => 'token',
		'#value' => isset($form_state['storage']['values']['datos_area']) ? $form_state['storage']['values']['datos_area'] : NULL,
	);
	if (isset($form_state['storage']['values']['datos_area'])) {
		$form['busqueda']['opciones']['derecha']['wrapper_area']['lista_elementos_area'] = lista_elementos(unserialize(descifrar($form_state['storage']['values']['datos_area'])), 'area');
	}


	$form['busqueda']['opciones']['derecha']['wrapper_tema'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="edit-opciones-tema">',
		'#suffix' => '</div>',
	);
	
	$form['busqueda']['opciones']['derecha']['wrapper_tema']['tema'] = array(
		'#type' => 'textfield',
		'#title' => 'Tema',
		'#attributes' => array('autocomplete' =>'off'),
		'#size' => 25,
		'#maxlength' => 128,
		'#autocomplete_path' => 'busqueda/autocomplete_tema/'. (!empty($areas) ? $areas : 0) .'/'. $hash,
	);
	$form['busqueda']['opciones']['derecha']['wrapper_tema']['agregar_tema'] = array(
		'#type' => 'image_button',
		'#description' => 'Agregar otro tema',
		'#src' => 'sites/all/themes/segana/images/add.png',
		'#return_value' => 'agregar_tema',
		'#submit' => array('busqueda_form_submit'),
		'#ajax' => array(
			'event' => 'click',
			'callback' => 'form_busqueda_elemento_callback',
			'wrapper' => 'edit-opciones-tema',
			'method' => 'html',
			'effect' => 'none',
			'progress' => array('type' => 'none'),
		)
	);
	$form['busqueda']['opciones']['derecha']['wrapper_tema']['datos_tema'] = array(
		'#type' => 'token',
		'#value' => isset($form_state['storage']['values']['datos_tema']) ? $form_state['storage']['values']['datos_tema'] : NULL,
	);

	if (isset($form_state['storage']['values']['datos_tema'])) {
		$form['busqueda']['opciones']['derecha']['wrapper_tema']['lista_elementos_tema'] = lista_elementos(unserialize(descifrar($form_state['storage']['values']['datos_tema'])), 'tema');
	}

	$form['busqueda']['opciones']['derecha']['wrapper_matriz'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="edit-opciones-matriz">',
		'#suffix' => '</div>',
	);
	$form['busqueda']['opciones']['derecha']['wrapper_matriz']['matriz'] = array(
		'#type' => 'textfield',
		'#title' => 'Matriz',
		'#attributes' => array('autocomplete' =>'off'),
		'#size' => 23,
		'#maxlength' => 128,
		'#autocomplete_path' => 'busqueda/autocomplete_matriz/'. (!empty($areas) ? $areas : 0) .'/'. (!empty($temas) ? $temas : 0) .'/'. $hash,
	);
	$form['busqueda']['opciones']['derecha']['wrapper_matriz']['agregar_matriz'] = array(
		'#type' => 'image_button',
		'#description' => 'Agregar otro matriz',
		'#src' => 'sites/all/themes/segana/images/add.png',
		'#return_value' => 'agregar_matriz',
		'#submit' => array('busqueda_form_submit'),
		'#ajax' => array(
			'event' => 'click',
			'callback' => 'form_busqueda_elemento_callback',
			'wrapper' => 'edit-opciones-matriz',
			'method' => 'html',
			'effect' => 'none',
			'progress' => array('type' => 'none'),
		)
	);
	$form['busqueda']['opciones']['derecha']['wrapper_matriz']['datos_matriz'] = array(
		'#type' => 'token',
		'#value' => isset($form_state['storage']['values']['datos_matriz']) ? $form_state['storage']['values']['datos_matriz'] : NULL,
	);

	if (isset($form_state['storage']['values']['datos_matriz'])) {
		$form['busqueda']['opciones']['derecha']['wrapper_matriz']['lista_elementos_matriz'] = lista_elementos(unserialize(descifrar($form_state['storage']['values']['datos_matriz'])), 'matriz');
	}

		$form['busqueda']['opciones']['derecha']['wrapper_actor'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="edit-opciones-actor">',
		'#suffix' => '</div>',
	);
	$form['busqueda']['opciones']['derecha']['wrapper_actor']['actor'] = array(
		'#type' => 'textfield',
		'#title' => 'Actor',
		'#attributes' => array('autocomplete' =>'off'),
		'#size' => 24,
		'#maxlength' => 128,
		'#autocomplete_path' => 'busqueda/autocomplete_actor/'. (!empty($areas) ? $areas : 0) .'/'. (!empty($temas) ? $temas : 0) .'/'. (!empty($matrices) ? $matrices : 0) .'/'. $hash,
	);
	$form['busqueda']['opciones']['derecha']['wrapper_actor']['agregar_actor'] = array(
		'#type' => 'image_button',
		'#description' => 'Agregar otro actor',
		'#src' => 'sites/all/themes/segana/images/add.png',
		'#return_value' => 'agregar_actor',
		'#submit' => array('busqueda_form_submit'),
		'#ajax' => array(
			'event' => 'click',
			'callback' => 'form_busqueda_elemento_callback',
			'wrapper' => 'edit-opciones-actor',
			'method' => 'html',
			'effect' => 'none',
			'progress' => array('type' => 'none'),
		)
	);
	$form['busqueda']['opciones']['derecha']['wrapper_actor']['datos_actor'] = array(
		'#type' => 'token',
		'#value' => isset($form_state['storage']['values']['datos_actor']) ? $form_state['storage']['values']['datos_actor'] : NULL,
	);

	if (isset($form_state['storage']['values']['datos_actor'])) {
		$form['busqueda']['opciones']['derecha']['wrapper_actor']['lista_elementos_actor'] = lista_elementos(unserialize(descifrar($form_state['storage']['values']['datos_actor'])), 'actor');
	}


	$form['busqueda']['opciones']['botones'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="botones">',
		'#suffix' => '</div>',
	);

	$form['busqueda']['opciones']['botones']['reset'] = array(
		'#type' => 'submit',
		'#value' => t('Reset'),
		'#return_value' => t('Reset'),
		'#submit' => array('busqueda_form_submit'),
		'#ajax' => array(
			'event' => 'click',
			'callback' => 'form_busqueda_reset_callback',
			'wrapper' => '',
			'method' => 'replace',
			'effect' => 'fade',
			'progress' => array('type' => 'throbber'),
		),
	);

	$form['busqueda']['opciones']['botones']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Mostrar resultados'),
		'#return_value' => t('Mostrar resultados'),
		'#submit' => array('busqueda_form_submit'),
		'#attributes' => array('onkeypress' => 'if(event.keyCode==13){this.form.submit();}'),
		'#ajax' => array(
			'event' => 'click',
			'callback' => 'form_busqueda_submit_callback',
			'wrapper' => '',
			'method' => 'html',
			'effect' => 'fade',
			'progress' => array('type' => 'throbber'),
		),
	);

	$form['busqueda']['resultados'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="resultados">',
		'#suffix' => '</div>',
	);

	$variables = isset($form_state['storage']['values']['variables']) ? unserialize($form_state['storage']['values']['variables']) : NULL;
	$form['busqueda']['resultados']['variables'] = array(
		'#type' => 'token',
		'#value' => isset($form_state['storage']['values']['variables']) ? base64_url_encode(cifrar($form_state['storage']['values']['variables'])) : NULL,
	);
	if (isset($form_state['triggering_element']['#return_value']) && $form_state['triggering_element']['#return_value'] == 'Mostrar resultados') {
		$form['busqueda']['resultados']['graficos'] = array(
			'#markup' => graficos($variables),
			'#prefix' => '<div id="graficos">',
			'#suffix' => '</div>',
		);
		$form['busqueda']['resultados']['resumen'] = array(
		    '#type' => 'container',
		    '#attributes' => array('id' => 'resumen')
		);

	}
	return $form;
}


function busqueda_form_submit($form, &$form_state) {
	$pressed_button = check_plain($form_state['triggering_element']['#return_value']);
	switch ($pressed_button) {
		case (strpos($pressed_button, "agregar_")):
			$variables = explode('_', $pressed_button);
			$tipo = $variables[1];
			$elemento = buscar_elemento_discursivo($form_state['values'][$tipo], $tipo);
			$datos = !empty($form_state['values']['datos_'. $tipo]) ? unserialize(descifrar($form_state['values']['datos_'. $tipo])) : NULL;
			$form_state['storage']['values']['datos_'. $tipo] = cifrar(serialize((array)$datos + (array)$elemento));
			ed('prueba '. descifrar($form_state['storage']['values']['datos_'. $tipo]));

			$form_state['rebuild'] = TRUE;
			break;
		case (strpos($pressed_button, "borrar_")):
			$variables = explode('_', $pressed_button);
			$tipo = $variables[1];
			$tid = $variables[2];
			$datos = !empty($form_state['values']['datos_'. $tipo]) ? unserialize(descifrar($form_state['values']['datos_'. $tipo])) : NULL;
			unset($datos[$tid]);
			$form_state['storage']['values']['datos_'. $tipo] = cifrar(serialize($datos));
			$form_state['rebuild'] = TRUE;
			break;
		case 'Reiniciar':
			$form_state['values'] = array();
			$form_state['storage'] = array();
			$form_state['input'] = array();
			$form_state['rebuild'] = TRUE;
			break;

		case 'reiniciar_colores':
			unset($form_state['input']['paleta_colores']);

		case 'Mostrar resultados':

			//agregar areas, temas, matrices y actores
			$tipos = tipos();
			foreach ($tipos as $tipo) {
				$datos = !empty($form_state['values']['datos_'. $tipo]) ? unserialize(descifrar($form_state['values']['datos_'. $tipo])) : NULL;
				if (!empty($form_state['values'][$tipo])) {
					$elemento = buscar_elemento_discursivo($form_state['values'][$tipo], $tipo);
					$form_state['values']['datos_'. $tipo] = cifrar(serialize((array)$datos + (array)$elemento));
					$variables[$tipo] = array_keys((array)$datos + (array)$elemento);
				}
				else {
					$variables[$tipo] = !empty($datos) ? array_keys($datos) : NULL;
				}
			}
			$variables['fecha_desde'] = strtotime($form_state['values']['fecha_desde']);
			$variables['fecha_hasta'] = strtotime($form_state['values']['fecha_hasta']);
			$variables['tipo_localizacion'] = $form_state['values']['tipo_localizacion'];
			foreach ($form_state['values']['medios'] as $key => $value) {
				if ($key == $value) {
					$variables['medio'][] = $value;
				}
			}
			$variables['palabra_clave'] = $form_state['values']['palabra_clave'];
			$variables['busqueda_exacta'] = $form_state['values']['busqueda_exacta'];
			$variables['cualquiera'] = $form_state['values']['cualquiera'];
			$variables['tipo-simple'] = $form_state['values']['tipo-simple'];
			$variables['cantidad_lineal'] = $form_state['values']['cantidad_lineal'];
			$variables['tipo-combinado'] = $form_state['values']['tipo-combinado'];
			$variables['apiladas'] = $form_state['values']['apiladas'];
			$variables['cantidad_combinado'] = $form_state['values']['cantidad_combinado'];
			$variables['conectores'] = $form_state['values']['conectores'];
			$variables['leyenda'] = $form_state['values']['leyenda'];
			$variables['otros'] = $form_state['values']['otros'];


			foreach ($form_state['values']['paleta_colores'] as $color) {
				$variables['paleta_colores'][] = '#' . $color;
			}
			$form_state['values']['variables'] = serialize($variables);
			$form_state['storage']['values'] = $form_state['values'];
			$form_state['rebuild'] = TRUE;
			break;
	}
}


function form_busqueda_tipo_localizacion_callback($form, $form_state) {
	return array(
	    '#type' => 'ajax',
	    '#commands' => array(
	      ajax_command_replace("#medios-wrapper", render($form['busqueda']['medios'])),
	      ajax_command_replace("#edit-opciones-izquierda", render($form['busqueda']['opciones']['izquierda']))
    	)
  	);
}
function form_busqueda_elemento_callback($form, $form_state) {
	// buscar tipo elemento
	$pressed_button = $form_state['triggering_element']['#name'];
	$variables = explode('_', $pressed_button);
	$tipo = $variables[1];

	//limpiar el textfield area
	$form['busqueda']['opciones']['derecha']['wrapper_'. $tipo][$tipo]['#value'] = NULL;
	return array(
		'#type' => 'ajax',
		'#commands' => array(
		  ajax_command_replace("#edit-opciones-area", render($form['busqueda']['opciones']['derecha']['wrapper_area'])),
		  ajax_command_replace("#edit-opciones-tema", render($form['busqueda']['opciones']['derecha']['wrapper_tema'])),
		  ajax_command_replace("#edit-opciones-matriz", render($form['busqueda']['opciones']['derecha']['wrapper_matriz'])),
		  ajax_command_replace("#edit-opciones-actor", render($form['busqueda']['opciones']['derecha']['wrapper_actor']))
		)
  );
}



function form_busqueda_borrar_elemento_callback($form, $form_state) {
	return array(
		'#type' => 'ajax',
		'#commands' => array(
		  ajax_command_replace("#edit-opciones-area", render($form['busqueda']['opciones']['derecha']['wrapper_area'])),
		  ajax_command_replace("#edit-opciones-tema", render($form['busqueda']['opciones']['derecha']['wrapper_tema'])),
		  ajax_command_replace("#edit-opciones-matriz", render($form['busqueda']['opciones']['derecha']['wrapper_matriz'])),
		  ajax_command_replace("#edit-opciones-actor", render($form['busqueda']['opciones']['derecha']['wrapper_actor']))
		)
  	);
}


function form_busqueda_submit_callback($form, $form_state) {
	//limpiar el textfield area, tema, matriz y actor
	$tipos = tipos();
	foreach ($tipos as $tipo) {
		$form['busqueda']['opciones']['derecha']['wrapper_'. $tipo][$tipo]['#value'] = NULL;
	}
	return array(
		'#type' => 'ajax',
		'#commands' => array(
		  ajax_command_replace("#busqueda", render($form['busqueda'])),
		  ajax_command_replace("#fecha", render($form['fecha']))
		)
  );
}


function form_busqueda_reset_callback($form, $form_state) {
	return array(
		'#type' => 'ajax',
		'#commands' => array(
		  ajax_command_replace("#busqueda", render($form['busqueda'])),
		  ajax_command_replace("#fecha", render($form['fecha']))
		)
  );
}

function form_busqueda_palabra_clave_callback($form, $form_state) {
	if ($form_state['triggering_element']['#name'] == 'busqueda_exacta') {
		return $form['busqueda']['opciones']['derecha']['cualquiera'];
	}
	if ($form_state['triggering_element']['#name'] == 'cualquiera') {
		return $form['busqueda']['opciones']['derecha']['busqueda_exacta'];
	}
}

function form_busqueda_reiniciar_colores_callback($form, $form_state) {
	return $form['busqueda']['opciones']['izquierda']['opciones_graficos']['paleta_colores'];
}
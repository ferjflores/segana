<?php
function informe_tematica_form($form, &$form_state, $iid = NULL, $hash = NULL) {
	if (isset($iid) && !isset($form_state['triggering_element']['#return_value'])) {
		$notas = cargar_tematica($iid);
		$title = $notas['title'];
		$fecha_desde = $notas['fecha_desde'];
		$fecha_hasta = $notas['fecha_hasta'];

		unset($notas['title']);
		unset($notas['fecha_desde']);
		unset($notas['fecha_hasta']);

		$form_state['values']['notas'] = serialize($notas);
	}
	$form['iid'] = array(
		'#type' => 'token',
		'#value' => isset($form_state['values']['iid']) ? $form_state['values']['iid'] : ($iid ? $iid : NULL),
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
		'#default_value' => isset($form_state['values']['fecha_desde']) ? $form_state['values']['fecha_desde'] : (isset($fecha_desde) ? date('Y-m-d', $fecha_desde) : date('Y-m-d')),
		'#ajax' => array(
			'event' => 'change',
			'callback' => '',
			'wrapper' => 'temas',
			'progress' => array('type' => 'none'),
			'effect' => 'none',
		),
	);
	$form['fecha']['fecha_hasta'] = array(
		'#type' => 'date_popup',
		'#title' => t('hasta el'),
		'#attributes' => array('autocomplete' =>'off'),
		'#date_format' => 'd-m-Y',
		'#default_value' => isset($form_state['values']['fecha_hasta']) ? $form_state['values']['fecha_hasta'] : (isset($fecha_hasta) ? date('Y-m-d', $fecha_hasta) : 0),
		'#date_year_range' => "$rango_ano_inicio:+0",
		'#size' => 6,
	);

 	$form['contenedor'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="contenedor">',
		'#suffix' => '</div>',
	);

	$form['contenedor']['superior'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="superior">',
		'#suffix' => '</div>',
	);

	$fecha_desde = isset($form_state['input']['fecha_desde']) ? $form_state['input']['fecha_desde'] : strtotime(date ('Y-m-d'));
	
	$form['contenedor']['superior']['titulo'] = array(
		'#type' => 'textfield',
		'#title' => 'Titulo',
		'#attributes' => array('autocomplete' =>'off'),
		'#default_value' => !empty($form_state['values']["titulo"]) ? $form_state['values']["titulo"] : (isset($title) ? $title : NULL),
		'#size' => 59,
		'#maxlength' => 128,
	);

	$form['contenedor']['superior']['area'] = array(
		'#type' => 'textfield',
		'#title' => 'Área',
		'#attributes' => array('autocomplete' =>'off'),
		'#default_value' => !empty($form_state['values']["area"]) ? $form_state['values']["area"] : NULL,
		'#size' => 60,
		'#maxlength' => 128,
		'#autocomplete_path' => 'notas/autocomplete/area/' . $fecha_desde . '/0',
	);

	$form['contenedor']['superior']['tema'] = array(
		'#type' => 'textfield',
		'#title' => 'Tema',
		'#attributes' => array('autocomplete' =>'off'),
		'#default_value' => !empty($form_state['values']["tema"]) ? $form_state['values']["tema"] : NULL,
		'#size' => 60,
		'#maxlength' => 128,
		'#autocomplete_path' => 'notas/autocomplete/tema/' . $fecha_desde . '/0',
	);

	$form['contenedor']['superior']['palabra_clave'] = array(
		'#type' => 'textfield',
		'#title' => 'Palabra clave',
		'#attributes' => array('autocomplete' =>'off'),
		'#default_value' => !empty($form_state['values']["palabra_clave"]) ? $form_state['values']["palabra_clave"] : NULL,
		'#size' => 52,
		'#maxlength' => 128,
	);

	if (isset($form_state['triggering_element']['#name'])) {
		switch ($form_state['triggering_element']['#name']) {
			case 'busqueda_exacta':
				unset($form_state['values']['cualquiera']);
				unset($form_state['input']['cualquiera']);
				break;
			case 'cualquiera':
				unset($form_state['values']['busqueda_exacta']);
				unset($form_state['input']['busqueda_exacta']);
				break;
		}
	}

	$form['contenedor']['superior']['busqueda_exacta'] = array(
		'#type' => 'checkbox',
		'#title' => ' ',
		'#attributes' => (isset($form_state['values']['busqueda_exacta']) && $form_state['values']['busqueda_exacta'] == 1 ) ? array('checked' => 'checked') : NULL,
		'#prefix' => '<div id="busqueda_exacta">',
		'#suffix' => 'Búsqueda exacta</div>',
		'#ajax' => array(
			'callback' => 'form_informe_tematica_palabra_clave_callback',
			'wrapper' => 'cualquiera',
			'progress' => array('type' => 'throbber'),
			'effect' => 'none',
		),
	 );	

	$form['contenedor']['superior']['cualquiera'] = array(
		'#type' => 'checkbox',
		'#title' => ' ',
		'#attributes' => (isset($form_state['values']['cualquiera']) && $form_state['values']['cualquiera'] == 1 ) ? array('checked' => 'checked') : NULL,
		'#prefix' => '<div id="cualquiera">',
		'#suffix' => 'Cualquiera de las palabras</div>',
		'#ajax' => array(
			'callback' => 'form_informe_tematica_palabra_clave_callback',
			'wrapper' => 'busqueda_exacta',
			'progress' => array('type' => 'throbber'),
			'effect' => 'none',
			'method' => 'replace',
		),
	 );

	$tendencia = tendencia();
	$form['contenedor']['superior']['tendencia_nota'] = array(
		'#type' => 'select',
		'#title' => 'Tendencia nota',
		'#default_value' => isset($form_state['values']['tendencia_nota']) ? $form_state['values']['tendencia_nota'] : 0,
		'#options' => $tendencia,
	);



	if (!empty($form_state['values']['notas'])) {
		$notas = unserialize($form_state['values']['notas']);
	}
	else {
		$notas = array();
	}

	if (isset($form_state['triggering_element']['#return_value']) && ($form_state['triggering_element']['#return_value'] == 'marcar_todo') ){
		if ($form_state['values']['busqueda']['marcar_todo'] === 'marcar_todo') {

			//determinar el peso del tema y la nota
			if (!empty($notas)){
				$ultimo_tema = end($form_state['values']['temas']);
				$weight_tema = $ultimo_tema['weight'] + 1;

				if (isset($notas[$tid])) {
					$ultimo_elemento = end($form_state['values']['temas'][$tid]['notas']);
					$weight = $ultimo_elemento['weight'] + 1;
				}
				else {
					$weight = -101;
				}
			}
			else {
				$weight_tema = -21;
				$weight = -101;
			}	

			foreach ($form_state['values']['busqueda'] as $key => $value) {
				$form_state['values']['busqueda'][$key]['checkbox'] = $key;
			
				if ($key != 'marcar_todo') {
					$variables = explode('_', $key);
					$nid = $variables[0];
					$delta = $variables[1];
					$node = node_load($nid);
					$wrapper = entity_metadata_wrapper('node', $node);
					$tema = $wrapper->field_tema[$delta]->name->value();
					$tid = $wrapper->field_tema[$delta]->tid->value();
					$title = $wrapper->title->value();

					$weight_tema++;
					$weight++;


					if (isset($notas[$tid])) {
						$notas[$tid]['notas'][$nid] = array(
							'title' => $title,
							'delta' => $delta,
							'weight' => $weight,
						);
					}
					else {
						$notas[$tid] = array(
							'nombre' => $tema,
							'weight' => $weight_tema,
							'notas' => array($nid => array('title' => $title, 'delta' => $delta, 'weight' => $weight) )
						);
					}
				}
			}
		}
		else {
			foreach ($form_state['values']['busqueda'] as $key => $value) {
				unset($form_state['values']['busqueda'][$key]);
				unset($form_state['input']['busqueda'][$key]);

				if ($key != 'marcar_todo') {
					$variables = explode('_', $key);
					$nid = $variables[0];
					$delta = $variables[1];
					$node = node_load($nid);
					$wrapper = entity_metadata_wrapper('node', $node);
					$tid = $wrapper->field_tema[$delta]->tid->value();
					
					unset($notas[$tid]['notas'][$nid]);

					// si no quedan mas notas por este tema, tambien borramos el indice de este tema
					if (empty($notas[$tid]['notas'])){
						unset($notas[$tid]);
					}
				}
			}
		}
	}


	if (isset($form_state['triggering_element']['#return_value']) && (0 === strpos($form_state['triggering_element']['#return_value'], 'nota_')) ){
		$variables = explode('_', $form_state['triggering_element']['#return_value']);
		$nid = $variables[1];
		$delta = $variables[2];
		$node = node_load($nid);
		$wrapper = entity_metadata_wrapper('node', $node);
		$tema = $wrapper->field_tema[$delta]->name->value();
		$tid = $wrapper->field_tema[$delta]->tid->value();
		$title = $wrapper->title->value();

		// si el checkbox fue desmarcado esta vacio su valor
		if (empty($form_state['values']['busqueda'][$nid."_".$delta]['checkbox'])) {
			unset($notas[$tid]['notas'][$nid]);

			// si no quedan mas notas por este tema, tambien borramos el indice de este tema
			if (empty($notas[$tid]['notas'])){
				unset($notas[$tid]);
			}
		}
		// agregamos la nota con su peso y titulo al arreglo notas
		else {
			//determinar el peso del tema y la nota
			if (!empty($notas)){
				$ultimo_tema = end($form_state['values']['temas']);
				$weight_tema = $ultimo_tema['weight'] + 1;

				if (isset($notas[$tid])) {
					$ultimo_elemento = end($form_state['values']['temas'][$tid]['notas']);
					$weight = $ultimo_elemento['weight'] + 1;
				}
				else {
					$weight = -100;
				}
			}
			else {
				$weight_tema = -20;
				$weight = -100;
			}


			if (isset($notas[$tid])) {
				$notas[$tid]['notas'][$nid] = array(
					'title' => $title,
					'delta' => $delta,
					'weight' => $weight,
				);
			}
			else {
				$notas[$tid] = array(
					'nombre' => $tema,
					'weight' => $weight_tema,
					'notas' => array($nid => array('title' => $title, 'delta' => $delta, 'weight' => $weight) )
				);
			}
		}
	}

	if (count($notas) > 0) {
		if (count($notas) > 1) {
			//actualizar pesos de los temas
			foreach ($notas as $tid => $values) {
				$notas[$tid]['weight'] = isset($form_state['values']['temas'][$tid]['weight']) ? $form_state['values']['temas'][$tid]['weight'] : $notas[$tid]['weight'];

				if ($values['notas'] > 1){
					foreach ($values['notas'] as $nid => $values_notas) {
						$notas[$tid]['notas'][$nid]['weight'] = isset($form_state['values']['temas'][$tid]['notas'][$nid]['weight']) ? $form_state['values']['temas'][$tid]['notas'][$nid]['weight'] : $notas[$tid]['notas'][$nid]['weight'];
					}
					stable_uasort($notas[$tid]['notas'], 'ordenar_por_peso');
				}
			}
			stable_uasort($notas, 'ordenar_por_peso');
		}
	}
	$form_state['values']['notas'] = serialize($notas);
	$form['contenedor']['superior']['notas'] = array(
		'#type' => 'token',
		'#value' => $form_state['values']['notas'],
	);

	$form['contenedor']['superior']['temas'] = array(
		'#theme' => 'tabledrag_temas',
		'#tree' => TRUE,
		'#attributes' => array('id' => array("temas")),
	);
	foreach ($notas as $tid => $values) {
		$form['contenedor']['superior']['temas'][$tid] = array(

      // We'll use a form element of type '#markup' to display the item name.
      'name' => array(
        '#markup' => $values['nombre'],
      ),
      "borrar_$tid" => array(
				'#type' => 'image_button',
				'#title' => 'Borrar tema',
				'#description' => 'Borrar tema ' . $tid,
				'#name' => "borrar_tema_$tid",
				'#return_value' => $tid,
				'#submit' => array('informe_tematica_form_submit'),
				'#src' => 'sites/all/themes/segana/images/delete.png',
				'#attributes' => array('class' => array('borrar_tema')),
				'#ajax' => array(
					'event' => 'click',
					'callback' => 'form_informe_tematica_borrar_tema_callback',
					'wrapper' => 'contenedor_table-temas',
					'method' => 'replace',
					'effect' => 'fade',
					'progress' => array('type' => 'none'),
					'prevent' => 'submit click',
				)
			),
      'tid' => array(
        // '#type' => 'hidden',
        // '#default_value' => $item->id,
        '#type' => 'textfield',
        '#size' => 3,
        '#default_value' => $tid,
        '#disabled' => TRUE,
      ),
      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => isset($form_state['values']["temas"][$tid]['weight']) ? $form_state['values']["temas"][$tid]['weight'] : $values['weight'],
        '#delta' => 20,
        '#title_display' => 'invisible',
      ),
	  );

	  $form['contenedor']['superior']['temas'][$tid]['notas'] = array(
			'#type' => 'fieldset',
			'#theme' => 'tabledrag_notas_tematica',
			//'#tree' => TRUE,
			'#attributes' => array('id' => array($tid)),
		);
		foreach ($values['notas'] as $nid => $values) {
			$form['contenedor']['superior']['temas'][$tid]['notas'][$nid] = array(
				'title' => array(
	        '#markup' => $values['title'],
	      ),
	      "borrar_$nid" => array(
					'#type' => 'image_button',
					'#title' => 'Borrar nota',
					'#description' => 'Borrar nota ' . $nid,
					'#name' => "borrar_nota_$nid",
					'#return_value' => $tid . '_' . $nid,
					'#submit' => array('informe_tematica_form_submit'),
					'#src' => 'sites/all/themes/segana/images/delete.png',
					'#attributes' => array('class' => array('borrar_nota')),
					'#ajax' => array(
						'event' => 'click',
						'callback' => 'form_informe_tematica_borrar_nota_callback',
						'wrapper' => 'contenedor_table-temas',
						'method' => 'replace',
						'effect' => 'fade',
						'progress' => array('type' => 'none'),
						'prevent' => 'submit click',
					)
				),
	      'delta' => array(
	        // '#type' => 'hidden',
	        // '#default_value' => $item->id,
	        '#type' => 'textfield',
	        '#size' => 3,
	        '#default_value' => $values['delta'],
	        '#disabled' => TRUE,
	      ),
	      'weight' => array(
	        '#type' => 'weight',
	        '#title' => t('Weight'),
	        '#default_value' => isset($form_state['values']["temas"][$tid]['notas'][$nid]['weight']) ? $form_state['values']["temas"][$tid]['notas'][$nid]['weight'] : $values['weight'],
	        '#delta' => 100,
	        '#title_display' => 'invisible',
	      ),
	    );
		}


	}

	$form['contenedor']['botones'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="botones">',
		'#suffix' => '</div>',
	);

	$form['contenedor']['botones']['reset'] = array(
		'#type' => 'submit',
		'#value' => t('Reset'),
		'#name' => 'reset',
		'#return_value' => t('Reset'),
	);

	$form['contenedor']['botones']['descargar'] = array(
		'#type' => 'submit',
		'#value' => t('Descargar'),
		'#return_value' => 'descargar',
		'#name' => 'descargar',
		'#submit' => array('informe_tematica_form_submit'),
		'#ajax' => array(
			'event' => 'click',
			'callback' => 'form_informe_tematica_descargar_callback',
			'wrapper' => 'inferior',
			'method' => 'replace',
			'effect' => 'fade',
			'progress' => array('type' => 'none'),
			//'prevent' => 'submit click mousedown',
		),
	);
  $form['#prefix'] = '<div id="formwrapper">';
  $form['#suffix'] = '</div>';

	//$form_id = str_replace('_', '-', $form_state['build_info']['form_id']);
	$form['contenedor']['botones']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Almacenar'),
		'#return_value' => 'almacenar',
		'#name' => 'almacenar',
		'#submit' => array('informe_tematica_form_submit'),
		'#ajax' => array(
			'event' => 'click',
			'callback' => 'form_informe_tematica_callback',
			'wrapper' => 'inferior',
			'method' => 'replace',
			'effect' => 'fade',
			'progress' => array('type' => 'none'),
			//'prevent' => 'submit click mousedown',
		),
	);

	$form['contenedor']['botones']['buscar'] = array(
		'#type' => 'submit',
		'#value' => t('Buscar'),
		'#return_value' => 'buscar',
		'#name' => 'buscar',
		'#submit' => array('informe_tematica_form_submit'),
		'#ajax' => array(
			'event' => 'click submit',
			'callback' => 'form_informe_tematica_buscar_callback',
			'wrapper' => 'busqueda',
			'method' => 'replace',
			'effect' => 'fade',
			'progress' => array('type' => 'throbber'),
			//'prevent' => 'submit click mousedown',
		),
	);


	$form['contenedor']['inferior'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="inferior">',
		'#suffix' => '</div>',
	);

	$form['contenedor']['inferior']['busqueda'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="busqueda">',
		'#suffix' => '</div>',
		'#tree' => TRUE,
	);


	$form['contenedor']['superior']['notas_busqueda'] = array(
		'#type' => 'token',
		'#value' => isset($form_state['values']['notas_busqueda']) ? $form_state['values']['notas_busqueda'] : NULL,
	);
	$notas_busqueda =	isset($form_state['values']['notas_busqueda']) ? unserialize($form_state['values']['notas_busqueda']) : NULL;

	if (!empty($form_state['values']['notas_busqueda']) && count($notas_busqueda) > 0){

		// encabezado	
		$titulo_busqueda	= theme('html_tag', array('element' => array('#tag' => 'div', '#attributes' => array('class' => 'titulo_busqueda'),'#value' => 'Noticias')));
		$area	= theme('html_tag', array('element' => array('#tag' => 'div', '#attributes' => array('class' => 'area'),'#value' => 'Área')));
		$title	= theme('html_tag', array('element' => array('#tag' => 'div', '#attributes' => array('class' => 'title'), '#value' => 'Titulo')));
		$autor	= theme('html_tag', array('element' => array('#tag' => 'div', '#attributes' => array('class' => 'autor'), '#value' => 'Autor')));
		$medio	= theme('html_tag', array('element' => array('#tag' => 'div', '#attributes' => array('class' => 'medio'), '#value' => 'Medio')));
		$fecha	= theme('html_tag', array('element' => array('#tag' => 'div', '#attributes' => array('class' => 'fecha'), '#value' => 'Fecha')));
		$tema	= theme('html_tag', array('element' => array('#tag' => 'div', '#attributes' => array('class' => 'tema'), '#value' => 'Tema')));
		$matriz	= theme('html_tag', array('element' => array('#tag' => 'div', '#attributes' => array('class' => 'matriz'), '#value' => 'Matriz')));
		$actor	= theme('html_tag', array('element' => array('#tag' => 'div', '#attributes' => array('class' => 'actor'), '#value' => 'Actor')));
		$tendencia_nota	= theme('html_tag', array('element' => array('#tag' => 'div', '#attributes' => array('class' => 'tendencia_nota'), '#value' => 'Tendencia')));
		$header = $area . $title . $autor . $medio . $fecha . $tema . $matriz . $actor . $tendencia_nota;

		$form['contenedor']['inferior']['busqueda']['titulo'] = array(
			'#markup' => theme('html_tag', array(
      	'element' => array(
        	'#tag' => 'div',
        	'#attributes' => array('class' => 'encabezado'),
        	'#value' => $titulo_busqueda
        )
			))
		);
		
		$form['contenedor']['inferior']['busqueda']['marcar_todo'] = array(
			'#type' => 'checkbox',
			'#title' => ' ',
			'#return_value' => "marcar_todo",
			'#attributes' => (isset($form_state['values']['marcar_todo'])) ? array('checked' => 'checked', 'class' => array('marcar_todo')) : array('class' => array('marcar_todo')),
			'#ajax' => array(
				'event' => 'click',
				'callback' => 'form_informe_tematica_nota_marcar_todo_callback',
				'method' => 'html',
				'effect' => 'fade',
				'progress' => array('type' => 'throbber'),
				//'prevent' => 'submit click mousedown',
			),
		);

		$form['contenedor']['inferior']['busqueda']['encabezado'] = array(
			'#markup' => theme('html_tag', array(
      	'element' => array(
        	'#tag' => 'div',
        	'#attributes' => array('class' => 'encabezado'),
        	'#value' => $header,
        )
			))
		);

		foreach ($notas_busqueda as $nid_delta => $values) {
			
			$form['contenedor']['inferior']['busqueda'][$nid_delta] = array(
				'#type' => 'container',
				'#attributes' => array('class' => array('nota')),
			);
			$form['contenedor']['inferior']['busqueda'][$nid_delta]['checkbox'] = array(
				'#type' => 'checkbox',
				'#title' => ' ',
				'#return_value' => "nota_" . $nid_delta,
				'#attributes' => (isset($form_state['values']['busqueda'][$nid_delta]['checkbox'])) ? array('checked' => 'checked') : NULL,
				'#ajax' => array(
					'event' => 'click',
					'callback' => 'form_informe_tematica_nota_callback',
					'wrapper' => 'contenedor_table-temas',
					'method' => 'replace',
					'effect' => 'fade',
					'progress' => array('type' => 'throbber'),
					//'prevent' => 'submit click mousedown',
				),
			);

			$area	= theme('html_tag', array('element' => array('#tag' => 'div', '#attributes' => array('class' => 'area'),'#value' => $values['area'])));
			$title	= theme('html_tag', array('element' => array('#tag' => 'div', '#attributes' => array('class' => 'title'), '#value' => $values['title'])));
			$autor	= theme('html_tag', array('element' => array('#tag' => 'div', '#attributes' => array('class' => 'autor'), '#value' => $values['autor'])));
			$medio	= theme('html_tag', array('element' => array('#tag' => 'div', '#attributes' => array('class' => 'medio'), '#value' => $values['medio'])));
			$fecha	= theme('html_tag', array('element' => array('#tag' => 'div', '#attributes' => array('class' => 'fecha'), '#value' => $values['fecha'])));
			$tema	= theme('html_tag', array('element' => array('#tag' => 'div', '#attributes' => array('class' => 'tema'), '#value' => $values['tema'])));
			$matriz	= theme('html_tag', array('element' => array('#tag' => 'div', '#attributes' => array('class' => 'matriz'), '#value' => $values['matriz'])));
			$actor	= theme('html_tag', array('element' => array('#tag' => 'div', '#attributes' => array('class' => 'actor'), '#value' => $values['actor'])));
			$tendencia_nota	= theme('html_tag', array('element' => array('#tag' => 'div', '#attributes' => array('class' => 'tendencia_nota'), '#value' => $values['tendencia_nota'])));


			$form['contenedor']['inferior']['busqueda'][$nid_delta]['titulo'] = array(
				'#markup' => theme('html_tag', array(
	      	'element' => array(
	        	'#tag' => 'div',
	        	'#attributes' => array('class' => 'titulo_nota'),
	        	'#value' => $area . $title . $autor . $medio . $fecha . $tema . $matriz . $actor . $tendencia_nota
	        )
				))
			);
		}
	}

	return $form;
}

function informe_tematica_form_submit($form, &$form_state) {
	$pressed_button = check_plain($form_state['triggering_element']['#name']);
	switch ($pressed_button) {
		case 'Reiniciar':
			$form_state['rebuild'] = TRUE;
			unset($form_state['storage']);
			break;
		case (strpos($pressed_button, "borrar_tema")):
			$notas = unserialize($form_state['values']['notas']);
			$tid = $form_state['triggering_element']['#return_value'];
			unset($notas[$tid]);
			$form_state['values']['notas'] = serialize($notas);
			$form_state['storage']['values'] = $form_state['values'];
			$form_state['rebuild'] = TRUE;
			break;
		case (strpos($pressed_button, "borrar_nota")):
			$notas = unserialize($form_state['values']['notas']);
			$tid_nid = explode('_', $form_state['triggering_element']['#return_value']);
			$tid = $tid_nid[0];
			$nid = $tid_nid[1];
			unset($notas[$tid]['notas'][$nid]);
			if (count($notas[$tid]['notas']) == 0) {
				unset($notas[$tid]);
			}
			$form_state['values']['notas'] = serialize($notas);

			$form_state['storage']['values'] = $form_state['values'];
			$form_state['rebuild'] = TRUE;
			break;
		case 'buscar':
			$tema = ($form_state['values']['tema']) ? $form_state['values']['tema'] : NULL;
			$area = ($form_state['values']['area']) ? $form_state['values']['area'] : NULL;
			$palabra_clave =  ($form_state['values']['palabra_clave']) ? $form_state['values']['palabra_clave'] : NULL;
			$busqueda_exacta =  ($form_state['values']['busqueda_exacta']) ? $form_state['values']['busqueda_exacta'] : NULL;
			$cualquiera =  ($form_state['values']['cualquiera']) ? $form_state['values']['cualquiera'] : NULL;
			$tendencia_nota = ($form_state['values']['tendencia_nota'] != 0) ? $form_state['values']['tendencia_nota'] : NULL;
			$fecha_desde = $form_state['values']['fecha_desde'];
			$fecha_hasta = $form_state['values']['fecha_hasta'];

			$query = db_select('node', 'n');
			$query->join('field_data_field_area', 'fa', 'n.nid = fa.entity_id');
			$query->join('field_data_field_tema', 'ft', 'n.nid = ft.entity_id');
			$query->leftJoin('field_data_field_matriz', 'fm', 'n.nid = fm.entity_id AND fm.delta = ft.delta');
			$query->leftJoin('field_data_field_actor', 'fac', 'n.nid = fac.entity_id AND fac.delta = ft.delta');
			$query->leftJoin('field_data_field_tendencia_nota', 'fte', 'n.nid = fte.entity_id');
			$query->join('field_data_field_fecha', 'ff', 'ff.entity_id = n.nid AND ff.bundle = :bundle', array(':bundle' => 'nota'));
			$query->join('field_data_field_medio', 'fme', 'fme.entity_id = n.nid');
			$query->join('field_data_field_autor', 'fau', 'fau.entity_id = n.nid');
			$query->join('taxonomy_term_data', 'ttd_area', 'fa.field_area_tid = ttd_area.tid');
			$query->join('taxonomy_term_data', 'ttd_tema', 'ft.field_tema_tid = ttd_tema.tid');
			$query->leftJoin('taxonomy_term_data', 'ttd_matriz', 'fm.field_matriz_tid = ttd_matriz.tid');
			$query->leftJoin('taxonomy_term_data', 'ttd_actor', 'fac.field_actor_tid = ttd_actor.tid');
			$query->leftJoin('taxonomy_term_data', 'ttd_tendencia_nota', 'fte.field_tendencia_nota_tid = ttd_tendencia_nota.tid');
			$query->join('taxonomy_term_data', 'ttd_medio', 'fme.field_medio_tid = ttd_medio.tid');
			$query
			->fields('ttd_area', array('name'))
			->fields('n', array('nid', 'title'))
			->fields('fau', array('field_autor_value'))
			->fields('ttd_medio', array('name'))
			->fields('ff', array('field_fecha_value'))
			->fields('ttd_tema', array('name'))
			->fields('ft', array('delta'))
			->fields('ttd_matriz', array('name'))
			->fields('ttd_actor', array('name'))
			->fields('ttd_tendencia_nota', array('name'))
			->orderBy('ttd_area.name', 'ASC')
			->orderBy('ttd_tema.name', 'ASC')
			->orderBy('ff.field_fecha_value', 'ASC')
			->orderBy('n.title', 'ASC');


			if (isset($area)) {
				$query->condition('ttd_area.name', $area);
			}
			if (isset($tema)) {
				$query->condition('ttd_tema.name', $tema);
			}
			if (isset($tendencia_nota)){
				$query->condition('ttd_tendencia_nota.tid', $tendencia_nota);
			}
			if (isset($palabra_clave)){
				$query = query_palabra_clave($query, array('palabra_clave' => $palabra_clave, 'busqueda_exacta' => $busqueda_exacta, 'cualquiera' => $cualquiera));
			}

			if (isset($fecha_hasta)) {
				$query->condition('ff.field_fecha_value', strtotime($fecha_desde), '>=')
					->condition('ff.field_fecha_value', strtotime($fecha_hasta), '<=');
			}
			else {
				$query->condition('ff.field_fecha_value', strtotime($fecha_desde), '=');
			}
			$resultado = $query->execute();
			$count_query = $resultado->rowCount();
			if ($count_query > 0) {
				$notas_busqueda = array();
				foreach ($resultado as $row) {
					$notas_busqueda[$row->nid . '_' . $row->delta] = array(
						'area' => $row->name,
						'title' => $row->title,
						'autor' => $row->field_autor_value,
						'medio' => $row->ttd_medio_name,
						'fecha' => date('d/m/Y', $row->field_fecha_value),
						'tema' => $row->ttd_tema_name,
						'matriz' => $row->ttd_matriz_name,
						'actor' => $row->ttd_actor_name,
						'tendencia_nota' => $row->ttd_tendencia_nota_name,
					);
				}
			}
			$form_state['values']['notas_busqueda'] = isset($notas_busqueda) ? serialize($notas_busqueda) : NULL;

			//si se realiza la busqueda de nuevo se deben marcar de nuevo los checkbox de las notas ya agregadas
			if (isset($form_state['values']['notas'])) {
				$notas = unserialize($form_state['values']['notas']);
				if (isset($notas)) {
					foreach ($notas as $nid => $value) {
						$form_state['values']['busqueda'][$nid] = 1;
					}
				}
			}

			$form_state['storage']['values'] = $form_state['values'];
			$form_state['rebuild'] = TRUE;
			break;

		case 'descargar':
		case 'almacenar':
			$fecha = $form_state['values']['fecha_desde'];
			if (!empty($form_state['values']['fecha_hasta'])) {
				$fecha = $fecha . " hasta " . $form_state['values']['fecha_hasta'];
			}
			if(!isset($form_state['values']['iid'])) {
				$informe = entity_create('informe', array(
					'title' => !empty($form_state['values']['titulo']) ? $form_state['values']['titulo'] : 'Temática ' .$fecha,
					'type' => 'tematica'
				));
			}
			else {
				$informe = entity_load_single('informe', $form_state['values']['iid']);
				$informe->title = !empty($form_state['values']['titulo']) ? $form_state['values']['titulo'] : 'Temática ' .$fecha;
				unset($informe->field_delta);
				unset($informe->field_nota);
			}
			$wrapper = entity_metadata_wrapper('informe', $informe);
			$wrapper->field_fecha_desde->set(strtotime($form_state['values']['fecha_desde']));
			$wrapper->field_fecha_hasta->set(!empty($form_state['values']['fecha_hasta']) ? strtotime($form_state['values']['fecha_hasta']) : NULL);

			$nota = array();
			$weight = array();
			$delta = array();

			$tema = array();
			$tema_weight = array();

			foreach ($form_state['values']['temas'] as $tid => $values) {
				$tema[] = $tid;
				$tema_weight[] = $values['weight'];
				foreach ($values['notas'] as $nid => $values_notas) {
					$nota[] = $nid;
					$weight[] = $form_state['values']['temas'][$tid]['notas'][$nid]['weight'];
					$delta[] = $form_state['values']['temas'][$tid]['notas'][$nid]['delta'];
				}
			}
			$wrapper->field_nota->set($nota);
			$wrapper->field_weight->set($weight);
			$wrapper->field_delta->set($delta);

			$wrapper->field_tema->set($tema);
			$wrapper->field_weight_tema->set($tema_weight);

			$wrapper->save();
			$form_state['values']['iid'] = $informe->iid;
			unset($form_state['values']['notas_busqueda']);
			$form_state['storage']['values'] = $form_state['values'];
			
			/*if ($pressed_button == 'descargar'){
				$form_state['redirect'] = 'tematica/export/' . $form_state['values']['iid'];
			}
			else {*/
				$form_state['rebuild'] = TRUE;
			//}
			break;
	}
}


function form_informe_tematica_buscar_callback($form, $form_state) {
	return array(
		'#type' => 'ajax',
		'#commands' => array(
			ajax_command_replace("#busqueda", render($form['contenedor']['inferior']['busqueda'])),
		),
  );
}

function form_informe_tematica_palabra_clave_callback($form, $form_state) {
	if ($form_state['triggering_element']['#name'] == 'busqueda_exacta') {
		return $form['contenedor']['superior']['cualquiera'];
	}
	if ($form_state['triggering_element']['#name'] == 'cualquiera') {
		return $form['contenedor']['superior']['busqueda_exacta'];
	}
}

function form_informe_tematica_nota_marcar_todo_callback($form, &$form_state) {
	return array(
		'#type' => 'ajax',
		'#commands' => array(
			ajax_command_replace("#contenedor_table-temas", render($form['contenedor']['superior']['temas'])),
			ajax_command_replace("#busqueda", render($form['contenedor']['inferior']['busqueda'])),
		),
  );
}
function form_informe_tematica_nota_callback($form, &$form_state) {
	return $form['contenedor']['superior']['temas'];
}

function form_informe_tematica_borrar_tema_callback($form, &$form_state) {
	return $form['contenedor']['superior']['temas'];
}

function form_informe_tematica_borrar_nota_callback($form, &$form_state) {
	return $form['contenedor']['superior']['temas'];
}


function form_informe_tematica_descargar_callback($form, &$form_state){
	ctools_include('ajax');
	ctools_add_js('ajax-responder');
	return array(
		'#type' => 'ajax',
		'#commands' => array(
			 ctools_ajax_command_redirect('tematica/export/' . $form_state['values']['iid']),
		),
  );
}

function theme_tabledrag_temas($variables) {
  $rows = array();

  foreach (element_children($variables['element']) as $tid) {

    $variables['element'][$tid]['weight']['#attributes']['class'] = array('item-tema-weight tabledrag-hide');
    $variables['element'][$tid]['tid']['#attributes']['class'] = array('item-tid tabledrag-hide' );
    $rows[] = array(
      'data' => array(
        array('data' => drupal_render($variables['element'][$tid]['name']) . drupal_render($variables['element'][$tid]["borrar_$tid"]), 'class' => 'tema'),
        drupal_render($variables['element'][$tid]['tid']),
        drupal_render($variables['element'][$tid]['weight']),
        drupal_render($variables['element'][$tid]['notas']),
      ),
      'class' => array('draggable'),
    );
  }

  $table_id = 'table-temas';

  $tabledrag = theme('table', array(
    'rows' => $rows,
    'attributes' => array('id' => $table_id),
  ));
  $output = theme('html_tag', array(
    'element' => array(
      '#tag' => 'div',
      '#attributes' => array(
        'id' => 'contenedor_' . $table_id,
        ),
      '#value' => $tabledrag,
    ),
 	));

  //$output .= drupal_render_children($variables['element']);
  drupal_add_tabledrag($table_id, 'order', 'sibling', 'item-tema-weight');

  return $output;
}

function theme_tabledrag_notas_tematica($variables) {
  $rows = array();

  foreach (element_children($variables['element']) as $nid) {

    $variables['element'][$nid]['weight']['#attributes']['class'] = array('item-nota-weight tabledrag-hide');
    $variables['element'][$nid]['delta']['#attributes']['class'] = array('item-delta tabledrag-hide' );

    $rows[] = array(
      'data' => array(
        array('data' => drupal_render($variables['element'][$nid]['title']) . drupal_render($variables['element'][$nid]["borrar_$nid"]), 'class' => 'title'),
				drupal_render($variables['element'][$nid]['delta']),
        drupal_render($variables['element'][$nid]['weight']),
      ),
      'class' => array('draggable'),
    );
  }
  $table_id = 'tema_'.$variables['element']['#attributes']['id'][0];

  $output = theme('table', array(
    'rows' => $rows,
    'attributes' => array('id' => $table_id),
  ));

  //$output .= drupal_render_children($variables['element']);
  drupal_add_tabledrag($table_id, 'order', 'sibling', 'item-nota-weight');

  return $output;
}

function cargar_tematica($iid, $completo = 0){
	$informe = informe_load($iid);
	$wrapper = entity_metadata_wrapper('informe', $informe);
	$fecha_desde = $wrapper->field_fecha_desde->value();
	$fecha_hasta = $wrapper->field_fecha_hasta->value();
	$title = $wrapper->label();
	$tema = $wrapper->field_tema->value();
	$tema_weight = $wrapper->field_weight_tema->value();
	$nota = $wrapper->field_nota->value();
	$delta_nota_tema = $wrapper->field_delta->value();
	$weight = $wrapper->field_weight->value();

	$notas = array();
	foreach ($tema as $delta_tema => $value) {
		$notas[$value->tid] = array(
			'nombre' => $value->name,
			'weight' => $tema_weight[$delta_tema],
			'notas' => array(),
		);
	}
	
	foreach ($nota as $delta_nota => $nid) {
		$node = node_load($nid);
		$wrapper_node = entity_metadata_wrapper('node', $node);
		$tema_nota = $wrapper_node->field_tema[$delta_nota_tema[$delta_nota]]->tid->value();
		$notas[$tema_nota]['notas'][$nid] = array(
			'title' => $wrapper_node->label(),
			'delta' => $delta_nota_tema[$delta_nota],
			'weight' => $weight[$delta_nota],
		);
		if($completo) {
          if (!empty($node->body)) {
            $notas[$tema_nota]['notas'][$nid]['body'] = preg_replace('/[\x00-\x09\x0B\x0C\x0E-\x1F\x7F]/', '', htmlspecialchars($wrapper_node->body->value->value(), ENT_QUOTES));
          }
          else {
            $notas[$tema_nota]['notas'][$nid]['body'] = "";
          }
          $notas[$tema_nota]['notas'][$nid]['autor'] = $wrapper_node->field_autor->value();
		  $notas[$tema_nota]['notas'][$nid]['medio'] = $wrapper_node->field_medio->name->value();
		  $notas[$tema_nota]['notas'][$nid]['pagina_web'] = $wrapper_node->field_medio->field_pagina_web->value();
		  $notas[$tema_nota]['notas'][$nid]['area'] = $wrapper_node->field_area->name->value();
		}
	}
	$notas['title'] = $title;
	$notas['fecha_desde'] = $fecha_desde;
	$notas['fecha_hasta'] = $fecha_hasta;

	return $notas;
}

?>
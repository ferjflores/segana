<?php
function informe_agenda_informativa_form($form, &$form_state, $iid = NULL, $hash = NULL) {
	if (isset($iid) && !isset($form_state['triggering_element']['#return_value'])) {
		$informe = informe_load($iid);

		$language = LANGUAGE_NONE;
		$fecha_desde = $informe->field_fecha_desde[$language][0]['value'];
		$fecha_hasta = isset($informe->field_fecha_hasta[$language][0]['value']) ? $informe->field_fecha_hasta[$language][0]['value'] : NULL;
		$tipo = isset($informe->field_ambito[$language]['value']) ? $informe->field_ambito[$language]['value'] : NULL;

		$tema = array();
		foreach ($informe->field_tema[$language] as $delta => $value) {
			$term = taxonomy_term_load($value['tid']);
			$tema[$value['tid']] = $term->name;
		}
		$tema_tid = array_keys($tema);
		$tema_array = array_values($tema);
		$cantidad_temas = count($tema);

		$notas = array();
		foreach ($informe->field_nota[$language] as $delta => $value) {

			$tid = $informe->field_tema[$language][$delta]['tid'];
			$tema_seleccionado = array_search($tid, $tema_tid);
			$nid = $value['value'];
			$node = node_load($nid);
			$title = $node->title;
			$pid = $informe->field_nota_padre[$language][$delta]['value'];
			$weight = $informe->field_weight[$language][$delta]['value'];

			$notas[$tema_seleccionado][$nid]['title'] = $title;
			$form_state['values']["notas_$tema_seleccionado"][$nid]['pid'] = $pid;
			$form_state['values']["notas_$tema_seleccionado"][$nid]['weight'] = $weight;
		}
		$form_state['values']['notas'] = serialize($notas);
	}
	else {
		$cantidad_temas = isset($form_state['storage']['values']['cantidad_temas']) ? $form_state['storage']['values']['cantidad_temas'] : 1;
		$tema_seleccionado = isset($form_state['storage']['values']['tema_seleccionado']) ? $form_state['storage']['values']['tema_seleccionado'] : 0;
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

	$tipos = array(0 => 'Ambos', 1 => 'Nacional', 2 => 'Internacional');

	$form['contenedor']['superior']['tipo'] = array(
		'#type' => 'radios',
		'#title' => 'TIPO DE SÍNTESIS',
		'#default_value' => isset($form_state['values']['tipo']) ? $form_state['values']['tipo'] : (isset($tipo) ? $tipo : 0),
		'#options' => $tipos,
	);

	$form['contenedor']['superior']['orden_temas'] = array(
		'#type' => 'fieldset',
		'#title' => "Cambiar orden de los temas",
		'#collapsible' => TRUE, 
		'#collapsed' => TRUE,
		'#attributes' => array('class' => array('orden_temas')),
	);

	$form['contenedor']['superior']['orden_temas']["temas"] = array(
		'#theme' => 'tabledrag_orden_temas',
		'#tree' => TRUE,
		'#attributes' => array('id' => array("orden_temas")),
	);
	if (!empty($notas) && count($notas) > 0) {

		foreach ($notas as $id => $values) {
			$form['contenedor']['superior']['orden_temas']["temas"][$id] = array(

		      // We'll use a form element of type '#markup' to display the item name.
		      'name' => array(
		        '#markup' => !empty($form_state['values']["tema_$id"]) ? $form_state['values']["tema_$id"] : (isset($tema_array[$id]) ? $tema_array[$id] : NULL),
		      ),
	      	'id' => array(
		        '#type' => 'textfield',
		        '#size' => 3,
		        '#default_value' => $id,
		        '#disabled' => TRUE,
		      ),
		      'depth' => array(
		        '#type' => 'hidden',
		        '#value' => $i,
		      ),
		  );
		}
	}

	$form['contenedor']['superior']['cantidad_temas'] = array(
		'#type' => 'token',
		'#value' => $cantidad_temas,
	);

	$form['contenedor']['superior']['temas'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="temas">',
		'#suffix' => '</div>',
	);

	if (!empty($form_state['values']['notas'])) {
		$notas = unserialize($form_state['values']['notas']);
	}
	else {
		$notas = array();
	}

if (isset($form_state['triggering_element']['#return_value']) && ($form_state['triggering_element']['#return_value'] === 'marcar_todo') ){
		if ($form_state['values']['busqueda']['marcar_todo'] === 'marcar_todo') {

			if (!empty($notas)){
				$ultimo_elemento = end($form_state['values']["notas_$tema_seleccionado"]);
				$weight = $ultimo_elemento['weight'] + 1;
			}
			else {
				$weight = -101;
			}
			foreach ($form_state['values']['busqueda'] as $key => $value) {			
				if ($key != 'marcar_todo') {
					$value = array_keys($value);
					$variables = explode('_', $value[0]);
					$nid_nota_seleccionada = $variables[0];
					$tema_seleccionado = $variables[1];
					$title_nota_seleccionada = $variables[2];
					$weight++;

					//marcar el checkbox
					$form_state['values']['busqueda'][$nid_nota_seleccionada][$value[0]] = $value[0];
					$notas[$tema_seleccionado][$nid_nota_seleccionada] = array(
						'title' => $title_nota_seleccionada,
						'weight' => $weight,
					);
				}
			}
		}
		else {
			foreach ($form_state['values']['busqueda'] as $key => $value) {

				if ($key != 'marcar_todo') {
					$value = array_keys($value);
					$variables = explode('_', $value[0]);
					$nid_nota_seleccionada = $variables[0];
					$tema_seleccionado = $variables[1];
					unset($notas[$tema_seleccionado][$nid_nota_seleccionada]);
					unset($form_state['values']['busqueda'][$nid_nota_seleccionada]);	
					unset($form_state['input']['busqueda'][$nid_nota_seleccionada]);
					// si no quedan mas notas por este tema, tambien borramos el indice de este tema
					if (empty($notas[$tema_seleccionado])){
						unset($notas[$tema_seleccionado]);
					}
				}
			}
			$form_state['values']['notas'] = serialize($notas);
		}
	}

	if (isset($form_state['triggering_element']['#return_value']) && (0 === strpos($form_state['triggering_element']['#return_value'], 'nota_')) ){
		$variables = explode('_', $form_state['triggering_element']['#return_value']);
		$nid_nota_seleccionada = $variables[1];
		$tema_seleccionado = $variables[2];
		$title_nota_seleccionada = $variables[3];

		if (!empty($notas)){
			$ultimo_elemento = end($form_state['values']["notas_$tema_seleccionado"]);
			$weight = $ultimo_elemento['weight'] + 1;
		}
		else {
			
			$weight = -20;
		}
		// si el checkbox fue desmarcado esta vacio su valor
		if (empty($form_state['values']['busqueda'][$nid_nota_seleccionada][$nid_nota_seleccionada ."_". $tema_seleccionado ."_". $title_nota_seleccionada])) {
			unset($notas[$tema_seleccionado][$nid_nota_seleccionada]);

			// si no quedan mas notas por este tema, tambien borramos el indice de este tema
			if (empty($notas[$tema_seleccionado])){
				unset($notas[$tema_seleccionado]);
			}
		}
		// agregamos la nota con su peso y titulo al arreglo notas
		else {
			$notas[$tema_seleccionado][$nid_nota_seleccionada] = array(
				'title' => $title_nota_seleccionada,
				'weight' => $weight,
			);
		}
	}
	if (count($notas) > 0) {
		//agregar parent al arreglos $notas
		foreach ($notas as $tema => $notas_por_tema) {
			foreach ($notas_por_tema as $nid => $values) {
				$notas[$tema][$nid]['pid'] = $form_state['values']["notas_$tema"][$nid]['pid'];
				if (!empty($form_state['values']["notas_$tema"][$nid]['weight'])) {
					$notas[$tema][$nid]['weight'] = $form_state['values']["notas_$tema"][$nid]['weight'];
				}
			}
		}

		// crear el arbol de la notas por tema
		foreach ($notas as $tema => $value) {
			if (count($notas[$tema]) > 1){
				$notas[$tema] = crear_arbol(0, $notas[$tema]);
			}
		}

		$form_state['values']['notas'] = serialize($notas);
	}

	$form['contenedor']['superior']['notas'] = array(
		'#type' => 'token',
		'#value' => $form_state['values']['notas'],
	);


	for ($i = 0; $i < $cantidad_temas; $i++) {
		$form['contenedor']['superior']['temas'][$i] = array(
			'#type' => 'fieldset',
			'#prefix' => '<div id="temas_'. $i .'">',
			'#suffix' => '</div>',

		);
		$fecha_desde = isset($form_state['input']['fecha_desde']) ? $form_state['input']['fecha_desde'] : strtotime(date ('Y-m-d'));
		$form['contenedor']['superior']['temas'][$i]["tema_$i"] = array(
			'#type' => 'textfield',
			'#title' => 'Tema ' . ($i+1),
			'#attributes' => array('autocomplete' =>'off'),
			'#default_value' => !empty($form_state['values']["tema_$i"]) ? $form_state['values']["tema_$i"] : (isset($tema_array[$i]) ? $tema_array[$i] : NULL),
			'#attributes' => array('class' => array('tema',($tema_seleccionado == $i ? 'tema_seleccionado' : NULL))),
			'#size' => 70,
			'#maxlength' => 128,
			'#autocomplete_path' => 'notas/autocomplete/tema/' . $fecha_desde . '/0',
		);
		if (!empty($notas[$i]) && count($notas[$i]) > 0) {
			$form['contenedor']['superior']['temas'][$i]["tema_$i"]['#attributes']['readonly'] = 'readonly';
		}

		$form['contenedor']['superior']['temas'][$i]["borrar_$i"] = array(
			'#type' => 'image_button',
			'#description' => 'Borrar tema ' .$i,
			'#name' => "borrar_tema_$i",
			'#src' => 'sites/all/themes/segana/images/delete.png',
			'#return_value' => $i,
			'#submit' => array('informe_agenda_informativa_form_submit'),
			'#ajax' => array(
				'event' => 'click',
				'callback' => 'form_informe_agenda_informativa_borrar_tema_callback',
				'wrapper' => 'temas',
				'method' => 'replace',
				'effect' => 'fade',
				'progress' => array('type' => 'none'),
				'prevent' => 'submit click',
			),
			//'#limit_validation_errors' => array(array('filas_discursivo'), array('elementos_discursivos')),
		);

		$form['contenedor']['superior']['temas'][$i]["notas_$i"] = array(
			'#theme' => 'tabledrag_notas',
			'#tree' => TRUE,
			'#attributes' => array('id' => array("notas_$i")),
		);

		if (!empty($notas[$i]) && count($notas[$i]) > 0) {

			foreach ($notas[$i] as $nid => $values) {
				$form['contenedor']['superior']['temas'][$i]["notas_$i"][$nid] = array(

			      // We'll use a form element of type '#markup' to display the item name.
			      'name' => array(
			        '#markup' => $values['title'],
			      ),
			      'nid' => array(
			        // '#type' => 'hidden',
			        // '#default_value' => $item->id,
			        '#type' => 'textfield',
			        '#size' => 3,
			        '#default_value' => $nid,
			        '#disabled' => TRUE,
			      ),
			      'pid' => array(
			        // '#type' => 'hidden',
			        // '#default_value' => $item->pid,
			        '#type' => 'textfield',
			        '#size' => 3,
			        '#default_value' => isset($form_state['values']["notas_$i"][$nid]['pid']) ? $form_state['values']["notas_$i"][$nid]['pid'] : 0,
			      ),
			      'weight' => array(
			        '#type' => 'weight',
			        '#title' => t('Weight'),
			        '#default_value' => isset($form_state['values']["notas_$i"][$nid]['pid']) ? $form_state['values']["notas_$i"][$nid]['pid'] : $values['weight'],
			        '#delta' => 20,
			        '#title_display' => 'invisible',
			      ),
			      'depth' => array(
			        '#type' => 'hidden',
			        '#value' => isset($values['depth']) ? $values['depth'] : 0,
			      ),
			  );
			}
		}
	}

$form['contenedor']['superior']['tema_seleccionado'] = array(
	'#type' => 'token',
	'#default_value' => $tema_seleccionado,
	'#prefix' => '<div id="tema_seleccionado">',
	'#suffix' => '</div>',
);

$form['contenedor']['superior']['agregar_tema'] = array(
		'#type' => 'image_button',
		'#description' => 'Agregar nuevo tema',
		'#name' => 'agregar_tema',
		'#src' => 'sites/all/themes/segana/images/add.png',
		'#return_value' => 'agregar_tema',
		'#submit' => array('informe_agenda_informativa_form_submit'),
		'#prefix' => '<div id="agregar_tema">Agregar nuevo tema',
		'#suffix' => '</div>',
		'#name' => 'agregar_tema',
		'#ajax' => array(
			'event' => 'click',
			'callback' => 'form_informe_agenda_informativa_agregar_tema_callback',
			'wrapper' => 'temas',
			'method' => 'replace',
			'effect' => 'fade',
			'progress' => array('type' => 'none'),
			'prevent' => 'submit click',
		),
		//'#limit_validation_errors' => array(array('filas_discursivo'), array('elementos_discursivos')),
	);



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

	$form['contenedor']['botones']['ver_informe'] = array(
		'#type' => 'submit',
		'#value' => t('Ver Informe'),
		'#return_value' => 'ver_informe',
		'#name' => 'ver_informe',
		'#submit' => array('informe_agenda_informativa_form_submit'),
		'#ajax' => array(
			'event' => 'click',
			'callback' => 'form_informe_agenda_informativa_ver_callback',
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
		'#submit' => array('informe_agenda_informativa_form_submit'),
		'#ajax' => array(
			'event' => 'click',
			'callback' => 'form_informe_agenda_informativa_callback',
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
		'#submit' => array('informe_agenda_informativa_form_submit'),
		'#ajax' => array(
			'event' => 'click submit',
			'callback' => 'form_informe_agenda_informativa_buscar_callback',
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

	

	$form['contenedor']['inferior']['notas_busqueda'] = array(
		'#type' => 'token',
		'#value' => isset($form_state['storage']['values']['notas_busqueda']) ? $form_state['storage']['values']['notas_busqueda'] : NULL,
	);
	$notas_busqueda =	isset($form_state['values']['notas_busqueda']) ? unserialize($form_state['values']['notas_busqueda']) : NULL;
	if (!empty($form_state['values']['notas_busqueda']) && count($notas_busqueda) > 0){

		$form['contenedor']['inferior']['busqueda']['titulo'] = array(
			'#markup' => theme('html_tag', array(
      	'element' => array(
        	'#tag' => 'div',
        	'#attributes' => array('class' => 'titulo_busqueda'),
        	'#value' => 'Noticias'
        )
			))
		);

		$form['contenedor']['inferior']['busqueda']['marcar_todo'] = array(
			'#type' => 'checkbox',
			'#title' => ' ',
			'#return_value' => "marcar_todo",
			'#attributes' => (isset($form_state['values']['marcar_todo'])) ? array('checked' => 'checked', 'class' => array('marcar_todo')) : array('class' => array('marcar_todo')),
			'#suffix' => '<label class="marcar_todo">Marcar todo</label>',
			'#ajax' => array(
				'event' => 'click',
				'callback' => 'form_informe_agenda_informativa_marcar_todo_callback',
				'method' => 'html',
				'effect' => 'fade',
				'progress' => array('type' => 'throbber'),
				//'prevent' => 'submit click mousedown',
			),
		);
		foreach ($notas_busqueda as $nid => $values) {
			
			$form['contenedor']['inferior']['busqueda'][$nid] = array(
				'#type' => 'container',
				'#attributes' => array('class' => array('nota')),
			);
			$form['contenedor']['inferior']['busqueda'][$nid][$nid . "_" . $tema_seleccionado . "_" . $values['title']] = array(
				'#type' => 'checkbox',
				'#title' => ' ',
				'#return_value' => "nota_" . $nid . "_" . $tema_seleccionado . "_" . $values['title'],
				'#attributes' => ($form_state['values']['busqueda'][$nid][$nid . "_" . $tema_seleccionado . "_" . $values['title']] === $nid . "_" . $tema_seleccionado . "_" . $values['title']) ? array('checked' => 'checked') : NULL,
				'#ajax' => array(
					'event' => 'click',
					'callback' => 'form_informe_agenda_informativa_nota_callback',
					'method' => 'html',
					'effect' => 'fade',
					'progress' => array('type' => 'throbber'),
					//'prevent' => 'submit click mousedown',
				),
			);
			$form['contenedor']['inferior']['busqueda'][$nid]['titulo'] = array(
				'#markup' => theme('html_tag', array(
	      	'element' => array(
	        	'#tag' => 'div',
	        	'#attributes' => array('class' => 'titulo_nota'),
	        	'#value' => '<span>' . $values['title'] .'</span><span>'. $values['medio'] .'</span><span>' . $values['fecha'] . '</span>'
	        )
				))
			);
		}
	}
	if (isset($form_state['triggering_element']['#return_value']) && ($form_state['triggering_element']['#return_value'] == 'almacenar' || $form_state['triggering_element']['#return_value'] == 'ver_informe') ) {
		$form['contenedor']['inferior']['mensaje'] = array (
			'#markup' => "Informe salvado con id " . $form_state['values']['iid'],
		);
	}




	return $form;
}

function informe_agenda_informativa_form_submit($form, &$form_state) {
	$pressed_button = check_plain($form_state['triggering_element']['#name']);
	switch ($pressed_button) {
		case 'Reiniciar':
			$form_state['rebuild'] = TRUE;
			unset($form_state['storage']);
			break;
		case 'agregar_tema':
			$form_state['storage']['values'] = $form_state['values'];
			$form_state['storage']['values']['cantidad_temas']++;
			$form_state['rebuild'] = TRUE;
			break;
		case (strpos($pressed_button, "borrar_tema")):
			$notas = unserialize($form_state['values']['notas']);
			$tema = $form_state['triggering_element']['#return_value'];
			unset($notas[$tema]);
			$form_state['values']['notas'] = serialize(array_values($notas));

			//desplazar los valores, si el tema eliminado es el ultimo no hacer nada
			for ($i=($tema+1); $i < $form_state['values']['cantidad_temas']; $i++) { 
				$form_state['values']["notas_" . ($i-1)] = $form_state['values']["notas_$i"];
				$form_state['input']["tema_" . ($i-1)] = $form_state['values']["tema_$i"];
			}
			$form_state['values']['tema_seleccionado']--;

			$form_state['values']['cantidad_temas']--;
			$form_state['storage']['values'] = $form_state['values'];
			$form_state['rebuild'] = TRUE;
		case 'buscar':
			$tema_seleccionado = $form_state['values']['tema_seleccionado'];
			$query = db_select('node', 'n');
			$query->join('field_data_field_tema', 'ft', 'n.nid = ft.entity_id');
			$query->join('taxonomy_term_data', 'ttd', 'ft.field_tema_tid = ttd.tid');
			$query->join('field_data_field_fecha', 'ff', 'ff.entity_id = n.nid');
			$query->join('field_data_field_medio', 'fme', 'fme.entity_id = n.nid');
			$query->join('taxonomy_term_data', 'ttd_medio', 'fme.field_medio_tid = ttd_medio.tid');
			$query
			->condition('ttd.name', $form_state['values']["tema_$tema_seleccionado"])
			->fields('n', array('nid', 'title'))
			->fields('ff', array('field_fecha_value'))
			->fields('ttd_medio', array('name'));

			if (isset($form_state['values']['fecha_hasta'])) {
				$query->condition('ff.field_fecha_value', strtotime($form_state['values']['fecha_desde']), '>=')
					->condition('ff.field_fecha_value', strtotime($form_state['values']['fecha_hasta']), '<=');
			}
			else {
				$query->condition('ff.field_fecha_value', strtotime($form_state['values']['fecha_desde']), '=');
			}
			$resultado = $query->execute();
			$count_query = $resultado->rowCount();
			if ($count_query > 0) {
				foreach ($resultado as $row) {
					$notas_busqueda[$row->nid] = array(
						'title' => $row->title,
						'fecha' => date('d/j/Y', $row->field_fecha_value),
						'medio' => $row->name,
					);
				}
			}
			$form_state['values']['notas_busqueda'] = serialize($notas_busqueda);

			//si se realiza la buscado por un tema previamente agregado y con notas, se deben marcar de nuevo los checkbox de las notas ya agregadas
			if (isset($form_state['values']['notas'])) {
				$notas = unserialize($form_state['values']['notas']);
				if (isset($notas[$tema_seleccionado])) {
					foreach ($notas[$tema_seleccionado] as $nid => $value) {
						$form_state['values']['busqueda'][$nid] = 1;
					}
				}
			}

			$form_state['storage']['values'] = $form_state['values'];
			$form_state['rebuild'] = TRUE;
			break;

		case 'ver_informe':
		case 'almacenar':
			$fecha = $form_state['values']['fecha_desde'];
			if (!empty($form_state['values']['fecha_hasta'])) {
				$fecha = $fecha . " hasta " . $form_state['values']['fecha_hasta'];
			}
			switch ($form_state['values']['tipo']) {
				case 1:
					$title = "Nacional " . $fecha;
					break;
				case 2:
					$title = "Internacional " . $fecha;
					break;
				default:
					$title = $fecha;
					break;
			}
			if(!isset($form_state['values']['iid'])) {
				$informe = entity_create('informe', array(
					'title' => $title,
					'type' => 'agenda_informativa'
				));
			}
			else {
				$informe = entity_load_single('informe', $form_state['values']['iid']);
				unset($informe->field_tema);
				unset($informe->field_nota);
				unset($informe->field_nota_padre);
				unset($informe->field_weight);
				unset($informe->field_depth);
			}
			$language = LANGUAGE_NONE;

			$informe->type = 'agenda_informativa';
			$informe->field_fecha_desde[$language][0]['value'] = strtotime($form_state['values']['fecha_desde']);
			$informe->field_fecha_hasta[$language][0]['value'] = !empty($form_state['values']['fecha_hasta']) ? strtotime($form_state['values']['fecha_hasta']) : NULL;
			$informe->field_ambito[$language][0]['value'] = $form_state['values']['tipo'];

			$delta = 0;
			for ($i=0; $i < $form_state['values']['cantidad_temas']; $i++) {
				$tema = $form_state['values']["tema_$i"];
				//actualizar el arbol
				$form_state['values']["notas_$i"] = crear_arbol(0, $form_state['values']["notas_$i"]);
				foreach ($form_state['values']["notas_$i"] as $nid => $values) {
					$informe->field_tema[$language][$delta]['tid'] = key(taxonomy_get_term_by_name($tema, 'tema'));
					$informe->field_nota[$language][$delta]['value'] = $nid;
					$informe->field_nota_padre[$language][$delta]['value'] = $values['pid'];
					$informe->field_weight[$language][$delta]['value'] = $values['weight'];
					$informe->field_depth[$language][$delta]['value'] = $values['depth'];
					$delta++;
				}
			}

			$informe->save();
			$form_state['values']['iid'] = $informe->iid;
			unset($form_state['values']['notas_busqueda']);
			$form_state['storage']['values'] = $form_state['values'];
			$form_state['rebuild'] = TRUE;
			break;
	}
}


function informe_agenda_informativa_form_validate($form, &$form_state) {
	//return $form['contenedor']['superior']['temas'];
}

function form_informe_agenda_informativa_agregar_tema_callback($form, $form_state) {
	return array(
	    '#type' => 'ajax',
	    '#commands' => array(
	      ajax_command_replace("#temas", render($form['contenedor']['superior']['temas'])),
    	)
  );
}

function form_informe_agenda_informativa_borrar_tema_callback($form, $form_state) {
	return array(
	    '#type' => 'ajax',
	    '#commands' => array(
	      ajax_command_replace("#temas", render($form['contenedor']['superior']['temas'])),
    	)
  );
}

function form_informe_agenda_informativa_buscar_callback($form, $form_state) {
	return array(
		'#type' => 'ajax',
		'#commands' => array(
			ajax_command_replace("#busqueda", render($form['contenedor']['inferior']['busqueda'])),
		),
  );
}

function form_informe_agenda_informativa_marcar_todo_callback($form, &$form_state) {
	$tema_seleccionado = $form_state['values']['tema_seleccionado'];

	return array(
	    '#type' => 'ajax',
	    '#commands' => array(
	     	ajax_command_replace("#busqueda", render($form['contenedor']['inferior'])),
	     	ajax_command_replace("#contenedor_notas_" . $tema_seleccionado, render($form['contenedor']['superior']['temas'][$tema_seleccionado]["notas_$tema_seleccionado"])),
    	)
  );
}

function form_informe_agenda_informativa_nota_callback($form, &$form_state) {
	$tema_seleccionado = $form_state['values']['tema_seleccionado'];

	return array(
	    '#type' => 'ajax',
	    '#commands' => array(
	     	ajax_command_replace("#busqueda", render($form['contenedor']['inferior'])),
	     	ajax_command_replace("#contenedor_notas_" . $tema_seleccionado, render($form['contenedor']['superior']['temas'][$tema_seleccionado]["notas_$tema_seleccionado"])),
    	)
  );
}

function form_informe_agenda_informativa_callback($form, &$form_state) {
	return $form['contenedor']['inferior'];
}

function form_informe_agenda_informativa_ver_callback($form, $form_state) {
	return array(
	    '#type' => 'ajax',
	    '#commands' => array(
	      ajax_command_replace("#inferior", render($form['contenedor']['inferior'])),
	      array (
					// The command in my JS file
					'command' => 'afterAjaxCallbackAgendaInformativa',
					// Pass the needed data to be processed and displayed for the page
					'selectedValue' => $form_state['values']['iid'],
				),
			)
  );
}

function theme_tabledrag_orden_temas($variables) {
  $rows = array();

  foreach (element_children($variables['element']) as $id) {

    $variables['element'][$id]['weight']['#attributes']['class'] = array('item-tema-weight tabledrag-hide');
    $variables['element'][$id]['id']['#attributes']['class'] = array('item-tid tabledrag-hide' );
    $rows[] = array(
      'data' => array(
        array('data' => drupal_render($variables['element'][$id]['name']), 'class' => 'tema'),
        drupal_render($variables['element'][$id]['id']),
        drupal_render($variables['element'][$id]['weight']),
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

function theme_tabledrag_notas($variables) {
	$rows = array();

	$subnoticia = 0;
	$publicado_tambien = 0;
	foreach (element_children($variables['element']) as $nid) {
		$variables['element'][$nid]['weight']['#attributes']['class'] = array('item-weight tabledrag-hide');
		$variables['element'][$nid]['nid']['#attributes']['class'] = array('item-nid tabledrag-hide' );
    $variables['element'][$nid]['pid']['#attributes']['class'] = array('item-pid tabledrag-hide');
    $class = array('draggable');
    switch ($variables['element'][$nid]['depth']['#value']) {
    	case 0:
    		$subnoticia = 0;
				$publicado_tambien = 0;
				$hidden = 0;
    		break;
    	case 1:    
				if ($subnoticia > 0){
					$hidden = 1;
				}
				else {
					$hidden = 0;
				}
				$publicado_tambien = 0;
    		$subnoticia++;
    		break;
   		case 2:
				if ($publicado_tambien > 0){
					$hidden = 1;
				}
				else {
					$hidden = 0;
				}
				$subnoticia = 0;
    		$publicado_tambien++;
    		break;
    }
    $indent = theme('indentation_agenda_informativa', array('size' => $variables['element'][$nid]['depth']['#value'], 'hidden' => $hidden));
    unset($variables['element'][$nid]['depth']);
    $rows[] = array(
      'data' => array(
        $indent . drupal_render($variables['element'][$nid]['name']),
        //drupal_render($variables['element'][$nid]['description']),
        drupal_render($variables['element'][$nid]['weight']),
        drupal_render($variables['element'][$nid]['nid']),
        drupal_render($variables['element'][$nid]['pid']),
      ),
      'class' => $class,
    );
	}
	$table_id = $variables['element']['#attributes']['id'][0];
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
	$output .= drupal_render_children($form);

	drupal_add_tabledrag($table_id, 'match', 'parent', 'item-pid', 'item-pid', 'item-nid', array('Weight'), 2);
	drupal_add_tabledrag($table_id, 'order', 'sibling', 'item-weight', NULL, NULL, FALSE);

  return $output;

}


function crear_arbol($parent, $notas_tema = array(), $depth = 0){
	// http://www.pepperpotdesign.co.uk/blog/drupal-draggable-tables-tutorial-part-2-hierarchic/
	$remnant = array();
	$children = array();
	foreach ($notas_tema as $nid => $item) {
		if ($item['pid'] == $parent) {
			$item['depth'] = $depth;
			$children[$nid] = $item;
		}
		else {
			$remnant[$nid] = $item;
		}
	}
	stable_uasort($children, 'ordenar_por_peso');
	$ancestors = array();
	foreach ($children as $nid => $child) {
		$child_children = crear_arbol($nid, $remnant, $depth +1);
		$ancestors[$nid] = $child;
		if (count($child_children)){
			$ancestors = $ancestors + $child_children;
		}
	}
	return $ancestors;
}

function ordenar_por_peso($a, $b) {
	if ($a['weight'] == $b['weight']){
		return 0;
	}
	return ($a['weight'] < $b['weight']) ? -1 : 1;
}

// http://php.net/manual/es/function.uasort.php#114535
function stable_uasort(&$array, $cmp_function) {
    if(count($array) < 2) {
        return;
    }
    $halfway = count($array) / 2;
    $array1 = array_slice($array, 0, $halfway, TRUE);
    $array2 = array_slice($array, $halfway, NULL, TRUE);

    stable_uasort($array1, $cmp_function);
    stable_uasort($array2, $cmp_function);
    if(call_user_func($cmp_function, end($array1), reset($array2)) < 1) {
        $array = $array1 + $array2;
        return;
    }
    $array = array();
    reset($array1);
    reset($array2);
    while(current($array1) && current($array2)) {
        if(call_user_func($cmp_function, current($array1), current($array2)) < 1) {
            $array[key($array1)] = current($array1);
            next($array1);
        } else {
            $array[key($array2)] = current($array2);
            next($array2);
        }
    }
    while(current($array1)) {
        $array[key($array1)] = current($array1);
        next($array1);
    }
    while(current($array2)) {
        $array[key($array2)] = current($array2);
        next($array2);
    }
    return;
}


function informe_agenda_informativa_preview_form ($form, &$form_state, $iid = NULL) {

	if (isset($iid) && !isset($form_state['triggering_element']['#return_value'])) {
		$variables = informe_agenda_informativa_cargar($iid);
	}

	$form['iid'] = array(
		'#type' => 'hidden',
		'#value' => isset($form_state['values']['iid']) ? $form_state['values']['iid'] : $iid,
	);


	$pagina =  isset($form_state['values']['pagina']) ? $form_state['values']['pagina'] : 0;
	$form['pagina'] = array(
		'#type' => 'hidden',
		'#value' => $pagina,
	);

	$variables = isset($form_state['values']['variables']) ? descifrar($form_state['values']['variables']) : $variables;
	$form['variables'] = array(
		'#type' => 'hidden',
		'#value' => cifrar($variables),
	);

	$variables = unserialize($variables);

	$form['informe'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="informe">',
		'#suffix' => '</div>'
	);

	if ($pagina == 0) {
		$form['informe']['indice'] = array(
			'#type' => 'fieldset',
			'#prefix' => '<div id="indice">',
			'#suffix' => '</div>'
		);


		$form['informe']['indice']['titulo'] = array(
			'#markup' => theme('html_tag', array(
      	'element' => array(
        	'#tag' => 'div',
        	'#attributes' => array('class' => 'titulo'),
        	'#value' => 'Agenda Informativa',
      	)
    	))
		);

		$form['informe']['indice']['tipo'] = array(
			'#markup' => theme('html_tag', array(
      	'element' => array(
        	'#tag' => 'div',
        	'#attributes' => array('class' => 'tipo'),
        	'#value' => isset($tipo) ? $tipo : '',
      	)
    	))
		);

		$form['informe']['indice']['periodo_fecha'] = array(
			'#type' => 'fieldset',
			'#attributes' => array('id' => array('periodo_fecha')),
		);

		$form['informe']['indice']['periodo_fecha']['periodo'] = array(
			'#markup' => theme('html_tag', array(
      	'element' => array(
        	'#tag' => 'div',
        	'#attributes' => array('class' => 'periodo'),
        	'#value' => 'Principales del ' . ( isset($variables['fecha_hasta']) ? 'periodo' : 'día' ),
      	)
    	))
		);
		
		$fecha_desde = format_date($variables['fecha_desde'], 'custom', 'd \d\e F');
		$año_desde = " de " . date('Y', $variables['fecha_desde']);

		if (isset($variables['fecha_hasta'])) {
			$año_hasta = " de " . date('Y', $variables['fecha_hasta']);

			if ($año_hasta == $año_desde) {
				$año_desde = NULL;
			}

			$fecha_hasta = " al " . format_date($variables['fecha_hasta'], 'custom', 'd \d\e F');
		}
		
		$form['informe']['indice']['periodo_fecha']['fecha'] = array(
			'#markup' => theme('html_tag', array(
      	'element' => array(
        	'#tag' => 'div',
        	'#attributes' => array('class' => 'fecha'),
        	'#value' => $fecha_desde . $año_desde . $fecha_hasta . $año_hasta,
      	)
    	))
		);
		$form['informe']['indice']['temas'] = array(
			'#type' => 'fieldset',
			'#attributes' => array('class' => array('temas')),
		);

		$rows = array();
		$cantidad_temas = count($variables['temas']);
		foreach ($variables['temas'] as $key => $values) {
			$rows[$key] = array(
				array('data' => $values['tema'], 'class' => array('tema')),
				$values['pagina']
			);

		}
		$tabla = array();
		$tabla['rows'] = $rows;
		$tabla['attributes'] = array('id' => 'tabla_contenido');

		$form['informe']['indice']['temas']['tabla'] = array(
			'#markup' => theme_table($tabla)
		);
	}
	else {
		$form['informe']['contenido'] = array(
			'#type' => 'fieldset',
			'#attributes' => array('id' => array('contenido')),
		);
		foreach ($variables['temas'] as $key => $values) {
			if ($values['pagina'] == $pagina){
				$form['informe']['contenido']['tema_' . $key] = array(
					'#title' => 'Tema ' . ($key+1),
					'#type' => 'textfield',
					'#attributes' => array('class' => array('tema')),
					'#default_value' => $values['tema'],
					'#disabled' => TRUE,
				);
			}

			$cantidad_publicado_tambien = 0;
			foreach ($variables['notas'][$values['tema']] as $nid => $nota) {
				if ($nota['pagina'] == $pagina) {
					$form['informe']['contenido'][$nid]['#tree'] = 'TRUE';
					$longitud_resumen = mb_strlen($nota['resumen']);
					switch ($nota['depth']) {
						case 0:
							$form['informe']['contenido'][$nid]['title'] = array(
								'#title' => 'GUIA',
								'#type' => 'textfield',
								'#default_value' => isset($form_state['values'][$nid]['title']) ? $form_state['values'][$nid]['title'] : $nota['title'],
								'#disable' => TRUE,
							);
							$form['informe']['contenido'][$nid]['medio'] = array(
								'#markup' => theme('html_tag', array(
						      	'element' => array(
						        	'#tag' => 'div',
						        	'#attributes' => array('class' => 'medio'),
						        	'#value' => isset($form_state['values'][$nid]['medio']) ? $form_state['values'][$nid]['medio'] : $nota['medio'],
						      	)
						    	)),
							);
							$form['informe']['contenido'][$nid]['resumen'] = array(
								'#type' => 'textarea',
								'#default_value' => isset($form_state['values'][$nid]['resumen']) ? $form_state['values'][$nid]['resumen'] : $nota['resumen'],
								'#attributes' => array('class' => array('guia', 'resumen'), 'id' => array($nid)),
								'#rows' => 1,
								'#cols' => 90,
								'#resizable' => FALSE,
							);
							if ($longitud_resumen > 900) {
								$form['informe']['contenido'][$nid]['resumen']['#attributes']['class'][] = 'over';
							}
							$contador = theme('html_tag', array(
									'element' => array('#tag' => 'div', '#attributes' => array('id' => 'counter-' . $nid), '#value' => 0)
								));
							$form['informe']['contenido'][$nid]['info_contador'] = array(
								'#markup' => theme('html_tag', array(
										'element' => array('#tag' => 'div', '#attributes' => array('class' => 'info_contador'), '#value' => "Quedan $contador caracteres")
									))
							);
							$cantidad_publicado_tambien = 0;
							break;
						case 1:
							$form['informe']['contenido'][$nid]['title'] = array(
								'#title' => 'SUBNOTICIA',
								'#type' => 'textfield',
								'#default_value' => isset($form_state['values'][$nid]['title']) ? $form_state['values'][$nid]['title'] : $nota['title'],
								'#disable' => TRUE,
							);
							$form['informe']['contenido'][$nid]['medio'] = array(
								'#markup' => theme('html_tag', array(
						      	'element' => array(
						        	'#tag' => 'div',
						        	'#attributes' => array('class' => 'medio'),
						        	'#value' => isset($form_state['values'][$nid]['medio']) ? $form_state['values'][$nid]['medio'] : $nota['medio'],
						      	)
						    	)),
							);
							$form['informe']['contenido'][$nid]['resumen'] = array(
								'#type' => 'textarea',
								'#default_value' => isset($form_state['values'][$nid]['resumen']) ? $form_state['values'][$nid]['resumen'] : $nota['resumen'],
								'#attributes' => array('class' => array('subnoticia', 'resumen'), 'id' => array($nid)),
								'#rows' => 1,
								'#cols' => 90,
								'#resizable' => FALSE,
							);
							if ($longitud_resumen > 900) {
								$form['informe']['contenido'][$nid]['resumen']['#attributes']['class'][] = 'over';
							}
							$contador = theme('html_tag', array(
									'element' => array('#tag' => 'div', '#attributes' => array('id' => 'counter-' . $nid), '#value' => 0)
								));
							$form['informe']['contenido'][$nid]['info_contador'] = array(
								'#markup' => theme('html_tag', array(
										'element' => array('#tag' => 'div', '#attributes' => array('class' => 'info_contador'), '#value' => "Quedan $contador caracteres")
									))
							);
							$cantidad_publicado_tambien = 0;
							break;
						case 2:
							if ($cantidad_publicado_tambien == 0) {
								$form['informe']['contenido'][$nid]['publicado_tambien'] = array(
									'#markup' => theme('html_tag', array(
							      	'element' => array(
							        	'#tag' => 'label',
							        	'#value' => 'Publicado también en:',
							      	)
							    	)),
								);
							}
							$form['informe']['contenido'][$nid]['title'] = array(
								'#markup' => theme('html_tag', array(
						      	'element' => array(
						        	'#tag' => 'div',
						        	'#attributes' => array('class' => 'publicado_tambien'),
						        	'#value' => isset($form_state['values'][$nid]['title']) ? $form_state['values'][$nid]['title'] : $nota['title'],
						      	)
						    	)),
							);
							$form['informe']['contenido'][$nid]['medio'] = array(
								'#markup' => theme('html_tag', array(
						      	'element' => array(
						        	'#tag' => 'div',
						        	'#attributes' => array('class' => 'publicado_tambien_medio'),
						        	'#value' => isset($form_state['values'][$nid]['medio']) ? $form_state['values'][$nid]['medio'] : $nota['medio'],
						      	)
						    	)),
							);
							$cantidad_publicado_tambien++;
							break;
					}

				}
			}
		}
	}

	$form['informe']['paginador'] = array(
		'#type' => 'fieldset',
		'#attributes' => array('id' => array('paginador')),
	);

	$form['informe']['paginador']['anterior'] = array(
		'#type' => 'image_button',
		'#description' => 'Salvar y página anterior',
		'#title' => ($pagina == 0) ? NULL : 'Salvar y página anterior',
		'#src' => 'sites/all/themes/segana/images/scroll-izq.png',
		'#return_value' => "anterior",
		'#ajax' => array(
			'event' => 'click',
			'callback' => 'form_informe_agenda_informativa_preview_paginador_callback',
			'wrapper' => 'informe',
			'method' => 'replace',
			'effect' => 'fade',
			'progress' => array('type' => 'throbber'),
			//'prevent' => 'submit click mousedown',
		),
		'#disabled' => ($pagina == 0) ? TRUE : FALSE,
	);


	// Buscar la última página
	$ultimo_tema = end($variables['notas']);
	$ultima_nota = end($ultimo_tema);
	$ultima_pagina = $ultima_nota['pagina'];

	$form['informe']['paginador']['siguiente'] = array(
		'#type' => 'image_button',
		'#description' => 'Salvar y siguiente página',
		'#title' => ($pagina == $ultima_pagina) ? NULL : (($pagina == 0) ? 'Siguiente página' : 'Salvar y siguiente página'),
		'#src' => 'sites/all/themes/segana/images/scroll-der.png',
		'#return_value' => "siguiente",
		'#ajax' => array(
			'event' => 'click',
			'callback' => 'form_informe_agenda_informativa_preview_paginador_callback',
			'wrapper' => 'informe',
			'method' => 'replace',
			'effect' => 'fade',
			'progress' => array('type' => 'throbber'),
			//'prevent' => 'submit click mousedown',
		),
		'#disabled' => ($pagina == $ultima_pagina) ? TRUE : FALSE,
	);
	$form['informe']['paginador']['indicador'] = array(
		'#markup' => theme('html_tag', array(
      	'element' => array(
        	'#tag' => 'div',
        	'#attributes' => array('class' => 'indicador'),
        	'#value' => 'Mostrando ' . $pagina . ' de ' . $ultima_pagina . ' páginas',
      	)
    	))
	);

	$form['informe']['links'] = array(
		'#type' => 'fieldset',
		'#attributes' => array('id' => array('links'))
	);

	if ($pagina == 0){
		$variables = array(
		      'path' => 'sites/all/themes/segana/images/descarga.png', 
		      'alt' => 'Descargar informe',
		      'title' => 'Descargar',
		      'width' => '14px',
		      'height' => '14px',
		      'attributes' => array('id' => 'descarga'),
		      );
		$descarga = theme('image', $variables);

		$form['informe']['links']['descarga'] = array(
			'#markup' => l('Descargar ' . $descarga, 'agenda_informativa/export/' . $iid, array('html' => TRUE)),
		);
	}
	else {
		$form['informe']['links']['pagina_indice'] = array(
			'#type' => 'image_button',
			'#description' => 'Ir al índice y salvar',
			'#name' => 'indice',
			'#src' => 'sites/all/themes/segana/images/indice.png',
			'#return_value' => 'indice',
			'#submit' => array('informe_agenda_informativa_preview_form_submit'),
			'#prefix' => '<div id="ir_indice">Índice',
			'#suffix' => '</div>',
			'#ajax' => array(
				'event' => 'click',
				'callback' => 'form_informe_agenda_informativa_preview_paginador_callback',
				'wrapper' => 'informe',
				'method' => 'replace',
				'effect' => 'fade',
				'progress' => array('type' => 'throbber'),
			),
		);
		$form['informe']['links']['salvar'] = array(
			'#type' => 'image_button',
			'#description' => 'Salvar',
			'#name' => 'salvar',
			'#src' => 'sites/all/themes/segana/images/guardar.png',
			'#return_value' => 'salvar',
			'#submit' => array('informe_agenda_informativa_preview_form_submit'),
			'#prefix' => '<div id="salvar">Salvar',
			'#suffix' => '</div>',
			'#ajax' => array(
				'event' => 'click',
				'callback' => 'form_informe_agenda_informativa_preview_salvar_callback',
				'wrapper' => 'informe',
				'method' => 'replace',
				'effect' => 'fade',
				'progress' => array('type' => 'none'),
				'prevent' => 'submit click',
			),
		);
	}

	$form['informe']['paginador']['estado'] = array(
	'#markup' => theme('html_tag', array(
    	'element' => array(
      	'#tag' => 'div',
      	'#attributes' => array('id' => 'estado'),
      	'#value' => '',
    	)
  	))
	);

	$form['#attached']['js'][] = 'sites/all/libraries/jquery-simply-countable/jquery.simplyCountable.js';
	$form['#attached']['js'][] = 'sites/all/libraries/ExpandingTextareas/expanding.js';
	$form['#attached']['js'][] = drupal_get_path('module', 'informe') . '/js/agenda_informativa_preview.js';
	return $form;
}


function informe_agenda_informativa_preview_form_submit($form, &$form_state) {
	$pressed_button = check_plain($form_state['triggering_element']['#return_value']);

	switch ($pressed_button) {
		case 'anterior':
			informe_agenda_informativa_salvar($form_state);

			// actualizar variables
			$variables = unserialize(descifrar($form_state['values']['variables']));
			foreach ($variables['notas'] as $tema => $nids) {
				foreach ($nids as $nid => $values) {
					if ( ($values['pagina'] == $form_state['values']['pagina']) && ($values['depth'] < 2) ){
						$variables['notas'][$tema][$nid]['resumen'] = $form_state['values'][$nid]['resumen'];
					}
				}
			}

			//recalcular paginas al ir al índice
			if ($form_state['values']['pagina'] == 0){
				$variables['notas'] = informe_agenda_informativa_pagina_nota($variables['notas']);
				$variables['temas'] = informe_agenda_informativa_pagina_tema($variables['temas'], $variables['notas']);
			}

			$form_state['values']['variables'] = cifrar(serialize($variables));
			$form_state['values']['pagina']--;
			$form_state['rebuild'] = TRUE;
			break;

		case 'siguiente':
			if ($form_state['values']['pagina'] != 0){
				informe_agenda_informativa_salvar($form_state);

				// actualizar variables
				$variables = unserialize(descifrar($form_state['values']['variables']));
				foreach ($variables['notas'] as $tema => $nids) {
					foreach ($nids as $nid => $values) {
						if ( ($values['pagina'] == $form_state['values']['pagina']) && ($values['depth'] < 2) ){
							$variables['notas'][$tema][$nid]['resumen'] = $form_state['values'][$nid]['resumen'];
						}
					}
				}
				$form_state['values']['variables'] = cifrar(serialize($variables));
			}

			$form_state['values']['pagina']++;
			$form_state['rebuild'] = TRUE;
			break;

		case 'indice':
			informe_agenda_informativa_salvar($form_state);

			// actualizar variables
			$variables = unserialize(descifrar($form_state['values']['variables']));
			foreach ($variables['notas'] as $tema => $nids) {
				foreach ($nids as $nid => $values) {
					if ( ($values['pagina'] == $form_state['values']['pagina']) && ($values['depth'] < 2) ){
						$variables['notas'][$tema][$nid]['resumen'] = $form_state['values'][$nid]['resumen'];
					}
				}
			}


			//recalcular paginas al ir al índice
			$variables['notas'] = informe_agenda_informativa_pagina_nota($variables['notas']);
			$variables['temas'] = informe_agenda_informativa_pagina_tema($variables['temas'], $variables['notas']);

			$form_state['values']['variables'] = cifrar(serialize($variables));

			$form_state['values']['pagina'] = 0;
			$form_state['rebuild'] = TRUE;
			break;
		case 'salvar':
			informe_agenda_informativa_salvar($form_state);
			$form_state['rebuild'] = TRUE;
			break;
	}
}


function form_informe_agenda_informativa_preview_paginador_callback($form, &$form_state) {
	return array(
	    '#type' => 'ajax',
	    '#commands' => array(
	      ajax_command_replace("#informe", render($form['informe'])),
	      array (
					'command' => 'afterAjaxCallbackAgendaInformativaPreview',
				),
				array (
					'command' => 'afterAjaxCallbackAgendaInformativaPreviewGuardar',
					'selectedValue' => $form_state['values']['estado'],
				),
			)
  );
}

function form_informe_agenda_informativa_preview_salvar_callback($form, &$form_state) {
	return array(
	    '#type' => 'ajax',
	    '#commands' => array(
	      array (
					'command' => 'afterAjaxCallbackAgendaInformativaPreviewGuardar',
					'selectedValue' => $form_state['values']['estado'],
				),
			)
  );
}


function informe_agenda_informativa_cargar($iid) {
	$informe = informe_load($iid, array('type' => 'agenda_informativa'));
	$language = LANGUAGE_NONE;

	$tema_array = array();
	foreach ($informe->field_tema[$language] as $delta => $value) {
		$term = taxonomy_term_load($value['tid']);
		$temas[$value['tid']]['tema'] = $term->name;
	}
	$temas_tid = $temas;
	$temas = array_values($temas);

	$variables = array();

	$variables['fecha_desde'] = $informe->field_fecha_desde[$language][0]['value'];
	$variables['fecha_hasta'] = isset($informe->field_fecha_hasta[$language][0]['value']) ? $informe->field_fecha_hasta[$language][0]['value'] : NULL;
	$variables['tipo'] = ( ($informe->field_ambito[$language][0]['value']) == 0) ? NULL : ( ($informe->field_ambito[$language][0]['value'] == 1) ? 'Nacional' : 'Internacional');
	$notas = array();
	foreach ($informe->field_nota[$language] as $delta => $value) {

		$tid = $informe->field_tema[$language][$delta]['tid'];
		$tema = $temas_tid[$tid]['tema'];
		$nid = $value['value'];
		$node = node_load($nid);
		$title = html_entity_decode($node->title);
		$pid = $informe->field_nota_padre[$language][$delta]['value'];
		$weight = $informe->field_weight[$language][$delta]['value'];
		$depth = $informe->field_depth[$language][$delta]['value'];
		$resumen = isset($node->field_resumen[$language][0]['value']) ? html_entity_decode($node->field_resumen[$language][0]['value']) : html_entity_decode($node->body[$language][0]['value']);
		$medio_term = taxonomy_term_load($node->field_medio[$language][0]['tid']);
		$medio = $medio_term->name;

		$notas[$tema][$nid] = array(
			'title' => $title,
			'pid' => $pid,
			'weight' => $weight,
			'depth' => $depth,
			'resumen' => preg_replace( "/\n\s+/", "", $resumen ),
			'medio' => $medio,
		);

	}
	$notas = informe_agenda_informativa_pagina_nota($notas);
	$temas = informe_agenda_informativa_pagina_tema($temas, $notas);
	
	$variables['notas'] = $notas;
	$variables['temas'] = $temas;

	return serialize($variables);
}

function informe_agenda_informativa_pagina_nota($notas) {
	$pagina = 1;
	$lineas_por_pagina = 50;
	$lineas_pagina_actual = 0;
	$caracteres_por_linea = 101;
	$cantidad_publicado_tambien = 0;
	$max_lineas = 45;
	foreach ($notas as $tema => $nids) {
		foreach ($nids as $nid => $values) {
			switch ($values['depth']) {
				case 0:
					// margen superior (2) + medio(3) + link (2) + margen inferior (1)
					$tamano = 7;
					$longitud_titulo = mb_strlen($values['title']);
					$tamano_titulo = ceil($longitud_titulo/60) * 3;
					$longitud_resumen = (mb_strlen($values['resumen']) > 900) ? 900 : mb_strlen($values['resumen']);
					$tamano_resumen = ceil($longitud_resumen/$caracteres_por_linea);
					$tamano = $tamano + $tamano_titulo + $tamano_resumen;
					$cantidad_publicado_tambien = 0;
					break;
				case 1:
					// margen superior (2) + medio(2) + link (2) + margen inferior (1)
					$tamano = 7;
					$longitud_titulo = mb_strlen($values['title']);
					$tamano_titulo = ceil($longitud_titulo/69) * 2;
					$longitud_resumen = (mb_strlen($values['resumen']) > 900) ? 900 : mb_strlen($values['resumen']);
					$tamano_resumen = ceil($longitud_resumen/$caracteres_por_linea);
					$tamano = $tamano + $tamano_titulo + $tamano_resumen;
					$cantidad_publicado_tambien = 0;
					break;
				case 2:
					$tamano_encabezado = ($cantidad_publicado_tambien == 0) ? 2 : 0;
					$longitud_titulo = mb_strlen($values['title']);
					$tamano_titulo = ceil($longitud_titulo/69);
					$tamano = $tamano_encabezado + $tamano_titulo;
					$cantidad_publicado_tambien++;
					break;
			}
			if ($lineas_pagina_actual < $max_lineas) {
				if ( ($lineas_pagina_actual + $tamano) > $max_lineas) {
					$pagina++;
					$lineas_pagina_actual = $tamano;
				}
				else {
					$lineas_pagina_actual = $lineas_pagina_actual + $tamano;
				}
			}
			else {
				$pagina++;
				$lineas_pagina_actual = $tamano;
			}
			$notas[$tema][$nid]['tamano'] = $tamano;
			$notas[$tema][$nid]['pagina'] = $pagina;

		}
	}
	return $notas;
}

function informe_agenda_informativa_pagina_tema ($temas, $notas) {
	foreach ($temas as $key => $values) {
		$tema = $values['tema'];
		$primera_nota_por_tema = reset($notas[$tema]);
		$pagina_tema = ($primera_nota_por_tema['pagina'] < 10) ? sprintf("%02d", $primera_nota_por_tema['pagina']) : $primera_nota_por_tema['pagina'];
		$temas[$key] = array(
			'tema' => $tema,
			'pagina' => $pagina_tema
		);
	}
	return $temas;
}


// para que almacene  la fecha changed cuando se almacena el resumen dentro del nodo
function informe_node_presave($node){
	$node->changed = REQUEST_TIME;
}

function informe_agenda_informativa_salvar(&$form_state) {
	$language = LANGUAGE_NONE;
	//notas en la pagina actual
	$pagina_actual = $form_state['values']['pagina'];
	$variables = unserialize(descifrar($form_state['values']['variables']));
	foreach ($variables['notas'] as $tema => $nids) {
		foreach ($nids as $nid => $values) {
			if ( ($pagina_actual == $values['pagina']) && ($values['depth'] < 2) ){
				// si algun campo resumen es mayor al valor permitido se retorna -1 que significa un valor invalido
				if (mb_strlen($form_state['values'][$nid]['resumen']) > 900){
					$form_state['values']['estado'] = -1;
					return -1;
				}
				//actualizar nodo solo si hubo cambios
				if ($variables['notas'][$tema][$nid]['resumen'] != $form_state['values'][$nid]['resumen']) {
					$node = node_load($nid);
					$changed = $node->changed;
					$node->field_resumen[$language][0]['value'] = trim($form_state['values'][$nid]['resumen']);
					node_save($node);
					if ($changed == $node->changed) {
						$form_state['values']['estado'] = 0;
						// hubo error al salvar el documento
						return 0;
					}
				}
			}
		}
	}

	// el salvado fue exitoso
	$form_state['values']['estado'] = 1;
	return 1;
}

?>

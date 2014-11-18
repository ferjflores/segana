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
		'#value' => $form_state['values']['iid'] ? $form_state['values']['iid'] : ($iid ? $iid : NULL),
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
		'#default_value' => ($form_state['values']['tipo']) ? $form_state['values']['tipo'] : (isset($tipo) ? $tipo : 0),
		'#options' => $tipos,
	);


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





	if (0 === strpos($form_state['triggering_element']['#return_value'], 'nota_') ){
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
		if (empty($form_state['values']['busqueda'][$nid_nota_seleccionada])) {
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
			'#attributes' => array('autocomplete' =>'off'),
			'#default_value' => !empty($form_state['storage']['values']["tema_$i"]) ? $form_state['values']["tema_$i"] : (isset($tema_array[$i]) ? $tema_array[$i] : NULL),
			'#attributes' => array('class' => array('tema',($tema_seleccionado == $i ? 'tema_seleccionado' : NULL))),
			'#size' => 80,
			'#maxlength' => 128,
			'#autocomplete_path' => 'notas/autocomplete/tema/' . $fecha_desde . '/0',
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



	$form['contenedor']['superior']['botones'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="botones">',
		'#suffix' => '</div>',
	);


	$form['contenedor']['superior']['botones']['buscar'] = array(
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
			'progress' => array('type' => 'none'),
			//'prevent' => 'submit click mousedown',
		),
	);

  $form['#prefix'] = '<div id="formwrapper">';
  $form['#suffix'] = '</div>';

	//$form_id = str_replace('_', '-', $form_state['build_info']['form_id']);
	$form['contenedor']['superior']['botones']['submit'] = array(
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

	$form['contenedor']['superior']['botones']['ver_informe'] = array(
		'#type' => 'submit',
		'#value' => t('Ver Informe'),
		'#return_value' => 'ver_informe',
		'#name' => 'ver_informe',
		'#submit' => array('informe_agenda_informativa_form_submit'),
		'#ajax' => array(
			'event' => 'click',
			'callback' => 'form_informe_agenda_informativa_callback',
			'wrapper' => 'formwrapper',
			'method' => 'replace',
			'effect' => 'fade',
			'progress' => array('type' => 'none'),
			//'prevent' => 'submit click mousedown',
		),
	);


	$form['contenedor']['superior']['botones']['reset'] = array(
		'#type' => 'submit',
		'#value' => t('Reset'),
		'#name' => 'reset',
		'#return_value' => t('Reset'),
//		'#attributes' => array('onclick' => 'this.form.reset(); return false;'),
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

		foreach ($notas_busqueda as $nid => $values) {
			
			$form['contenedor']['inferior']['busqueda'][$nid] = array(
				'#type' => 'checkbox',
				'#title' => '<span>' . $values['title'] .'</span><span>'. $values['medio'] .'</span><span>' . $values['fecha'] . '</span>',
				'#return_value' => "nota_" . $nid . "_" . $tema_seleccionado . "_" . $values['title'],
				'#attributes' => (isset($form_state['values']['busqueda'][$nid]) && $form_state['values']['busqueda'][$nid] == 1 ) ? array('checked' => 'checked') : NULL,
				'#ajax' => array(
					'event' => 'click',
					'callback' => 'form_informe_agenda_informativa_nota_callback',
					'method' => 'html',
					'effect' => 'fade',
					'progress' => array('type' => 'none'),
					//'prevent' => 'submit click mousedown',
				),
			);
		}
	}
	if ($form_state['triggering_element']['#return_value'] == 'almacenar') {
		$form['contenedor']['inferior']['mensaje'] = array (
			'#markup' => "Informe guardado con id " . $form_state['values']['iid'],
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
					'title' => $title
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


function form_informe_agenda_informativa_buscar_callback($form, $form_state) {
	return array(
	    '#type' => 'ajax',
	    '#commands' => array(
	      ajax_command_replace("#busqueda", render($form['contenedor']['inferior']['busqueda'])),
    	)
  );
}

function form_informe_agenda_informativa_nota_callback($form, &$form_state) {
	$tema_seleccionado = $form_state['values']['tema_seleccionado'];

	return array(
	    '#type' => 'ajax',
	    '#commands' => array(
	     	ajax_command_replace("#busqueda", render($form['contenedor']['inferior']['busqueda'])),
	     	ajax_command_replace("#contenedor_notas_" . $tema_seleccionado, render($form['contenedor']['superior']['temas'][$tema_seleccionado]["notas_$tema_seleccionado"])),
    	)
  );
}

function form_informe_agenda_informativa_callback($form, &$form_state) {
	return $form['contenedor']['inferior'];
}

function theme_tabledrag_notas($variables) {
	$rows = array();
	foreach (element_children($variables['element']) as $nid) {
		$variables['element'][$nid]['weight']['#attributes']['class'] = array('item-weight tabledrag-hide');
		$variables['element'][$nid]['nid']['#attributes']['class'] = array('item-nid tabledrag-hide' );
    $variables['element'][$nid]['pid']['#attributes']['class'] = array('item-pid tabledrag-hide');
    $class = array('draggable');
    $indent = theme('indentation', array('size' => $variables['element'][$nid]['depth']['#value']));
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
	uasort($children, 'ordenar_por_peso');
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

?>

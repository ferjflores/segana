<?php

function grafico_lineal($variables, $tipo = NULL, $combinado = NULL, $ejeX = 'tiempo') {
	if ($ejeX == 'tiempo'){
		


		// obtener el top n por $tipo
		$query = db_select('node', 'n');
		$query->join('field_data_field_fecha', 'ff', 'n.nid = ff.entity_id');

		$query = query_area($query, $variables, $tipo);
		$query = query_discursivo($query, $variables, $tipo);
		$query = query_medio($query, $variables, $tipo);
		$query = query_palabra_clave($query, $variables);


		$query->addExpression('COUNT(n.nid)', 'nid_count');
		$query
			->fields('ttd', array('tid'))
			->condition('ff.entity_type', 'node')
			->groupBy('ttd.tid')
			->range(0, $variables['cantidad_lineal']);

			
		if ($variables['fecha_hasta']) {
			$query->condition('ff.field_fecha_value', $variables['fecha_desde'], '>=')
				->condition('ff.field_fecha_value', $variables['fecha_hasta'], '<=');
		}
		else {
			$query->condition('ff.field_fecha_value', $variables['fecha_desde'], '=');
		}
			
		$query->orderBy('nid_count', 'DESC');
		$query_tipo = $query;



		$query = db_select('node', 'n');
		$query->join('field_data_field_fecha', 'ff', 'n.nid = ff.entity_id');
		$query = query_area($query, $variables, $tipo);
		$query = query_discursivo($query, $variables, $tipo);
		$query = query_medio($query, $variables, $tipo);
		$query = query_palabra_clave($query, $variables);

		if ($tipo) {
			$query->addJoin('INNER', $query_tipo, 'top_tipo', 'top_tipo.tid = ttd.tid');
		}


		$query->addExpression('COUNT(n.nid)', 'nid_count');
		$query
			->condition('ff.entity_type', 'node')
			->condition('ff.field_fecha_value', $variables['fecha_desde'], '>=')
			->condition('ff.field_fecha_value', $variables['fecha_hasta'], '<=')
			->fields('ff', array('field_fecha_value'))
			->groupBy('fecha')
			->orderBy('fecha', 'ASC');

		if ( ($variables['fecha_hasta'] - $variables['fecha_desde']) >= (5 * 30 * 24 * 3600) ) {
			$query->addExpression("UNIX_TIMESTAMP(CONCAT(FROM_UNIXTIME(ff.field_fecha_value, '%Y%m'), '01'))", 'fecha');
			$tooltip = array('day' => '%B %Y');
		}
		elseif ( ($variables['fecha_hasta'] - $variables['fecha_desde']) < (5 * 30 * 24 * 3600) && ($variables['fecha_hasta'] - $variables['fecha_desde']) > (2 * 30 * 24 * 3600) ) {
			//seleccionar el primer dia de la semana de la fecha correspondiente, para agrupar por semana y graficar agrupado por esta.
			$query->addExpression("UNIX_TIMESTAMP(STR_TO_DATE(CONCAT(FROM_UNIXTIME(ff.field_fecha_value, '%Y%u'), ' Monday'), '%x%v %W'))", 'fecha');
			$tooltip = array('week' => "Semana del %A, %b %e, %Y");
		}
		else {
			$query->addField('ff', 'field_fecha_value', 'fecha');
		}

		// agrupar por tipo
		if ($tipo) {
			$query
				->fields('ttd', array('name'))
				->groupBy('ttd.name');
		}
		$resultado = $query->execute();
		$serie = array();
		foreach ($resultado as $row) {
			
			if ($tipo) {
				if (isset($combinado)) {
					$serie_combinado[$row->fecha] = $row->nid_count + (isset($serie_combinado[$row->fecha]) ? $serie_combinado[$row->fecha] : NULL);
					$serie_barra[$row->name][] = array((intval($row->fecha) * 1000 ) - (4.5*3600*1000), intval($row->nid_count));
					$serie_torta[$row->name] = $row->nid_count + (isset($serie_torta[$row->name]) ? $serie_torta[$row->name] : NULL);
				}
				else {
					$serie[$row->name][] = array((intval($row->fecha) * 1000 ) - (4.5*3600*1000), intval($row->nid_count));
					$top_tipo[$row->name] = (isset($top_tipo[$row->name]) ? $top_tipo[$row->name] : NULL) + $row->nid_count;
				}

			}
			else {
				$serie[] = array((intval($row->fecha) * 1000 ) - (4.5*3600*1000), intval($row->nid_count));
			}
		}

		if ($combinado) {
			$serie = array();
			foreach ($serie_combinado as $fecha => $nid_count) {
				$serie[] = array(intval(($fecha * 1000) - (4.5*3600*1000) ), intval($nid_count));
			}
			$serieX = array();
			$serieY = array();
			foreach ($serie_torta as $name => $nid_count) {
				$serieX[] = $nid_count;
				$serieY[] = $name;
			}
		}

		$chart = array(
			'#type' => 'chart',
			//'#chart_type' => 'column',
			'#title' => t("Comportamiento en el período del ". date('d-m-Y',$variables['fecha_desde']) . " hasta el " . date('d-m-Y',$variables['fecha_hasta'])),
			'#legend_position' => 'bottom',
			'#title_font_style' => 'bold',
			'#title_color' => '#8797A0',
			'#title_font_weight' => 'bold',
			'#font' => 'ubuntu',
			'#title_font_size' => 18,
			'#dateTimeLabelFormats' => isset($tooltip) ? $tooltip : NULL,
			'#zoomType' => 'x',
			'#width' => 1054,
			'#colors' => $variables['paleta_colores'],
		);



		if ($tipo) {
			///Subtitulo
			// corregir accento del area
			$tipo = $tipo == 'area' ? 'área' : $tipo;

			///plural de tipos
			switch ($tipo) {
				case 'matriz':
					$plural_tipo = 'matrices';
					break;
				case 'actor':
					$plural_tipo = 'actores';
					break;
				default:
					$plural_tipo = $tipo ."s";
					break;
			}
			$chart['#subtitle'] = t("Principales $plural_tipo");
			$chart['#subtitle_color'] = '#8797A0';
			$chart['#subtitle_font_weight'] = 'normal';
			$chart['#subtitle_font_size'] = 13;
		}




		if (($tipo) && !isset($combinado)) {

			$chart['#tooltips_shared'] = TRUE;

			// ordernar por cantidad de notas
			arsort($top_tipo, SORT_NUMERIC);
			
			// seleccionar los tres primeros
			//$top_tipo = array_slice($top_tipo, 0, 3, TRUE);
			

			foreach ($top_tipo as $tipo_name => $value) {
					$chart[$tipo_name] = array(
					'#type' => 'chart_data',
					'#chart_type' => 'spline',
					'#data' => $serie[$tipo_name],
					'#title' => t($tipo_name),
					'#target_axis' => 'yaxis',
				);
			}
			$chart['#plotOptions'] = array(
				'series' => array(
					'stickyTracking' => FALSE,
				),
				/*'spline' => array(
					'tooltip' => array(
						'dateTimeLabelFormats' => array('week' => "Semana del %A, %b %e, %Y"),
						'shared' => TRUE,
					),
				),*/
			);
		}
		else {
			$chart['line'] = array(
				'#type' => 'chart_data',
				'#chart_type' => 'spline',
				'#data' => $serie,
				'#title' => (isset($combinado) ? t('Total notas') : t('Número de Notas')),
				'#target_axis' => 'yaxis',
				'#color' => $variables['paleta_colores'][7],
				'#marker_lineWidth' => 2,
				'#marker_fillColor' => 'white',
				'#marker_radius' => 4,
				'#marker_lineColor' => $variables['paleta_colores'][7],
			);

			if (isset($combinado)) {
				foreach ($serie_barra as $title => $values) {
					$chart[$title] = array(
						'#type' => 'chart_data',
						'#chart_type' => 'column',
						'#title' => t($title),
						'#data' => $serie_barra[$title],
						//'#width' => '250px',
						//'#showInLegend' => FALSE,
						//'#labels' => array($key),
						'#target_axis' => 'yaxis',
					);
					$chart['pie'] = array(
						'#type' => 'chart_data',
						'#chart_type' => 'pie',
						'#title' => t('Número de notas'),
						'#labels' => $serieY,
						'#data' => $serieX,
						'#show_in_legend' => FALSE,
						'#zIndex' => -10,
						'#center' => array(60, 0),
						'#size' => 120,

					);
					$chart['#plotOptions'] = array(
						'pie' => array(
							/*'borderWidth' => 0,*/
							'cursor' => 'pointer',
							'dataLabels' => array(
								'distance' => -25,
								'color' => 'white',
								'style' => array('fontSize' => 12),
								'connectorWidth' => 0,
								'inside' => TRUE,
								'padding' => 0,
								'y' => 0,
								'x' => 0,
								'format' => '{percentage:.1f}%',
							),
							 'allowPointSelect' => TRUE,
						),
					);


				}

			}
		}


		// formato para dia en el eje x
		if ( ($variables['fecha_hasta'] - $variables['fecha_desde']) <= (10 * 24 * 3600) ) {
			$day_format = "%A %e %b";
		}
		else {
			$day_format = "%e %b";
		}


		$chart['xaxis'] = array(
			'#type' => 'chart_xaxis',
			'#axis_type' => 'datetime',
			'#dateTimeLabelFormats' => array('day' => $day_format, 'hour' => ' ', ),
			'#labels_step' => 1,
			'#maxPadding' => 0.05,
		);

		$chart['yaxis'] = array(
			'#type' => 'chart_yaxis',
			'#axis_type' => 'linear',
			'#title' => t('Notas'),
			'#labels_color' => 'black',
			'#title_color' => 'black',
			'#min' => 0,
			'#allowDecimals' => FALSE,
		);
	}
/*	if ($tipo) {
		
		foreach ($top_tipo as $key => $value) {
			$a .= $key ." ". $value ."</br>";
		}
		return $a;
	}
	else {*/
		return drupal_render($chart);
	//}
}


function grafico_simple($variables, $tipo) {

	$rango = ($variables['otros']) ? NULL : 8;

	$query = query_por_tipo($variables, $tipo, $rango);

	$resultado = $query->execute();
	$serieY = array();
	$serieX = array();
	$serie = array();
	$contador = 0;

	switch ($tipo) {
		case 'area':
			$otros = 'Otras áreas';
			break;
		case 'tema':
			$otros = 'Otros temas';
			break;
		case 'matriz':
			$otros = 'Otras matrices';
			break;
		case 'actor':
			$otros = 'Otros actores';
			break;
		case 'medio':
			$otros = 'Otros medios';
			break;
	}

	foreach ($resultado as $row) {
		if ( ($variables['otros']) && $contador > 6) {
			$serieX[7] = intval((isset($serieX[7]) ? $serieX[7] : NULL) + $row->nid_count);
			$serieY[7] = $otros;
			$serie[$otros] = intval((isset($serie[$otros]) ? $serie[$otros] : NULL) + intval($row->nid_count));
		}
		else {
			$serieX[] = intval($row->nid_count);
			$serieY[] = $row->name;
			$serie[$row->name] = intval($row->nid_count);
		}
		$contador++;
		
	}



	//subtitulo
	$tipos = tipos();
	$tipos[] = 'medio';

	$subtitulo = NULL;
	foreach ($tipos as $value) {
		if (isset($variables[$value])) {
			$subtitulo .= '<b>' . mb_ucfirst($value) . ':</b> ';
			unset($elementos);
			$elementos = NULL;
			foreach ($variables[$value] as $value2) {
				$term = taxonomy_term_load($value2);
				$name = $term->name;
				$elementos .= $name .", ";
			}
			$value = $value == 'area' ? 'área' : $value;
			$elementos = substr($elementos, 0, -2);
			$subtitulo .= $elementos . '    ';
		}
	}


	// corregir accento del area
	$tipo = $tipo == 'area' ? 'área' : $tipo;
	$torta = array(
		'#type' => 'chart',
		'#title' => t('Porcentaje de notas por ' . $tipo ),
		'#chart_library' => 'highcharts',
		'#data_labels' => TRUE,
		'#tooltips' => TRUE,
		'#colors' => $variables['paleta_colores'],
		
		'#title_font_style' => 'bold',
		'#title_color' => '#8797A0',
		'#title_font_weight' => 'bold',
		'#font' => 'ubuntu',
		'#title_font_size' => 18,
		'#width' => 400,
		'#height' => 450,
		'#subtitle' => isset($subtitulo) ? $subtitulo : NULL,
		'#subtitle_color' => '#8797A0',
		'#subtitle_font_weight' => 'normal',
		'#subtitle_font_size' => 11,
		'#legend' => ($variables['leyenda']) ? TRUE : FALSE,
		'#legend_position' => 'bottom',
		'#legend_width' => 400,
		'#legend_itemWidth' => ($variables['leyenda']) ? 400 / $variables['leyenda'] : 400,
		'#legend_itemStyle' => array('width' => (($variables['leyenda']) ? (400 / $variables['leyenda']) - 6 : 400) - 14 ),
		'#legend_symbolRadius' => 5,
		'#legend_symbolHeight' => 10,
		'#legend_symbolWidth' => 10,
		'#legend_symbolPadding' => 4,
		'#legend_itemMarginBottom' => 1,

	);
	$torta['pie'] = array(
		'#type' => 'chart_data',
		'#chart_type' => 'pie',
		'#title' => t('Número de notas'),
		'#labels' => $serieY,
		'#data' => $serieX,
	);
	$torta['#plotOptions'] = array(
		'series' => array(
			'dataLabels' => array(
				'enabled' => TRUE,
			),
		),
		'pie' => array(
			'borderWidth' => 0,
			'cursor' => 'pointer',
			'dataLabels' => array(
				'distance' => (($variables['conectores']) ? 20 : 38),
				'color' => '#424a52',
				'style' => array('fontWeight' => 'bold', 'fontSize' => 16),
				'connectorPadding' => (($variables['conectores']) ? 0 : -30),
				'connectorWidth' => (($variables['conectores']) ? 1 : 0),
				'inside' => TRUE,
				'padding' => 0,
				'y' => 0,
				'x' => 0,
				'format' => '{percentage:.1f}%',
			),
			 'allowPointSelect' => TRUE,
		),
	);


	$columnas = array(
		'#type' => 'chart',
		'#title' => t('Número de notas por ' . $tipo ),
		'#chart_library' => 'highcharts',
		'#data_labels' => TRUE,
		'#tooltips' => FALSE,
		'#colors' => $variables['paleta_colores'],
		'#title_font_style' => 'bold',
		'#title_color' => '#8797A0',
		'#title_font_weight' => 'bold',
		'#font' => 'ubuntu',
		'#title_font_size' => 18,
		'#legend' => FALSE,
		'#width' => 753,
		'#height' => 450,
		'#subtitle' => isset($subtitulo) ? $subtitulo : NULL,
		'#subtitle_color' => '#8797A0',
		'#subtitle_font_weight' => 'normal',
		'#subtitle_font_size' => 11,
	);

	/*foreach ($serie as $key => $value) {
		$data = array();
		foreach ($serie as $key2 => $value2) {
			if ($key == $key2) {
				$data[] = $value2;
			}
			else {
				$data[] = NULL;
			}
		}
		$columnas[$key] = array(
			'#type' => 'chart_data',
			'#chart_type' => 'column',
			'#title' => t($key),
			'#data' => $data,
			'#width' => '250px',
			'#showInLegend' => FALSE,
			'#labels' => array($key),
			'#target_axis' => 'yaxis',
		);
	}*/
	$columnas['barras_columnas'] = array(
		'#type' => 'chart_data',
		'#chart_type' => $variables['tipo-simple'],
		'#title' => t('Número de notas'),
		'#data' => $serieX,
		'#width' => '250px',
		'#showInLegend' => FALSE,
		'#labels' => $serieY,
		'#target_axis' => 'yaxis',
	);
	$columnas['#plotOptions'] = array(
		'series' => array(
			'dataLabels' => array(
				'enabled' => TRUE,
				'style' => array('fontWeight' => 'bold', 'fontSize' => 15),
				//'distance' => 50,
				'inside' => FALSE,
			),
			'stacking' => 'normal',
			'colorByPoint' => TRUE,

		),
		'column' => array(
			'grouping' => FALSE,
			'inside' => FALSE,
		),
	);
	$columnas['xaxis'] = array(
		'#type' => 'chart_xaxis',
		'#labels' => $serieY,
		'#labels_font_size' => 12,
		'#labels_step' => 0,
		'#maxStaggerLines' => 1,


	);	
	$columnas['yaxis'] = array(
		'#type' => 'chart_yaxis',
		'#axis_type' => 'linear',
		'#title' => t('Notas'),
		'#labels_color' => 'black',
		'#title_color' => 'black',
		'#min' => 0,
		'#title_font_size' => 14,
		'#allowDecimals' => FALSE,
	);

	$tabs['items'] = array(drupal_render($torta), drupal_render($columnas));
	$tabs['type'] = 'ul';

	return theme('item_list', $tabs);

}


function graficos_barras($variables, $tipo, $tipo2) {

	// obtener el top n por $tipo
	$query = db_select('node', 'n');
	$query->join('field_data_field_fecha', 'ff', 'n.nid = ff.entity_id');

	$query = query_area($query, $variables, $tipo);
	$query = query_discursivo($query, $variables, $tipo);
	$query = query_medio($query, $variables, $tipo);
	$query = query_palabra_clave($query, $variables);


	$query->addExpression('COUNT(n.nid)', 'nid_count');
	$query
		->fields('ttd', array('tid'))
		->condition('ff.entity_type', 'node')
		->groupBy('ttd.tid')
		->range(0,$variables['cantidad_combinado']);

		
	if ($variables['fecha_hasta']) {
		$query->condition('ff.field_fecha_value', $variables['fecha_desde'], '>=')
			->condition('ff.field_fecha_value', $variables['fecha_hasta'], '<=');
	}
	else {
		$query->condition('ff.field_fecha_value', $variables['fecha_desde'], '=');
	}
		
	$query->orderBy('nid_count', 'DESC');
	$query_tipo = $query;




	// obtener el top n por $tipo2
	$query = db_select('node', 'n');
	$query->join('field_data_field_fecha', 'ff', 'n.nid = ff.entity_id');

	$query = query_area($query, $variables, $tipo2);
	$query = query_discursivo($query, $variables, $tipo2);
	$query = query_medio($query, $variables, $tipo2);
	$query = query_palabra_clave($query, $variables);


	$query->addExpression('COUNT(n.nid)', 'nid_count');
	$query
		->fields('ttd', array('tid'))
		->condition('ff.entity_type', 'node')
		->groupBy('ttd.tid')
		->range(0,$variables['cantidad_combinado']);

		
	if ($variables['fecha_hasta']) {
		$query->condition('ff.field_fecha_value', $variables['fecha_desde'], '>=')
			->condition('ff.field_fecha_value', $variables['fecha_hasta'], '<=');
	}
	else {
		$query->condition('ff.field_fecha_value', $variables['fecha_desde'], '=');
	}
		
	$query->orderBy('nid_count', 'DESC');
	$query_tipo2 = $query;



	$query = db_select('node', 'n');
	$query->join('field_data_field_fecha', 'ff', 'n.nid = ff.entity_id');

	$query = query_area($query, $variables, $tipo);
	$query = query_discursivo($query, $variables, $tipo);
	$query = query_medio($query, $variables, $tipo);
	$query = query_palabra_clave($query, $variables);

	$query->join('field_data_field_' . $tipo2, 'tipo2', 'n.nid = tipo2.entity_id');
	$query->join('taxonomy_term_data', 'ttd2', 'tipo2.field_' . $tipo2 . '_tid = ttd2.tid');

	$query->addJoin('INNER', $query_tipo, 'top_tipo', 'top_tipo.tid = ttd.tid');
	$query->addJoin('INNER', $query_tipo2, 'top_tipo2', 'top_tipo2.tid = ttd2.tid');


	$query->addExpression('COUNT(n.nid)', 'nid_count');
	$query
		->condition('ff.entity_type', 'node')
		->groupBy('ttd.tid')
		->groupBy('ttd2.tid');

	$query->addField('ttd', 'name', 'name');
	$query->addField('ttd2','name', 'name2');

		
	if ($variables['fecha_hasta']) {
		$query->condition('ff.field_fecha_value', $variables['fecha_desde'], '>=')
			->condition('ff.field_fecha_value', $variables['fecha_hasta'], '<=');
	}
	else {
		$query->condition('ff.field_fecha_value', $variables['fecha_desde'], '=');
	}
	$query->orderBy('ttd.name', 'ASC');
	$query->orderBy('ttd2.name', 'ASC');


	$resultado = $query->execute();

	foreach ($resultado as $row) {
		$serie[$row->name2][$row->name] = (int)$row->nid_count;
		$lista_tipo[$row->name] = $row->nid_count + (isset($lista_tipo[$row->name]) ? $lista_tipo[$row->name] : NULL);
	}


	// corregir accento del area
	$tipo = $tipo == 'area' ? 'área' : $tipo;
	///plural de tipos
	switch ($tipo2) {
		case 'matriz':
			$titulo = "Número de notas de las principales matrices por $tipo";
			break;
		case 'actor':
			$titulo = "Número de notas de los principales actores por $tipo";
			break;
		default:
			$titulo = "Número de notas de las principales " . $tipo2 . "s por " .$tipo;
			break;
	}
	///plural de tipos
	switch ($tipo2) {
		case 'matriz':
			$plural_tipo2 = 'matrices';
			break;
		case 'actor':
			$plural_tipo2 = 'actores';
			break;
		default:
			$plural_tipo2 = $tipo2 ."s";
			break;
	}
	switch ($tipo) {
		case 'matriz':
			$plural_tipo = 'matrices';
			break;
		case 'actor':
			$plural_tipo = 'actores';
			break;
		default:
			$plural_tipo = $tipo ."s";
			break;
	}
	$columnas = array(
		'#type' => 'chart',
		'#title' => t($titulo),
		'#chart_library' => 'highcharts',
		//'#data_labels' => TRUE,
		'#tooltips' => TRUE,
		'#colors' => $variables['paleta_colores'],
		'#title_font_style' => 'bold',
		'#title_color' => '#6B7A84',
		'#title_font_weight' => 'bold',
		'#font' => 'ubuntu',
		'#title_font_size' => 18,
		'#legend' => TRUE,
		'#width' => 1153,
		'#height' => 450,
		'#legend_title' => mb_ucfirst($plural_tipo2),
		'#legend_title_font_size' => 15,
		'#legend_title_font_weight' => 'normal',
		'#legend_title_color' => '#8797A0',
		'#legend_itemMarginBottom' => 3,

		//'#legend_position' => 'bottom',
	);

	
	// ordernar por cantidad de notas
	arsort($lista_tipo, SORT_NUMERIC);
	$lista_tipo = array_keys($lista_tipo);



	foreach ($serie as $tipo2 => $value) {
		$data = array();
		foreach ($lista_tipo as $tipo) {
			$data[] = !empty($value[$tipo]) ? $value[$tipo] : NULL;
		}
		$columnas[$tipo2] = array(
			'#type' => 'chart_data',
			'#chart_type' => $variables['tipo-combinado'],
			'#title' => t($tipo2),
			'#data' => $data,
			'#width' => '250px',
			//'#showInLegend' => FALSE,
			//'#labels' => array($key),
			'#target_axis' => 'yaxis',
		);
	}
	$columnas['#plotOptions'] = array(
		'series' => array(
			'dataLabels' => array(
				'enabled' => FALSE,
				'style' => array('fontWeight' => 'normal', 'fontSize' => 12),
				//'distance' => 50,
				'inside' => FALSE,
			),
			'stacking' => $variables['apiladas'] ? 'normal' : NULL,
			'groupPadding' => 0.05,
			'pointPadding' => 0.04,
			'minPointLength' => 3,
		),
		$variables['tipo-combinado'] => array(
			'borderWidth' => 0,
		),
	);
	$columnas['xaxis'] = array(
		'#type' => 'chart_xaxis',
		'#labels' => $lista_tipo,
		'#labels_font_size' => 12,
		'#labels_step' => 0,
		'#maxStaggerLines' => 1,
		'#title' => t(mb_ucfirst($plural_tipo)),
		'#title_color' => '#8797A0',
		'#title_font_size' => 13,

	);

	$columnas['yaxis'] = array(
		'#type' => 'chart_yaxis',
		'#axis_type' => 'linear',
		'#title' => t('Notas'),
		'#labels_color' => 'black',
		'#title_color' => 'black',
		'#min' => 0,
		'#title_font_size' => 14,
		'#title_color' => '#8797A0',
		'#allowDecimals' => FALSE,
	);


	return drupal_render($columnas);

}
















function paleta() {
	$paleta = array('#50B0E7', '#F03899', '#40BE9D', '#C4E112', '#FFF430', '#F03850', '#F7C609', '#7050AD');
	return $paleta;
}



function query_area(&$query, $variables, $tipo = NULL) {
	if ($variables['area'] || $tipo == 'area') {
		$query->join('field_data_field_area', 'fa', 'n.nid = fa.entity_id');
		if ($tipo == 'area') {
			$query->join('taxonomy_term_data', 'ttd', 'fa.field_area_tid = ttd.tid');
		}
		if ($variables['area']) {
			$db_or_area = db_or();
			foreach ($variables['area'] as $area_tid) {
				$db_or_area->condition('fa.field_area_tid', $area_tid);	
			}
			$query->condition($db_or_area);
		}
	}
	return $query;
}



function query_palabra_clave(&$query, $variables){
	if($variables['palabra_clave']) {
		$query->join('field_data_body', 'b', 'n.nid = b.entity_id');
		if ($variables['busqueda_exacta']) {
			$db_or_be = db_or();
			$db_or_be->condition('n.title', '%' . db_like($variables['palabra_clave']) .'%', 'LIKE');
			$db_or_be->condition('b.body_value', '%' . db_like($variables['palabra_clave']) .'%', 'LIKE');
			$query->condition($db_or_be);
		}
		elseif ($variables['cualquiera']) {
			$db_or_c = db_or();
			$palabras = str_word_count($variables['palabra_clave'], 2);
			foreach ($palabras as $palabra) {
				$db_or_c->condition('n.title', '%' . db_like($palabra) . '%', 'LIKE');
				$db_or_c->condition('b.body_value', '%' . db_like($palabra) . '%', 'LIKE');
			}
			$query->condition($db_or_c);
		}
		else {
			$palabras = str_word_count($variables['palabra_clave'], 2);
			//$db_and_pc = db_and();
			foreach ($palabras as $palabra) {
				$query->where( "CONCAT(n.title, ' ', b.body_value) LIKE :$palabra", array(":$palabra" => '%' . $palabra . '%') );
			}
		}
	}
	return $query;
}

function query_medio(&$query, $variables, $tipo = NULL) {
	if (isset($variables['medio']) || $tipo == 'medio' || $variables['tipo_localizacion'] != 0) {
		$query->join('field_data_field_medio', 'fme', 'n.nid = fme.entity_id');

		if ($variables['tipo_localizacion'] != 0) {
			if ($variables['tipo_localizacion'] != -1) {
				$query->join('field_data_field_localizacion', 'fl', 'fme.field_medio_tid = fl.entity_id');
				$query->join('field_data_field_tipo_localizacion', 'ftl', 'fl.field_localizacion_tid = ftl.entity_id');
				$query->condition('ftl.field_tipo_localizacion_tid', $variables['tipo_localizacion']);
			}
		}


		if (isset($variables['medio']) || $tipo == 'medio') {
			if ($tipo == 'medio') {
				$query->join('taxonomy_term_data', 'ttd', 'fme.field_medio_tid = ttd.tid');
			}
			if (isset($variables['medio'])) {
				$db_or_medio = db_or();
				foreach ($variables['medio'] as $medio_tid) {
					$db_or_medio->condition('fme.field_medio_tid', $medio_tid);
				}
				$query->condition($db_or_medio);
			}
		}
	}
	return $query;
}

function query_discursivo(&$query, $variables, $tipo = NULL){
	if ( ($variables['tema']) || ($variables['matriz']) || ($variables['actor']) || ($tipo == 'tema') || ($tipo == 'matriz') || ($tipo == 'actor')) {
		$query->join('field_data_field_tema', 'ft', 'n.nid = ft.entity_id');
		$query->join('field_data_field_matriz', 'fm', 'n.nid = fm.entity_id AND ft.delta = fm.delta');
		$query->join('field_data_field_actor', 'fac', 'n.nid = fac.entity_id AND ft.delta = fac.delta');

		switch ($tipo) {
			case 'tema':
				$query->join('taxonomy_term_data', 'ttd', 'ft.field_tema_tid = ttd.tid');
				break;
			case 'matriz':
				$query->join('taxonomy_term_data', 'ttd', 'fm.field_matriz_tid = ttd.tid');
				break;
			case 'actor':
				$query->join('taxonomy_term_data', 'ttd', 'fac.field_actor_tid = ttd.tid');
				break;
		}

		if ($variables['tema']) {
			$db_or_tema = db_or();
			foreach ($variables['tema'] as $tema_tid) {
				$db_or_tema->condition('ft.field_tema_tid', $tema_tid);
			}
			$query->condition($db_or_tema);
		}
		if ($variables['matriz']) {
			$db_or_matriz = db_or();
			foreach ($variables['matriz'] as $matriz_tid) {
				$db_or_matriz->condition('fm.field_matriz_tid', $matriz_tid);
			}
			$query->condition($db_or_matriz);
		}
		if ($variables['actor']) {
			$db_or_actor = db_or();
			foreach ($variables['actor'] as $actor_tid) {
				$db_or_actor->condition('fac.field_actor_tid', $actor_tid);
			}
			$query->condition($db_or_actor);
		}
	}
	return $query;
}


function query_por_tipo($variables, $tipo, $rango = NULL){
	$query = db_select('node', 'n');
	$query->join('field_data_field_fecha', 'ff', 'n.nid = ff.entity_id');

	$query = query_area($query, $variables, $tipo);
	$query = query_discursivo($query, $variables, $tipo);
	$query = query_medio($query, $variables, $tipo);
	$query = query_palabra_clave($query, $variables);


	$query->addExpression('COUNT(n.nid)', 'nid_count');
	$query
		->fields('ttd', array('name', 'tid'))
		->condition('ff.entity_type', 'node')
		->groupBy('ttd.tid');
	if ($rango) {
		$query->range(0, $rango);
	}

		
	if ($variables['fecha_hasta']) {
		$query->condition('ff.field_fecha_value', $variables['fecha_desde'], '>=')
			->condition('ff.field_fecha_value', $variables['fecha_hasta'], '<=');
	}
	else {
		$query->condition('ff.field_fecha_value', $variables['fecha_desde'], '=');
	}
	$query->orderBy('nid_count', 'DESC');

	return $query;

}



?>
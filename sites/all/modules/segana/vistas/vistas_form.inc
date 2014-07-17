<?php

function vistas_forms($form_id) {
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
	$elementos_discursivos = array('area','tema','subtema','matriz','actor');
	foreach($elementos_discursivos as $elemento_discursivo){
		if (strpos($form_id, 'vistas_'. $elemento_discursivo .'_form') === 0) {
			$forms[$form_id] = array(
				'callback' => 'vistas_discursivo_form',
				'callback arguments' => array($elemento_discursivo),
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
	$elementos_discursivos = array('area','tema','subtema','matriz','actor');
	foreach($elementos_discursivos as $elemento_discursivo){
		if (strpos($form_id, 'vistas_'. $elemento_discursivo .'_form') === 0) {
			$form['#attributes'] = array('class' => 'vistas-discursivo-form');
		}
	}
}

function vistas_nota_form($form_state, $nid = NULL) {
	$form = array();
	ctools_include('dependent');

	if ( ($nid) && empty($form_state['clicked_button']['#value']) ){
		$node = node_load($nid);
		$titulo = $node->title;
		$fecha = $node->field_fecha[0]['value'];
		$cuerpo_texto = $node->body;
		$medio_term = taxonomy_get_term($node->field_medio[0]['value']);
		$medio = $medio_term->name;
		$tipo_medio = $node->field_tipo_nota[0]['value'];
		$tipo_carga_term = taxonomy_get_term($node->field_tipo_carga[0]['value']);
		$tipo_carga = $tipo_carga_term->name;
		$tipo_informacion = $node->field_tipo_informacion[0]['value'];
		$enlace = $node->field_enlace[0]['value'];
		$autor_term = taxonomy_get_term($node->field_autor[0]['value']);
		$autor = $autor_term->name;
		$ubicacion = $node->field_ubicacion[0]['value'];
		$cuerpo = $node->field_cuerpo[0]['value'];
		$pagina = $node->field_pagina[0]['value'];
		$cuadrante = $node->field_cuadrante[0]['value'];
		$centimetros = $node->field_centimetros[0]['value'];
		$columnas = $node->field_columnas[0]['value'];
		$centimetraje = $node->field_centimetraje[0]['value'];
	}
	else {
		$fecha = date('Y-m-d');
	}


	return $form;
}
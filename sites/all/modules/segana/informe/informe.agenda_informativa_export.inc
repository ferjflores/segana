<?php
use PhpOffice\PhpWord\Autoloader;
use PhpOffice\PhpWord\Settings;
use PhpOffice\PhpWord\IOFactory;

require_once 'sites/all/libraries/PHPWord/src/PhpWord/Autoloader.php';
//\PhpOffice\PhpWord\Autoloader::register();

Autoloader::register();
Settings::loadConfig();

function  informe_agenda_informativa_export($iid){

	$variables = unserialize(informe_agenda_informativa_cargar($iid));

	$phpWord = new \PhpOffice\PhpWord\PhpWord();


	$sectionStyle = array(
	    'orientation' => 'portrait',
	    'colsNum' => 1,
	    'pageSizeW' => 12240,
	    'pageSizeH' => 15840,
	    'marginTop' => 1133.858267717, //2 cm
	    'marginLeft' => 1133.858267717,
	    'marginRight' => 1133.858267717,
	    'marginBottom' => 1133.858267717,
	    'pageNumberingStart' => 1,
	);



	$paragraphLeftStyle = array('align' => 'left');
	$paragraphIndentLeftStyle = array('align' => 'left', 'indent' => 1);
	$paragraphRightStyle = array('align' => 'right');
	$paragraphJustStyle = array('align' => 'justify');
	$paragraphCenterStyle = array('align' => 'center');

	$phpWord->setDefaultFontName('Ubuntu');
	$phpWord->setDefaultFontSize(12);

	$tocFontStyle = array(
		'color' => '424a52',
		'size' => '18',
		'name' => 'Ubuntu',
		'bold' => TRUE,
		'spaceAfter' => 60,
	);

	$tocNumeroFontStyle = array(
		'color' => 'ffffff',
		'size' => '22',
		'name' => 'Ubuntu',
		'bold' => TRUE,
	);

	$tituloFontStyle = array(
		'color' => 'ffffff',
		'size' => '24',
		'name' => 'Ubuntu',
		'bold' => TRUE,
	);
	$tipoFontStyle = array(
		'color' => '848f96',
		'size' => '40',
		'name' => 'Ubuntu',
		'bold' => TRUE,
	);
	$periodoFontStyle = array(
		'color' => 'black',
		'size' => '16',
		'name' => 'Ubuntu',
		'bold' => TRUE,
	);
	$fechaFontStyle = array(
		'color' => 'c40000',
		'size' => '16',
		'name' => 'Ubuntu',
		'bold' => TRUE,
	);
	$fechaEncabezado = array(
		'color' => '424a52',
		'size' => '12.5',
		'name' => 'Ubuntu',
		'bold' => TRUE,
	);
	$tituloEncabezado = array(
		'color' => '848f96',
		'size' => '12.5',
		'name' => 'Ubuntu',
		'bold' => TRUE,
	);
	$temaFontStyle = array(
		'color' => 'a00000',
		'size' => '17.5',
		'name' => 'Ubuntu',
		'bold' => TRUE,
	);

	$tema2FontStyle = array(
		'color' => '848f96',
		'size' => '15.75',
		'name' => 'Ubuntu',
		'bold' => TRUE,
	);


	$guiaStyle = array(
		'color' => '000000',
		'size' => '22.5',
		'name' => 'Ubuntu',
		'bold' => TRUE,
	);

	$subnoticiaStyle = array(
		'color' => '616c74',
		'size' => '17.5',
		'name' => 'Ubuntu',
		'bold' => TRUE,
	);

	$medioStyle = array(
		'color' => '848f96',
		'size' => '15.75',
		'name' => 'Ubuntu',
		'bold' => TRUE,
	);

	$textoStyle = array(
		'color' => '000000',
		'size' => '12.5',
		'name' => 'Ubuntu Light',
	);

	$publicadoTambienStyle = array(
		'color' => '616c74',
		'size' => '11.25',
		'name' => 'Ubuntu',
		'bold' => TRUE,
	);

	$publicadoTambienMedioStyle = array(
		'color' => '231f20',
		'size' => '11.25',
		'name' => 'Ubuntu',
		'bold' => TRUE,
	);


	$phpWord->addTitleStyle(1, $temaFontStyle, $paragraphLeftStyle);

	$styleTable = array('borderSize' => 1,'borderBottomSize' => 1, 'borderColor' => '006699', 'cellMarginRight' => 80);
	//$styleFirstRow = array(, 'borderBottomColor' => '0000FF');
	$styleCell = array('valign' => 'center');
	$styleCellBTLR = array('valign' => 'center', 'textDirection' => \PhpOffice\PhpWord\Style\Cell::TEXT_DIR_BTLR);
	$phpWord->addTableStyle('PublicadoTambien', $styleTable, $styleFirstRow);




	/*$tocStyle = array(
		'tabPos' => 2000,
		'indent' => 4000,
	);*/

	/*$toc = $section->addTOC($textoStyle);
	$section->addPageBreak();
	$section->addTextBreak(49);*/

	$portadaStyle = array(
	    'width' => 620,
	    'height' => 882,
	    'wrappingStyle' => 'behind',
	    'positioning' => 'absolute',
	    'posHorizontalRel' => 'margin',
	    'posVerticalRel' => 'line',
	);
	$indiceStyle = array('borderSize' => 0,'borderColor' => '8a9298', "cellMargin" => 0, "cellMarginRight" => 0, "cellMarginBottom" => 0, "cellMarginLeft" => 0);


	$portada = $phpWord->createSection($sectionStyle);

	$header = $portada->addHeader();
	$header->firstPage();
	$header->addImage(drupal_get_path('module', 'informe') . '/imagenes/portada_agenda_informativa.png', $portadaStyle);

	$portada->addTextBreak(5);
	$portada->addText("Agenda Informativa", $tituloFontStyle, $paragraphCenterStyle);
	if (isset($variables['tipo'])) {
		$portada->addText(mb_strtoupper($variables['tipo']), $tipoFontStyle, $paragraphCenterStyle);
	}
	else {
		$portada->addTextBreak(1, array('size' => 40));
	}
	$portada->addTextBreak(16);

	$fecha = $portada->addTextRun($paragraphCenterStyle);
	$fecha->addText('Principales del ' . ( isset($variables['fecha_hasta']) ? 'periodo' : 'día' ) . "    ", $periodoFontStyle);

	//construir fecha
	$fecha_desde = format_date($variables['fecha_desde'], 'custom', 'd \d\e F');
	$año_desde = " de " . date('Y', $variables['fecha_desde']);

	if (isset($variables['fecha_hasta'])) {
		$año_hasta = " de " . date('Y', $variables['fecha_hasta']);

		if ($año_hasta == $año_desde) {
			$año_desde = NULL;
		}

		$fecha_hasta = " al " . format_date($variables['fecha_hasta'], 'custom', 'd \d\e F');
	}
	$fecha->addText($fecha_desde . $año_desde . $fecha_hasta . $año_hasta, $fechaFontStyle);
	$portada->addTextBreak(4);
	$indice = $portada->addTable($indiceStyle);

	foreach ($variables['temas'] as $key => $values) {
		$indice->addRow(600);
		$indice->addCell(8500, $styleCell)->addText($values['tema'], $tocFontStyle, $paragraphIndentLeftStyle);
		$indice->addCell(1000, $styleCell)->addText($values['pagina'], $tocNumeroFontStyle, $paragraphRightStyle);
	}





	$section = $phpWord->createSection($sectionStyle);

	$header_contenido = $section->addHeader();
	$imageStyle = array(
	    'width' => 70,
	    'height' => 47,
	    'wrappingStyle' => 'square',
	    'positioning' => 'absolute',
	    'posHorizontalRel' => 'margin',
	    'posVerticalRel' => 'line',
	);


	$header_contenido->addImage(drupal_get_path('module', 'informe') . '/imagenes/logo_encabezado_tematica.jpg', $imageStyle);
	$titulo_encabezado = $header_contenido->addTextRun($paragraphRightStyle);
	$titulo_encabezado->addText('Agenda Informativa ' . $variables['tipo'] .'  ',$tituloEncabezado);
	$titulo_encabezado->addText($fecha_desde . $año_desde . $fecha_hasta . $año_hasta, $fechaEncabezado);
	$header_contenido->addTextBreak();

	$footer = $section->addFooter();
	$footer->addPreserveText('| {PAGE} |', array('align' => 'center'));
	$cantidad_publicado_tambien = 0;
	foreach ($variables['notas'] as $tema => $nids) {
		if ($cantidad_publicado_tambien != 0) {
			$section->addTextBreak(3);
		}
		$section->addText('TEMA', $tema2FontStyle, $paragraphLeftStyle);
		$section->addTitle(strtoupper($tema), 1);
		$section->addTextBreak();
		foreach ($nids as $nid => $values) {
			switch ($values['depth']) {
				case 0:
					if ($cantidad_publicado_tambien != 0) {
						$section->addTextBreak(3);
					}
					$section->addText($values['title'], $guiaStyle, $paragraphLeftStyle);
					//$section->addTextBreak();
					$section->addText($values['medio'], $medioStyle, $paragraphLeftStyle);
					$section->addTextBreak();
					$section = $phpWord->addSection(array('breakType' => 'continuous', 'colsNum' => 2, 'colsSpace' => 230,));
					$section->addText($values['resumen'], $textoStyle, $paragraphJustStyle);
					$section = $phpWord->addSection(array('breakType' => 'continuous'));
					$section->addTextBreak(2);
					$cantidad_publicado_tambien = 0;
					break;
				case 1:
					if ($cantidad_publicado_tambien != 0) {
						$section->addTextBreak(3);
					}
					$section->addText($values['title'], $subnoticiaStyle, $paragraphLeftStyle);
					//$section->addTextBreak();
					$section->addText($values['medio'], $medioStyle, $paragraphLeftStyle);
					$section->addTextBreak();
					$section = $phpWord->addSection(array('breakType' => 'continuous', 'colsNum' => 2, 'colsSpace' => 230,));
					$section->addText($values['resumen'], $textoStyle, $paragraphJustStyle);
					$section = $phpWord->addSection(array('breakType' => 'continuous'));
					$section->addTextBreak(2);
					$cantidad_publicado_tambien = 0;
					break;
				case 2:
					if ($cantidad_publicado_tambien == 0) {
						$section->addText('PUBLICADO TAMBIÉN EN', $publicadoTambienStyle, $paragraphLeftStyle);
					}
					${'table' . $nid} = $section->addTable('PublicadoTambien');
					${'table' . $nid}->addRow(400);
					${'table' . $nid}->addCell(3000, $styleCell)->addText($values['medio'], $publicadoTambienMedioStyle);
					${'table' . $nid}->addCell(6600, $styleCell)->addText($values['title'], $publicadoTambienStyle);
					$section->addTextBreak(1);
					$cantidad_publicado_tambien++;
					break;
			}
		}
	}




	// para cambiar 2 columnas
	//$section = $phpWord->addSection(array('breakType' => 'continuous', 'colsNum' => 2));

	$fecha_desde = format_date($variables['fecha_desde'], 'custom', 'd \d\e F');
	$año_desde = " de " . date('Y', $variables['fecha_desde']);

	if (isset($variables['fecha_hasta'])) {
		$año_hasta = " de " . date('Y', $variables['fecha_hasta']);

		if ($año_hasta == $año_desde) {
			$año_desde = NULL;
		}

		$fecha_hasta = " al " . format_date($variables['fecha_hasta'], 'custom', 'd \d\e F');
	}

	$file = 'AgendaInformativa-' . $fecha_desde . $año_desde . $fecha_hasta . $año_hasta . '.doc';
	header("Content-Description: File Transfer");
	header('Content-Disposition: attachment; filename="' . $file . '"');
	header('Content-Type: application/vnd.openxmlformats-officedocument.wordprocessingml.document');
	header('Content-Transfer-Encoding: binary');
	header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
	header('Expires: 0');
	$xmlWriter = \PhpOffice\PhpWord\IOFactory::createWriter($phpWord, 'Word2007');
	/*print "<pre>";
	var_dump($xmlWriter);
	print "</pre>";*/
	$xmlWriter->save("php://output");
}
?>
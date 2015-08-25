<?php
use PhpOffice\PhpWord\Autoloader;
use PhpOffice\PhpWord\Settings;
use PhpOffice\PhpWord\IOFactory;

require_once 'sites/all/libraries/PHPWord/src/PhpWord/Autoloader.php';
//\PhpOffice\PhpWord\Autoloader::register();

Autoloader::register();
Settings::loadConfig();

function  informe_tematica_export($iid){
	$notas = cargar_tematica($iid, 1);
	$title = $notas['title'];
	$fecha_desde = $notas['fecha_desde'];
	$fecha_hasta = $notas['fecha_hasta'];


	$fecha_desde = format_date($notas['fecha_desde'], 'custom', 'd \d\e F');
	$año_desde = " de " . date('Y', $notas['fecha_desde']);
	if (isset($notas['fecha_hasta'])) {
		$año_hasta = " de " . date('Y', $notas['fecha_hasta']);
		if ($año_hasta == $año_desde) {
			$año_desde = NULL;
		}
		$fecha_hasta = " al " . format_date($notas['fecha_hasta'], 'custom', 'd \d\e F');
	}

	$fecha = $fecha_desde . $año_desde . (isset($fecha_hasta) ? $fecha_hasta : NULL) . (isset($año_hasta) ? $año_hasta : NULL);

	unset($notas['title']);
	unset($notas['fecha_desde']);
	unset($notas['fecha_hasta']);

	$phpWord = new \PhpOffice\PhpWord\PhpWord();
	$sectionStyle = array(
		'orientation' => 'portrait',
		'colsNum' => 1,
		'pageSizeW' => 12240,
		'pageSizeH' => 15840,
		'marginTop' => 850.393700787, //1.5 cm
		'marginLeft' => 850.393700787,
		'marginRight' => 850.393700787,
		'marginBottom' => 850.393700787,
		'pageNumberingStart' => 1,
		//'headerHeight' => 1000,
	);



	$paragraphLeftStyle = array('align' => 'left');
	$paragraphLeftPageBreakStyle = array('align' => 'left', 'pageBreakBefore' => TRUE);
	$paragraphJustStyle = array('align' => 'justify');
	$paragraphRightStyle = array('align' => 'right');
	$paragraphEncabezadoRightStyle = array('align' => 'right');

	$phpWord->setDefaultFontName('Ubuntu');
	$phpWord->setDefaultFontSize(12);

	$titulo1Portada = array(
		'color' => 'ffffff',
		'size' => '34.5',
		'name' => 'Ubuntu',
		'bold' => TRUE,
	);

	$titulo2Portada = array(
		'color' => '848f96',
		'size' => '34.5',
		'name' => 'Ubuntu',
		'bold' => TRUE,
	);

	$fechaPortada = array(
		'color' => 'ffffff',
		'size' => '17.5',
		'name' => 'Ubuntu',
		'bold' => TRUE,
	);

	$paginaIndice = array(
		'color' => '848f96',
		'size' => '20.5',
		'name' => 'Ubuntu',
		'bold' => TRUE,
	);

	$temaIndiceStyle = array(
		'color' => 'black',
		'size' => '14',
		'name' => 'Ubuntu',
		'bold' => TRUE,
	);

	$notaIndiceStyle = array(
		'color' => 'black',
		'size' => '11',
		'name' => 'Ubuntu',
		'bold' => FALSE,
	);

	$tituloEncabezado = array(
		'color' => '848f96',
		'size' => '12.5',
		'name' => 'Ubuntu',
		'bold' => TRUE,
	);

	$fechaEncabezado = array(
		'color' => '424a52',
		'size' => '12.5',
		'name' => 'Ubuntu',
		'bold' => TRUE,
	);
	$tema_areaFontStyle = array(
		'color' => '848f96',
		'size' => '15.75',
		'name' => 'Ubuntu',
		'bold' => TRUE,
	);

	$tema_area2FontStyle = array(
		'color' => '424a52',
		'size' => '13.5',
		'name' => 'Ubuntu',
		'bold' => TRUE,
	);

	$tituloFontStyle = array(
		'color' => 'black',
		'size' => '20',
		'name' => 'Ubuntu',
		'bold' => TRUE,
	);

	$contenidoFontStyle = array(
		'color' => 'black',
		'size' => '11',
		'name' => 'Ubuntu',
		'bold' => FALSE,
	);
	$autorFontStyle = array(
		'color' => '424a52',
		'size' => '11.25',
		'name' => 'Ubuntu',
		'bold' => TRUE,
	);
	$medioFontStyle = array(
		'color' => '231f20',
		'size' => '11.25',
		'name' => 'Ubuntu',
		'bold' => TRUE,
	);
	$pagina_webFontStyle = array(
		'color' => '424a52',
		'size' => '11.25',
		'name' => 'Ubuntu',
		'bold' => TRUE,
		'italic' => TRUE,
	);

	$portadaStyle = array(
		'width' => 660,
		'height' => 882,
		'wrappingStyle' => 'behind',
		'positioning' => 'absolute',
		'posHorizontalRel' => 'margin',
		'posVerticalRel' => 'line',
	);
	$styleCell = array('valign' => 'center');

	$portada = $phpWord->createSection($sectionStyle);

	$header = $portada->addHeader();
	$header->firstPage();
	$header->addImage(drupal_get_path('module', 'informe') . '/imagenes/portada_tematica.png', $portadaStyle);
	$header_indice = $portada->addHeader();
	$header_indice->addImage(drupal_get_path('module', 'informe') . '/imagenes/portada_tematica2.png', $portadaStyle);

	$indiceStyle = array('borderSize' => 0,'borderColor' => 'e6e9eb', "cellMargin" => 0, "cellMarginRight" => 0, "cellMarginBottom" => 0, "cellMarginLeft" => 0);

	$palabras_titulo = array_values(preg_split('/\s+/', $title));
	$cantidad_palabras_titulo = count($palabras_titulo);
	ed($palabras_titulo);
	if ( $cantidad_palabras_titulo > 1 ) {
		$cantidad_palabras_titulo_1 = ceil($cantidad_palabras_titulo/2);
		$title1 = '';
		$title2 = '';
		for ($i=0; $i < $cantidad_palabras_titulo_1; $i++) {
			$title1 .=  $palabras_titulo[$i];
		}

		for ($i=$cantidad_palabras_titulo_1; $i < $cantidad_palabras_titulo; $i++) {
			$title2 .=  $palabras_titulo[$i];
		}

	}
	else {
		$title1 = $title;
	}

	$portada->addText($title1 .'  ', $titulo1Portada, $paragraphRightStyle);
	if (isset($title2)) {
		$portada->addText($title2 .'  ', $titulo2Portada, $paragraphRightStyle);
	}
	else {
		$portada->addTextBreak(1, $titulo2Portada);
	}
	$portada->addTextBreak(1, array('size' => 22));
	$portada->addText($fecha . '   ', $fechaPortada, $paragraphRightStyle);
	$portada->addTextBreak(3);

	$portada = $phpWord->addSection(array('breakType' => 'continuous', 'colsNum' => 2, 'colsSpace' => 200));

	$page = 1;
	foreach ($notas as $tid => $values) {
		${'indice_' . $tid} = $portada->addTable($indiceStyle);
		${'indice_' . $tid}->addRow(600);
		${'indice_' . $tid}->addCell(1000, $styleCell)->addText(str_pad($page, 2, '0', STR_PAD_LEFT), $paginaIndice);
		${'indice_' . $tid}->addCell(4000, $styleCell)->addText($values['nombre'], $temaIndiceStyle);
		foreach ($values['notas'] as $nid => $values_nota) {
			${'indice_' . $tid}->addRow(400);
			${'indice_' . $tid}->addCell(1000, $styleCell)->addText(' ', $paginaIndice);
			$titulo_nota = ${'indice_' . $tid}->addCell(4000, $styleCell);
			$titulo_nota->addText(htmlspecialchars($values_nota['title']), $notaIndiceStyle);
			$titulo_nota->addTextBreak(1, array('size' => 6));
			$page++;
			if (strlen($values_nota['body']) > 3000) {
				$page++;
			}
		}
		$portada->addTextBreak();
	}

	$portada = $phpWord->addSection(array('breakType' => 'continuous'));


	/*$indice = $portada->addTable('Indice');
	$indice->addRow(400);
	$indice->addCell(3000, $styleCell)->addText('pruebaaaa', $publicadoTambienMedioStyle);
	$indice->addCell(7600, $styleCell)->addText($values['title'], $publicadoTambienStyle);*/

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
	$titulo_encabezado = $header_contenido->addTextRun($paragraphEncabezadoRightStyle);
	$titulo_encabezado->addText($title .'  ',$tituloEncabezado);
	$titulo_encabezado->addText($fecha, $fechaEncabezado);
	$header_contenido->addTextBreak();

	$footer = $section->addFooter();
	$footer->addPreserveText('| {PAGE} |', array('align' => 'right'));

	foreach ($notas as $tid => $values) {
		foreach ($values['notas'] as $nid => $values_nota) {
			//$section->addImage(drupal_get_path('module', 'informe') . '/imagenes/tematica_contenido_bg.png', array('marginTop' => 200, 'marginLeft' => 55));
			$area = $section->addTextRun($paragraphLeftPageBreakStyle);
			$area->addText('AREA | ', $tema_areaFontStyle);
			$area->addText(mb_strtoupper($values_nota['area']),$tema_area2FontStyle);
			$tema = $section->addTextRun();
			$tema->addText('TEMA | ', $tema_areaFontStyle);
			$tema->addText(mb_strtoupper($values['nombre']),$tema_area2FontStyle);
			$section->addTextBreak(2);

			$section->addText($values_nota['title'],$tituloFontStyle);
			//$section->addText(mb_strtoupper($values_nota['autor']), $autorFontStyle, $paragraphLeftStyle);
			$section->addText(mb_strtoupper($values_nota['medio']), $medioFontStyle, $paragraphLeftStyle);
			//$section->addText(strtolower($values_nota['pagina_web']), $pagina_webFontStyle, $paragraphLeftStyle);
			$section->addTextBreak(1);
			//$section = $phpWord->addSection(array('breakType' => 'continuous', 'colsNum' => 2, 'colsSpace' => 300,));
			$section->addText($values_nota['body'], $contenidoFontStyle);//, $paragraphJustStyle);
			//$section = $phpWord->addSection(array('breakType' => 'continuous'));
		}

	}
	$file = 'Temática ' . $title . '.docx';
	header("Content-Description: File Transfer");
	header('Content-Disposition: attachment; filename="' . $file . '"');
	header('Content-Type: application/vnd.openxmlformats-officedocument.wordprocessingml.document');
	header('Content-Transfer-Encoding: binary');
	header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
	header('Expires: 0');
	$xmlWriter = \PhpOffice\PhpWord\IOFactory::createWriter($phpWord, 'Word2007');
	$xmlWriter->save("php://output");
}
<?php
return array(
	'grabber' => array(
		'%.*.xml%' => array(
			'test_url' => 'http://www.2001.com.ve/logica/interna/con-la-gente_avance-izquierda_407.xml',
			'body' => array(
				'//*',
			),
			'strip' => array(
				'//script'
			),
		),
		'%.*%' => array(
			'test_url' => 'http://www.2001.com.ve/mundo-loco/99044/record-mundial--ruso-arrastra-una-casa-de-madera-de-30-toneladas---video-.html',
			'body' => array(
				'//table[@id="noti_completas"]',
			),
			'strip' => array(
				'//script'
			),
		),
	)
);
<?php
return array(
	'grabber' => array(
		'%^/noticias/.*/.*%' => array(
			'test_url' => 'http://www.ultimasnoticias.com.ve/actualidad/politica.aspx',
			'body' => array(
				'//div[@id="detalle_nota_colleft"]/div[1]',
				'//span[@id="InfoNoticia"]',
			),
			'strip' => array(
				'//script'
			),
		),
		'%.*/.*.aspx%' => array(
			'test_url' => '',
			'body' => array(
				'//div[@id="contenedor_default_izq"]',
			),
			'strip' => array(
				'//script'
			),
		),
	)
);
<?php
return array(
	'grabber' => array(
		'%.*%' => array(
			'test_url' => 'http://www.elmundo.com.ve/Noticias/Estilo-de-Vida/Musica/Lista---Nominados-de-los-Premios-Billboard-de-esta.aspx',
			'body' => array(
				'//div[@id="Nota"]',
			),
			'strip' => array(
				'//script'
			),
		)
	)
);
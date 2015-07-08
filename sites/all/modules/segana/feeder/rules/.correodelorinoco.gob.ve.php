<?php
return array(
	'grabber' => array(
		'%^/categoria/.*%' => array(
			'test_url' => 'http://www.correodelorinoco.gob.ve/categoria/categorias/vivienda-categorias/',
			'body' => array(
				'//div[@id="archive"]',
			),
			'strip' => array(
				'//script'
			),
		),
		'%.*%' => array(
			'test_url' => 'http://www.correodelorinoco.gob.ve/curiosidades/fotos-futura-alcaldesa-barcelona-ada-colau-que-causan-revuelo-red/',
			'body' => array(
				'//div[@class="post"]',
			),
			'strip' => array(
				'//script'
			),
		),
	)
);
<?php
return array(
	'grabber' => array(
		'%.*%' => array(
			'test_url' => 'http://www.panorama.com.ve/ciudad/Berna-Iskandar--Ningun-nino--se-malcria-por-exceso-de-amor-20150511-0010.html',
			'body' => array(
				'//span[@class="nav"]',
				'//article[@class="nota-main"]',
			),
			'strip' => array(
				'//script'
			),
		)
	)
);
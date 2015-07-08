<?php
return array(
	'grabber' => array(
		'%^/.*/.*.html%' => array(
			'test_url' => 'http://www.el-nacional.com/regiones/amanecio-Higuerote-protestas-falta-luz_0_632336787.html',
			'body' => array(
				'//strong[@class="title"]',
				'//div[@class="gdu u3-5-n first"]',
			),
			'strip' => array(
				'//script'
			),
		),
		'%^/.*/%' => array(
			'test_url' => 'http://www.el-nacional.com/economia/',
			'body' => array(
				'//div[@class="gdu u3-5-n first"]',
			),
			'strip' => array(
				'//script'
			),
		),
	)
);
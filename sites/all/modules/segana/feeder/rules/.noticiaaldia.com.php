<?php
return array(
	'grabber' => array(
		'"%.*%' => array(
			'test_url' => '',
			'body' => array(
				'//',
			),
			'strip' => array(
				'//script'
			),
		),
		'%/.*/%' => array(
			'test_url' => 'http://noticiaaldia.com/',
			'body' => array(
				'//html',
			),
			'strip' => array(
				'//script'
			),
		),
	)
);
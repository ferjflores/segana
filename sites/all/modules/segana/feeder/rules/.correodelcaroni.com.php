<?php
return array(
	'grabber' => array(
		'%.*%' => array(
			'test_url' => 'http://www.correodelcaroni.com/index.php/opinion/item/32101-es-contigo-nicolas',
			'body' => array(
				'//div[@id="k2Container"]',
			),
			'strip' => array(
				'//script'
			),
		),
	)
);
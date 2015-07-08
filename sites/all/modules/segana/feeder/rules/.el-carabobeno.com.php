<?php
return array(
	'grabber' => array(
		'%^/.*/articulo/.*%' => array(
			'test_url' => 'http://www.el-carabobeno.com/',
			'body' => array(
				'//ul[@class="tags"]',
				'//div[@class="article"]',
			),
			'strip' => array(
				'//script'
			),
		),
		'%.*%' => array(
			'test_url' => '',
			'body' => array(
				'//div[@id="articles"]',
				'//dl[@class="community-articles"][2]',
			),
			'strip' => array(
				'//script'
			),
		),
	)
);
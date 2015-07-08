<?php
return array(
	'grabber' => array(
		'%.*%' => array(
			'test_url' => 'http://eluniversal.com/caracas/150519/suministro-de-agua-cayo-en-3000-litros-por-segundo',
			'body' => array(
				'//div[@id="cont_cen1"]/div[@class="pod_izq_gl"]',
			),
			'strip' => array(
				'//script'
			),
		)
	)
);
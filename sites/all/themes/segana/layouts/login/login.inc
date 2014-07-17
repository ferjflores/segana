<?php

/**
 * implementation of hook_panels_layouts()
 */
// Plugin definition
$plugin = array(
  'title' => t('Pagina de acceso'),
  'category' => t('segana'),
  'icon' => 'login.png',
  'theme' => 'login',
  'css' => 'login.css',
  'regions' => array('inicio' => t('Inicio')),
);

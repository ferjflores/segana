<?php

/**
 * Informe view callback.
 */
function informe_view($informe) {
  drupal_set_title(entity_label('informe', $informe));
  return entity_view('informe', array(entity_id('informe', $informe) => $informe), 'full');
}

#!/usr/bin/drush -r /data/segana7
<?php
class Logging {
    // declare log file and file pointer as private properties
    private $log_file, $fp;
    // set log file (path and name)
    public function lfile($path) {
        $this->log_file = $path;
    }
    // write message to the log file
    public function lwrite($message) {
        // if file pointer doesn't exist, then open log file
        if (!is_resource($this->fp)) {
            $this->lopen();
        }
        // define script name
        $script_name = pathinfo($_SERVER['PHP_SELF'], PATHINFO_FILENAME);
        // define current time and suppress E_WARNING if using the system TZ settings
        // (don't forget to set the INI setting date.timezone)
        $time = @date('[d/M/Y:H:i:s]');
        // write current time, script name and message to the log file
        fwrite($this->fp, "$time ($script_name) $message" . PHP_EOL);
    }
    // close log file (it's always a good idea to close a file when you're done with it)
    public function lclose() {
        fclose($this->fp);
    }
    // open log file (private method)
    private function lopen() {
        // in case of Windows set default log file
        if (strtoupper(substr(PHP_OS, 0, 3)) === 'WIN') {
            $log_file_default = 'c:/php/logfile.txt';
        }
        // set default log file for Linux and other systems
        else {
            $log_file_default = '/tmp/logfile.txt';
        }
        // define log file from lfile method or use previously set default
        $lfile = $this->log_file ? $this->log_file : $log_file_default;
        // open log file for writing only and place file pointer at the end of the file
        // (if the file does not exist, try to create it)
        $this->fp = fopen($lfile, 'a') or exit("Can't open $lfile!");
    }
}

// Logging class initialization
$log = new Logging();
 
// set path and name of log file (optional)
$log->lfile('/tmp/mylog.txt');
 
// write message to the log file
$log->lwrite(date('M d H:i:s') . ' Inicio de importación');
 
//require('/usr/src/segana7/scripts/funciones.php');
require('/data/segana7/scripts/funciones.php');
$segana_viejo = new mysqli('udun', 'fflores', 'cbc1560', 'segana');
//$limit = "limit 10";

$elementos = (drush_get_option('elementos')) ? drush_get_option('elementos') : 'todo';


/*$vocabulary = taxonomy_get_vocabularies();
foreach ($vocabulary as $item) {
	drush_print($item->vid ."  ". $item->name);
	drush_print();
}*/

if ($elementos == "basica" || $elementos == "todo") {
	require('/data/segana7/scripts/estructura_constante.inc');
	require('/data/segana7/scripts/estructura_variable.inc');	
}


///////////actor//////////
//////////////////////////////////

$tipo ='actor';
if ($elementos == "actor" || $elementos == "todo") {

	$query = db_select('field_data_field_seganaid', 'fs');
	$query->join('taxonomy_term_data', 'ttd', 'fs.entity_id = ttd.tid');
	$query->join('field_data_field_fecha_actualizacion', 'ffa', 'fs.entity_id = ffa.entity_id');
	$query
		->condition('fs.bundle', $tipo, '=')
		->fields('fs', array('field_seganaid_value'))
		->fields('ttd', array('tid', 'name', 'description'))
		->fields('ffa', array('field_fecha_actualizacion_value'));
	$resultado = $query->execute();
	$count_query = $resultado->rowCount();
	$array = array();
	$array_term = array();
	if ($count_query > 0) {
		foreach ($resultado as $row) {
			$seganaid = $row->field_seganaid_value;
			$name = $row->name;
			$tid = $row->tid;
			$description = $row->description;
			$fecha_actualizacion = $row->field_fecha_actualizacion_value;
			$array[$seganaid] = $name.$description.$fecha_actualizacion;
			$array_term[$seganaid] = $tid;
		}
	}

	$query_viejo = "SELECT * FROM Actor ORDER BY id_actor ASC $limit";
	$resultado_viejo = $segana_viejo->query($query_viejo);
	$array_viejo = array();
	while ($row = $resultado_viejo->fetch_object()) {
		$seganaid = $row->id_actor;
		$actor = fixUTF8(forceUTF8(trim($row->actor)));
		$fecha = strtotime($row->fecha_creacion);
		$description = fixUTF8(forceUTF8(trim($row->caracterizacion)));
		$array_viejo[] = $seganaid;
		drush_print($seganaid ."  Tipo: ". $tipo ."   ". $actor);
		if (!array_key_exists($seganaid, $array)) {
			$term = new stdClass();
			$vid = 9;
			$term->name = $actor;
			if (strlen($description) > 0){
				$term->description = $description;
			}
			$term->vid = $vid;
			$term->field_seganaid[LANGUAGE_NONE][0][value]  = $seganaid;
			$term->field_fecha[LANGUAGE_NONE][0][value] = $fecha;
			$term->field_fecha_actualizacion[LANGUAGE_NONE][0][value] = $fecha;
			$term->field_user[LANGUAGE_NONE][0][target_id] = 1;
			$term->field_user[LANGUAGE_NONE][0][target_type] = 'user';
			taxonomy_term_save($term);
			drush_print($term->tid ." ". $term->name . "             ". $term->description);
		}
		else {
			if ($array[$seganaid] != $actor.$description.$fecha){
				$term = taxonomy_term_load($array_term[$seganaid]);
				$term->name = $actor;
				if (strlen($description) > 0){
					$term->description = $description;
				}
				$term->field_fecha_actualizacion[LANGUAGE_NONE][0][value] = $fecha;
				taxonomy_term_save($term);
				drush_print($term->tid ." ". $term->name . "       ACTUALIZADO");
			}
		}
		drush_print();
	}
	///borrar
	$borrados = array_diff(array_keys($array_term), $array_viejo);
	foreach ($borrados as $seganaid) {
		$tid = $array_term[$seganaid];
		$nombre = $array[$seganaid];
		taxonomy_term_delete($tid);
		drush_print("borrado ". $tid ."  ". $nombre);
		drush_print();
	}
	

}


///////////area//////////
//////////////////////////////////
if ($elementos == "area" || $elementos == "todo") {

	$tipo ='area';
	$query = db_select('field_data_field_seganaid', 'fs');
	$query->join('taxonomy_term_data', 'ttd', 'fs.entity_id = ttd.tid');
	$query->join('field_data_field_fecha_actualizacion', 'ffa', 'fs.entity_id = ffa.entity_id');
	$query
		->condition('fs.bundle', $tipo, '=')
		->fields('fs', array('field_seganaid_value'))
		->fields('ttd', array('tid', 'name', 'description'))
		->fields('ffa', array('field_fecha_actualizacion_value'));
	$resultado = $query->execute();
	$count_query = $resultado->rowCount();
	$array = array();
	$array_term = array();
	if ($count_query > 0) {
		foreach ($resultado as $row) {
			$seganaid = $row->field_seganaid_value;
			$name = $row->name;
			$tid = $row->tid;
			$fecha_actualizacion = $row->field_fecha_actualizacion_value;
			$array[$seganaid] = $name.$fecha_actualizacion;
			$array_term[$seganaid] = $tid;
		}
	}

	$query_viejo = "SELECT * FROM Area ORDER BY id_area ASC $limit";
	$resultado_viejo = $segana_viejo->query($query_viejo);
	$array_viejo = array();
	while ($row = $resultado_viejo->fetch_object()) {
		$seganaid = $row->id_area;
		$area = fixUTF8(forceUTF8(trim($row->nombre)));
		$fecha = time();
		$array_viejo[] = $seganaid;
		drush_print($seganaid ."  Tipo: ". $tipo ."   ". $area);
		drush_print();
		if (!array_key_exists($seganaid, $array)) {
			$term = new stdClass();
			$vid = 10;
			$term->name = $area;
			$term->vid = $vid;
			$term->field_seganaid[LANGUAGE_NONE][0][value]  = $seganaid;
			$term->field_fecha[LANGUAGE_NONE][0][value] = $fecha;
			$term->field_fecha_actualizacion[LANGUAGE_NONE][0][value] = $fecha;
			$term->field_user[LANGUAGE_NONE][0][target_id] = 1;
			$term->field_user[LANGUAGE_NONE][0][target_type] = 'user';
			taxonomy_term_save($term);
			drush_print($term->tid ." ". $term->name . "             ");
		}
		else {
			if ($array[$seganaid] != $area.$fecha_actualizacion){
				$term = taxonomy_term_load($array_term[$seganaid]);
				$term->name = $area;
				$term->field_fecha_actualizacion[LANGUAGE_NONE][0][value] = $fecha;
				taxonomy_term_save($term);
				drush_print($term->tid ." ". $term->name . "       ACTUALIZADO");
			}
		}
		drush_print();
	}
	///borrar
	$borrados = array_diff(array_keys($array_term), $array_viejo);
	foreach ($borrados as $seganaid) {
		$tid = $array_term[$seganaid];
		$nombre = $array[$seganaid];
		taxonomy_term_delete($tid);
		drush_print("borrado ". $tid ."  ". $nombre);
		drush_print();
	}
}


///////////autor//////////
//////////////////////////////////
/*if ($elementos == "autor" || $elementos == "todo") {

	$tipo ='autor';

	$query = db_select('field_data_field_seganaid', 'fs');
	$query->join('taxonomy_term_data', 'ttd', 'fs.entity_id = ttd.tid');
	$query
		->condition('fs.bundle', $tipo, '=')
		->fields('fs', array('field_seganaid_value'))
		->fields('ttd', array('tid', 'name', 'description'));
	$resultado = $query->execute();
	$count_query = $resultado->rowCount();
	$array = array();
	$array_term = array();
	if ($count_query > 0) {
		foreach ($resultado as $row) {
			$seganaid = $row->field_seganaid_value;
			$name = $row->name;
			$tid = $row->tid;
			$array[$seganaid] = $name;
			$array_term[$seganaid] = $tid;
		}
	}


	$query_viejo = "SELECT id_nota,autor2 from (SELECT id_nota,TRIM(TRAILING ')' FROM TRIM(LEADING '(' FROM TRIM(autor))) AS autor2, LOWER(TRIM(TRAILING ')' FROM TRIM(LEADING '(' FROM TRIM(autor)))) AS autor_lower FROM Notas GROUP BY autor_lower ORDER BY id_nota) AS T1 GROUP BY autor2 ORDER BY id_nota ASC $limit";
	$resultado_viejo = $segana_viejo->query($query_viejo);
	$array_viejo = array();
	while ($row = $resultado_viejo->fetch_object()) {
		$seganaid = $row->id_nota;
		$autor = preg_replace(array('/\s\s+/','/^\s+/','/\\r/'),array(' ',' ',' '), $row->autor2);
		$autor = fixUTF8(forceUTF8($autor));
		$autor = iconv(mb_detect_encoding($autor, mb_detect_order(), true), "UTF-8", $autor);
		$autor = str_ireplace('negativa', '', $autor);
		$autor = str_ireplace('positiva', '', $autor);
		if (strlen($autor) < 2) {
			continue;
		}
		drush_print($seganaid." sin limpiar ".$autor);
		$limpio = 0;
		while ($limpio == 0) {
			$autor_sa = sin_acentos($autor);
			$primer_caracter = $autor_sa[0];
			$ultimo_caracter = $autor_sa[strlen($autor_sa)-1];
			drush_print("primer:".$primer_caracter."|ultimo:".$ultimo_caracter."|");
			if (ctype_alnum_portable($primer_caracter) && ctype_alnum_portable($ultimo_caracter)) {
				$limpio = 1;
				drush_print("limpio ".$autor);
			}
			else{
				if (!ctype_alnum_portable($primer_caracter)) {
					drush_print($primer_caracter." de primero");
					$autor = substr($autor, 1);
				}
				if (!ctype_alnum_portable($ultimo_caracter)) {
					drush_print($ultimo_caracter." de ultimo");
					$autor = substr($autor, 0, strlen($autor)-1);
				}
			}
		}
		if (strlen($autor) < 2) {
			continue;
		}
		drush_print($seganaid ."  Tipo: ". $tipo ."   ". $autor ."|");

		////verificar si el autor existe/////
		$query = db_select('taxonomy_term_data', 'ttd')
			->condition(db_and()->condition('ttd.vid', 8, '=')->condition('ttd.name', $autor, '='))
			->fields('ttd',array('tid'));
		$resultado = $query->execute();
		$count_query = $resultado->rowCount();

		if ( (!array_key_exists($seganaid, $array)) && ($count_query == 0) ) {
			$term = new stdClass();
			$vid = 8;
			$term->name = $autor;  
			$term->vid = $vid;
			$term->field_seganaid[LANGUAGE_NONE][0][value]  = $seganaid; 
			taxonomy_term_save($term);
			drush_print($term->tid ." ". $term->name);
		}
		drush_print();
	}
}
*/

///////////matriz//////////
//////////////////////////////////
if ($elementos == "matriz" || $elementos == "todo") {
	$tipo ='matriz';

	$query = db_select('field_data_field_seganaid', 'fs');
	$query->join('taxonomy_term_data', 'ttd', 'fs.entity_id = ttd.tid');
	$query->join('field_data_field_fecha_actualizacion', 'ffa', 'fs.entity_id = ffa.entity_id');
	$query
		->condition('fs.bundle', $tipo, '=')
		->fields('fs', array('field_seganaid_value'))
		->fields('ttd', array('tid', 'name', 'description'))
		->fields('ffa', array('field_fecha_actualizacion_value'));
	$resultado = $query->execute();
	$count_query = $resultado->rowCount();
	$array = array();
	$array_term = array();
	if ($count_query > 0) {
		foreach ($resultado as $row) {
			$seganaid = $row->field_seganaid_value;
			$name = $row->name;
			$tid = $row->tid;
			$description = $row->description;
			$fecha_actualizacion = $row->field_fecha_actualizacion_value;
			$array[$seganaid] = $name.$description.$fecha_actualizacion;
			$array_term[$seganaid] = $tid;
		}
	}



	$query_viejo = "SELECT * FROM Argumento ORDER BY id_argumento ASC $limit";
	$resultado_viejo = $segana_viejo->query($query_viejo);
	$array_viejo = array();
	while ($row = $resultado_viejo->fetch_object()) {
		$seganaid = $row->id_argumento;
		$matriz = fixUTF8(forceUTF8(trim($row->argumento)));
		$description = fixUTF8(forceUTF8(trim($row->caracterizacion)));
		$fecha = strtotime($row->fecha_creacion);
		$array_viejo[] = $seganaid;
		drush_print($seganaid ."  Tipo: ". $tipo ."   ". $matriz);
		if (!array_key_exists($seganaid, $array)) {
			$term = new stdClass();
			$vid = 14;
			$term->name = $matriz;
			if (strlen($description) > 0){
				$term->description = $description;
			}
			$term->vid = $vid;
			$term->field_seganaid[LANGUAGE_NONE][0][value]  = $seganaid;
			$term->field_fecha[LANGUAGE_NONE][0][value] = $fecha;
			$term->field_fecha_actualizacion[LANGUAGE_NONE][0][value] = $fecha;
			$term->field_user[LANGUAGE_NONE][0][target_id] = 1;
			$term->field_user[LANGUAGE_NONE][0][target_type] = 'user';
			taxonomy_term_save($term);
			drush_print($term->tid ." ". $term->name . "             ". $term->description);
		}
		else {
			if ($array[$seganaid] != $matriz.$description.$fecha){
				$term = taxonomy_term_load($array_term[$seganaid]);
				$term->name = $matriz;
				if (strlen($description) > 0){
					$term->description = $description;
				}
				taxonomy_term_save($term);
				drush_print($term->tid ." ". $term->name . "       ACTUALIZADO");
			}
		}
		drush_print();
	}
	///borrar
	$borrados = array_diff(array_keys($array_term), $array_viejo);
	foreach ($borrados as $seganaid) {
		$tid = $array_term[$seganaid];
		$nombre = $array[$seganaid];
		taxonomy_term_delete($tid);
		drush_print("borrado ". $tid ."  ". $nombre);
		drush_print();
	}
}



///////////tema//////////
//////////////////////////////////
if ($elementos == "tema" || $elementos == "todo") {

	$tipo ='tema';


	$query = db_select('field_data_field_seganaid', 'fs');
	$query->join('taxonomy_term_data', 'ttd', 'fs.entity_id = ttd.tid');
	$query->join('field_data_field_fecha_actualizacion', 'ffa', 'fs.entity_id = ffa.entity_id');
	$query
		->condition('fs.bundle', $tipo, '=')
		->fields('fs', array('field_seganaid_value'))
		->fields('ttd', array('tid', 'name', 'description'))
		->fields('ffa', array('field_fecha_actualizacion_value'));
	$resultado = $query->execute();
	$count_query = $resultado->rowCount();
	$array = array();
	$array_term = array();
	if ($count_query > 0) {
		foreach ($resultado as $row) {
			$seganaid = $row->field_seganaid_value;
			$name = $row->name;
			$tid = $row->tid;
			$description = $row->description;
			$fecha_actualizacion = $row->field_fecha_actualizacion_value;
			$array[$seganaid] = $name.$description.$fecha_actualizacion;
			$array_term[$seganaid] = $tid;
		}
	}

	$query_viejo = "SELECT * FROM Tema ORDER BY id_tema ASC $limit";
	$resultado_viejo = $segana_viejo->query($query_viejo);
	$array_viejo = array();
	while ($row = $resultado_viejo->fetch_object()) {
		$seganaid = $row->id_tema;
		$tema = fixUTF8(forceUTF8(trim($row->nombre)));
		$description = fixUTF8(forceUTF8(trim($row->caracterizacion)));
		$fecha = strtotime($row->fecha_creacion);
		$array_viejo[] = $seganaid;
		drush_print($seganaid ."  Tipo: ". $tipo ."   ". $row->nombre);
		if (!array_key_exists($seganaid, $array)) {
			$term = new stdClass();
			$vid = 15;
			$term->name = $tema;
			if (strlen($description) > 0){
				$term->description = $description;
			}
			$term->vid = $vid;
			$term->field_seganaid[LANGUAGE_NONE][0][value]  = $seganaid;
			$term->field_fecha[LANGUAGE_NONE][0][value] = $fecha;
			$term->field_fecha_actualizacion[LANGUAGE_NONE][0][value] = $fecha;
			$term->field_user[LANGUAGE_NONE][0][target_id] = 1;
			$term->field_user[LANGUAGE_NONE][0][target_type] = 'user';
			taxonomy_term_save($term);
			drush_print($term->tid ." ". $term->name . "             ". $term->description);
		}
		else {
			if ($array[$seganaid] != $tema.$description.$fecha){
				$term = taxonomy_term_load($array_term[$seganaid]);
				$term->name = $tema;
				if (strlen($description) > 0){
					$term->description = $description;
				}
				taxonomy_term_save($term);
				drush_print($term->tid ." ". $term->name . "       ACTUALIZADO");
			}
		}
		drush_print();
	}
	///borrar
	$borrados = array_diff(array_keys($array_term), $array_viejo);
	foreach ($borrados as $seganaid) {
		$tid = $array_term[$seganaid];
		$nombre = $array[$seganaid];
		taxonomy_term_delete($tid);
		drush_print("borrado ". $tid ."  ". $nombre);
		drush_print();
	}
}




///////////user//////////
//////////////////////////////////
if ($elementos == "users" || $elementos == "todo") {

	$tipo = "users";
	$users = entity_load('user');
	$user_names = array();
	foreach ($users as $user_id => $user) {
	  $user_names[] = $user->name;
	}

	$query = "SELECT id_usuario, usuario FROM Usuario WHERE clave <> MD5('usuario eliminado') ORDER BY id_usuario ASC $limit";
	$resultado = $segana_viejo->query($query);
	$usuarios_viejo = array();

	while ($row = $resultado->fetch_object()) {
		$seganaid = $row->id_usuario;
		$usuario = $row->usuario;

		////busqueda ldap///////
		$ds=ldap_connect("ldap");
		if ($ds) {
			$r=ldap_bind($ds);
			$sr=ldap_search($ds, "ou=usuario,dc=abn,dc=info,dc=ve", "uid=$usuario");
			$info = ldap_first_entry($ds, $sr);
			
		}
		if (!empty($info)){
			$info2 = ldap_get_attributes($ds, $info);
			$nombre = $info2[cn][0];
			$mail = $info2[mail][0];
			drush_print($seganaid ."  Tipo: ". $tipo ."   ". $usuario. "       ". $nombre);
			drush_print();	
			if (!in_array($usuario, $user_names)) {

				$roles = array(4 => TRUE, 3 => TRUE);
				$new_user = array(
			      'name' => $usuario,
			      'pass' => user_password(),
			      'mail' => $mail,
			      'init' => $mail,
			      'field_nombre' => array(LANGUAGE_NONE => array(array('value' => $nombre))),
			      'field_seganaid' => array(LANGUAGE_NONE => array(array('value' => $seganaid))),
			      'status' => 1,
			      'access' => REQUEST_TIME,
			      'roles' => $roles,
				);
				// $account returns user object
				$account = user_save(null, $new_user);

				drush_print($new_user->uid ." ". $new_user->name . "             ". $new_user->field_nombre);
				drush_print();
			}
		}
		else {
			drush_print($seganaid ."  Tipo: ". $tipo ."   ". $usuario. "   USUARIO BORRADO");
			drush_print();
		}
		
	}
}

///////////nota//////////
//////////////////////////////////
if ($elementos == "nota" || $elementos == "todo") {

	if (drush_get_option('solodia')) {
		$fecha = date('Y-m-d');
		$where_query_viejo = "WHERE fecha = '$fecha'";
	}
	elseif (drush_get_option('nota')) {
		drush_print(drush_get_option('nota'));
		$where_query_viejo = "WHERE id_nota= " . drush_get_option('nota');
	}
	elseif (drush_get_option('apartir')) {
		drush_print(drush_get_option('apartir'));
		$where_query_viejo = "WHERE id_nota > " . drush_get_option('apartir');
	}
	else{
		//buscar maximo seganaid
		$query_max_seganaid = db_select('field_data_field_seganaid', 'fs');
		$query_max_seganaid->addExpression('MAX(fs.field_seganaid_value)');
		$resultado = $query_max_seganaid->execute();
		$seganaid_inicio = "WHERE id_nota >= " . ($resultado->fetchField($column_index) - 15000);
		//$seganaid_inicio = "WHERE id_nota = 959276";
	}

	$inicio = microtime(TRUE);
	//$limit = 'limit 864000, 1';
	$ano = '2011';
	//$where_query_viejo = "WHERE fecha LIKE '$ano%'";
	//$where_nota ="WHERE id_nota=29159";
	$tipo ='nota';

	$query = db_select('node', 'node');
	$query->join('field_data_field_seganaid', 'fs', 'fs.entity_id = node.nid');
	$query->join('field_data_body', 'body', 'node.nid = body.entity_id');
	$query
		->condition('fs.bundle', $tipo, '=')
		->fields('fs', array('field_seganaid_value'))
		->fields('node', array('nid', 'title'))
		->fields('body', array('body_value'));
	$resultado = $query->execute();
	$count_query = $resultado->rowCount();
	$array = array();
	$array_term = array();
	if ($count_query > 0) {
		foreach ($resultado as $row) {
			$seganaid = $row->field_seganaid_value;
			$title = $row->title;
			$nid = $row->nid;
			$body = $row->body_value;
			$array[$seganaid] = $title.$body;
			$array_node[$seganaid] = $nid;
		}
	}

	$query_viejo = "SELECT * FROM Notas $where_query_viejo $seganaid_inicio ORDER BY id_nota ASC $limit";
	$resultado_viejo = $segana_viejo->query($query_viejo);
	$array_viejo = array();
	drush_print("tiempo carga de nodos y notas:	 ". (microtime(TRUE) - $inicio));
	while ($row = $resultado_viejo->fetch_object()) {
		$seganaid = $row->id_nota;
		// reemplazar comillas de Micro$oft
		drush_print(mb_detect_encoding($row->titulo));
		$title = trim(html_entity_decode($row->titulo));
		$body = trim(html_entity_decode($row->cuerpo_texto));

		//$title = fixUTF8(forceUTF8(trim($title)));
		//$body = check_plain(fixUTF8(forceUTF8(trim($body))));
		

		
		$fecha = strtotime($row->fecha);
		$fecha = ($fecha > 0 ? $fecha : 0);
		drush_print($seganaid." ".$title ." ".$fecha);


		///////elementos discursivos/////////////
		$query_elementos_discurisvos = "SELECT * FROM Notas_Aspectos_Discursivos WHERE id_nota = $seganaid";
		$resultado_elementos_discursivos = $segana_viejo->query($query_elementos_discurisvos);
		$rows_elementos_discursivos = mysqli_num_rows($resultado_elementos_discursivos);
		if ($rows_elementos_discursivos > 0) {
			
			$tema_tid = array();
			$matriz_tid = array();
			$cuerpo_argumentativo = array();
			$actor_tid = array();
			$tendencia_tid = array();
			$delta = 0;
			$tendencia_nota = 0;
			unset($tendencia_nota_tid);
			while ($row_elementos_discursivos = $resultado_elementos_discursivos->fetch_object()) {

				$tema = $row_elementos_discursivos->id_tema;
				if ($tema != 5033) {
					$query = db_select('field_data_field_seganaid', 'fs')
						->condition(db_and()->condition('fs.bundle', 'tema', '=')->condition('fs.field_seganaid_value', $tema, '='))
						->fields('fs', array('entity_id'));
					$resultado = $query->execute();
					$row_tema = $resultado->fetchObject();
					$tema_tid[$delta] = $row_tema->entity_id;
				}

				$matriz = $row_elementos_discursivos->id_argumento;
				if ($matriz != 4195) {
					$query = db_select('field_data_field_seganaid', 'fs')
						->condition(db_and()->condition('fs.bundle', 'matriz', '=')->condition('fs.field_seganaid_value', $matriz, '='))
						->fields('fs', array('entity_id'));
					$resultado = $query->execute();
					$row_matriz = $resultado->fetchObject();
					$matriz_tid[$delta] = $row_matriz->entity_id;
				}

				if ($row_elementos_discursivos->cuerpo_argumento != 'No Analizada') {
					$cuerpo_argumentativo[$delta] = fixUTF8(forceUTF8(trim($row_elementos_discursivos->cuerpo_argumento)));
				}

				
				$actor = $row_elementos_discursivos->id_actor;
				if ($actor != 46058){
	 				$query = db_select('field_data_field_seganaid', 'fs')
						->condition(db_and()->condition('fs.bundle', 'actor', '=')->condition('fs.field_seganaid_value', $actor, '='))
						->fields('fs', array('entity_id'));
					$resultado = $query->execute();
					$row_actor = $resultado->fetchObject();
					$actor_tid[$delta] = $row_actor->entity_id;
				}

				$tendencia = $row_elementos_discursivos->tendencia;
				if ($tendencia > 0){
					$tendencia_tid[$delta] = 1;
					$tendencia_nota++;
				}
				elseif ($tendencia < 0) {
					$tendencia_tid[$delta] = 2;
					$tendencia_nota--;
				}
				

				drush_print($tema_tid[$delta] ." ". $matriz_tid[$delta] ." ". $actor_tid[$delta] ." ". $tendencia_tid[$delta]);
				$delta++;
 			}
 			if ($tendencia_nota != 0) {
				if ($tendencia_nota > 0) {
					$tendencia_nota_tid = 1;
				}
				else {
					$tendencia_nota_tid = 2;
				}
			}

		}


		////usuario
		$usuario = $row->id_usuario;
		$query = db_select('field_data_field_seganaid', 'fs')
			->condition(db_and()->condition('fs.bundle', 'user', '=')->condition('fs.field_seganaid_value', $usuario, '='))
			->fields('fs', array('entity_id'));
		$resultado = $query->execute();
		if ($resultado->rowCount() > 0){
			$row_usuario = $resultado->fetchObject();
			$usuario_uid = $row_usuario->entity_id;
		}
		else {
			$usuario_uid = 1;
		}



		if (!array_key_exists($seganaid, $array)) {
			$tipo_medio = $row->id_tipo_nota;
			$query = db_select('field_data_field_seganaid', 'fs')
				->condition(db_and()->condition('fs.bundle', 'tipo_medio', '=')->condition('fs.field_seganaid_value', $tipo_medio, '='))
				->fields('fs', array('entity_id'));
			$resultado = $query->execute();
			$row_tipo_medio = $resultado->fetchObject();
			$tipo_medio_tid = $row_tipo_medio->entity_id;
			
			$tipo_titulo = $row->id_tipo_titulo;
			$query = db_select('field_data_field_seganaid', 'fs')
				->condition(db_and()->condition('fs.bundle', 'tipo_titulo', '=')->condition('fs.field_seganaid_value', $tipo_titulo, '='))
				->fields('fs', array('entity_id'));
			$resultado = $query->execute();
			$row_tipo_titulo = $resultado->fetchObject();
			$tipo_titulo_tid = $row_tipo_titulo->entity_id;

			$tipo_informacion = $row->id_tipo_informacion;
			$query = db_select('field_data_field_seganaid', 'fs')
				->condition(db_and()->condition('fs.bundle', 'tipo_informacion', '=')->condition('fs.field_seganaid_value', $tipo_informacion, '='))
				->fields('fs', array('entity_id'));
			$resultado = $query->execute();
			$row_tipo_informacion = $resultado->fetchObject();
			$tipo_informacion_tid = $row_tipo_informacion->entity_id;

			$area = $row->id_area;
			$query = db_select('field_data_field_seganaid', 'fs')
				->condition(db_and()->condition('fs.bundle', 'area', '=')->condition('fs.field_seganaid_value', $area, '='))
				->fields('fs', array('entity_id'));
			$resultado = $query->execute();
			$row_area = $resultado->fetchObject();
			$area_tid = $row_area->entity_id;

			$medio = $row->id_medio_informacion;
			$query = db_select('field_data_field_seganaid', 'fs')
				->condition(db_and()->condition('fs.bundle', 'medio', '=')->condition('fs.field_seganaid_value', $medio, '='))
				->fields('fs', array('entity_id'));
			$resultado = $query->execute();
			$row_medio = $resultado->fetchObject();
			$medio_tid = $row_medio->entity_id;


			$autor = preg_replace(array('/\s\s+/','/^\s+/','/\\r/'),array(' ',' ',' '), $row->autor);
			$autor = fixUTF8(forceUTF8($autor));
			$autor = iconv(mb_detect_encoding($autor, mb_detect_order(), true), "UTF-8", $autor);
			$autor = str_ireplace('negativa', '', $autor);
			$autor = str_ireplace('positiva', '', $autor);
			drush_print($seganaid." sin limpiar ".$autor ." lenght:". strlen($autor));
			if (strlen($autor) > 2){
				$limpio = 0;
				while ($limpio == 0) {
					$autor_sa = sin_acentos($autor);
					$primer_caracter = $autor_sa[0];
					$ultimo_caracter = $autor_sa[strlen($autor_sa)-1];
					drush_print("primer:".$primer_caracter."|ultimo:".$ultimo_caracter."|");
					if (ctype_alnum_portable($primer_caracter) && ctype_alnum_portable($ultimo_caracter)) {
						$limpio = 1;
						drush_print("limpio ".$autor);
					}
					else{
						if (!ctype_alnum_portable($primer_caracter)) {
							drush_print($primer_caracter." de primero");
							$autor = substr($autor, 1);
						}
						if (!ctype_alnum_portable($ultimo_caracter)) {
							drush_print($ultimo_caracter." de ultimo");
							$autor = substr($autor, 0, strlen($autor)-1);
						}
					}
				}
			}
			else {
				unset($autor);
			}


			if ($tipo_medio == 2){
				$titulo_primera_pagina = fixUTF8(forceUTF8(trim($row->titulo_primera_pagina)));
				$cuerpo = fixUTF8(forceUTF8(trim($row->cuerpo)));
				$pagina = fixUTF8(forceUTF8(trim($row->pagina)));
				
				$cuadrante = $row->cuadrante;
				$query = db_select('field_data_field_seganaid', 'fs')
					->condition(db_and()->condition('fs.bundle', 'cuadrante', '=')->condition('fs.field_seganaid_value', $cuadrante, '='))
					->fields('fs', array('entity_id'));
				$resultado = $query->execute();
				$row_cuadrante = $resultado->fetchObject();
				$cuadrante_tid = $row_cuadrante->entity_id;

				//actualizar el campo versión impresa del medio
				$medio_term = taxonomy_term_load($medio_tid);
				if($taxonomy_term != FALSE) {
					$medio_term->field_version_impresa[LANGUAGE_NONE][0]['value'] = 1;
					taxonomy_term_save($medio_term);
				}
			}


			$node = new stdClass();
			$node->title = $title;
			$node->type = 'nota';
			node_object_prepare($node);

			$node->language = LANGUAGE_NONE;
			$node->uid = $usuario_uid;
			$node->status = 1;
			
			$node->body[$node->language][0][value] = $body;
			$node->field_fecha[$node->language][0][value] = $fecha;
			$node->field_tipo_medio[$node->language][0][tid] = $tipo_medio_tid;
			$node->field_tipo_titulo[$node->language][0][tid] = $tipo_titulo_tid;
			$node->field_tipo_informacion[$node->language][0][tid] = $tipo_informacion_tid;
			$node->field_area[$node->language][0][tid] = $area_tid;
			$node->field_medio[$node->language][0][tid] = $medio_tid;
			if (!empty($autor)){
				$node->field_autor[$node->language][0][value] = $autor;
			}
			$node->field_seganaid[$node->language][0][value] = $seganaid;
			$prueba = "";
			$tendencia_nota = 0;
			if ($rows_elementos_discursivos > 0) {
				for ($i=0; $i <= $delta; $i++) {
					if ($tema_tid[$i]) {
						$node->field_tema[$node->language][$i][tid] = $tema_tid[$i];
						$prueba.="tema ".$tema_tid[$i];
					}
					if ($matriz_tid[$i]){
						$node->field_matriz[$node->language][$i][tid] = $matriz_tid[$i];
						$prueba.="matriz ".$matriz_tid[$i];
					}
					if ($actor_tid[$i]){
						$node->field_actor[$node->language][$i][tid] = $actor_tid[$i];
						$prueba.="actor ".$actor_tid[$i];
					}
					if (!empty($cuerpo_argumentativo[$i])){
						$node->field_cuerpo_argumentativo[$node->language][$i][value] = $cuerpo_argumentativo[$i];
					}
					if ($tendencia_tid[$i]){
						$node->field_tendencia[$node->language][$i][tid] = $tendencia_tid[$i];
						$prueba.="tendencia ".$tendencia_tid[$i];
					}
				}
				if (isset($tendencia_nota_tid)) {
					$node->field_tendencia_nota[$node->language][0][tid] = $tendencia_nota_tid;
					$prueba.=" tendencia_nota ".$tendencia_nota_tid;
				}				
			}
			drush_print("tm ".$tipo_medio_tid."tt ".$tipo_titulo_tid."ti ".$tipo_informacion_tid."a ".$area_tid. "medio ".$medio_tid." ".$prueba);
			if($node = node_submit($node)) { // Prepare node for saving
				drush_print("insertando nodo");
				$node->created = time();
				$node->changed = $node->created;
				node_save($node);
			}
			//drush_print_r($node);
			drush_print($node->nid ." ". $node->title);
		}
		else {
			$inicio = microtime(TRUE);
			$query = db_select('field_data_field_seganaid', 'fs')
				->condition(db_and()->condition('fs.bundle', $tipo, '=')->condition('fs.field_seganaid_value', $seganaid, '='))
				->fields('fs', array('entity_id'));
			$resultado = $query->execute();
			$row = $resultado->fetchObject();
			$nid = $row->entity_id;

			$node = node_load($nid);
			unset($elementos_discursivos);
			unset($elementos_discursivos_nuevo);
			$delta_viejo = count($node->field_tema[$node->language]);
			for ($i=0; $i < $delta; $i++) {
				$elementos_discursivos_nuevo .= $tema_tid[$i] . $matriz_tid[$i] . $actor_tid[$i] . $cuerpo_argumentativo[$i] . $tendencia_tid[$i];
			}
			for ($i=0; $i < $delta_viejo; $i++) { 
				$elementos_discursivos .= $node->field_tema[$node->language][$i][tid] . $node->field_matriz[$node->language][$i][tid] . $node->field_actor[$node->language][$i][tid] . $node->field_cuerpo_argumentativo[$node->language][$i][value] . $node->field_tendencia[$node->language][$i][tid];
			}
			$elementos_discursivos_nuevo .=  $tendencia_nota_tid;
			$elementos_discursivos .= $node->field_tendencia_nota[$node->language][tid];

			$nuevo = md5($title . $body .$elementos_discursivos_nuevo);
			$actual = md5($array[$seganaid] . $elementos_discursivos);
			if ($actual != $nuevo){
				$node->title = $title;
				$node->language = LANGUAGE_NONE;
				$node->body[$node->language][0][value] = $body;
				$node->uid = $usuario_uid;
				if ($rows_elementos_discursivos > 0) {
					for ($i=0; $i <= $delta; $i++) {
						if ($tema_tid[$i]) {
							$node->field_tema[$node->language][$i][tid] = $tema_tid[$i];
						}
						if ($matriz_tid[$i]){
							$node->field_matriz[$node->language][$i][tid] = $matriz_tid[$i];
						}
						if ($actor_tid[$i]){
							$node->field_actor[$node->language][$i][tid] = $actor_tid[$i];
						}
						if (!empty($cuerpo_argumentativo[$i])){
							$node->field_cuerpo_argumentativo[$node->language][$i][value] = $cuerpo_argumentativo[$i];
						}
						if ($tendencia_tid[$i]){
							$node->field_tendencia[$node->language][$i][tid] = $tendencia_tid[$i];
						}
						else {
							unset($node->field_tendencia[$node->language][$i]);
						}
					}

					if (isset($tendencia_nota_tid)){
						$node->field_tendencia_nota[$node->language][0][tid] = $tendencia_nota_tid;
					}
					else {
						unset($node->field_tendencia_nota[$node->language]);
					}
				}
				node_save($node);
				drush_print("actualizado");
			}
			else {
				drush_print("sin cambios");
			}
			drush_print("tiempo comparacion actualizacion: ". (microtime(TRUE) - $inicio));
			drush_print('nid '.$nid);
		}
		drush_print();
	}
}
// write message to the log file
$log->lwrite(date('M d H:i:s') . ' Fin de importación');
 
// close log file
$log->lclose();

?>

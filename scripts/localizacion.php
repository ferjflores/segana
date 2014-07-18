<?php

include 'funciones.php';


$segana_db_drupal = 'segananew';

$conexion_jigoku = new mysqli('udun', 'fflores', 'cbc1560', 'segana');
$conexion_mitlan = new mysqli('jigoku', 'fflores', 'cbc1560', $segana_db_drupal);
$link = mysql_connect('udun', 'fflores', 'cbc1560');

$conexion_insert = $conexion_mitlan;

$hora_actual= time();

$estructura_basica = 1;
$discursivos = 0;
$notas = 0;
//$discursivo = 'subtema';

/////////////////////////////// estructura basica//////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

if ($estructura_basica == 0) {



////////cuadrante///////
///////////////////////

//cleaning

	$tipo = 'cuadrante';
//	borrar_node($tipo,$conexion_mitlan);
	
	$nombres = array(1=>'Arriba Derecha', 2=>'Arriba Izquierda', 3=>'Abajo Derecha', 4=>'Abajo Izquierda');
	
	$max = nuevo_nid_vid($conexion_mitlan);
	$nid = $max->nid;
	$vid = $max->vid;

	foreach ($nombres as $i=>$value){
		print $value." Tipo: ".$tipo."\n";
		if (!traducir_seganaid($i,$tipo,$conexion_mitlan)){
			$query_node = insertar_node($nid,$vid,$tipo,$nombres[$i],$hora_actual,'',$conexion_mitlan);
			$query_content_field = insertar_content_field($nid,$vid,$i,'seganaid',$conexion_mitlan,NULL);
			$tid = nuevo_tid($conexion_mitlan);
			$void = 16;
			$query_term_data = insertar_term_data($tid,$void,$nombres[$i],'',$conexion_mitlan);
			$query_term_hierarchy = insertar_term_hierarchy($tid,$conexion_mitlan);
			$query_nat = insertar_nat($nid,$tid,$void,$conexion_mitlan);
			$nid++;
			$vid++;

			print $query_node;
			print $query_content_field;
			print $query_term_data;
			print $query_nat;
			print "\n\n";
		}
	}


////////tipo_localizacion/////////
/////////////////////////////////

	$tipo = 'tipo_localizacion';
//	borrar_node($tipo,$conexion_mitlan);

	$nombres = array(1=>'Estatal', 2=>'Regional', 3=>'Nacional', 4=>'Internacional');

	$max = nuevo_nid_vid($conexion_mitlan);
	$nid = $max->nid;
	$vid = $max->vid;

	foreach ($nombres as $i=>$value){
		print $value." Tipo: ".$tipo."\n";
		if (!traducir_seganaid($i,$tipo,$conexion_mitlan)){
			$query_node = insertar_node($nid,$vid,$tipo,$nombres[$i],$hora_actual,'',$conexion_mitlan);
			$query_content_field = insertar_content_field($nid,$vid,$i,'seganaid',$conexion_mitlan,NULL);
			$tid = nuevo_tid($conexion_mitlan);
			$void = 8;
			$query_term_data = insertar_term_data($tid,$void,$nombres[$i],'',$conexion_mitlan);
			$query_term_hierarchy = insertar_term_hierarchy($tid,$conexion_mitlan);
			$query_nat = insertar_nat($nid,$tid,$void,$conexion_mitlan);
			$nid++;
			$vid++;

			print $query_node;
			print $query_content_field;
			print $query_term_data;
			print $query_nat;
			print "\n\n";
		}
	}


////////////tipo_medio//////////

	$tipo = 'tipo_medio';
//	borrar_node($tipo,$conexion_mitlan);

	$nombres = array(1=>'Tabloide', 2=>'Estandar', 3=>'No Tiene');

	$max = nuevo_nid_vid($conexion_mitlan);
	$nid = $max->nid;
	$vid = $max->vid;

	foreach ($nombres as $i=>$value){
		print $value." Tipo: ".$tipo."\n";
		if (!traducir_seganaid($i,$tipo,$conexion_mitlan)){
			$query_node = insertar_node($nid,$vid,$tipo,$nombres[$i],$hora_actual,'',$conexion_mitlan);
			$query_content_field = insertar_content_field($nid,$vid,$i,'seganaid',$conexion_mitlan,NULL);
			$tid = nuevo_tid($conexion_mitlan);
			$void = 17;
			$query_term_data = insertar_term_data($tid,$void,$nombres[$i],'',$conexion_mitlan);
			$query_term_hierarchy = insertar_term_hierarchy($tid,$conexion_mitlan);
			$query_nat = insertar_nat($nid,$tid,$void,$conexion_mitlan);
			$nid++;
			$vid++;

			print $query_node;
			print $query_content_field;
			print $query_term_data;
			print $query_nat;
			print "\n\n";
		}
	}


////////////tipo_medio//////////

	$tipo = 'periodicidad';
//	borrar_node($tipo,$conexion_mitlan);

	$nombres = array(1=>'Diario', 2=>'Vespertino', 3=>'Semanario', 4=>'Quincenal', 5=>'Mensual', 6=>'No Tiene');

	$max = nuevo_nid_vid($conexion_mitlan);
	$nid = $max->nid;
	$vid = $max->vid;

	foreach ($nombres as $i=>$value){
		print $value." Tipo: ".$tipo."\n";
		if (!traducir_seganaid($i,$tipo,$conexion_mitlan)){
			$query_node = insertar_node($nid,$vid,$tipo,$nombres[$i],$hora_actual,'',$conexion_mitlan);
			$query_content_field = insertar_content_field($nid,$vid,$i,'seganaid',$conexion_mitlan,NULL);
			$tid = nuevo_tid($conexion_mitlan);
			$void = 18;
			$query_term_data = insertar_term_data($tid,$void,$nombres[$i],'',$conexion_mitlan);
			$query_term_hierarchy = insertar_term_hierarchy($tid,$conexion_mitlan);
			$query_nat = insertar_nat($nid,$tid,$void,$conexion_mitlan);
			$nid++;
			$vid++;

			print $query_node;
			print $query_content_field;
			print $query_term_data;
			print $query_nat;
			print "\n\n";
		}
	}

////////////tipo_medio//////////

	$tipo = 'tipo_publicacion';
//	borrar_node($tipo,$conexion_mitlan);

	$nombres = array(1=>'General', 2=>'Especializado', 3=>'No Tiene');

	$max = nuevo_nid_vid($conexion_mitlan);
	$nid = $max->nid;
	$vid = $max->vid;

	foreach ($nombres as $i=>$value){
		print $value." Tipo: ".$tipo."\n";
		if (!traducir_seganaid($i,$tipo,$conexion_mitlan)){
			$query_node = insertar_node($nid,$vid,$tipo,$nombres[$i],$hora_actual,'',$conexion_mitlan);
			$query_content_field = insertar_content_field($nid,$vid,$i,'seganaid',$conexion_mitlan,NULL);
			$tid = nuevo_tid($conexion_mitlan);
			$void = 19;
			$query_term_data = insertar_term_data($tid,$void,$nombres[$i],'',$conexion_mitlan);
			$query_term_hierarchy = insertar_term_hierarchy($tid,$conexion_mitlan);
			$query_nat = insertar_nat($nid,$tid,$void,$conexion_mitlan);
			$nid++;
			$vid++;

			print $query_node;
			print $query_content_field;
			print $query_term_data;
			print $query_term_hierarchy;
			print $query_nat;
			print "\n\n";
		}
	}

////////////tipo_pagina//////////

	$tipo = 'tipo_pagina';
//	borrar_node($tipo,$conexion_mitlan);

	$nombres = array(1=>'Personal', 2=>'Página web de medio', 3=>'Portal', 4=>'Institucional');

	$max = nuevo_nid_vid($conexion_mitlan);
	$nid = $max->nid;
	$vid = $max->vid;

	foreach ($nombres as $i=>$value){
		print $value." Tipo: ".$tipo."\n";
		if (!traducir_seganaid($i,$tipo,$conexion_mitlan)){
			$query_node = insertar_node($nid,$vid,$tipo, utf8_decode(fixUTF8(forceUTF8($nombres[$i]))),$hora_actual,'',$conexion_mitlan);
			$query_content_field = insertar_content_field($nid,$vid,$i,'seganaid',$conexion_mitlan,NULL);
			$tid = nuevo_tid($conexion_mitlan);
			$void = 20;
			$query_term_data = insertar_term_data($tid,$void, utf8_decode(fixUTF8(forceUTF8($nombres[$i]))),'',$conexion_mitlan);
			$query_term_hierarchy = insertar_term_hierarchy($tid,$conexion_mitlan);
			$query_nat = insertar_nat($nid,$tid,$void,$conexion_mitlan);
			$nid++;
			$vid++;

			print $query_node;
			print $query_content_field;
			print $query_term_data;
			print $query_term_hierarchy;
			print $query_nat;
			print "\n\n";
		}
	}

////////////tipo_pagina//////////

	$tipo = 'tipo_carga';
//	borrar_node($tipo,$conexion_mitlan);

	$nombres = array(1=>'Primera Página', 2=>'Cuerpo');

	$max = nuevo_nid_vid($conexion_mitlan);
	$nid = $max->nid;
	$vid = $max->vid;

	foreach ($nombres as $i=>$value){
		print $value." Tipo: ".$tipo."\n";
		if (!traducir_seganaid($i,$tipo,$conexion_mitlan)){
			$query_node = insertar_node($nid,$vid,$tipo,utf8_decode(fixUTF8(forceUTF8($nombres[$i]))),$hora_actual,'',$conexion_mitlan);
			$query_content_field = insertar_content_field($nid,$vid,$i,'seganaid',$conexion_mitlan,NULL);
			$tid = nuevo_tid($conexion_mitlan);
			$void = 22;
			$query_term_data = insertar_term_data($tid,$void,utf8_decode(fixUTF8(forceUTF8($nombres[$i]))),'',$conexion_mitlan);
			$query_term_hierarchy = insertar_term_hierarchy($tid,$conexion_mitlan);
			$query_nat = insertar_nat($nid,$tid,$void,$conexion_mitlan);
			$nid++;
			$vid++;

			print $query_node;
			print $query_content_field;
			print $query_term_data;
			print $query_term_hierarchy;
			print $query_nat;
			print "\n\n";
		}
	}

////////////tendencia//////////

	$tipo = 'tendencia';
//	borrar_node($tipo,$conexion_mitlan);

	$nombres = array(1=>'Favor', 2=>'Contra');

	$max = nuevo_nid_vid($conexion_mitlan);
	$nid = $max->nid;
	$vid = $max->vid;

	foreach ($nombres as $i=>$value){
		print $value." Tipo: ".$tipo."\n";
		if (!traducir_seganaid($i,$tipo,$conexion_mitlan)){
			$query_node = insertar_node($nid,$vid,$tipo,$nombres[$i],$hora_actual,'',$conexion_mitlan);
			$query_content_field = insertar_content_field($nid,$vid,$i,'seganaid',$conexion_mitlan,NULL);
			$tid = nuevo_tid($conexion_mitlan);
			$void = 15;
			$query_term_data = insertar_term_data($tid,$void,$nombres[$i],'',$conexion_mitlan);
			$query_term_hierarchy = insertar_term_hierarchy($tid,$conexion_mitlan);
			$query_nat = insertar_nat($nid,$tid,$void,$conexion_mitlan);
			$nid++;
			$vid++;

			print $query_node;
			print $query_content_field;
			print $query_term_data;
			print $query_term_hierarchy;
			print $query_nat;
			print "\n\n";
		}
	}

////////////tipo_informacion//////////

	$tipo = 'tipo_informacion';
//	borrar_node($tipo,$conexion_mitlan);

	$nombres = array(1=>'Artículo de opinión', 2=>'Editorial', 3=>'Entrevista', 4=>'Noticia', 5=>'Reportaje', 6=>'Reseña');

	$max = nuevo_nid_vid($conexion_mitlan);
	$nid = $max->nid;
	$vid = $max->vid;

	foreach ($nombres as $i=>$value){
		print $value." Tipo: ".$tipo."\n";
		if (!traducir_seganaid($i,$tipo,$conexion_mitlan)){
			$query_node = insertar_node($nid,$vid,$tipo,utf8_decode(fixUTF8(forceUTF8($nombres[$i]))),$hora_actual,'',$conexion_mitlan);
			$query_content_field = insertar_content_field($nid,$vid,$i,'seganaid',$conexion_mitlan,NULL);
			$tid = nuevo_tid($conexion_mitlan);
			$void = 10;
			$query_term_data = insertar_term_data($tid,$void,utf8_decode(fixUTF8(forceUTF8($nombres[$i]))),'',$conexion_mitlan);
			$query_term_hierarchy = insertar_term_hierarchy($tid,$conexion_mitlan);
			$query_nat = insertar_nat($nid,$tid,$void,$conexion_mitlan);
			$nid++;
			$vid++;

			print $query_node;
			print $query_content_field;
			print $query_term_data;
			print $query_term_hierarchy;
			print $query_nat;
			print "\n\n";
		}
	}

////////////tipo_pagina//////////

	$tipo = 'tipo_nota';
//	borrar_node($tipo,$conexion_mitlan);

	$nombres = array(1=>'Digital', 2=>'Impreso');

	$max = nuevo_nid_vid($conexion_mitlan);
	$nid = $max->nid;
	$vid = $max->vid;

	foreach ($nombres as $i=>$value){
		print $value." Tipo: ".$tipo."\n";
		if (!traducir_seganaid($i,$tipo,$conexion_mitlan)){
			$query_node = insertar_node($nid,$vid,$tipo,$nombres[$i],$hora_actual,'',$conexion_mitlan);
			$query_content_field = insertar_content_field($nid,$vid,$i,'seganaid',$conexion_mitlan,NULL);
			$tid = nuevo_tid($conexion_mitlan);
			$void = 7;
			$query_term_data = insertar_term_data($tid,$void,$nombres[$i],'',$conexion_mitlan);
			$query_term_hierarchy = insertar_term_hierarchy($tid,$conexion_mitlan);
			$query_nat = insertar_nat($nid,$tid,$void,$conexion_mitlan);
			$nid++;
			$vid++;

			print $query_node;
			print $query_content_field;
			print $query_term_data;
			print $query_term_hierarchy;
			print $query_nat;
			print "\n\n";
		}
	}


///////////localizacion//////////
//////////////////////////////////

//cleaning

	$tipo = 'localizacion';
//	borrar_node($tipo,$conexion_mitlan);


	$query_localizacion = "SELECT * FROM Localizacion";

	$resultado_localizacion = $conexion_jigoku->query($query_localizacion);

	while ($row_localizacion = $resultado_localizacion->fetch_object()) {
		$seganaid = $row_localizacion->id_localizacion;
		$tipo_localizacion_segana = $row_localizacion->id_tipo_localizacion;
		$nombre = trim($row_localizacion->nombre);
		$tipo_localizacion = traducir_seganaid($tipo_localizacion_segana,'tipo_localizacion',$conexion_mitlan);



		$query_localizacion_mitlan = "SELECT * FROM content_field_seganaid JOIN node ON content_field_seganaid.nid = node.nid WHERE field_seganaid_value = $seganaid AND node.type = '$tipo'";
		$resultado_localizacion_mitlan = $conexion_mitlan->query($query_localizacion_mitlan);

		print $nombre." Tipo: ".$tipo."\n";
		if ($resultado_localizacion_mitlan->num_rows == 0) {

			$max = nuevo_nid_vid($conexion_mitlan);
			$nid=$max->nid;
			$vid=$max->vid;
		
			$query_node = insertar_node($nid,$vid,'localizacion',$nombre,$hora_actual,'',$conexion_mitlan);
		
			$fields = FALSE;
			$query_content_type = insertar_content_type($nid,$vid,$fields,'localizacion',$conexion_mitlan);

			$query_content_field = insertar_content_field($nid,$vid,$tipo_localizacion,'tipo_localizacion',$conexion_mitlan,NULL);
			$query_content_field2 = insertar_content_field($nid,$vid,$seganaid,'seganaid',$conexion_mitlan,NULL);
			$tid = nuevo_tid($conexion_mitlan);
			$void = 12;		
			$query_term_data = insertar_term_data($tid,$void,$nombre,'',$conexion_mitlan);
			$query_term_node = insertar_term_node($nid,$vid,$tipo_localizacion,$conexion_mitlan);
			$query_term_hierarchy = insertar_term_hierarchy($tid,$conexion_mitlan);
			$query_nat = insertar_nat($nid,$tid,$void,$conexion_mitlan);

			print $query_node;
			print $query_content_type;
			print $query_content_field;
			print $query_content_field2;
			print $query_term_node;
			print $query_term_hierarchy;
			print $query_nat;
			print "\n\n";
		}
	}


///////////medio//////////
//////////////////////////////////

//cleaning

		$tipo = 'medio';
//	borrar_node($tipo,$conexion_mitlan);


		$query_medio = "SELECT * FROM Medio_Informacion";

		$resultado_medio = $conexion_jigoku->query($query_medio);

		while ($row_medio = $resultado_medio->fetch_object()) {
			$seganaid = $row_medio->id_medio_informacion;
			$nombre = trim($row_medio->nombre);
			$pagina_web = $row_medio->pagina_web;
			$localizacion_segana = $row_medio->localizacion;
			$tipo_medio_segana = $row_medio->tipo_medio;
			$periodicidad_segana = $row_medio->periodicidad;
			$tipo_publicacion_segana = $row_medio->tipo_publicacion;


			$localizacion = traducir_seganaid($localizacion_segana,'localizacion',$conexion_mitlan);
			$tipo_medio = traducir_seganaid($tipo_medio_segana,'tipo_medio',$conexion_mitlan);
			$periodicidad = traducir_seganaid($periodicidad_segana,'periodicidad',$conexion_mitlan);
			$tipo_publicacion = traducir_seganaid($tipo_publicacion_segana,'tipo_publicacion',$conexion_mitlan);



			$query_medio_mitlan = "SELECT * FROM content_field_seganaid JOIN node ON content_field_seganaid.nid = node.nid WHERE field_seganaid_value = $seganaid AND node.type = '$tipo'";
			$resultado_medio_mitlan = $conexion_mitlan->query($query_medio_mitlan);

			print $nombre." Tipo: ".$tipo."\n";
			if ($resultado_medio_mitlan->num_rows == 0) {

				$max = nuevo_nid_vid($conexion_mitlan);
				$nid=$max->nid;
				$vid=$max->vid;
		
				$query_node = insertar_node($nid,$vid,$tipo,$nombre,$hora_actual,'',$conexion_mitlan);
		
				$fields = array($localizacion,$tipo_medio,$periodicidad,$tipo_publicacion,0,"'$pagina_web'","'Digital'");
				$query_content_type = insertar_content_type($nid,$vid,$fields,$tipo,$conexion_mitlan);

				$query_content_field = insertar_content_field($nid,$vid,$seganaid,'seganaid',$conexion_mitlan,NULL);
				$tid = nuevo_tid($conexion_mitlan);
				$void = 6;		
				$query_term_data = insertar_term_data($tid,$void,$nombre,'',$conexion_mitlan);
				$query_term_node = insertar_term_node($nid,$vid,$localizacion,$conexion_mitlan);
				$query_term_node2 = insertar_term_node($nid,$vid,$tipo_medio,$conexion_mitlan);
				$query_term_node3 = insertar_term_node($nid,$vid,$periodicidad,$conexion_mitlan);
				$query_term_node4 = insertar_term_node($nid,$vid,$tipo_publicacion,$conexion_mitlan);
				$query_term_hierarchy = insertar_term_hierarchy($tid,$conexion_mitlan);
				$query_nat = insertar_nat($nid,$tid,$void,$conexion_mitlan);

				print $query_node;
				print $query_content_type;
				print $query_content_field;
				print $query_term_node;
				print $query_term_node2;
				print $query_term_node3;
				print $query_term_node4;
				print $query_term_hierarchy;
				print $query_nat;
				print "\n\n";
			}
		}
	}

/////// discursivos/////////



if ($discursivos == 0 ){

///////////Área//////////
//////////////////////////////////

//cleaning

	if (empty($discursivo) || $discursivo == 'area'){
		$tipo = 'area';
		//	borrar_node($tipo,$conexion_mitlan);


		$query_area_mitlan = "SELECT field_seganaid_value, title, node.nid, nat.tid FROM content_field_seganaid JOIN node ON content_field_seganaid.nid = node.nid JOIN nat ON content_field_seganaid.nid = nat.nid WHERE node.type = '$tipo' ORDER BY field_seganaid_value ASC";
		$resultado_area_mitlan = $conexion_mitlan->query($query_area_mitlan);
		$array_area_mitlan = array();
		while ($row_area_mitlan = $resultado_area_mitlan->fetch_object()){
			$array_area_mitlan[] = $row_area_mitlan->field_seganaid_value;
			$array_area_mitlan_md5[$row_area_mitlan->field_seganaid_value]['title'] = $row_area_mitlan->title;
			$array_area_mitlan_md5[$row_area_mitlan->field_seganaid_value]['nid'] = $row_area_mitlan->nid;
			$array_area_mitlan_md5[$row_area_mitlan->field_seganaid_value]['tid'] = $row_area_mitlan->tid;
		}

		$query_area = "SELECT * FROM Area ORDER BY id_area ASC";
		$resultado_area = $conexion_jigoku->query($query_area);

		while ($row_area = $resultado_area->fetch_object()) {
					$seganaid = $row_area->id_area;

			print $seganaid."Tipo: ".$tipo."\n";

			if (!in_array($seganaid, $array_area_mitlan)) {
				$nombre = trim($row_area->nombre);
				$max = nuevo_nid_vid($conexion_mitlan);
				$nid=$max->nid;
				$vid=$max->vid;
		
				$query_node = insertar_node($nid,$vid,$tipo,$nombre,$hora_actual,'',$conexion_mitlan);
		
				$fields = array();
				$query_content_type = insertar_content_type($nid,$vid,$fields,$tipo,$conexion_mitlan);

				$query_content_field = insertar_content_field($nid,$vid,$seganaid,'seganaid',$conexion_mitlan,NULL);
				$tid = nuevo_tid($conexion_mitlan);
				$void = 14;		
				$query_term_data = insertar_term_data($tid,$void,$nombre,'',$conexion_mitlan);
				$query_term_hierarchy = insertar_term_hierarchy($tid,$conexion_mitlan);
				$query_nat = insertar_nat($nid,$tid,$void,$conexion_mitlan);

				print $query_node;
				print $query_content_type;
				print $query_content_field;
				print $query_term_hierarchy;
				print $query_nat;
				print "\n\n";
			}
			else {
				$nombre = trim($row_area->nombre);
				if ($array_area_mitlan_md5[$seganaid]['title'] != $nombre) {
					$query_update_node =  update_node($array_area_mitlan_md5[$seganaid]['nid'], $nombre, NULL, $conexion_mitlan);
					$query_update_term_data = update_term_data($array_area_mitlan_md5[$seganaid]['tid'], $nombre, NULL, $conexion_mitlan);
					print $query_update_node;
					print $query_update_term_data;
					print "\n\n";
				}
			}
		}
		foreach ($array_area_mitlan as $seganaid) {
			$id_area  = $seganaid;
			$query_buscar_id_area = "SELECT * FROM Area WHERE id_area = $id_area";
			$resultado_buscar_id_area = $conexion_jigoku->query($query_buscar_id_area);
			if ($resultado_buscar_id_area->num_rows == 0) {
				error_log($id_area."\n",3,'/tmp/prueba');
			}
		}

		mysqli_free_result($resultado_area_mitlan);
		mysqli_free_result($resultado_area);

	}


///////////actor//////////
//////////////////////////////////

//cleaning

	if (empty($discursivo) || $discursivo == 'actor'){
		$tipo = 'actor';
		//	borrar_node($tipo,$conexion_mitlan);


		$query_actor_mitlan = "SELECT field_seganaid_value, node.nid, node.title, field_descripcion_breve_value, tid FROM content_field_seganaid JOIN node ON content_field_seganaid.nid = node.nid LEFT JOIN content_field_descripcion_breve ON node.nid = content_field_descripcion_breve.nid JOIN nat ON node.nid = nat.nid WHERE node.type = '$tipo' ORDER BY field_seganaid_value ASC";
		$resultado_actor_mitlan = $conexion_mitlan->query($query_actor_mitlan);
		$array_actor_mitlan = array();
		while ($row_actor_mitlan = $resultado_actor_mitlan->fetch_object()){
			$array_actor_mitlan[] = $row_actor_mitlan->field_seganaid_value;
			$array_actor_mitlan_nuevo[$row_actor_mitlan->field_seganaid_value]['title'] = mysql_real_escape_string($row_actor_mitlan->title);
			$array_actor_mitlan_nuevo[$row_actor_mitlan->field_seganaid_value]['nid'] = $row_actor_mitlan->nid;
			$array_actor_mitlan_nuevo[$row_actor_mitlan->field_seganaid_value]['descripcion_breve'] = mysql_real_escape_string($row_actor_mitlan->field_descripcion_breve_value);
			$array_actor_mitlan_nuevo[$row_actor_mitlan->field_seganaid_value]['tid'] = $row_actor_mitlan->tid;
		}

		$query_actor = "SELECT * FROM Actor ORDER BY id_actor ASC";
		$resultado_actor = $conexion_jigoku->query($query_actor);

		$array_actor_jigoku = array();
		while ($row_actor = $resultado_actor->fetch_object()) {
			$seganaid = $row_actor->id_actor;
			$array_actor_jigoku[] = $seganaid;


			print $seganaid." Tipo: ".$tipo."\n";

			if (!in_array($seganaid, $array_actor_mitlan)) {
				$nombre = mysql_real_escape_string(utf8_decode(fixUTF8(forceUTF8(trim($row_actor->actor)))));
				$descripcion_breve = mysql_real_escape_string(utf8_decode(fixUTF8(forceUTF8(trim($row_actor->caracterizacion)))));
				$fecha_creacion = strtotime($row_actor->fecha_creacion)+16200;
				$cuerpo='';
				$max = nuevo_nid_vid($conexion_mitlan);
				$nid=$max->nid;
				$vid=$max->vid;
		
				$query_node = insertar_node($nid,$vid,$tipo,$nombre,$fecha_creacion,$cuerpo,$conexion_mitlan);
		
				$fields = array();
				$query_content_type = insertar_content_type($nid,$vid,$fields,$tipo,$conexion_mitlan);

				$query_content_field = insertar_content_field($nid,$vid,$seganaid,'seganaid',$conexion_mitlan,NULL);
				if (!empty($descripcion_breve)){
					$query_content_field2 = insertar_content_field($nid,$vid,"'$descripcion_breve'",'descripcion_breve',$conexion_mitlan,NULL);
				}
				$tid = nuevo_tid($conexion_mitlan);
				$void = 5;		
				$query_term_data = insertar_term_data($tid,$void,$nombre,$cuerpo,$conexion_mitlan);
				$query_term_hierarchy = insertar_term_hierarchy($tid,$conexion_mitlan);
				$query_nat = insertar_nat($nid,$tid,$void,$conexion_mitlan);

				print $query_node;
				print $query_content_type;
				print $query_content_field;
				print $query_content_field2;
				print $query_term_hierarchy;
				print $query_nat;
				print "\n\n";
			}
			else {
				$nombre = mysql_real_escape_string(utf8_decode(fixUTF8(forceUTF8(trim($row_actor->actor)))));
				$descripcion_breve = mysql_real_escape_string(utf8_decode(fixUTF8(forceUTF8(trim($row_actor->caracterizacion)))));
				if ($array_actor_mitlan_nuevo[$seganaid]['title'].$array_actor_mitlan_nuevo[$seganaid]['descripcion_breve'] != $nombre.$descripcion_breve){
					$query_update_node = update_node($array_actor_mitlan_nuevo[$seganaid]['nid'], $nombre, NULL, $conexion_mitlan);
					$query_update_term_data = update_term_data($array_actor_mitlan_nuevo[$seganaid]['tid'], $nombre, NULL, $conexion_mitlan);
					$query_update_content_field= update_content_field($array_actor_mitlan_nuevo[$seganaid]['nid'], $descripcion_breve, 'descripcion_breve', $conexion_mitlan, NULL);
				
					print $query_update_node;
					print $query_update_term_data;
					print $query_update_content_field;
					print "\n\n";
				}
			}
		}
		foreach ($array_actor_mitlan as $seganaid) {
			$id_actor  = $seganaid;
			if (!in_array($id_actor, $array_actor_jigoku)) {
				error_log($id_actor."\n",3,'/tmp/prueba');
			}
		}
		mysqli_free_result($resultado_actor_mitlan);
		mysqli_free_result($resultado_actor);
	}

/////////////autor/////////////////
//////////////////////////////////

////cleaning////

	if (empty($discursivo) || $discursivo == 'autor'){
		$tipo = 'autor';
		//	borrar_node($tipo,$conexion_mitlan);

		$query_autor_mitlan = "SELECT field_seganaid_value FROM content_field_seganaid JOIN node ON content_field_seganaid.nid = node.nid WHERE node.type = '$tipo' ORDER BY field_seganaid_value ASC";
		$resultado_autor_mitlan = $conexion_mitlan->query($query_autor_mitlan);
		$resultado_autor_array = array();
		while ($row = $resultado_autor_mitlan->fetch_object()){
			$resultado_autor_array[] = $row->field_seganaid_value;
		}

		$query_autor = "SELECT id_nota,autor2 from (SELECT id_nota,TRIM(TRAILING ')' FROM TRIM(LEADING '(' FROM TRIM(autor))) AS autor2, LOWER(TRIM(TRAILING ')' FROM TRIM(LEADING '(' FROM TRIM(autor)))) AS autor_lower FROM Notas GROUP BY autor_lower ORDER BY id_nota) AS T1 ORDER BY id_nota ASC";
		$resultado_autor = $conexion_jigoku->query($query_autor);

		while ($row_autor = $resultado_autor->fetch_object()) {
			$seganaid = $row_autor->id_nota;

			print $seganaid." Tipo: ".$tipo."\n";
			if (!in_array($seganaid, $resultado_autor_array)) {
				$nombre = trim(preg_replace(array('/\s\s+/','/^\s+/','/\\r/'),array(' ',' ',' '), $row_autor->autor2));
				$nombre = utf8_decode(fixUTF8(forceUTF8($nombre)));
				$nombre = mysql_real_escape_string($nombre);
				$max = nuevo_nid_vid($conexion_mitlan);
				$nid=$max->nid;
				$vid=$max->vid;
		
				$query_node = insertar_node($nid,$vid,$tipo,$nombre,$hora_actual,'',$conexion_mitlan);
		
				$fields = array();
				$query_content_type = insertar_content_type($nid,$vid,$fields,$tipo,$conexion_mitlan);

				$query_content_field = insertar_content_field($nid,$vid,$seganaid,'seganaid',$conexion_mitlan,NULL);
				$tid = nuevo_tid($conexion_mitlan);
				$void = 11;		
				$query_term_data = insertar_term_data($tid,$void,$nombre,'',$conexion_mitlan);
				$query_term_hierarchy = insertar_term_hierarchy($tid,$conexion_mitlan);
				$query_nat = insertar_nat($nid,$tid,$void,$conexion_mitlan);

				print $query_node;
				print $query_content_type;
				print $query_content_field;
				print $query_term_hierarchy;
				print $query_nat;
				print "\n\n";
			}
		}
		mysqli_free_result($resultado_autor_mitlan);
		mysqli_free_result($resultado_autor);
	}

/////////////matriz/////////////////
//////////////////////////////////

////cleaning////

	if (empty($discursivo) || $discursivo == 'matriz'){
		$tipo = 'matriz';
		//	borrar_node($tipo,$conexion_mitlan);

		$query_matriz_mitlan = "SELECT field_seganaid_value, node.nid, node.title, field_descripcion_breve_value, tid FROM content_field_seganaid JOIN node ON content_field_seganaid.nid = node.nid LEFT JOIN content_field_descripcion_breve ON node.nid = content_field_descripcion_breve.nid JOIN nat ON node.nid = nat.nid WHERE node.type = '$tipo' ORDER BY field_seganaid_value ASC";
		$resultado_matriz_mitlan = $conexion_mitlan->query($query_matriz_mitlan);
		$array_matriz_mitlan = array();
		while ($row_matriz_mitlan = $resultado_matriz_mitlan->fetch_object()){
			$seganaid = $row_matriz_mitlan->field_seganaid_value;
			$array_matriz_mitlan[] = $seganaid;
			$array_matriz_mitlan_nuevo[$row_matriz_mitlan->field_seganaid_value]['title'] = mysql_real_escape_string($row_matriz_mitlan->title);
			$array_matriz_mitlan_nuevo[$row_matriz_mitlan->field_seganaid_value]['nid'] = $row_matriz_mitlan->nid;
			$array_matriz_mitlan_nuevo[$row_matriz_mitlan->field_seganaid_value]['descripcion_breve'] = mysql_real_escape_string($row_matriz_mitlan->field_descripcion_breve_value);
			$array_matriz_mitlan_nuevo[$row_matriz_mitlan->field_seganaid_value]['tid'] = $row_matriz_mitlan->tid;
		}

		$query_matriz = "SELECT * from Argumento ORDER BY id_argumento ASC";
		$resultado_matriz = $conexion_jigoku->query($query_matriz);

		$array_matriz_jigoku = array();
		while ($row_matriz = $resultado_matriz->fetch_object()) {
			$seganaid = $row_matriz->id_argumento;
			$array_matriz_jigoku[] = $seganaid;
			print $seganaid." Tipo: ".$tipo."\n";

			if (!in_array($seganaid, $array_matriz_mitlan)) {
				$nombre = mysql_real_escape_string(utf8_decode(fixUTF8(forceUTF8(trim($row_matriz->argumento)))));
				$descripcion_breve = mysql_real_escape_string(utf8_decode(fixUTF8(forceUTF8(trim($row_matriz->caracterizacion)))));
				$fecha_creacion = strtotime($row_matriz->fecha_creacion)+16200;
				$max = nuevo_nid_vid($conexion_mitlan);
				$nid=$max->nid;
				$vid=$max->vid;
		
				$query_node = insertar_node($nid,$vid,$tipo,$nombre,$fecha_creacion,'',$conexion_mitlan);
		
				$fields = array();
				$query_content_type = insertar_content_type($nid,$vid,$fields,$tipo,$conexion_mitlan);

				if ($descripcion_breve){
					$query_content_field = 	insertar_content_field($nid,$vid,"'$descripcion_breve'",'descripcion_breve',$conexion_mitlan,NULL);	
				}
				$query_content_field2 = insertar_content_field($nid,$vid,$seganaid,'seganaid',$conexion_mitlan,NULL);
				$tid = nuevo_tid($conexion_mitlan);
				$void = 3;		
				$query_term_data = insertar_term_data($tid,$void,$nombre,$descripcion_breve,$conexion_mitlan);
				$query_term_hierarchy = insertar_term_hierarchy($tid,$conexion_mitlan);
				$query_nat = insertar_nat($nid,$tid,$void,$conexion_mitlan);

				print $query_node;
				print $query_content_type;
				print $query_content_field;
				print $query_content_field2;
				print $query_term_data;
				print $query_term_hierarchy;
				print $query_nat;
				print "\n\n";
			}
			else {
				$nombre = mysql_real_escape_string(utf8_decode(fixUTF8(forceUTF8(trim($row_matriz->argumento)))));
				$descripcion_breve = mysql_real_escape_string(utf8_decode(fixUTF8(forceUTF8(trim($row_matriz->caracterizacion)))));
				if ($array_matriz_mitlan_nuevo[$seganaid]['title'].$array_matriz_mitlan_nuevo[$seganaid]['descripcion_breve'] != $nombre.$descripcion_breve){
					$query_update_node = update_node($array_matriz_mitlan_nuevo[$seganaid]['nid'], $nombre, NULL, $conexion_mitlan);
					$query_update_term_data = update_term_data($array_matriz_mitlan_nuevo[$seganaid]['tid'], $nombre, NULL, $conexion_mitlan);
					$query_update_content_field= update_content_field($array_matriz_mitlan_nuevo[$seganaid]['nid'], $descripcion_breve, 'descripcion_breve', $conexion_mitlan, NULL);
				
					print $query_update_node;
					print $query_update_term_data;
					print $query_update_content_field;
					print "\n\n";
				}
			}
		}
		foreach ($array_matriz_mitlan as $seganaid) {
			$id_matriz  = $seganaid;
			if (!in_array($id_matriz, $array_matriz_jigoku)) {

			}
		}
		mysqli_free_result($resultado_matriz_mitlan);
		mysqli_free_result($resultado_matriz);
	}

/////////////subtema/////////////////
//////////////////////////////////

////cleaning////

	if (empty($discursivo) || $discursivo == 'subtema'){
		$tipo = 'subtema';
		//	borrar_node($tipo,$conexion_mitlan);

		$query_subtema_mitlan = "SELECT field_seganaid_value, node.nid, node.title, field_descripcion_breve_value, tid FROM content_field_seganaid JOIN node ON content_field_seganaid.nid = node.nid LEFT JOIN content_field_descripcion_breve ON node.nid = content_field_descripcion_breve.nid JOIN nat ON node.nid = nat.nid WHERE node.type = '$tipo' ORDER BY field_seganaid_value ASC";
		$resultado_subtema_mitlan = $conexion_mitlan->query($query_subtema_mitlan);
		$array_subtema_mitlan = array();
		while ($row_subtema_mitlan = $resultado_subtema_mitlan->fetch_object()){
			$seganaid = $row_subtema_mitlan->field_seganaid_value;
			$array_subtema_mitlan[] = $seganaid;
			$array_subtema_mitlan_nuevo[$seganaid]['title'] = mysql_real_escape_string($row_subtema_mitlan->title);
			$array_subtema_mitlan_nuevo[$seganaid]['nid'] = $row_subtema_mitlan->nid;
			$array_subtema_mitlan_nuevo[$seganaid]['descripcion_breve'] = mysql_real_escape_string($row_subtema_mitlan->field_descripcion_breve_value);
			$array_subtema_mitlan_nuevo[$seganaid]['tid'] = $row_subtema_mitlan->tid;
		}


		$query_subtema = "SELECT * from Tema ORDER BY id_tema ASC";
		$resultado_subtema = $conexion_jigoku->query($query_subtema);

		$array_subtema_jigoku = array();
		while ($row_subtema = $resultado_subtema->fetch_object()) {
			$seganaid = $row_subtema->id_tema;
			$array_subtema_jigoku[] = $seganaid;

			print $seganaid." Tipo: ".$tipo."\n";

			if (!in_array($seganaid, $array_subtema_mitlan)) {
				$conexion_mitlan->query("START TRANSACTION");
				$nombre = mysql_real_escape_string(utf8_decode(fixUTF8(forceUTF8(trim($row_subtema->nombre)))));
				$descripcion_breve = mysql_real_escape_string(utf8_decode(fixUTF8(forceUTF8(trim($row_subtema->caracterizacion)))));
				$fecha_creacion = strtotime($row_subtema->fecha_creacion)+16200;
				$max = nuevo_nid_vid($conexion_mitlan);
				$nid=$max->nid;
				$vid=$max->vid;
		
				$query_node = insertar_node($nid,$vid,$tipo,$nombre,$fecha_creacion,'',$conexion_mitlan);
		
				$fields = array();
				$query_content_type = insertar_content_type($nid,$vid,$fields,$tipo,$conexion_mitlan);

				if ($descripcion_breve){
					$query_content_field = insertar_content_field($nid,$vid,"'$descripcion_breve'",'descripcion_breve',$conexion_mitlan);
				}
				$query_content_field2 = insertar_content_field($nid,$vid,$seganaid,'seganaid',$conexion_mitlan,NULL);
				$tid = nuevo_tid($conexion_mitlan);
				$void = 2;
				$query_term_data = insertar_term_data($tid,$void,$nombre,$descripcion_breve,$conexion_mitlan);
				$query_term_hierarchy = insertar_term_hierarchy($tid,$conexion_mitlan);
				$query_nat = insertar_nat($nid,$tid,$void,$conexion_mitlan);
				$conexion_mitlan->query("COMMIT");
				print $query_node;
				print $query_content_type;
				print $query_content_field;
				print $query_content_field2;
				print $query_term_hierarchy;
				print $query_nat;
				print "\n\n";
			}
			else {
				$nombre = mysql_real_escape_string(utf8_decode(fixUTF8(forceUTF8(trim($row_subtema->nombre)))));
				$descripcion_breve = mysql_real_escape_string(utf8_decode(fixUTF8(forceUTF8(trim($row_subtema->caracterizacion)))));
				if ($array_subtema_mitlan_nuevo[$seganaid]['title'].$array_subtema_mitlan_nuevo[$seganaid]['descripcion_breve'] != $nombre.$descripcion_breve){
					$conexion_mitlan->query("START TRANSACTION");
					$query_update_node = update_node($array_subtema_mitlan_nuevo[$seganaid]['nid'], $nombre, NULL, $conexion_mitlan);
					$query_update_term_data = update_term_data($array_subtema_mitlan_nuevo[$seganaid]['tid'], $nombre, NULL, $conexion_mitlan);
					$query_update_content_field= update_content_field($array_subtema_mitlan_nuevo[$seganaid]['nid'], $descripcion_breve, 'descripcion_breve', $conexion_mitlan, NULL);
				$conexion_mitlan->query("COMMIT");
					print $query_update_node;
					print $query_update_term_data;
					print $query_update_content_field;
					print "\n\n";
				}
			}
		}
		foreach ($array_subtema_mitlan as $seganaid) {
			$id_subtema  = $seganaid;
			if (!in_array($id_subtema, $array_subtema_jigoku)) {
				borrar_node('subtema',$conexion_mitlan,$array_subtema_mitlan_nuevo[$id_subtema]['nid']);
			}
		}
		mysqli_free_result($resultado_subtema_mitlan);
		mysqli_free_result($resultado_subtema);
		}
	}




if ($notas == 0) {

/////////////Notas/////////////////
//////////////////////////////////

////cleaning////


	$ano = '2013-07';
	$tipo = 'nota';
//borrar_node($tipo,$conexion_mitlan);

	// cargar usuarios segananew////
	$query_usuarios_mitlan = "SELECT users.uid,name FROM users JOIN users_roles ON (users.uid = users_roles.uid) WHERE rid=6";
	$resultado_usuarios_mitlan = $conexion_mitlan->query($query_usuarios_mitlan);
	$array_usuarios_mitlan = array();
	while ($row_usuarios_mitlan = $resultado_usuarios_mitlan->fetch_object()){
		$array_usuarios_mitlan[$row_usuarios_mitlan->uid] = $row_usuarios_mitlan->name;
	}

	//cargar usuarios segana viejo ///
	$query_usuarios_jigoku = "SELECT id_usuario,usuario FROM Usuario WHERE clave <> MD5('usuario eliminado')";
	$resultado_usuarios_jigoku = $conexion_jigoku->query($query_usuarios_jigoku);
	$array_usuarios_jigoku = array();
	while ($row_usuarios_jigoku = $resultado_usuarios_jigoku->fetch_object()){
		$array_usuarios_jigoku[$row_usuarios_jigoku->id_usuario] = $row_usuarios_jigoku->usuario;
	}

	$query_minmax = "SELECT MIN(id_nota) as min ,MAX(id_nota) as max from Notas WHERE fecha LIKE '$ano%'";
	$resultado_minmax = $conexion_jigoku->query($query_minmax);
	$row_minmax = $resultado_minmax->fetch_object();
	$rango_min_nota = $row_minmax->min;
	$rango_max_nota = $row_minmax->max;
	$max_nota = $rango_min_nota;
	while ($max_nota <= $rango_max_nota){
		$min_nota = $max_nota;
		$max_nota = $max_nota+1000;
		print "Rango: ".$min_nota." - ".$max_nota."\n\n";
		$inicio = microtime(true);
		$query_nota_mitlan = "SELECT field_seganaid_value, node.nid, node.vid FROM content_field_seganaid JOIN node ON content_field_seganaid.nid = node.nid WHERE node.type = '$tipo' AND field_seganaid_value >= $min_nota AND field_seganaid_value <= $max_nota ORDER BY field_seganaid_value ASC";
		print $query_nota_mitlan ."\n";
		$resultado_nota_mitlan = $conexion_mitlan->query($query_nota_mitlan);
		$total = microtime(true)-$inicio;
		print "seganaid: ".$total."\n";

		$inicio = microtime(true);
		$query_autor = "SELECT tid,name AS name FROM term_data WHERE vid = 11";
		$resultado_autor = $conexion_mitlan->query($query_autor);
		$autor_array = array();
		while ($row = $resultado_autor->fetch_object()){
			$autor_array[] = $row;
		}
		print "\n suma arreglo autor ".count($autor_array)."\n";
		$total = microtime(true)-$inicio;
		print "arreglo autores: ".$total."\n";

		$inicio = microtime(true);
		$query_aspectos_discursivos = "SELECT * FROM Notas_Aspectos_Discursivos WHERE id_nota >= $min_nota AND id_nota <= $max_nota ORDER BY id_nota ASC";
		$resultado_aspectos_discursivos = $conexion_jigoku->query($query_aspectos_discursivos);
		$aspectos_discursivos_array = array();
		while ($row = $resultado_aspectos_discursivos->fetch_object()){
			$aspectos_discursivos_array[] = $row;
		}
		print "\n suma arreglo discursivos: ".count($aspectos_discursivos_array)."\n";
		$total = microtime(true)-$inicio;
		print "arreglo discursivos: ".$total."\n";

		$inicio = microtime(true);
		$query_aspectos_discursivos_mitlan = "SELECT area.nid AS nid, filas_discursivo, area.delta AS delta, area.field_area_value AS area, subtema.field_subtema_value AS subtema, matriz.field_matriz_value AS matriz, argumentador.field_argumentador_value AS argumentador, texto_argumentativo.field_texto_argumentativo_value AS texto_argumentativo, content_field_seganaid.field_seganaid_value AS seganaid FROM content_field_area AS area JOIN content_field_seganaid ON area.nid = content_field_seganaid.nid JOIN (content_field_subtema AS subtema, content_field_matriz AS matriz, content_field_argumentador AS argumentador, content_field_texto_argumentativo AS texto_argumentativo) ON (area.nid = subtema.nid AND area.delta = subtema.delta AND area.nid = matriz.nid AND area.delta = matriz.delta AND area.nid = argumentador.nid AND area.delta = argumentador.delta AND area.nid = texto_argumentativo.nid AND area.delta = texto_argumentativo.delta) JOIN (SELECT count(delta) AS filas_discursivo, content_field_area.nid AS nid from content_field_area JOIN content_field_seganaid ON content_field_area.nid = content_field_seganaid.nid WHERE content_field_seganaid.field_seganaid_value >= $min_nota AND content_field_seganaid.field_seganaid_value <= $max_nota GROUP BY nid) AS contador_delta ON area.nid = contador_delta.nid WHERE content_field_seganaid.field_seganaid_value >= $min_nota AND content_field_seganaid.field_seganaid_value <= $max_nota ORDER BY area.nid,delta";
		$resultado_aspectos_discursivos_mitlan = $conexion_mitlan->query($query_aspectos_discursivos_mitlan);
		$array_aspectos_discursivos_nuevo = array();
		while ($row = $resultado_aspectos_discursivos_mitlan->fetch_object()){
			$array_aspectos_discursivos_nuevo[] = $row;
		}
		print "\n suma arreglo discursivos mitlan: ".count($array_aspectos_discursivos_nuevo)."\n";
		$total = microtime(true)-$inicio;
		print "arreglo discursivos mitlan: ".$total."\n";

		$query_traducir = "SELECT field_seganaid_value AS seganaid,tid,type FROM nat JOIN content_field_seganaid ON nat.nid = content_field_seganaid.nid JOIN node ON nat.nid = node.nid WHERE type != 'nota'";
		$resultado_traducir = $conexion_mitlan->query($query_traducir);
		$traducir = array();
		while ($row_traducir = $resultado_traducir->fetch_object()) {
			$traducir[$row_traducir->type][$row_traducir->seganaid] = $row_traducir->tid;
		}


		$query_nota = "SELECT * from Notas WHERE id_nota >= $min_nota AND id_nota <= $max_nota ORDER by id_nota ASC";
		$resultado_nota = $conexion_jigoku->query($query_nota);

		while ($row_nota = $resultado_nota->fetch_object()) {
			$seganaid = $row_nota->id_nota;

			print $seganaid." Tipo: ".$tipo."\n";

			$inicio = microtime(true);
			if (!($nid = buscar($seganaid, $resultado_nota_mitlan))) {
				$total_nota_inicio = microtime(true);
				$total = microtime(true)-$inicio;
				print "Buscar seganaid: ".$total."\n";
				$tipo_nota = traducir_array($row_nota->id_tipo_nota,$traducir['tipo_nota']);
				$tipo_informacion = traducir_array($row_nota->id_tipo_informacion,$traducir['tipo_informacion']);
				print $tipo_informacion."\n";
				$area = traducir_array($row_nota->id_area, $traducir['area']);
				$medio = traducir_array($row_nota->id_medio_informacion,$traducir['medio']);
				$cuerpo_pagina = mysql_real_escape_string($row_nota->cuerpo);
				$pagina = $row_nota->pagina;
				$autor_str_segana = trim(preg_replace(array('/\s\s+/','/^\s+/','/\\r/'),array(' ',' ',' '), utf8_decode(fixUTF8(forceUTF8(rtrim(ltrim(trim($row_nota->autor), "("), ")"))))));
				$cuerpo_texto = mysql_real_escape_string(utf8_decode(fixUTF8(forceUTF8(trim($row_nota->cuerpo_texto)))));
				$cuadrante = traducir_array($row_nota->cuadrante,$traducir['cuadrante']);
				$titulo_primera_pagina = utf8_decode(fixUTF8(forceUTF8(trim($row_nota->titulo_primera_pagina))));
				$uid_segana = $row_nota->id_usuario;
				print "uid segana ". $uid_segana ."\n";
				if (array_key_exists($uid_segana,$array_usuarios_jigoku)) {
					$usuario_segana = $array_usuarios_jigoku[$uid_segana];
					if (in_array($usuario_segana,$array_usuarios_mitlan)){
						$uid = array_search($usuario_segana, $array_usuarios_mitlan);
					}
				}
				else {
					//$usuario_segana = 'admin';
					$uid = 1;
					$usuario_segana = 'no existe';
				}
				print "usuario segana ". $usuario_segana ."\n";
				print "uid segananew ". $uid ."\n";

				$fecha_creacion = strtotime($row_nota->fecha)+16200;
				$fecha_nota = date('Y-m-d\T00:00:00',$fecha_creacion);
				$nombre = mysql_real_escape_string(utf8_decode(fixUTF8(forceUTF8(trim($row_nota->titulo)))));

				// insertar field_impreso_value en content_type_medio
				if ($row_nota->id_tipo_nota == 2) {
					$query_medio_impreso = "UPDATE content_type_medio set field_impreso_value = 'Impreso' WHERE nid=(SELECT nid FROM nat where tid = $medio)";
					$resultado_medio_impreso = $conexion_mitlan->query($query_medio_impreso);
					print $query_medio_impreso."\n";
				}
		


				// determinar autor
				if (isset($autor_str_segana)) {

					print "Autor: ".$autor_str_segana."\n";
					$inicio = microtime(true);
					$autor = buscar_autor($autor_str_segana, $autor_array);
					$total = microtime(true)-$inicio;
					print "Buscar autor: ".$total."\n";
				}

				// determinar tipo de carga
				if ($row_nota->id_tipo_nota == 2){
					$tipo_carga = traducir_array(2,$traducir['tipo_carga']);
				}
				else {
					$tipo_carga = 'NULL';
				}

				$max = nuevo_nid_vid($conexion_mitlan);
				$nid=$max->nid;
				$vid=$max->vid;
						$comienzo = microtime(true);
				$conexion_mitlan->query("START TRANSACTION");
				$query_node = insertar_node($nid,$vid,$tipo,utf8_decode(fixUTF8(forceUTF8($nombre))),$fecha_creacion,$cuerpo_texto,$conexion_mitlan,$uid);
		
				$fields = array($tipo_informacion,$tipo_nota,'NULL','NULL',"'$pagina'",$cuadrante,$autor,$tipo_carga,0,0,0,"'$cuerpo_pagina'","'$fecha_nota'",'NULL');
				$query_content_type = insertar_content_type($nid,$vid,$fields,$tipo,$conexion_mitlan);


		
				$query_content_field = insertar_content_field($nid,$vid,$seganaid,'seganaid',$conexion_mitlan);
				$query_content_field7 = insertar_content_field($nid,$vid,$medio,'medio',$conexion_mitlan);

				$query_term_node = insertar_term_node($nid,$vid,$medio,$conexion_mitlan);
				$query_term_node2 = insertar_term_node($nid,$vid,$tipo_informacion,$conexion_mitlan);
				$query_term_node3 = insertar_term_node($nid,$vid,$tipo_nota,$conexion_mitlan);
				$query_term_node4 = insertar_term_node($nid,$vid,$cuadrante,$conexion_mitlan);
				$query_term_node5 = insertar_term_node($nid,$vid,$autor,$conexion_mitlan);
				$query_term_node6 = insertar_term_node($nid,$vid,$tipo_carga,$conexion_mitlan);



				// aspectos discursivos


				$delta = 0;
				unset($subtema,$matriz,$texto_argumentativo,$actor,$deltas);
				$inicio = microtime(true);
				$aspectos_discursivos = buscar_aspectos_discursivos($seganaid, $aspectos_discursivos_array);
				if (count($aspectos_discursivos) > 0) {
					$total = microtime(true)-$inicio;
					print "Buscar aspectos_discursivos: ".$total."\n";
					foreach ($aspectos_discursivos  as $row_aspectos_discursivos) {
						$subtema[] = traducir_array($row_aspectos_discursivos->id_tema, $traducir['subtema']);
						$matriz[] = traducir_array($row_aspectos_discursivos->id_argumento,$traducir['matriz']);
						$texto_argumentativo[] = mysql_real_escape_string(utf8_decode(fixUTF8(forceUTF8($row_aspectos_discursivos->cuerpo_argumento))));
						$actor[] = traducir_array($row_aspectos_discursivos->id_actor,$traducir['actor']);
						$deltas[] = $delta;
						$delta++;
					}
		
		

					if (isset($deltas)) {
						foreach ($deltas as $delta) {
							$query_content_field2 = insertar_content_field($nid,$vid,$subtema[$delta],'subtema',$conexion_mitlan,$delta);
							$query_content_field3 = insertar_content_field($nid,$vid,$matriz[$delta],'matriz',$conexion_mitlan,$delta);
							$query_content_field4 = insertar_content_field($nid,$vid,"'$texto_argumentativo[$delta]'",'texto_argumentativo',$conexion_mitlan,$delta);
							$query_content_field5 = insertar_content_field($nid,$vid,$actor[$delta],'argumentador',$conexion_mitlan,$delta);
							$query_content_field6 = insertar_content_field($nid,$vid,$area,'area',$conexion_mitlan,$delta);

							$query_term_node7 = insertar_term_node($nid,$vid,$subtema[$delta],$conexion_mitlan);
							$query_term_node8 = insertar_term_node($nid,$vid,$matriz[$delta],$conexion_mitlan);
							$query_term_node9 = insertar_term_node($nid,$vid,$actor[$delta],$conexion_mitlan);
							$query_term_node10 = insertar_term_node($nid,$vid,$area,$conexion_mitlan);
						}
					}

					print $query_node;
					print $query_content_type;
					print $query_content_field;
					print $query_content_field2;
					print $query_content_field3;
					print $query_content_field4;
					print $query_content_field5;
					print $query_content_field6;
					print $query_content_field7;
					print "\n\n";
				}
				else {
					print "No se encontraron aspectos_discursivos para $seganaid";
				}
				// insertar nodo primera pagina_web
				if ($titulo_primera_pagina) {
					$tipo_carga = traducir_array(1,$traducir['tipo_carga']);
					$nombre = $titulo_primera_pagina;

					$max = nuevo_nid_vid($conexion_mitlan);
					$nid=$max->nid;
					$vid=$max->vid;
		
					$query_node_pp = insertar_node($nid,$vid,$tipo,utf8_decode(fixUTF8(forceUTF8($nombre))),$fecha_creacion,NULL,$conexion_mitlan);
					$fields = array($tipo_informacion,$tipo_nota,'NULL','NULL','NULL','NULL',$autor,$tipo_carga,0,0,0,'NULL',"'$fecha_nota'",'NULL');
					$query_content_type_pp = insertar_content_type($nid,$vid,$fields,$tipo,$conexion_mitlan);
					$query_content_field_pp = insertar_content_field($nid,$vid,$seganaid,'seganaid',$conexion_mitlan);
					$query_content_field_pp7 = insertar_content_field($nid,$vid,$medio,'medio',$conexion_mitlan);

					$query_term_node_pp1 = insertar_term_node($nid,$vid,$medio,$conexion_mitlan);
					$query_term_node_pp2 = insertar_term_node($nid,$vid,$tipo_informacion,$conexion_mitlan);
					$query_term_node_pp3 = insertar_term_node($nid,$vid,$tipo_nota,$conexion_mitlan);
					$query_term_node_pp4 = insertar_term_node($nid,$vid,$autor,$conexion_mitlan);
					$query_term_node_pp5 = insertar_term_node($nid,$vid,$tipo_carga,$conexion_mitlan);
					$query_term_node_pp6 = insertar_term_node($nid,$vid,$area,$conexion_mitlan);
			
					if (isset($deltas)) {
						foreach ($deltas as $delta) {
							$query_content_field_pp2 = insertar_content_field($nid,$vid,$subtema[$delta],'subtema',$conexion_mitlan,$delta);
							$query_content_field_pp3 = insertar_content_field($nid,$vid,$matriz[$delta],'matriz',$conexion_mitlan,$delta);
							$query_content_field_pp4 = insertar_content_field($nid,$vid,"'$texto_argumentativo[$delta]'",'texto_argumentativo',$conexion_mitlan,$delta);
							$query_content_field_pp5 = insertar_content_field($nid,$vid,$actor[$delta],'argumentador',$conexion_mitlan,$delta);
							$query_content_field_pp6 = insertar_content_field($nid,$vid,$area,'area',$conexion_mitlan,$delta);

							$query_term_node_pp6 = insertar_term_node($nid,$vid,$subtema[$delta],$conexion_mitlan);
							$query_term_node_pp7 = insertar_term_node($nid,$vid,$matriz[$delta],$conexion_mitlan);
							$query_term_node_pp8 = insertar_term_node($nid,$vid,$actor[$delta],$conexion_mitlan);

						}
					}
					print "Primera página\n";
					print $query_node_pp;
					print $query_content_type_pp;
					print $query_content_field_pp;
					print $query_content_field_pp2;
					print $query_content_field_pp3;
					print $query_content_field_pp4;
					print $query_content_field_pp5;
					print $query_content_field_pp6;
					print $query_content_field_pp7;
					print "\n\n";

				}

			$conexion_mitlan->query("COMMIT");
				$final = microtime(true)-$comienzo;
				print "prueba tiempo ". $final ."\n";
			$total_nota = microtime(true)-$total_nota_inicio;
			print "Buscar Tiempo total por nota: ".$total_nota."\n";
			}
			else {
				print "nota ya insertada \n";
				$delta = 0;
				unset($area_delta,$subtema,$matriz,$texto_argumentativo,$actor,$deltas);
				$inicio = microtime(true);
				$aspectos_discursivos = buscar_aspectos_discursivos($seganaid, $aspectos_discursivos_array);
				$total = microtime(true)-$inicio;
				print "Buscar aspectos_discursivos: ".$total."\n";
				$aspectos_discursivos_string = '';
				foreach ($aspectos_discursivos  as $row_aspectos_discursivos) {
					$area_delta[] = $area;
					$subtema[] = traducir_array($row_aspectos_discursivos->id_tema, $traducir['subtema']);
					$matriz[] = traducir_array($row_aspectos_discursivos->id_argumento,$traducir['matriz']);
					$texto_argumentativo[] = mysql_real_escape_string(utf8_decode(fixUTF8(forceUTF8($row_aspectos_discursivos->cuerpo_argumento))));
					$actor[] = traducir_array($row_aspectos_discursivos->id_actor,$traducir['actor']);
					$deltas[] = $delta;
					$aspectos_discursivos_string .= $area_delta[$delta].$subtema[$delta].$matriz[$delta].$texto_argumentativo[$delta].$actor[$delta];
					$delta++;
				}
				$delta_mitlan = 0;
				unset($subtema_mitlan,$matriz_mitlan,$texto_argumentativo_mitlan,$actor_mitlan,$deltas_mitlan);
				$inicio = microtime(true);
				$aspectos_discursivos_mitlan = buscar_aspectos_discursivos_mitlan($seganaid, $array_aspectos_discursivos_nuevo);
				$total = microtime(true)-$inicio;
				print "Buscar aspectos_discursivos_mitlan: ".$total."\n";
				$aspectos_discursivos_string_mitlan = '';
				foreach ($aspectos_discursivos_mitlan  as $row_aspectos_discursivos_mitlan) {
					$area_mitlan[] = $row_aspectos_discursivos_mitlan->area;
					$subtema_mitlan[] = $row_aspectos_discursivos_mitlan->subtema;
					$matriz_mitlan[] = $row_aspectos_discursivos_mitlan->matriz;
					$texto_argumentativo_mitlan[] = mysql_real_escape_string($row_aspectos_discursivos_mitlan->texto_argumentativo);
					$actor_mitlan[] = $row_aspectos_discursivos_mitlan->argumentador;
					$deltas_mitlan[] = $delta_mitlan;
					$aspectos_discursivos_string_mitlan .= $area_mitlan[$delta_mitlan].$subtema_mitlan[$delta_mitlan].$matriz_mitlan[$delta_mitlan].$texto_argumentativo_mitlan[$delta_mitlan].$actor_mitlan[$delta_mitlan];
					$delta_mitlan++;
				}

				print "nid segananew". $nid ."\n";
				print "segana ".md5($aspectos_discursivos_string)."\n";
				print "segananew ".md5($aspectos_discursivos_string_mitlan)."\n";
				print "seganaid: ".$seganaid." nid: ".$nid."\n"; 
				if (md5($aspectos_discursivos_string) != md5($aspectos_discursivos_string_mitlan)){
					$conexion_mitlan->query("START TRANSACTION");
					$query_delete = delete_content_field($nid,'area',$conexion_mitlan);
					$query_delete .= delete_content_field($nid,'subtema',$conexion_mitlan);
					$query_delete .= delete_content_field($nid,'matriz',$conexion_mitlan);
					$query_delete .= delete_content_field($nid,'argumentador',$conexion_mitlan);
					$query_delete .= delete_content_field($nid,'texto_argumentativo',$conexion_mitlan);
					$query_delete .= delete_term_node($nid, $conexion_mitlan);
					print $query_delete;
					$area = traducir_array($row_nota->id_area, $traducir['area']);
					if (isset($deltas)) {
						foreach ($deltas as $delta) {
							$vid = $nid;
							$query_content_field = insertar_content_field($nid,$vid,$subtema[$delta],'subtema',$conexion_mitlan,$delta);
							$query_content_field .= insertar_content_field($nid,$vid,$matriz[$delta],'matriz',$conexion_mitlan,$delta);
							$query_content_field .= insertar_content_field($nid,$vid,"'$texto_argumentativo[$delta]'",'texto_argumentativo',$conexion_mitlan,$delta);
							$query_content_field .= insertar_content_field($nid,$vid,$actor[$delta],'argumentador',$conexion_mitlan,$delta);
							$query_content_field .= insertar_content_field($nid,$vid,$area,'area',$conexion_mitlan,$delta);

							$query_term_node = insertar_term_node($nid,$vid,$subtema[$delta],$conexion_mitlan);
							$query_term_node .= insertar_term_node($nid,$vid,$matriz[$delta],$conexion_mitlan);
							$query_term_node .= insertar_term_node($nid,$vid,$actor[$delta],$conexion_mitlan);
							$query_term_node .= insertar_term_node($nid,$vid,$area,$conexion_mitlan);
							print $query_content_field;
							print $query_term_node."\n\n";
						}
					}
					$conexion_mitlan->query("COMMIT");
				}
			}
		}
	}
}




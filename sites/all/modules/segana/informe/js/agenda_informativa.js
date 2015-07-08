(function ($) {

	$("div#temas").livequery(function() {
		 $('input.tema').bind('focus', function () {
		 	var id_tema_old = $("div#temas").find('input.tema.tema_seleccionado').parent().parent().parent().parent().attr('id')
		 	$("div#temas").find('input.tema').removeClass('tema_seleccionado');
		 	$(this).addClass('tema_seleccionado');
		 	var id_tema = $(this).parent().parent().parent().parent().attr('id');
		 	var tema_seleccionado = id_tema.match(/\d+/)[0];
		 	$('div#tema_seleccionado input').val(tema_seleccionado);
		 	// limpiar la busqueda si se selecciona otro tema
		 	if (id_tema != id_tema_old){
		 		$('div#busqueda .fieldset-wrapper').empty();
		 	}
		 });
	});


  Drupal.ajax.prototype.commands.afterAjaxCallbackAgendaInformativa = function(ajax, response, status) {
    $.colorbox({
    	iframe:true,
    	href:"/agenda_informativa/preview/" +response.selectedValue,
    	"width": "645px",
    	"height": function(){return $(window).height() -50;},
    	"innerHeight": "838px",
    	"transition": "elastic",
    	"speed": "350",
    	"opacity": "0.90",
    	maxHeight : "880px",
		});

  };

})(jQuery);
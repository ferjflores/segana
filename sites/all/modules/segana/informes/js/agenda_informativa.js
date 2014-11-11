(function ($) {

	$("div#temas").livequery(function() {
		 $('input.tema').bind('focus', function () {
		 	$("div#temas").find('input.tema').removeClass('tema_seleccionado');
		 	$(this).addClass('tema_seleccionado');
		 	var id_tema = $(this).parent().parent().parent().parent().attr('id');
		 	var tema_seleccionado = id_tema.match(/\d+/)[0];
		 	$('div#tema_seleccionado input').val(tema_seleccionado);
		 });
	});

})(jQuery);
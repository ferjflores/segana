(function ($) {

	Drupal.ajax.prototype.commands.afterAjaxCallbackAgendaInformativaPreview = function(ajax, response, status) {
		$('textarea.resumen').each(function(){
			$(this).simplyCountable({
					counter: '#counter-' + this.id,
					maxCount: 900,
					thousandSeparator: '.',
					onOverCount: function(count, countable, counter){
						$('textarea#' + countable[0].id).addClass('over');
					},
					onSafeCount:        function(count, countable, counter){
						$('textarea#' + countable[0].id).removeClass('over');
					},
			});

			//Expanding Textareas jQuery Plugin https://github.com/bgrins/ExpandingTextareas
			$(this).expanding();
		});		
	};

	Drupal.ajax.prototype.commands.afterAjaxCallbackAgendaInformativaPreviewGuardar = function(ajax, response, status) {
		if (response.selectedValue == 1) {
			$('div#estado').text('Infome salvado').css("color",'green').fadeTo( "slow" , 1);
		}
		else if (response.selectedValue == -1) {
			$('div#estado').text('El campo resumen no puede ser mayor a 900 caracteres').css("color",'red').fadeTo( "slow" , 1);
		}
		else if (response.selectedValue == 0) {
			$('div#estado').text('Ha ocurrido un error al salvar').css("color",'red').fadeTo( "slow" , 1);
		}
	};
})(jQuery);
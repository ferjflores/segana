/**
 * @file
 * JavaScript integration between Highcharts and Drupal.
 */
(function ($) {

Drupal.behaviors.chartsHighcharts = {};
Drupal.behaviors.chartsHighcharts.attach = function(context, settings) {
  $('.charts-highchart').once('charts-highchart', function() {
    if ($(this).attr('data-chart')) {
      var config = $.parseJSON($(this).attr('data-chart'));
      Highcharts.setOptions({
		lang: {
			months: ['Enero', 'Febrero', 'Marzo', 'Abril', 'Mayo', 'Junio', 'Julio', 'Agosto', 'Septiembre', 'Octubre', 'Noviembre', 'Diciembre'],
			shortMonths: ['Ene', 'Feb', 'Mar', 'Abr', 'May', 'Jun', 'Jul', 'Ago', 'Sep', 'Oct', 'Nov', 'Dic'],
		    weekdays: ['Domingo', 'Lunes', 'Martes', 'Miércoles', 'Jueves', 'Viernes', 'Sabado'],
		    exportButtonTitle: "Exportar",
        printButtonTitle: "Imprimir",
        printChart: "Imprimir gráfico",
        rangeSelectorFrom: "De",
        rangeSelectorTo: "Hasta",
        rangeSelectorZoom: "Periodo",
        downloadPNG: 'Descargar imagen PNG',
        downloadJPEG: 'Descargar imagen JPEG',
        downloadPDF: 'Descargar documento PDF',
        downloadSVG: 'Descargar imagen SVG',
        numericSymbols: null,
        resetZoom: 'Reiniciar zoom',
        decimalPoint: ',',
        thousandsSep: '.',
		} });
      $(this).highcharts(config);
    }
  });
};

})(jQuery);

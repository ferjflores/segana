(function ($) {
    Drupal.behaviors.distribucionNotas = {
      attach: function (context, settings) {
          $('[id^="distribucion-notas-form"] table thead input').change(function() {
              var checkboxes = $('[id^="distribucion-notas-form"] table tbody').find('input[type="checkbox"]');
              var checkbox_header = $('div.fixedHeader table thead').find('input[type="checkbox"]');
              if($(this).is(':checked')) {
                  checkboxes.prop('checked', true);
                  checkbox_header.prop('checked', true);
              }
              else {
                  checkboxes.prop('checked', false);
                  checkbox_header.prop('checked', false);
              }
          });
          /*$('div.encabezado.tabla').once().children('div').each(function(){
              $(this).click(function(){
                  var elemento = $(this).text().toLowerCase();
                  valores_html = $('div#busqueda').find('div.titulo_nota div.' + elemento).toArray();
                  if (valores_html !== 0) {
                      var valores = [];
                      for (i = 0; i < valores_html.length; i++) {
                          valor =valores_html[i].innerText;
                          if (elemento == 'fecha' ) {
                              valor = moment(valor, 'DD/MM/YYYY h:mm a').unix();
                          }
                          valores.push(valor);
                      }
                      console.log(valores[0]);
                      valores.sort(function(a, b) {
                          return a>b ? -1 : a<b ? 1 : 0;
                      });
                      console.log(valores[0]);
                      console.log(valores);
                  }
              })
          });*/
      }
    };

    Drupal.ajax.prototype.commands.afterAjaxCallbackDialogoSeleccionarAnalista = function(ajax, response, status) {
        $("#asignacion").show();
        $("#colorbox, #cboxOverlay").appendTo('form:first');
        $.colorbox({
            inline:true,
            href:"#asignacion",
            "width": "265px",
            "height": "370px",
            "innerHeight": "348px",
            "transition": "elastic",
            "speed": "150",
            "opacity": "0.90",
            "maxHeight": "370px",
            "fixed": true,
            onClosed: function(){
                $("#asignacion").hide();
            },
            scrolling: false
            /*onComplete : function() {
                $(this).colorbox.resize();
            },*/
        });
    };

    Drupal.ajax.prototype.commands.afterAjaxCallbackConfirmarNotas = function(ajax, response, status) {
        $.colorbox.close();
        $("#asignacion").hide();
        $('[id^="distribucion-notas-form"] input.marcar_todo').prop('checked', false);
    };

    Drupal.ajax.prototype.commands.afterAjaxCallbackBuscar = function(ajax, response, status) {
        $.colorbox.close();
        $("#asignacion").hide();
    };


})(jQuery);
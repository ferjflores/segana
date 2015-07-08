(function ($) {
    $( "textarea[id^='edit-body']" ).livequery(function() {
            $( this ).expanding();
    });
    $( "textarea[id^='edit-sumario']" ).livequery(function() {
        $( this ).expanding();
    });
    $( "textarea[id^='edit-admin-canal-seccion']" ).livequery(function() {
        $( this ).expanding();
    });
    $( "textarea[id^='edit-admin-canal-rss']" ).livequery(function() {
        $( this ).expanding();
    });

    Drupal.ajax.prototype.commands.afterAjaxCallbackBorrarCerrar = function(ajax, response, status) {
       setTimeout(function() {
         window.close();
       }, 5000);
    }

    Drupal.ajax.prototype.commands.afterAjaxCallbackMedioAdmin = function(ajax, response, status) {
        $.colorbox({
            iframe:true,
            href:"/medio/" +response.selectedTipo+ "/" +response.selectedValue,
            "width": "745px",
            "height": function(){return $(window).height();},
            "innerHeight": function(){return $(window).height() -30;},
            "transition": "elastic",
            "speed": "350",
            "opacity": "0.90",
            maxHeight :function(){return $(window).height() -50;},
        });

    };

})(jQuery);
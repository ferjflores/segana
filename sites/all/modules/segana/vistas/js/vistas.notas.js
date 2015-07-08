(function ($) {
  Drupal.behaviors.vistasNotas = {
    attach: function (context, settings) {
      $('th.fecha input').once(function(){
        $(this).val($.datepicker.formatDate('dd-mm-yy', new Date()));
      });
    }
  };
})(jQuery);
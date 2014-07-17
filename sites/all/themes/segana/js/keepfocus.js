	/* =================================================================
	*  Keep focus on auto-submitted text-fields without surrounding block
	*  ============================================================== */
function SetCaretAtEnd(elem) {
  var elemLen = elem.value.length;
  // For IE Only
  if (document.selection) {
      // Set focus
      elem.focus();
      // Use IE Ranges
      var oSel = document.selection.createRange();
      // Reset position to 0 & then set at end
      oSel.moveStart('character', -elemLen);
      oSel.moveStart('character', elemLen);
      oSel.moveEnd('character', 0);
      oSel.select();
  }
  else if (elem.selectionStart || elem.selectionStart == '0') {
      // Firefox/Chrome
      elem.selectionStart = elemLen;
      elem.selectionEnd = elemLen;
      elem.focus();
  } // if
} // SetCaretAtEnd()
var textboxToFocus = {};
jQuery(function($) {
  var addFocusReminder = function(textbox) {
    textbox.bind('keypress keyup', function(e) {
      textboxToFocus.formid = $(this).closest('form').attr('id');
      textboxToFocus.name = $(this).attr('name');
      if(e.type == 'keypress') {
        if(e.keyCode != 8) { // everything except return
          textboxToFocus.value = $(this).val() + String.fromCharCode(e.charCode);
        } else {
          textboxToFocus.value = $(this).val().substr(0, $(this).val().length-1)
        }
      }
      else { // keyup
        textboxToFocus.value = $(this).val();
      }
    });
  }
  addFocusReminder($('.view-filters input:text.ctools-auto-submit-processed'));
  $(document).ajaxComplete(function(event,request, settings) {
    if(typeof textboxToFocus.formid !== 'undefined') {
      var textBox = $('#' + textboxToFocus.formid + ' input:text[name="' + textboxToFocus.name + '"]');
      textBox.val(textboxToFocus.value);
      SetCaretAtEnd(textBox[0]);
      addFocusReminder(textBox);
      //textboxToFocus = {}; // if you have other auto-submitted inputs as well
    }
  });
});

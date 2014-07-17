(function ($) {
	$("div.throbber").livequery(function() {
			$('div.throbber').attr('id', 'throbber');

			var throb = Throbber({
				color: '#505E67',
				padding: 55,
				size: 100,
				fade: 200,
				lines: 25,
				strokewidth: 5,
				rotationspeed: 10,
				//clockwise: false
			}).appendTo( document.getElementById( 'throbber' ) ).start();
	});
})(jQuery);
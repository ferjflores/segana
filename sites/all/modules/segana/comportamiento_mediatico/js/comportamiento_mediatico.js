(function ($) {
	$("div#graficos").livequery(function() {
    jQuery("#tabs_graficos").tabs();
 	});
	
	$("div#resumen").livequery(function() {
			var throb = Throbber({
				color: '#D3D9DD',
				padding: 75,
				size: 200,
				fade: 200,
				lines: 25,
				strokewidth: 5,
				rotationspeed: 10,
				//clockwise: false
			}).appendTo( document.getElementById( 'resumen' ) ).start();
	});


	$("div#resumen").livequery(function() {
		var variables = $('input[name=variables]').attr("value");
    	/*jQuery.get('/comportamiento_mediatico/resultado/' + variables, null, function(data) {
    		$("div#resumen").append(data);
		}, 'html');*/
    	$("div#resumen").load('/comportamiento_mediatico/resultado/' + variables + " #accordion");
		
	});

	$("div#accordion").livequery(function() {
    
    $( "#accordion" ).accordion({
    		heightStyle: "content",
    		icons: null,
    		//animate: false,
  	});


    $("div#valores:visible").once(function() {

      $(this).mCustomScrollbar({
      	theme: "dark",
	      scrollButtons:{enable: 1},
	      horizontalScroll: 0,
	      mouseWheel: 1,
	      scrollInertia: "550",
	      autoDraggerLength: false,
	      contentTouchScroll: 0,
	      autoHideScrollbar: 0,
	      advanced:{
		      updateOnContentResize: 1,
		      updateOnBrowserResize: 1,
		      autoScrollOnFocus: 0
	    	},
	    })	
    });
	  $("div#resumen_notas:visible").mCustomScrollbar({
			theme: "dark",
			scrollButtons:{enable: 1},
			horizontalScroll: 0,
			mouseWheel: 1,
			scrollInertia: "550",
			autoDraggerLength: false,
			contentTouchScroll: 0,
			autoHideScrollbar: 0,
			advanced:{
			  updateOnContentResize: 1,
			  updateOnBrowserResize: 1,
			  autoScrollOnFocus: 0
			},
		});

	  //$(".colorbox-notas").colorbox({"iframe": true, "width": "600px", "height": function(){return $(window).height();}, "innerHeight": "700px", "transition": "elastic", "speed": "350", "opacity": "0.90"});
	  $(".colorbox-notas").colorbox({html: function(){
	  		var url = $(this).attr('href');
	  		$.get(url, function (data) {
    			return data;
			});
	  	},
	  	"width": "750px", "height": function(){return $(window).height() -100;}, "innerHeight": "700px", "transition": "elastic", "speed": "350", "opacity": "0.90"
	  });
		


		$("div#accordion h3").click(function() {

			$(this).next().children("div#valores").once(function() {
				$(this).mCustomScrollbar({
			      	theme: "dark",
				      scrollButtons:{enable: 1},
				      horizontalScroll: 0,
				      mouseWheel: 1,
				      scrollInertia: "550",
				      autoDraggerLength: false,
				      contentTouchScroll: 0,
				      autoHideScrollbar: 0,
				      advanced:{
					      updateOnContentResize: 1,
					      updateOnBrowserResize: 1,
					      autoScrollOnFocus: 0
				    	},
		    	})	
			});

			var $this = $(this);
			setTimeout(function() {
				var id_activo = $this.next().children("div#valores").find("span.activo").attr("id");
			
				$this.next().children("div#valores").mCustomScrollbar("scrollTo", "#" + id_activo);
			}, 500);
			

			$(this).next().children("div#resumen_notas").mCustomScrollbar({
		      	theme: "dark",
			      scrollButtons:{enable: 1},
			      horizontalScroll: 0,
			      mouseWheel: 1,
			      scrollInertia: "550",
			      autoDraggerLength: false,
			      contentTouchScroll: 0,
			      autoHideScrollbar: 0,
			      advanced:{
				      updateOnContentResize: 1,
				      updateOnBrowserResize: 1,
				      autoScrollOnFocus: 0
			    	},	
			});
		});

		$("div#valores span").click(function() {
			$(this).parents("div#valores").find("span").removeClass('activo')
			$(this).addClass('activo');
			var tid = $(this).attr("id");
			var variables = $('input[name=variables]').attr("value");
			var resumen_notas = $(this).parents("div#valores").next("div#resumen_notas");
			var contenedor_resumen_notas = $(resumen_notas).find("div.mCSB_container");

			$(resumen_notas).mCustomScrollbar("scrollTo","top");
			$(contenedor_resumen_notas).load('/comportamiento_mediatico/notas/' + variables + "/" + tid, function(){
				$(".colorbox-notas").colorbox({html: function(){
				  		var url = $(this).attr('href');
				  		$.get(url, function (data) {
			    			return data;
						});
			  		},
			  		"width": "750px", "height": function(){return $(window).height() -100;}, "innerHeight": "700px", "transition": "elastic", "speed": "350", "opacity": "0.90"
			  	});
			});
			

		});

	});

	$("div#cboxLoadedContent").livequery(function(){
		$(this).mCustomScrollbar({
	      	theme: "dark",
		      scrollButtons:{enable: 1},
		      horizontalScroll: 0,
		      mouseWheel: 1,
		      scrollInertia: "550",
		      autoDraggerLength: true,
		      contentTouchScroll: 0,
		      autoHideScrollbar: 0,
		      advanced:{
			      updateOnContentResize: 1,
			      updateOnBrowserResize: 1,
			      autoScrollOnFocus: 0
		    	},
		});
	});

})(jQuery);

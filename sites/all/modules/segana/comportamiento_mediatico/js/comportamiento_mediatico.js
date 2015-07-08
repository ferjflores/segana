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
			$(this).parents("div#valores").find("span").removeClass('activo');
			$(this).addClass('activo');
			var tid = $(this).attr("id");
			var tipo = $(this).children('.derecha').attr("id");
			var variables = $('input[name=variables]').attr("value");
			var resumen_notas = $(this).parents("div#valores").next("div#resumen_notas");
			var contenedor_resumen_notas = $(resumen_notas).find("div.mCSB_container");

			$(resumen_notas).mCustomScrollbar("scrollTo","top");
			$(contenedor_resumen_notas).load('/comportamiento_mediatico/notas/' + variables + "/" + tid + "/" + tipo, function(){
				$(".colorbox-notas").colorbox({
					html: function(){
						var url = $(this).attr('href');
						$.get(url, function (data) {
							return data;
						});
					},
					"width": "750px", "height": function(){return $(window).height() -100;}, "innerHeight": "700px", "transition": "elastic", "speed": "350", "opacity": "0.90"
				});
			});
		});

		$("span.expandir").click(function() {
			var tipo = $(this).attr("id");
			var variables = $('input[name=variables]').attr("value");
			$.colorbox({href:"/comportamiento_mediatico/resultado_expandido/" + variables + "/" + tipo, width: "1000px", height: function(){return $(window).height() -100;}, innerHeight: "950px", transition: "elastic", speed: "350", opacity: "0.90", open: true,
				onComplete:function(){
					switch (tipo) {
					    case 'area':
					    	var index = 0;
					    	break;
					    case 'tema':
					    	var index = 1;
					    	break;
					    case 'matriz':
					    	var index = 2;
					    	break;
					    case 'actor':
					    	var index = 3;
					    	break;
					    case 'medio':
					    	var index = 4;
					    	break;
						case 'usuario':
							var index = 5;
							break;
						case 'tendencia_nota':
							var index = 6;
							break;
					}
					jQuery("#tabs_resumen_expandido").tabs({ 
						active: index,
						load: function( event, ui){
							
							// se debe aplicar el datatables uno por uno a cada id https://www.datatables.net/forums/discussion/3963/tabletools-on-multiple-tables
							var index = ui.tab.index();
							switch (index){
								case 0:
									var tipo = 'area';
									break;
								case 1:
									var tipo = 'tema';
									break;
								case 2:
									var tipo = 'matriz';
									break;
								case 3:
									var tipo = 'actor';
									break;
								case 4:
									var tipo = 'medio';
									break;
								case 5:
									var tipo = 'usuario';
									break;
								case 5:
									var tipo = 'medio';
									break;
							}

							$('table#' + tipo).once(function(){
								$(this).dataTable({
									dom: 'T<"clear">lfrtip',
									tableTools: {
										"sSwfPath": "/sites/all/libraries/datatables/extensions/TableTools/swf/copy_csv_xls_pdf.swf",
										"aButtons": [
											"copy",
											{
		                    "sExtends": "xls",
		                    "sTitle": "Comportamiento mediatico - " + tipo
			                },
											{
		                    "sExtends": "pdf",
		                    "sTitle": "Comportamiento mediatico - " + tipo
			                }
										]
									},
									"order": [[ 0, "desc" ]],
									"lengthMenu": [ [15, 25, 50, -1], [15, 25, 50, "All"] ],
							        "oLanguage": {
								      "oPaginate": {
								      	"sPrevious": "Anterior",
								        "sNext": "Siguiente"
								      },
								      "sLengthMenu": "Mostrar _MENU_ registros por página",
								      "sZeroRecords": "No se encontraron resultados",
								      "sInfo": "Mostrando página _PAGE_ de _PAGES_ (_MAX_ registros)",
							          "sInfoEmpty": "No hay registros disponibles",
							          "sInfoFiltered": "(filtrado de _MAX_ total registros)",
							          "sSearch": "Buscar",
								    },
								});
							});
						},
					});
				},
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
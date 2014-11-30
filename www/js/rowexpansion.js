$(document).ready(function(){
				 $('.one').hide();
		
				$('.main').click(function(){
				$var=$(this).next();
				$var1 = $(this);
				while($var.hasClass('one')==true){
				$var.toggle();
				if($var1.hasClass('main')==true)
				{
					$(this).find('.arrow').toggleClass('up');
				}
				$var=$var.next();
				$var1=$var1.next();
				
				}
			/*	while($(this).className)
						/*var row = $(this).closest('tr');
						var next = row.next();
						document.write(row);
						$(this).next('tr').toggle();
						$(this).next('tr').next('tr').toggle();
						$(this).find('.arrow').toggleClass('up');*/
						//$('#report').jExpand();
						//$('.one').show();
					});
					
				});
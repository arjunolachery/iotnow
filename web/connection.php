<?php
//database credentials
session_start();
if($_SESSION['userId']==""){
    header('location: ../index.html');
}
$servername = "localhost";
$dbname = "id16978089_iotnow";
$username = "root";
$password = '';

$conn = new mysqli($servername, $username, $password, $dbname); // create mysqli connection
if ($conn->connect_error) { // check whether connection successful
    die("Connection failed: " . $conn->connect_error); 
}
?>
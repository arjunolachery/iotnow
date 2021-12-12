<?php
//database credentials
session_start();
if(!isset($_SESSION['userId'])){
    header('location: ../index.html');
}
$servername = "localhost";
$dbname = "id16978089_iotnow";
$username = "id16978089_iotnowdb";
$password = 'xFzN%Gm8X$x4Ds$D';


$conn = new mysqli($servername, $username, $password, $dbname); // create mysqli connection
if ($conn->connect_error) { // check whether connection successful
    die("Connection failed: " . $conn->connect_error); 
} 
?>
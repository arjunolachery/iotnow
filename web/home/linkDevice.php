<?php
include "../connection.php";

// initializing variables
$errors = array(); 

// REGISTER USER
if (isset($_POST['macAddress'])) {
  // receive all input values from the form
  $email = mysqli_real_escape_string($conn, $_SESSION['email']);
  $macAddress = mysqli_real_escape_string($conn, $_POST['macAddress']);
//   $macAddress = $_POST['macAddress'];

  //get user id
  $user_check_query = "SELECT * FROM users WHERE email='$email' LIMIT 1";
  $result = mysqli_query($conn, $user_check_query);
  $user = mysqli_fetch_assoc($result);
  $userId = $user['userId'];
  // first check the database to make sure 
  // a user does not already exist with the same username and/or email

  //get deviceid
  $user_check_query = "SELECT * FROM devices WHERE MACAddress='$macAddress' LIMIT 1";
  $result = mysqli_query($conn, $user_check_query);
  $user = mysqli_fetch_assoc($result);
  $deviceId = $user['deviceId'];
  $deviceUserId = $user['userId'];


  $user_check_query = "UPDATE devices SET userId='$userId' WHERE MACAddress='$macAddress'";
//   $user_check_query = "SELECT * FROM devices WHERE MACAddress='$macAddress' LIMIT 1";
if (mysqli_query($conn, $user_check_query)) {
    if($deviceId !=""){
        if($deviceUserId == $userId){
            echo "Device with MAC Address '".$macAddress."' was already linked before. Try delinking if you wish to add this device to another account.";
        }
        else{
            echo "Device with MAC Address '".$macAddress."' has been successfully linked.";
        }
    }
    else
    {
    echo "Device with MAC Address '".$macAddress."' was not found. Try setting up a nearby personal hotspot with ssid='iotnow' and password='iotnow' and turn on the device.";
    }
} else {
    echo "Error updating record: " . mysqli_error($conn);
  }
//   $userId = $user['userId'];
}

?>
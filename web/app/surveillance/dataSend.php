<?php
include '../../connectionApi.php';
date_default_timezone_set('Asia/Kolkata');

if ($_SERVER["REQUEST_METHOD"] == "POST") {
        $MACAddress = process_input($_POST["MACAddress"]);
        //check if device already exists
        $user_check_query = "SELECT * FROM devices WHERE MACAddress='$MACAddress' LIMIT 1";
        $result = mysqli_query($conn, $user_check_query);
        $user = mysqli_fetch_assoc($result);
        $deviceId = $user['deviceId'];
        $userId = $user['userId'];
        $objectName = process_input($_POST["objectName"]);
        $probability = process_input($_POST["probability"]);
        $time = time();
        echo "Intrustion Detected! Object Name - ".$objectName.", Probability - ".$probability.", Date/Time - ".date('m/d/Y H:i:s',$time);

        if($deviceId !=""){
            $sqlQuery = "INSERT INTO appSurveillance (userId, MACAddress, deviceId, objectName, probability, Time)
            VALUES ('" . $userId . "','" . $MACAddress . "','" . $deviceId . "', '" . $objectName . "', '" . $probability . "', '" . $time . "')";
        
        if ($conn->query($sqlQuery) === TRUE) {
            echo "Intrusion details sent to server";
        } 
        else {
            echo "Error, couldn't send data";
        }
        }
        else{
            echo "This device has not yet been registered with iotnow.";
        }
}
else {
    echo "Data not received";
}

function process_input($input) {
    $input = trim($input);
    $input = stripslashes($input);
    $input = htmlspecialchars($input);
    return $input;
}
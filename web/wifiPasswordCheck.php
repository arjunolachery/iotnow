<?php
include 'connectionApi.php';
if ($_SERVER["REQUEST_METHOD"] == "POST") {
        // $MCUId = process_input($_POST["MCUId"]);
        // echo $MCUId;
        // $userId = 0;
        $macAddress = process_input($_POST["macAddress"]);
        // $deviceName = "iotnow node";
        // $deviceLocation = "lab";
        // $wifiSsid = "iotnow";
        // $wifiPassword = "iotnowpass";
        // $autoUpdate = 1;
        // $programChoice = process_input($_POST["programId"]);
        // $versionChoice = process_input($_POST["versionId"]);
        // $ipAddress =$_SERVER['REMOTE_ADDR'];
        // $lastActive = time();


        //check if device already exists
        $user_check_query = "SELECT * FROM devices WHERE MACAddress='$macAddress' LIMIT 1";
        $result = mysqli_query($conn, $user_check_query);
        $user = mysqli_fetch_assoc($result);
        $wifiPassword = $user['wifiPassword'];

        echo $wifiPassword;
        $conn->close();
}
else {
    // echo "Data not received";
    echo "";
}

function process_input($input) {
    $input = trim($input);
    $input = stripslashes($input);
    $input = htmlspecialchars($input);
    return $input;
}

<?php
include "../connection.php";
date_default_timezone_set('Asia/Kolkata');
?>
<html>
<head>
    <link rel="preconnect" href="https://fonts.gstatic.com">
    <link href="https://fonts.googleapis.com/css2?family=Maven+Pro:wght@500&display=swap" rel="stylesheet"> 
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css" integrity="sha384-MCw98/SFnGE8fJT3GXwEOngsV7Zt27NXFoaoApmYm81iuXoPkFOJwJ8ERdknLPMO" crossorigin="anonymous"> 
    <script src="https://code.jquery.com/jquery-3.3.1.slim.min.js" integrity="sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo" crossorigin="anonymous"></script>
    <script src="https://code.jquery.com/jquery-1.10.2.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.3/umd/popper.min.js" integrity="sha384-ZMP7rVo3mIykV+2+9J3UJ46jBk0WLaUAdn689aCwoqbBJiSnjAK/l8WvCWPIPm49" crossorigin="anonymous"></script>
<script src="https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/js/bootstrap.min.js" integrity="sha384-ChfqqxuZUCnJSK3+MXmPNIyE6ZbWh2IMqE241rYiqJxyMiZ6OW/JmZQ5stwEULTy" crossorigin="anonymous"></script>
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/css/bootstrap.min.css">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/js/bootstrap.min.js"></script>
<style type="text/css">
body{
    padding:1em;
    /* margin:1em; */
    background-color: #f7f9fb;
	  font-size: 14px;
    
}
#dataUpdatedSuccess{
  background-color: green;
  color:white;
  padding:1em;
}
</style>
</head>  
<body>
<div id="dataUpdatedSuccess" style="position:absolute;top:0;right:0">&#9989;&nbsp;Data Updated</div>
<br>
<div class="container">
<div class="row">
<div class="col-1"></div>
<div class="col-10">
<center><table><tr style="vertical-align: center;"><td><img src="../logo.png"></td><td style="vertical-align: center;"><table style="padding: 0.5em;"><tr><td style="font-family: 'Maven Pro', sans-serif; font-size:3em">iotnow
</td></tr><tr><td style="font-family: 'Maven Pro', sans-serif; font-size:1em; font-style: italic; color:dark grey">Empowering people through trusted networks</td></tr></table></td></tr></table>
</center>
</div>
<div class="col-1" style="text-align:right">
<br>
<button  class="btn btn-default btn-sm" onclick="window.location.href='../logout.php'">
  <span class="glyphicon glyphicon-log-out"></span> Log out
</button>
</div>

</div>
</div>
<?php
//get MCU Name
$userId=$_SESSION['userId'];
$user_check_query = "SELECT * FROM users WHERE userId='$userId' LIMIT 1";
$result = mysqli_query($conn, $user_check_query);
$user = mysqli_fetch_assoc($result);
$userName = $user['name'];
?>

<br><h4><?php echo $userName."'s"?> Devices &nbsp;&nbsp;<button id="toggleMCUDetails" class="btn btn-info btn-link" type="button">Show MCU Details</button></h4>

<script>



$(document).ready(function () {
  $("#submitMac").click(function() {
    var data = {
    macAddress: document.getElementById('macAddress').value
    }
    $.post('linkDevice.php', data).done(function(response){
      alert(response);
    });
  });
  $(".cpuDetail").hide();
  $("#toggleMCUDetails").click(function() {
    $(".cpuDetail").toggle();

    if ($("#toggleMCUDetails").html() == "Show MCU Details") {
      $("#toggleMCUDetails").html("Hide MCU Details");
    }
    else {
      $("#toggleMCUDetails").html("Show MCU Details");
    }
  });

});

</script>


<table class="table table-sm table-hover .table-striped">
  <thead class="thead-light">
    <tr>
      <th scope="col">MAC Address</th>
      <th scope="col">Device Name &#9998;</th>
      <th scope="col">Device Location &#9998;</th>
      <th scope="col" class="cpuDetail">MCU Name</th>
      <th scope="col">programId &#9998;</th>
      <th scope="col">versionId &#9998;</th>
      <th scope="col" class="cpuDetail">IP Address</th>
      <th scope="col" class="cpuDetail">Last Activated</th>
      <th scope="col">Wifi SSID &#9998;</th>
      <th scope="col">Wifi Password &#9998;</th>
      <th scope="col">Auto Update &#9998;</th>
      <th scope="col" class="cpuDetail">CPU</th>
      <th scope="col" class="cpuDetail">Clock Frequency (Hz)</th>
      <th scope="col" class="cpuDetail">ROM (MB)</th>
      <th scope="col" class="cpuDetail">RAM (kB)</th>
    </tr>
  </thead>
  <tbody>
    
  
<?php
//database credentials

// $userId = $_SESSION["userId"];


$sql = "SELECT * FROM devices WHERE userId ='$userId'"; 

$result = $conn->query($sql);
// $result = mysqli_query($conn, $sql);

if ($result = $conn->query($sql)) {

    while ($row = $result->fetch_assoc()) {

      $MACAddress = $row["MACAddress"];
      $deviceName = $row["deviceName"];
      $deviceLocation = $row["deviceLocation"];

      $MCUId = $row["MCUId"];



      //get MCU Name
      $user_check_query = "SELECT * FROM MCU WHERE MCUId='$MCUId' LIMIT 1";
      $result2 = mysqli_query($conn, $user_check_query);
      $user = mysqli_fetch_assoc($result2);
      $MCUName = $user['name'];

      $deviceId = $row["deviceId"];
      
      $programChoice = $row["programChoice"];
      $versionChoice = $row["versionChoice"];
      $ipAddress = $row["ipAddress"];
      $lastActive = $row["lastActive"];

      $wifiSsid = $row["wifiSsid"];
      $wifiPassword = $row["wifiPassword"];
      $autoUpdate = $row["autoUpdate"];

      $MCUCPU = $user['CPU'];
      $MCUClockFrequency = $user['clockFrequency'];
      $MCUFlashMemory = $user['flashMemory'];
      $MCURAM = $user['RAM'];
      
        // $deviceName = $row["deviceName"];
        // $deviceLocation = $row["deviceLocation"];
        // $programChoice = $row["programChoice"];
        // $versionChoice = $row["versionChoice"];
        // $MACAddress = $row["MACAddress"]; 
        // $timestamp = $row["timestamp"]; 
        // $ipAddress = $row["ipAddress"];
        // $deviceId = $row["deviceId"];
      
        echo '<tr> 
                <td>' . $MACAddress . '</td> 
                <td class="deviceName" data-id1="'.$deviceId.'" contenteditable>' . $deviceName . '</td> 
                <td class="deviceLocation" data-id1="'.$deviceId.'" contenteditable>' . $deviceLocation . '</td>
                <td class="cpuDetail">' . $MCUName . '</td>  
                <td class="programChoice" data-id1="'.$deviceId.'" contenteditable>'. $programChoice . '</td> 
                <td class="versionChoice" data-id1="'.$deviceId.'" contenteditable>' . $versionChoice . '</td>
                <td class="cpuDetail">' . $ipAddress . '</td> 
                <td class="cpuDetail">' . date('m/d/Y H:i:s',$lastActive). '</td> 
                <td class="wifiSsid" data-id1="'.$deviceId.'" contenteditable>' . $wifiSsid . '</td> 
                <td class="wifiPassword" data-id1="'.$deviceId.'" contenteditable>' . $wifiPassword . '</td> 
                <td class="autoUpdate" data-id1="'.$deviceId.'" contenteditable>' . $autoUpdate . '</td> 
                <td class="cpuDetail">' . $MCUCPU . '</td> 
                <td class="cpuDetail">' . $MCUClockFrequency . '</td> 
                <td class="cpuDetail">' . $MCUFlashMemory . '</td> 
                <td class="cpuDetail">' . $MCURAM . '</td> 
              </tr>';
    }
    $result->free();
}
?>
</tbody>
</table>

<script>
$(document).ready(function () {
$("#dataUpdatedSuccess").hide();
function edit_data(id, text, column_name)  
    {  
        $.ajax({  
            url:"edit.php",  
            method:"POST",  
            data:{id:id, text:text, column_name:column_name},  
            dataType:"text",  
            success:function(data){  
                //alert(data);
            $("#dataUpdatedSuccess").fadeIn("slow").delay(2000).fadeOut("slow");
            }  
        });  
    }  
    $(document).on('blur', '.deviceName', function(){  
        var id = $(this).data("id1");  
        var val = $(this).text();  
        edit_data(id, val, "deviceName");  
        // alert('update');
    });  
    $(document).on('blur', '.deviceLocation', function(){  
        var id = $(this).data("id1");  
        var val = $(this).text();  
        edit_data(id, val, "deviceLocation");  
        // alert('update');
    });
    $(document).on('blur', '.programChoice', function(){  
        var id = $(this).data("id1");  
        var val = $(this).text();  
        edit_data(id, val, "programChoice");  
        // alert('update');
    });  
    $(document).on('blur', '.versionChoice', function(){  
        var id = $(this).data("id1");  
        var val = $(this).text();  
        edit_data(id, val, "versionChoice");  
        // alert('update');
    });  
    $(document).on('blur', '.wifiSsid', function(){  
        var id = $(this).data("id1");  
        var val = $(this).text();  
        edit_data(id, val, "wifiSsid");  
        // alert('update');
    });  
    $(document).on('blur', '.wifiPassword', function(){  
        var id = $(this).data("id1");  
        var val = $(this).text();  
        edit_data(id, val, "wifiPassword");  
        // alert('update');
    });  
    $(document).on('blur', '.autoUpdate', function(){  
        var id = $(this).data("id1");  
        var val = $(this).text();  
        edit_data(id, val, "autoUpdate");  
        // alert('update');
    });    
});
</script>

<button class="btn btn-info" type="button" data-toggle="collapse" data-target="#collapseExample" aria-expanded="false" aria-controls="collapseExample">
    Add Device
</button>

<div class="collapse" id="collapseExample">
  <br>
  <form id="linkDeviceForm">

  <div class="container-fluid">
  <div class="row">
  <div class="col-11">
<br>
<div class="input-group mb-3">
  <input type="text" class="form-control" style="width:50%" placeholder="Enter MAC Address" name="macAddress" id="macAddress">
  <div class="input-group-append">
    <button class="btn btn-success" type="button" name="submitMac" id="submitMac">Add</button>
    <button class="btn btn-danger" type="button" data-toggle="collapse" data-target="#collapseExample" aria-expanded="false" aria-controls="collapseExample">X</button>
  </div>
</div>

</div>
</div>
</div>

</form>
<br>
</div>


<br><br>

<br><h4>Program Versions List</h4>


<table class="table table-sm table-hover .table-striped">
  <thead class="thead-light">
    <tr>
      <th scope="col">programVersionsId</th>
      <th scope="col">programId</th>
      <th scope="col">versionId</th>
      <th scope="col">MCU Name</th>
      <th scope="col">Program Title</th>
      <th scope="col">Program Details</th>
      <th scope="col">Version Details</th>
      <th scope="col">Version Created At</th>
    </tr>
  </thead>
  <tbody>

  
<?php
$sql = "SELECT * FROM programVersions"; 

if ($result = $conn->query($sql)) {
    while ($row = $result->fetch_assoc()) {
      $programVersionsId = $row["programVersionsId"];
      $programId = $row["programId"];
      $versionId = $row["versionId"];

      $MCUId = $row["MCUId"];

      // get MCU Name
      $user_check_query = "SELECT * FROM MCU WHERE MCUid='$MCUId' LIMIT 1";
      $result2 = mysqli_query($conn, $user_check_query);
      $user = mysqli_fetch_assoc($result2);
      $MCUName = $user['name'];
      // echo $MCUName;

      // get program details
      $user_check_query = "SELECT * FROM programs WHERE programId='$programId' LIMIT 1";
      $result2 = mysqli_query($conn, $user_check_query);
      $user = mysqli_fetch_assoc($result2);
      $programTitle = $user['title'];
      $programDetails = $user['details'];

      $versionDetails = $row['details'];
      $versionCreatedAt = $row['createdAt'];
      
      // $MCUName = "h";
      // $programTitle ="s";
      // $programDetails = "a";
      
        echo '<tr> 
                <td>' . $programVersionsId . '</td> 
                <td>' . $programId . '</td> 
                <td>' . $versionId . '</td> 
                <td>' . $MCUName . '</td> 
                <td>' . $programTitle . '</td> 
                <td>' . $programDetails . '</td> 
                <td>' . $versionDetails . '</td> 
                <td>' . date('m/d/Y H:i:s',$versionCreatedAt) . '</td> 

              </tr>';
    }
    $result->free();
}
?>
</tbody>
</table>




<br><h4>Custom Application - Surveillance</h4>


<table class="table table-sm table-hover .table-striped">
  <thead class="thead-light">
    <tr>
      <th scope="col">Id</th>
      <th scope="col">deviceId</th>
      <th scope="col">deviceName</th>
      <th scope="col">objectName</th>
      <th scope="col">probability</th>
      <th scope="col">time</th>
    </tr>
  </thead>
  <tbody>

  
<?php
$sql = "SELECT * FROM appSurveillance WHERE userId = '$userId'"; 

if ($result = $conn->query($sql)) {
    while ($row = $result->fetch_assoc()) {
      $surveillanceId = $row["id"];
      $deviceId = $row["deviceId"];
      $objectName = $row["objectName"];
      $probability = $row["probability"];
      $time = $row["time"];


      // get device Name
      $user_check_query = "SELECT * FROM devices WHERE deviceId='$deviceId' LIMIT 1";
      $result2 = mysqli_query($conn, $user_check_query);
      $user = mysqli_fetch_assoc($result2);
      $deviceName = $user['deviceName'];
      // echo $MCUName;

      // // get program details
      // $user_check_query = "SELECT * FROM programs WHERE programId='$programId' LIMIT 1";
      // $result2 = mysqli_query($conn, $user_check_query);
      // $user = mysqli_fetch_assoc($result2);
      // $programTitle = $user['title'];
      // $programDetails = $user['details'];

      // $versionDetails = $row['details'];
      // $versionCreatedAt = $row['createdAt'];
      
      // $MCUName = "h";
      // $programTitle ="s";
      // $programDetails = "a";
      
        echo '<tr> 
                <td>' . $surveillanceId . '</td> 
                <td>' . $deviceId . '</td> 
                <td>' . $deviceName . '</td> 
                <td>' . $objectName . '</td> 
                <td>' . $probability . '</td> 
                <td>' . date('m/d/Y H:i:s',$time) . '</td> 
              </tr>';
    }
    $result->free();
}
?>
</tbody>
</table>
<br><br>


<?php $conn->close(); ?>



</body>

</html>


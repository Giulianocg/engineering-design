<?php
//This line will make the page auto-refresh each 15 seconds
$page = $_SERVER['PHP_SELF'];
$sec = "15";
?>


<html>
<head>
<!--//I've used bootstrap for the tables, so I inport the CSS files for taht as well...-->
<meta http-equiv="refresh" content="<?php echo $sec?>;URL='<?php echo $page?>'">		
<!-- Latest compiled and minified CSS -->
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/css/bootstrap.min.css">
<!-- jQuery library -->
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
<!-- Latest compiled JavaScript -->
<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/js/bootstrap.min.js"></script>
</head>
	
	
	
	
	
<body>    
<?php
include("database_connect.php"); //We include the database_connect.php which has the data for the connection to the database


// Check the connection
if (mysqli_connect_errno()) {
  echo "Failed to connect to MySQL: " . mysqli_connect_error();
}
//Again, we grab the table out of the database, name is ESPtable2 in this case
$result = mysqli_query($con,"SELECT * FROM ESPtable2");//table select


		  
//Now we create the table with all the values from the database	  
echo "<table class='table' style='font-size: 30px;'>
	<thead>
		<tr>
		<th>Active controls</th>	
		</tr>
	</thead>
	
    <tbody>
      <tr class='active'>
        
        <td>Mode </td>
        <td>Side </td>
		<td>Automatic </td>
      </tr>  
		";
		  
//loop through the table and print the data into the table
while($row = mysqli_fetch_array($result)) {
	
   echo "<tr class='success'>"; 	
    $unit_id = $row['id'];
    //echo "<td>" . $row['id'] . "</td>";
	
    $column1 = "BOOL_OPEN_CLOSE";
    $column2 = "BOOL_SIDE";
    $column3 = "BOOL_AUTO";
	
    $current_bool_1 = $row['BOOL_OPEN_CLOSE'];
	$current_bool_2 = $row['BOOL_SIDE'];
	$current_bool_3 = $row['BOOL_AUTO'];

	if($current_bool_1 == 1){
    $inv_current_bool_1 = 0;
	$text_current_bool_1 = "Open";
	$color_current_bool_1 = "#6ed829";
	}
	else{
    $inv_current_bool_1 = 1;
	$text_current_bool_1 = "Closed";
	$color_current_bool_1 = "#e04141";
	}
	
	
	if($current_bool_2 == 1){
    $inv_current_bool_2 = 0;
	$text_current_bool_2 = "Absorb";
	$color_current_bool_2 = "#808080";
	}
	else{
    $inv_current_bool_2 = 1;
	$text_current_bool_2 = "Reflect";
	$color_current_bool_2 = "#C3C3C3";
	}
	
	
	if($current_bool_3 == 1){
    $inv_current_bool_3 = 0;
	$text_current_bool_3 = "Auto";
	$color_current_bool_3 = "#6ed829";
	}
	else{
    $inv_current_bool_3 = 1;
	$text_current_bool_3 = "Manual";
	$color_current_bool_3 = "#e04141";
	}
	
	
	echo "<td><form action= update_values.php method= 'post'>
  	<input type='hidden' name='value2' value=$current_bool_1   size='15' >	
	<input type='hidden' name='value' value=$inv_current_bool_1  size='15' >	
  	<input type='hidden' name='unit' value=$unit_id >
  	<input type='hidden' name='column' value=$column1 >
  	<input type= 'submit' name= 'change_but' style=' margin-left: 5p; margin-top: 10p; font-size: 50px; text-align:center; background-color: $color_current_bool_1' value=$text_current_bool_1></form></td>";
	
     
	
	echo "<td><form action= update_values.php method= 'post'>
  	<input type='hidden' name='value2' value=$current_bool_2   size='15' >	
	<input type='hidden' name='value' value=$inv_current_bool_2  size='15' >	
  	<input type='hidden' name='unit' value=$unit_id >
  	<input type='hidden' name='column' value=$column2 >
  	<input type= 'submit' name= 'change_but' style=' margin-left: 5p; margin-top: 10p; font-size: 50px; text-align:center; background-color: $color_current_bool_2' value=$text_current_bool_2></form></td>";
	
	
	echo "<td><form action= update_values.php method= 'post'>
  	<input type='hidden' name='value2' value=$current_bool_3   size='15' >	
	<input type='hidden' name='value' value=$inv_current_bool_3  size='15' >	
  	<input type='hidden' name='unit' value=$unit_id >
  	<input type='hidden' name='column' value=$column3 >
  	<input type= 'submit' name= 'change_but' style=' margin-left: 5p; margin-top: 10p; font-size: 50px; text-align:center; background-color: $color_current_bool_3' value=$text_current_bool_3></form></td>";
	
	echo "</tr>
	  </tbody>"; 
	
}
echo "</table>
<br>
";	
?>
		
		
		
		
		
<?php
include("database_connect.php");

if (mysqli_connect_errno()) {
  echo "Failed to connect to MySQL: " . mysqli_connect_error();
}

$result = mysqli_query($con,"SELECT * FROM ESPtable2");//table select

echo "<table class='table' style='font-size: 30px;'>
	<thead>
		<tr>
		<th>Temperature</th>	
		</tr>
	</thead>
	
    <tbody>
      <tr class='active'>
        <td>Current Temperature</td>
        <td>TARGET TEMPERATURE</td>
      </tr>  
		";
		  
while($row = mysqli_fetch_array($result)) {

 	echo "<tr class='success'>";
   	
    $column4 = "TARGET_T";
    $current_num_1 = $row['TARGET_T'];
	
 	echo "<tr class='info'>";
	echo "<td>" . $row['CURRENT_T'] . "</td>";
	
		
	echo "<td><form action= update_values.php method= 'post'>
  	<input type='text' name='value' style='width: 120px;' value=$current_num_1  size='15' >
  	<input type='hidden' name='unit' style='width: 120px;' value=$unit_id >
  	<input type='hidden' name='column' style='width: 120px;' value=$column4 >
  	<input type= 'submit' name= 'change_but' style='width: 120px; text-align:center;' value='change'></form></td>";
	
	
	echo "</tr>
	  </tbody>"; 
	  
}

echo "</table>
<br>
";		
?>
		

	
	


	
	
	
	
	
<?php




include("database_connect.php");

if (mysqli_connect_errno()) {
  echo "Failed to connect to MySQL: " . mysqli_connect_error();
}

$result = mysqli_query($con,"SELECT * FROM ESPtable2");//table select

echo "<table class='table' style='font-size: 30px;'>
	<thead>
		<tr>
		<th> ----------- </th>	
		</tr>
	</thead>
	
    <tbody>
      <tr class='active'>
       
        <td>STATUS</td>
      </tr>  
		";
	  
	
	
while($row = mysqli_fetch_array($result)) {

 	$cur_sent_bool_1 = $row['BOOL_ON_OFF'];
	

	if($cur_sent_bool_1 == 1){
    $label_sent_bool_1 = "label-success";
	$text_sent_bool_1 = "On";
	}
	else{
    $label_sent_bool_1 = "label-danger";
	$text_sent_bool_1 = "Off";
	}
	
	 
		
	  echo "<tr class='info'>";
	  $unit_id = $row['id'];
        //echo "<td>" . $row['id'] . "</td>";	
		echo "<td>
		<span class='label $label_sent_bool_1'>"
			. $text_sent_bool_1 . "</td>
	    </span>";
		
	  echo "</tr>
	  </tbody>"; 
      

	
}
echo "</table>";
?>
		
	
	
	
	
	
		
		
		

		
		
		
		

		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
    
<?php 
 
//error_reporting(0);

function SignIn() 
{
	//session_start(); 
	if(!empty($_POST['user'])) 
	{ 
		$username =$_POST['user'];
		$password=$_POST['pass'];

		try { 
			$conn = new PDO('sqlite:SDTS.db'); 
			$conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION); 
			$stmt = $conn->prepare("SELECT username,pass from login where username = :user AND pass = :passw"); 

			$stmt->bindParam(':user', $username, PDO::PARAM_STR, 20);
			$stmt->bindParam(':passw', $password, PDO::PARAM_STR, 20);
	
			$stmt->execute();

			$result = $stmt->fetchAll();
		
			if(empty($result))
				echo "<script type='text/javascript'>alert('SORRY... YOU ENTERD WRONG ID AND PASSWORD... PLEASE 					RETRY...'); window.location.href='http://localhost/login.html';</script>";
			else
			{

			foreach ($result as $row)
			{
				$stmtdev = $conn->prepare("SELECT devid,devname,serialno,devtype FROM device where user = :id"); 

				$stmtdev->bindParam(':id', $username, PDO::PARAM_STR, 20);
	
				$stmtdev->execute();

				$resultdev = $stmtdev->fetchAll();

				echo"
				<html>
				<head>
					<title>Asset Tracking Demo</title>
					<link rel='stylesheet' type='text/css' href='http://localhost/css/tableformat.css'> 	
					<script type='text/javascript' src='http://localhost/js/jquery.js'></script>
					<script type='text/javascript' src='http://localhost/js/rowexpansion.js'></script>     
				</head>
				<body>
					<h4 align = 'right'>Welcome $username !</h4>
					<h1 align = 'center'>Device List</h1>
					<table id='report' align = 'center' width = '800'>
					<tr align = 'center'>
						<th></th>
						<th>$username Device List</th>
						<th></th>
						<th></th>
					</tr>";

				foreach ($resultdev as $rowdev)
				{
					$list = $rowdev['devname'];
					$serial = $rowdev['serialno'];
					$type = $rowdev['devtype'];

					echo "
				<tr class = 'main'>";
				if($type == "Mobile")
					echo "<td width = '5%'><img src='http://localhost/images/mobile.JPG' alt='' style='width:14px; height:auto;'></td>";
				else if($type == "Camera")
					echo "<td width = '5%'><img src='http://localhost/images/camera.JPG' alt='' style='width:14px; height:auto;'></td>";
				else
					echo "<td width = '5%'><img src='http://localhost/images/smartwatch.JPG' alt='' style='width:14px; height:auto;'></td>";
				echo"
					<td width = '45%'>$list</td>
					<td width = '45%'>Serial No: $serial</td>
					<td width = '5%'><div class='arrow'></div></td>
				</tr>
				<tr class = 'one' align = 'center'>
					<td><b><i>Id</i></b></td>
					<td>
						<b><i>Username</i></b> 
					</td>
					<td>
						<b><i>Date</i></b>  
					</td>
					<td>
						<b><i>Time</i></b>
					</td>
				</tr>
				";

				$stmtvalue = $conn->prepare("SELECT userid,username,userloc,mysingle,extn,dateused,timeused from dummyinfo di INNER JOIN userinfo ui ON di.mac = ui.macaddress where di.serialno = :sno"); 

				$stmtvalue->bindParam(':sno', $serial, PDO::PARAM_STR, 20);
	
				$stmtvalue->execute();

				$resultvalue = $stmtvalue->fetchAll();
					$id = 1;

				foreach ($resultvalue as $rowvalue)
				{
					$usern = $rowvalue['username'];
					$loc = $rowvalue['userloc'];
					$email = $rowvalue['mysingle'];
					$ext = $rowvalue['extn'];
					$uid = $rowvalue['userid'];
					$date = $rowvalue['dateused'];
					$time = $rowvalue['timeused'];
					
					echo"
				<tr class = 'one' align = 'center'>
					<td>$id.</td>
					<td title='Name: $usern\nLocation: $loc\nMySingle: $email\nExtension: $ext'>
						$uid 
					</td>
					<td>
						$date 
					</td>
					<td>
						$time
					</td>
				</tr>";
				$id = $id+1;
				}
				
				}
				echo"
				</table>
				</body>
				</html>";	
			  }
			}
		} catch( PDOException $Exception ) 
		{ 
			echo $Exception->getMessage() ."\n"; 
		} 	
	}

}

if(isset($_POST['submit'])) 
{ 
	SignIn(); 
} 

?>


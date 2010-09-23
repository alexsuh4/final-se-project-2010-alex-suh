
<?php
	$reqTypes= array(
					"keepAlive"=>"0",
					"none"=>"1",
					"header-end"=>"3",
					"login"=>"2",
					"logout"=>"4",
					"switch_gamelet"=>"5");
	
	$headerFields= array(
					"messageType"=>"0",
					"playerId"=>"1",
					"gameletSessionId"=>"2",
					"header-end"=>"3",
					"username"=>"4",
					"password"=>"5",
					"gamelet_type_id","6");
	$start_time=time();
	//$pid=$_GET["player_id"];
		
	$username=$_POST["player_user_name"];
	$password=$_POST["player_password"];
	$rounds=1;
        //echo "user name= " . $username . "password " .$password;

	if ($message == "")
	{
		//$message =" ".$message . $headerFields["messageType"] . " " .$reqTypes["keepAlive"]. " ";
		//$message =" ".$message . $headerFields["playerId"] . " " . $pid . " ";
		//$message =" ".$message . $headerFields["header-end"] . " " .$reqTypes["header-end"]. " ";
		$message =" ".$message . $headerFields["messageType"] . " " .$reqTypes["login"]. " ";
		$message =" ".$message . $headerFields["username"] . " " .$username. " ";
		$message =" ".$message . $headerFields["password"] . " " .$password. " ";
		
	}
	// form submitted
	// where is the socket server?
	$host="localhost";
	$port = 12345;
	
		$fp = fsockopen ($host, $port, $errno, $errstr);
		if (!$fp)
		{
			$result = "<script type=\"text/javascript\" >alert('Error: could not open socket connection')</script>";
		}
		else
		{									
			fputs ($fp, $message);
			// get the result
			$result=fgets($fp,1024);
			// close the connection
			fclose ($fp);
			$result = trim($result);
                        echo $result;
		}
?>






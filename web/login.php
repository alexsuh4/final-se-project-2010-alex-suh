

<?php
	$reqTypes= array(
					"keepAlive"          =>"0"
					,"none"              =>"1"
					,"header-end"        =>"3"
					,"login"             =>"2"
					,"logout"            =>"4"
					,"switch_gamelet"    =>"5"
                                        ,"register"          =>"7"
                        );
	
	$headerFields= array(
					"messageType"        =>"0"
					,"playerId"          =>"1"
					,"gameletSessionId"  =>"2"
					,"header-end"        =>"3"
					,"username"          =>"4"
					,"password"          =>"5"
					,"gamelet_type_id"   =>"6"
                                        ,"email"             =>"7"
                            );
	
	
	$operation=$_POST["operation"];
	
	
        if ($operation=="login")
        {
            $username=$_POST["player_user_name"];
            $password=$_POST["player_password"];
            //login
            $message =" ".$message . $headerFields["messageType"] . " " .$reqTypes["login"]. " ";
            $message =" ".$message . $headerFields["username"] . " " .$username. " ";
            $message =" ".$message . $headerFields["password"] . " " .$password. " ";


        }
        else if($operation=="register")
        {
            $username=$_POST["player_user_name"];
            $password=$_POST["player_password"];
            $email=$_POST["player_email"];
            //register
            $message =" ".$message . $headerFields["messageType"] . " " .$reqTypes["register"]. " ";
            $message =" ".$message . $headerFields["username"] . " " .$username. " ";
            $message =" ".$message . $headerFields["password"] . " " .$password. " ";
            $message =" ".$message . $headerFields["email"] . " " .$email. " ";
            echo "you requested " . $message;
            //return;
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






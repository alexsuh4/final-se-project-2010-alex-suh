
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
	

        /*
	$username="sampleuser";
	$password="samplepassword";
	

	if ($message == "")
	{
            $message =" ".$message . $headerFields["messageType"] . " " .$reqTypes["login"]. " ";
            $message =" ".$message . $headerFields["username"] . " " .$username. " ";
            $message =" ".$message . $headerFields["password"] . " " .$password. " ";	
	}*/
        $operation=$_GET["operation"];
        $message="";
        if ($operation=="update")
        {
            $player_id=$_GET["player_id"];
            //update operation requested
            $message =" ".$message . $headerFields["messageType"] . " " .$reqTypes["keepAlive"]. " ";
            $message =" ".$message . $headerFields["playerId"] . " " .$player_id. " ";
        }
        $gamelet_date=$_GET["gamelet_data"];
        if ($gamelet_date!="")
        {
            $message =" ".$message." "."gamelet_data"." ".$gamelet_date." ";
        }
	// form submitted
	// where is the socket server?
	$host="localhost";
	$port = 12345;
	// open a client connection
        
	$fp = fsockopen ($host, $port, $errno, $errstr);
	if (!$fp)
	{
            $result = "Error: could not open socket connection";
	}
	else
	{									
            fputs ($fp, $message);
            // get the result
            $result=fgets($fp,1024);
            // close the connection
            fclose ($fp);
            $result = trim($result);
            // now print it to the browser
            echo $result;
        }
        
?>



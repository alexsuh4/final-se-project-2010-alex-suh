function ChatManager()
{
this.sendReq = getXmlHttpRequestObject();
this.receiveReq = getXmlHttpRequestObject();
this.lastMessage = 0;
this.mTimer=0;
var me=this;
//Function for initializating the page.

this.Instance=function()
{
    return new ChatManager();
}

this.startChat=function (){
	//Set the focus to the Message Box.
	document.getElementById('txt_message').focus();
	//Start Recieving Messages.
	getChatText();
}		
//Gets the browser specific XmlHttpRequest Object
this.getXmlHttpRequestObject=function() {
	if (window.XMLHttpRequest) {
		return new XMLHttpRequest();
	} else if(window.ActiveXObject) {
		return new ActiveXObject("Microsoft.XMLHTTP");
	} else {
		document.getElementById('p_status').innerHTML = 'Status: Cound not create XmlHttpRequest Object.  Consider upgrading your browser.';
	}
}

//Gets the current messages from the server
this.getChatText= function () {
    if (receiveReq.readyState == 4 || receiveReq.readyState == 0) 
	{
		receiveReq.open("GET", 'getChat.php?chat=1&last=' + this.lastMessage, true);
		receiveReq.onreadystatechange = handleReceiveChat; 
		receiveReq.send(null);
	}			
}
//Add a message to the chat server.
this.sendChatText= function () {
	if(document.getElementById('txt_message').value == '') {
		alert("You have not entered a message!");
		return;
	}
	if (this.sendReq.readyState == 4 || this.sendReq.readyState == 0) {
		this.sendReq.open("POST", 'getChat.php?chat=1&last=' + this.lastMessage, true);
		this.sendReq.setRequestHeader('Content-Type','application/x-www-form-urlencoded');
		this.sendReq.onreadystatechange = handleSendChat;
		var param = 'message=' + document.getElementById('txt_message').value;
		param += '&name=Ryan Smith';
		param += '&chat=1';
		this.sendReq.send(param);
		document.getElementById('txt_message').value = '';
	}							
}
//When our message has been sent, update our page.
this.handleSendChat= function () {
	//Clear out the existing timer so we don't have 
	//multiple timer instances running.
	clearInterval(this.mTimer);
	getChatText();
}
//Function for handling the return of chat text
this.handleReceiveChat=function () {
	if (receiveReq.readyState == 4) {
		var chat_div = document.getElementById('div_chat');
		var xmldoc = receiveReq.responseXML;
		var message_nodes = xmldoc.getElementsByTagName("message"); 
		var n_messages = message_nodes.length
                var i;
		for (i = 0; i < n_messages; i++) {
			var user_node = message_nodes[i].getElementsByTagName("user");
			var text_node = message_nodes[i].getElementsByTagName("text");
			var time_node = message_nodes[i].getElementsByTagName("time");
			chat_div.innerHTML += user_node[0].firstChild.nodeValue + '&nbsp;';
			chat_div.innerHTML += '<font class="chat_time">' + time_node[0].firstChild.nodeValue + '</font><br />';
			chat_div.innerHTML += text_node[0].firstChild.nodeValue + '<br />';
			chat_div.scrollTop = chat_div.scrollHeight;
			me.this.lastMessage = (message_nodes[i].getAttribute('id'));
		}
		me.this.mTimer = setTimeout('getChatText();',2000); //Refresh our chat in 2 seconds
	}
}
//This functions handles when the user presses enter.  Instead of submitting the form, we
//send a new message to the server and return false.
this.blockSubmit=function () {
	sendChatText();
	return false;
}
//This cleans out the database so we can start a new chat session.
this.resetChat=function () {
	if (this.sendReq.readyState == 4 || this.sendReq.readyState == 0) {
		this.sendReq.open("POST", 'getChat.php?chat=1&last=' + this.lastMessage, true);
		this.sendReq.setRequestHeader('Content-Type','application/x-www-form-urlencoded');
		this.sendReq.onreadystatechange = handleResetChat;
		var param = 'action=reset';
		this.sendReq.send(param);
		document.getElementById('txt_message').value = '';
	}							
}
//This function handles the response after the page has been refreshed.
this.handleResetChat=function () {
	document.getElementById('div_chat').innerHTML = '';
	getChatText();
}
}
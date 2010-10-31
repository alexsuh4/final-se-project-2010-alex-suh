//ajax variables
var OPERATION_UPDATE="update";
var OPERATION_LOGIN="login";
var OPERATION_LOGOUT="logout";
var OPERATION_SWITCH_GAMELET="switch_gamelet";
var VAR_OPERTION="operation";
var OPERATION_REGISTER="register";


var BASE_GAMELET_PATH="gamelets";
function getCurrentGameletPath()
{
   
    var result=BASE_GAMELET_PATH;
    var current_gamelet=currentState.attribs["gamelet"];
    if (current_gamelet && current_gamelet.getCurrentGameletPath)
        result+="/"+current_gamelet.getCurrentGameletPath();
    return result;
}

function debug_trace(reason,msg)
{
     if (debugMode)
     {
            alert(reason+"\n====="+"\n"+msg);
     }
}

function getMainPanel()
{
    return document.getElementById("user_client_main");
}
function getControlsPanel()
{
    return document.getElementById("user_client_controls");
}
function getInfoPanel()
{
    return document.getElementById("user_client_info");
}

function newlineelm()
{
    return document.createElement("br");
}


/**
 *sendAjax send an asynchrounus javascript request to a given server
 *@param params request parameters
 *@param onResponse response callback function
 *@param url serevr url
 *@param elm an element where status updates are printed
 *@param method the ajax method to use (GET or POST) default is GET
 */
function sendAjax(method,params,onResponse,url,elm)
{
    var xmlhttp;
    if (elm)
        elm.setAttribute("value", "Please wait...");
    if (window.XMLHttpRequest)
    {// code for IE7+, Firefox, Chrome, Opera, Safari
        xmlhttp=new XMLHttpRequest();
    }
    else
    {// code for IE6, IE5
        xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
    }
    xmlhttp.onreadystatechange=function(){
       if (xmlhttp.readyState==4 && xmlhttp.status==200)
            onResponse(xmlhttp);
    };
    if (method=="POST")
    {
        xmlhttp.open("POST",url,true);
       
        xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
        xmlhttp.send(params);
    }
    else
    {
        if (params!="" && params !=null)
            url=url+"?"+params;
        xmlhttp.open("GET",url,true);
        xmlhttp.send();
    }
   
}

/**
 *LoginContext
 *represend a login contex class used to carry out logins
 **/
function LoginContext()
{
    this.loginform=null;
    this.txtUserName=null;
    this.txtPassword=null;
    this.btnSubmit=null;
    this.init=function()
    {
        this.loginform=this.createLoginForm();
        return this.loginform;
    }
    var me=this;
    var SERVER_URL="login.php";
        
    this.createLoginForm=function()
    {
        var LOGIN_FORM_ID="loginform";
        var USER_NAME_ID="player_user_name";
        var PASSWORD_ID="player_password";

        var form=document.createElement("form");

        form.setAttribute("id", LOGIN_FORM_ID);
        

        var txtUserName=document.createElement("input");
        txtUserName.setAttribute("type", "text");
        txtUserName.setAttribute("name", USER_NAME_ID);
        this.txtUserName=txtUserName;

        var txtPassword=document.createElement("input");
        txtPassword.setAttribute("type", "password");
        txtPassword.setAttribute("name", PASSWORD_ID);
        this.txtPassword=txtPassword;

        //ajax request

        var btnSubmit=document.createElement("input");
        btnSubmit.setAttribute("name", "submit");
        btnSubmit.setAttribute("value", "submit");
        btnSubmit.setAttribute("type", "button");
        
        
        var lblUserName=document.createElement("span");
        lblUserName.innerHTML="<b>User Name</b>";

        var lblPassword=document.createElement("span");
        lblPassword.innerHTML="<b>Password</b>";

        form.appendChild(lblUserName);
        form.appendChild(txtUserName);
        form.appendChild(newlineelm());
        form.appendChild(lblPassword);
        form.appendChild(txtPassword);
        form.appendChild(newlineelm());
        form.appendChild(btnSubmit);

        
        //btnSubmit.setAttribute("onClick", this.ajax_login);
        
        btnSubmit.onclick=this.ajax_login;
        this.btnSubmit=btnSubmit;
        return form;

    }
    
    this.ajax_login=function ()
    {
        var username=me.txtUserName.value;
        var password=me.txtPassword.value;
        sendAjax(
            "POST"
            ,"player_user_name="+username+"&player_password="+password+"&operation="+OPERATION_LOGIN
            ,me.loginCallbackFunction,
            SERVER_URL,
            me.btnSubmit);
        
    }
    this.loginCallbackFunction=function(xmlhttp)
    {
   
        if (xmlhttp.readyState==4 && xmlhttp.status==200)
        {
            var response=xmlhttp.responseText.split(";");
            
            
            //debug_trace("paramrs split by ;  \n"+dbgTxt);

            var gamelet_url=response[0];
            //debug_trace("gamelet url "+gamelet_url);
            var index=gamelet_url.indexOf("gamelets");
            if (index<0)
            {
                //debug_trace("could not load gamelet!,response text was "+response+"\nindex="+index);
                me.btnSubmit.value="Login";
                return;
            }
            var playerid=response[1];
            currentState.attribs["playerid"]=playerid;

            //chat integration start

            var info=getInfoPanel();
            var errMsg="";
            if (ChatSystsem)
            {
                info.innerHTML+="<br/>loading chat System ...";
                info.innerHTML="";
                //init chat system
                currentState.attribs["Chat"]= new ChatSystsem(info,"testplayer");
            }
            else
            {
                   errMsg+="\ncould not load Chat system !";
            }
            if (errMsg.length>0)
                alert(errMsg);

            //chat integration end

            //debug_trace("loading gamlet from "+gamelet_url+"\n"+
            //      "player id is "+currentState.attribs["playerid"]);

            sendAjax(
                "GET"
                ,""
                ,loadGameletCallbackFunction
                ,gamelet_url
                , me.btnSubmit);
            
        }
       
       
    }


}




function loadGameletCallbackFunction(xmlhttp)
{

        if (xmlhttp.readyState==4 && xmlhttp.status==200)
        {
            //debug_trace("gamelet recieved!");
            var response=xmlhttp.responseText;
            //debug_trace("evalueting "+response);
            eval(response);
            var main=getMainPanel();

            var loadedgamelet=new Gamelet(main);
            if (currentState.attribs["current_gamlet_container"])
                currentState.attribs["current_gamlet_container"].stop();
            currentState.attribs["current_gamlet_container"]=new Gamelet_container(loadedgamelet);
            currentState.attribs["current_gamlet_container"].init();

            currentState.current_gamelet=loadedgamelet;

            currentState.attribs["gamelet"]=currentState.current_gamelet;

        }

}

function do_loginCallbackFunction(xmlhttp)
    {

        if (xmlhttp.readyState==4 && xmlhttp.status==200)
        {
            var response=xmlhttp.responseText.split(";");


            //debug_trace("paramrs split by ;  \n"+dbgTxt);

            var gamelet_url=response[0];
            //debug_trace("gamelet url "+gamelet_url);
            var index=gamelet_url.indexOf("gamelets");
            if (index<0)
            {
                //debug_trace("could not load gamelet!,response text was "+response+"\nindex="+index);

                return;
            }
            var playerid=response[1];
            currentState.attribs["playerid"]=playerid;

            //chat integration start

            var info=getInfoPanel();
            var errMsg="";
            if (ChatSystsem)
            {
                //init chat system
                currentState.attribs["Chat"]= new ChatSystsem(info,"testplayer");
            }
            else
            {
                   errMsg+="\ncould not load Chat system !";
            }
            if (errMsg.length>0)
                alert(errMsg);

            //chat integration end

            //debug_trace("loading gamlet from "+gamelet_url+"\n"+
            //      "player id is "+currentState.attribs["playerid"]);

            sendAjax(
                "GET"
                ,""
                ,loadGameletCallbackFunction
                ,gamelet_url
                , me.btnSubmit);

        }
    }
        
function do_login(userName,Password)
{
    sendAjax(
            "POST"
            ,"player_user_name="+userName+"&player_password="+Password+"&operation="+OPERATION_LOGIN
            ,do_loginCallbackFunction,
            SERVER_URL,
            null);
}





function chnage_gamelet(newGamletUrl)
{
    sendAjax(
                "GET"
                ,""
                ,loadGameletCallbackFunction
                ,newGamletUrl
                ,null);
}
function Gamelet_container(_mygamelet)
{
    
    var me=this;
    this.mygamelet=_mygamelet;
    this.draw_refresh_speed=10;
    this.sync_speed_long=100;
    this.sync_speed_short=100;
    this.updateModel_speed=11;
    //called when Sync error accured , can be used for event handling attachment
    this.syncError=null;
    this.init=function()
    {
        
        me.timer_redraw= setTimeout(me.redraw,me.draw_refresh_speed);
        me.timer_sync= setTimeout(me.sync,me.sync_speed_long);
        me.timer_update= setTimeout(me.updateModel,me.updateModel_speed);
        
    }
    this.redraw=function()
    {
        
        me.mygamelet.draw_Model();
        me.timer_redraw=setTimeout(me.redraw,me.draw_refresh_speed);
        
    }
    this.sync=function()
    {
        var SERVER_URL="web_connector.php";
        var playerid=currentState.attribs["playerid"];
        
        
        var chatmanager=currentState.attribs["Chat"];
        var chatParams="";
        
       
        if (chatmanager)
            chatParams=chatmanager.serializeCurrentMessage();
         

        var params="player_id="+playerid+"&"+VAR_OPERTION+"="+OPERATION_UPDATE;
        if (chatParams!="")
            {
                params+="&"+chatParams;
            } 
        params+="&gamelet_data="+me.mygamelet.UpdateServer();
        sendAjax
        ("GET"
         ,params
         ,me.newModelRecieved
         ,SERVER_URL
         ,null
        );
    }
    this.newModelRecieved=function(xmlhttp)
    {
        var isError=false;
        var new_Model_Serialized=xmlhttp.responseText;
        var new_Model;
        try
        {
            //evaluate system message
            
           new_Model=eval("("+new_Model_Serialized+")");
            
            
        if (new_Model.system_data)
        {
            
            ////system data in thisd batch
            //update time
            if (new_Model.system_data.updateTime)
            {
                
                //debug_trace("system time");
                //accamodate for server actual speed
                me.updateModel_speed=new_Model.system_data.updateTime;
               
            }
             //debug_trace("ChangeGameletTo is "+(new_Model.system_data.ChangeGameletTo?"definden":"not defined"));
             //Change Gamelet To another gamelet
            if (new_Model.system_data.ChangeGameletTo)
            {
                
                //debug_trace("message to change gameelt been recieved...");
                change_gamelet(new_Model.system_data.ChangeGameletTo);
                return;
            }
            //chat handling start
            if (new_Model.system_data.ChatMessages && new_Model.system_data.ChatMessages.length>0)
            {
                var messages=new_Model.system_data.ChatMessages;
                for (var i=0;i<messages.length;i++)
                    {
                        var chatMan=currentState.attribs["Chat"];
                        if (chatMan)
                            {
                                chatMan.appnedMessage
                                (
                                    messages[i].from
                                    ,messages[i].body
                                );
                            }
                    }
            }
            //chat handling end
        }
        else//no sstem data
            {
                debug_trace("no system data");
            }
    me.mygamelet.sync_Model(new_Model.GameletModel);

    }//try
        catch(exp)
        {


            //model syncing went wrong
            var errtxt="excpetion accured";
            errtxt+="\ndescription  : "             +exp.description;
            errtxt+="\nname         : "             +exp.name;
            errtxt+="\nmessage      : "             +exp.message;
            errtxt+="\nmore data:\n========\n";
            errtxt+="\n   exp\n======";

            for(prop in exp)
            {
                errtxt+="\n"+prop+"="+exp[prop];
            }
            errtxt+="\n   new_Model\n=========";
            for(prop in new_Model)
            {
                errtxt+="\n"+prop+"="+new_Model[prop];
            }
            errtxt="serialized data was : " +new_Model_Serialized+"\n"+errtxt;
            debug_trace("exception", errtxt);
            isError=true;
            if (me.syncError) me.syncError(exp);
        }
         //even if modelparsing fails, attemp to retransmit
         if (isError)//<= if error accured , wait some time and try again
             me.timer_sync= setTimeout(me.sync,me.sync_speed_long*2);
         else
            me.timer_sync= setTimeout(me.sync,me.sync_speed_long);
    }
    this.updateModel=function()
    {
        var selectedObject=me.mygamelet.getSelectedObject();
        //alert("updateModel me.mygamelet.getSelectedObject"+selectedObject );
        if (selectedObject!="" && selectedObject && currentState.attribs["Chat"])
        {
            currentState.attribs["Chat"].setTo(selectedObject);
        }
        me.mygamelet.extrapolateModel();
        me.timer_update=setTimeout(me.updateModel,me.updateModel_speed);
    }
    this.stop=function()
    {
        try
        {
            if (me.timers_redraw)
                 clearTimeout(me.timer_redraw);
        }
        catch (e){}
        try
        {
            if (me.timers_sync)
                 clearTimeout(me.timer_sync);
        }
        catch(e){}
        try
        {
            if(me.timer_update)
                clearTimeout(me.timer_update);
        }
        catch (e){}
    }
}
var debugMode=false;
function State()
{
    this.current_gamelet=null;
    this.attribs=new Object();
    var me=this;
    this.setPropery=function(key,val)
    {
        me.attribs[key]=val;
    }
    this.getProperty=function(key)
    {
        return me.attribs[key];
    }

}
var currentState;
var _loginContext;

function init()
{
    //init session State
    currentState=new State();
    //show login screen
    var controlPanel=getControlsPanel();
    _loginContext=new LoginContext();
    
    var form=_loginContext.init();
    controlPanel.appendChild(form);
    
}
function handleControls(event)
{
    var key='';
    if (event.which == null)
     key= String.fromCharCode(event.keyCode);    // IE
    else if (event.which != 0 && event.charCode != 0)
     key= String.fromCharCode(event.which);	  // All others

    //debug_trace(currentState.current_gamelet);
    if (currentState.current_gamelet)
    {
        //debug_trace("test");
        currentState.current_gamelet.handleControls(key);
    }

}

function registerUser(email,username,password) {

  
    var reg_qry="player_email="+email+"&player_user_name="+username+"&player_password="+password;
    reg_qry+="&operation="+OPERATION_REGISTER;
    var reg_serevr_url="login.php";
    
    sendAjax(
    "POST"
    ,reg_qry
    ,registerUserCallbackFunction
    ,reg_serevr_url
    ,null);
    
}

function registerUserCallbackFunction(xmlhttp) {


    if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
        //login succesfull
        window.location = "user_client.html";
        }
    }


function load()
{
    init();
    
}
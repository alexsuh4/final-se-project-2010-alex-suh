

////the Gamelet Class acts as an interface to the game engine
//this class acts as an interface for an flash gamelet
function Gamelet(_divOnPage)
{
    var me=this;
  this.getCurrentGameletPath=function()
    {
        return "flashSampleGamelet/flash";
    }

    this.name="Flash gamelet"
    this.canvas = _divOnPage;
    this.myFlashObj = null;
    this.isInitialized=false;
    

    
   
    
    //this get excecuted before first run and after the new

    this.init = function() {
    //alert("this.init");
    if (me.isInitialized)
    {
       // alert("this.init is already initialized");
        return;
    }
    //alert("initializing");
        //     <object id='mySwf' classid='clsid:D27CDB6E-AE6D-11cf-96B8-444553540000' codebase='http://fpdownload.macromedia.com/get/flashplayer/current/swflash.cab' height='200' width='400'>
        //                <param name='src' value='WrapperCaller.swf'/>
        //                <param name='flashVars' value=''/>
        //                <embed name='mySwf' src='WrapperCaller.swf' pluginspage='http://www.adobe.com/go/getflashplayer' height='100%' width='100%' flashVars=''/>
        //            </object>
        //
        var newObj;

       var binFileName="flashGamelet.swf";
        var binPath=
            BASE_GAMELET_PATH+"/"+me.getCurrentGameletPath()+"/"+binFileName;
            //"gamelets/flashSampleGamelet/flash/flashGamelet.swf";
        var currentPath=
            BASE_GAMELET_PATH+"/"+me.getCurrentGameletPath();
            //"gamelets"+"/flashSampleGamelet/flash/";

        var encodedPath=encodeURI(currentPath);
        this.currentPath=encodedPath;
        newObj = document.createElement("object");
        newObj.id = "flashGamelet";
        newObj.classid = "clsid:D27CDB6E-AE6D-11cf-96B8-444553540000";
        newObj.codebase = "http://fpdownload.macromedia.com/get/flashplayer/current/swflash.cab";
        newObj.width = "800";
        newObj.height = "600";


        var param1;
        param1 = document.createElement("param");
        param1.name = "src";
        param1.value = binPath;
        newObj.appendChild(param1);

        var param2;
        param2 = document.createElement("param");
        param2.name = "flashVars";
        param2.value = "path="+encodedPath;
        newObj.appendChild(param2);

        var embedObj;
        embedObj = document.createElement("embed");
        embedObj.name = "flashGamelet";
        embedObj.src = binPath;
        embedObj.pluginspage = "http://www.adobe.com/go/getflashplayer";
        embedObj.height = "100%";
        embedObj.width = "100%";
        embedObj.flashVars = "path="+encodedPath;
        newObj.appendChild(embedObj);

        me.canvas.appendChild(newObj);

        //me.myFlashObj = document.getElementById("flashGamelet");
         if (window.XMLHttpRequest)
        {// code for IE7+, Firefox, Chrome, Opera, Safari
            me.myFlashObj=embedObj;
        }
        else
        {// code for IE6, IE5
            //scriptObject=me.myFlashObj.ch
            me.myFlashObj=newObj;
        }

        _divOnPage.style.width="800px";
        _divOnPage.style.height="600px";


        me.isInitialized=true;

        if (!me.myFlashObj)
        {
            //alert("Embeding object failed!")
        }
        
        //alert("initzlized");
    }
    me.init();

    ///this gets exceuted every game cycle
    
    this.mainloop=function()
    {
       
    }
    this.start=function ()
    {
       
    }
    this.end=function ()
    {
        
    }
    ///leave empty if got own timing logic
    ///triggered by engine in a loop
    ///this is where you put your drawing logic
    ///if using no javascript gamlet ,leave it unimplemented and implement
    ///drawing in flash instead
    this.draw_Model=function()
    {
         
     
    }
    this.currentPlayer=null;
    //triggered by engine in a loop
    ///this is where you put your synchronization with server  logic
    /// new model holds a Json decoded javascript object
    //that supposed to represent your current game state
    this.sync_Model=function(newModelObj)
    {
       
        
        if (!me.isInitialized)
        {
            //alert("sync_Model enter not initialized");
            me.init();
        }
        

        // call flex function and pass newModelObj as a state of current game state
        var x;
        var y;
        var ang;
        var vel;
        var id;
        var isCurrentPlayer;

 
        me.myFlashObj.testFunc
        (
            newModelObj.objects
        );
        if (  currentState.attribs["playerid"] && currentState.attribs["playerid"]!="" &&  me.myFlashObj   &&   me.myFlashObj.SetCurrentPlayer)
        {
            me.currentPlayer=currentState.attribs["playerid"];
            me.myFlashObj.SetCurrentPlayer
            (
                    me.currentPlayer
            );
        }
        
//        for(var i=0 ;i <newModelObj.objects.length;i++ )
//           {
//                x=newModelObj.objects[i].x;
//                y=newModelObj.objects[i].y;
//                ang=newModelObj.objects[i].ang;
//                vel=newModelObj.objects[i].vel;
//                id=newModelObj.objects[i].player_id;
//                isCurrentPlayer=currentState.attribs["playerid"]==id;// current state is assumed to be defined in client framework (script.js)
//                //alert("x = "+x+" y= "+y+" ang = "+ang+ " vel = "+vel + " player_id = "+id+ " isCurrentPlayer = "+isCurrentPlayer+" \nme.myFlashObj.sync_Model is a "+me.myFlashObj.sync_Model);
//                //me.myFlashObj
//                if ( !me.myFlashObj.sync_Model)
//                    return ;//not ready
//                me.myFlashObj.sync_Model
//                (
//                    isCurrentPlayer
//                    ,id
//                    ,x
//                    ,y
//                    ,ang
//                    ,vel
//                );
//           }

    }   
    ///leave empty if got own timing logic
    ///triggered by engine in a loop
    ///this is where you put your drawing logic
    ///if using no javascript gamlet ,leave it unimplemented and implement
    ///extrapolateModel (moving) in flash(or whatever you use) instead
    this.extrapolateModel=function()
    {
    }
    ///passes pressed controls in HTML context
    this.handleControls=function(key)
    {
       
    }
    //returns anything this gamelet wishes to send to the sererv 
    //right now supported GET method only
    this.UpdateServer=function()
    {
        var message="";
        if (me.myFlashObj.UpdateServer)     message=me.myFlashObj.UpdateServer();
        //if (message!="")                    alert(message);
        return message;
    }
    /**
     *returns a string representing id of currently selecte d
     *object 
     */
    this.getSelectedObject=function()
    {
        var message="";
        if (me.myFlashObj && me.myFlashObj.getSelectedObject)
            message=me.myFlashObj.getSelectedObject();
        //if (message&& message!="") alert(message);
        return message;
        
    }
    ///passes on clicks , clicked on HTML surface
    this.handleClick=function(x,y)
    {
       //N/a - handles by flash
    }

//uncomment to use , currently do ont need it
 /**
 * Converts the given data structure to a JSON string.
 * Argument: arr - The data structure that must be converted to JSON
 * Example: var json_string = array2json(['e', {pluribus: 'unum'}]);
 * 			var json = array2json({"success":"Sweet","failure":false,"empty_array":[],"numbers":[1,2,3],"info":{"name":"Binny","site":"http:\/\/www.openjs.com\/"}});
 * http://www.openjs.com/scripts/data/json_encode.php
 */
//    this.array2json=function (arr) {
//    var parts = [];
//    var is_list = (Object.prototype.toString.apply(arr) === '[object Array]');
//
//    for(var key in arr) {
//    	var value = arr[key];
//        if(typeof value == "object") { //Custom handling for arrays
//            if(is_list) parts.push(array2json(value)); /* :RECURSION: */
//            else parts[key] = array2json(value); /* :RECURSION: */
//        } else {
//            var str = "";
//            if(!is_list) str = '"' + key + '":';
//
//            //Custom handling for multiple data types
//            if(typeof value == "number") str += value; //Numbers
//            else if(value === false) str += 'false'; //The booleans
//            else if(value === true) str += 'true';
//            else str += '"' + value + '"'; //All other things
//            // :TODO: Is there any more datatype we should be in the lookout for? (Functions?)
//
//            parts.push(str);
//        }
//    }
//    var json = parts.join(",");
//
//    if(is_list) return '[' + json + ']';//Return numerical JSON
//    return '{' + json + '}';//Return associative JSON
//}
    
}





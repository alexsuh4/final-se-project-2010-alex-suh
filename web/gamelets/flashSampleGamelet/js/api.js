//the Gamelet Class acts as an interface to the game engine
//this class acts as an interface for an flash gamelet
function Gamelet(_divOnPage)
{

    this.name="Flash gamelet"
    this.canvas = _divOnPage;
    this.myFlashObj = null;
    //this.canvas.innerHTML="Loaded!";
    //this.myModel=new Model(this.canvas);
    var me=this;
    //this get excecuted before first run and after the new

    this.init = function() {

        //     <object id='mySwf' classid='clsid:D27CDB6E-AE6D-11cf-96B8-444553540000' codebase='http://fpdownload.macromedia.com/get/flashplayer/current/swflash.cab' height='200' width='400'>
        //                <param name='src' value='WrapperCaller.swf'/>
        //                <param name='flashVars' value=''/>
        //                <embed name='mySwf' src='WrapperCaller.swf' pluginspage='http://www.adobe.com/go/getflashplayer' height='100%' width='100%' flashVars=''/>
        //            </object>
        //
        var newObj;

        newObj = document.createElement("object");
        newObj.id = "GameClientFlash";
        newObj.classid = "clsid:D27CDB6E-AE6D-11cf-96B8-444553540000";
        newObj.codebase = "http://fpdownload.macromedia.com/get/flashplayer/current/swflash.cab";
        newObj.width = "800";
        newObj.height = "600";


        var param1;
        param1 = document.createElement("param");
        param1.name = "src";
        param1.value = "GameClientFlash.swf";
        newObj.appendChild(param1);

        var param2;
        param2 = document.createElement("param");
        param2.name = "flashVars";
        param2.value = "1";
        newObj.appendChild(param2);

        var embedObj;
        embedObj = document.createElement("embed");
        embedObj.name = "GameClientFlash";
        embedObj.src = "GameClientFlash.swf";
        embedObj.pluginspage = "http://www.adobe.com/go/getflashplayer";
        embedObj.height = "100%";
        embedObj.width = "100%";
        embedObj.flashVars = "1";
        newObj.appendChild(embedObj);

        me.canvas.appendChild(newObj);

        me.myFlashObj = document.getElementById("GameClientFlash");
    }
    ///this gets exceuted every game cycle
    
    this.mainloop=function()
    {
       // if (me.canvas==null)
       //     return;
      //  me.canvas.innerHTML="running...";
    }
    this.start=function ()
    {
       //  if (me.canvas==null)
       //     return;
      //  me.canvas.innerHTML="starting ...";
    }
    this.end=function ()
    {
        // if (me.canvas==null)
       //     return;
      //  me.canvas.innerHTML="ending ...";
    }
    ///leave empty if got own timing logic
    ///triggered by engine in a loop
    ///this is where you put your drawing logic
    ///if using no javascript gamlet ,leave it unimplemented and implement
    ///drawing in flash instead
    this.draw_Model=function()
    {
         
     //   me.canvas.innerHTML="drawing Model";
    }
    
    //triggered by engine in a loop
    ///this is where you put your synchronization with server  logic
    /// new model holds a Json decoded javascript object
    //that supposed to represent your current game state
    this.sync_Model=function(newModelObj)
    {
        // call flex function and pass newModelObj as a state of current game state
        
        me.myFlashObj.sync_Model(newModelObj);

    }   
    ///leave empty if got own timing logic
    ///triggered by engine in a loop
    ///this is where you put your drawing logic
    ///if using no javascript gamlet ,leave it unimplemented and implement
    ///extrapolateModel (moving) in flash instead
    this.extrapolateModel=function()
    {
       
        
        
       
    }
    ////
    ///
    ///???
    ///
    this.handleControls=function(key)
    {
       
    }
    //returns anything this gamelet wishes to send to the sererv 
    //right now supported GET method only
    this.UpdateServer=function()
    {
//        return me.myModel.UpdateServer();
    }
}
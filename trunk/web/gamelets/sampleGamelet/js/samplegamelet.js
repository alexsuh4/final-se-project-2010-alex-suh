

//the Gamelet Class acts as an interface to the game engine
function Gamelet(_canvas)
{

    this.name="sample gamelet"
    this.canvas=_canvas;
    
    this.myModel=new Model(this.canvas);
    var me=this;
    //this get excecuted before first run and after the new 
    
    this.init=function()
    {
       // me.canvas.innerHTML="intialized";
       
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
         me.myModel.drawModel();
     //   me.canvas.innerHTML="drawing Model";
    }
    
    //triggered by engine in a loop
    ///this is where you put your synchronization with server  logic
    /// new model holds a Json decoded javascript object
    //that supposed to represent your current game state
    this.sync_Model=function(newModelObj)
    {
       

        //var newModelObj = eval("(" + newModel + ")");
       
           
        me.myModel.setModel(newModelObj);

    }   
    ///leave empty if got own timing logic
    ///triggered by engine in a loop
    ///this is where you put your drawing logic
    ///if using no javascript gamlet ,leave it unimplemented and implement
    ///extrapolateModel (moving) in flash instead
    this.extrapolateModel=function()
    {
       
         me.myModel.updateModel();

       
    }
    ////
    ///
    ///???
    ///
    this.handleControls=function(key)
    {
        me.myModel.handleControls(key);
    }
    //returns anything this gamelet wishes to send to the sererv 
    //right now supported GET method only
    this.UpdateServer=function()
    {
        return me.myModel.UpdateServer();
    }
    this.getSelectedObject=function()
    {
        return me.myModel.getSelectedObject();
    }
    this.handleClick=function(x,y)
    {
        me.myModel.handleClick(x,y);
    }

}

//represent the gamelet model (state)
function Model(canvas)
    {
        this.objects=new Array();
        
        this.num_of_objects=5;
        canvas.style.width=80+"%";
        canvas.style.height=80+"%";

        canvas.style.backgroundRepeat="repeat";
        canvas.style.backgroundImage="url(gamelets/sampleGamelet/img/background.bmp)";

        //creatting some obejcts
        
        var i=0;
        var x;//object x position
        var y;//object y position
        var ang;//object angle
        var vel;//object velocity
        for(i=0;i<this.num_of_objects;i++)
        {
            x=Math.floor(Math.random()*400);
            y=Math.floor(Math.random()*400);
            ang=Math.floor(Math.random()*2*3.14);
            vel=Math.floor(Math.random()*4);

            this.objects[i]=new GameObject(x,y,vel,ang,canvas); //(10,10,0.1,2,canvas);
        }
        this.currentToUpdate=0;
        this.currentToDraw=0;
        this.canvas=canvas;
        var me=this;

        

        this.userKey=null;
        this.UpdateServer=function()
        {
            var userKey="";
            var result="";
            if (me.userKey!=null)
            {
                userKey=me.userKey;
                me.userKey=null;
                result+="userKey>"+userKey+"";
            }
            return result;
        }
        this.handleControls=function(key)
        {
            me.userKey=key;
        }
        this.setModel=function(newModel)
        {
            
            var num_of_objects=newModel.objects.length;
           
            var i=0;
            //update existing object
            for (i=0;i<num_of_objects;i++)
                {
                    me.objects[i].setObject(newModel.objects[i]);
                }

            //book keep added objects
            for (i=me.num_of_objects;i<num_of_objects;i++)
            {
               
                me.objects[i]=new GameObject(0,0,0,0,me.canvas);
              
                me.objects[i].setObject(newModel.objects[i]);
               
            }
           
            //book-keep removed objects
            if (me.num_of_objects>num_of_objects)
            {
                
                    for (i=num_of_objects;i<me.num_of_objects;i++)
                        me.objects[i].destroyMe();
                    me.current=0;
            }
          
            me.num_of_objects=num_of_objects;
           
        }
        this.updateModel=function()
        {
          
            //update the Model
            
            me.objects[me.currentToUpdate].move();
            me.currentToUpdate++;
            if (me.currentToUpdate>=me.num_of_objects)
                me.currentToUpdate=0;
        }
        this.drawModel=function()
        {

            me.objects[me.currentToDraw].draw();
            me.currentToDraw++;
            if (me.currentToDraw>=me.num_of_objects)
                me.currentToDraw=0;
        }
        this.selectedObject="";

        this.getSelectedObject=function()
        {
         
            return me.selectedObject;

        }

        this.handleClick=function(x,y)
        {
            //some ugly offset code
            x=x-30+0;
            y=y-30+0;
            for (var i=0;i<me.num_of_objects;i++)
                {
                if (
                        (Math.abs(x- me.objects[i].x)<=50)
                        && (Math.abs(y- me.objects[i].y)<=50)
                    )
                    {
                            me.selectedObject=me.objects[i].id;
                            return;
                    }
                }
                 me.selectedObject="";
        }
    }



    /**
     * represents a game object
     * @param x xlocation
     * @param y y location
     * @param vel velocity
     * @param ang angle
     * @param canvas a HTML element relative to which  element is appended
     *          as drawing conteext
     */
        function GameObject(x,y,vel,ang,canvas)
        {
            var me=this;
            this.Guid=function () {
                var me=this;
               this.s4=function ()
               {
                    return (((1+Math.random())*0x10000)|0).toString(16).substring(1);
                }
               this.get=function()
               {
                    return (me.s4()+me.s4()+"-"+me.s4()+"-"+me.s4()+"-"+me.s4()+"-"+me.s4()+me.s4()+me.s4());
               }
            }
            this.id=0;

            //parameters
            
            this.x=x-0;
            this.y=y-0;
            this.vel=vel-0;
            this.ang=ang-0;
            this.objectType="";

            var objPic=document.createElement("img");

            objPic.src="gamelets/sampleGamelet/img/skel.gif";
            objPic.style.position="absolute";
            objPic.style.width=50+"px"
            objPic.style.height=50+"px";
            
           
            canvas.appendChild(objPic);
           
            this.draw_context=0;
            
            this.draw_context=objPic;

            this._canvas=canvas;

            this.destroyMe=function()
            {
                
                if (me.draw_context.parentNode)
                    me.draw_context.parentNode.removeChild(me.draw_context);
                 //me._canvas.removeChild(me.draw_context);
                
            }
            this.draw=function()
            {
                

               if (me.draw_context==null || me.draw_context.style==undefined)
                    return;
                me.draw_context.style.position="relative";
                me.draw_context.style.left=me.x+"px";//me.x+"px";
                me.draw_context.style.top=me.y+"px";//me.y+"px";

                 
            }
           
            //move action
            this.move=function()
            {
               
                 
                var newX=Number(me.x) + me.vel*Math.cos(me.ang);
                var newY=Number(me.y) + me.vel*Math.sin(me.ang);
                me.x=newX;
                me.y=newY;

                 
            }
            //setst GameObject with new parameters
            this.setObject=function(newObject)
            {
               

                me.x=newObject.x;
                me.y=newObject.y;
                me.vel=newObject.vel;
                me.ang=newObject.ang;
                me.id=newObject.id;
                if (newObject.objectType && newObject.objectType!=me.objectType )
                {
                    if (newObject.objectType=="portal")
                    {
                        me.draw_context.src="gamelets/sampleGamelet/img/portal.gif";
                    }
                    if (newObject.objectType=="boy")
                    {
                        me.draw_context.src="gamelets/sampleGamelet/img/boy.gif";
                    }
                    if (newObject.objectType=="girl")
                    {
                        me.draw_context.src="gamelets/sampleGamelet/img/girl.gif";
                    }
                }
                
            }
        }
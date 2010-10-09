function ChatSystsem(container,currentplayer)
{
    
    this.ChatControl=function(container,currentplayer)
    {
        this.mainText=0;
        this.inputText=0;
        this.btnSend=0;
        this.chatChrome=0;
        this.container=container;
        this.msgText="";
        this.currentplayer=currentplayer;
        this.to="";
        this.CurrentMessage=0;
        this.isVisible=true;
        var me=this;
        
     this.ChatMessage= function(to,from,body)
        {  
            this.to=to;
            this.from=from;
            this.body=body;
            var me=this;
           
            this.serialize=function()
            {
                if (me.body!="" && me.from!="")
                    {
                        return "messageTo="+ me.to+"&messageBody="+me.body;
                    }
                    else
                        return "";
            }
        }
        this.init=function()
        {
            var outerTable=document.createElement("table")
            outerTable.id="ChatSystem_outerTableID";
            outerTable.class="chatOuterTable";

            var tbody=document.createElement("tbody");
            var upperTRow=document.createElement("tr");
            var middleTrow=document.createElement("tr");
            var lowerTrow=document.createElement("tr");
            var toolbarTrow=document.createElement("tr");

            var toolbarTable=document.createElement("table");
            toolbarTable.style.width="100%";
            var toolbarTBody=document.createElement("tbody");
            var toolbarTableTrow=document.createElement("tr");

            toolbarTable.appendChild(toolbarTBody);
            toolbarTBody.appendChild(toolbarTableTrow);

            var toolbarTds=new Array();
            for (var i=0;i<10;i++)
                {
                    toolbarTds[i]=document.createElement("td");
                    toolbarTds[i].style.width="10%";
                    toolbarTableTrow.appendChild(toolbarTds[i]);
                }
            toolbarTrow.appendChild(toolbarTable);
            

            tbody.appendChild(toolbarTrow);
            tbody.appendChild(upperTRow);
            tbody.appendChild(middleTrow);
            tbody.appendChild(lowerTrow);
            outerTable.appendChild(tbody);

            var toolToggleVisibility=document.createElement("div");
            toolToggleVisibility.onclick=me.toggleVisible;
            toolToggleVisibility.class="chatToolToggleVisibility";
            toolToggleVisibility.id="ChatSystsem_chatToolToggleVisibilityID";
            toolToggleVisibility.innerHTML="---";

            me.chatChrome=document.createElement("div");
            me.chatChrome.class="chatChrome";
            me.chatChrome.id="ChatSystsem_chatChromeID";
            me.mainText=document.createElement("div");
            me.mainText.class="chatMaintext";
            me.mainText.id="ChatSystsem_chatMaintextID"
            me.inputText=document.createElement("input");
            me.inputText.type="text";
            
            me.inputText.class="chatInputText";
            me.inputText.id="ChatSystsem_chatInputTextID"
            me.btnSend=document.createElement("div");
            me.btnSend.innerHTML="Send";
            me.btnSend.class="chatBtnSend";
            me.btnSend.id="ChatSystsem_chatBtnSendID"
            me.btnSend.onclick=me.sendMessage;


            upperTRow.appendChild(me.mainText);
            middleTrow.appendChild(me.inputText);
            lowerTrow.appendChild(me.btnSend);
            toolbarTds[9].appendChild(toolToggleVisibility);
            
            me.chatChrome.appendChild(outerTable);
            
            me.container.appendChild(me.chatChrome);
            
    }
    this.toggleVisible=function()
    {
        
        if (me.isVisible==true)
         {
             me.isVisible=false;
            
            me.mainText.style.visibility="hidden";
            me.inputText.style.visibility="hidden";;
            me.btnSend.style.visibility="hidden";

            me.chatChrome.style.height="2em";
            
            me.mainText.style.height="0";
            me.inputText.style.visibility="0";;
            me.btnSend.style.visibility="0";


         }
         else
         {
             me.isVisible=true;
            me.mainText.style.visibility="visible";
            me.inputText.style.visibility="visible";
            me.btnSend.style.visibility="visible";

           
            me.mainText.style.height="10em";
            me.inputText.style.height="2em";;
            me.btnSend.style.height="";
            me.chatChrome.style.height="";
         }

    }
   
        this.sendMessage=function()
        {
            var msgText= me.inputText.value;

            
            if (msgText==""||me.to==""||!me.to||!msgText)
                return;
            me.to="test message";
            
            var chatMessage=new me.ChatMessage(me.to, me.currentplayer,msgText);
            me.appendMessage(chatMessage);
            me.inputText.value="";
            me.CurrentMessage=chatMessage;
        }
        this.appendMessageText=function(newMsg)
        {
            me.mainText.innerHTML+=newMsg+"<br/>";
        }
        this.appendMessage=function(newMsgObj)
        {
            var d=new Date();
            var time=d.getHours()+":"+d.getMinutes();
           var newMsgText="["+time+"] "+newMsgObj.from+": "+newMsgObj.body;
           me.appendMessageText(newMsgText);
        }
        this.setTo=function(to)
        {
            
            me.to=to;
            
        }
        this.serializeCurrentMessage=function()
        {
            var result="";
            if (me.CurrentMessage)
             {
                //result= "messageTo="+ me.CurrentMessage.to+"&messageBody="+me.CurrentMessage.body;
                result=me.CurrentMessage.serialize();
                me.CurrentMessage=null;
             }
//            if (result!="")
//                alert("serializeCurrentMessage : "+result);
            return result;

        }
    }
    
    this.container=container;
    this.currentplayer=currentplayer;
    var me=this;
    var _chatControl=new me.ChatControl(container,currentplayer);
    _chatControl.init();
    this.chatControl=_chatControl;
    
    this.serializeCurrentMessage=function()
    {
        return me.chatControl.serializeCurrentMessage(container,currentplayer);
    }

    this.setTo=function(playerTo)
    {
        
        me.chatControl.setTo(playerTo);
        
       
    }

}


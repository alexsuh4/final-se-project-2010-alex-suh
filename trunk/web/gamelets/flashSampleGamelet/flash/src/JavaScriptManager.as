package
{
	import flash.events.MouseEvent;
	import flash.external.*;
	import flash.geom.Point;
	import mx.controls.Alert;
	
	// This singletone class contain all gate functions to comunnication with 
	// brousers javascript. 
	public class JavaScriptManager
	{
		// static instance 
		protected static var instance:JavaScriptManager = null;
		
		static public function  Instance(_GamelClientFlash:GameClientFlash):JavaScriptManager
		{
			if ( instance == null )
				instance = new JavaScriptManager(_GamelClientFlash);
			return instance;
		}
		static public function  CurrentInstance():JavaScriptManager
		{
			if ( instance == null )
				throw new Error("Call Instance(_GamelClientFlash:GameClientFlash) to initalize this instance");
			return instance;
		}
		protected var _GamelClientFlash:GameClientFlash;
		// Singletone constructor
		public  function JavaScriptManager(_GamelClientFlash:GameClientFlash)
		{
			if ( instance != null )
				throw new Error( "Only one Singleton instance should be instantiated" ); 
			
			this._GamelClientFlash = _GamelClientFlash;
			//function registartion for JS call
			//Alert.show("initalizng JavaScriptManager , \nExternalInterface.available is "+ExternalInterface.available);
			if ( !ExternalInterface.available)
				return;
			ExternalInterface.addCallback("myFlexFunction",myFunc);	
			ExternalInterface.addCallback("sync_Model", sync_Model);
			ExternalInterface.addCallback("UpdateServer", UpdateServer);
			ExternalInterface.addCallback("getSelectedObject", getSelectedObject);
			ExternalInterface.addCallback("testFunc", testFunc);
			//Alert.show("Callbackes added");
		}
		
		protected var lastClickX:int;
		protected var lastClickY:int;
		protected var isClicked:Boolean;

		public var frameWidth:int = 0;
		public var frameHeight:int = 0;
		
		public function myFunc(s:String):void {
				//testLabel.text=s;
			}
			
		public function sync_Model( isCurrentPlayer:Boolean, modelGuid:String, posX:String, posY:String, angle:String, movingSpeed:String):void
			{
				//Alert.show("sync_Model enter");
				var msg:String;
				msg=
				"sync_Model "
				+"isCurrentPlayer:String =" 			+ 	isCurrentPlayer .toString()
				+" modelGuid:String = " 				+	modelGuid
				+"posX " 								+ 	posX
				+" posY " 								+	posY
				+"angle " 								+ 	angle
				+"movingSpeed " 						+ 	movingSpeed;
				//Alert.show(msg);
				_GamelClientFlash.sync_Model(
				isCurrentPlayer
				,modelGuid, 
				posX
				, posY
				, angle
				, movingSpeed);
				
			}
			public function testFunc(args:Object):void
			{
				var msg:String="";
				
				//update existing objects
				var x:Number;
				var y:Number;
				var id:String;
				var ang:Number;
				var vel:Number;
				var obj:GameObject;
				var objectType:String;
				
				//delete objects not in update
				var currentPlayersids:Array = GameObjectManager.Instance.getActiveIDs();
				var argsArray:Array = args as Array;
				
				if (argsArray != null)
				{
					var tobedeleted:Boolean=true;
					for (var playerindex:int = 0; playerindex < currentPlayersids.length; playerindex++)
					{
						tobedeleted = true;
						//if not in update , remove him
						for (var varindex:int = 0; varindex < argsArray.length; varindex++)
						{
							if (argsArray[varindex].player_id == currentPlayersids[playerindex])
							{
								tobedeleted = false;
								break;
							}
							
						}
						if (tobedeleted)
						{
							GameObjectManager.Instance.removeBaseObject(
									GameObjectManager.Instance.findObjectInArray(
									currentPlayersids[playerindex]));
						}
					}	
				}
				
				//add/update objects
				for (var i:int; i < args.length; i++)
				{
					/*msg += "\n"
					+ "args[" + i + "].x = " 			+ args[i].x
					+",args[" + i + "].y = " 			+ args[i].y
					+",args[" + i + "].ang = " 			+ args[i].ang
					+",args[" + i + "].vel = " 			+ args[i].ang
					+",args[" + i + "].id = " 			+ args[i].player_id*/
					x = args[i].x;
					y = args[i].y;
					id = args[i].player_id;
					ang = args[i].ang;
					vel = 2;//args[i].vel ;
					objectType = args[i].objectType;
					obj = GameObjectManager.Instance.findObjectInArray(id);
					if (obj != null)
					{
						//object exists
						obj.position.x = x;
						obj.position.y = y;
						obj.unitSpeed = vel;
						obj.unitMoveAngle = ang;
					}
					else
					{
						//object does not exist, create it
						try
						{
							if (!ResourceManager.isInitalized) continue;
							if (objectType.toUpperCase()=="PORTAL")
							{
								//Alert.show("portal");
								var gameObject:GameObject = new GameObject();
								gameObject.startupGameObject(id, ResourceManager.MineGraphics, new Point(x, y), 0, 5);
							}
							else
							{
								//player
								var newPlayer:Player;
								newPlayer 						= 	new Player(); 
								newPlayer.startupPlayer(new Point(x, y));
								newPlayer.guidID 				= 	id;
								newPlayer.unitMoveAngle			=	ang;
								newPlayer.unitSpeed				=	vel;
							}
						}
						catch (e:Error)
						{
								//ignore
						}
					}
				}
				
				
				
				
			}
			public function UpdateServer():String 
			{
				if (isClicked)
				{
					//collect click
					isClicked = false;
					return "mX>" + lastClickX.toString() + ";mY>" + lastClickY.toString();
				}
				//no click accured 
				return "";
			}	
			public function click(event:MouseEvent):void
			{
				lastClickX = ViewPort.Instance.getTLpoint().x + event.stageX - (frameWidth / 2);
				lastClickY = ViewPort.Instance.getTLpoint().y + event.stageY - (frameHeight / 2);
				isClicked = true;
			}
			public function getSelectedObject():String 
			{
				return "getSelectedObject():String ";
			}
		
			
			
	}
}
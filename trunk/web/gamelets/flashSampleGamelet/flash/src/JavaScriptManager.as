package
{
	import flash.events.MouseEvent;
	import flash.external.*;
	
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
		
		protected var _GamelClientFlash:GameClientFlash;
		// Singletone constructor
		public  function JavaScriptManager(_GamelClientFlash:GameClientFlash)
		{
			if ( instance != null )
				throw new Error( "Only one Singleton instance should be instantiated" ); 
			
			this._GamelClientFlash = _GamelClientFlash;
			//function registartion for JS call
			if ( !ExternalInterface.available)
				return;
			ExternalInterface.addCallback("myFlexFunction",myFunc);	
			ExternalInterface.addCallback("sync_Model", sync_Model);
			ExternalInterface.addCallback("UpdateServer", UpdateServer);
			ExternalInterface.addCallback("getSelectedObject", getSelectedObject);
			
		}
		
		protected var lastClickX:int;
		protected var lastClickY:int;
		
		public function myFunc(s:String):void {
				//testLabel.text=s;
			}
			
		public function sync_Model( isCurrentPlayer:String, modelGuid:String, posX:String, posY:String, angle:String, movingSpeed:String):void
			{
				_GamelClientFlash.sync_Model(
				isCurrentPlayer
				,modelGuid, 
				posX
				, posY
				, angle
				, movingSpeed);
				
			}
		
			public function UpdateServer():String 
			{
				return "UpdateServer():String ";
			}	
			public function click(event:MouseEvent):void
			{
				lastClickX = event.stageX;
				lastClickY = event.stageY;
			}
			public function getSelectedObject():String 
			{
				return "getSelectedObject():String ";
			}
		
	}
}
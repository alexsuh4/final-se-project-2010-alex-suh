package 
{
	import flash.display.*;
	import flash.events.Event;
	import flash.events.IOErrorEvent;
	import flash.geom.*;
	import flash.net.URLLoader;
	import flash.net.URLRequest;
	import flash.utils.Dictionary;
	
	import mx.controls.Alert;
	import mx.core.*;
		
	
	public final class ResourceManager
	{	
		
		private var resourceCollections:Array = null; 
		public var animationCollection:Array = null;
		
		private var groundEnvironment:Array = null;
		public var environment:Array = null;
		
		private static var classInstance:ResourceManager = null;
		
		private var currentLoaded:int = 0;
		private var maxResources:int = 0;
		
		private var gameLetXML:XML = null;
		
		private var refCallClass:GameClientFlash = null;
		
		public function ResourceManager()
		{
			
		}
		
		public static function get Instance():ResourceManager
		{
			if(classInstance == null )
			{
				classInstance = new ResourceManager();
			}
			
			return classInstance;
		}

		
		// This function parses XML file to get pictures paths and loadit into array
		public function LoadResources(gamelet:XML, refFromCallClass:GameClientFlash):void
		{
			maxResources = 0;
			gameLetXML = gamelet;
			refCallClass = refFromCallClass;
			
			var ActiveObjectsDefinitions:XML = gamelet.ActiveObjectsDefinitions[0];
			maxResources = parseInt(ActiveObjectsDefinitions.@count);
			
			resourceCollections = new Array();
			
			for(var i:int=0; i< ActiveObjectsDefinitions.ActiveObject.length(); i++)
			{
				// get animationObejct and all it's own animations
				var ActiveObject:XML = ActiveObjectsDefinitions.ActiveObject[i];
				// create sub array for pictures to load
				resourceCollections[ActiveObject.@skin_name] = new Array();
				
				for(var act:int = 0; act < ActiveObject.Action.length(); act++)
				{
					var Actions:XML = ActiveObject.Action[act];
					var actionType:String = Actions.@type;
					// load all frames of current animation
					for( var j:int = 0; j < Actions.animation.length(); j++)
					{
						// get single animation object
						var animation:XML = Actions.animation[j];
						var resourceName:String = actionType + "_" + animation.@angle;
						// add new Loader for object
						resourceCollections[ActiveObject.@skin_name][resourceName] = new Loader();					
						
						// register same Event function for each Loader to count loaded resources
						resourceCollections[ActiveObject.@skin_name][resourceName].contentLoaderInfo.addEventListener(Event.COMPLETE, countLoadedResources);
						resourceCollections[ActiveObject.@skin_name][resourceName].contentLoaderInfo.addEventListener(IOErrorEvent.IO_ERROR, ioErrForResource);
						
						// start loading...
						resourceCollections[ActiveObject.@skin_name][resourceName].load(new URLRequest(encodeURI(animation.@src)));
					}
				}
			}
			
			// get list of MAP objects
			var mapObjectsList:XML = gamelet.Map.objectslist[0];
			
			// add to maxResources of animations, number of ground objects
			maxResources = maxResources + parseInt(mapObjectsList.@count);
			
			groundEnvironment = new Array();
			
			// load all frames of current animation
			for( var obj:int = 0; obj < mapObjectsList.object.length(); obj++)
			{
				// get single animation object
				var mapObj:XML = mapObjectsList.object[obj];
				
				
				// add new Loader for object
				groundEnvironment[mapObj.@name] = new Loader();					
				
				// register same Event function for each Loader to count loaded resources
				groundEnvironment[mapObj.@name].contentLoaderInfo.addEventListener(Event.COMPLETE, countLoadedResources);
				groundEnvironment[mapObj.@name].contentLoaderInfo.addEventListener(IOErrorEvent.IO_ERROR, ioErrForResource);
				
				// start loading...
				groundEnvironment[mapObj.@name].load(new URLRequest(encodeURI(mapObj.@src)));
			}
		}
		
		// function to proccess pictures
		public function proccessResources():void
		{
			var ActiveObjectsDefinitions:XML = gameLetXML.ActiveObjectsDefinitions[0];
			
			animationCollection = new Array();
			
			for(var i:int=0; i< ActiveObjectsDefinitions.ActiveObject.length(); i++)
			{
				// get animationObejct and all it's own animations
				var ActiveObject:XML = ActiveObjectsDefinitions.ActiveObject[i];
				
				animationCollection[ActiveObject.@skin_name] = new Array();
				
				for(var act:int = 0; act < ActiveObject.Action.length(); act++)
				{
					var Actions:XML = ActiveObject.Action[act];
					var actionType:String = Actions.@type;
					
					// load all frames of current animation
					for( var j:int = 0; j < Actions.animation.length(); j++)
					{
						// get single animation object
						var animation:XML = Actions.animation[j];
						var resourceName:String = actionType + "_" + animation.@angle;
						// create GraphicsResource object
						animationCollection[ActiveObject.@skin_name][resourceName] = new GraphicsResource(
							resourceCollections[ActiveObject.@skin_name][resourceName].content,
							animation.@frames,
							animation.@fps);
					}
				}
			}
			
			// get list of MAP objects
			var mapObjectsList:XML = gameLetXML.Map.objectslist[0];
			
			environment = new Array();
			
			// load all frames of current animation
			for( var obj:int = 0; obj < mapObjectsList.object.length(); obj++)
			{
				// get single animation object
				var mapObj:XML = mapObjectsList.object[obj];

				environment[mapObj.@name] = new GraphicsResource(groundEnvironment[mapObj.@name].content);
			}
		}
		
		// signal to start proccessing
		public function countLoadedResources(e:Event):void
		{
			currentLoaded++;
			
			// All pictures loaded
			if (maxResources <= currentLoaded)
			{
				proccessResources();
				
				// go back to main class and activate next game STATE
				refCallClass.ResourcesRecieved();
			}
		}
		
		public function ioErrForResource(e:IOErrorEvent):void
		{
			mx.controls.Alert.show("Something goes wrong! Check URLs!");
			throw new Error("Resources_IO_Error");
		}
		
		[Embed(source="../media/cloud.png")]
		public static var Cloud:Class;
		public static var CloudGraphics:GraphicsResource = new GraphicsResource(new Cloud());
		
		
//		[Embed(source="../media/track1.mp3")]
//		public static var Track1Sound:Class;
//		public static var Track1FX:SoundAsset = new Track1Sound() as SoundAsset;	
		
		
		
	}
	
}
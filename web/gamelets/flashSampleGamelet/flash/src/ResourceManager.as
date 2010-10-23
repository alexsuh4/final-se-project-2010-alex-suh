package 
{
	import flash.display.*;
	import flash.events.Event;
	import flash.geom.*;
	import flash.net.URLLoader;
	import flash.net.URLRequest;
	import flash.utils.Dictionary;
	
	import mx.controls.Alert;
	import mx.core.*;
		
	
	public final class ResourceManager
	{	
		
		public var animationsCollections:Array = null; 
		public var environment:Array = null;
		
		private static var classInstance:ResourceManager = null;
		
		private var currentLoaded:int = 0;
		private var maxResources:int = 0;
		
		private var refCallClass:GameClientFlash = null;
		
		public function ResourceManager()
		{
			animationsCollections = new Array();
			environment = new Array();
		}
		
		public static function getInstance():ResourceManager
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
			var onLoadFinish:Function;
			refCallClass = refFromCallClass;
			var ActiveObjectsDefinitions:XML = gamelet.ActiveObjectsDefinitions[0];
			maxResources = ActiveObjectsDefinitions.@count;

			for(var i:int=0; i< ActiveObjectsDefinitions.ActiveObject.length(); i++)
			{
				// get animationObejct and all it's own animations
				var ActiveObject:XML = ActiveObjectsDefinitions.ActiveObject[i];
				// create sub array for pictures to load
				animationsCollections[ActiveObject.@skin_name] = new Array();
				
				// load all frames of current animation
				for( var j:int = 0; j < ActiveObject.animation.length(); j++)
				{
					// get single animation object
					var animation:XML = ActiveObject.animation[j];

					var resourceLoader:Loader = new Loader();					
					// make a request for a picture from XML file
					resourceLoader.load(new URLRequest(animation.@src));
					//mx.controls.Alert.show("1");
					// register Event to count loaded resources
					resourceLoader.addEventListener(Event.COMPLETE, 
						// function to proccess picture
						onLoadFinish = function (e:Event):void
						{
							var loaderInf:LoaderInfo = LoaderInfo(e.target);

							var tempGraphic:GraphicsResource = new GraphicsResource(loaderInf.content,8,10);
							animationsCollections[ActiveObject.@name][animation.@action] = tempGraphic;
							 
							currentLoaded++;
							mx.controls.Alert.show(currentLoaded.toString());
							// All pictures loaded
							if (maxResources <= currentLoaded)
							{
								mx.controls.Alert.show("All resources loaded");
								refCallClass.ResourcesRecieved();
							}
						}
					);
				}
			}
		}

		// function to proccess picture
		private function countLoadedResources(e:Event):void
		{
			currentLoaded++;
			
			// All pictures loaded
			if (maxResources <= currentLoaded)
			{
				mx.controls.Alert.show("All resources loaded");
				refCallClass.ResourcesRecieved();
			}
		}
		
		[Embed(source="../media/cloud.png")]
		public static var Cloud:Class;
		public static var CloudGraphics:GraphicsResource = new GraphicsResource(new Cloud());
		
		
//		[Embed(source="../media/track1.mp3")]
//		public static var Track1Sound:Class;
//		public static var Track1FX:SoundAsset = new Track1Sound() as SoundAsset;	
		
		
		
	}
	
}
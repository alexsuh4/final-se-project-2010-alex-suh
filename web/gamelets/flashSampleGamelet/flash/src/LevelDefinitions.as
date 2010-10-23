package
{
	
	import flash.events.Event;
	import flash.geom.*;
	import flash.net.*;
	import flash.utils.*;
	
	import mx.controls.Alert;
	import mx.core.*;
	
	public class LevelDefinitions
	{
		
		private var definitionXML:XML=null;
		public var map:TiledBackgroundDefinition = null;
		
		protected static var instance:LevelDefinitions = null;
		
		static public function get Instance():LevelDefinitions
		{
			if ( instance == null )
				instance = new LevelDefinitions();
			return instance;
		}
		
		public function LevelDefinitions()
		{
			
		}		
		
		public function startup(myXML:XML):void
		{
			
			this.definitionXML=myXML.Map;
			
			defineLevel1();
			
		}
		
		public function shutdown():void
		{
			
		}
		public function alert(arg:String):void
		{
			mx.controls.Alert.show(arg);
		}
		
		protected function defineLevel1():void
		{
			if (definitionXML==null)
			{
			
				mx.controls.Alert.show("xml file not found !!!");
				return;
			}			
			
			map = new TiledBackgroundDefinition();
			
			map.mapWidth = definitionXML.definitions.mapwidth;
			map.mapHeight = definitionXML.definitions.mapheight;
			
			map.tileWidth = definitionXML.definitions.tilewidth;
			map.tileHeight = definitionXML.definitions.tileheight;
			
			map.tileHeightHalf = map.tileHeight / 2;
			map.tileWidthHalf = map.tileWidth / 2;
			map.scrRowsCount = FlexGlobals.topLevelApplication.height / map.tileHeightHalf;
			map.scrColsCount = FlexGlobals.topLevelApplication.width / map.tileWidth;
			
			// Init Array of Arrays which will hold each layer
			map.mapLayers = new Array();
			var totalLayers:int =  definitionXML.layer.length();
			
			for( var iterLayers:int = 0; iterLayers <  totalLayers; iterLayers++ )
			{
				map.mapLayers[iterLayers] = new Array();
				
				for( var i:int = 0 ; i < map.mapWidth; i++)
				{
					map.mapLayers[iterLayers][i]=new Array();
					for( var j:int = 0 ; j < map.mapHeight; j++)
					{
						map.mapLayers[iterLayers][i][j] = "";
					}
				}
			}
			
			//mx.controls.Alert.show
			for (var layerIndex:int=0 ; layerIndex < definitionXML.layer.length() ; layerIndex++ )
			{
				// get type of current layer
				var layerType:String=definitionXML.layer[layerIndex].@type;
				
				// get constructor of layer: auto / manual
				var layerBuilder:String = definitionXML.layer[layerIndex].@builder;
				
				// list of objects
				var objList:XML=definitionXML.layer[layerIndex];
				
				switch(layerBuilder)
				{
					case "auto":
						// get total count of objects to shuffle
						for( var w:int = 0 ; w < map.mapWidth; w++)
						{
							for( var h:int = 0 ; h < map.mapHeight; h++)
							{
								map.mapLayers[layerIndex][w][h] = objList[MathUtils.randomInteger(1,objList.length())].@type;
							}
						}
						break;
					case "manual":
		
						for (var spriteIndex:int=0; spriteIndex < objList.sprite.length(); spriteIndex++)
						{
							var spriteType:String = objList.sprite[spriteIndex].@type;
							var spriteX:int=objList.sprite[spriteIndex].@x;
							var spriteY:int=objList.sprite[spriteIndex].@y;
					
							alert(spriteType);
							map.mapLayers[layerIndex][spriteX][spriteY] = spriteType;
						}
						break;
					default: // tranparent
						break;
				}
			}													
		}
	}
}
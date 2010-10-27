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
			
			this.definitionXML=myXML.Map[0];
			
			defineLevel();
			
		}
		
		public function shutdown():void
		{
			
		}
		
		protected function defineLevel():void
		{			
			
			map = new TiledBackgroundDefinition();
			
			map.mapWidth = definitionXML.definitions.width;
			map.mapHeight = definitionXML.definitions.height;
			
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
						map.mapLayers[iterLayers][i][j] = "#";
					}
				}
			}
			
			// Fill Arrays
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
								map.mapLayers[layerIndex][w][h] = objList.sprite[MathUtils.randomInteger(1,objList.sprite.length())].@type;
							}
						}
						break;
					case "manual":
						// fill layer according to list of objects
						for (var index:int=0; index < objList.sprite.length(); index++)
						{
							var spriteType:String = objList.sprite[index].@type;
							var colNumber:int=parseInt(objList.sprite[index].@x);
							var rowNumber:int=parseInt(objList.sprite[index].@y);
							
							if(colNumber >= 0 && colNumber < map.mapWidth)
							{
								if(rowNumber >= 0 && rowNumber < map.mapHeight)
								{
									map.mapLayers[layerIndex][rowNumber][colNumber] = spriteType;
								}
							}
						}
						break;
					default: // tranparent
						break;
				}
			}
			
			
			// Show filled layers
//			var str:String = "";
//			for(var lay:int = 0; lay < map.mapLayers.length; lay++)
//			{
//				for(var q:int = 0; q < map.mapWidth; q++)
//				{
//					for(var e:int = 0; e < map.mapWidth; e++)
//					{
//						str = str + map.mapLayers[lay][q][e];
//					}
//					str = str + "\n"
//				}
//				mx.controls.Alert.show(str);
//			}
		}
	}
}
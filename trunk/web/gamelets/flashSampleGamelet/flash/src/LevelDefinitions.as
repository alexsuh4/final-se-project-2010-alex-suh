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
		
		protected static var instance:LevelDefinitions = null;
		protected var levelDefinitions:Dictionary = new Dictionary();
		public var levelTileMaps:Dictionary = new Dictionary();
		
		static public function get Instance():LevelDefinitions
		{
			if ( instance == null )
				instance = new LevelDefinitions();
			return instance;
		}
		
		public function LevelDefinitions()
		{
			
		}
		
		public function addLevelDefinition(levelID:int, element:LevelDefinitionElement):void
		{
			if (levelDefinitions[levelID] == null)
				levelDefinitions[levelID] = new Array();
			
			(levelDefinitions[levelID] as Array).push(element);
			
			levelDefinitions[levelID].sort(LevelDefinitionElement.sort);
		}
		
		public function getNextLevelDefinitionElements(levelID:int, lastTime:Number):Array
		{
			var returnArray:Array = new Array();
			var nextTime:Number = -1;
			
			if (levelDefinitions[levelID] != null)
			{
				for each (var levelDefElement:LevelDefinitionElement in levelDefinitions[levelID])
				{
					if (levelDefElement.time > lastTime && nextTime == -1)
					{
						returnArray.push(levelDefElement);
						nextTime = levelDefElement.time;
					}
					else if (levelDefElement.time == nextTime)
					{
						returnArray.push(levelDefElement);
					}
					else if (levelDefElement.time > nextTime && nextTime != -1)
						break;
				}			
			}
			
			return returnArray.length == 0?null:returnArray;
		}
		
		
		public function getNextLevelID(levelID:int):int
		{
			if (levelDefinitions[levelID + 1] == null) return 0;
			return levelID + 1;
		}
		
		
		public function startup(myXML:XML):void
		{
			
			this.definitionXML=myXML
			
			
			GameObjectManager.Instance.addCollidingPair(CollisionIdentifiers.PLAYER, CollisionIdentifiers.ENEMY);
			GameObjectManager.Instance.addCollidingPair(CollisionIdentifiers.ENEMY, CollisionIdentifiers.PLAYERWEAPON);
			GameObjectManager.Instance.addCollidingPair(CollisionIdentifiers.PLAYER, CollisionIdentifiers.ENEMYWEAPON);
			
			
			defineLevel1();
			//defineLevel1();	
			//defineLevel2();			
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
			
			var map:TiledBackgroundDefinition = new TiledBackgroundDefinition();
			levelTileMaps[1] = map;
			
			map.mapWidth = definitionXML.definitions.mapwidth;
			map.mapHeight = definitionXML.definitions.mapheight;
			
			map.tileWidth = definitionXML.definitions.tilewidth;
			map.tileHeight = definitionXML.definitions.tileheight;
			
			map.tileHeightHalf = map.tileHeight / 2;
			map.tileWidthHalf = map.tileWidth / 2;
			map.scrRowsCount = FlexGlobals.topLevelApplication.height / map.tileHeightHalf;
			map.scrColsCount = FlexGlobals.topLevelApplication.width / map.tileWidth;
			
			
			map.tiles=new Array();
			map.nature=new Array();
			

			for( var i:int = 0 ; i < map.mapWidth; i++)
			{

				map.tiles[i]=new Array();
				map.nature[i]=new Array();
				for( var j:int = 0 ; j < map.mapHeight; j++)
				{
					map.tiles[i][j]=(int)(Math.random()*5);
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
						
						break;
					case "manual":
						switch(layerType)
						{
							case "natureLayer":
								
								for (var spriteIndex:int=0; spriteIndex < objList.sprite.length(); spriteIndex++)
								{
									var spriteType:String = objList.sprite[spriteIndex].@type;
									var spriteX:int=objList.sprite[spriteIndex].@x;
									var spriteY:int=objList.sprite[spriteIndex].@y;

									switch(spriteType)
									{
										case "mine":
											alert("mine");
											map.nature[spriteX][spriteY]=1;
											break;
										case "big_tree":
											alert("big_tree");
											map.nature[spriteX][spriteY]=2;
											break;
										case "farm":
											alert("farm");
											map.nature[spriteX][spriteY]=3;
											break;
										case "lumberjack":
											alert("lumberjack");
											map.nature[spriteX][spriteY]=3;
											break;
										default:
											map.nature[spriteX][spriteY]=0;
											break;
									}
								}
								break;
							default:
								break;
						}
						
				}
			}
			
			

			
//			level1Tiles.environment =
//					[
//						 [0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,	[0000, 0000, 0000, 0000, 0000, 0000, 0000, 0001, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]	
//					];
//			
//			level1Tiles.nature = 		
//					[
//						 [0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0003, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0000, 0000, 0000, 0002, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0001, 0000, 0000, 0000, 0000, 0000, 0003, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0000, 0000, 0000, 0000, 0001, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0000, 0000, 0001, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0001, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0000, 0000, 0000, 0001, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//						,[0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000]
//					];
																
		}
	}
}
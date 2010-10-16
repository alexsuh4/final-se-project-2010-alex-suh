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
		
		private var myXML:XML=null;
		
		
		
		public function startup(myXML:XML):void
		{
			
			this.myXML=myXML
			
			
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
			if (myXML==null)
			{
			
				mx.controls.Alert.show("xml file not found !!!");
				return;
			}			
			
			var level1Tiles:TiledBackgroundDefinition = new TiledBackgroundDefinition();
			levelTileMaps[1] = level1Tiles;
			level1Tiles.tileHeight = 32;
			level1Tiles.tileWidth = 64;
			level1Tiles.tileHeightHalf = level1Tiles.tileHeight / 2;
			level1Tiles.tileWidthHalf = level1Tiles.tileWidth / 2;
			level1Tiles.scrRowsCount = FlexGlobals.topLevelApplication.height / level1Tiles.tileHeightHalf;
			level1Tiles.scrColsCount = FlexGlobals.topLevelApplication.width / level1Tiles.tileWidth;
			level1Tiles.tiles=new Array();
			
			level1Tiles.nature=new Array();
			
			//base layer
			var baseLayer:String=myXML.baselayer;
			
			switch(baseLayer)
			{
				case "grass":
					
					for( var i:int = 0 ; i < 50; i++)
					{
						//mx.controls.Alert.show("i="+i);
						level1Tiles.tiles[i]=new Array();
						level1Tiles.nature[i]=new Array();
						for( var j:int = 0 ; j < 50; j++)
						{
							level1Tiles.tiles[i][j]=(int)(Math.random()*5);
						}
					}
					
					break;
				default:
					break;
			}
			
			//mx.controls.Alert.show
			for (var layerIndex:int=0;layerIndex<myXML.layer.length();layerIndex++)
			{
				var layerType:String=myXML.layer[layerIndex].@type;
				var tiles:XML=myXML.layer[layerIndex];
				switch(layerType)
				{
					case "natureLayer":
						
						for (var spriteIndex:int=0;spriteIndex<tiles.sprite.length();spriteIndex++)
						{
							var spriteType:String=tiles.sprite[spriteIndex].@type;
							var spriteX:int=tiles.sprite[spriteIndex].@x;
							var spriteY:int=tiles.sprite[spriteIndex].@y;
							alert("x="+spriteX+"y="+spriteY+"spriteType="+spriteType);
							switch(spriteType)
							{
								case "mine":
									level1Tiles.nature[spriteX][spriteY]=0;
									break;
								case "bigTree":
									level1Tiles.nature[spriteX][spriteY]=1;
									break;
								case "farm":
									level1Tiles.nature[spriteX][spriteY]=2;
									break;
								case "lumberjack":
									level1Tiles.nature[spriteX][spriteY]=3;
									break;
								default:
									break;
							}
						}
						break;
					default:
						break;
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
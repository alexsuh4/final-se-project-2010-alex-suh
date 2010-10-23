package
{
	import flash.events.*;
	import flash.geom.*;
	import flash.media.*;
	import flash.net.*;
	import flash.utils.*;
	
	import mx.core.*;
	
	public class Level
	{
		protected static var instance:Level = null;
		
		protected static const TimeBetweenClouds:Number = 2.5;
		protected var timeToNextCloud:Number = 0;
		
		protected var tileDefinitions:TiledBackgroundDefinition = null;
		
		
		// UNDO for background music
		// protected var backgroundMusic:SoundChannel = null;

		static public function get Instance():Level
		{
			if ( instance == null )
				instance = new Level();
			return instance;
		}
		
		public function Level()
		{
	
		}
		
		public function startup(levelID:int):void
		{
			new Player().startupPlayer(new Point(200,200));
			//new Enemy().startupBasicEnemy("test1", ResourceManager.WarriorAvatarGraphics, new Point(300, 400), 0, 0);
			// UNDO for background music
			//backgroundMusic = ResourceManager.Track1FX.play(0, int.MAX_VALUE);
						
			tileDefinitions = LevelDefinitions.Instance.map;			
			if (tileDefinitions != null)
				(TiledBackground.pool.ItemFromPool as TiledBackground).startupTiledBackground(tileDefinitions);
		}
		
		public function shutdown():void
		{
			// UNDO for background music
			// backgroundMusic.stop();
			// backgroundMusic = null;
		}
		
		public function enterFrame(dt:Number):void
		{		
			// add cloud
			timeToNextCloud -= dt;
			
			if (timeToNextCloud <= dt)
			{
				timeToNextCloud = TimeBetweenClouds;
				var cloudBackgroundLevelElement:BackgroundLevelElement = BackgroundLevelElement.pool.ItemFromPool as BackgroundLevelElement;
				cloudBackgroundLevelElement.startupBackgroundLevelElement(
					ResourceManager.CloudGraphics, 
					new Point(Math.random() * FlexGlobals.topLevelApplication.width, -ResourceManager.CloudGraphics.bitmap.height),
					ZOrders.CLOUDSBELOWZORDER,
					75);
			}
		}
	}
}
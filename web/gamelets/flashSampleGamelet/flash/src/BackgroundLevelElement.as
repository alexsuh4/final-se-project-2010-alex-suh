package
{
	import flash.geom.Point;
	import mx.core.*;
	import mx.collections.*;

	/**
	 *  Clouds & Birds obejcts (very close to camera obejcts)
	 */
	public class BackgroundLevelElement extends GameObject
	{
		static public var pool:ResourcePool = new ResourcePool(NewBackgroundLevelElement);
		protected var scrollRate:Number = 0;
		
		static public function NewBackgroundLevelElement():BaseObject
		{
			return new 	BackgroundLevelElement();
		}
		
		public function BackgroundLevelElement()
		{
			super();
		}
		
		public function startupBackgroundLevelElement(bitmap:GraphicsResource, position:Point, z:int, scrollRate:Number):void
		{
			startupGameObject("none" ,bitmap, position, 0, z);
			//could move rate
			this.scrollRate = scrollRate; 
		}
		
		public override function enterFrame(dt:Number):void
		{
			if (position.y > FlexGlobals.topLevelApplication.height + graphics.bitmap.height )
				this.shutdown();
			
			// moving of clouds
			position.y += scrollRate * dt;

		}
	}
}
package 
{
	import flash.display.*;
	import flash.geom.*;
	import flash.utils.Dictionary;
	
	import mx.collections.*;

	public class MultiAnimatedGameObject extends AnimatedGameObject
	{
		public static const PI:Number = 3.14;
		public static const PIdiv8:Number = PI/8; 
		
		public static const North:Number = PI / 2;
		public static const South:Number = -PI / 2;
		public static const East:Number = 0;
		public static const West:Number = PI;
		public static const NorthEast:Number = PI / 4;
		public static const NorthWest:Number = 3 * PI / 4;
		public static const SouthEast:Number = -PI / 4;
		public static const SouthWest:Number = -3 * PI / 4;
		
		
		public static const borderNorth:Number = North + PIdiv8 ;
		public static const borderSouth:Number = South + PIdiv8 ;
		public static const borderEast:Number = East + PIdiv8 ;
		public static const borderWest:Number = West + PIdiv8 ;
		public static const borderNorthEast:Number = NorthEast + PIdiv8 ;
		public static const borderNorthWest:Number = NorthWest + PIdiv8 ;
		public static const borderSouthEast:Number = SouthEast + PIdiv8 ;
		public static const borderSouthWest:Number = SouthWest + PIdiv8 ;
		
		public var multiGraphicsArray:Array = null;
		public var animationForAngle:String = "E";
		
		static public function NewMultiAnimatedGameObject():MultiAnimatedGameObject
		{
			return new 	MultiAnimatedGameObject();
		}
				
		public function MultiAnimatedGameObject()
		{
			super();		
		}
		
		public function startupMultiAnimatedGameObject(guid:String ,graphicsArray:Array, position:Point, angle:Number,  z:int = 0, playOnce:Boolean = false):void
		{
			if(graphicsArray == null || graphicsArray.length == 0)
			{
				throw new Error("MultiAnimatedGameObject.graphicsArray:Array - can't be empty or null!");
			}
			multiGraphicsArray = graphicsArray;
			startupGameObject(guid, multiGraphicsArray[0], position, angle, z);
		}
		
		override public function enterFrame(dt:Number):void
		{
			var isAnimationChanged:Boolean = false;
			if (inuse)
			{
				// East
				if(unitMoveAngle <= borderEast && unitMoveAngle >= borderSouthEast && animationForAngle != "E")
				{
					animationForAngle = "E";		
				}			
				// West
				else if(unitMoveAngle <= Math.abs(borderNorthWest) && animationForAngle != "W")
				{
					animationForAngle = "W";
				}
				
				if(isAnimationChanged)
				{
					graphics = multiGraphicsArray[animationForAngle] as GraphicsResource;
					currentFrame = 0;
				}
			}
		}
	}
	
}
package
{
	import flash.geom.Point;
	import flash.geom.Rectangle;
	import flash.geom.Utils3D;
	
	import mx.core.FlexGlobals;
	import mx.utils.UIDUtil;
	
	import spark.primitives.Rect;
/**
 * Represents viewport of the game world.
 * In this viewport only part of the game world is visible in 
 * any given time. 
 */
	public final class ViewPort
	{
		// static instance 
		private static var instance:ViewPort = null;
		
		private var _viewPortWidth:int = 0;
		private var _viewPortHeight:int = 0;
		
		private var globalTLpoint:Point = new Point(0, 0);
		
		public function ViewPort()
		{
			if(instance != null)
			{
				throw new Error( "Screen class: Only one Singleton instance should be instantiated" ); 
			}
			
			_viewPortWidth = FlexGlobals.topLevelApplication.width;
			_viewPortHeight = FlexGlobals.topLevelApplication.height;
			
		}
		
		// returns screen object instance
		static public function get Instance():ViewPort
		{
			if ( instance == null )
				instance = new ViewPort();
			return instance;
		}
		
		/** 
		 * sets the screen to global coordinates
		 * Sets viewport centered on input point (globalPosition)
		*/
		public function setShowArea(newViewPortCenter:Point):void
		{
			var newViewPortCenterX:int;
			var newViewPortCenterY:int;
			
			newViewPortCenterX=newViewPortCenter.x-_viewPortWidth / 2;
			newViewPortCenterY=newViewPortCenter.y-_viewPortHeight / 2;
			
			globalTLpoint = new Point(newViewPortCenterX, newViewPortCenterY);
		}
		
		// returns global coordinates of ViewPort corner (Top,Left)
		public function getTLpoint():Point
		{
			return globalTLpoint;
		}
		
		// checks if an object should be drawn to the screen
		// null - object out off screen
		// Point - coordinates for drawing object
		public function drawPositionOnScreen(globalObjectPosition:Point, objectRect:Rectangle):Point
		{
			if((globalObjectPosition.x <= globalTLpoint.x + _viewPortWidth) && 
				(globalObjectPosition.x + objectRect.width >= globalTLpoint.x))
			{
				if((globalObjectPosition.y <= globalTLpoint.y + _viewPortHeight) && 
					(globalObjectPosition.y + objectRect.height >= globalTLpoint.y))
					
					// point where to render image
					return new Point(globalObjectPosition.x - globalTLpoint.x,
										globalObjectPosition.y - globalTLpoint.y);
			}
			
			// otherwise don't render
			return null;
		}

		public function get viewPortWidth():int
		{
			return _viewPortWidth;
		}

		public function set viewPortWidth(value:int):void
		{
			_viewPortWidth = value;
		}

		public function get viewPortHeight():int
		{
			return _viewPortHeight;
		}

		public function set viewPortHeight(value:int):void
		{
			_viewPortHeight = value;
		}

		


	}
}
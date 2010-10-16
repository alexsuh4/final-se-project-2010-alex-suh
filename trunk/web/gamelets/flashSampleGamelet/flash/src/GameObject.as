package
{
	import flash.display.*;
	import flash.geom.*;
	
	/*
		The base class for all objects in the game.
	*/
	public class GameObject extends BaseObject
	{
		public var guidID:String = "";
		// object position
		public var position:Point = null;
		// object angel
		public var unitMoveAngle:Number = 0;
		// current speed of moving unit
		public var unitSpeed:Number = 0;
		// the bitmap data to display
		public var graphics:GraphicsResource = null;
		public var collisionArea:Rectangle;
		public var collisionName:String = CollisionIdentifiers.NONE;	
		
		public function get CollisionArea():Rectangle
		{
			return new Rectangle(position.x, position.y, collisionArea.width, collisionArea.height);
		}	
		
		public function GameObject()
		{
			super();
		}
		
		public function startupGameObject( guid:String, graphics:GraphicsResource, position:Point, angle:Number, zOrder:int = 0):void
		{
			if (!inuse)
			{
				guidID = guid;
				super.startupBaseObject(zOrder);			
				this.graphics = graphics;
				this.position = position.clone();
				unitMoveAngle = angle;
				setupCollision();
			}
		}
		
		override public function shutdown():void
		{
			if (inuse)
			{				
				super.shutdown();
				graphics = null;							
			}
		}
		
		override public function copyToBackBuffer(db:BitmapData):void
		{
			if (inuse)
			{
				var drawObjPoint:Point = ViewPort.Instance.drawPositionOnScreen(position, graphics.bitmap.rect);
				if(drawObjPoint != null)
				{
					db.copyPixels(graphics.bitmap, graphics.bitmap.rect, drawObjPoint, graphics.bitmapAlpha, new Point(0, 0), true);
				}
			}
		}
		
		protected function setupCollision():void
		{
			collisionArea = graphics.bitmap.rect;
		}
		
		public function getCenterCords():Point
		{
			return new Point(position.x + this.collisionArea.width/2 ,position.y+this.collisionArea.height/2);
		}
	}
}
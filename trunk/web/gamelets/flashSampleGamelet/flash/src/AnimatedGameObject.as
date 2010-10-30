package 
{
	import flash.display.*;
	import flash.geom.*;
	
	import mx.collections.*;

	public class AnimatedGameObject extends GameObject
	{
		static public var pool:ResourcePool = new ResourcePool(NewAnimatedGameObject);
		public var frameTime:Number = 0;
		public var currentFrame:int = 0;
		public var frameWidth:int = 0;
		public var frameHeight:int = 0;
		
		public var playOnce:Boolean = false;
		
		static public function NewAnimatedGameObject():AnimatedGameObject
		{
			return new 	AnimatedGameObject();
		}
				
		public function AnimatedGameObject()
		{
			super();		
		}
		
		public function startupAnimatedGameObject(guid:String ,graphics:GraphicsResource, position:Point, angle:Number,  z:int = 0, playOnce:Boolean = false):void
		{			
			this.playOnce = playOnce;			
			this.frameWidth = graphics.bitmap.width / graphics.frames;
			this.frameHeight = graphics.bitmap.height;
			
			JavaScriptManager.CurrentInstance().frameHeight = this.frameHeight;
			JavaScriptManager.CurrentInstance().frameWidth = this.frameWidth;
			
			
			startupGameObject(guid, graphics, position, angle, z);
		}
		
		override public function enterFrame(dt:Number):void
		{
			if (inuse)
			{
				frameTime += dt;
				if (graphics.fps != -1)
				{
					while (frameTime > 1/graphics.fps)
					{
						frameTime -= 1/graphics.fps;
						currentFrame = (currentFrame + 1) % graphics.frames;
						if (currentFrame == 0 && playOnce)
						{
							shutdown();
							break;
						}
					}
				}
			}
		}
		
		override public function copyToBackBuffer(db:BitmapData):void
		{
			if (inuse)
			{
				var drawObjPoint:Point = ViewPort.Instance.drawPositionOnScreen(position, graphics.bitmap.rect);
				if(drawObjPoint != null)
				{
					var drawRect:Rectangle = new Rectangle(currentFrame * frameWidth, 0, frameWidth, graphics.bitmap.height);
					db.copyPixels(graphics.bitmap, drawRect, drawObjPoint, graphics.bitmapAlpha, new Point(drawRect.x, 0), true);
				}
			}
		}
		
		override protected function setupCollision():void
		{
			collisionArea = new Rectangle(0, 0, frameWidth, graphics.bitmap.height);
		}
	}
	
}
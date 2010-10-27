package
{
	import flash.events.*;
	import flash.geom.*;
	import flash.media.*;
	
	import mx.core.*;

	public class Player extends MultiAnimatedGameObject
	{
		public static var globalPlayerPos:Point = new Point(0, 0);
		
		public static const myGuidID:String = "current_player_guid";
		
		// target position, mouse pointer clicked
		public var targetPosition:Point = new Point(0, 0);
		
		// object position center
		public var centerPos:Point = new Point(0, 0);
				
		// determine when object pass over the target, and bring to stop moving
		public var tempMoveAngle:Number = 0;
		
		
		public function Player()
		{			
			super();
		}
		
		public function startupPlayer(playerInitPosition:Point):void
		{
			globalPlayerPos = playerInitPosition;
			super.startupMultiAnimatedGameObject(myGuidID, "human_miner", "walk", globalPlayerPos , 0, ZOrders.PLAYERZORDER);
			
			this.collisionName = CollisionIdentifiers.PLAYER;
			
			// center the screen on player
			ViewPort.Instance.setShowArea(this.getCenterCords());
		}
		
		override public function shutdown():void
		{
			super.shutdown();
		}
		
		override public function enterFrame(dt:Number):void
		{
			super.enterFrame(dt);
			
			if(unitSpeed != 0)
			{
				// move player to mouse position
				position.x += unitSpeed * Math.cos( unitMoveAngle ); 
				position.y += unitSpeed * Math.sin(	unitMoveAngle );
				
				// determine if object need to stop
				tempMoveAngle = Math.atan2(targetPosition.y - position.y,targetPosition.x - position.x);
				if((tempMoveAngle * unitMoveAngle) < 0)
				{
					unitSpeed = 0;
				}
				
				// keep player on the map
//				if (position.x < 0)
//					position.x = 0;
//				if (position.x > Application.application.width - graphics.bitmap.width / graphics.frames)
//					position.x = Application.application.width - graphics.bitmap.width / graphics.frames;
//				if (position.y < 0)
//					position.y = 0;
//				if (position.y > Application.application.height - graphics.bitmap.height )
//					position.y = Application.application.height - graphics.bitmap.height ;	
				
				ViewPort.Instance.setShowArea(this.getCenterCords());
			}
		}
		
		override public function mouseMove(event:MouseEvent):void
		{
			
		}
		
		override public function mouseDown(event:MouseEvent):void
		{

		}
		
		override public function mouseUp(event:MouseEvent):void
		{			
			// set target position where the center of unit to move
			// substruct unit half-width and half-height
			targetPosition.x = ViewPort.Instance.getTLpoint().x + event.stageX - (frameWidth / 2);
			targetPosition.y = ViewPort.Instance.getTLpoint().y + event.stageY - (frameHeight / 2);
			
			// calculate unit center
			//centerPos.x = position.x + (frameWidth / 2);
			//centerPos.y = position.y + (frameHeight / 2);
				
			unitMoveAngle = Math.atan2(targetPosition.y - position.y, targetPosition.x - position.x);
			
			unitSpeed = 1.1;

		}
		
		override public function collision(other:BaseObject):void
		{
			
			// Death animation
//			var animatedGameObject:AnimatedGameObject = AnimatedGameObject.pool.ItemFromPool as AnimatedGameObject;
//			animatedGameObject.startupAnimatedGameObject(
//				ResourceManager.BigExplosionGraphics, 
//				new Point(
//					position.x + graphics.bitmap.width / graphics.frames / 2 - ResourceManager.BigExplosionGraphics.bitmap.width / ResourceManager.BigExplosionGraphics.frames / 2, 
//					position.y + graphics.bitmap.height / 2 - ResourceManager.BigExplosionGraphics.bitmap.height / 2), 
//				ZOrders.PLAYERZORDER, 
//				true);			
			this.shutdown();
		}
	}
}
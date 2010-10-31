package
{
	import flash.events.*;
	import flash.geom.*;
	import flash.media.*;
	import mx.controls.Alert;
	
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
			//ViewPort.Instance.setShowArea(this.getCenterCords());
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
				//super
				super.updateAction("walk");
				// move player to mouse position
				position.x += unitSpeed * Math.cos( unitMoveAngle ); 
				position.y += unitSpeed * Math.sin(	unitMoveAngle );
				
				// determine if object need to stop
				tempMoveAngle = Math.atan2(targetPosition.y - position.y,targetPosition.x - position.x);
				//if((tempMoveAngle * unitMoveAngle) < 0)
				//{
				//	unitSpeed = 0;
				//	super.updateAction("stopped");
				//}
				
				if (currentPlayer == this)
				{
					ViewPort.Instance.setShowArea(this.getCenterCords());
				}
			}
			else
			{
				super.updateAction("stopped");
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
			//targetPosition.x = ViewPort.Instance.getTLpoint().x + event.stageX - (frameWidth / 2);
			//targetPosition.y = ViewPort.Instance.getTLpoint().y + event.stageY - (frameHeight / 2);
			var pos:Point=new Point();
			pos.x = ViewPort.Instance.getTLpoint().x + event.stageX - (frameWidth / 2);
			pos.y = ViewPort.Instance.getTLpoint().y + event.stageY - (frameHeight / 2);
			if (Math.abs(pos.x - position.x) <= 30 && Math.abs(pos.x - position.x) <= 30)
			{
				//Alert.show("Clicked on " + this.guidID);
				JavaScriptManager.CurrentInstance().selectedObjectID = this.guidID;
			}
			//unitMoveAngle = Math.atan2(targetPosition.y - position.y, targetPosition.x - position.x);
			
			//unitSpeed = 1.1;
			
			JavaScriptManager.CurrentInstance().click(event);
			
			//super.updateAction("walk");

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
			//this.shutdown();
		}
	}
}
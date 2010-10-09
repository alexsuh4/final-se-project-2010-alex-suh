package
{
	import flash.events.*;
	import flash.geom.*;
	import flash.media.*;
	
	import mx.core.*;

	public class Player extends AnimatedGameObject
	{
		public static var globalPlayerPos:Point = new Point(0, 0);
		// moving flag
		protected var unitIsMoving:Boolean = false;
		
		// target position, mouse clicked
		protected var targetPosition:Point = new Point(0, 0);
		// object position center
		protected var centerPos:Point = new Point(0, 0);
		
		// current speed of moving unit
		protected var unitSpeed:Number = 0;
		
		protected var unitMoveAngle:Number = 0;
		
		// determine when object pass over the target, and bring to stop moving
		protected var tempMoveAngle:Number = 0;
		
		protected static const TimeBetweenShots:Number = 0.25;
		protected var shooting:Boolean = false;
		protected var timeToNextShot:Number = 0;
		
		public function Player()
		{			
			super();
		}
		
		public function startupPlayer():void
		{
			globalPlayerPos = new Point(100, 100);
			super.startupAnimatedGameObject(ResourceManager.WarriorAvatarGraphics, globalPlayerPos , ZOrders.PLAYERZORDER);
			// center the screen on player
			shooting = false;
			timeToNextShot = 0;
			this.collisionName = CollisionIdentifiers.PLAYER;
			ViewPort.Instance.setShowArea(this.getCenterCords());
		}
		
		override public function shutdown():void
		{
			super.shutdown();
		}
		
		override public function enterFrame(dt:Number):void
		{
			super.enterFrame(dt);
			
			if(unitIsMoving)
			{
				// move player to mouse position
				position.x += unitSpeed * Math.cos( unitMoveAngle ); 
				position.y += unitSpeed * Math.sin(	unitMoveAngle );
				
				// determine if object need to stop
				tempMoveAngle = Math.atan2(targetPosition.y - position.y,targetPosition.x - position.x);
				if((tempMoveAngle * unitMoveAngle) < 0)
				{
					unitIsMoving = false;
				}
				
				// keep player on the screen
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
			
			/*
			timeToNextShot -= dt;
			if (timeToNextShot <= 0 && shooting)
			{
				timeToNextShot = TimeBetweenShots;
				var weapon:Weapon = Weapon.pool.ItemFromPool as Weapon;
				weapon.startupBasicWeapon(
					ResourceManager.TwoBulletsGraphics,
					new Point(
						position.x + graphics.bitmap.width / graphics.frames / 2 - ResourceManager.TwoBulletsGraphics.bitmap.width / 2, 
						position.y),
					150,
					true);
				
				ResourceManager.Gun1FX.play();
			
			}
			*/
		}
		
		override public function mouseMove(event:MouseEvent):void
		{
			
		}
		
		override public function mouseDown(event:MouseEvent):void
		{
			shooting = true;
		}
		
		override public function mouseUp(event:MouseEvent):void
		{
			shooting = false;		
			
			// set target position where the center of unit to move
			//	substruct unit half-width and half-height
			targetPosition.x = ViewPort.Instance.getTLpoint().x + event.stageX - (frameWidth / 2);
			targetPosition.y = ViewPort.Instance.getTLpoint().y + event.stageY - (frameHeight / 2);
			
			// calculate unit center
			//centerPos.x = position.x + (frameWidth / 2);
			//centerPos.y = position.y + (frameHeight / 2);
				
			unitMoveAngle = Math.atan2(targetPosition.y - position.y, targetPosition.x - position.x);
			unitSpeed = 1.2;
			
			// start moving flag
			unitIsMoving = true;

		}
		
		override public function collision(other:BaseObject):void
		{
			Level.Instance.levelEnd = true;
			
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
			
			// only play the sound if we didn't crash into an enemy, because an
			// enemy is already playing an explosion sound
			if (other as Enemy == null)
				ResourceManager.ExplosionFX.play();
		}
	}
}
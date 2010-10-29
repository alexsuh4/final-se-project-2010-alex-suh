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
		
		
		public static const NorthLeftBrd:Number = North + PIdiv8 ;
		public static const NorthRightBrd:Number = North - PIdiv8 ;
		public static const SouthRightBrd:Number = South + PIdiv8 ;
		public static const SouthLeftBrd:Number = South - PIdiv8 ;
		
		public static const EastTopBrd:Number = East + PIdiv8 ;
		public static const EastLowBrd:Number = East - PIdiv8 ;
		public static const WestTopBrd:Number = West - PIdiv8 ;
		public static const WestLowBrd:Number = - West + PIdiv8 ;
		
		protected var isAnimationChanged:Boolean = false;
		
		public var animationAngleString:String = "";
		public var currentSkinName:String = "";
		public var currentActionName:String = "";
		
		static public function NewMultiAnimatedGameObject():MultiAnimatedGameObject
		{
			return new 	MultiAnimatedGameObject();
		}
				
		public function MultiAnimatedGameObject()
		{
			super();		
		}
		
		public function startupMultiAnimatedGameObject(guid:String ,skinName:String, actionName:String, position:Point, angle:Number = North,  z:int = 0, playOnce:Boolean = false):void
		{
			// store unit look (skin) according to XML
			currentSkinName = skinName;
			
			// store action name, like: walk, stand, run ...
			currentActionName = actionName;
			
			// store direction as string
			animationAngleString = convertAngleToString(angle);
			
			super.startupAnimatedGameObject(guid, ResourceManager.Instance.animationCollection[currentSkinName][currentActionName + "_" + animationAngleString] as GraphicsResource, position, angle, ZOrders.PLAYERZORDER);
		}
		
		override public function enterFrame(dt:Number):void
		{
			if (inuse)
			{
				updateAnimation();
				super.enterFrame(dt);	
			}
		}
		
		public function convertAngleToString(_angle:Number):String
		{
			// East
			if(_angle <= EastTopBrd && _angle >= EastLowBrd)
			{
				return "E";
			}			
				// West
			else if(_angle <= WestLowBrd || _angle >= WestTopBrd)
			{
				return "W";
			}
				// South direction actualy. Positive direction is down (North)
			else if(_angle <= NorthLeftBrd && _angle >= NorthRightBrd)
			{
				return "S";
			}
				// North direction actualy. Negative direction is up (South)
			else if(_angle <= SouthRightBrd && _angle >= SouthLeftBrd)
			{
				return "N";
			}
				// SouthEast (NorthEast)
			else if(_angle <= NorthRightBrd && _angle >= EastTopBrd)
			{
				return "SE";
			}
				// NorthEast (SouthEast)
			else if(_angle <= EastLowBrd && _angle >= SouthRightBrd)
			{
				return "NE";
			}
				// SouthWest (NorthWest)
			else if(_angle <= WestTopBrd && _angle >= NorthLeftBrd)
			{
				return "SW";
			}
				// NorthWest (SouthWest)
			else // if(_angle <= SouthLeftBrd && _angle >= WestLowBrd)
			{
				return "NW";
			}
		}
		
		public function updateAnimation():void
		{
			isAnimationChanged = false;
			
			// East
			if(unitMoveAngle <= EastTopBrd && unitMoveAngle >= EastLowBrd && animationAngleString != "E")
			{
				animationAngleString = "E";
				isAnimationChanged = true;
			}			
			// West
			else if((unitMoveAngle <= WestLowBrd || unitMoveAngle >= WestTopBrd) && animationAngleString != "W")
			{
				animationAngleString = "W";
				isAnimationChanged = true;
			}
			// South direction actualy. Positive direction is down (North)
			else if(unitMoveAngle <= NorthLeftBrd && unitMoveAngle >= NorthRightBrd && animationAngleString != "S")
			{
				animationAngleString = "S";
				isAnimationChanged = true;
			}
			// North direction actualy. Negative direction is up (South)
			else if(unitMoveAngle <= SouthRightBrd && unitMoveAngle >= SouthLeftBrd && animationAngleString != "N")
			{
				animationAngleString = "N";
				isAnimationChanged = true;
			}
			// SouthEast (NorthEast)
			else if(unitMoveAngle <= NorthRightBrd && unitMoveAngle >= EastTopBrd && animationAngleString != "SE")
			{
				animationAngleString = "SE";
				isAnimationChanged = true;
			}
			// NorthEast (SouthEast)
			else if(unitMoveAngle <= EastLowBrd && unitMoveAngle >= SouthRightBrd && animationAngleString != "NE")
			{
				animationAngleString = "NE";
				isAnimationChanged = true;
			}
			// SouthWest (NorthWest)
			else if(unitMoveAngle <= WestTopBrd && unitMoveAngle >= NorthLeftBrd && animationAngleString != "SW")
			{
				animationAngleString = "SW";
				isAnimationChanged = true;
			}
			// NorthWest (SouthWest)
			else if(unitMoveAngle <= SouthLeftBrd && unitMoveAngle >= WestLowBrd && animationAngleString != "NW")
			{
				animationAngleString = "NW";
				isAnimationChanged = true;
			}
			
			if(isAnimationChanged)
			{
				graphics = ResourceManager.Instance.animationCollection[currentSkinName][currentActionName + "_" + animationAngleString] as GraphicsResource;
				currentFrame = 0;
			}			
		}
		
		public function updateSkin(newSkinName:String, newAction:String):void
		{
			if(newSkinName != this.currentSkinName)
			{
				this.currentSkinName = newSkinName;
				graphics = ResourceManager.Instance.animationCollection[newSkinName][newAction + "_" + animationAngleString] as GraphicsResource;
				currentFrame = 0;
			}
		}
		
		public function updateAction(newAction:String):void
		{
			if(newAction != this.currentActionName)
			{
				this.currentActionName = newAction;
				graphics = ResourceManager.Instance.animationCollection[currentSkinName][newAction + "_" + animationAngleString] as GraphicsResource;
				currentFrame = 0;
			}
		}
	}
	
}
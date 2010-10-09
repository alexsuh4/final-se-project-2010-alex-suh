package
{
	
	import flash.display.*;
	import flash.geom.*;
	
	import mx.collections.*;
	import mx.core.*;
	public class TiledBackground extends BaseObject
	{
		public var scrolling:Boolean = true;
		protected var yOffset:Number = 0;
		protected var xOffset:Number = 0;
		protected var definition:TiledBackgroundDefinition = null;
		
		protected var tilesStorage:Array = null;
		protected var natureStorage:Array = null;
		static public var pool:ResourcePool = new ResourcePool(NewTiledBackground);
		
		static public function NewTiledBackground():TiledBackground
		{
			 return new TiledBackground();
		}
		
		public function TiledBackground()
		{
			super();
		}
		
		public function startupTiledBackground(_definition:TiledBackgroundDefinition):void
		{
			super.startupBaseObject(ZOrders.BACKGROUNDZORDER);
			this.tilesStorage = ResourceManager.getInstance().getTilesLayer();
			this.natureStorage = ResourceManager.getInstance().getNatureLayer();
			this.definition = _definition;
			this.xOffset = definition.tileWidthHalf;
			this.scrolling = true;
		}
		
		override public function shutdown():void
		{
			super.shutdown();
		}
		
		override public function enterFrame(dt:Number):void
		{
//			if (scrolling)
//			{	
//				var mapHeight:int = definition.tiles.length * definition.tileHeight;
//				var mapOverlap:int = mapHeight - FlexGlobals.topLevelApplication.height;
//				if (yOffset > mapOverlap)
//				{
//					scrolling = false;
//					yOffset =  mapOverlap;
//				}
//			}
		}
		
		override public function copyToBackBuffer(db:BitmapData):void
		{
			var graphics:GraphicsResource = null;
			var elementNumber:Number;
			var tlPoint:Point = ViewPort.Instance.getTLpoint();
			var startRowN:int = tlPoint.y / definition.tileHeightHalf -1;
		 	var startColN:int = tlPoint.x / definition.tileWidth -1;	
		 	

			var evenOffsetFlag:Boolean;
			// if starting row is even so don't add offset
			if(startRowN % 2 == 0)
			{
				evenOffsetFlag = true;
			}
			else
			{
				evenOffsetFlag = false;
			}
			
			
		 	//determine upper left point relative to starting pile  to start drawing
			//this is done to draw only visible part of border tiles
			var rowOffsetP:int = definition.tileHeightHalf * startRowN - tlPoint.y;
			var colOffsetP:int = definition.tileWidth * startColN - tlPoint.x;
			
			
			var row:int = 0;
			var col:int = 0;
			// loop through each row of "tiles"
			for (row = 0; row < definition.scrRowsCount +2 ; row++)
			{
				// cyclic flag change
				evenOffsetFlag = !evenOffsetFlag;
				// check for the current row would exists in array
				if( ((startRowN + row) >= 0) && ((startRowN + row) < definition.tiles.length) )
				{
					// loop through each column of the current row
			 		for (col = 0; col < definition.scrColsCount +2 ; col++)
			 		{
						// check for the current col would exist in array
						if( ((startColN + col) >= 0) && ((startColN + col) < definition.tiles[startRowN + row].length) )
						{
				 			graphics = tilesStorage[(definition.tiles[startRowN + row][startColN + col] as Number)] as GraphicsResource;
				 			
				 			if (graphics != null)
				 			{			 							 			
					 			db.copyPixels(
					 				graphics.bitmap,
					 				graphics.drawRect, 
					 				new Point(
					 					col * definition.tileWidth + (evenOffsetFlag?xOffset:0)  + colOffsetP, 
					 					row * definition.tileHeightHalf + rowOffsetP),
					 				graphics.bitmapAlpha, 
					 				new Point(
					 					graphics.drawRect.x, 
					 					graphics.drawRect.y), 
					 				false);
				 			}
						}
			 		}
				}
		 	}

				
			
			// ########### Nature layer ###########################################

			// if starting row is even so don't add offset
			if(startRowN % 2 == 0)
			{
				evenOffsetFlag = true;
			}
			else
			{
				evenOffsetFlag = false;
			}
			
			// loop through each row of "tiles"
			for (row = 0; row < definition.scrRowsCount; row++)
			{
				// cyclic flag change
				evenOffsetFlag = !evenOffsetFlag;
				// check for the current row would exists in array
				if( ((startRowN + row) >= 0) && ((startRowN + row) < definition.nature.length) )
				{
					// loop through each column of the current row
					for (col = 0; col < definition.scrColsCount ; col++)
					{
						// check for the current col would exist in array
						if( ((startColN + col) >= 0) && ((startColN + col) < definition.nature[startRowN + row].length) )
						{
							elementNumber = definition.nature[startRowN + row][startColN + col] as Number;
							if(elementNumber > 0)
							{
								graphics = natureStorage[elementNumber] as GraphicsResource;
								
								if (graphics != null)
								{			 							 			
									db.copyPixels(
										graphics.bitmap,
										graphics.drawRect, 
										new Point(
											col * definition.tileWidth + (evenOffsetFlag?xOffset:0)  + colOffsetP, 
											row * definition.tileHeightHalf + rowOffsetP),
										graphics.bitmapAlpha, 
										new Point(
											graphics.drawRect.x, 
											graphics.drawRect.y), 
										false);
								}
							}
						}
					}
				}
			}
			
	 	}// end
		
	}
}
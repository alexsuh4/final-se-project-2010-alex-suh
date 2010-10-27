package
{
	
	import flash.display.*;
	import flash.geom.*;
	
	import mx.collections.*;
	import mx.core.*;
	public class TiledBackground extends BaseObject
	{
		protected var yOffset:Number = 0;
		protected var xOffset:Number = 0;
		protected var definition:TiledBackgroundDefinition = null;
		
		protected var environmentStorage:Array = null;

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
			super.startupBaseObject(_definition.layerZorder);
			this.definition = _definition;
			this.environmentStorage = ResourceManager.Instance.environment;
			this.xOffset = definition.tileWidthHalf;
		}
		
		override public function shutdown():void
		{
			super.shutdown();
		}
		
		override public function copyToBackBuffer(db:BitmapData):void
		{
			var graphics:GraphicsResource = null;
			var elementNumber:Number;
			var tlPoint:Point = ViewPort.Instance.getTLpoint();
			var startRowN:int = (tlPoint.y / definition.tileHeightHalf) -1;
		 	var startColN:int = (tlPoint.x / definition.tileWidth) -1;	
		 	

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
			
			var renderRow:int = 0;
			var renderCol:int = 0;
			
			var layer:int = 0;
			var row:int = 0;
			var col:int = 0;
			for (layer = 0; layer < definition.mapLayers.length; layer++)
			{
				// loop through each row of "tiles"
				for (row = 0; row < definition.scrRowsCount + 2; row++)
				{
					renderRow = startRowN + row; 
					// cyclic flag change
					evenOffsetFlag = !evenOffsetFlag;
					
					var totalRowLen:int = definition.mapLayers[layer].length;
					// check for the current row would exists in array
					if( (renderRow >= 0) && (renderRow < totalRowLen ))
					{
						// loop through each column of the current row
				 		for (col = 0; col < definition.scrColsCount +2 ; col++)
				 		{
							renderCol = startColN + col;
							var totalColLen:int = definition.mapLayers[layer][renderRow].length;
							// check for the current col would exist in array
							if( (renderCol >= 0) && (renderCol < totalColLen) )
							{
								var keyStorage:String = definition.mapLayers[layer][renderRow][renderCol];
								if(keyStorage != "#" || keyStorage != "" || keyStorage != null)
								{
					 				graphics = environmentStorage[keyStorage] as GraphicsResource;
								
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
			}

	 	}
		
	}
}
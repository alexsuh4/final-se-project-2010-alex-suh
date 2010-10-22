package
{
	// This singletone class contain all gate functions to comunnication with 
	// brousers javascript. 
	public class JavaScriptManager
	{
		// static instance 
		protected static var instance:JavaScriptManager = null;
		
		static public function get Instance():JavaScriptManager
		{
			if ( instance == null )
				instance = new JavaScriptManager();
			return instance;
		}
		
		// Single tone constructor
		private function JavaScriptManager()
		{
			if ( instance != null )
				throw new Error( "Only one Singleton instance should be instantiated" ); 
		}
	}
}
using System;

namespace generate_lat_long
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			string lcLongLabel;
			string lcLatLabel;
			
			int lcMin = -80;
			int lcMax = 80;
			int lcIncrement = int.Parse(args[0]);
			int lcId = 1;
			
			for(int lcLong=lcMin; lcLong <= lcMax; lcLong+=lcIncrement)
			{
				lcLongLabel = lcLong < 0 ? Math.Abs(lcLong).ToString()+"ºW" : Math.Abs(lcLong).ToString()+"ºE";
				Console.WriteLine(String.Format("insert into latlong{0} values ({5}, ST_GeomFromText('LINESTRING({1} {3} , {1} {4})', 4326), '{2}');", lcIncrement, lcLong, lcLongLabel, lcMin, lcMax, lcId++));
			}
				                  
			for(int lcLat=lcMin; lcLat<=lcMax; lcLat+=lcIncrement)
			{
				lcLatLabel = lcLat < 0 ? Math.Abs(lcLat).ToString()+"ºS" : Math.Abs(lcLat).ToString()+"ºN";
				Console.WriteLine(String.Format("insert into latlong{0} values ({5}, ST_GeomFromText('LINESTRING({3} {1}, {4} {1})', 4326), '{2}');", lcIncrement, lcLat, lcLatLabel, lcMin, lcMax, lcId++));
			}
		}
	}
}


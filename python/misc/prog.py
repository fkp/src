import argparse

parser = argparse.ArgumentParser(description='A script to generate OSM tiles in a specific coordinate system')

parser.add_argument("projection", help="The Proj4 projection to use")
parser.add_argument("config", help="The OSM stylesheet master configuration file")
parser.add_argument("minx", type=float, help="The minimum X coordinate (in the destination coordinate system)")
parser.add_argument("miny", type=float, help="The minimum Y coordinate (in the destination coordinate system")
parser.add_argument("maxx", type=float, help="The maximum X coordinate (in the destination coordinate system")
parser.add_argument("maxy", type=float, help="The maximum Y coordinate (in the destination coordinate system")
parser.add_argument("--basedir", help="The base directory to generate the tiles into")

args = parser.parse_args()
print "Projection is: " + args.projection
print "Config file is: " + args.config
print "MinX is: " + str(args.minx)
print "MinY is: " + str(args.miny)
print "MaxX is: " + str(args.maxx)
print "MaxY is: " + str(args.maxy)
if args.basedir is not None:
    print "Base directory is: " + args.basedir

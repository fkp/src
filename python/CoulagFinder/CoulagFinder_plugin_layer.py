# -*- coding: utf-8 -*-
"""
/***************************************************************************
CoulagFinder Plugin
A QGIS plugin
Render coulagfinder showing basic QgsPluginLayer usage

                             -------------------
begin                : 2010-01-28
copyright            : (C) 2010 by Sourcepole
email                : info at sourcepole dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
"""
from PyQt4.QtCore import *
from PyQt4.QtGui import *
from qgis.core import *
from math import radians, cos, sin, asin, sqrt

class CoulagFinder_plugin_layer(QgsPluginLayer):

  LAYER_TYPE="coulagfinder"

  def __init__(self):
    QgsPluginLayer.__init__(self, CoulagFinder_plugin_layer.LAYER_TYPE, "CoulagFinder layer")
    self.setValid(True)

    self.coulagsLatLong = QgsPoint(-5.40491, 57.45018)

  def draw(self, rendererContext):
    painter = rendererContext.painter()
    extent = rendererContext.extent()
    center = extent.center()

    if rendererContext.coordinateTransform() != None:
   	
        currentMapCRS = rendererContext.coordinateTransform().destCRS()
        latLongCRS = QgsCoordinateReferenceSystem(4326, QgsCoordinateReferenceSystem.PostgisCrsId)
        transformToLatLong = QgsCoordinateTransform(currentMapCRS, latLongCRS)

        # setup painter
        painter.save()

        myLocation = QgsPoint(center.x(), center.y())
        myLocationLatLong = transformToLatLong.transform(myLocation)

        # This is the line between the coordinates in Lat/Long so we will take this angle, but it really depends on the
        # coordinate system being used whether this will give the right result..
        distanceLine = QgsGeometry.fromPolyline([myLocationLatLong,self.coulagsLatLong])

        linesVisual = [ QLineF(1, 5, 9, 5), QLineF(9, 5, 7, 9), QLineF(9, 5, 7, 1) ]
        distance = self.haversine(myLocationLatLong.x(), myLocationLatLong.y(), self.coulagsLatLong.x(), self.coulagsLatLong.y())
        painter.drawLines(linesVisual)

        # Useful for debugging
        #currentMapCRS.description()

        painter.drawText(10, 10, "Don't stress, Coulags is only " + str(distance) + " km away from here")
        painter.restore()

    return True

  def haversine(self, lon1, lat1, lon2, lat2):
    """
    Calculate the great circle distance between two points 
    on the earth (specified in decimal degrees)
    """
    # convert decimal degrees to radians 
    lon1, lat1, lon2, lat2 = map(radians, [lon1, lat1, lon2, lat2])
    # haversine formula 
    dlon = lon2 - lon1 
    dlat = lat2 - lat1 
    a = sin(dlat/2)**2 + cos(lat1) * cos(lat2) * sin(dlon/2)**2
    c = 2 * asin(sqrt(a)) 
    km = 6367 * c
    return km 

  def readXml(self, node):
    # custom properties
    return True

  def writeXml(self, node, doc):
    element = node.toElement();
    # write plugin layer type to project  (essential to be read from project)
    element.setAttribute("type", "plugin")
    element.setAttribute("name", CoulagFinder_plugin_layer.LAYER_TYPE);
    # custom properties
    #element.setAttribute("image_path", str(self.image_path))
    return True



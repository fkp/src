# -*- coding: utf-8 -*-
"""
/***************************************************************************
CoulagFinder Plugin
A QGIS plugin
Render CoulagFinder showing basic QgsPluginLayer usage

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

from CoulagFinder_plugin_layer import *

class CoulagFinder_plugin_layer_type(QgsPluginLayerType):

  def __init__(self):
    QgsPluginLayerType.__init__(self, CoulagFinder_plugin_layer.LAYER_TYPE)

  def createLayer(self):
    return CoulagFinder_plugin_layer()

  def showLayerProperties(self, layer):
    layer.showImageDialog()

    # indicate that we have shown the properties dialog
    return True

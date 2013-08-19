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

import resources

from CoulagFinder_plugin_layer import *
from CoulagFinder_plugin_layer_type import *

class CoulagFinder_plugin:

  def __init__(self, iface):
    # Save reference to the QGIS interface
    self.iface = iface

  def initGui(self):
    # Register plugin layer type
    QgsPluginLayerRegistry.instance().addPluginLayerType(CoulagFinder_plugin_layer_type())

    layer = CoulagFinder_plugin_layer()
    QgsMapLayerRegistry.instance().addMapLayer(layer)

  def unload(self):
    # Unregister plugin layer type
    QgsPluginLayerRegistry.instance().removePluginLayerType(CoulagFinder_plugin_layer.LAYER_TYPE)

  def run(self):
    pass


# -*- coding: utf-8 -*-
"""
/***************************************************************************
Watermark Plugin
A QGIS plugin
Render watermark showing basic QgsPluginLayer usage

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
 This script initializes the plugin, making it known to QGIS.
"""
def name():
  return "Coulag Finder Plugin"
def description():
  return "A plugin to keep you informed of how far away Coulags is"
def version():
  return "Version 0.1"
def icon():
  return "icon.png"
def qgisMinimumVersion():
  return "1.5"
def authorName():
  return "FKP"
def homepage():
  return "(none)"
def classFactory(iface):
  from CoulagFinder_plugin import CoulagFinder_plugin
  return CoulagFinder_plugin(iface)

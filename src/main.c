/*
** Jupiter Mass watch face - Color or Mono
**
**    https://github.com/clach04/watchface_JupiterMass
**
** Acknowledgements
**   * This is only a slightly modified version of Pebble Watchface tutorial
**       * http://developer.getpebble.com/getting-started/watchface-tutorial/part2/
**       * https://github.com/pebble-examples/classio-battery-connection/
**   * Ideas for additional extras from
**       * http://clintkprojects.blogspot.com/2014/11/pebble-watchface-saving-battery-life.html
**   * Font ISL Jupiter by Isurus Labs
**       * http://www.dafont.com/isl-jupiter.font
**   * Logo Vector by Karlika
**       * http://karlika.deviantart.com/art/Mass-Effect-Vector-Pack-178422616
**
** TODO
**   * Add Bluetooth status, connect/disconnected is easy.
**     What about Bluetooth off (airplane mode)?
**     Show icons (rather than text)?
**       * battery
**           * https://github.com/Remper/Simplicity2/blob/master/src/simplicity2.c
**           * https://github.com/tkafka/ModernExtra20/tree/master/resources/images
**           * https://github.com/foxel/pebble-simplef/tree/master/resources/images
**       * BT https://github.com/drwrose/rosewright/tree/master/resources
**   * Add shake/tap to display seconds
*/

#include <pebble.h>

#include "watchface.h"

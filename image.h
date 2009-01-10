/*
 * File: image.h Author: Roger Light
 * Project: gen_cross_section
 *
 * Copyright (C) 2006-2009 Roger Light.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <png.h>
#include "layers.h"

void make_image(png_byte **image, layerdef *layers, unsigned int num_layers, char ***cross_section, png_uint_32 imagewidth, png_uint_32 height, png_uint_32 pixelwidth);

#endif

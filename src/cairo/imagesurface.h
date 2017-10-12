/*
  +-----------------------------------------------------------+
  | Copyright (c) 2017 Collet Valentin                        |
  +-----------------------------------------------------------+
  | This source file is subject to version the BDS license,   |
  | that is bundled with this package in the file LICENSE     |
  +-----------------------------------------------------------+
  | Author: Collet Valentin <valentin@famillecollet.com>      |
  +-----------------------------------------------------------+
*/

#ifndef PHP_CAIRO_IMAGE_SURFACE_DEF
#define PHP_CAIRO_IMAGE_SURFACE_DEF

#include <cairo.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "exception.h"
#include "surface.h"

/*************************/
/* Intern Data Structure */
/*************************/

ZEND_BEGIN_ARG_INFO_EX(arginfo_image_surface_construct, 0, 0, 3)
	ZEND_ARG_TYPE_INFO(0, format, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, width, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, height, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 1)
ZEND_END_ARG_INFO()

PGGI_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_image_surface_stride_for_width, 0, 0, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, format, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, width, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_image_surface_write_to_png, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, filename, IS_STRING, 0)
ZEND_END_ARG_INFO()

/**************/
/* properties */
/**************/

#define IMAGE_SURFACE_DATA "data"

/*****************************/
/* Class information getters */
/*****************************/

/*==========================================================================*/
/** 
 * get the class entry
 */
zend_class_entry * pc_image_surface_get_class_entry(void);

/*==========================================================================*/
/** 
 * get the object handler
 */
zend_object_handlers * pc_image_surface_get_object_handlers();


/*****************************/
/* PHP Methods and Functions */
/*****************************/

#define IMAGE_SURFACE_METHOD(name) \
PHP_METHOD(ImageSurface, name)

IMAGE_SURFACE_METHOD(__construct);
IMAGE_SURFACE_METHOD(getData);
IMAGE_SURFACE_METHOD(getFormat);
IMAGE_SURFACE_METHOD(getWidth);
IMAGE_SURFACE_METHOD(getHeight);
IMAGE_SURFACE_METHOD(getStride);
PHP_FUNCTION(strideForWidth);

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *pc_image_surface_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * get properties handling function
 */
HashTable *pc_image_surface_get_properties(zval *object);

/*==========================================================================*/
/** 
 * write property handling function
 */
void pc_image_surface_write_property(zval *object, zval *member, zval *value, void **cache_slot);

/************************/
/* Class Initialization */
/************************/

/*==========================================================================*/
/** 
 * init the class
 */
void pc_image_surface_init(int module_number);


#endif

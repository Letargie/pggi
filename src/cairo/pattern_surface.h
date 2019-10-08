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

#ifndef PHP_CAIRO_PATTERN_SURFACE_DEF
#define PHP_CAIRO_PATTERN_SURFACE_DEF

#include <cairo.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "exception.h"
#include "pattern.h"
#include "surface.h"

ZEND_BEGIN_ARG_INFO_EX(arginfo_pc_pattern_surface_construct, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, x, PGGI\\Cairo\\Surface, 0)
ZEND_END_ARG_INFO()

#define PATTERN_SURFACE_SURFACE "surface"

/*****************************/
/* Class information getters */
/*****************************/

/*==========================================================================*/
/** 
 * get the class entry
 */
zend_class_entry * pc_pattern_surface_get_class_entry(void);

/*==========================================================================*/
/** 
 * get the object handler
 */
zend_object_handlers * pc_pattern_surface_get_object_handlers();

/***************/
/* PHP Methods */
/***************/

#define PATTERN_SURFACE_METHOD(name) \
PHP_METHOD(PatternSurface, name)

/* PatternSurface Functions */

/************************/
/* Class Initialization */
/************************/

/*==========================================================================*/
/** 
 * init the class
 */
void pc_pattern_surface_init(int module_number);





#endif

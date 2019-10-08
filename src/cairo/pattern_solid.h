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

#ifndef PHP_CAIRO_PATTERN_SOLID_DEF
#define PHP_CAIRO_PATTERN_SOLID_DEF

#include <cairo.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "exception.h"
#include "pattern.h"


ZEND_BEGIN_ARG_INFO_EX(arginfo_pc_pattern_solid_construct, 0, 0, 3)
	ZEND_ARG_TYPE_INFO(0, r, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, g, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/*****************************/
/* Class information getters */
/*****************************/

/*==========================================================================*/
/** 
 * get the class entry
 */
zend_class_entry * pc_pattern_solid_get_class_entry(void);

/*==========================================================================*/
/** 
 * get the object handler
 */
zend_object_handlers * pc_pattern_solid_get_object_handlers();

/***************/
/* PHP Methods */
/***************/

#define PATTERN_SOLID_METHOD(name) \
PHP_METHOD(PatternSolid, name)

/* PatternSolid Functions */

/************************/
/* Class Initialization */
/************************/

/*==========================================================================*/
/** 
 * init the class
 */
void pc_pattern_solid_init(int module_number);





#endif

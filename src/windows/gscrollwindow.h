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


#ifndef __GSCROLL_WINDOW_DEF__
#define __GSCROLL_WINDOW_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "../abstracts/gcontainer.h"

/***************************/
/* GScroll_Window Arginfos */
/***************************/

/****************************/
/* GScrollWindow Properties */
/****************************/

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *gscroll_window_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * Get property handling function
 */
HashTable * gscroll_window_get_properties(zval *object);

/*==========================================================================*/
/** 
 * Write property handling function
 */
void gscroll_window_write_property(zval *object, zval *member, zval *value, void **cache_slot);


/***************/
/* PHP Methods */
/***************/

#define GSCROLL_WINDOW_METHOD(name) \
PHP_METHOD(GScrollWindow, name)

GSCROLL_WINDOW_METHOD(__construct);

/**************************************/
/* GScrollWindow class initialisation */
/**************************************/

void gscroll_window_init(int module_number);

#endif

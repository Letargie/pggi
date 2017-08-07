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


#ifndef __GTEXT_VIEW_DEF__
#define __GTEXT_VIEW_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "hub.h"
#include "gcontainer.h"
#include "gtextbuffer.h"

/**********************/
/* GTextView Arginfos */
/**********************/

/************************/
/* GTextView Properties */
/************************/

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *gtext_view_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * Get property handling function
 */
HashTable * gtext_view_get_properties(zval *object);

/*==========================================================================*/
/** 
 * Write property handling function
 */
void gtext_view_write_property(zval *object, zval *member, zval *value, void **cache_slot);


/***************/
/* PHP Methods */
/***************/

#define GTEXT_VIEW_METHOD(name) \
PHP_METHOD(GTextView, name)

GTEXT_VIEW_METHOD(__construct);

/**********************************/
/* GTextView class initialisation */
/**********************************/

void gtext_view_init(int module_number);

#endif

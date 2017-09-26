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


#ifndef __GCELL_RENDERER_TEXT_DEF__
#define __GCELL_RENDERER_TEXT_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "hub.h"
#include "gcellrenderer.h"

/****************************/
/* GCellRenderer properties */
/****************************/

/*****************************/
/* Class information getters */
/*****************************/

zend_class_entry * gcell_renderer_text_get_class_entry(void);

zend_object_handlers * gcell_renderer_text_get_object_handlers();

/***************/
/* PHP Methods */
/***************/

#define GCELL_RENDERER_TEXT_METHOD(name) \
PHP_METHOD(GCellRendererText, name)

GCELL_RENDERER_TEXT_METHOD(__construct);

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *gcell_renderer_text_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * get properties handling function
 */
HashTable *gcell_renderer_text_get_properties(zval *object);

/*==========================================================================*/
/** 
 * write property handling function
 */
void gcell_renderer_text_write_property(zval *object, zval *member, zval *value, void **cache_slot);

/************************************/
/* GCellRenderer Class Initialization */
/************************************/

void gcell_renderer_text_init(int module_number);

/* __GCELL_RENDERER_DEF__ */

#endif

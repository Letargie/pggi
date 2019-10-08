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


#ifndef __GCOMBO_BOX_DEF__
#define __GCOMBO_BOX_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "../abstracts/gcontainer.h"


zend_class_entry * gcombo_box_get_class_entry();

zend_object_handlers * gcombo_box_get_object_handlers();

/***********************/
/* GComboBox: Arginfos */
/***********************/

/************************/
/* GComboBox Properties */
/************************/

#define GCOMBO_BOX_ACTIVE "active"

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *gcombo_box_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * Get property handling function
 */
HashTable * gcombo_box_get_properties(zval *object);

/*==========================================================================*/
/** 
 * Write property handling function
 */
PHP_WRITE_PROP_HANDLER_TYPE gcombo_box_write_property(zval *object, zval *member, zval *value, void **cache_slot);


/***************/
/* PHP Methods */
/***************/

void gcombo_box_func_changed(GtkWidget* w, gpointer data);

void gcombo_box_func_move_active(GtkWidget* w, gpointer data);

void gcombo_box_on(long val,ze_gwidget_object * ze_obj, zval * function, zval * param);

#define GCOMBO_BOX_METHOD(name) \
PHP_METHOD(GComboBox, name)

GCOMBO_BOX_METHOD(on         );
GCOMBO_BOX_METHOD(__construct);

/***********************************/
/* GComboBox class initialisation */
/***********************************/

void gcombo_box_init(int module_number);

#endif

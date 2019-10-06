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


#ifndef __GEVENT_KEY_DEF__
#define __GEVENT_KEY_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "gevent.h"

/************************/
/* GEventKey properties */
/************************/

#define GEVENT_KEY_KEYVAL "keyval"
#define GEVENT_KEY_STATE "state"

/*****************************/
/* Class information getters */
/*****************************/

zend_class_entry * gevent_key_get_class_entry(void);

zend_object_handlers * gevent_key_get_object_handlers();



/***************/
/* PHP Methods */
/***************/

#define GEVENT_KEY_METHOD(name) \
PHP_METHOD(GEventKey, name)

GEVENT_KEY_METHOD(__construct);

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *gevent_key_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * get properties handling function
 */
HashTable *gevent_key_get_properties(zval *object);

/*==========================================================================*/
/** 
 * write property handling function
 */
PHP_WRITE_PROP_HANDLER_TYPE gevent_write_key_property(zval *object, zval *member, zval *value, void **cache_slot);

/**********************************/
/* GEventKey Class Initialization */
/**********************************/

void gevent_key_init(int module_number);

/* __GEVENT_KEY_DEF__ */

#endif

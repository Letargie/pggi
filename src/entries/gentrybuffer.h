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


#ifndef __GENTRY_BUFFER_DEF__
#define __GENTRY_BUFFER_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"

ZEND_BEGIN_ARG_INFO(arginfo_gentry_buffer_construct, 0)
	ZEND_ARG_TYPE_INFO(0, string, IS_STRING, 1)
ZEND_END_ARG_INFO()

/**************************************/
/* GEntryBuffer Intern Data Structure */
/**************************************/

typedef struct{
	GtkEntryBuffer * intern;
	zval signals;
} * gentry_buffer_ptr, gentry_buffer_t;

typedef struct{
	gentry_buffer_ptr buffer_ptr;
	zend_object std;
} ze_gentry_buffer_object;

/**************************/
/* GEntryBuffer properties */
/**************************/

#define GENTRY_BUFFER_TEXT       "text"
#define GENTRY_BUFFER_MAX_LENGTH "maxLength"

/*****************************/
/* Class information getters */
/*****************************/

zend_class_entry * gentry_buffer_get_class_entry(void);

zend_object_handlers * gentry_buffer_get_object_handlers();

/****************************/
/* Memory handling function */
/****************************/

static inline ze_gentry_buffer_object *php_gentry_buffer_fetch_object(zend_object *obj) {
	return (ze_gentry_buffer_object *)((char*)(obj) - XtOffsetOf(ze_gentry_buffer_object, std));
}

inline zend_object *php_gentry_buffer_reverse_object(ze_gentry_buffer_object *obj) {
	return (zend_object *)((char*)(obj) + sizeof(gentry_buffer_ptr));
}

#define Z_GENTRY_BUFFER_P(zv) php_gentry_buffer_fetch_object(Z_OBJ_P((zv)))

zend_object *gentry_buffer_object_new(zend_class_entry *class_type);

void gentry_buffer_dtor(gentry_buffer_ptr intern);

void gentry_buffer_object_free_storage(zend_object *object);
void gentry_buffer_free_resource(zend_resource *rsrc);

/*********************************/
/* internal on-related functions */
/*********************************/

//void gentry_buffer_func_changed(GtkEntryBuffer* buffer, gpointer data);

//void gentry_buffer_function(gpointer data, unsigned int type);

/***************/
/* PHP Methods */
/***************/

#define GENTRY_BUFFER_METHOD(name) \
PHP_METHOD(GEntryBuffer, name)

//GENTRY_BUFFER_METHOD(on          );
GENTRY_BUFFER_METHOD(__construct   );
GENTRY_BUFFER_METHOD(getLength     );
GENTRY_BUFFER_METHOD(getBytes      );
GENTRY_BUFFER_METHOD(insertText    );
GENTRY_BUFFER_METHOD(deleteText    );
GENTRY_BUFFER_METHOD(emitInsertText);
GENTRY_BUFFER_METHOD(emitDeleteText);

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *gentry_buffer_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * get properties handling function
 */
HashTable *gentry_buffer_get_properties(zval *object);

/*==========================================================================*/
/** 
 * write property handling function
 */
void gentry_buffer_write_property(zval *object, zval *member, zval *value, void **cache_slot);

/************************************/
/* GEntryBuffer Class Initialization */
/************************************/

void gentry_buffer_init(int module_number);

/* __GENTRY_BUFFER_DEF__ */

#endif

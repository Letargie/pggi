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


#ifndef __GTEXT_BUFFER_DEF__
#define __GTEXT_BUFFER_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"

/*************************************/
/* GTextBuffer Intern Data Structure */
/*************************************/

typedef struct{
	GtkTextBuffer * intern;
	zval signals;
} * gtext_buffer_ptr, gtext_buffer_t;

typedef struct{
	gtext_buffer_ptr buffer_ptr;
	zend_object std;
} ze_gtext_buffer_object;

/**************************/
/* GTextBuffer properties */
/**************************/

#define GTEXT_BUFFER_TEXT	"text"

/*****************************/
/* Class information getters */
/*****************************/

zend_class_entry * gtext_buffer_get_class_entry(void);

zend_object_handlers * gtext_buffer_get_object_handlers();

/****************************/
/* Memory handling function */
/****************************/

static inline ze_gtext_buffer_object *php_gtext_buffer_fetch_object(zend_object *obj) {
	return (ze_gtext_buffer_object *)((char*)(obj) - XtOffsetOf(ze_gtext_buffer_object, std));
}

static inline zend_object *php_gtext_buffer_reverse_object(ze_gtext_buffer_object *obj) {
	return (zend_object *)((char*)(obj) + sizeof(gtext_buffer_ptr));
}

#define Z_GTEXT_BUFFER_P(zv) php_gtext_buffer_fetch_object(Z_OBJ_P((zv)))

zend_object *gtext_buffer_object_new(zend_class_entry *class_type);

void gtext_buffer_dtor(gtext_buffer_ptr intern);

void gtext_buffer_object_free_storage(zend_object *object);
void gtext_buffer_free_resource(zend_resource *rsrc);

/*********************************/
/* internal on-related functions */
/*********************************/

void gtext_buffer_func_changed(GtkTextBuffer* buffer, gpointer data);

void gtext_buffer_function(gpointer data, unsigned int type);

/***************/
/* PHP Methods */
/***************/

#define GTEXT_BUFFER_METHOD(name) \
PHP_METHOD(GTextBuffer, name)

GTEXT_BUFFER_METHOD(on          );
GTEXT_BUFFER_METHOD(__construct );
GTEXT_BUFFER_METHOD(getLineCount);
GTEXT_BUFFER_METHOD(getCharCount);

GTEXT_BUFFER_METHOD(insert); // iter string length
GTEXT_BUFFER_METHOD(insertAtCursor); //string length
GTEXT_BUFFER_METHOD(insertRange); // iter iter iter
GTEXT_BUFFER_METHOD(delete); // iter iter
GTEXT_BUFFER_METHOD(placeCursor); // iter
GTEXT_BUFFER_METHOD(selectRange); // iter iter
GTEXT_BUFFER_METHOD(getIterAtLineOffset); // line offset
GTEXT_BUFFER_METHOD(getIterAtOffset); // offset
GTEXT_BUFFER_METHOD(getIterAtLine); // line
GTEXT_BUFFER_METHOD(getIterAtLineIndex); // line index
GTEXT_BUFFER_METHOD(getStartIter); 
GTEXT_BUFFER_METHOD(getEndIter); 
GTEXT_BUFFER_METHOD(getBound); // return 2 iterator 

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *gtext_buffer_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * get properties handling function
 */
HashTable *gtext_buffer_get_properties(zval *object);

/*==========================================================================*/
/** 
 * write property handling function
 */
void gtext_buffer_write_property(zval *object, zval *member, zval *value, void **cache_slot);

/************************************/
/* GTextBuffer Class Initialization */
/************************************/

void gtext_buffer_init(int module_number);

/* __GTEXT_BUFFER_DEF__ */

#endif

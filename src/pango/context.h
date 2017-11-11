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

#ifndef PHP_PANGO_CONTEXT_DEF
#define PHP_PANGO_CONTEXT_DEF

#include <pango/pango.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "exception.h"
#include "../abstracts/gwidget.h"

/*************************/
/* Intern Data Structure */
/*************************/

typedef struct{
	PangoContext * intern;
	int to_destroy : 1;
} * pp_context_ptr, pp_context_t;

typedef struct{
	pp_context_ptr context_ptr;
	zend_object std;
} ze_pp_context_object;

ZEND_BEGIN_ARG_INFO_EX(arginfo_pp_context_construct, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, x, PGGI\\GWidget, 0)
ZEND_END_ARG_INFO()

/**************/
/* properties */
/**************/

#define CONTEXT_FONT_MAP         "fontMap"
#define CONTEXT_BASE_DIR         "baseDir"
#define CONTEXT_BASE_GRAVITY     "baseGravity"
#define CONTEXT_GRAVITY_HINT     "gravityHint"
#define CONTEXT_MATRIX           "matrix"
#define CONTEXT_LANGUAGE         "language"
#define CONTEXT_FONT_DESCRIPTION "fontDescription"

/*****************************/
/* Class information getters */
/*****************************/

/*==========================================================================*/
/** 
 * get the class entry
 */
zend_class_entry * pp_context_get_class_entry(void);

/*==========================================================================*/
/** 
 * get the object handler
 */
zend_object_handlers * pp_context_get_object_handlers();

/****************************/
/* Memory handling function */
/****************************/

pp_context_ptr pp_context_new(void);

static inline ze_pp_context_object *php_pp_context_fetch_object(zend_object *obj) {
	return (ze_pp_context_object *)((char*)(obj) - XtOffsetOf(ze_pp_context_object, std));
}

inline zend_object *php_pp_context_reverse_object(ze_pp_context_object *obj) {
	return (zend_object *)((char*)(obj) + sizeof(pp_context_ptr));
}

#define Z_PP_CONTEXT_P(zv) php_pp_context_fetch_object(Z_OBJ_P((zv)))

zend_object *pp_context_object_new(zend_class_entry *class_type);

void pp_context_dtor(pp_context_ptr intern);

void pp_context_object_free_storage(zend_object *object);
void pp_context_free_resource(zend_resource *rsrc);

/***************/
/* PHP Methods */
/***************/

#define PANGO_CONTEXT_METHOD(name) \
	PREFIX_METHOD(Pango, Context, name)

#define PANGO_CONTEXT_ME(name, arg_info, flags) \
	PREFIX_ME(Pango, Context, name, arg_info, flags)

/* Context Functions */

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *pp_context_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * get properties handling function
 */
HashTable *pp_context_get_properties(zval *object);

/*==========================================================================*/
/** 
 * write property handling function
 */
void pp_context_write_property(zval *object, zval *member, zval *value, void **cache_slot);

/************************/
/* Class Initialization */
/************************/

/*==========================================================================*/
/** 
 * init the class
 */
void pp_context_init(int module_number);





#endif

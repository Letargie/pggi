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

#ifndef PHP_CAIRO_PATTERN_DEF
#define PHP_CAIRO_PATTERN_DEF

#include <cairo.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "exception.h"
#include "../gdk/rgba.h"

/*************************/
/* Intern Data Structure */
/*************************/

typedef struct{
	cairo_pattern_t * intern;
	int to_destroy : 1;
} * pc_pattern_ptr, pc_pattern_t;

typedef struct{
	pc_pattern_ptr pattern_ptr;
	zend_object std;
} ze_pattern_object;

/**************/
/* properties */
/**************/

#define PATTERN_EXTEND "extend"
#define PATTERN_FILTER "filter"

/*****************************/
/* Class information getters */
/*****************************/

/*==========================================================================*/
/** 
 * get the class entry
 */
zend_class_entry * pc_pattern_get_class_entry(void);

/*==========================================================================*/
/** 
 * get the object handler
 */
zend_object_handlers * pc_pattern_get_object_handlers();

/****************************/
/* Memory handling function */
/****************************/

pc_pattern_ptr pc_pattern_new(void);

static inline ze_pattern_object *php_pattern_fetch_object(zend_object *obj) {
	return (ze_pattern_object *)((char*)(obj) - XtOffsetOf(ze_pattern_object, std));
}

inline zend_object *php_pattern_reverse_object(ze_pattern_object *obj) {
	return (zend_object *)((char*)(obj) + sizeof(pc_pattern_ptr));
}

#define Z_PATTERN_P(zv) php_pattern_fetch_object(Z_OBJ_P((zv)))

zend_object *pc_pattern_object_new(zend_class_entry *class_type);

void pc_pattern_dtor(pc_pattern_ptr intern);

void pc_pattern_object_free_storage(zend_object *object);
void pc_pattern_free_resource(zend_resource *rsrc);

/***************/
/* PHP Methods */
/***************/

#define PATTERN_METHOD(name) \
PHP_METHOD(Pattern, name)

/* Pattern Functions */

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *pc_pattern_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * get properties handling function
 */
HashTable *pc_pattern_get_properties(zval *object);

/*==========================================================================*/
/** 
 * write property handling function
 */
void pc_pattern_write_property(zval *object, zval *member, zval *value, void **cache_slot);

/************************/
/* Class Initialization */
/************************/

/*==========================================================================*/
/** 
 * init the class
 */
void pc_pattern_init(int module_number);





#endif

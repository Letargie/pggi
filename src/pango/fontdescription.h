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

#ifndef PHP_PANGO_FONT_DESCRIPTION_DEF
#define PHP_PANGO_FONT_DESCRIPTION_DEF

#include <pango/pango.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "exception.h"
#include "context.h"

/*************************/
/* Intern Data Structure */
/*************************/

typedef struct{
	PangoFontDescription * intern;
} * font_description_ptr, font_description_t;

typedef struct{
	font_description_ptr description_ptr;
	zend_object std;
} ze_font_description_object;

ZEND_BEGIN_ARG_INFO_EX(arginfo_font_description_construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, string, IS_STRING, 0)
ZEND_END_ARG_INFO()

/**************/
/* properties */
/**************/

#define FONT_DESCRIPTION_STYLE   "style"
#define FONT_DESCRIPTION_VARIANT "variant"
#define FONT_DESCRIPTION_STRETCH "stretch"
#define FONT_DESCRIPTION_FAMILY  "family"

/*****************************/
/* Class information getters */
/*****************************/

/*==========================================================================*/
/** 
 * get the class entry
 */
zend_class_entry * font_description_get_class_entry(void);

/*==========================================================================*/
/** 
 * get the object handler
 */
zend_object_handlers * font_description_get_object_handlers();

/****************************/
/* Memory handling function */
/****************************/

font_description_ptr font_description_new(void);

static inline ze_font_description_object *php_font_description_fetch_object(zend_object *obj) {
	return (ze_font_description_object *)((char*)(obj) - XtOffsetOf(ze_font_description_object, std));
}

inline zend_object *php_font_description_reverse_object(ze_font_description_object *obj) {
	return (zend_object *)((char*)(obj) + sizeof(font_description_ptr));
}

#define Z_FONT_DESCRIPTION_P(zv) php_font_description_fetch_object(Z_OBJ_P((zv)))

zend_object *font_description_object_new(zend_class_entry *class_type);

void font_description_dtor(font_description_ptr intern);

void font_description_object_free_storage(zend_object *object);
void font_description_free_resource(zend_resource *rsrc);

/***************/
/* PHP Methods */
/***************/

#define FONT_DESCRIPTION_METHOD(name) \
PHP_METHOD(FontDescription, name)

/* font_description Functions */

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *font_description_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * get properties handling function
 */
HashTable *font_description_get_properties(zval *object);

/*==========================================================================*/
/** 
 * write property handling function
 */
void font_description_write_property(zval *object, zval *member, zval *value, void **cache_slot);

/************************/
/* Class Initialization */
/************************/

/*==========================================================================*/
/** 
 * init the class
 */
void font_description_init(int module_number);





#endif

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

#ifndef PHP_CAIRO_SURFACE_DEF
#define PHP_CAIRO_SURFACE_DEF

#include <cairo.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "exception.h"


PGGI_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_surface_create_similar, 0, 3, pc_surface_get_class_entry(), 0)
	ZEND_ARG_TYPE_INFO(0, content, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, width  , IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, height , IS_LONG, 0)
ZEND_END_ARG_INFO()


/*************************/
/* Intern Data Structure */
/*************************/

typedef struct{
	cairo_surface_t * intern;
	int to_destroy;
} * pc_surface_ptr, pc_surface_t;

typedef struct{
	pc_surface_ptr surface_ptr;
	zend_object std;
} ze_surface_object;

/**************/
/* properties */
/**************/


/*****************************/
/* Class information getters */
/*****************************/

/*==========================================================================*/
/** 
 * get the class entry
 */
zend_class_entry * pc_surface_get_class_entry(void);

/*==========================================================================*/
/** 
 * get the object handler
 */
zend_object_handlers * pc_surface_get_object_handlers();

/****************************/
/* Memory handling function */
/****************************/

pc_surface_ptr pc_surface_new(void);

static inline ze_surface_object *php_surface_fetch_object(zend_object *obj) {
	return (ze_surface_object *)((char*)(obj) - XtOffsetOf(ze_surface_object, std));
}

static inline zend_object *php_surface_reverse_object(ze_surface_object *obj) {
	return (zend_object *)((char*)(obj) + sizeof(pc_surface_ptr));
}

#define Z_SURFACE_P(zv) php_surface_fetch_object(Z_OBJ_P((zv)))

zend_object *pc_surface_object_new(zend_class_entry *class_type);

zend_object * pc_surface_ctor(zend_class_entry *ce, cairo_surface_t * surface, int to_destroy);

void pc_surface_dtor(pc_surface_ptr intern);

void pc_surface_object_free_storage(zend_object *object);
void pc_surface_free_resource(zend_resource *rsrc);

/***************/
/* PHP Methods */
/***************/

#define SURFACE_METHOD(name) \
PHP_METHOD(Surface, name)

SURFACE_METHOD(createSimilar);
SURFACE_METHOD(flush);
SURFACE_METHOD(getDevice);
SURFACE_METHOD(getFontOptions);
SURFACE_METHOD(getContent);
SURFACE_METHOD(markDirty);
SURFACE_METHOD(markDirtyRectangle);
SURFACE_METHOD(setDeviceOffset);
SURFACE_METHOD(getDeviceOffset); // deviceOffsetX Y double
SURFACE_METHOD(setScale);
SURFACE_METHOD(getScale); // XScale Y (double)
SURFACE_METHOD(setFallbackResolution);
SURFACE_METHOD(getFallbackResolution); // x YPixelPerInch
SURFACE_METHOD(getType);
SURFACE_METHOD(copyPage);
SURFACE_METHOD(showPage);
SURFACE_METHOD(mapToImage);
SURFACE_METHOD(unmapImage);



/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *pc_surface_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * get properties handling function
 */
HashTable *pc_surface_get_properties(zval *object);

/*==========================================================================*/
/** 
 * write property handling function
 */
void pc_surface_write_property(zval *object, zval *member, zval *value, void **cache_slot);

/************************/
/* Class Initialization */
/************************/

/*==========================================================================*/
/** 
 * init the class
 */
void pc_surface_init(int module_number);


#endif

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
 * Get the PGGI\Pango\Context class entry
 *
 * @return zend_class_entry * The class entry
 */
zend_class_entry * pp_context_get_class_entry(void);

/*==========================================================================*/
/**
 * Get the PGGI\Pango\Context object handlers
 *
 * @return zend_object_handlers * The object handlers
 */
zend_object_handlers * pp_context_get_object_handlers(void);

/****************************/
/* Memory handling function */
/****************************/

/*==========================================================================*/
/**
 * Construct a new pp_context_ptr used to contains data for a PGGI\Pango\Context
 * @constructor
 *
 * @return pp_context_ptr A pointer to the newly created context
 */
pp_context_ptr pp_context_new(void);

/*==========================================================================*/
/**
 * Retrieve the ze_pp_context_object used to contains data for a PGGI\Pango\Context from a zend object
 *
 * @param zend_object * obj The object
 *
 * @return ze_pp_context_ptr 
 */
static inline ze_pp_context_object *php_pp_context_fetch_object(zend_object *obj) {
	return (ze_pp_context_object *)((char*)(obj) - XtOffsetOf(ze_pp_context_object, std));
}

/*==========================================================================*/
/**
 * from a ze_pp_context_ptr retrieve the zend object that it contains
 *
 * @param ze_pp_context_object * obj The object
 *
 * @return zend_object * the object
 */
static inline zend_object *php_pp_context_reverse_object(ze_pp_context_object *obj) {
	return (zend_object *)((char*)(obj) + sizeof(pp_context_ptr));
}

/*==========================================================================*/
/**
 * Retrieve a pointer to the ze_pp_context_t used to contains data for a PGGI\Pango\Context from a zval
 * 
 * @param zval * The object
 *
 * @return ze_pp_context_ptr A pointer to the newly created context
 */
#define Z_PP_CONTEXT_P(zv) php_pp_context_fetch_object(Z_OBJ_P((zv)))

/*==========================================================================*/
/**
 * Construct a new zend_object * corresponding to a PGGI\Pango\Context
 * @constructor
 * 
 * @return zend_object * Our object
 */
zend_object *pp_context_object_new(zend_class_entry *class_type);

/*==========================================================================*/
/**
 * Destroy a pp_context_t used to contains data for a PGGI\Pango\Context
 * @destructor
 *
 * @param pp_context_ptr intern A pointer to the structure to destroy
 */
void pp_context_dtor(pp_context_ptr intern);

/*==========================================================================*/
/**
 * Destroy a zend object corresponding to a PGGI\Pango\Context
 * @destructor
 *
 * @param zend_object *object A pointer to the object corresponding to a PGGI\Pango\Context
 */
void pp_context_object_free_storage(zend_object *object);

/*==========================================================================*/
/**
 * Destroy a resource corresponding to a PGGI\Pango\Context
 * @destructor
 *
 * @param zend_resource *rsrc A pointer to the object corresponding to a PGGI\Pango\Context
 */
void pp_context_free_resource(zend_resource *rsrc);

/***************/
/* PHP Methods */
/***************/

#define PANGO_CONTEXT_METHOD(name) \
	PREFIX_METHOD(Pango, Context, name)

#define PANGO_CONTEXT_ME(name, arg_info, flags) \
	PREFIX_ME(Pango, Context, name, arg_info, flags)

/* Context Functions */

/*==========================================================================*/
/**
 * Construct a new PGGI\Pango\Context associate to a GWidget
 * @constructor
 * 
 * @param PGGI\GWidget the widget we want associate with our context
 */
PANGO_CONTEXT_METHOD(__construct);

/*==========================================================================*/
/**
 * Retrieves the gravity for the context,
 * it can be different from the baseGravity attribute if its value is GRAVITY_AUTO
 * for which Gravity::getForMatrix() is used to return the gravity from the current context matrix.
 * (function not yet implemented in php)
 *
 * @return int The gravity of the context (PGGI\Pango\Context::GRAVITY_*)
 */
PANGO_CONTEXT_METHOD(getGravity);

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/**
 * Retrieves the value of a property of a PGGI\Pango\Context object 
 *
 * Properties you can retrieve :
 * - int baseDir
 * - int baseGravity
 * - int gravityHint
 *
 * @param zval *  object     The object we want to get the value of the property 
 * @param zval *  member     The property we want to get the value of
 * @param int     type       The type of the request
 * @param void ** cache_slot The cache slot
 * @param zval *  rv         The variable we fill with the return value
 *
 * @return zval * The return of the property
 */
zval *pp_context_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/**
 * Get an array of properties we want to display for a PGGI\Pango\Context object 
 *
 * Content of the array :
 * - int baseDir
 * - int baseGravity
 * - int gravityHint
 *
 * @param zval * object The object we want to get the properties of
 *
 * @return Hashtable * The array of properties 
 */
HashTable *pp_context_get_properties(zval *object);

/*==========================================================================*/
/**
 * Set the value of a property for a PGGI\Pango\Context object 
 *
 * Properties you can edit with the authorized types :
 * - int baseDir     : int (need to be a PGGI\Pango\DIRECTION_*) // TODO Context::Direction for now
 *                         throw a PGGI\Pango\Exception otherwise
 * - int baseGravity : int (need to be a PGGI\Pango\Context::GRAVITY_*)
 *                         throw a PGGI\Pango\Exception otherwise
 * - int gravityHint : int (need to be a PGGI\Pango\Context::GRAVITY_HINT_*)
 *                         throw a PGGI\Pango\Exception otherwise
 *
 * @param zval * object      The object we want to set the properties of
 * @param zval * member      The property we want to set
 * @param zval * value       The value we want to use to set the property
 * @param void ** cache_slot The cache slot
 */
void pp_context_write_property(zval *object, zval *member, zval *value, void **cache_slot);

/************************/
/* Class Initialization */
/************************/

/*==========================================================================*/
/**
 * Initialize the class PGGI\Pango\Context
 *
 * @param int module_number The module number
 */
void pp_context_init(int module_number);





#endif

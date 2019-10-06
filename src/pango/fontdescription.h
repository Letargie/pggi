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
 * Get the PGGI\Pango\FontDescription class entry
 *
 * @return zend_class_entry * The class entry
 */
zend_class_entry * font_description_get_class_entry(void);

/*==========================================================================*/
/**
 * Get the PGGI\Pango\FontDescription object handlers
 *
 * @return zend_object_handlers * The object handlers
 */
zend_object_handlers * font_description_get_object_handlers();

/****************************/
/* Memory handling function */
/****************************/

/*==========================================================================*/
/**
 * Construct a new font_description_ptr used to contains data for a PGGI\Pango\FontDescription
 * @constructor
 *
 * @return font_description_ptr A pointer to the newly created context
 */
font_description_ptr font_description_new(void);

/*==========================================================================*/
/**
 * Retrieve the ze_pp_context_t used to contains data for a PGGI\Pango\Context from a zend object
 *
 * @param zend_object * obj The object
 *
 * @return ze_pp_context_ptr 
 */
static inline ze_font_description_object *php_font_description_fetch_object(zend_object *obj) {
	return (ze_font_description_object *)((char*)(obj) - XtOffsetOf(ze_font_description_object, std));
}

/*==========================================================================*/
/**
 * from a ze_pp_context_ptr retrieve the zend object that it contains
 *
 * @param ze_pp_context_object * obj The object
 *
 * @return zend_object * the object
 */
static inline zend_object *php_font_description_reverse_object(ze_font_description_object *obj) {
	return (zend_object *)((char*)(obj) + sizeof(font_description_ptr));
}

/*==========================================================================*/
/**
 * Retrieve a pointer to the ze_pp_context_t used to contains data for a PGGI\Pango\FontDescription from a zval
 * 
 * @param zval * The object
 *
 * @return ze_pp_context_ptr A pointer to the newly created context
 */
#define Z_FONT_DESCRIPTION_P(zv) php_font_description_fetch_object(Z_OBJ_P((zv)))

/*==========================================================================*/
/**
 * Construct a new zend_object * corresponding to a PGGI\Pango\FontDescription
 * @constructor
 * 
 * @return zend_object * Our object
 */
zend_object *font_description_object_new(zend_class_entry *class_type);

/*==========================================================================*/
/**
 * Destroy a pp_context_t used to contains data for a PGGI\Pango\FontDescription
 * @destructor
 *
 * @param pp_context_ptr intern A pointer to the structure to destroy
 */
void font_description_dtor(font_description_ptr intern);

/*==========================================================================*/
/**
 * Destroy a zend object corresponding to a PGGI\Pango\FontDescription
 * @destructor
 *
 * @param zend_object *object A pointer to the object corresponding to a PGGI\Pango\FontDescription
 */
void font_description_object_free_storage(zend_object *object);
void font_description_free_resource(zend_resource *rsrc);

/***************/
/* PHP Methods */
/***************/

#define FONT_DESCRIPTION_METHOD(name) \
PHP_METHOD(FontDescription, name)

/* font_description Functions */

/**
 * Construct a new PGGI\Pango\FontDescription from a string representation in the form "FAMILY-LIST [SIZE]",
 * where FAMILY-LIST is a comma separated list of families optionally terminated by a comma,
 * STYLE_OPTIONS is a whitespace separated list of words where each word describes one of style, variant, weight, stretch, or gravity,
 * and SIZE is a decimal number (size in points) or optionally followed by the unit modifier "px" for absolute size.
 * Any one of the options may be absent.
 * If FAMILY-LIST is absent, then the family_name field of the resulting font description will be initialized to NULL.
 * If STYLE-OPTIONS is missing, then all style options will be set to the default values.
 * If SIZE is missing, the size in the resulting font description will be set to 0.
 *
 * If the representation is null Creates a new font description structure with all fields unset.
 * @constructor
 * 
 * @param string (Optionnal) The string corresponding to our font description
 */
FONT_DESCRIPTION_METHOD(__construct);

/*==========================================================================*/
/**
 * Computes a hash of a FontDescription structure suitable to be used
 *
 * @return int the hash created
 */
FONT_DESCRIPTION_METHOD(hash);

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/**
 * Retrieves the value of a property of a PGGI\Pango\FontDescription object 
 *
 * Properties you can retrieve :
 * - int stretch
 * - int style
 * - int variant
 *
 * @param zval *  object     The object we want to get the value of the property 
 * @param zval *  member     The property we want to get the value of
 * @param int     type       The type of the request
 * @param void ** cache_slot The cache slot
 * @param zval *  rv         The variable we fill with the return value
 *
 * @return zval * The return of the property
 */
zval *font_description_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/**
 * Get an array of properties we want to display for a PGGI\Pango\FontDescription object 
 *
 * Content of the array :
 * - int stretch
 * - int style
 * - int variant
 *
 * @param zval * object The object we want to get the properties of
 *
 * @return Hashtable * The array of properties 
 */
HashTable *font_description_get_properties(zval *object);

/*==========================================================================*/
/**
 * Set the value of a property for a PGGI\Pango\FontDescription object 
 *
 * Properties you can edit with the authorized types :
 * - int stretch int (need to be a PGGI\Pango\FontDescription::STRETCH_*)
 *                   throw a PGGI\Pango\Exception otherwise
 * - int style
 * - int variant
 *
 * @param zval * object      The object we want to set the properties of
 * @param zval * member      The property we want to set
 * @param zval * value       The value we want to use to set the property
 * @param void ** cache_slot The cache slot
 */
PHP_WRITE_PROP_HANDLER_TYPE font_description_write_property(zval *object, zval *member, zval *value, void **cache_slot);

/************************/
/* Class Initialization */
/************************/

/*==========================================================================*/
/**
 * Initialize the class PGGI\Pango\FontDescription
 *
 * @param int module_number The module number
 */
void font_description_init(int module_number);





#endif

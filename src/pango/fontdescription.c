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


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include "fontdescription.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_font_description;
static zend_object_handlers font_description_object_handlers;
static zend_class_entry * font_description_class_entry_ce;

/*==========================================================================*/
/**
 * Get the PGGI\Pango\FontDescription class entry
 *
 * @return zend_class_entry * The class entry
 */
zend_class_entry * font_description_get_class_entry(){
	return font_description_class_entry_ce;
}

/*==========================================================================*/
/**
 * Get the PGGI\Pango\FontDescription object handlers
 *
 * @return zend_object_handlers * The object handlers
 */
zend_object_handlers * font_description_get_object_handlers(){
	return &font_description_object_handlers;
}

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
font_description_ptr font_description_new(){
	font_description_ptr tor = ecalloc(1, sizeof(font_description_t));
	return tor;
}

/*==========================================================================*/
/**
 * Construct a new zend_object * corresponding to a PGGI\Pango\FontDescription
 * @constructor
 * 
 * @return zend_object * Our object
 */
zend_object *font_description_object_new(zend_class_entry *class_type){
	ze_font_description_object *intern;
	intern = ecalloc(1, sizeof(ze_font_description_object) + zend_object_properties_size(class_type));
	zend_object_std_init  (&intern->std, class_type);
	object_properties_init(&intern->std, class_type);
	intern->std.handlers = &font_description_object_handlers;
	return &intern->std;
}

/*==========================================================================*/
/**
 * Destroy a pp_context_t used to contains data for a PGGI\Pango\FontDescription
 * @destructor
 *
 * @param pp_context_ptr intern A pointer to the structure to destroy
 */
void font_description_dtor(font_description_ptr intern){
	if (intern->intern){
		pango_font_description_free(intern->intern);
	}
	efree(intern);
}

/*==========================================================================*/
/**
 * Destroy a zend object corresponding to a PGGI\Pango\FontDescription
 * @destructor
 *
 * @param zend_object *object A pointer to the object corresponding to a PGGI\Pango\FontDescription
 */
void font_description_object_free_storage(zend_object *object){
	ze_font_description_object *intern = php_font_description_fetch_object(object);
	if (!intern) {
		return;
	}
	if (intern->description_ptr){
		font_description_dtor(intern->description_ptr);
	}
	intern->description_ptr = NULL;
	zend_object_std_dtor(&intern->std);
}

/*==========================================================================*/
/**
 * Destroy a resource corresponding to a PGGI\Pango\FontDescription
 * @destructor
 *
 * @param zend_resource *rsrc A pointer to the object corresponding to a PGGI\Pango\FontDescription
 */
void font_description_free_resource(zend_resource *rsrc) {
	font_description_ptr font_description = (font_description_ptr) rsrc->ptr;
	font_description_dtor(font_description);
}

/***************/
/* PHP Methods */
/***************/

/*==========================================================================*/
/*
 *Structures representing abstract fonts
 * Pango supports a flexible architecture where a particular rendering architecture can supply an implementation of fonts.
 * Pango provides routines to list available fonts, and to load a font of a given description.
 */

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
FONT_DESCRIPTION_METHOD(__construct){
	zval * self = getThis();
	ze_font_description_object * ze_obj;
	char * str;
	size_t str_len = 0;
	ze_obj = Z_FONT_DESCRIPTION_P(self);
	ze_obj->description_ptr = font_description_new();
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|s", &str, &str_len) == FAILURE)
		return;
	if(str_len == 0)
		ze_obj->description_ptr->intern = pango_font_description_new();
	else
		ze_obj->description_ptr->intern = pango_font_description_from_string(str);
}

/*==========================================================================*/
/**
 * Computes a hash of a FontDescription structure suitable to be used
 *
 * @return int the hash created
 */
FONT_DESCRIPTION_METHOD(hash){
	zval * self = getThis();
	ze_font_description_object * ze_obj;
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_FONT_DESCRIPTION_P(self);
	RETURN_LONG(pango_font_description_hash(ze_obj->description_ptr->intern));
}

//PGGI\Pango\FontDescription Functions
static const zend_function_entry font_description_class_functions[] = {
	PHP_ME(FontDescription, __construct, arginfo_font_description_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(FontDescription, hash       , arginfo_pggi_get_long             , ZEND_ACC_PUBLIC)
	PHP_FE_END
};

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
zval *font_description_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_font_description_object * intern = Z_FONT_DESCRIPTION_P(object);
	font_description_ptr d = intern->description_ptr;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	 if(!strcmp(member_val, FONT_DESCRIPTION_STRETCH)){
		ZVAL_LONG(rv, pango_font_description_get_stretch(d->intern));
	}else if(!strcmp(member_val, FONT_DESCRIPTION_STYLE)){
		ZVAL_LONG(rv, pango_font_description_get_style(d->intern));
	}else if(!strcmp(member_val, FONT_DESCRIPTION_VARIANT)){
		ZVAL_LONG(rv, pango_font_description_get_variant(d->intern));
	}else
		return std_object_handlers.read_property(object, member, type, cache_slot, rv);
	return rv;
}

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
HashTable *font_description_get_properties(zval *object){
	G_H_UPDATE_INIT(zend_std_get_properties(object));
	ze_font_description_object * intern = Z_FONT_DESCRIPTION_P(object);
	font_description_ptr d = intern->description_ptr;
	G_H_UPDATE_LONG(FONT_DESCRIPTION_STRETCH, pango_font_description_get_stretch(d->intern));
	G_H_UPDATE_LONG(FONT_DESCRIPTION_STYLE  , pango_font_description_get_style  (d->intern));
	G_H_UPDATE_LONG(FONT_DESCRIPTION_VARIANT, pango_font_description_get_variant(d->intern));
	return G_H_UPDATE_RETURN;
}

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
PHP_WRITE_PROP_HANDLER_TYPE font_description_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_font_description_object * intern = Z_FONT_DESCRIPTION_P(object);
	font_description_ptr d = intern->description_ptr;
	long tmp_l;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	if(!strcmp(member_val, FONT_DESCRIPTION_STRETCH)){
		convert_to_long(value);
		tmp_l = Z_LVAL_P(value);
		switch(tmp_l){
			case PANGO_STRETCH_ULTRA_CONDENSED :
			case PANGO_STRETCH_EXTRA_CONDENSED :
			case PANGO_STRETCH_CONDENSED       :
			case PANGO_STRETCH_SEMI_CONDENSED  :
			case PANGO_STRETCH_NORMAL          :
			case PANGO_STRETCH_SEMI_EXPANDED   :
			case PANGO_STRETCH_EXPANDED        :
			case PANGO_STRETCH_EXTRA_EXPANDED  :
			case PANGO_STRETCH_ULTRA_EXPANDED  :
				pango_font_description_set_stretch(d->intern, tmp_l);
				break;
			default:
				zend_throw_exception_ex(pggi_exception_get(), 0, "Can't change the stretch property, needs to be a FontDescription::STRETCH_*");
				return;
				break;
		}
		
	}else if(!strcmp(member_val, FONT_DESCRIPTION_STYLE)){
		convert_to_long(value);
		tmp_l = Z_LVAL_P(value);
		pango_font_description_set_style(d->intern, tmp_l);
	}else if(!strcmp(member_val, FONT_DESCRIPTION_VARIANT)){
		convert_to_long(value);
		tmp_l = Z_LVAL_P(value);
		switch(tmp_l){
			case PANGO_VARIANT_NORMAL     :
			case PANGO_VARIANT_SMALL_CAPS :
				pango_font_description_set_variant(d->intern, tmp_l);
				break;
			default:
				zend_throw_exception_ex(pggi_exception_get(), 0, "Can't change the variant property, needs to be a FontDescription::VARIANT_*");
				return;
				break;
		}
	} else {
		PHP_WRITE_PROP_HANDLER_RETURN(std_object_handlers.write_property(object, member, value, cache_slot));
	}
	PHP_WRITE_PROP_HANDLER_RETURN(value);
}

/************************/
/* Class Initialization */
/************************/

#define DECLARE_FONT_DESCRIPTION_PROP(name) \
DECLARE_CLASS_PROPERTY(font_description_class_entry_ce, name)

#define FONT_DESCRIPTION_CONSTANT(name, value) \
zend_declare_class_constant_long(font_description_class_entry_ce, name, sizeof(name)-1, value);

/*==========================================================================*/
/**
 * Initialize the class PGGI\Pango\FontDescription
 *
 * @param int module_number The module number
 */
void font_description_init(int module_number){
	zend_class_entry ce;
	le_font_description = zend_register_list_destructors_ex(font_description_free_resource, NULL, "PGGI\\Pango\\FontDescription", module_number);

	memcpy(&font_description_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	font_description_object_handlers.offset         = XtOffsetOf(ze_font_description_object, std);
	font_description_object_handlers.free_obj       = font_description_object_free_storage;
	font_description_object_handlers.clone_obj      = NULL;
	font_description_object_handlers.read_property  = font_description_read_property;
	font_description_object_handlers.get_properties = font_description_get_properties;
	font_description_object_handlers.write_property = font_description_write_property;
	INIT_CLASS_ENTRY(ce, "PGGI\\Pango\\FontDescription", font_description_class_functions);
	ce.create_object = font_description_object_new;
	font_description_class_entry_ce = zend_register_internal_class(&ce);


	DECLARE_FONT_DESCRIPTION_PROP(FONT_DESCRIPTION_STRETCH); // The stretch field of a font description
	/**
     * The style field of a PangoFontDescription.
     * The PangoStyle enumeration describes whether the font is slanted and the manner in which it is slanted;
     * it can be either STYLE_NORMAL, STYLE_ITALIC, or STYLE_OBLIQUE. Most fonts will either have a italic style or an oblique style, but not both,
     *  and font matching in Pango will match italic specifications with oblique fonts and vice-versa if an exact match is not found.
     */
	DECLARE_FONT_DESCRIPTION_PROP(FONT_DESCRIPTION_STYLE  ); 
	DECLARE_FONT_DESCRIPTION_PROP(FONT_DESCRIPTION_VARIANT); // the variant field of a FontDescription

/*==========================================================================*/
/*
 * The bits in a FontMask correspond to fields in a FontDescription that have been set.
 */
	FONT_DESCRIPTION_CONSTANT("FONT_MASK_FAMILY"       , PANGO_FONT_MASK_FAMILY       ); // the font family is specified.
	FONT_DESCRIPTION_CONSTANT("FONT_MASK_STYLE"        , PANGO_FONT_MASK_STYLE        ); // the font style is specified.
	FONT_DESCRIPTION_CONSTANT("FONT_MASK_VARIANT"      , PANGO_FONT_MASK_VARIANT      ); // the font variant is specified.
	FONT_DESCRIPTION_CONSTANT("FONT_MASK_WEIGHT"       , PANGO_FONT_MASK_WEIGHT       ); // the font weight is specified.
	FONT_DESCRIPTION_CONSTANT("FONT_MASK_STRETCH"      , PANGO_FONT_MASK_STRETCH      ); // the font stretch is specified.
	FONT_DESCRIPTION_CONSTANT("FONT_MASK_SIZE"         , PANGO_FONT_MASK_SIZE         ); // the font size is specified.
	FONT_DESCRIPTION_CONSTANT("FONT_MASK_GRAVITY"      , PANGO_FONT_MASK_GRAVITY      ); // the font gravity is specified // (Since: 1.16.)

/*==========================================================================*/
/*
 * An enumeration specifying the width of the font relative to other designs within a family. 
 */
	FONT_DESCRIPTION_CONSTANT("STRETCH_ULTRA_CONDENSED", PANGO_STRETCH_ULTRA_CONDENSED); // ultra condensed width
	FONT_DESCRIPTION_CONSTANT("STRETCH_EXTRA_CONDENSED", PANGO_STRETCH_EXTRA_CONDENSED); // extra condensed width
	FONT_DESCRIPTION_CONSTANT("STRETCH_CONDENSED"      , PANGO_STRETCH_CONDENSED      ); // condensed width
	FONT_DESCRIPTION_CONSTANT("STRETCH_SEMI_CONDENSED" , PANGO_STRETCH_SEMI_CONDENSED ); // semi condensed width
	FONT_DESCRIPTION_CONSTANT("STRETCH_NORMAL"         , PANGO_STRETCH_NORMAL         ); // the normal width
	FONT_DESCRIPTION_CONSTANT("STRETCH_SEMI_EXPANDED"  , PANGO_STRETCH_SEMI_EXPANDED  ); // semi expanded width
	FONT_DESCRIPTION_CONSTANT("STRETCH_EXPANDED"       , PANGO_STRETCH_EXPANDED       ); // expanded width
	FONT_DESCRIPTION_CONSTANT("STRETCH_EXTRA_EXPANDED" , PANGO_STRETCH_EXTRA_EXPANDED ); // extra expanded width
	FONT_DESCRIPTION_CONSTANT("STRETCH_ULTRA_EXPANDED" , PANGO_STRETCH_ULTRA_EXPANDED ); // ultra expanded width
	
/*==========================================================================*/
/*
 * An enumeration specifying the weight (boldness) of a font. This is a numerical value ranging from 100 to 1000, but there are some predefined values:
 */
	FONT_DESCRIPTION_CONSTANT("WEIGHT_LIGHT"           , PANGO_WEIGHT_LIGHT           ); // the thin weight       (= 100 ) // Since: 1.24
	FONT_DESCRIPTION_CONSTANT("WEIGHT_ULTRALIGHT"      , PANGO_WEIGHT_ULTRALIGHT      ); // the ultralight weight (= 200 )
	FONT_DESCRIPTION_CONSTANT("WEIGHT_THIN"            , PANGO_WEIGHT_THIN            ); // the light weight      (= 300 )
	FONT_DESCRIPTION_CONSTANT("WEIGHT_ULTRAHEAVY"      , PANGO_WEIGHT_ULTRAHEAVY      ); // the semilight weight  (= 350 ) // Since: 1.36.7
	FONT_DESCRIPTION_CONSTANT("WEIGHT_HEAVY"           , PANGO_WEIGHT_HEAVY           ); // the book weight       (= 380 ) // Since: 1.24
	FONT_DESCRIPTION_CONSTANT("WEIGHT_ULTRABOLD"       , PANGO_WEIGHT_ULTRABOLD       ); // the default weight    (= 400 )
	FONT_DESCRIPTION_CONSTANT("WEIGHT_BOLD"            , PANGO_WEIGHT_BOLD            ); // the normal weight     (= 500 ) // Since: 1.24
	FONT_DESCRIPTION_CONSTANT("WEIGHT_SEMIBOLD"        , PANGO_WEIGHT_SEMIBOLD        ); // the semibold weight   (= 600 )
	FONT_DESCRIPTION_CONSTANT("WEIGHT_MEDIUM"          , PANGO_WEIGHT_MEDIUM          ); // the bold weight       (= 700 )
	FONT_DESCRIPTION_CONSTANT("WEIGHT_NORMAL"          , PANGO_WEIGHT_NORMAL          ); // the ultrabold weight  (= 800 )
	FONT_DESCRIPTION_CONSTANT("WEIGHT_BOOK"            , PANGO_WEIGHT_BOOK            ); // the heavy weight      (= 900 )
	FONT_DESCRIPTION_CONSTANT("WEIGHT_SEMILIGHT"       , PANGO_WEIGHT_SEMILIGHT       ); // the ultraheavy weight (= 1000) // Since: 1.24
/*==========================================================================*/
/*
 * An enumeration specifying capitalization variant of the font.
 */
	FONT_DESCRIPTION_CONSTANT("VARIANT_NORMAL"         , PANGO_VARIANT_NORMAL          ); // A normal font.
	FONT_DESCRIPTION_CONSTANT("VARIANT_SMALL_CAPS"     , PANGO_VARIANT_SMALL_CAPS      ); // A font with the lower case characters replaced by smaller variants of the capital characters.
}


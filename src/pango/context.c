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


#include "context.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_context;
static zend_object_handlers context_object_handlers;
static zend_class_entry * context_class_entry_ce;

/*==========================================================================*/
/**
 * Get the PGGI\Pango\Context class entry
 *
 * @return zend_class_entry * The class entry
 */
zend_class_entry * pp_context_get_class_entry(void){
	return context_class_entry_ce;
}

/*==========================================================================*/
/**
 * Get the PGGI\Pango\Context object handlers
 *
 * @return zend_object_handlers * The object handlers
 */
zend_object_handlers * pp_context_get_object_handlers(void){
	return &context_object_handlers;
}

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
pp_context_ptr pp_context_new(void){
	pp_context_ptr tor = ecalloc(1, sizeof(pp_context_t));
	tor->to_destroy = 0;
	return tor;
}

/*==========================================================================*/
/**
 * Construct a new zend_object * corresponding to a PGGI\Pango\Context
 * @constructor
 * 
 * @return zend_object * Our object
 */
zend_object *pp_context_object_new(zend_class_entry *class_type){
	ze_pp_context_object *intern;
	intern = ecalloc(1, sizeof(ze_pp_context_object) + zend_object_properties_size(class_type));
	zend_object_std_init  (&intern->std, class_type);
	object_properties_init(&intern->std, class_type);
	// use our context_object_handlers as the PGGI\Pango\Context handlers
	intern->std.handlers = &context_object_handlers;
	return &intern->std;
}

/*==========================================================================*/
/**
 * Destroy a pp_context_t used to contains data for a PGGI\Pango\Context
 * @destructor
 *
 * @param pp_context_ptr intern A pointer to the structure to destroy
 */
void pp_context_dtor(pp_context_ptr intern){
	if (intern->intern && intern->to_destroy){
		g_object_unref(intern->intern);
	}
	efree(intern);
}

/*==========================================================================*/
/**
 * Destroy a zend object corresponding to a PGGI\Pango\Context
 * @destructor
 *
 * @param zend_object *object A pointer to the object corresponding to a PGGI\Pango\Context
 */
void pp_context_object_free_storage(zend_object *object){
	ze_pp_context_object *intern = php_pp_context_fetch_object(object);
	if (!intern) {
		return;
	}
	if (intern->context_ptr){
		pp_context_dtor(intern->context_ptr);
	}
	intern->context_ptr = NULL;
	zend_object_std_dtor(&intern->std);
}

/*==========================================================================*/
/**
 * Destroy a resource corresponding to a PGGI\Pango\Context
 * @destructor
 *
 * @param zend_resource *rsrc A pointer to the object corresponding to a PGGI\Pango\Context
 */
void pp_context_free_resource(zend_resource *rsrc) {
	pp_context_ptr context = (pp_context_ptr) rsrc->ptr;
	pp_context_dtor(context);
}

/***************/
/* PHP Methods */
/***************/

/*==========================================================================*/
/*
 * Object used to run the rendering pipeline of pango
 */

/**
 * Construct a new PGGI\Pango\Context associate to a GWidget
 * @constructor
 * 
 * @param PGGI\GWidget the widget we want associate with our context
 */
PANGO_CONTEXT_METHOD(__construct){
	zval * self = getThis();
	ze_pp_context_object * ze_obj;
	zval * widget;
	// A fontmap
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &widget, gwidget_get_class_entry()) == FAILURE)
		return;
	ze_obj = Z_PP_CONTEXT_P(self);
	ze_gwidget_object * obj = Z_GWIDGET_P(widget);
	ze_obj->context_ptr = pp_context_new();
	ze_obj->context_ptr->intern = gtk_widget_create_pango_context(obj->widget_ptr->intern);
	//pango_font_map_create_context();// font map
	ze_obj->context_ptr->to_destroy = 1;
}

/*==========================================================================*/
/**
 * Retrieves the gravity for the context,
 * it can be different from the baseGravity attribute if its value is GRAVITY_AUTO
 * for which Gravity::getForMatrix() is used to return the gravity from the current context matrix.
 * (function not yet implemented in php)
 *
 * @return int The gravity of the context (PGGI\Pango\Context::GRAVITY_*)
 */
PANGO_CONTEXT_METHOD(getGravity){
	zval * self = getThis();
	ze_pp_context_object * ze_obj;
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_PP_CONTEXT_P(self);
	RETURN_LONG(pango_context_get_gravity(ze_obj->context_ptr->intern));
}

//PGGI\Pango\Context Functions
static const zend_function_entry pp_context_class_functions[] = {
	PANGO_CONTEXT_ME(__construct, arginfo_pp_context_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PANGO_CONTEXT_ME(getGravity , arginfo_pggi_get_long       , ZEND_ACC_PUBLIC)
	PHP_FE_END
};

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
zval *pp_context_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_pp_context_object * intern = Z_PP_CONTEXT_P(object);
	pp_context_ptr c = intern->context_ptr;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	 if(!strcmp(member_val, CONTEXT_BASE_DIR)){
		ZVAL_LONG(rv, pango_context_get_base_dir(c->intern));
	}else if(!strcmp(member_val, CONTEXT_BASE_GRAVITY)){
		ZVAL_LONG(rv, pango_context_get_base_gravity(c->intern));
	}else if(!strcmp(member_val, CONTEXT_GRAVITY_HINT)){
		ZVAL_LONG(rv, pango_context_get_gravity_hint(c->intern));
	}else
		return std_object_handlers.read_property(object, member, type, cache_slot, rv);
	return rv;
}

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
HashTable *pp_context_get_properties(zval *object){
	G_H_UPDATE_INIT(zend_std_get_properties(object));
	ze_pp_context_object * intern = Z_PP_CONTEXT_P(object);
	pp_context_ptr c = intern->context_ptr;
	G_H_UPDATE_LONG(CONTEXT_BASE_DIR    , pango_context_get_base_dir    (c->intern));
	G_H_UPDATE_LONG(CONTEXT_BASE_GRAVITY, pango_context_get_base_gravity(c->intern));
	G_H_UPDATE_LONG(CONTEXT_GRAVITY_HINT, pango_context_get_gravity_hint(c->intern));
	return G_H_UPDATE_RETURN;
}

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
PHP_WRITE_PROP_HANDLER_TYPE pp_context_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_pp_context_object * intern = Z_PP_CONTEXT_P(object);
	pp_context_ptr c = intern->context_ptr;
	long tmp_l;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	if(!strcmp(member_val, CONTEXT_BASE_DIR)){
		convert_to_long(value);
		tmp_l = Z_LVAL_P(value);
		switch(tmp_l){
			case PANGO_DIRECTION_LTR     :
			case PANGO_DIRECTION_RTL     :
			case PANGO_DIRECTION_WEAK_LTR:
			case PANGO_DIRECTION_WEAK_RTL:
			case PANGO_DIRECTION_NEUTRAL :
				pango_context_set_base_dir(c->intern, tmp_l);
				break;
			default:
				zend_throw_exception_ex(pp_exception_get(), 0, "Can't change the baseDir property, needs to be a Context::DIRECTION_*");
				return;
				break;
		}
	}else if(!strcmp(member_val, CONTEXT_BASE_GRAVITY)){
		convert_to_long(value);
		tmp_l = Z_LVAL_P(value);
		switch(tmp_l){
			case PANGO_GRAVITY_SOUTH:
			case PANGO_GRAVITY_EAST :
			case PANGO_GRAVITY_NORTH:
			case PANGO_GRAVITY_WEST :
			case PANGO_GRAVITY_AUTO :
				pango_context_set_base_gravity(c->intern, tmp_l);
				break;
			default:
				zend_throw_exception_ex(pp_exception_get(), 0, "Can't change the baseGravity property, needs to be a Context::GRAVITY_*");
				return;
				break;
		}
	}else if(!strcmp(member_val, CONTEXT_GRAVITY_HINT)){
		convert_to_long(value);
		tmp_l = Z_LVAL_P(value);
		switch(tmp_l){
			case PANGO_GRAVITY_HINT_NATURAL:
			case PANGO_GRAVITY_HINT_STRONG :
			case PANGO_GRAVITY_HINT_LINE   :
				pango_context_set_gravity_hint(c->intern, tmp_l);
				break;
			default:
				zend_throw_exception_ex(pp_exception_get(), 0, "Can't change the gravityHint property, needs to be a Context::GRAVITY_HINT_*");
				return;
				break;
		}
	}else
		PHP_WRITE_PROP_HANDLER_RETURN(std_object_handlers.write_property(object, member, value, cache_slot));
	PHP_WRITE_PROP_HANDLER_RETURN(value);
}

/********************************/
/* Context Class Initialization */
/********************************/

#define DECLARE_CONTEXT_PROP(name) \
DECLARE_CLASS_PROPERTY(context_class_entry_ce, name)

#define CONTEXT_CONSTANT(name, value) \
zend_declare_class_constant_long(context_class_entry_ce, name, sizeof(name)-1, value);

/*==========================================================================*/
/**
 * Initialize the class PGGI\Pango\Context
 *
 * @param int module_number The module number
 */
void pp_context_init(int module_number){
	zend_class_entry ce;
	le_context = zend_register_list_destructors_ex(pp_context_free_resource, NULL, "PGGI\\Pango\\Context", module_number);

	memcpy(&context_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	context_object_handlers.offset         = XtOffsetOf(ze_context_object, std);
	context_object_handlers.free_obj       = pp_context_object_free_storage;
	context_object_handlers.clone_obj      = NULL;
	context_object_handlers.read_property  = pp_context_read_property;
	context_object_handlers.get_properties = pp_context_get_properties;
	context_object_handlers.write_property = pp_context_write_property;
	INIT_CLASS_ENTRY(ce, "PGGI\\Pango\\Context", pp_context_class_functions);
	ce.create_object = pp_context_object_new;
	context_class_entry_ce = zend_register_internal_class(&ce);

	/**
     * @var int the base direction for the context
     * (has to be a DIRECTION_*)
     * The base direction is used in applying the Unicode bidirectional algorithm;
     * if the direction is DIRECTION_LTR or DIRECTION_RTL,
     * then the value will be used as the paragraph direction in the Unicode bidirectional algorithm.
     * A value of DIRECTION_WEAK_LTR or DIRECTION_WEAK_RTL is used only for paragraphs that do not contain any strong characters themselves.
     */
	DECLARE_CONTEXT_PROP(CONTEXT_BASE_DIR    );

	/**
     * @var int the base gravity for the context
     * (has to be a GRAVITY_*)
     */
	DECLARE_CONTEXT_PROP(CONTEXT_BASE_GRAVITY);

	/**
     * @var int gravity hint for the context
     * The gravity hint is used in laying vertical text out,
     * and is only relevant if gravity of the context as returned by Context::getGravity() is set GRAVITY_EAST or GRAVITY_WEST.
     */
	DECLARE_CONTEXT_PROP(CONTEXT_GRAVITY_HINT);

/*==========================================================================*/
/*
 * The Direction type represents a direction in the Unicode bidirectional algorithm;
 * not every value in this enumeration makes sense for every usage of Direction;
 * for example, the return value of unicharDirection() and findBaseDir() cannot be DIRECTION_WEAK_LTR or DIRECTION_WEAK_RTL,
 * since every character is either neutral or has a strong direction;
 * on the other hand DIRECTION_NEUTRAL doesn't make sense to pass to itemizeWithBaseDir().
 * (All those function are not yet implemented in php)
 * See Gravity for how vertical text is handled in Pango.
 */
	CONTEXT_CONSTANT("DIRECTION_LTR"     , PANGO_DIRECTION_LTR     ); // A strong left-to-right direction
	CONTEXT_CONSTANT("DIRECTION_RTL"     , PANGO_DIRECTION_RTL     ); // A strong right-to-left direction
	CONTEXT_CONSTANT("DIRECTION_WEAK_LTR", PANGO_DIRECTION_WEAK_LTR); // A weak left-to-right direction
	CONTEXT_CONSTANT("DIRECTION_WEAK_RTL", PANGO_DIRECTION_WEAK_RTL); // A weak right-to-left direction
	CONTEXT_CONSTANT("DIRECTION_NEUTRAL" , PANGO_DIRECTION_NEUTRAL ); // No direction specified

/*==========================================================================*/
/*
 * The Gravity type represents the orientation of glyphs in a segment of text. This is useful when rendering vertical text layouts.
 * In those situations, the layout is rotated using a non-identity Matrix, and then glyph orientation is controlled using Gravity.
 * Not every value in this enumeration makes sense for every usage of Gravity;
 * for example, GRAVITY_AUTO only can be passed and returned using the property baseGravity of PGGI\Pango\Context
*/
	CONTEXT_CONSTANT("GRAVITY_SOUTH", PANGO_GRAVITY_SOUTH); // Glyphs stand upright (default)
	CONTEXT_CONSTANT("GRAVITY_EAST" , PANGO_GRAVITY_EAST ); // Glyphs are rotated 90 degrees clockwise
	CONTEXT_CONSTANT("GRAVITY_NORTH", PANGO_GRAVITY_NORTH); // Glyphs are upside-down
	CONTEXT_CONSTANT("GRAVITY_WEST" , PANGO_GRAVITY_WEST ); // Glyphs are rotated 90 degrees counter-clockwise
	CONTEXT_CONSTANT("GRAVITY_AUTO" , PANGO_GRAVITY_AUTO ); // Gravity is resolved from the context matrix

/*==========================================================================*/
/*
 * The PangoGravityHint defines how horizontal scripts should behave in a vertical context.
 * That is, English excerpt in a vertical paragraph for example.
 * See Gravity
 */
	CONTEXT_CONSTANT("GRAVITY_HINT_NATURAL", PANGO_GRAVITY_HINT_NATURAL); // scripts will take their natural gravity based on the base gravity and the script. This is the default.
	CONTEXT_CONSTANT("GRAVITY_HINT_STRONG" , PANGO_GRAVITY_HINT_STRONG ); // always use the base gravity set, regardless of the script.
	/**
     * for scripts not in their natural direction (eg. Latin in East gravity),
     * choose per-script gravity such that every script respects the line progression.
     * This means, Latin and Arabic will take opposite gravities and both flow top-to-bottom for example.
     */
	CONTEXT_CONSTANT("GRAVITY_HINT_LINE"   , PANGO_GRAVITY_HINT_LINE   ); 
}


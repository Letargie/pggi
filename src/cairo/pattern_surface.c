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


#include "pattern_surface.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_pattern_surface;
static zend_object_handlers pattern_surface_object_handlers;
static zend_class_entry * pattern_surface_class_entry_ce;

zend_class_entry * pc_pattern_surface_get_class_entry(){
	return pattern_surface_class_entry_ce;
}

zend_object_handlers * pc_pattern_surface_get_object_handlers(){
	return &pattern_surface_object_handlers;
}

/***************/
/* PHP Methods */
/***************/

PATTERN_SURFACE_METHOD(__construct){
	zval * self = getThis();
	ze_pattern_object * ze_obj;
	zval * surface;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &surface, pc_surface_get_class_entry()) == FAILURE)
		return;
	ze_obj = Z_PATTERN_P(self);
	ze_surface_object * c = Z_SURFACE_P(surface);
	ze_obj->pattern_ptr = pc_pattern_new();
	ze_obj->std.handlers = pc_pattern_get_object_handlers();
	cairo_pattern_create_for_surface(c->surface_ptr->intern);
	zend_update_property(pc_pattern_get_class_entry(), self, PATTERN_SURFACE_SURFACE, sizeof(PATTERN_SURFACE_SURFACE) - 1, surface);
	pc_exception(cairo_pattern_status(ze_obj->pattern_ptr->intern));
	ze_obj->pattern_ptr->to_destroy = 1;
}

PATTERN_SURFACE_METHOD(getSurface){
	zval * self = getThis();
	zval * surface;
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	surface = zend_read_property(pc_pattern_get_class_entry(), self, PATTERN_SURFACE_SURFACE, sizeof(PATTERN_SURFACE_SURFACE) - 1, 1, return_value);
	RETURN_ZVAL(surface, 1, 0);
}

// matrix usage
static const zend_function_entry pc_pattern_surface_class_functions[] = {
	PHP_ME(PatternSurface, __construct, arginfo_pc_pattern_surface_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(PatternSurface, getSurface , arginfo_pggi_void                   , ZEND_ACC_PUBLIC)
	PHP_FE_END
};


void pc_pattern_surface_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_pattern_object * intern = Z_PATTERN_P(object);
	pc_pattern_ptr c = intern->pattern_ptr;
	long tmp_l;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	switch(Z_TYPE_P(value)){
		case IS_LONG :
			tmp_l = Z_LVAL_P(value);
			if(!strcmp(member_val, PATTERN_EXTEND)){
				switch(tmp_l){
					case CAIRO_EXTEND_NONE    :
					case CAIRO_EXTEND_REPEAT  :
					case CAIRO_EXTEND_REFLECT :
					case CAIRO_EXTEND_PAD     :
						cairo_pattern_set_extend(c->intern, tmp_l);
						break;
					default:
						zend_throw_exception_ex(pggi_exception_get(), 0, "Can't change the extend property, needs to be a Pattern::EXTEND_*");
						break;
				}
			}else
				std_object_handlers.write_property(object, member, value, cache_slot);
			break;
		default:
			std_object_handlers.write_property(object, member, value, cache_slot);
	}
	pc_exception(cairo_pattern_status(c->intern));
}

/********************************/
/* GWidget Class Initialization */
/********************************/

#define DECLARE_PATTERN_SURFACE_PROP(name) \
DECLARE_CLASS_PROPERTY(pattern_surface_class_entry_ce, name)

#define PATTERN_SURFACE_CONSTANT(name, value) \
zend_declare_class_constant_long(pattern_surface_class_entry_ce, name, sizeof(name)-1, value);


void pc_pattern_surface_init(int module_number){
	zend_class_entry ce;
	le_pattern_surface = zend_register_list_destructors_ex(pc_pattern_free_resource, NULL, "PGGI\\Cairo\\PatternSurface", module_number);

	memcpy(&pattern_surface_object_handlers, pc_pattern_get_object_handlers(), sizeof(zend_object_handlers));
	pattern_surface_object_handlers.write_property = pc_pattern_surface_write_property;
	INIT_CLASS_ENTRY(ce, "PGGI\\Cairo\\PatternSurface", pc_pattern_surface_class_functions);
	pattern_surface_class_entry_ce = zend_register_internal_class_ex(&ce, pc_pattern_get_class_entry());

	PATTERN_SURFACE_CONSTANT("EXTEND_REFLECT", CAIRO_EXTEND_REFLECT);
	PATTERN_SURFACE_CONSTANT("EXTEND_PAD"    , CAIRO_EXTEND_PAD    );

	DECLARE_PATTERN_SURFACE_PROP(PATTERN_SURFACE_SURFACE);
}


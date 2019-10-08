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


#include "pattern_solid.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_pattern_solid;
static zend_object_handlers pattern_solid_object_handlers;
static zend_class_entry * pattern_solid_class_entry_ce;

zend_class_entry * pc_pattern_solid_get_class_entry(){
	return pattern_solid_class_entry_ce;
}

zend_object_handlers * pc_pattern_solid_get_object_handlers(){
	return &pattern_solid_object_handlers;
}

/***************/
/* PHP Methods */
/***************/

PATTERN_SOLID_METHOD(__construct){
	zval * self = getThis();
	ze_pattern_object * ze_obj;
	double r, g, b, a = -1;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "ddd|d", &r, &g, &b, &a) == FAILURE)
		return;
	ze_obj = Z_PATTERN_P(self);
	ze_obj->pattern_ptr = pc_pattern_new();
	if(a != -1)
		ze_obj->pattern_ptr->intern = cairo_pattern_create_rgba(r, g, b, a);
	else
		ze_obj->pattern_ptr->intern = cairo_pattern_create_rgb(r, g, b);
	pc_exception(cairo_pattern_status(ze_obj->pattern_ptr->intern));
	ze_obj->pattern_ptr->to_destroy = 1;
}

// matrix usage
static const zend_function_entry pc_pattern_solid_class_functions[] = {
	PHP_ME(PatternSolid, __construct, arginfo_pc_pattern_solid_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_FE_END
};

/********************************/
/* GWidget Class Initialization */
/********************************/

#define DECLARE_PATTERN_SOLID_PROP(name) \
DECLARE_CLASS_PROPERTY(pattern_solid_class_entry_ce, name)

#define DECLARE_PATTERN_SOLID_PRIV_PROP(name) \
DECLARE_PRIVATE_CLASS_PROPERTY(pattern_solid_class_entry_ce, name)

#define PATTERN_SOLID_CONSTANT(name, value) \
zend_declare_class_constant_long(pattern_solid_class_entry_ce, name, sizeof(name)-1, value);


void pc_pattern_solid_init(int module_number){
	zend_class_entry ce;
	le_pattern_solid = zend_register_list_destructors_ex(pc_pattern_free_resource, NULL, "PGGI\\Cairo\\PatternSolid", module_number);

	memcpy(&pattern_solid_object_handlers, pc_pattern_get_object_handlers(), sizeof(zend_object_handlers));
	INIT_CLASS_ENTRY(ce, "PGGI\\Cairo\\PatternSolid", pc_pattern_solid_class_functions);
	pattern_solid_class_entry_ce = zend_register_internal_class_ex(&ce, pc_pattern_get_class_entry());

	DECLARE_PATTERN_SOLID_PRIV_PROP("surface");
}


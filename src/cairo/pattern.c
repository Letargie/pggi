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


#include "pattern.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_pattern;
static zend_object_handlers pattern_object_handlers;
static zend_class_entry * pattern_class_entry_ce;

zend_class_entry * pc_pattern_get_class_entry(){
	return pattern_class_entry_ce;
}

zend_object_handlers * pc_pattern_get_object_handlers(){
	return &pattern_object_handlers;
}

/****************************/
/* Memory handling function */
/****************************/

pc_pattern_ptr pc_pattern_new(){
	pc_pattern_ptr tor = ecalloc(1, sizeof(pc_pattern_t));
	tor->to_destroy = 0;
	return tor;
}

zend_object *pc_pattern_object_new(zend_class_entry *class_type){
	ze_pattern_object *intern;
	intern = ecalloc(1, sizeof(ze_pattern_object) + zend_object_properties_size(class_type));
	zend_object_std_init  (&intern->std, class_type);
	object_properties_init(&intern->std, class_type);
	intern->std.handlers = &pattern_object_handlers;
	return &intern->std;
}

void pc_pattern_dtor(pc_pattern_ptr intern){
	if (intern->intern && intern->to_destroy){
		cairo_pattern_destroy(intern->intern);
	}
	efree(intern);
}

void pc_pattern_object_free_storage(zend_object *object){
	ze_pattern_object *intern = php_pattern_fetch_object(object);
	if (!intern) {
		return;
	}
	if (intern->pattern_ptr){
		pc_pattern_dtor(intern->pattern_ptr);
	}
	intern->pattern_ptr = NULL;
	zend_object_std_dtor(&intern->std);
}

void pc_pattern_free_resource(zend_resource *rsrc) {
	pc_pattern_ptr pattern = (pc_pattern_ptr) rsrc->ptr;
	pc_pattern_dtor(pattern);
}

/***************/
/* PHP Methods */
/***************/

// matrix usage
static const zend_function_entry pc_pattern_class_functions[] = {
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *pc_pattern_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_pattern_object * intern = Z_PATTERN_P(object);
	pc_pattern_ptr c = intern->pattern_ptr;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	 if(!strcmp(member_val, PATTERN_EXTEND)){
		ZVAL_LONG(rv, cairo_pattern_get_extend(c->intern));
	}else if(!strcmp(member_val, PATTERN_FILTER)){
		ZVAL_LONG(rv, cairo_pattern_get_filter(c->intern));
	}else
		return std_object_handlers.read_property(object, member, type, cache_slot, rv);
	pc_exception(cairo_pattern_status(c->intern));
	return rv;
}

HashTable *pc_pattern_get_properties(zval *object){
	G_H_UPDATE_INIT(zend_std_get_properties(object));
	ze_pattern_object * intern = Z_PATTERN_P(object);
	pc_pattern_ptr c = intern->pattern_ptr;
	G_H_UPDATE_LONG(PATTERN_FILTER, cairo_pattern_get_filter(c->intern));
	G_H_UPDATE_LONG(PATTERN_EXTEND, cairo_pattern_get_extend(c->intern));
	pc_exception(cairo_pattern_status(c->intern));
	return G_H_UPDATE_RETURN;
}

void pc_pattern_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_pattern_object * intern = Z_PATTERN_P(object);
	pc_pattern_ptr c = intern->pattern_ptr;
	long tmp_l;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	switch(Z_TYPE_P(value)){
		case IS_LONG :
			tmp_l = Z_LVAL_P(value);
			if(!strcmp(member_val, PATTERN_FILTER)){
				switch(tmp_l){
					case CAIRO_FILTER_FAST     :
					case CAIRO_FILTER_GOOD     :
					case CAIRO_FILTER_BEST     :
					case CAIRO_FILTER_NEAREST  :
					case CAIRO_FILTER_BILINEAR :
						cairo_pattern_set_filter(c->intern, tmp_l);
						break;
					default:
						zend_throw_exception_ex(pggi_exception_get(), 0, "Can't change the filter property, needs to be a Pattern::FILTER_*");
						return;
						break;
				}
			}else if(!strcmp(member_val, PATTERN_EXTEND)){
				switch(tmp_l){
					case CAIRO_EXTEND_NONE   :
					case CAIRO_EXTEND_REPEAT :
						cairo_pattern_set_extend(c->intern, tmp_l);
						break;
					default:
						zend_throw_exception_ex(pggi_exception_get(), 0, "Can't change the extend property, needs to be a Pattern::EXTEND_*");
						return;
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

#define DECLARE_PATTERN_PROP(name) \
DECLARE_CLASS_PROPERTY(pattern_class_entry_ce, name)

#define PATTERN_CONSTANT(name, value) \
zend_declare_class_constant_long(pattern_class_entry_ce, name, sizeof(name)-1, value);


void pc_pattern_init(int module_number){
	zend_class_entry ce;
	le_pattern = zend_register_list_destructors_ex(pc_pattern_free_resource, NULL, "PGGI\\Cairo\\Pattern", module_number);

	memcpy(&pattern_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	pattern_object_handlers.offset         = XtOffsetOf(ze_pattern_object, std);
	pattern_object_handlers.free_obj       = pc_pattern_object_free_storage;
	pattern_object_handlers.clone_obj      = NULL;
	pattern_object_handlers.read_property  = pc_pattern_read_property;
	pattern_object_handlers.get_properties = pc_pattern_get_properties;
	pattern_object_handlers.write_property = pc_pattern_write_property;
	INIT_CLASS_ENTRY(ce, "PGGI\\Cairo\\Pattern", pc_pattern_class_functions);
	ce.create_object       = pc_pattern_object_new;
	pattern_class_entry_ce = zend_register_internal_class(&ce);
	pattern_class_entry_ce->ce_flags |= ZEND_ACC_ABSTRACT;

	DECLARE_PATTERN_PROP(PATTERN_EXTEND);
	DECLARE_PATTERN_PROP(PATTERN_FILTER);

	PATTERN_CONSTANT("EXTEND_NONE"   , CAIRO_EXTEND_NONE   );
	PATTERN_CONSTANT("EXTEND_REPEAT" , CAIRO_EXTEND_REPEAT );

	PATTERN_CONSTANT("FILTER_FAST"    , CAIRO_FILTER_FAST    );
	PATTERN_CONSTANT("FILTER_GOOD"    , CAIRO_FILTER_GOOD    );
	PATTERN_CONSTANT("FILTER_BEST"    , CAIRO_FILTER_BEST    );
	PATTERN_CONSTANT("FILTER_NEAREST" , CAIRO_FILTER_NEAREST );
	PATTERN_CONSTANT("FILTER_BILINEAR", CAIRO_FILTER_BILINEAR);

}


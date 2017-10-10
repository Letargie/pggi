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


#include "surface.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_surface;
static zend_object_handlers surface_object_handlers;
static zend_class_entry * surface_class_entry_ce;

zend_class_entry * pc_surface_get_class_entry(){
	return surface_class_entry_ce;
}

zend_object_handlers * pc_surface_get_object_handlers(){
	return &surface_object_handlers;
}

/****************************/
/* Memory handling function */
/****************************/

pc_surface_ptr pc_surface_new(){
	pc_surface_ptr tor = ecalloc(1, sizeof(pc_surface_t));
	return tor;
}

zend_object *pc_surface_object_new(zend_class_entry *class_type){
	ze_surface_object *intern;
	intern = ecalloc(1, sizeof(ze_surface_object) + zend_object_properties_size(class_type));
	zend_object_std_init  (&intern->std, class_type);
	object_properties_init(&intern->std, class_type);
	intern->std.handlers = &surface_object_handlers;
	return &intern->std;
}

void pc_surface_dtor(pc_surface_ptr intern){
	if (intern->intern){
		cairo_surface_destroy(intern->intern);
	}
	efree(intern);
}

void pc_surface_object_free_storage(zend_object *object){
	ze_surface_object *intern = php_surface_fetch_object(object);
	if (!intern) {
		return;
	}
	if (intern->surface_ptr){
		pc_surface_dtor(intern->surface_ptr);
	}
	intern->surface_ptr = NULL;
	zend_object_std_dtor(&intern->std);
}

void pc_surface_free_resource(zend_resource *rsrc) {
	pc_surface_ptr surface = (pc_surface_ptr) rsrc->ptr;
	pc_surface_dtor(surface);
}

/***************/
/* PHP Methods */
/***************/

static const zend_function_entry pc_surface_class_functions[] = {
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *pc_surface_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	/*ze_surface_object * intern = Z_SURFACE_P(object);
	pc_surface_ptr c = intern->surface_ptr;
	const char * tmp;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);*/

	return std_object_handlers.read_property(object, member, type, cache_slot, rv);
}

HashTable *pc_surface_get_properties(zval *object){
	/*G_H_UPDATE_INIT(zend_std_get_properties(object));
	const char * tmp;
	ze_surface_object * intern = Z_SURFACE_P(object);
	pc_surface_ptr c = intern->surface_ptr;
	
	return G_H_UPDATE_RETURN;*/
	return zend_std_get_properties(object);
}

void pc_surface_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	/*ze_surface_object * intern = Z_SURFACE_P(object);
	pc_surface_ptr c = intern->surface_ptr;

	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);*/
	switch(Z_TYPE_P(value)){
		default:
			std_object_handlers.write_property(object, member, value, cache_slot);
	}
}

/********************************/
/* GWidget Class Initialization */
/********************************/

#define DECLARE_SURFACE_PROP(name) \
DECLARE_CLASS_PROPERTY(surface_class_entry_ce, name)

#define SURFACE_CONSTANT(name, value) \
zend_declare_class_constant_long(surface_class_entry_ce, name, sizeof(name)-1, value);


void pc_surface_init(int module_number){
	zend_class_entry ce;
	le_surface = zend_register_list_destructors_ex(pc_surface_free_resource, NULL, "Cairo\\Surface", module_number);

	memcpy(&surface_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	surface_object_handlers.offset         = XtOffsetOf(ze_surface_object, std);
	surface_object_handlers.free_obj       = pc_surface_object_free_storage;
	surface_object_handlers.clone_obj      = NULL;
	surface_object_handlers.read_property  = pc_surface_read_property;
	surface_object_handlers.get_properties = pc_surface_get_properties;
	surface_object_handlers.write_property = pc_surface_write_property;
	INIT_CLASS_ENTRY(ce, "Cairo\\Surface", pc_surface_class_functions);
	ce.create_object = pc_surface_object_new;
	surface_class_entry_ce = zend_register_internal_class(&ce);
	surface_class_entry_ce->ce_flags |= ZEND_ACC_ABSTRACT;

	SURFACE_CONSTANT("CONTENT_COLOR"      ,CAIRO_CONTENT_COLOR      );
	SURFACE_CONSTANT("CONTENT_ALPHA"      ,CAIRO_CONTENT_ALPHA      );
	SURFACE_CONSTANT("CONTENT_COLOR_ALPHA",CAIRO_CONTENT_COLOR_ALPHA);
}


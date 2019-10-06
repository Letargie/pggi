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


#include "gevent.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_gevent;
static zend_object_handlers gevent_object_handlers;
static zend_class_entry * gevent_class_entry_ce;

zend_class_entry * gevent_get_class_entry(){
	return gevent_class_entry_ce;
}

zend_object_handlers * gevent_get_object_handlers(){
	return &gevent_object_handlers;
}

zend_object * gevent_ctor(zend_class_entry *ce, GdkEvent * event){
	zend_object * tor = gevent_object_new(ce);
	ze_gevent_object * obj = php_gevent_fetch_object(tor);
	//obj->std.handlers = &gevent_object_handlers;
	obj->event_ptr = gevent_new();
	return tor;
}

/****************************/
/* Memory handling function */
/****************************/

gevent_ptr gevent_new(){
	gevent_ptr tor = ecalloc(1, sizeof(gevent_t));
	return tor;
}

zend_object *gevent_object_new(zend_class_entry *class_type){
	ze_gevent_object *intern;
	intern = ecalloc(1, sizeof(ze_gevent_object) + zend_object_properties_size(class_type));
	zend_object_std_init	(&intern->std, class_type);
	object_properties_init	(&intern->std, class_type);
	intern->std.handlers = &gevent_object_handlers;
	return &intern->std;
}

void gevent_dtor(gevent_ptr intern){
	efree(intern);
}

void gevent_object_free_storage(zend_object *object){
	ze_gevent_object *intern = php_gevent_fetch_object(object);
	if (!intern) {
		return;
	}
	if (intern->event_ptr){
		gevent_dtor(intern->event_ptr);
	}
	intern->event_ptr = NULL;
	zend_object_std_dtor(&intern->std);
}

void gevent_free_resource(zend_resource *rsrc) {
	gevent_ptr event = (gevent_ptr) rsrc->ptr;
	gevent_dtor(event);
}

/***************/
/* PHP Methods */
/***************/

GEVENT_METHOD(__construct){}

static const zend_function_entry gevent_class_functions[] = {
	PHP_ME(GEvent, __construct , arginfo_pggi_void, ZEND_ACC_PRIVATE | ZEND_ACC_CTOR)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *gevent_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	return std_object_handlers.read_property(object, member, type, cache_slot, rv);
}

HashTable *gevent_get_properties(zval *object){
	//G_H_UPDATE_INIT(zend_std_get_properties(object));
	//return G_H_UPDATE_RETURN;
	return zend_std_get_properties(object);
}

PHP_WRITE_PROP_HANDLER_TYPE gevent_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	PHP_WRITE_PROP_HANDLER_RETURN(std_object_handlers.write_property(object, member, value, cache_slot));
}

/*******************************/
/* GEvent Class Initialization */
/*******************************/

#define DECLARE_GEVENT_PROP(name) \
DECLARE_CLASS_PROPERTY(gevent_class_entry_ce, name)

#define GEVENT_CONSTANT(name, value) \
zend_declare_class_constant_long(gevent_class_entry_ce, name, sizeof(name)-1, value);


void gevent_init(int module_number){
	zend_class_entry ce;
	le_gevent = zend_register_list_destructors_ex(gevent_free_resource, NULL, "PGGI\\GDK\\Event", module_number);

	memcpy(&gevent_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	gevent_object_handlers.offset         = XtOffsetOf(ze_gevent_object, std);
	gevent_object_handlers.free_obj       = gevent_object_free_storage;
	gevent_object_handlers.clone_obj      = NULL;
	gevent_object_handlers.read_property  = gevent_read_property;
	gevent_object_handlers.get_properties = gevent_get_properties;
	gevent_object_handlers.write_property = gevent_write_property;
	INIT_CLASS_ENTRY(ce, "PGGI\\GDK\\Event", gevent_class_functions);
	ce.create_object = gevent_object_new;
	gevent_class_entry_ce = zend_register_internal_class(&ce);
}



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


#include "rgba.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_rgba;
static zend_object_handlers rgba_object_handlers;
static zend_class_entry * rgba_class_entry_ce;

zend_class_entry * rgba_get_class_entry(){
	return rgba_class_entry_ce;
}

zend_object_handlers * rgba_get_object_handlers(){
	return &rgba_object_handlers;
}

/****************************/
/* Memory handling function */
/****************************/

rgba_ptr rgba_new(){
	rgba_ptr tor = ecalloc(1, sizeof(rgba_t));
	tor->to_destroy = 0;
	return tor;
}

zend_object *rgba_object_new(zend_class_entry *class_type){
	ze_rgba_object *intern;
	intern = ecalloc(1, sizeof(ze_rgba_object) + zend_object_properties_size(class_type));
	zend_object_std_init	(&intern->std, class_type);
	object_properties_init	(&intern->std, class_type);
	intern->std.handlers = &rgba_object_handlers;
	return &intern->std;
}

void rgba_dtor(rgba_ptr intern){
	if(intern->to_destroy)
		gdk_rgba_free(intern->color);
	efree(intern);
}

void rgba_object_free_storage(zend_object *object){
	ze_rgba_object *intern = php_rgba_fetch_object(object);
	if (!intern) {
		return;
	}
	if (intern->ptr){
		rgba_dtor(intern->ptr);
	}
	intern->ptr = NULL;
	zend_object_std_dtor(&intern->std);
}

void rgba_free_resource(zend_resource *rsrc) {
	rgba_ptr event = (rgba_ptr) rsrc->ptr;
	rgba_dtor(event);
}

/***************/
/* PHP Methods */
/***************/

RGBA_METHOD(__construct){
	char * data;
	size_t data_len;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &data ,&data_len) == FAILURE)
		return;
	ze_rgba_object * intern = Z_RGBA_P(getThis());
	GdkRGBA tmp;
	if(!gdk_rgba_parse(&tmp, data))
		zend_throw_exception_ex(pggi_exception_get(), 0, "error during the parsing");
	intern->ptr = rgba_new();
	intern->ptr->color = gdk_rgba_copy(&tmp);
	intern->ptr->to_destroy = 1;
}

RGBA_METHOD(copy){
	if (pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_rgba_object * intern = Z_RGBA_P(getThis());
	
	zend_object * tor = rgba_object_new(rgba_class_entry_ce);
	ze_rgba_object * tmp = php_rgba_fetch_object(tor);
	tmp->ptr = rgba_new();
	tmp->ptr->color = gdk_rgba_copy(intern->ptr->color);
	tmp->ptr->to_destroy = 1;
	RETURN_OBJ(tor);
}

RGBA_METHOD(equals){
	zval * obj;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &obj, rgba_class_entry_ce) == FAILURE)
		return;
	ze_rgba_object * intern = Z_RGBA_P(getThis());
	ze_rgba_object * other = Z_RGBA_P(obj);
	RETURN_BOOL(gdk_rgba_equal((gconstpointer)intern->ptr->color, (gconstpointer)other->ptr->color));
}


static const zend_function_entry rgba_class_functions[] = {
	PHP_ME(RGBA, __construct , arginfo_rgba_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(RGBA, copy        , arginfo_rgba_copy     , ZEND_ACC_PUBLIC)
	PHP_ME(RGBA, equals      , arginfo_rgba_equals   , ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *rgba_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_rgba_object * intern = Z_RGBA_P(object);
	rgba_ptr c = intern->ptr;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	if(!strcmp(member_val, RGBA_RED)){
		ZVAL_DOUBLE(rv, c->color->red);
	}else if(!strcmp(member_val, RGBA_GREEN)){
		ZVAL_DOUBLE(rv, c->color->green);
	}else if(!strcmp(member_val, RGBA_BLUE)){
		ZVAL_DOUBLE(rv, c->color->blue);
	}else if(!strcmp(member_val, RGBA_ALPHA)){
		ZVAL_DOUBLE(rv, c->color->alpha);
	}else
		return std_object_handlers.read_property(object, member, type, cache_slot, rv);
	return rv;
}

HashTable *rgba_get_properties(zval *object){
	G_H_UPDATE_INIT(zend_std_get_properties(object));
	ze_rgba_object * intern = Z_RGBA_P(object);
	rgba_ptr c = intern->ptr;
	G_H_UPDATE_DOUBLE(RGBA_RED  , c->color->red  );
	G_H_UPDATE_DOUBLE(RGBA_GREEN, c->color->green);
	G_H_UPDATE_DOUBLE(RGBA_BLUE , c->color->blue );
	G_H_UPDATE_DOUBLE(RGBA_ALPHA, c->color->alpha);
	return zend_std_get_properties(object);
}

PHP_WRITE_PROP_HANDLER_TYPE rgba_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_rgba_object * intern = Z_RGBA_P(object);
	rgba_ptr c = intern->ptr;
	double tmp_d;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	if(!strcmp(member_val, RGBA_RED)){
		convert_to_double(value);
		tmp_d = Z_DVAL_P(value);
		if(tmp_d < 0 || tmp_d > 1)
			zend_throw_exception_ex(pggi_exception_get(), 0, "the value should be between 0 and 1");
		c->color->red = tmp_d;
	}else if(!strcmp(member_val, RGBA_GREEN)){
		convert_to_double(value);
		tmp_d = Z_DVAL_P(value);
		if(tmp_d < 0 || tmp_d > 1)
			zend_throw_exception_ex(pggi_exception_get(), 0, "the value should be between 0 and 1");
		c->color->green = tmp_d;
	}else if(!strcmp(member_val, RGBA_BLUE)){
		convert_to_double(value);
		tmp_d = Z_DVAL_P(value);
		if(tmp_d < 0 || tmp_d > 1)
			zend_throw_exception_ex(pggi_exception_get(), 0, "the value should be between 0 and 1");
		c->color->blue = tmp_d;
	}else if(!strcmp(member_val, RGBA_ALPHA)){
		convert_to_double(value);
		tmp_d = Z_DVAL_P(value);
		if(tmp_d < 0 || tmp_d > 1)
			zend_throw_exception_ex(pggi_exception_get(), 0, "the value should be between 0 and 1");
		c->color->alpha = tmp_d;
	}else
		PHP_WRITE_PROP_HANDLER_RETURN(std_object_handlers.write_property(object, member, value, cache_slot));
	PHP_WRITE_PROP_HANDLER_RETURN(value);
}

#define DECLARE_RGBA_PROP(name) \
DECLARE_CLASS_PROPERTY(rgba_class_entry_ce, name)

void rgba_init(int module_number){
	zend_class_entry ce;
	le_rgba = zend_register_list_destructors_ex(rgba_free_resource, NULL, "PGGI\\GDK\\RGBA", module_number);

	memcpy(&rgba_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	rgba_object_handlers.offset         = XtOffsetOf(ze_rgba_object, std);
	rgba_object_handlers.free_obj       = rgba_object_free_storage;
	rgba_object_handlers.clone_obj      = NULL;
	rgba_object_handlers.read_property  = rgba_read_property;
	rgba_object_handlers.get_properties = rgba_get_properties;
	rgba_object_handlers.write_property = rgba_write_property;
	INIT_CLASS_ENTRY(ce, "PGGI\\GDK\\RGBA", rgba_class_functions);
	ce.create_object = rgba_object_new;
	rgba_class_entry_ce = zend_register_internal_class(&ce);

	DECLARE_RGBA_PROP(RGBA_RED  );
	DECLARE_RGBA_PROP(RGBA_GREEN);
	DECLARE_RGBA_PROP(RGBA_BLUE );
	DECLARE_RGBA_PROP(RGBA_ALPHA);
}



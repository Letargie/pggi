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


#include "gpixbuf.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_gpixbuf;
static zend_object_handlers gpixbuf_object_handlers;
static zend_class_entry * gpixbuf_class_entry_ce;

zend_class_entry * gpixbuf_get_class_entry(){
	return gpixbuf_class_entry_ce;
}

zend_object_handlers * gpixbuf_get_object_handlers(){
	return &gpixbuf_object_handlers;
}

/****************************/
/* Memory handling function */
/****************************/

gpixbuf_ptr gpixbuf_new(){
	gpixbuf_ptr tor = ecalloc(1, sizeof(gpixbuf_t));
	tor->to_destroy = 0;
	return tor;
}

zend_object *gpixbuf_object_new(zend_class_entry *class_type){
	ze_gpixbuf_object *intern;
	intern = ecalloc(1, sizeof(ze_gpixbuf_object) + zend_object_properties_size(class_type));
	zend_object_std_init	(&intern->std, class_type);
	object_properties_init	(&intern->std, class_type);
	intern->std.handlers = &gpixbuf_object_handlers;
	return &intern->std;
}

zend_object * gpixbuf_ctor(zend_class_entry *ce, GdkPixbuf * pixbuf){
	zend_object * tor = gpixbuf_object_new(ce);
	ze_gpixbuf_object * obj = php_gpixbuf_fetch_object(tor);
	//obj->std.handlers = &gevent_object_handlers;
	obj->pixbuf_ptr = gpixbuf_new();
	obj->pixbuf_ptr->intern = pixbuf;
	return tor;
}

void gpixbuf_dtor(gpixbuf_ptr intern){
	if (intern->intern && intern->to_destroy){
		g_object_unref(intern->intern);
	}
	efree(intern);
}

void gpixbuf_object_free_storage(zend_object *object){
	ze_gpixbuf_object *intern = php_gpixbuf_fetch_object(object);
	if (!intern) {
		return;
	}
	if (intern->pixbuf_ptr){
		gpixbuf_dtor(intern->pixbuf_ptr);
	}
	intern->pixbuf_ptr = NULL;
	zend_object_std_dtor(&intern->std);
}

void gpixbuf_free_resource(zend_resource *rsrc) {
	gpixbuf_ptr pixbuf = (gpixbuf_ptr) rsrc->ptr;
	gpixbuf_dtor(pixbuf);
}

/***************/
/* PHP Methods */
/***************/

GPIXBUF_METHOD(__construct){
	int has_alpha;
	long bits_per_sample, width, height;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "blll", &has_alpha, &bits_per_sample, &width, &height) == FAILURE)
		return;
	ze_gpixbuf_object * intern = Z_GPIXBUF_P(getThis());
	intern->pixbuf_ptr = gpixbuf_new();
	intern->pixbuf_ptr->to_destroy = 1;
	intern->pixbuf_ptr->intern = gdk_pixbuf_new(GDK_COLORSPACE_RGB, has_alpha, bits_per_sample, width, height);
}


GPIXBUF_METHOD(createFromFile){
	char * filename;
	int len, preserve_ratio = 1;
	long width = -1, height = -1;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s|llb", &filename, &len, &width, &height, &preserve_ratio) == FAILURE)
		return;
	GError * error = NULL;
	GdkPixbuf * pix;
	if(width == -1 && height == -1){
		pix = gdk_pixbuf_new_from_file(filename, &error);
	}else if(preserve_ratio == 1){
		pix = gdk_pixbuf_new_from_file_at_size (filename, width, height, &error);
	}else{
		pix = gdk_pixbuf_new_from_file_at_scale (filename, width, height, preserve_ratio, &error);
	}
	if(!pix){
		zend_throw_exception_ex(pggi_exception_get(), 0, "%s", error->message);
		return;
	}
	zval new_pixbuf;
	ZVAL_OBJ(&new_pixbuf, gpixbuf_ctor(gpixbuf_get_class_entry(), pix));
	RETURN_ZVAL(&new_pixbuf, 1, 0);
}

GPIXBUF_METHOD(getWidth){
	if (pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_gpixbuf_object * intern = Z_GPIXBUF_P(getThis());
	RETURN_LONG(gdk_pixbuf_get_width(intern->pixbuf_ptr->intern));
}

GPIXBUF_METHOD(getHeight){
	if (pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_gpixbuf_object * intern = Z_GPIXBUF_P(getThis());
	RETURN_LONG(gdk_pixbuf_get_height(intern->pixbuf_ptr->intern));
}

static const zend_function_entry gpixbuf_class_functions[] = {
	PHP_ME(GPixbuf, __construct   , arginfo_pixbuf_construct       , ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(GPixbuf, createFromFile, arginfo_pixbuf_create_from_file, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(GPixbuf, getWidth      , arginfo_pggi_get_long          , ZEND_ACC_PUBLIC)
	PHP_ME(GPixbuf, getHeight     , arginfo_pggi_get_long          , ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *gpixbuf_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	return std_object_handlers.read_property(object, member, type, cache_slot, rv);
}

HashTable *gpixbuf_get_properties(zval *object){
	//G_H_UPDATE_INIT(zend_std_get_properties(object));
	//return G_H_UPDATE_RETURN;
	return zend_std_get_properties(object);
}

PHP_WRITE_PROP_HANDLER_TYPE gpixbuf_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	PHP_WRITE_PROP_HANDLER_RETURN(std_object_handlers.write_property(object, member, value, cache_slot));
}

/********************************/
/* GPixbuf Class Initialization */
/********************************/

#define DECLARE_GPIXBUF_PROP(name) \
DECLARE_CLASS_PROPERTY(gpixbuf_class_entry_ce, name)

#define GPIXBUF_CONSTANT(name, value) \
zend_declare_class_constant_long(gpixbuf_class_entry_ce, name, sizeof(name)-1, value);


void gpixbuf_init(int module_number){
	zend_class_entry ce;
	le_gpixbuf = zend_register_list_destructors_ex(gpixbuf_free_resource, NULL, "PGGI\\GDK\\Pixbuf", module_number);

	memcpy(&gpixbuf_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	gpixbuf_object_handlers.offset         = XtOffsetOf(ze_gpixbuf_object, std);
	gpixbuf_object_handlers.free_obj       = gpixbuf_object_free_storage;
	gpixbuf_object_handlers.clone_obj      = NULL;
	gpixbuf_object_handlers.read_property  = gpixbuf_read_property;
	gpixbuf_object_handlers.get_properties = gpixbuf_get_properties;
	gpixbuf_object_handlers.write_property = gpixbuf_write_property;
	INIT_CLASS_ENTRY(ce, "PGGI\\GDK\\Pixbuf", gpixbuf_class_functions);
	ce.create_object = gpixbuf_object_new;
	gpixbuf_class_entry_ce = zend_register_internal_class(&ce);

}



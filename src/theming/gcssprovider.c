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


#include "gcssprovider.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_gcss_provider;
static zend_object_handlers gcss_provider_object_handlers;
static zend_class_entry * gcss_provider_class_entry_ce;

zend_class_entry * gcss_provider_get_class_entry(){
	return gcss_provider_class_entry_ce;
}

zend_object_handlers * gcss_provider_get_object_handlers(){
	return &gcss_provider_object_handlers;
}

/****************************/
/* Memory handling function */
/****************************/

gcss_provider_ptr gcss_provider_new(){
	gcss_provider_ptr tor = ecalloc(1, sizeof(gcss_provider_t));
	array_init(&tor->signals);
	return tor;
}

zend_object *gcss_provider_object_new(zend_class_entry *class_type){
	ze_gcss_provider_object *intern;
	intern = ecalloc(1, sizeof(ze_gcss_provider_object) + zend_object_properties_size(class_type));
	zend_object_std_init	(&intern->std, class_type);
	object_properties_init	(&intern->std, class_type);
	intern->std.handlers = &gcss_provider_object_handlers;
	return &intern->std;
}

void gcss_provider_dtor(gcss_provider_ptr intern){
	if (intern->intern){
		g_object_unref(intern->intern);
	}
	zend_hash_destroy(Z_ARRVAL_P(&intern->signals));
	efree(intern);
}

void gcss_provider_object_free_storage(zend_object *object){
	ze_gcss_provider_object *intern = php_gcss_provider_fetch_object(object);
	if (!intern) {
		return;
	}
	if (intern->provider_ptr){
		gcss_provider_dtor(intern->provider_ptr);
	}
	intern->provider_ptr = NULL;
	zend_object_std_dtor(&intern->std);
}

void gcss_provider_free_resource(zend_resource *rsrc) {
	gcss_provider_ptr provider = (gcss_provider_ptr) rsrc->ptr;
	gcss_provider_dtor(provider);
}


#define INDEX_ON_FUNCTION_NAME 1
#define INDEX_ON_FUNCTION_PARAM 2

void gcss_provider_function(gpointer data, unsigned int type){
	zval retval;
	ze_gcss_provider_object * b = data;
	zval * array, * value, * zv;
	zval args[2];
	if(type){
		zend_object * this = php_gcss_provider_reverse_object(b);
		ZVAL_OBJ(&args[0], this);
		array = &b->provider_ptr->signals;
		value = (zend_hash_index_find(Z_ARRVAL_P(array), type));
		if(value != NULL){
			ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(value), zv){
				zval * function	= zend_hash_index_find(Z_ARRVAL_P(zv), INDEX_ON_FUNCTION_NAME);
				zval * tmp = zend_hash_index_find(Z_ARRVAL_P(zv), INDEX_ON_FUNCTION_PARAM);
				if(tmp){
					ZVAL_COPY(&args[1], tmp);
				}else
					ZVAL_NULL(&args[1]);
				if(call_user_function(EG(function_table), NULL, function, &retval, 1, args) != SUCCESS){
					zend_error(E_ERROR, "Function call failed");
				}
			} ZEND_HASH_FOREACH_END();
		}
	}
}

/*********************************/
/* internal on-related functions */
/*********************************/

/*
void gcss_provider_func_changed(GtkCssProvider* context, gpointer data){
	gcss_provider_function(data, gsignal_gcss_provider_changed);
}*/

/***************/
/* PHP Methods */
/***************/

GCSS_PROVIDER_METHOD(__construct){
	ze_gcss_provider_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none(self) == FAILURE)
		return ;
	ze_obj = Z_GCSS_PROVIDER_P(self);
	ze_obj->provider_ptr = gcss_provider_new();
	ze_obj->provider_ptr->intern = gtk_css_provider_new();
}

/*

GCSS_PROVIDER_METHOD(on){
	zval * function, * data,* narray, * this, * param = NULL;
	long val;
	ze_gcss_provider_object *ze_obj = NULL;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "lz|z", &val ,&function, &param) == FAILURE)
		return;
	this = getThis();
	ze_obj = Z_GCSS_PROVIDER_P(this);
	switch(val){
		case gsignal_gcss_provider_changed :
			break;
		default :
			zend_error(E_ERROR, "Signal unknown");
	}
	zval * data_to_insert = ecalloc(1,sizeof(zval));
	array_init(data_to_insert);
	zend_hash_index_add(Z_ARRVAL_P(data_to_insert), INDEX_ON_FUNCTION_NAME, function);
	Z_TRY_ADDREF_P(function);
	if(param){
		zend_hash_index_add(Z_ARRVAL_P(data_to_insert), INDEX_ON_FUNCTION_PARAM, param);
		Z_TRY_ADDREF_P(param);
	}
	data = zend_hash_index_find(Z_ARRVAL_P(&ze_obj->provider_ptr->signals), val);
	if(data == NULL){
		narray = ecalloc(1,sizeof(zval));
		array_init(narray);
		zend_hash_index_add(Z_ARRVAL_P(&ze_obj->provider_ptr->signals), val, narray);
		zend_hash_next_index_insert(Z_ARRVAL_P(narray), data_to_insert);
		switch(val){
			case gsignal_gcss_provider_changed :
				g_signal_connect(ze_obj->provider_ptr->intern, GSIGNAL_GCSS_PROVIDER_CHANGED, G_CALLBACK (gcss_provider_func_changed), (gpointer) ze_obj);
				break;
		}
	}else{
		zend_hash_next_index_insert(Z_ARRVAL_P(data), data_to_insert);
	}
}*/

GCSS_PROVIDER_METHOD(loadFromPath){
	ze_gcss_provider_object *ze_obj = NULL;
	zval * self = getThis();
	char * in = NULL;
	size_t in_len = 0;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &in ,&in_len) == FAILURE)
		return;
	ze_obj = Z_GCSS_PROVIDER_P(self);
	gtk_css_provider_load_from_path(ze_obj->provider_ptr->intern, in, NULL);
}

GCSS_PROVIDER_METHOD(loadFromResource){
	ze_gcss_provider_object *ze_obj = NULL;
	zval * self = getThis();
	char * in = NULL;
	size_t in_len = 0;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &in ,&in_len) == FAILURE)
		return;
	ze_obj = Z_GCSS_PROVIDER_P(self);
	gtk_css_provider_load_from_resource(ze_obj->provider_ptr->intern, in);
}

static const zend_function_entry gcss_provider_class_functions[] = {
	//PHP_ME(GCssProvider, on            , arginfo_pggi_on                     , ZEND_ACC_PUBLIC)
	PHP_ME(GCssProvider, __construct     , arginfo_pggi_void                   , ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(GCssProvider, loadFromPath    , arginfo_gcss_provider_load_from_path, ZEND_ACC_PUBLIC)
	PHP_ME(GCssProvider, loadFromResource, arginfo_gcss_provider_load_from_path, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *gcss_provider_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	return std_object_handlers.read_property(object, member, type, cache_slot, rv);
}

HashTable *gcss_provider_get_properties(zval *object){
	//G_H_UPDATE_INIT(zend_std_get_properties(object));
	//return G_H_UPDATE_RETURN;
	return zend_std_get_properties(object);
}

PHP_WRITE_PROP_HANDLER_TYPE gcss_provider_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	PHP_WRITE_PROP_HANDLER_RETURN(std_object_handlers.write_property(object, member, value, cache_slot));
}

/**************************************/
/* GCssProvider Class Initialization */
/**************************************/

#define DECLARE_GCSS_PROVIDER_PROP(name) \
DECLARE_CLASS_PROPERTY(gcss_provider_class_entry_ce, name)

#define GCSS_PROVIDER_CONSTANT(name, value) \
zend_declare_class_constant_long(gcss_provider_class_entry_ce, name, sizeof(name)-1, value);


void gcss_provider_init(int module_number){
	zend_class_entry ce;
	le_gcss_provider = zend_register_list_destructors_ex(gcss_provider_free_resource, NULL, "PGGI\\GCssProvider", module_number);

	memcpy(&gcss_provider_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	gcss_provider_object_handlers.offset         = XtOffsetOf(ze_gcss_provider_object, std);
	gcss_provider_object_handlers.free_obj       = gcss_provider_object_free_storage;
	gcss_provider_object_handlers.clone_obj      = NULL;
	gcss_provider_object_handlers.read_property  = gcss_provider_read_property;
	gcss_provider_object_handlers.get_properties = gcss_provider_get_properties;
	gcss_provider_object_handlers.write_property = gcss_provider_write_property;
	INIT_CLASS_ENTRY(ce, "PGGI\\GCssProvider", gcss_provider_class_functions);
	ce.create_object = gcss_provider_object_new;
	gcss_provider_class_entry_ce = zend_register_internal_class(&ce);

}



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


#include "gscreen.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_gscreen;
static zend_object_handlers gscreen_object_handlers;
static zend_class_entry * gscreen_class_entry_ce;

zend_class_entry * gscreen_get_class_entry(){
	return gscreen_class_entry_ce;
}

zend_object_handlers * gscreen_get_object_handlers(){
	return &gscreen_object_handlers;
}

/****************************/
/* Memory handling function */
/****************************/

gscreen_ptr gscreen_new(){
	gscreen_ptr tor = ecalloc(1, sizeof(gscreen_t));
	array_init(&tor->signals);
	return tor;
}

zend_object *gscreen_object_new(zend_class_entry *class_type){
	ze_gscreen_object *intern;
	intern = ecalloc(1, sizeof(ze_gscreen_object) + zend_object_properties_size(class_type));
	zend_object_std_init	(&intern->std, class_type);
	object_properties_init	(&intern->std, class_type);
	intern->std.handlers = &gscreen_object_handlers;
	return &intern->std;
}

void gscreen_dtor(gscreen_ptr intern){
	zval *  zv, * tmp;
	if (intern->intern){
	}
	zend_hash_destroy(Z_ARRVAL_P(&intern->signals));
	efree(intern);
}

void gscreen_object_free_storage(zend_object *object){
	ze_gscreen_object *intern = php_gscreen_fetch_object(object);
	if (!intern) {
		return;
	}
	if (intern->screen_ptr){
		gscreen_dtor(intern->screen_ptr);
	}
	intern->screen_ptr = NULL;
	zend_object_std_dtor(&intern->std);
}

void gscreen_free_resource(zend_resource *rsrc) {
	gscreen_ptr screen = (gscreen_ptr) rsrc->ptr;
	gscreen_dtor(screen);
}


#define INDEX_ON_FUNCTION_NAME 1
#define INDEX_ON_FUNCTION_PARAM 2

void gscreen_function(gpointer data, unsigned int type){
	zval retval;
	ze_gscreen_object * b = data;
	zval * array, * value, * zv;
	zval args[2];
	if(type){
		zend_object * this = php_gscreen_reverse_object(b);
		ZVAL_OBJ(&args[0], this);
		array = &b->screen_ptr->signals;
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
void gscreen_func_changed(GtkScreen* context, gpointer data){
	gscreen_function(data, gsignal_gscreen_changed);
}*/

/***************/
/* PHP Methods */
/***************/

GSCREEN_METHOD(__construct){}

/*

GSCREEN_METHOD(on){
	zval * function, * data,* narray, * this, * param = NULL;
	long val;
	ze_gscreen_object *ze_obj = NULL;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "lz|z", &val ,&function, &param) == FAILURE)
		return;
	this = getThis();
	ze_obj = Z_GSCREEN_P(this);
	switch(val){
		case gsignal_gscreen_changed :
			break;
		default :
			zend_error(E_ERROR, "Signal unknown");
	}
	zval * data_to_insert = ecalloc(1,sizeof(zval));
	array_init(data_to_insert);
	zend_hash_index_add(Z_ARRVAL_P(data_to_insert), INDEX_ON_FUNCTION_NAME, function);
	zval_addref_p(function);
	if(param){
		zend_hash_index_add(Z_ARRVAL_P(data_to_insert), INDEX_ON_FUNCTION_PARAM, param);
		zval_addref_p(param);
	}
	data = zend_hash_index_find(Z_ARRVAL_P(&ze_obj->screen_ptr->signals), val);
	if(data == NULL){
		narray = ecalloc(1,sizeof(zval));
		array_init(narray);
		zend_hash_index_add(Z_ARRVAL_P(&ze_obj->screen_ptr->signals), val, narray);
		zend_hash_next_index_insert(Z_ARRVAL_P(narray), data_to_insert);
		switch(val){
			case gsignal_gscreen_changed :
				g_signal_connect(ze_obj->screen_ptr->intern, GSIGNAL_GSCREEN_CHANGED, G_CALLBACK (gscreen_func_changed), (gpointer) ze_obj);
				break;
		}
	}else{
		zend_hash_next_index_insert(Z_ARRVAL_P(data), data_to_insert);
	}
}*/

GSCREEN_METHOD(addProvider){
	ze_gscreen_object *ze_obj = NULL;
	ze_gcss_provider_object *obj;
	zval * self = getThis();
	zval * provider;
	long priority;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "Ol", &provider, gcss_provider_get_class_entry(), &priority) == FAILURE)
		return;
	obj = Z_GCSS_PROVIDER_P(provider);
	ze_obj = Z_GSCREEN_P(self);
	gtk_style_context_add_provider_for_screen(ze_obj->screen_ptr->intern, GTK_STYLE_PROVIDER(obj->provider_ptr->intern), priority);
}

static const zend_function_entry gscreen_class_functions[] = {
	//PHP_ME(GScreen, on            , arginfo_pggi_on                     , ZEND_ACC_PUBLIC)
	PHP_ME(GScreen, __construct     , arginfo_pggi_void                   , ZEND_ACC_PRIVATE | ZEND_ACC_CTOR)
	PHP_ME(GScreen, addProvider     , arginfo_gscreen_add_provider        , ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *gscreen_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	return std_object_handlers.read_property(object, member, type, cache_slot, rv);
}

HashTable *gscreen_get_properties(zval *object){
	G_H_UPDATE_INIT(zend_std_get_properties(object));
	return G_H_UPDATE_RETURN;
}

void gscreen_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	switch(Z_TYPE_P(value)){
		default:
			std_object_handlers.write_property(object, member, value, cache_slot);
	}
}

/**************************************/
/* GScreen Class Initialization */
/**************************************/

#define DECLARE_GSCREEN_PROP(name) \
DECLARE_CLASS_PROPERTY(gscreen_class_entry_ce, name)

#define GSCREEN_CONSTANT(name, value) \
zend_declare_class_constant_long(gscreen_class_entry_ce, name, sizeof(name)-1, value);


void gscreen_init(int module_number){
	zend_class_entry ce;
	le_gscreen = zend_register_list_destructors_ex(gscreen_free_resource, NULL, "PGGI\\GScreen", module_number);

	memcpy(&gscreen_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	gscreen_object_handlers.offset         = XtOffsetOf(ze_gscreen_object, std);
	gscreen_object_handlers.free_obj       = gscreen_object_free_storage;
	gscreen_object_handlers.clone_obj      = NULL;
	gscreen_object_handlers.read_property  = gscreen_read_property;
	gscreen_object_handlers.get_properties = gscreen_get_properties;
	gscreen_object_handlers.write_property = gscreen_write_property;
	INIT_CLASS_ENTRY(ce, "PGGI\\GScreen", gscreen_class_functions);
	ce.create_object = gscreen_object_new;
	gscreen_class_entry_ce = zend_register_internal_class(&ce);

}



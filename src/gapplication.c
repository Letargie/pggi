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


#include "gapplication.h"

static int le_gapplication;
static zend_object_handlers gapplication_object_handlers;
static zend_class_entry * gapplication_class_entry_ce;

zend_class_entry * gapplication_get_class_entry(){
	return gapplication_class_entry_ce;
}

zend_object_handlers * gapplication_get_object_handlers(){
	return &gapplication_object_handlers;
}


/*********************************/
/* GApplication memory functions */
/*********************************/

void gapplication_add_windows(gapplication_ptr intern, zval * window){
	zval obj;
	zend_object * this = php_gwidget_reverse_object(Z_GWIDGET_P(window));
	ZVAL_OBJ(&obj, this);
	zend_hash_next_index_insert(Z_ARRVAL_P(&intern->windows), &obj);
	zval_addref_p(&obj);
}

gapplication_ptr gapplication_ctor(){
	gapplication_ptr tor = ecalloc(1, sizeof(gapplication_len));	
	array_init(&tor->windows);
	array_init(&tor->signals);
	return tor;
}

void gapplication_dtor(gapplication_ptr intern){
	unsigned int i;
	zval * zv, * tmp, *val;
	if (intern->app){
		g_object_unref(intern->app);
	}
	zend_hash_destroy(Z_ARRVAL_P(&intern->signals));
	zend_hash_destroy(Z_ARRVAL_P(&intern->windows));
	
	efree(intern);
}

zend_object * gapplication_object_new(zend_class_entry *class_type){
	ze_gapplication_object *intern;
	intern = ecalloc(1, sizeof(ze_gapplication_object) + zend_object_properties_size(class_type));
	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);
	intern->std.handlers = &gapplication_object_handlers;
	return &intern->std;
}

void gapplication_object_free_storage(zend_object *object){
	ze_gapplication_object *intern = php_gapplication_fetch_object(object);
	if (!intern) {
		return;
	}
	if (intern->app_ptr){
		gapplication_dtor(intern->app_ptr);
	}
	intern->app_ptr = NULL;
	zend_object_std_dtor(&intern->std);
}

void gapplication_free_resource(zend_resource *rsrc) {
	gapplication_ptr app = (gapplication_ptr) rsrc->ptr;
	gapplication_dtor(app);
}

/********************************/
/* GApplication signal handling */
/********************************/

#define INDEX_ON_FUNCTION_NAME 1
#define INDEX_ON_FUNCTION_PARAM 2

void gapplication_function(gpointer data, unsigned int type){
	zval retval;
	ze_gapplication_object * app = data;
	zval * array, * value, * zv;
	zval args[2];
	if(type){
		zend_object * this = php_gapplication_reverse_object(app);
		ZVAL_OBJ(&args[0], this);
		array = &app->app_ptr->signals;
		value = (zend_hash_index_find(Z_ARRVAL_P(array), type));
		if(value != NULL){
			ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(value), zv){
				zval * function	= zend_hash_index_find(Z_ARRVAL_P(zv), INDEX_ON_FUNCTION_NAME);
				zval * tmp = zend_hash_index_find(Z_ARRVAL_P(zv), INDEX_ON_FUNCTION_PARAM);
				if(tmp){
					ZVAL_COPY(&args[1], tmp);
				}else
					ZVAL_NULL(&args[1]);
				if(call_user_function(EG(function_table), NULL, function, &retval, 2, args) != SUCCESS){
					zend_error(E_ERROR, "Function call failed");
				}
			} ZEND_HASH_FOREACH_END();
		}
	}
}


void gapplication_func_activate(GtkApplication* app, gpointer data){
	gapplication_function(data, gsignal_gapplication_activate);
}

void gapplication_func_startup(GtkApplication* app, gpointer data){
	gapplication_function(data, gsignal_gapplication_startup);
}

void gapplication_func_shutdown(GtkApplication* app, gpointer data){
	gapplication_function(data, gsignal_gapplication_shutdown);
}

/***************/
/* PHP METHODS */
/***************/

PHP_METHOD(GApplication, on){
	zval * function, * data,* narray, * this, * param = NULL;
	long val;
	ze_gapplication_object *ze_obj = NULL;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "lz|z", &val ,&function, &param) == FAILURE)
		return;
	this = getThis();
	ze_obj = Z_GAPPLICATION_P(this);
	switch(val){
		case gsignal_gapplication_startup  :
		case gsignal_gapplication_shutdown :
		case gsignal_gapplication_activate :
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
	data = zend_hash_index_find(Z_ARRVAL_P(&ze_obj->app_ptr->signals), val);
	if(data == NULL){
		narray = ecalloc(1,sizeof(zval));
		array_init(narray);
		zend_hash_index_add(Z_ARRVAL_P(&ze_obj->app_ptr->signals), val, narray);
		zend_hash_next_index_insert(Z_ARRVAL_P(narray), data_to_insert);
		switch(val){
			case gsignal_gapplication_startup :
				g_signal_connect(ze_obj->app_ptr->app, GSIGNAL_GAPPLICATION_STARTUP, G_CALLBACK (gapplication_func_startup), (gpointer) ze_obj);
				break;
			case gsignal_gapplication_shutdown :
				g_signal_connect(ze_obj->app_ptr->app, GSIGNAL_GAPPLICATION_SHUTDOWN, G_CALLBACK (gapplication_func_shutdown), (gpointer) ze_obj);
				break;
			case gsignal_gapplication_activate :
				g_signal_connect(ze_obj->app_ptr->app, GSIGNAL_GAPPLICATION_ACTIVATE, G_CALLBACK (gapplication_func_activate), (gpointer) ze_obj);
				break;
		}
	}else{
		zend_hash_next_index_insert(Z_ARRVAL_P(data), data_to_insert);
	}
}

PHP_METHOD(GApplication, __construct){
	ze_gapplication_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_GAPPLICATION_P(self);
	ze_obj->app_ptr = gapplication_ctor();
	ze_obj->app_ptr->app = gtk_application_new("pggi.application", G_APPLICATION_FLAGS_NONE);
}

PHP_METHOD(GApplication, run){
	ze_gapplication_object *ze_obj = NULL;
	zval *self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_GAPPLICATION_P(self);
	RETURN_LONG(g_application_run(G_APPLICATION(ze_obj->app_ptr->app), 0, 0));
}

PHP_METHOD(GApplication, quit){
	ze_gapplication_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_GAPPLICATION_P(self);
	g_application_quit(G_APPLICATION(ze_obj->app_ptr->app));
}

PHP_METHOD(GApplication, hold){
	ze_gapplication_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_GAPPLICATION_P(self);
	g_application_hold(G_APPLICATION(ze_obj->app_ptr->app));
}

static const zend_function_entry gapplication_class_functions[] = {
	PHP_ME(GApplication, on				, arginfo_pggi_on	, ZEND_ACC_PUBLIC)
	PHP_ME(GApplication, run			, arginfo_pggi_void	, ZEND_ACC_PUBLIC)
	PHP_ME(GApplication, quit			, arginfo_pggi_void	, ZEND_ACC_PUBLIC)
	PHP_ME(GApplication, __construct	, arginfo_pggi_void	, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_FE_END
};

/******************************/
/* GApplication Initilializer */
/******************************/

void gapplication_init(int module_number){
	zend_class_entry ce;
	le_gapplication = zend_register_list_destructors_ex(gapplication_free_resource, NULL, "PGGI\\GApplication", module_number);

	memcpy(&gapplication_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	gapplication_object_handlers.offset    = XtOffsetOf(ze_gapplication_object, std);
	gapplication_object_handlers.free_obj  = gapplication_object_free_storage;
	gapplication_object_handlers.clone_obj = NULL;
	INIT_CLASS_ENTRY(ce, "PGGI\\GApplication", gapplication_class_functions);
	ce.create_object = gapplication_object_new;
	gapplication_class_entry_ce = zend_register_internal_class(&ce);
}



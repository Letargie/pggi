#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include "gapplication.h"

static int le_gapplication;
static zend_object_handlers gapplication_object_handlers;
static zend_class_entry * gapplication_class_entry_ce;

void gapplication_add_windows(gapplication_ptr intern, zval * window){
	zend_hash_next_index_insert(Z_ARRVAL_P(&intern->windows), window);
	zval_addref_p(window);
}

gapplication_ptr gapplication_ctor(){
	gapplication_ptr tor = ecalloc(1, sizeof(gapplication_len));	
	tor->app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	array_init(&tor->windows);
	return tor;
}

void gapplication_dtor(gapplication_ptr intern){
	unsigned int i;
	if (intern->app){
		g_object_unref(intern->app);
	}
	for(i = 0; i < gapplication_func_type_count; ++i)
		if(zend_is_callable(&intern->functions[i], 0, NULL))
			zval_dtor(&intern->functions[i]);
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

void gapplication_function(gpointer data, unsigned int type){
	zval retval;
	zval * tmp = data;
	if(tmp){
		ze_gapplication_object * app = Z_GAPPLICATION_P(tmp);
		if(call_user_function(EG(function_table), NULL, &app->app_ptr->functions[type], &retval, 1, tmp) != SUCCESS){
			zend_error(E_ERROR, "Function call failed");
		}
	}
}

void gapplication_func_activate(GtkApplication* app, gpointer data){
	gapplication_function(data, gapplication_func_type_activate);
}

void gapplication_func_startup(GtkApplication* app, gpointer data){
	gapplication_function(data, gapplication_func_type_startup);
}

void gapplication_func_shutdown(GtkApplication* app, gpointer data){
	gapplication_function(data, gapplication_func_type_shutdown);
}

void gapplication_set_function(INTERNAL_FUNCTION_PARAMETERS, char * function_name, zval * this, unsigned int type){
	zval * function;
	ze_gapplication_object *ze_obj = NULL;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &function) == FAILURE) {
        RETURN_NULL();
    }
	if(!zend_is_callable(function, 0, NULL))
		zend_error(E_ERROR, "Function requires string argument");

	ze_obj = Z_GAPPLICATION_P(this);
	if(zend_is_callable(&ze_obj->app_ptr->functions[type], 0, NULL))
		zval_dtor(&ze_obj->app_ptr->functions[type]);
	ZVAL_COPY(&ze_obj->app_ptr->functions[type], function);
	switch(type){
		case gapplication_func_type_activate:
			g_signal_connect(ze_obj->app_ptr->app, function_name, G_CALLBACK (gapplication_func_activate), (gpointer) this);
			break;
		case gapplication_func_type_startup:
			g_signal_connect(ze_obj->app_ptr->app, function_name, G_CALLBACK (gapplication_func_startup), (gpointer) this);
			break;
		case gapplication_func_type_shutdown:
			g_signal_connect(ze_obj->app_ptr->app, function_name, G_CALLBACK (gapplication_func_shutdown), (gpointer) this);
			break;
	}
}

PHP_METHOD(GApplication, __construct){
	ze_gapplication_object *ze_obj = NULL;
	zval * self = getThis();
	if(self){
		ze_obj = Z_GAPPLICATION_P(self);
		ze_obj->app_ptr = gapplication_ctor();
	}
}

PHP_METHOD(GApplication, run){
	ze_gapplication_object *ze_obj = NULL;
	zval *self = getThis();
	if(self){
		ze_obj = Z_GAPPLICATION_P(self);
		RETURN_LONG(g_application_run(G_APPLICATION(ze_obj->app_ptr->app), 0, 0));
	}
}


PHP_METHOD(GApplication, onActivate){
	zval * this = getThis();	
	if(!this) zend_error(E_ERROR, "function call on null");
	gapplication_set_function(INTERNAL_FUNCTION_PARAM_PASSTHRU, "activate", this, gapplication_func_type_activate);
}

PHP_METHOD(GApplication, onStartup){
	zval * this = getThis();	
	if(!this) zend_error(E_ERROR, "function call on null");
	gapplication_set_function(INTERNAL_FUNCTION_PARAM_PASSTHRU, "startup", this, gapplication_func_type_startup);
}

PHP_METHOD(GApplication, onShutdown){
	zval * this = getThis();	
	if(!this) zend_error(E_ERROR, "function call on null");
	gapplication_set_function(INTERNAL_FUNCTION_PARAM_PASSTHRU, "shutdown", this, gapplication_func_type_shutdown);
}

PHP_METHOD(GApplication, quit){
	ze_gapplication_object *ze_obj = NULL;
	zval * self = getThis();
	if(self){
		ze_obj = Z_GAPPLICATION_P(self);
		g_application_quit(G_APPLICATION(ze_obj->app_ptr->app));
	}
}

PHP_METHOD(GApplication, hold){
	ze_gapplication_object *ze_obj = NULL;
	zval * self = getThis();
	if(self){
		ze_obj = Z_GAPPLICATION_P(self);
		g_application_hold(G_APPLICATION(ze_obj->app_ptr->app));
	}
}

static const zend_function_entry gapplication_class_functions[] = {
	PHP_ME(GApplication, onActivate		, arginfo_gapplication_on	, ZEND_ACC_PUBLIC)
	PHP_ME(GApplication, onStartup		, arginfo_gapplication_on	, ZEND_ACC_PUBLIC)
	PHP_ME(GApplication, onShutdown		, arginfo_gapplication_on	, ZEND_ACC_PUBLIC)
	PHP_ME(GApplication, run			, arginfo_pggi_void			, ZEND_ACC_PUBLIC)
	PHP_ME(GApplication, quit			, arginfo_pggi_void			, ZEND_ACC_PUBLIC)
	PHP_ME(GApplication, __construct	, arginfo_pggi_void			, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_FE_END
};


void gapplication_init(int module_number){
	zend_class_entry ce;
	le_gapplication = zend_register_list_destructors_ex(gapplication_free_resource, NULL, "gapplication", module_number);

	memcpy(&gapplication_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	gapplication_object_handlers.offset = XtOffsetOf(ze_gapplication_object, std);
	gapplication_object_handlers.free_obj = gapplication_object_free_storage;
	gapplication_object_handlers.clone_obj = NULL;
	INIT_CLASS_ENTRY(ce, "GApplication", gapplication_class_functions);
	ce.create_object = gapplication_object_new;
	gapplication_class_entry_ce = zend_register_internal_class(&ce);
}



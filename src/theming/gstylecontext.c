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


#include "gstylecontext.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_gstyle_context;
static zend_object_handlers gstyle_context_object_handlers;
static zend_class_entry * gstyle_context_class_entry_ce;

zend_class_entry * gstyle_context_get_class_entry(){
	return gstyle_context_class_entry_ce;
}

zend_object_handlers * gstyle_context_get_object_handlers(){
	return &gstyle_context_object_handlers;
}

/****************************/
/* Memory handling function */
/****************************/

gstyle_context_ptr gstyle_context_new(){
	gstyle_context_ptr tor = ecalloc(1, sizeof(gstyle_context_t));
	array_init(&tor->signals);
	return tor;
}

zend_object *gstyle_context_object_new(zend_class_entry *class_type){
	ze_gstyle_context_object *intern;
	intern = ecalloc(1, sizeof(ze_gstyle_context_object) + zend_object_properties_size(class_type));
	zend_object_std_init	(&intern->std, class_type);
	object_properties_init	(&intern->std, class_type);
	intern->std.handlers = &gstyle_context_object_handlers;
	return &intern->std;
}

void gstyle_context_dtor(gstyle_context_ptr intern){
	if (intern->intern  && intern->own){
		g_object_unref(intern->intern);
	}
	zend_hash_destroy(Z_ARRVAL_P(&intern->signals));
	efree(intern);
}

void gstyle_context_object_free_storage(zend_object *object){
	ze_gstyle_context_object *intern = php_gstyle_context_fetch_object(object);
	if (!intern) {
		return;
	}
	if (intern->context_ptr){
		gstyle_context_dtor(intern->context_ptr);
	}
	intern->context_ptr = NULL;
	zend_object_std_dtor(&intern->std);
}

void gstyle_context_free_resource(zend_resource *rsrc) {
	gstyle_context_ptr context = (gstyle_context_ptr) rsrc->ptr;
	gstyle_context_dtor(context);
}


#define INDEX_ON_FUNCTION_NAME 1
#define INDEX_ON_FUNCTION_PARAM 2

void gstyle_context_function(gpointer data, unsigned int type){
	zval retval;
	ze_gstyle_context_object * b = data;
	zval * array, * value, * zv;
	zval args[2];
	if(type){
		zend_object * this = php_gstyle_context_reverse_object(b);
		ZVAL_OBJ(&args[0], this);
		array = &b->context_ptr->signals;
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


void gstyle_context_func_changed(GtkStyleContext* context, gpointer data){
	gstyle_context_function(data, gsignal_gstyle_context_changed);
}

/***************/
/* PHP Methods */
/***************/

GSTYLE_CONTEXT_METHOD(__construct){
	ze_gstyle_context_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none(self) == FAILURE)
		return ;
	ze_obj = Z_GSTYLE_CONTEXT_P(self);
	ze_obj->context_ptr = gstyle_context_new();
	ze_obj->context_ptr->intern = gtk_style_context_new();
	ze_obj->context_ptr->own = 1;
}



GSTYLE_CONTEXT_METHOD(on){
	zval * function, * data,* narray, * this, * param = NULL;
	long val;
	ze_gstyle_context_object *ze_obj = NULL;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "lz|z", &val ,&function, &param) == FAILURE)
		return;
	this = getThis();
	ze_obj = Z_GSTYLE_CONTEXT_P(this);
	switch(val){
		case gsignal_gstyle_context_changed :
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
	data = zend_hash_index_find(Z_ARRVAL_P(&ze_obj->context_ptr->signals), val);
	if(data == NULL){
		narray = ecalloc(1,sizeof(zval));
		array_init(narray);
		zend_hash_index_add(Z_ARRVAL_P(&ze_obj->context_ptr->signals), val, narray);
		zend_hash_next_index_insert(Z_ARRVAL_P(narray), data_to_insert);
		switch(val){
			case gsignal_gstyle_context_changed :
				g_signal_connect(ze_obj->context_ptr->intern, GSIGNAL_GSTYLE_CONTEXT_CHANGED, G_CALLBACK (gstyle_context_func_changed), (gpointer) ze_obj);
				break;
		}
	}else{
		zend_hash_next_index_insert(Z_ARRVAL_P(data), data_to_insert);
	}
}

GSTYLE_CONTEXT_METHOD(addProvider){
	ze_gstyle_context_object *ze_obj = NULL;
	ze_gcss_provider_object *obj;
	zval * self = getThis();
	zval * provider;
	long priority;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "Ol", &provider, gcss_provider_get_class_entry(), &priority) == FAILURE)
		return;
	ze_obj = Z_GSTYLE_CONTEXT_P(self);
	obj = Z_GCSS_PROVIDER_P(provider);
	gtk_style_context_add_provider(ze_obj->context_ptr->intern, GTK_STYLE_PROVIDER(obj->provider_ptr->intern), priority);
}

GSTYLE_CONTEXT_METHOD(renderBackground){
	ze_gstyle_context_object *ze_obj;
	ze_context_object * context;
	zval * self = getThis();
	zval * obj;
	double x, y, width, height;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "Odddd", &obj, pc_context_get_class_entry(), &x, &y, &width, &height) == FAILURE)
		return;
	ze_obj = Z_GSTYLE_CONTEXT_P(self);
	context = Z_CONTEXT_P(obj);
	gtk_render_background(ze_obj->context_ptr->intern, context->context_ptr->intern, x, y, width, height);
}

GSTYLE_CONTEXT_METHOD(getState){
	ze_gstyle_context_object *ze_obj;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_GSTYLE_CONTEXT_P(self);
	RETURN_LONG(gtk_style_context_get_state(ze_obj->context_ptr->intern));
}

GSTYLE_CONTEXT_METHOD(getColor){
	ze_gstyle_context_object *ze_obj;
	zval * self = getThis();
	long flags;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &flags) == FAILURE)
		return;
	ze_obj = Z_GSTYLE_CONTEXT_P(self);
	GdkRGBA color;
	gtk_style_context_get_color(ze_obj->context_ptr->intern, flags, &color);
	zend_object * tor = rgba_object_new(rgba_get_class_entry());
	ze_rgba_object * tmp = php_rgba_fetch_object(tor);
	tmp->ptr = rgba_new();
	tmp->ptr->color = gdk_rgba_copy(&color);
	tmp->ptr->to_destroy = 1;
	RETURN_OBJ(tor);
}

static const zend_function_entry gstyle_context_class_functions[] = {
	PHP_ME(GStyleContext, on              , arginfo_pggi_on                         , ZEND_ACC_PUBLIC)
	PHP_ME(GStyleContext, __construct     , arginfo_pggi_void                       , ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(GStyleContext, addProvider     , arginfo_gstyle_context_add_provider     , ZEND_ACC_PUBLIC)
	PHP_ME(GStyleContext, renderBackground, arginfo_gstyle_context_render_background, ZEND_ACC_PUBLIC)
	PHP_ME(GStyleContext, getState        , arginfo_pggi_get_long                   , ZEND_ACC_PUBLIC)
	PHP_ME(GStyleContext, getColor        , arginfo_gstyle_context_get_color        , ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *gstyle_context_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
		return std_object_handlers.read_property(object, member, type, cache_slot, rv);
}

HashTable *gstyle_context_get_properties(zval *object){
	//G_H_UPDATE_INIT(zend_std_get_properties(object));
	//return G_H_UPDATE_RETURN;
	return zend_std_get_properties(object);
}

PHP_WRITE_PROP_HANDLER_TYPE gstyle_context_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	PHP_WRITE_PROP_HANDLER_RETURN(std_object_handlers.write_property(object, member, value, cache_slot));
}

/**************************************/
/* GStyleContext Class Initialization */
/**************************************/

#define DECLARE_GSTYLE_CONTEXT_PROP(name) \
DECLARE_CLASS_PROPERTY(gstyle_context_class_entry_ce, name)

#define GSTYLE_CONTEXT_CONSTANT(name, value) \
zend_declare_class_constant_long(gstyle_context_class_entry_ce, name, sizeof(name)-1, value);


void gstyle_context_init(int module_number){
	zend_class_entry ce;
	le_gstyle_context = zend_register_list_destructors_ex(gstyle_context_free_resource, NULL, "PGGI\\GStyleContext", module_number);

	memcpy(&gstyle_context_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	gstyle_context_object_handlers.offset         = XtOffsetOf(ze_gstyle_context_object, std);
	gstyle_context_object_handlers.free_obj       = gstyle_context_object_free_storage;
	gstyle_context_object_handlers.clone_obj      = NULL;
	gstyle_context_object_handlers.read_property  = gstyle_context_read_property;
	gstyle_context_object_handlers.get_properties = gstyle_context_get_properties;
	gstyle_context_object_handlers.write_property = gstyle_context_write_property;
	INIT_CLASS_ENTRY(ce, "PGGI\\GStyleContext", gstyle_context_class_functions);
	ce.create_object = gstyle_context_object_new;
	gstyle_context_class_entry_ce = zend_register_internal_class(&ce);

	GSTYLE_CONTEXT_CONSTANT("PRIORITY_FALLBACK"   ,GTK_STYLE_PROVIDER_PRIORITY_FALLBACK   );
	GSTYLE_CONTEXT_CONSTANT("PRIORITY_THEME"      ,GTK_STYLE_PROVIDER_PRIORITY_THEME      );
	GSTYLE_CONTEXT_CONSTANT("PRIORITY_SETTINGS"   ,GTK_STYLE_PROVIDER_PRIORITY_SETTINGS   );
	GSTYLE_CONTEXT_CONSTANT("PRIORITY_APPLICATION",GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	GSTYLE_CONTEXT_CONSTANT("PRIORITY_USER"       ,GTK_STYLE_PROVIDER_PRIORITY_USER       );
}



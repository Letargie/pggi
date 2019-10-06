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


#include "gwindow.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_gdk_gwindow;
static zend_object_handlers gdk_gwindow_object_handlers;
static zend_class_entry * gdk_gwindow_class_entry_ce;

zend_class_entry * gdk_gwindow_get_class_entry(){
	return gdk_gwindow_class_entry_ce;
}

zend_object_handlers * gdk_gwindow_get_object_handlers(){
	return &gdk_gwindow_object_handlers;
}

/****************************/
/* Memory handling function */
/****************************/

gdk_gwindow_ptr gdk_gwindow_new(){
	gdk_gwindow_ptr tor = ecalloc(1, sizeof(gdk_gwindow_t));
	return tor;
}

zend_object *gdk_gwindow_object_new(zend_class_entry *class_type){
	ze_gdk_gwindow_object *intern;
	intern = ecalloc(1, sizeof(ze_gdk_gwindow_object) + zend_object_properties_size(class_type));
	zend_object_std_init	(&intern->std, class_type);
	object_properties_init	(&intern->std, class_type);
	intern->std.handlers = &gdk_gwindow_object_handlers;
	return &intern->std;
}

zend_object * gdk_gwindow_ctor(zend_class_entry *ce, GdkWindow * gwindow){
	zend_object * tor = gdk_gwindow_object_new(ce);
	ze_gdk_gwindow_object * obj = php_gdk_gwindow_fetch_object(tor);
	//obj->std.handlers = &gevent_object_handlers;
	obj->window_ptr = gdk_gwindow_new();
	obj->window_ptr->intern = gwindow;
	return tor;
}

void gdk_gwindow_dtor(gdk_gwindow_ptr intern){
	if (intern->intern){
		// No transfert for now
		//gdk_window_destroy(intern->intern);
	}
	efree(intern);
}

void gdk_gwindow_object_free_storage(zend_object *object){
	ze_gdk_gwindow_object *intern = php_gdk_gwindow_fetch_object(object);
	if (!intern) {
		return;
	}
	if (intern->window_ptr){
		gdk_gwindow_dtor(intern->window_ptr);
	}
	intern->window_ptr = NULL;
	zend_object_std_dtor(&intern->std);
}

void gdk_gwindow_free_resource(zend_resource *rsrc) {
	gdk_gwindow_ptr gwindow = (gdk_gwindow_ptr) rsrc->ptr;
	gdk_gwindow_dtor(gwindow);
}

/***************/
/* PHP Methods */
/***************/

GDK_WINDOW_METHOD(__construct){}

GDK_WINDOW_METHOD(createSimilarSurface){
	ze_gdk_gwindow_object * window_object;
	zval * this = getThis();
	long content, width, height;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "lll", &content, &width, &height) == FAILURE)
		return;
	switch(content){
		case CAIRO_CONTENT_COLOR       :
		case CAIRO_CONTENT_ALPHA       :
		case CAIRO_CONTENT_COLOR_ALPHA :
			break;
		default :
			zend_throw_exception_ex(pggi_exception_get(), 0, "The content needs to be a PGGI\\Cairo::CONTENT_*");
			break;
	}
	window_object = Z_GDK_GWINDOW_P(this);
	cairo_surface_t * new = gdk_window_create_similar_surface(window_object->window_ptr->intern, content, width, height);
	pc_exception(cairo_surface_status(new));
	zval * new_surface;
	ZVAL_OBJ(new_surface, pc_surface_ctor(pc_surface_get_class_entry(), new, 1));  
	RETURN_ZVAL(new_surface, 1, 0);
}

GDK_WINDOW_METHOD(drawFrame){
	ze_gdk_gwindow_object * window_object;
	zval * this = getThis();
	zval * callback, * param;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "z|z", &callback, &param) == FAILURE)
		return;
	window_object = Z_GDK_GWINDOW_P(this);
	GdkWindow * window = window_object->window_ptr->intern;
	cairo_region_t * clip = gdk_window_get_clip_region(window);
	GdkDrawingContext * dc = gdk_window_begin_draw_frame(window, clip);
	cairo_t * context = gdk_drawing_context_get_cairo_context(dc);
	zval args[2];
	zend_object * obj = pc_context_object_new(pc_context_get_class_entry());
	ze_context_object * tmp = php_context_fetch_object(obj);
	tmp->context_ptr = pc_context_new();
	tmp->context_ptr->intern = context;
	ZVAL_OBJ(&args[0], obj);
	if(param){
		ZVAL_COPY(&args[1], param);
	}else
		ZVAL_NULL(&args[1]);
	zval retval;
	if(call_user_function(EG(function_table), NULL, callback, &retval, 2, args) != SUCCESS){
		zend_throw_exception_ex(pggi_exception_get(), 0, "Function call failed");
	}
	gdk_window_end_draw_frame(window, dc);
}

static const zend_function_entry gdk_gwindow_class_functions[] = {
	GDK_WINDOW_ME(__construct         , arginfo_pggi_void                        , ZEND_ACC_PRIVATE | ZEND_ACC_CTOR)
	GDK_WINDOW_ME(createSimilarSurface, arginfo_gdk_window_create_similar_surface, ZEND_ACC_PUBLIC)
	GDK_WINDOW_ME(drawFrame           , arginfo_gdk_window_draw_frame            , ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *gdk_gwindow_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	return std_object_handlers.read_property(object, member, type, cache_slot, rv);
}

HashTable *gdk_gwindow_get_properties(zval *object){
	//G_H_UPDATE_INIT(zend_std_get_properties(object));
	//return G_H_UPDATE_RETURN;
	return zend_std_get_properties(object);
}

PHP_WRITE_PROP_HANDLER_TYPE gdk_gwindow_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	PHP_WRITE_PROP_HANDLER_RETURN(std_object_handlers.write_property(object, member, value, cache_slot));
}

/********************************/
/* GWindow Class Initialization */
/********************************/

#define DECLARE_GWINDOW_PROP(name) \
DECLARE_CLASS_PROPERTY(gdk_gwindow_class_entry_ce, name)

#define GWINDOW_CONSTANT(name, value) \
zend_declare_class_constant_long(gdk_gwindow_class_entry_ce, name, sizeof(name)-1, value);


void gdk_gwindow_init(int module_number){
	zend_class_entry ce;
	le_gdk_gwindow = zend_register_list_destructors_ex(gdk_gwindow_free_resource, NULL, "PGGI\\GDK\\Window", module_number);

	memcpy(&gdk_gwindow_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	gdk_gwindow_object_handlers.offset         = XtOffsetOf(ze_gdk_gwindow_object, std);
	gdk_gwindow_object_handlers.free_obj       = gdk_gwindow_object_free_storage;
	gdk_gwindow_object_handlers.clone_obj      = NULL;
	gdk_gwindow_object_handlers.read_property  = gdk_gwindow_read_property;
	gdk_gwindow_object_handlers.get_properties = gdk_gwindow_get_properties;
	gdk_gwindow_object_handlers.write_property = gdk_gwindow_write_property;
	INIT_CLASS_ENTRY(ce, "PGGI\\GDK\\Window", gdk_gwindow_class_functions);
	ce.create_object = gdk_gwindow_object_new;
	gdk_gwindow_class_entry_ce = zend_register_internal_class(&ce);

}



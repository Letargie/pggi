#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include "gwidget.h"


static int le_gwidget;
static zend_object_handlers gwidget_object_handlers;
static zend_class_entry * gwidget_class_entry_ce;

zend_class_entry * gwidget_get_class_entry(){
	return gwidget_class_entry_ce;
}

void window_destructed(GtkWidget* this, gpointer data){
	ze_gwidget_object * widget = data;
	widget->widget_ptr->intern = NULL;
}

gwidget_ptr gwidget_new(){
	gwidget_ptr tor = ecalloc(1, sizeof(gwidget_t));
	return tor;
}

zend_object *gwidget_object_new(zend_class_entry *class_type){
	ze_gwidget_object *intern;
	intern = ecalloc(1, sizeof(ze_gwidget_object) + zend_object_properties_size(class_type));
	zend_object_std_init	(&intern->std, class_type);
	object_properties_init	(&intern->std, class_type);
	intern->std.handlers = &gwidget_object_handlers;
	return &intern->std;
}

PHP_METHOD(GWidget, __construct){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if(self){
		ze_obj = Z_GWIDGET_P(self);
		ze_obj->widget_ptr = gwidget_new();
		g_signal_connect(ze_obj->widget_ptr->intern, "destroy", G_CALLBACK (window_destructed), ze_obj);	
	}
}


void gwidget_dtor(gwidget_ptr intern){
	if (intern->intern){	
		gtk_widget_destroy(intern->intern);
	}
	efree(intern);
}

void gwidget_object_free_storage(zend_object *object){
	ze_gwidget_object *intern = php_gwidget_fetch_object(object);
	if (!intern) {
		return;
	}
	if (intern->widget_ptr){
		gwidget_dtor(intern->widget_ptr);
	}
	intern->widget_ptr = NULL;
	zend_object_std_dtor(&intern->std);
}

void gwidget_free_resource(zend_resource *rsrc) {
	gwidget_ptr widget = (gwidget_ptr) rsrc->ptr;
	gwidget_dtor(widget);
}

PHP_METHOD(GWidget, show){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if(self){
		ze_obj = Z_GWIDGET_P(self);
		gtk_widget_show(ze_obj->widget_ptr->intern);
	}
}

PHP_METHOD(GWidget, hide){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if(self){
		ze_obj = Z_GWIDGET_P(self);
		gtk_widget_hide(ze_obj->widget_ptr->intern);
	}
}


PHP_METHOD(GWidget, showAll){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if(self){
		ze_obj = Z_GWIDGET_P(self);
		gtk_widget_show_all(ze_obj->widget_ptr->intern);
		/*gtk_main ();*/
	}
}

static const zend_function_entry gwidget_class_functions[] = {
	PHP_ME(GWidget, show		, arginfo_pggi_void	, ZEND_ACC_PUBLIC)
	PHP_ME(GWidget, hide		, arginfo_pggi_void	, ZEND_ACC_PUBLIC)
	PHP_ME(GWidget, showAll		, arginfo_pggi_void	, ZEND_ACC_PUBLIC)
	PHP_ME(GWidget, __construct	, arginfo_pggi_void	, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_FE_END
};

void gwidget_init(int module_number){
	zend_class_entry ce;
	le_gwidget = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "gwidget", module_number);

	memcpy(&gwidget_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	gwidget_object_handlers.offset		= XtOffsetOf(ze_gwidget_object, std);
	gwidget_object_handlers.free_obj	= gwidget_object_free_storage;
	gwidget_object_handlers.clone_obj	= NULL;
	INIT_CLASS_ENTRY(ce, "GWidget", gwidget_class_functions);
	ce.create_object 		= gwidget_object_new;
	gwidget_class_entry_ce 	= zend_register_internal_class(&ce);

}



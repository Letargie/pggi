#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include "gwindow.h"

static int le_gwindow;
static zend_object_handlers gwindow_object_handlers;
static zend_class_entry * gwindow_class_entry_ce;

PHP_METHOD(GWindow, __construct){
	ze_gapplication_object * ze_obj;
	zval * obj;
	ze_gwidget_object * widget;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &obj) == FAILURE) {
        RETURN_NULL();
    }
	ze_obj = Z_GAPPLICATION_P(obj);
	widget = Z_GWIDGET_P(getThis());
	if(!ze_obj || !ze_obj->app_ptr)
		RETURN_NULL();
	widget->widget_ptr = gwidget_new();
	widget->widget_ptr->intern = gtk_application_window_new(ze_obj->app_ptr->app);
	gtk_window_set_title(GTK_WINDOW(widget->widget_ptr->intern),"test");
	gapplication_add_windows(ze_obj->app_ptr, getThis());
	g_signal_connect(widget->widget_ptr->intern, "destroy", G_CALLBACK (window_destructed), widget);
}

PHP_METHOD(GWindow, setTitle){
	ze_gwidget_object *ze_obj = NULL;
	gwidget_ptr a;
	char * name;
	size_t name_len;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &name, &name_len) == FAILURE) {
        RETURN_NULL();
    }
	zval * self = getThis();
	if(self){
		ze_obj = Z_GWIDGET_P(self);
		a = ze_obj->widget_ptr;
		gtk_window_set_title(GTK_WINDOW(ze_obj->widget_ptr->intern), name);
	}
}

PHP_METHOD(GWindow, setDefaultSize){
	ze_gwidget_object *ze_obj = NULL;
	long x, y;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "l|l", &x, &y) == FAILURE) {
        RETURN_NULL();
    }
	zval * self = getThis();
	if(self){
		ze_obj = Z_GWIDGET_P(self);
		gtk_window_set_default_size(GTK_WINDOW(ze_obj->widget_ptr->intern), x, y);
	}
}

PHP_METHOD(GWindow, close){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if(self){
		ze_obj = Z_GWIDGET_P(self);
		gtk_window_close(GTK_WINDOW(ze_obj->widget_ptr->intern));
	}
}

static const zend_function_entry gwindow_class_functions[] = {
	PHP_ME(GWindow, setTitle		, arginfo_gwindow_set_title			, ZEND_ACC_PUBLIC)
	PHP_ME(GWindow, setDefaultSize	, arginfo_gwindow_set_default_size	, ZEND_ACC_PUBLIC)
	PHP_ME(GWindow, close			, arginfo_pggi_void					, ZEND_ACC_PUBLIC)
	PHP_ME(GWindow, __construct		, arginfo_gwidget_object_new		, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_FE_END
};

void gwindow_init(int module_number){
	zend_class_entry ce;
	le_gwindow = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "gwindow", module_number);

	memcpy(&gwindow_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	gwindow_object_handlers.free_obj = gwidget_object_free_storage;
	gwindow_object_handlers.clone_obj = NULL;
	INIT_CLASS_ENTRY(ce, "GWindow", gwindow_class_functions);
	ce.create_object 		= gwidget_object_new;
	gwindow_class_entry_ce	= zend_register_internal_class_ex(&ce, gwidget_get_class_entry());
}



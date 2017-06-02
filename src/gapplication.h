#ifndef __GAPPLICATION_DEF__
#define __GAPPLICATION_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "pggi_parameters.h"

enum{
		gapplication_func_type_startup,
		gapplication_func_type_activate,
		gapplication_func_type_shutdown,
		gapplication_func_type_count
} gapplication_func_types;

typedef struct{
	GtkApplication * app;
	zval functions[gapplication_func_type_count];
	zval windows;
} * gapplication_ptr, gapplication_len;

typedef struct{
	gapplication_ptr app_ptr;
	zend_object std;
} ze_gapplication_object;

static inline ze_gapplication_object *php_gapplication_fetch_object(zend_object *obj) {
	return (ze_gapplication_object *)((char*)(obj) - XtOffsetOf(ze_gapplication_object, std));
}

#define Z_GAPPLICATION_P(zv) php_gapplication_fetch_object(Z_OBJ_P((zv)))

void gapplication_add_windows(gapplication_ptr intern, zval * window);

gapplication_ptr gapplication_ctor();

void gapplication_dtor(gapplication_ptr intern);
zend_object * gapplication_object_new(zend_class_entry *class_type);
void gapplication_object_free_storage(zend_object *object);
void gapplication_free_resource(zend_resource *rsrc);

void gapplication_function(gpointer data, unsigned int type);
void gapplication_func_activate(GtkApplication* app, gpointer data);
void gapplication_func_startup(GtkApplication* app, gpointer data);
void gapplication_func_shutdown(GtkApplication* app, gpointer data);

void gapplication_set_function(INTERNAL_FUNCTION_PARAMETERS, char * function_name, zval * this, unsigned int type);

PHP_METHOD(GApplication, __construct);

PHP_METHOD(GApplication, gapplication_run);

PHP_METHOD(GApplication, onActivate);
PHP_METHOD(GApplication, onStartup);
PHP_METHOD(GApplication, onShutdown);

PHP_METHOD(GApplication, quit);
PHP_METHOD(GApplication, hold);

void gapplication_init(int module_number);


#endif

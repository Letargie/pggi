#ifndef __GWIDGET_DEF__
#define __GWIDGET_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "pggi_parameters.h"

typedef struct{
	GtkWidget * intern;
	zval data;
} * gwidget_ptr, gwidget_t;

typedef struct{
	gwidget_ptr widget_ptr;
	zend_object std;
} ze_gwidget_object;


zend_class_entry * gwidget_get_class_entry(void);

void window_destructed(GtkWidget* this, gpointer data);

gwidget_ptr gwidget_new(void);

static inline ze_gwidget_object *php_gwidget_fetch_object(zend_object *obj) {
	return (ze_gwidget_object *)((char*)(obj) - XtOffsetOf(ze_gwidget_object, std));
}

#define Z_GWIDGET_P(zv) php_gwidget_fetch_object(Z_OBJ_P((zv)))

zend_object *gwidget_object_new(zend_class_entry *class_type);

void gwidget_dtor(gwidget_ptr intern);
void gwidget_object_free_storage(zend_object *object);
void gwidget_free_resource(zend_resource *rsrc);

PHP_METHOD(GWidget, gwidget_object_new	);
PHP_METHOD(GWidget, gwidget_show		);
PHP_METHOD(GWidget, gwidget_hide		);
PHP_METHOD(GWidget, gwidget_show_all	);

void gwidget_init(int module_number);

#endif

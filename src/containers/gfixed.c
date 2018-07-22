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




/**
 * GFixed — A container which allows you to position widgets at fixed coordinates
 *
 */

#include "gfixed.h"

static int le_gfixed;
static zend_object_handlers gfixed_object_handlers;
static zend_class_entry * gfixed_class_entry_ce;

zend_class_entry * gfixed_get_class_entry(){
	return gfixed_class_entry_ce;
}

zend_object_handlers * gfixed_get_object_handlers(){
	return &gfixed_object_handlers;
}

/**
 * Creates a new GFixed.
 */
PHP_METHOD(GFixed, __construct){
	ze_gwidget_object * widget;
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	widget = Z_GWIDGET_P(getThis());
	widget->std.handlers = &gfixed_object_handlers;
	widget->widget_ptr = gwidget_new();
	widget->widget_ptr->intern = gtk_fixed_new();
	GCONTAINER_ADD_ELEMENT(widget);
	g_signal_connect(widget->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), widget);
}

/*
 * Adds a widget to a GFixed container at the given position.
 *
 * @param $object GWidget The added widget
 * @param $x The x position where to place the widget
 * @param $yt The y position where to place the widget
 */
PHP_METHOD(GFixed, put){
	ze_gwidget_object *ze_obj = NULL;
	ze_gwidget_object * data;
	zval * self = getThis();
	zval * obj;
	long x, y;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "Oll", &obj , gwidget_get_class_entry(), &x, &y) == FAILURE)
		return;
	data = Z_GWIDGET_P(obj);
	ze_obj = Z_GWIDGET_P(self);
	gtk_fixed_put(GTK_FIXED(ze_obj->widget_ptr->intern), data->widget_ptr->intern, x, y);
	gcontainer_add_data(ze_obj->widget_ptr, obj);
	//RETURN_ZVAL(self, 0, 0);
}


/*
 * Moves a child of a GFixed container to the given position.
 *
 * @param $object GWidget The added widget
 * @param $x The x position where to place the widget
 * @param $yt The y position where to place the widget
 */
PHP_METHOD(GFixed, move){
	ze_gwidget_object *ze_obj = NULL;
	ze_gwidget_object * data;
	zval * self = getThis();
	zval * obj;
	int x, y;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "Oll", &obj , gwidget_get_class_entry(), &x, &y) == FAILURE)
		return;
	data = Z_GWIDGET_P(obj);
	ze_obj = Z_GWIDGET_P(self);
	gtk_fixed_move(GTK_FIXED(ze_obj->widget_ptr->intern), data->widget_ptr->intern, x, y);
	//RETURN_ZVAL(self, 0, 0);
}



static const zend_function_entry gfixed_class_functions[] = {
	PHP_ME(GFixed, __construct , arginfo_pggi_void                        , ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(GFixed, put         , arginfo_gfixed_manipulate_widget_position, ZEND_ACC_PUBLIC)
	PHP_ME(GFixed, move        , arginfo_gfixed_manipulate_widget_position, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

void gfixed_init(int module_number){
	zend_class_entry ce;
	le_gfixed = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "PGGI\\GFixed", module_number);

	memcpy(&gfixed_object_handlers, gcontainer_get_object_handlers(), sizeof(zend_object_handlers));
	INIT_CLASS_ENTRY(ce, "PGGI\\GFixed", gfixed_class_functions);
	ce.create_object = gwidget_object_new;
	gfixed_class_entry_ce = zend_register_internal_class_ex(&ce, gcontainer_get_class_entry());
}



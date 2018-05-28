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


#include "gbuttonbox.h"

static int le_gbuttonbox;
static zend_object_handlers gbuttonbox_object_handlers;
static zend_class_entry * gbuttonbox_class_entry_ce;

PHP_METHOD(GButtonBox, __construct){
	long orientation;
	ze_gwidget_object * widget;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &orientation) == FAILURE)
		return;
	widget = Z_GWIDGET_P(getThis());
	widget->std.handlers = &gbuttonbox_object_handlers;
	switch(orientation){
		case GTK_ORIENTATION_HORIZONTAL:
		case GTK_ORIENTATION_VERTICAL:
			widget->widget_ptr = gwidget_new();
			widget->widget_ptr->intern = gtk_button_box_new(orientation);
			break;
		default :
			zend_throw_exception_ex(pggi_exception_get(), 0, " the orientation needs to be an ORIENTATION_*");
			return;
	}
	GCONTAINER_ADD_ELEMENT(widget);
	g_signal_connect(widget->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), widget);
}

PHP_METHOD(GButtonBox, getLayout){
	ze_gwidget_object * widget = Z_GWIDGET_P(getThis());
	if(!widget)
		RETURN_NULL();
	gtk_button_box_get_layout(GTK_BUTTON_BOX(widget->widget_ptr->intern));
}

PHP_METHOD(GButtonBox, setLayout){
	long layout;
	ze_gwidget_object * widget;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "l", &layout) == FAILURE)
		return;
	widget = Z_GWIDGET_P(getThis());
	switch(layout){
		case GTK_BUTTONBOX_SPREAD :
		case GTK_BUTTONBOX_EDGE   :
		case GTK_BUTTONBOX_START  :
		case GTK_BUTTONBOX_END    :
		case GTK_BUTTONBOX_CENTER :
		case GTK_BUTTONBOX_EXPAND :
			gtk_button_box_set_layout (GTK_BUTTON_BOX(widget->widget_ptr->intern), layout);
			break;
		default:
			zend_throw_exception_ex(pggi_exception_get(), 0, " the layout needs to be a LAYOUT_*");
	}
}

#define GBUTTON_BOX_CONSTANT(name, value) \
zend_declare_class_constant_long(gbuttonbox_class_entry_ce, name, sizeof(name)-1, value);

static const zend_function_entry gbuttonbox_class_functions[] = {
	PHP_ME(GButtonBox, __construct, arginfo_gbuttonbox_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(GButtonBox, getLayout  , arginfo_pggi_get_long       , ZEND_ACC_PUBLIC)
	PHP_ME(GButtonBox, setLayout  , arginfo_pggi_set_long       , ZEND_ACC_PUBLIC)
	PHP_FE_END
};

void gbuttonbox_init(int module_number){
	zend_class_entry ce;
	le_gbuttonbox = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "PGGI\\GButtonBox", module_number);

	memcpy(&gbuttonbox_object_handlers, gbox_get_object_handlers(), sizeof(zend_object_handlers));
	INIT_CLASS_ENTRY(ce, "PGGI\\GButtonBox", gbuttonbox_class_functions);
	gbuttonbox_class_entry_ce	= zend_register_internal_class_ex(&ce, gbox_get_class_entry());

	GBUTTON_BOX_CONSTANT("LAYOUT_SPREAD", GTK_BUTTONBOX_SPREAD);
	GBUTTON_BOX_CONSTANT("LAYOUT_EDGE"  , GTK_BUTTONBOX_EDGE  );
	GBUTTON_BOX_CONSTANT("LAYOUT_START" , GTK_BUTTONBOX_START );
	GBUTTON_BOX_CONSTANT("LAYOUT_END"   , GTK_BUTTONBOX_END   );
	GBUTTON_BOX_CONSTANT("LAYOUT_CENTER", GTK_BUTTONBOX_CENTER);
	GBUTTON_BOX_CONSTANT("LAYOUT_EXPAND", GTK_BUTTONBOX_EXPAND);

}



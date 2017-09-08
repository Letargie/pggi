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
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &orientation) == FAILURE) {
        RETURN_NULL();
    }
	widget = Z_GWIDGET_P(getThis());
	widget->std.handlers = &gbuttonbox_object_handlers;
	if(!widget)
		RETURN_NULL();
	switch(orientation){
		case GTK_ORIENTATION_HORIZONTAL:
		case GTK_ORIENTATION_VERTICAL:
			widget->widget_ptr = gwidget_new();
			widget->widget_ptr->intern = gtk_button_box_new(orientation);
			break;
		default :
			RETURN_NULL();
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
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &layout) == FAILURE) {
        RETURN_NULL();
    }
	widget = Z_GWIDGET_P(getThis());
	if(widget)
		switch(layout){
			case GTK_BUTTONBOX_SPREAD :
			case GTK_BUTTONBOX_EDGE :
			case GTK_BUTTONBOX_START :
			case GTK_BUTTONBOX_END :
			case GTK_BUTTONBOX_CENTER :
			case GTK_BUTTONBOX_EXPAND :
				gtk_button_box_set_layout (GTK_BUTTON_BOX(widget->widget_ptr->intern), layout);
				break;
		}
}

#define DECLARE_CLASS_CONSTANT_LONG(name, value) \
		zend_declare_class_constant_long(ce, name, sizeof(name) - 1, value);

static const zend_function_entry gbuttonbox_class_functions[] = {
	PHP_ME(GButtonBox, __construct	, arginfo_gbuttonbox_construct	, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR	)
	PHP_ME(GButtonBox, getLayout	, arginfo_pggi_void				, ZEND_ACC_PUBLIC					)
	PHP_ME(GButtonBox, setLayout	, arginfo_gbuttonbox_set_layout	, ZEND_ACC_PUBLIC					)
	PHP_FE_END
};

void gbuttonbox_init(int module_number){
	zend_class_entry ce;
	le_gbuttonbox = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "gbuttonbox", module_number);

	memcpy(&gbuttonbox_object_handlers, gbox_get_object_handlers(), sizeof(zend_object_handlers));
	INIT_CLASS_ENTRY(ce, "GButtonBox", gbuttonbox_class_functions);

	DECLARE_CLASS_CONSTANT_LONG("PGGI_BUTTONBOX_SPREAD"	, GTK_BUTTONBOX_SPREAD	);
	DECLARE_CLASS_CONSTANT_LONG("PGGI_BUTTONBOX_EDGE"	, GTK_BUTTONBOX_EDGE	);
	DECLARE_CLASS_CONSTANT_LONG("PGGI_BUTTONBOX_START"	, GTK_BUTTONBOX_START	);
	DECLARE_CLASS_CONSTANT_LONG("PGGI_BUTTONBOX_END"	, GTK_BUTTONBOX_END		);
	DECLARE_CLASS_CONSTANT_LONG("PGGI_BUTTONBOX_CENTER"	, GTK_BUTTONBOX_CENTER	);
	DECLARE_CLASS_CONSTANT_LONG("PGGI_BUTTONBOX_EXPAND"	, GTK_BUTTONBOX_EXPAND	);

	gbuttonbox_class_entry_ce	= zend_register_internal_class_ex(&ce, gbox_get_class_entry());
}



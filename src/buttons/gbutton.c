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


#include "gbutton.h"

static int le_gbutton;
static zend_object_handlers gbutton_object_handlers;
static zend_class_entry * gbutton_class_entry_ce;

enum {
	gbutton_with_label,
	gbutton_with_mnemonic,
	gbutton_with_image_16,
	gbutton_with_image_24,
	gbutton_with_image_32,
	gbutton_with_image_48
} gbutton_types;

PHP_METHOD(GButton, __construct){
	zend_string * label = NULL;
	ze_gwidget_object * widget;
	long type = gbutton_with_label;
	widget = Z_GWIDGET_P(getThis());
	widget->std.handlers = &gbutton_object_handlers;
	if(!widget)
		RETURN_NULL();
	widget->widget_ptr = gwidget_new();
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "|Sl", &label, &type) == FAILURE)
		RETURN_NULL();
	if(label){
		switch(type){
			case gbutton_with_label :
				widget->widget_ptr->intern = gtk_button_new_with_label(ZSTR_VAL(label));
				break;
			case gbutton_with_mnemonic :
				widget->widget_ptr->intern = gtk_button_new_with_mnemonic(ZSTR_VAL(label));
				break;
			case gbutton_with_image_16 :
			case gbutton_with_image_24 :
			case gbutton_with_image_32 :
			case gbutton_with_image_48 :
				widget->widget_ptr->intern = gtk_button_new_from_icon_name(ZSTR_VAL(label), type);
				break;
			default :
				zend_throw_exception_ex(pggi_exception_get(), 0, "the type of the button is invalid, needs to be a GButton:: Constant");
		}
	}else
		widget->widget_ptr->intern = gtk_button_new();
	GCONTAINER_ADD_ELEMENT(widget);
	g_signal_connect(widget->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), widget);
}


void gbutton_func_clicked(GtkWidget * button, gpointer data){
	zval args[2];
	gwidget_function(data, gsignal_gbutton_clicked, args, 2);
}

void gbutton_on(long val,ze_gwidget_object * ze_obj, zval * function, zval * param){
	switch(val){
		case gsignal_gbutton_clicked :
			gwidget_adding_function(val, GSIGNAL_GBUTTON_CLICKED, G_CALLBACK(gbutton_func_clicked), ze_obj, function, param);
			break;
		default :
			gcontainer_on(val, ze_obj, function, param);
	}
}

PHP_METHOD(GButton, on){
	zval * function, * this, *param = NULL;
	long val;
	ze_gwidget_object *ze_obj = NULL;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "lz|z", &val ,&function, &param) == FAILURE)
		return;
	this = getThis();
	ze_obj = Z_GWIDGET_P(this);
	gbutton_on(val, ze_obj, function, param);
}

static const zend_function_entry gbutton_class_functions[] = {
	PHP_ME(GButton, __construct, arginfo_pggi_void, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(GButton, on         , arginfo_pggi_on  , ZEND_ACC_PUBLIC)
	PHP_FE_END
};

zval *gbutton_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	char * member_val = Z_STRVAL_P(member);
	GtkButton * but = GTK_BUTTON(w->intern);
	convert_to_string(member);

	if(!strcmp(member_val, GBUTTON_LABEL)){
		const char * tmp = gtk_button_get_label(but);
		ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
	}else if(!strcmp(member_val, GBUTTON_IMAGE_POSITION)){
		ZVAL_LONG(rv, gtk_button_get_image_position(but));
	}else if(!strcmp(member_val, GBUTTON_RELIEF)){
		ZVAL_LONG(rv, gtk_button_get_relief(but));
	}else if(!strcmp(member_val, GBUTTON_ALWAYS_SHOW_IMAGE)){
		ZVAL_LONG(rv, gtk_button_get_always_show_image(but));
	}else if(!strcmp(member_val, GBUTTON_USE_UNDERLINE)){
		ZVAL_LONG(rv, gtk_button_get_use_underline(but));
	}else if(!strcmp(member_val, GBUTTON_IMAGE)){
		return std_object_handlers.read_property(object, member, type, cache_slot, rv);
	}else
		return gcontainer_read_property(object, member, type, cache_slot, rv);
	return rv;
}

HashTable *gbutton_get_properties(zval *object){
	G_H_UPDATE_INIT(gcontainer_get_properties(object));
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	GtkButton * but = GTK_BUTTON(w->intern);
	G_H_UPDATE_STRING(GBUTTON_LABEL            , gtk_button_get_label            (but));
	G_H_UPDATE_BOOL  (GBUTTON_ALWAYS_SHOW_IMAGE, gtk_button_get_always_show_image(but));
	G_H_UPDATE_BOOL  (GBUTTON_USE_UNDERLINE    , gtk_button_get_use_underline    (but));
	G_H_UPDATE_LONG  (GBUTTON_IMAGE_POSITION   , gtk_button_get_image_position   (but));
	G_H_UPDATE_LONG  (GBUTTON_RELIEF           , gtk_button_get_relief           (but));
	return G_H_UPDATE_RETURN;
}

PHP_WRITE_PROP_HANDLER_TYPE gbutton_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	ze_gwidget_object * tmp_widget;
	gwidget_ptr w = intern->widget_ptr;
	long tmp_l;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkButton * but = GTK_BUTTON(w->intern);
	if(!strcmp(Z_STRVAL_P(member), GBUTTON_LABEL)){
		convert_to_string(value);
		gtk_button_set_label(but, Z_STRVAL_P(value));
	}else if(!strcmp(Z_STRVAL_P(member), GBUTTON_IMAGE_POSITION)){
		convert_to_long(value);
		tmp_l = Z_LVAL_P(value);
		switch(tmp_l){
			case GTK_POS_LEFT  :
			case GTK_POS_RIGHT :
			case GTK_POS_TOP   :
			case GTK_POS_BOTTOM:
				gtk_button_set_image_position(but, tmp_l);
				break;
			default :
				zend_throw_exception_ex(pggi_exception_get(), 0, "Can't change the image position property. New value should be a POS_*");
		}
	}else if(!strcmp(Z_STRVAL_P(member), GBUTTON_RELIEF)){
		convert_to_long(value);
		tmp_l = Z_LVAL_P(value);
		switch(tmp_l){
			case GTK_RELIEF_NORMAL:
			case GTK_RELIEF_NONE  :
				gtk_button_set_relief(but, tmp_l);
				break;
			default :
				zend_throw_exception_ex(pggi_exception_get(), 0, "Can't change the relief property. New value should be a RELIEF_*");
		}
	}else if(!strcmp(member_val, GBUTTON_ALWAYS_SHOW_IMAGE)){
		convert_to_boolean(value);
		gtk_button_set_always_show_image(but, GET_BOOL_FROM_ZVAL(value));
	}else if(!strcmp(member_val, GBUTTON_USE_UNDERLINE)){
		convert_to_boolean(value);
		gtk_button_set_use_underline(but, GET_BOOL_FROM_ZVAL(value));
	}else if(!strcmp(member_val, GBUTTON_IMAGE)){
		tmp_widget = Z_GWIDGET_P(value);
		if(!tmp_widget){
			zend_throw_exception_ex(pggi_exception_get(), 0, "the image need to be an image");
			return ;
		}
		w = tmp_widget->widget_ptr;
		std_object_handlers.write_property(object, member, value, cache_slot);
		gtk_button_set_image(but, w->intern);
	} else {
		PHP_WRITE_PROP_HANDLER_RETURN(gcontainer_write_property(object, member, value, cache_slot));
	}
	PHP_WRITE_PROP_HANDLER_RETURN(value);
}


#define DECLARE_GBUTTON_PROP(name) \
DECLARE_CLASS_PROPERTY(gbutton_class_entry_ce, name)

#define GBUTTON_CONSTANT(name, value) \
zend_declare_class_constant_long(gbutton_class_entry_ce, name, sizeof(name)-1, value);

void gbutton_init(int module_number){
	zend_class_entry ce;
	le_gbutton = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "PGGI\\GButton", module_number);

	memcpy(&gbutton_object_handlers, gcontainer_get_object_handlers(), sizeof(zend_object_handlers));
	gbutton_object_handlers.free_obj = gwidget_object_free_storage;
	gbutton_object_handlers.clone_obj = NULL;
	INIT_CLASS_ENTRY(ce, "PGGI\\GButton", gbutton_class_functions);
	gbutton_object_handlers.read_property  = gbutton_read_property;
	gbutton_object_handlers.get_properties = gbutton_get_properties;
	gbutton_object_handlers.write_property = gbutton_write_property;
	gbutton_class_entry_ce	= zend_register_internal_class_ex(&ce, gcontainer_get_class_entry());

	GBUTTON_CONSTANT("LABEL"   , gbutton_with_label   );
	GBUTTON_CONSTANT("MNEMONIC", gbutton_with_mnemonic);
	GBUTTON_CONSTANT("IMAGE_16", gbutton_with_image_16);
	GBUTTON_CONSTANT("IMAGE_24", gbutton_with_image_24);
	GBUTTON_CONSTANT("IMAGE_32", gbutton_with_image_32);
	GBUTTON_CONSTANT("IMAGE_48", gbutton_with_image_48);


	DECLARE_GBUTTON_PROP(GBUTTON_ALWAYS_SHOW_IMAGE);
	DECLARE_GBUTTON_PROP(GBUTTON_IMAGE            );
	DECLARE_GBUTTON_PROP(GBUTTON_IMAGE_POSITION   );
	DECLARE_GBUTTON_PROP(GBUTTON_LABEL            );
	DECLARE_GBUTTON_PROP(GBUTTON_RELIEF           );
	DECLARE_GBUTTON_PROP(GBUTTON_USE_UNDERLINE    );
}



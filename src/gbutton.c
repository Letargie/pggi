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
				RETURN_NULL();
		}
	}else
		widget->widget_ptr->intern = gtk_button_new();
	g_signal_connect(widget->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), widget);
}

static const zend_function_entry gbutton_class_functions[] = {
	PHP_ME(GButton, __construct	, arginfo_pggi_void		, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_FE_END
};

zval *gbutton_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	zval zobj;
	zend_long lval;
	char * member_val = Z_STRVAL_P(member);
	ZVAL_NULL(rv);
	if(w){
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
	}
	return rv;
}

HashTable *gbutton_get_properties(zval *object){
	HashTable *props;
	zval zv;
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	if(!w){
		return NULL;
	}
	GtkButton * but = GTK_BUTTON(w->intern);
	props = gcontainer_get_properties(object);
	G_H_UPDATE_STRING	(GBUTTON_LABEL				, gtk_button_get_label				(but));
	G_H_UPDATE_BOOL		(GBUTTON_ALWAYS_SHOW_IMAGE	, gtk_button_get_always_show_image	(but));
	G_H_UPDATE_BOOL		(GBUTTON_USE_UNDERLINE		, gtk_button_get_use_underline		(but));
	G_H_UPDATE_LONG		(GBUTTON_IMAGE_POSITION		, gtk_button_get_image_position		(but));
	G_H_UPDATE_LONG		(GBUTTON_RELIEF				, gtk_button_get_relief				(but));

	return props;
}

void gbutton_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	ze_gwidget_object * tmp_widget;
	gwidget_ptr w = intern->widget_ptr;
	zval * tmp_member;
	long tmp_l;
	int tmp_b;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkButton * but = GTK_BUTTON(w->intern);
	switch(Z_TYPE_P(value)){
		case IS_STRING :
			if(!strcmp(Z_STRVAL_P(member), GBUTTON_LABEL))
				gtk_button_set_label(but, Z_STRVAL_P(value));
			else
				gcontainer_write_property(object, member, value, cache_slot);
			break;
		case IS_LONG :
			tmp_l = Z_LVAL_P(value);
			if(!strcmp(Z_STRVAL_P(member), GBUTTON_IMAGE_POSITION))
				gtk_button_set_image_position(but, tmp_l);
				/*switch(tmp_l){
				TODO
				}*/
			else if(!strcmp(Z_STRVAL_P(member), GBUTTON_RELIEF))
				gtk_button_set_relief(but, tmp_l);
				/* TODO
				*/
			else
				gcontainer_write_property(object, member, value, cache_slot);
			break;
		case IS_TRUE :
		case IS_FALSE :
			tmp_b = Z_LVAL_P(value);
			if(!strcmp(member_val, GBUTTON_ALWAYS_SHOW_IMAGE))
				gtk_button_set_always_show_image(but, tmp_b);
			else if(!strcmp(member_val, GBUTTON_USE_UNDERLINE))
				gtk_button_set_use_underline(but, tmp_b);
			else
				gcontainer_write_property(object, member, value, cache_slot);
			break;
		case IS_OBJECT :
				if(!strcmp(member_val, GBUTTON_IMAGE)){
					tmp_widget = Z_GWIDGET_P(value);
					if(!tmp_widget){
						/* error */
						return ;
					}
					w = tmp_widget->widget_ptr;
					if(!w){
						/*error*/
						return;
					}
					std_object_handlers.write_property(object, member, value, cache_slot);
					gtk_button_set_image(but, w->intern);
				}else
					gcontainer_write_property(object, member, value, cache_slot);
			break;
		default :
			gcontainer_write_property(object, member, value, cache_slot);
	}
}


#define DECLARE_GBUTTON_PROP(name) \
DECLARE_CLASS_PROPERTY(gbutton_class_entry_ce, name)

#define GBUTTON_CONSTANT(name, value) \
zend_declare_class_constant_long(gbutton_class_entry_ce, name, sizeof(name)-1, value);

void gbutton_init(int module_number){
	zend_class_entry ce;
	le_gbutton = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "gbutton", module_number);

	memcpy(&gbutton_object_handlers, gcontainer_get_object_handlers(), sizeof(zend_object_handlers));
	gbutton_object_handlers.free_obj = gwidget_object_free_storage;
	gbutton_object_handlers.clone_obj = NULL;
	INIT_CLASS_ENTRY(ce, "GButton", gbutton_class_functions);
	gbutton_object_handlers.read_property  = gbutton_read_property;
	gbutton_object_handlers.get_properties = gbutton_get_properties;
	gbutton_object_handlers.write_property = gbutton_write_property;
	gbutton_class_entry_ce	= zend_register_internal_class_ex(&ce, gcontainer_get_class_entry());

	GBUTTON_CONSTANT("LABEL"	, gbutton_with_label	);
	GBUTTON_CONSTANT("MNEMONIC"	, gbutton_with_mnemonic	);
	GBUTTON_CONSTANT("IMAGE_16"	, gbutton_with_image_16	);
	GBUTTON_CONSTANT("IMAGE_24"	, gbutton_with_image_24	);
	GBUTTON_CONSTANT("IMAGE_32"	, gbutton_with_image_32	);
	GBUTTON_CONSTANT("IMAGE_48"	, gbutton_with_image_48	);


	DECLARE_GBUTTON_PROP(GBUTTON_ALWAYS_SHOW_IMAGE	);
	DECLARE_GBUTTON_PROP(GBUTTON_IMAGE				);
	DECLARE_GBUTTON_PROP(GBUTTON_IMAGE_POSITION		);
	DECLARE_GBUTTON_PROP(GBUTTON_LABEL				);
	DECLARE_GBUTTON_PROP(GBUTTON_RELIEF				);
	DECLARE_GBUTTON_PROP(GBUTTON_USE_UNDERLINE		);
}



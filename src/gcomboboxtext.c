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


#include "gcomboboxtext.h"

static int le_gcombo_box_text;
static zend_object_handlers gcombo_box_text_object_handlers;
static zend_class_entry * gcombo_box_text_class_entry_ce;

/***************/
/* PHP Methods */
/***************/

GCOMBO_BOX_TEXT_METHOD(__construct){
	ze_gwidget_object * ze_obj;
	zval * self = getThis();
	if(!self)
		RETURN_NULL();
	ze_obj = Z_GWIDGET_P(getThis());
	ze_obj->std.handlers = &gcombo_box_text_object_handlers;
	ze_obj->widget_ptr = gwidget_new();
	ze_obj->widget_ptr->intern = gtk_combo_box_text_new();
	g_signal_connect(ze_obj->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), Z_GWIDGET_P(self));
}

GCOMBO_BOX_TEXT_METHOD(append){
	ze_gwidget_object * ze_obj;
	zend_string * text;
	zend_string * id = NULL;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "S|S", &text, &id) == FAILURE) {
        RETURN_NULL();
    }
	ze_obj = Z_GWIDGET_P(getThis());
	char * ctext = ZSTR_VAL(text);
	if(!id)
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(ze_obj->widget_ptr->intern), ctext);
	else{
		char * cid = ZSTR_VAL(id);
		gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(ze_obj->widget_ptr->intern), ctext, cid);
	}
}

GCOMBO_BOX_TEXT_METHOD(prepend){
	ze_gwidget_object * ze_obj;
	zend_string * text;
	zend_string * id = NULL;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "S|S", &text, &id) == FAILURE) {
        RETURN_NULL();
    }
	ze_obj = Z_GWIDGET_P(getThis());
	char * ctext = ZSTR_VAL(text);
	if(!id)
		gtk_combo_box_text_prepend_text(GTK_COMBO_BOX_TEXT(ze_obj->widget_ptr->intern), ctext);
	else{
		char * cid = ZSTR_VAL(id);
		gtk_combo_box_text_prepend(GTK_COMBO_BOX_TEXT(ze_obj->widget_ptr->intern), ctext, cid);
	}
}

GCOMBO_BOX_TEXT_METHOD(insert){
	ze_gwidget_object * ze_obj;
	zend_string * text;
	zend_string * id = NULL;
	long val;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "lS|S", &val, &text, &id) == FAILURE) {
        RETURN_NULL();
    }
	ze_obj = Z_GWIDGET_P(getThis());
	char * ctext = ZSTR_VAL(text);
	if(!id)
		gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(ze_obj->widget_ptr->intern), val, ctext);
	else{
		char * cid = ZSTR_VAL(id);
		gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(ze_obj->widget_ptr->intern), val, ctext, cid);
	}
}

GCOMBO_BOX_TEXT_METHOD(remove){
	ze_gwidget_object * ze_obj;
	long val;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &val) == FAILURE) {
        RETURN_NULL();
    }
	ze_obj = Z_GWIDGET_P(getThis());
	gtk_combo_box_text_remove(GTK_COMBO_BOX_TEXT(ze_obj->widget_ptr->intern), val);
}

GCOMBO_BOX_TEXT_METHOD(removeAll){
	ze_gwidget_object * ze_obj;
	ze_obj = Z_GWIDGET_P(getThis());
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(ze_obj->widget_ptr->intern));
}

GCOMBO_BOX_TEXT_METHOD(getActiveText){
	ze_gwidget_object * ze_obj;
	ze_obj = Z_GWIDGET_P(getThis());
	char * string = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(ze_obj->widget_ptr->intern));
	if(string){
		RETURN_STRING(string);
	}else{
		RETURN_NULL();
	}
}

/**
 * List of GComboBoxText functions and methods with their arguments
 */
static const zend_function_entry gcombo_box_text_class_functions[] = {
	PHP_ME(GComboBoxText, __construct	, arginfo_pggi_void					, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(GComboBoxText, insert		, arginfo_gcombo_box_text_insert	, ZEND_ACC_PUBLIC)
	PHP_ME(GComboBoxText, append		, arginfo_gcombo_box_text_append	, ZEND_ACC_PUBLIC)
	PHP_ME(GComboBoxText, prepend		, arginfo_gcombo_box_text_prepend	, ZEND_ACC_PUBLIC)
	PHP_ME(GComboBoxText, getActiveText	, arginfo_pggi_void					, ZEND_ACC_PUBLIC)
	PHP_ME(GComboBoxText, removeAll		, arginfo_pggi_void					, ZEND_ACC_PUBLIC)
	PHP_ME(GComboBoxText, remove		, arginfo_gcombo_box_text_remove	, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**************************************/
/* GComboBoxText class initialisation */
/**************************************/

void gcombo_box_text_init(int module_number){
	DECLARE_CLASS_PROPERTY_INIT();
	le_gcombo_box_text = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "gcombo_box_text", module_number);
	memcpy(&gcombo_box_text_object_handlers, gcombo_box_get_object_handlers(), sizeof(zend_object_handlers));
	INIT_CLASS_ENTRY(ce, "GComboBoxText", gcombo_box_text_class_functions);
	gcombo_box_text_class_entry_ce	= zend_register_internal_class_ex(&ce, gcombo_box_get_class_entry());
}



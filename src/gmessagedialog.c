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


#include "gmessagedialog.h"

static int le_gmessage_dialog;
static zend_object_handlers gmessage_dialog_object_handlers;
static zend_class_entry * gmessage_dialog_class_entry_ce;

/***************/
/* PHP Methods */
/***************/

GMESSAGE_DIALOG_METHOD(__construct){
	ze_gwidget_object * widget = Z_GWIDGET_P(getThis());
	ze_gwidget_object * ze_parent;
	long flag, message_type, button_type;
	char * primary_message, * secondary_message = NULL;
	size_t primary_len, secondary_len = 0;
	zval * parent;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "Ollls|s", &parent, gwindow_get_class_entry(),
	                                                            &flag, &message_type, &button_type,
	                                                            &primary_message  , &primary_len,
	                                                            &secondary_message, &secondary_len) == FAILURE)
		return;

	switch(flag){
		case GDIALOG_FLAG_MODAL               :
		case GDIALOG_FLAG_DESTROY_WITH_PARENT :
		case GDIALOG_FLAG_USE_HEADER_BAR      :
			break;
		default :
			zend_throw_exception_ex(pggi_exception_get(), 0, "the flag must be a GDialog::FLAG_*");
			return;
	}

	switch(message_type){
		case GTK_MESSAGE_INFO     :
		case GTK_MESSAGE_WARNING  :
		case GTK_MESSAGE_QUESTION :
		case GTK_MESSAGE_ERROR    :
		case GTK_MESSAGE_OTHER    :
			break;
		default :
			zend_throw_exception_ex(pggi_exception_get(), 0, "the message type must be a GMessageDialog::MESSAGE_*");
			return;
	}

	switch(button_type){
		case GTK_BUTTONS_NONE      :
		case GTK_BUTTONS_OK        :
		case GTK_BUTTONS_CLOSE     :
		case GTK_BUTTONS_CANCEL    :
		case GTK_BUTTONS_YES_NO    :
		case GTK_BUTTONS_OK_CANCEL :
			break;
		default :
			zend_throw_exception_ex(pggi_exception_get(), 0, "the buttons type must be a GMessageDialog::BUTTONS_*");
			return;
	}
	GtkWindow * p = NULL;
	if(Z_TYPE_P(parent) == IS_NULL){
		ze_parent = Z_GWIDGET_P(parent);
		p = GTK_WINDOW(ze_parent->widget_ptr->intern);
	}
	widget->std.handlers = &gmessage_dialog_object_handlers;
	widget->widget_ptr = gwidget_new();
	widget->widget_ptr->intern = gtk_message_dialog_new(p, flag, message_type, button_type, "%s", (primary_len > 0 ? primary_message : ""));
	if(secondary_len > 0)
		gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(widget->widget_ptr->intern) , "%s", secondary_message);
	GCONTAINER_ADD_ELEMENT(widget);
	g_signal_connect(widget->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), widget);
}

static const zend_function_entry gmessage_dialog_class_functions[] = {
	PHP_ME(GMessageDialog, __construct        , arginfo_pggi_void, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *gmessage_dialog_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	//ze_gwidget_object * intern = Z_GWIDGET_P(object);
	//gwidget_ptr w = intern->widget_ptr;
	return gwindow_read_property(object, member, type, cache_slot, rv);
}

HashTable *gmessage_dialog_get_properties(zval *object){
	//G_H_UPDATE_INIT(gwindow_get_properties(object));
	//ze_gwidget_object * intern = Z_GWIDGET_P(object);
	//gwidget_ptr w = intern->widget_ptr;
	//GtkMessageDialog * dialog = GTK_MESSAGE_DIALOG(w->intern);
	//return G_H_UPDATE_RETURN;
	return gwindow_get_properties(object);
}

void gmessage_dialog_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	//ze_gwidget_object * intern = Z_GWIDGET_P(object);
	//gwidget_ptr w = intern->widget_ptr;
	//convert_to_string(member);
	//char * member_val = Z_STRVAL_P(member);
	//GtkMessageDialog * dialog = GTK_MESSAGE_DIALOG(w->intern);
	switch(Z_TYPE_P(value)){
		default :
			gwindow_write_property(object, member, value, cache_slot);
	}
}

/***************************************/
/* GMessageDialog class initialisation */
/***************************************/

#define DECLARE_GMESSAGE_DIALOG_PROP(name) \
DECLARE_CLASS_PROPERTY(gmessage_dialog_class_entry_ce, name)


#define GMESSAGE_DIALOG_CONSTANT(name, value) \
zend_declare_class_constant_long(gmessage_dialog_class_entry_ce, name, sizeof(name)-1, value);

void gmessage_dialog_init(int module_number){
	DECLARE_CLASS_PROPERTY_INIT();
	le_gmessage_dialog = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "PGGI\\GMessageDialog", module_number);

	memcpy(&gmessage_dialog_object_handlers, gdialog_get_object_handlers(), sizeof(zend_object_handlers));
	gmessage_dialog_object_handlers.read_property  = gmessage_dialog_read_property;
	gmessage_dialog_object_handlers.get_properties = gmessage_dialog_get_properties;
	gmessage_dialog_object_handlers.write_property = gmessage_dialog_write_property;

	INIT_CLASS_ENTRY(ce, "PGGI\\GMessageDialog", gmessage_dialog_class_functions);
	gmessage_dialog_class_entry_ce = zend_register_internal_class_ex(&ce, gdialog_get_class_entry());

	GMESSAGE_DIALOG_CONSTANT("MESSAGE_INFO"    , GTK_MESSAGE_INFO    );
	GMESSAGE_DIALOG_CONSTANT("MESSAGE_WARNING" , GTK_MESSAGE_WARNING );
	GMESSAGE_DIALOG_CONSTANT("MESSAGE_QUESTION", GTK_MESSAGE_QUESTION);
	GMESSAGE_DIALOG_CONSTANT("MESSAGE_ERROR"   , GTK_MESSAGE_ERROR   );
	GMESSAGE_DIALOG_CONSTANT("MESSAGE_OTHER"   , GTK_MESSAGE_OTHER   );

	GMESSAGE_DIALOG_CONSTANT("BUTTONS_NONE"     , GTK_BUTTONS_NONE     );
	GMESSAGE_DIALOG_CONSTANT("BUTTONS_OK"       , GTK_BUTTONS_OK       );
	GMESSAGE_DIALOG_CONSTANT("BUTTONS_CLOSE"    , GTK_BUTTONS_CLOSE    );
	GMESSAGE_DIALOG_CONSTANT("BUTTONS_CANCEL"   , GTK_BUTTONS_CANCEL   );
	GMESSAGE_DIALOG_CONSTANT("BUTTONS_YES_NO"   , GTK_BUTTONS_YES_NO   );
	GMESSAGE_DIALOG_CONSTANT("BUTTONS_OK_CANCEL", GTK_BUTTONS_OK_CANCEL);
}



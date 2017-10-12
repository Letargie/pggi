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


#include "gdialog.h"

static int le_gdialog;
static zend_object_handlers gdialog_object_handlers;
static zend_class_entry * gdialog_class_entry_ce;

zend_class_entry * gdialog_get_class_entry(){
	return gdialog_class_entry_ce;
}

zend_object_handlers * gdialog_get_object_handlers(){
	return &gdialog_object_handlers;
}


/***************/
/* PHP Methods */
/***************/

GDIALOG_METHOD(__construct){
	zend_string * title = NULL;
	zval * parent = NULL;
	ze_gwidget_object * widget;
	long flags = 0;
	zval * array = NULL;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "|SOlz", &title, &parent, gwindow_get_class_entry(), &flags, &array) == FAILURE)
		return;
	widget = Z_GWIDGET_P(getThis());
	widget->std.handlers = &gdialog_object_handlers;
	widget->widget_ptr = gwidget_new();
	// SHOULD take care of parameters later
	widget->widget_ptr->intern = gtk_dialog_new();
	GCONTAINER_ADD_ELEMENT(widget);
	g_signal_connect(widget->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), widget);
}

GDIALOG_METHOD(run){
	ze_gwidget_object * ze_obj;
	ze_obj = Z_GWIDGET_P(getThis());
	RETURN_LONG(gtk_dialog_run(GTK_DIALOG(ze_obj->widget_ptr->intern)));
}

/**
 * List of GDialog functions and methods with their arguments
 */
static const zend_function_entry gdialog_class_functions[] = {
	PHP_ME(GDialog, __construct, arginfo_gdialog_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(GDialog, run        , arginfo_pggi_void        , ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

/*
zval *gdialog_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	zval zobj;
	zend_long lval;

	ZVAL_NULL(rv);
	if(w){
		convert_to_string(member);
		char * member_val = Z_STRVAL_P(member);
		GtkWindow * win = GTK_WINDOW(w->intern);
		if(!strcmp(member_val, GWINDOW_TITLE)){
			const char * tmp = gtk_window_get_title(GTK_WINDOW(intern->widget_ptr->intern));
			ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
		}else if(!strcmp(member_val, GWINDOW_FOCUS_VISIBLE))
			ZVAL_BOOL(rv, gtk_window_get_focus_visible(win));
		else if(!strcmp(member_val, GWINDOW_FOCUS_ON_MAP))
			ZVAL_BOOL(rv, gtk_window_get_focus_on_map(win));
		else if(!strcmp(member_val, GWINDOW_DECORATED))
			ZVAL_BOOL(rv, gtk_window_get_decorated(win));
		else if(!strcmp(member_val, GWINDOW_DELETABLE))
			ZVAL_BOOL(rv, gtk_window_get_deletable(win));
		else if(!strcmp(member_val, GWINDOW_MNEMONICS_VISIBLE))
			ZVAL_BOOL(rv, gtk_window_get_mnemonics_visible(win));
		else if(!strcmp(member_val, GWINDOW_RESIZABLE))
			ZVAL_BOOL(rv, gtk_window_get_resizable(win));
		else if(!strcmp(member_val, GWINDOW_HIDE_TITLEBAR_WHEN_MAXIMIZED))
			ZVAL_BOOL(rv, gtk_window_get_hide_titlebar_when_maximized(win));
		else if(!strcmp(member_val, GWINDOW_ACCEPT_FOCUS))
			ZVAL_BOOL(rv, gtk_window_get_accept_focus(win));
		else
			return gcontainer_read_property(object, member, type, cache_slot, rv);
	}
	return rv;
}

HashTable *gdialog_get_properties(zval *object){
	G_H_UPDATE_INIT(gcontainer_get_properties(object));
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	if(!w){
		return NULL;
	}
	GtkWindow * win = GTK_WINDOW(w->intern);


	return G_H_UPDATE_RETURN;
}

void gdialog_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	zval * tmp_member;
	long tmp_l;
	int tmp_b;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkWindow * win = GTK_WINDOW(w->intern);
	switch(Z_TYPE_P(value)){
		case IS_STRING :
			if(!strcmp(member_val, GWINDOW_TITLE))
				gtk_window_set_title(win, Z_STRVAL_P(value));
			else
				gcontainer_write_property(object, member, value, cache_slot);
			break;
		case IS_TRUE :
		case IS_FALSE :
			tmp_b = value == IS_TRUE ? 1 :0;
			if(!strcmp(member_val, GWINDOW_FOCUS_VISIBLE))
				gtk_window_set_focus_visible(win, tmp_l);
			else if(!strcmp(member_val, GWINDOW_FOCUS_ON_MAP))
				gtk_window_set_focus_on_map(win, tmp_l);
			else if(!strcmp(member_val, GWINDOW_DECORATED))
				gtk_window_set_decorated(win, tmp_l);
			else if(!strcmp(member_val, GWINDOW_DELETABLE))
				gtk_window_set_deletable(win, tmp_l);
			else if(!strcmp(member_val, GWINDOW_MNEMONICS_VISIBLE))
				gtk_window_set_mnemonics_visible(win, tmp_l);
			else if(!strcmp(member_val, GWINDOW_RESIZABLE))
				gtk_window_set_resizable(win, tmp_l);
			else if(!strcmp(member_val, GWINDOW_HIDE_TITLEBAR_WHEN_MAXIMIZED))
				gtk_window_set_hide_titlebar_when_maximized(win, tmp_l);
			else if(!strcmp(member_val, GWINDOW_ACCEPT_FOCUS))
				gtk_window_set_accept_focus(win, tmp_l);
			else
				gcontainer_write_property(object, member, value, cache_slot);
			break;
		default :
			gcontainer_write_property(object, member, value, cache_slot);
	}
}*/

/********************************/
/* GDialog class initialisation */
/********************************/

#define DECLARE_GDIALOG_PROP(name) \
DECLARE_CLASS_PROPERTY(gdialog_class_entry_ce, name)

#define GDIALOG_CONSTANT(name, value) \
zend_declare_class_constant_long(gdialog_class_entry_ce, name, sizeof(name)-1, value);

void gdialog_init(int module_number){
	DECLARE_CLASS_PROPERTY_INIT();
	le_gdialog = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "PGGI\\GDialog", module_number);

	memcpy(&gdialog_object_handlers, gwindow_get_object_handlers(), sizeof(zend_object_handlers));
	//gdialog_object_handlers.read_property  = gdialog_read_property;
	//gdialog_object_handlers.get_properties = gdialog_get_properties;
	//gdialog_object_handlers.write_property = gdialog_write_property;

	INIT_CLASS_ENTRY(ce, "PGGI\\GDialog", gdialog_class_functions);
	gdialog_class_entry_ce = zend_register_internal_class_ex(&ce, gwindow_get_class_entry());

	/*DECLARE_GDIALOG_PROP(GDIALOG_USE_HEADER_BAR);
	DECLARE_GDIALOG_PROP(GDIALOG_BUTTON_SPACING);
	DECLARE_GDIALOG_PROP(GDIALOG_AREA_BORDER);
	DECLARE_GDIALOG_PROP(GDIALOG_AREA_SPACING);*/

	GDIALOG_CONSTANT("FLAG_MODAL"                ,GDIALOG_FLAG_MODAL                );
	GDIALOG_CONSTANT("FLAG_DESTROY_WITH_PARENT"  ,GDIALOG_FLAG_DESTROY_WITH_PARENT  );
	GDIALOG_CONSTANT("FLAG_USE_HEADER_BAR"       ,GDIALOG_FLAG_USE_HEADER_BAR       );

	GDIALOG_CONSTANT("RESPONSE_TYPE_NONE"        ,GDIALOG_RESPONSE_TYPE_NONE        );
	GDIALOG_CONSTANT("RESPONSE_TYPE_REJECT"      ,GDIALOG_RESPONSE_TYPE_REJECT      );
	GDIALOG_CONSTANT("RESPONSE_TYPE_ACCEPT"      ,GDIALOG_RESPONSE_TYPE_ACCEPT      );
	GDIALOG_CONSTANT("RESPONSE_TYPE_DELETE_EVENT",GDIALOG_RESPONSE_TYPE_DELETE_EVENT);
	GDIALOG_CONSTANT("RESPONSE_TYPE_OK"          ,GDIALOG_RESPONSE_TYPE_OK          );
	GDIALOG_CONSTANT("RESPONSE_TYPE_CANCEL"      ,GDIALOG_RESPONSE_TYPE_CANCEL      );
	GDIALOG_CONSTANT("RESPONSE_TYPE_CLOSE"       ,GDIALOG_RESPONSE_TYPE_CLOSE       );
	GDIALOG_CONSTANT("RESPONSE_TYPE_YES"         ,GDIALOG_RESPONSE_TYPE_YES         );
	GDIALOG_CONSTANT("RESPONSE_TYPE_NO"          ,GDIALOG_RESPONSE_TYPE_NO          );
	GDIALOG_CONSTANT("RESPONSE_TYPE_APPLY"       ,GDIALOG_RESPONSE_TYPE_APPLY       );
	GDIALOG_CONSTANT("RESPONSE_TYPE_HELP"        ,GDIALOG_RESPONSE_TYPE_HELP        );
}



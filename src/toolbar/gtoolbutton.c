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


#include "gtoolbutton.h"

static int le_gtool_button;
static zend_object_handlers gtool_button_object_handlers;
static zend_class_entry * gtool_button_class_entry_ce;

zend_class_entry * gtool_button_get_class_entry(){
	return gtool_button_class_entry_ce;
}

zend_object_handlers * gtool_button_get_object_handlers(){
	return &gtool_button_object_handlers;
}

void gtool_button_func_clicked(GtkToolButton* w, gpointer data){
	zval args[2];
	gwidget_function(data, gsignal_gtool_button_clicked, args, 2);
}


void gtool_button_on(long val,ze_gwidget_object * ze_obj, zval * function, zval * param){
	switch(val){
		case gsignal_gtool_button_clicked :
			gwidget_adding_function(val, GSIGNAL_GTOOL_BUTTON_CLICKED, G_CALLBACK(gtool_button_func_clicked), ze_obj, function, param);
			break;
		default :
			gcontainer_on(val, ze_obj, function, param);
	}
	
}

PHP_METHOD(GToolButton, __construct){
	ze_gwidget_object * widget, * obj_icon, * obj_label = NULL;
	zval * icon, * label = NULL;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "z|z", &icon, &label) == FAILURE)
		return;
	widget = Z_GWIDGET_P(getThis());
	widget->std.handlers = &gtool_button_object_handlers;
	widget->widget_ptr = gwidget_new();
	switch(Z_TYPE_P(icon)){
		case IS_NULL :
			if(label){
				switch(Z_TYPE_P(label)){
					case IS_NULL   :
						widget->widget_ptr->intern = GTK_WIDGET(gtk_tool_button_new(NULL, NULL));
						break;
					case IS_STRING :
						widget->widget_ptr->intern = GTK_WIDGET(gtk_tool_button_new(NULL, Z_STRVAL_P(label)));
						break;
					case IS_OBJECT :
						obj_label = Z_GWIDGET_P(label);
						if(!obj_label){
							zend_throw_exception_ex(pggi_exception_get(), 0, "the label needs to be a string or a widget");
						}
						widget->widget_ptr->intern = GTK_WIDGET(gtk_tool_button_new(NULL, NULL));
						gtk_tool_button_set_label_widget(GTK_TOOL_BUTTON(widget->widget_ptr->intern), obj_label->widget_ptr->intern);
						break;
					default :
						zend_throw_exception_ex(pggi_exception_get(), 0, "the label needs to be a string or a widget");
						return;
				}
			}else
				widget->widget_ptr->intern = GTK_WIDGET(gtk_tool_button_new(NULL, NULL));
			break;
		case IS_STRING :
			if(label){
				switch(Z_TYPE_P(label)){
					case IS_NULL   :
						widget->widget_ptr->intern = GTK_WIDGET(gtk_tool_button_new(NULL, NULL));
						break;
					case IS_STRING :
						widget->widget_ptr->intern = GTK_WIDGET(gtk_tool_button_new(NULL, Z_STRVAL_P(label)));
						break;
					case IS_OBJECT :
						obj_icon = Z_GWIDGET_P(label);
						if(!obj_label){
							zend_throw_exception_ex(pggi_exception_get(), 0, "the label needs to be a string or a widget");
						}
						widget->widget_ptr->intern = GTK_WIDGET(gtk_tool_button_new(NULL, NULL));
						gtk_tool_button_set_label_widget(GTK_TOOL_BUTTON(widget->widget_ptr->intern), obj_label->widget_ptr->intern);
						break;
					default :
						zend_throw_exception_ex(pggi_exception_get(), 0, "the label needs to be a string or a widget");
						return;
				}
			}else
				widget->widget_ptr->intern = GTK_WIDGET(gtk_tool_button_new(NULL, NULL));
			gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(widget->widget_ptr->intern), Z_STRVAL_P(icon));
			break;
		case IS_OBJECT :
			obj_icon = Z_GWIDGET_P(icon);
			if(!obj_icon){
				zend_throw_exception_ex(pggi_exception_get(), 0, "the icon needs to be a string or a widget");
			}
			if(label){
				switch(Z_TYPE_P(label)){
					case IS_NULL   :
						widget->widget_ptr->intern = GTK_WIDGET(gtk_tool_button_new(obj_icon->widget_ptr->intern, NULL));
						break;
					case IS_STRING :
						widget->widget_ptr->intern = GTK_WIDGET(gtk_tool_button_new(obj_icon->widget_ptr->intern, Z_STRVAL_P(label)));
						break;
					case IS_OBJECT :
						obj_label = Z_GWIDGET_P(label);
						if(!obj_icon){
							zend_throw_exception_ex(pggi_exception_get(), 0, "the label needs to be a string or a widget");
						}
						widget->widget_ptr->intern = GTK_WIDGET(gtk_tool_button_new(obj_icon->widget_ptr->intern, NULL));
						gtk_tool_button_set_label_widget(GTK_TOOL_BUTTON(widget->widget_ptr->intern), obj_label->widget_ptr->intern);
						break;
					default :
						zend_throw_exception_ex(pggi_exception_get(), 0, "the label needs to be a string or a widget");
						return;
				}
			}else{
				
			}
			break;
		default :
			zend_throw_exception_ex(pggi_exception_get(), 0, "the icon needs to be a string or a widget");
			return;
	}
	GCONTAINER_ADD_ELEMENT(widget);
	g_signal_connect(widget->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), widget);
}


static const zend_function_entry gtool_button_class_functions[] = {
	PHP_ME(GToolButton, __construct, arginfo_gtool_button_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_FE_END
};



zval *gtool_button_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	if(!strcmp(member_val, GTOOL_BUTTON_USE_UNDERLINE)){
		ZVAL_BOOL(rv, gtk_tool_button_get_use_underline(GTK_TOOL_BUTTON(w->intern)));
	}else if(!strcmp(member_val, GTOOL_BUTTON_ICON_LABEL)){
		ZVAL_STRING(rv, gtk_tool_button_get_label(GTK_TOOL_BUTTON(w->intern)));
	}else if(!strcmp(member_val, GTOOL_BUTTON_ICON_NAME)){
		ZVAL_STRING(rv, gtk_tool_button_get_icon_name(GTK_TOOL_BUTTON(w->intern)));
	}else if(!strcmp(member_val, GTOOL_BUTTON_LABEL_WIDGET)){
		return std_object_handlers.read_property(object, member, type, cache_slot, rv);
	}else if(!strcmp(member_val, GTOOL_BUTTON_ICON_WIDGET)){
		return std_object_handlers.read_property(object, member, type, cache_slot, rv);
	}else
		return gcontainer_read_property(object, member, type, cache_slot, rv);
	return rv;
}

HashTable *gtool_button_get_properties(zval *object){
	G_H_UPDATE_INIT(gcontainer_get_properties(object));
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;

	G_H_UPDATE_STRING(GTOOL_BUTTON_ICON_LABEL   , gtk_tool_button_get_label        (GTK_TOOL_BUTTON(w->intern)));
	G_H_UPDATE_STRING(GTOOL_BUTTON_ICON_NAME    , gtk_tool_button_get_icon_name    (GTK_TOOL_BUTTON(w->intern)));
	G_H_UPDATE_BOOL  (GTOOL_BUTTON_USE_UNDERLINE, gtk_tool_button_get_use_underline(GTK_TOOL_BUTTON(w->intern)));

	return G_H_UPDATE_RETURN;
}

PHP_WRITE_PROP_HANDLER_TYPE gtool_button_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	ze_gwidget_object * tmp_widget;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	if(!strcmp(member_val, GTOOL_BUTTON_ICON_LABEL)){
		convert_to_string(value);
		gtk_tool_button_set_label(GTK_TOOL_BUTTON(w->intern), Z_STRVAL_P(value));
	}else if(!strcmp(member_val, GTOOL_BUTTON_ICON_NAME)){
		convert_to_string(value);
		gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(w->intern), Z_STRVAL_P(value));
	}else if(!strcmp(member_val, GTOOL_BUTTON_USE_UNDERLINE)){
		convert_to_boolean(value);
		gtk_tool_button_set_use_underline(GTK_TOOL_BUTTON(w->intern), GET_BOOL_FROM_ZVAL(value));
	}else if(!strcmp(member_val, GTOOL_BUTTON_ICON_WIDGET)){
		tmp_widget = Z_GWIDGET_P(value);
		if(!tmp_widget){
			zend_throw_exception_ex(pggi_exception_get(), 0, "the icon needs to be a widget");
			PHP_WRITE_PROP_HANDLER_RETURN(NULL);
		}
		w = tmp_widget->widget_ptr;
		std_object_handlers.write_property(object, member, value, cache_slot);
		gtk_tool_button_set_icon_widget(GTK_TOOL_BUTTON(w->intern), w->intern);
	}else if(!strcmp(member_val, GTOOL_BUTTON_LABEL_WIDGET)){
		tmp_widget = Z_GWIDGET_P(value);
		if(!tmp_widget){
			zend_throw_exception_ex(pggi_exception_get(), 0, "the label needs to be a widget");
			PHP_WRITE_PROP_HANDLER_RETURN(NULL);
		}
		w = tmp_widget->widget_ptr;
		std_object_handlers.write_property(object, member, value, cache_slot);
		gtk_tool_button_set_label_widget(GTK_TOOL_BUTTON(w->intern), w->intern);
	} else {
		PHP_WRITE_PROP_HANDLER_RETURN(gcontainer_write_property(object, member, value, cache_slot));
	}
	PHP_WRITE_PROP_HANDLER_RETURN(value);
}


#define DECLARE_GTOOL_BUTTON_PROP(name) \
DECLARE_CLASS_PROPERTY(gtool_button_class_entry_ce, name)

void gtool_button_init(int module_number){
	zend_class_entry ce;
	le_gtool_button = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "PGGI\\GToolButton", module_number);

	memcpy(&gtool_button_object_handlers, gcontainer_get_object_handlers(), sizeof(zend_object_handlers));
	gtool_button_object_handlers.read_property  = gtool_button_read_property;
	gtool_button_object_handlers.get_properties = gtool_button_get_properties;
	gtool_button_object_handlers.write_property = gtool_button_write_property;	
	INIT_CLASS_ENTRY(ce, "PGGI\\GToolButton", gtool_button_class_functions);
	ce.create_object 	= gwidget_object_new;
	gtool_button_class_entry_ce	= zend_register_internal_class_ex(&ce, gtool_item_get_class_entry());

	DECLARE_GTOOL_BUTTON_PROP(GTOOL_BUTTON_ICON_NAME    );
	DECLARE_GTOOL_BUTTON_PROP(GTOOL_BUTTON_ICON_WIDGET  );
	DECLARE_GTOOL_BUTTON_PROP(GTOOL_BUTTON_ICON_LABEL   );
	DECLARE_GTOOL_BUTTON_PROP(GTOOL_BUTTON_LABEL_WIDGET );
	DECLARE_GTOOL_BUTTON_PROP(GTOOL_BUTTON_USE_UNDERLINE);
}



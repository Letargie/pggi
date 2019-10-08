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


#include "glabel.h"

static int le_glabel;
static zend_object_handlers glabel_object_handlers;
static zend_class_entry * glabel_class_entry_ce;

zend_class_entry * glabel_get_class_entry(){
	return glabel_class_entry_ce;
}

/***************/
/* PHP Methods */
/***************/

PHP_METHOD(GLabel, __construct){
	zend_string * label = NULL;
	ze_gwidget_object * ze_obj;
	zval * self = getThis();
	ze_obj = Z_GWIDGET_P(self);
		ze_obj->std.handlers = &glabel_object_handlers;
	char * c = NULL;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|S", &label) == FAILURE)
		return;
	if(label)
		c = ZSTR_VAL(label);
	ze_obj->widget_ptr = gwidget_new();
	ze_obj->widget_ptr->intern = gtk_label_new(c);
	g_signal_connect(ze_obj->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), ze_obj);
}

static const zend_function_entry glabel_class_functions[] = {
	PHP_ME(GLabel, __construct, arginfo_glabel_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/


zval *glabel_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	const char * tmp;
	GtkLabel * label = GTK_LABEL(w->intern);
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	if(!strcmp(member_val, GLABEL_ELLIPSIZE)){
		ZVAL_LONG(rv, gtk_label_get_ellipsize(label));
	}else if(!strcmp(member_val, GLABEL_LINES)){
		ZVAL_LONG(rv, gtk_label_get_lines(label));
	}else if(!strcmp(member_val, GLABEL_WIDTH_CHARS)){
		ZVAL_LONG(rv, gtk_label_get_width_chars(label));
	}else if(!strcmp(member_val, GLABEL_MAX_WIDTH_CHARS)){
		ZVAL_LONG(rv, gtk_label_get_max_width_chars(label));
	}else if(!strcmp(member_val, GLABEL_XALIGN)){
		ZVAL_DOUBLE(rv, gtk_label_get_xalign(label));
	}else if(!strcmp(member_val, GLABEL_YALIGN)){
		ZVAL_DOUBLE(rv, gtk_label_get_yalign(label));
	}else if(!strcmp(member_val, GLABEL_TEXT)){
		tmp = gtk_label_get_text(label);
		if(tmp)
			ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
		else
			ZVAL_NULL(rv);
	}else if(!strcmp(member_val, GLABEL_SELECTABLE)){
		ZVAL_BOOL(rv, gtk_label_get_selectable(label));
	}else if(!strcmp(member_val, GLABEL_TRACK_VISITED_LINKS)){
		ZVAL_BOOL(rv, gtk_label_get_track_visited_links(label));
	}else if(!strcmp(member_val, GLABEL_USE_MARKUP)){
		ZVAL_BOOL(rv, gtk_label_get_use_markup(label));
	}else if(!strcmp(member_val, GLABEL_USE_UNDERLINE)){
		ZVAL_BOOL(rv, gtk_label_get_use_underline(label));
	}else
		return gwidget_read_property(object, member, type, cache_slot, rv);
	return rv;
}

HashTable *glabel_get_properties(zval *object){
	G_H_UPDATE_INIT(gwidget_get_properties(object));
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	intern->std.handlers = &glabel_object_handlers;
	gwidget_ptr w = intern->widget_ptr;
	GtkLabel * label = GTK_LABEL(w->intern);

	G_H_UPDATE_LONG  (GLABEL_ELLIPSIZE          , gtk_label_get_ellipsize          (label));
	G_H_UPDATE_LONG  (GLABEL_LINES              , gtk_label_get_lines              (label));
	G_H_UPDATE_LONG  (GLABEL_WIDTH_CHARS        , gtk_label_get_width_chars        (label));
	G_H_UPDATE_LONG  (GLABEL_MAX_WIDTH_CHARS    , gtk_label_get_max_width_chars    (label));
	G_H_UPDATE_BOOL  (GLABEL_SELECTABLE         , gtk_label_get_selectable         (label));
	G_H_UPDATE_BOOL  (GLABEL_TRACK_VISITED_LINKS, gtk_label_get_track_visited_links(label));
	G_H_UPDATE_BOOL  (GLABEL_USE_MARKUP         , gtk_label_get_use_markup         (label));
	G_H_UPDATE_BOOL  (GLABEL_USE_UNDERLINE      , gtk_label_get_use_underline      (label));
	G_H_UPDATE_DOUBLE(GLABEL_XALIGN             , gtk_label_get_xalign             (label));
	G_H_UPDATE_DOUBLE(GLABEL_YALIGN             , gtk_label_get_yalign             (label));
	G_H_UPDATE_STRING(GLABEL_TEXT               , gtk_label_get_text               (label));

	return G_H_UPDATE_RETURN;
}

PHP_WRITE_PROP_HANDLER_TYPE glabel_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	GtkLabel * label = GTK_LABEL(w->intern);
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	if(!strcmp(member_val, GLABEL_ELLIPSIZE)){
		convert_to_long(value);
		gtk_label_set_ellipsize(label, Z_LVAL_P(value));
	}else if(!strcmp(member_val, GLABEL_LINES)){
		convert_to_long(value);
		gtk_label_set_lines(label, Z_LVAL_P(value));
	}else if(!strcmp(member_val, GLABEL_WIDTH_CHARS)){
		convert_to_long(value);
		gtk_label_set_width_chars(label, Z_LVAL_P(value));
	}else if(!strcmp(member_val, GLABEL_MAX_WIDTH_CHARS)){
		convert_to_long(value);
		gtk_label_set_max_width_chars(label, Z_LVAL_P(value));
	}else if(!strcmp(member_val, GLABEL_XALIGN)){
		convert_to_double(value);
		gtk_label_set_xalign(label, Z_DVAL_P(value));
	}else if(!strcmp(member_val, GLABEL_YALIGN)){
		convert_to_double(value);
		gtk_label_set_yalign(label, Z_DVAL_P(value));
	}else if(!strcmp(member_val, GLABEL_TEXT)){
		convert_to_string(value);
		gtk_label_set_text(label, Z_STRVAL_P(value));
	}else if(!strcmp(member_val, GLABEL_SELECTABLE)){
		convert_to_boolean(value);
		gtk_label_set_selectable(label, GET_BOOL_FROM_ZVAL(value));
	}else if(!strcmp(member_val, GLABEL_USE_UNDERLINE)){
		convert_to_boolean(value);
		gtk_label_set_use_underline(label, GET_BOOL_FROM_ZVAL(value));
	}else if(!strcmp(member_val, GLABEL_USE_MARKUP)){
		convert_to_boolean(value);
		gtk_label_set_use_markup(label, GET_BOOL_FROM_ZVAL(value));
	}else if(!strcmp(member_val, GLABEL_TRACK_VISITED_LINKS)){
		convert_to_boolean(value);
		gtk_label_set_track_visited_links(label, GET_BOOL_FROM_ZVAL(value));
	}else
		gwidget_write_property(object, member, value, cache_slot);
	PHP_WRITE_PROP_HANDLER_RETURN(value);
}

/*******************************/
/* GLabel class initialisation */
/*******************************/

#define DECLARE_GLABEL_PROP(name) \
DECLARE_CLASS_PROPERTY(glabel_class_entry_ce, name)

#define GLABEL_CONSTANT(name, value) \
zend_declare_class_constant_double(glabel_class_entry_ce, name, sizeof(name)-1, value);

void glabel_init(int module_number){
	zend_class_entry ce;
	le_glabel = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "PGGI\\GLabel", module_number);

	memcpy(&glabel_object_handlers, gwidget_get_object_handlers(), sizeof(zend_object_handlers));
	glabel_object_handlers.read_property  = glabel_read_property;
	glabel_object_handlers.get_properties = glabel_get_properties;
	glabel_object_handlers.write_property = glabel_write_property;
	INIT_CLASS_ENTRY(ce, "PGGI\\GLabel", glabel_class_functions);
	ce.create_object = gwidget_object_new;
	glabel_class_entry_ce = zend_register_internal_class_ex(&ce, gwidget_get_class_entry());

	GLABEL_CONSTANT("ELLIPSIZE_NONE"  , PANGO_ELLIPSIZE_NONE  );
	GLABEL_CONSTANT("ELLIPSIZE_START" , PANGO_ELLIPSIZE_START );
	GLABEL_CONSTANT("ELLIPSIZE_MIDDLE", PANGO_ELLIPSIZE_MIDDLE);
	GLABEL_CONSTANT("ELLIPSIZE_END"   , PANGO_ELLIPSIZE_END   );

	DECLARE_GLABEL_PROP(GLABEL_TRACK_VISITED_LINKS);
	DECLARE_GLABEL_PROP(GLABEL_USE_MARKUP         );
	DECLARE_GLABEL_PROP(GLABEL_USE_UNDERLINE      );
	DECLARE_GLABEL_PROP(GLABEL_XALIGN             );
	DECLARE_GLABEL_PROP(GLABEL_YALIGN             );
	DECLARE_GLABEL_PROP(GLABEL_ELLIPSIZE          );
	DECLARE_GLABEL_PROP(GLABEL_WIDTH_CHARS        );
	DECLARE_GLABEL_PROP(GLABEL_MAX_WIDTH_CHARS    );
	DECLARE_GLABEL_PROP(GLABEL_LINES              );
	DECLARE_GLABEL_PROP(GLABEL_SELECTABLE         );
	DECLARE_GLABEL_PROP(GLABEL_TEXT               );
}



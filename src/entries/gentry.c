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


#include "gentry.h"


static int le_gentry;
static zend_object_handlers gentry_object_handlers;
static zend_class_entry * gentry_class_entry_ce;

zend_class_entry * gentry_get_class_entry(){
	return gentry_class_entry_ce;
}

zend_object_handlers * gentry_get_object_handlers(){
	return &gentry_object_handlers;
}

/*****************/
/* PHP FUNCTIONS */
/*****************/

void gentry_func_activate(GtkWidget * entry, gpointer data){
	zval args[2];
	gwidget_function(data, gsignal_gentry_activate, args, 2);
}

void gentry_on(long val,ze_gwidget_object * ze_obj, zval * function, zval * param){
	switch(val){
		case gsignal_gentry_activate :
			gwidget_adding_function(val, GSIGNAL_GENTRY_ACTIVATE, G_CALLBACK(gentry_func_activate), ze_obj, function, param);
			break;
		default :
			gwidget_on(val, ze_obj, function, param);
	}
}

PHP_METHOD(GEntry, on){
	zval * function, * this, *param = NULL;
	long val;
	ze_gwidget_object *ze_obj = NULL;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "lz|z", &val ,&function, &param) == FAILURE)
		return;
	this = getThis();
	ze_obj = Z_GWIDGET_P(this);
	gentry_on(val, ze_obj, function, param);
}

GENTRY_METHOD(__construct){
	zval * obj;
	ze_gwidget_object * widget;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &obj, gentry_buffer_get_class_entry()) == FAILURE)
		return;
	ze_gentry_buffer_object *ze_obj = NULL;
	ze_obj = Z_GENTRY_BUFFER_P(obj);
	widget = Z_GWIDGET_P(getThis());
	widget->std.handlers = &gentry_object_handlers;
	widget->widget_ptr = gwidget_new();
	widget->widget_ptr->intern = gtk_entry_new_with_buffer(ze_obj->buffer_ptr->intern);
	g_signal_connect(widget->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), widget);
}

GENTRY_METHOD(getTextLength){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none(self) == FAILURE)
		return ;
	ze_obj = Z_GWIDGET_P(self);
	RETURN_LONG(gtk_entry_get_text_length(GTK_ENTRY(ze_obj->widget_ptr->intern)));
}
GENTRY_METHOD(unsetInvisibleChar){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none(self) == FAILURE)
		return ;
	ze_obj = Z_GWIDGET_P(self);
	gtk_entry_unset_invisible_char(GTK_ENTRY(ze_obj->widget_ptr->intern));
}
GENTRY_METHOD(progressPulse){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none(self) == FAILURE)
		return ;
	ze_obj = Z_GWIDGET_P(self);
	gtk_entry_progress_pulse(GTK_ENTRY(ze_obj->widget_ptr->intern));
}


static const zend_function_entry gentry_class_functions[] = {
	PHP_ME(GEntry, __construct       , arginfo_gentry_construct, ZEND_ACC_PUBLIC  | ZEND_ACC_CTOR)
	PHP_ME(GEntry, getTextLength     , arginfo_pggi_get_long   , ZEND_ACC_PUBLIC)
	PHP_ME(GEntry, unsetInvisibleChar, arginfo_pggi_void       , ZEND_ACC_PUBLIC)
	PHP_ME(GEntry, progressPulse     , arginfo_pggi_void       , ZEND_ACC_PUBLIC)
	PHP_ME(GEntry, on                , arginfo_pggi_on         , ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *gentry_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	const char * tmp;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkEntry * e = GTK_ENTRY(w->intern);
	if(!strcmp(member_val, GENTRY_TEXT)){
		tmp = gtk_entry_get_text(e);
		if(tmp)
			ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
		else
			ZVAL_NULL(rv);
	}else if(!strcmp(member_val, GENTRY_VISIBILITY)){
		ZVAL_BOOL(rv, gtk_entry_get_visibility(e));
	}else if(!strcmp(member_val, GENTRY_INVISIBLE_CHAR)){
		ZVAL_LONG(rv, gtk_entry_get_invisible_char(e));
	}else if(!strcmp(member_val, GENTRY_MAX_LENGTH)){
		ZVAL_LONG(rv, gtk_entry_get_max_length(e));
	}else if(!strcmp(member_val, GENTRY_ACTIVATES_DEFAULT)){
		ZVAL_BOOL(rv, gtk_entry_get_activates_default(e));
	}else if(!strcmp(member_val, GENTRY_HAS_FRAME)){
		ZVAL_BOOL(rv, gtk_entry_get_has_frame(e));
	}else if(!strcmp(member_val, GENTRY_WIDTH_CHARS)){
		ZVAL_LONG(rv, gtk_entry_get_width_chars(e));
	}else if(!strcmp(member_val, GENTRY_MAX_WIDTH_CHARS)){
		ZVAL_LONG(rv, gtk_entry_get_max_width_chars(e));
	}else if(!strcmp(member_val, GENTRY_ALIGNMENT)){
		ZVAL_DOUBLE(rv, gtk_entry_get_alignment(e));
	}else if(!strcmp(member_val, GENTRY_PLACEHOLDER_TEXT)){
		tmp = gtk_entry_get_placeholder_text(e);
		if(tmp)
			ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
		else
			ZVAL_NULL(rv);
	}else if(!strcmp(member_val, GENTRY_PROGRESS_FRACTION)){
		ZVAL_DOUBLE(rv, gtk_entry_get_progress_fraction(e));
	}else if(!strcmp(member_val, GENTRY_OVERWRITE_MODE)){
		ZVAL_BOOL(rv, gtk_entry_get_overwrite_mode(e));
	}else if(!strcmp(member_val, GENTRY_PROGRESS_PULSE_STEP)){
		ZVAL_DOUBLE(rv, gtk_entry_get_progress_pulse_step(e));
	}else if(!strcmp(member_val, GENTRY_BUFFER)){
		return std_object_handlers.read_property(object, member, type, cache_slot, rv);
	}else 
		return gwidget_read_property(object, member, type, cache_slot, rv);
	return rv;
}

HashTable *gentry_get_properties(zval *object){
	G_H_UPDATE_INIT(gwidget_get_properties(object));
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	GtkEntry * e = GTK_ENTRY(w->intern);
	G_H_UPDATE_STRING(GENTRY_TEXT               , gtk_entry_get_text               (e));
	G_H_UPDATE_BOOL  (GENTRY_VISIBILITY         , gtk_entry_get_visibility         (e));
	G_H_UPDATE_LONG  (GENTRY_INVISIBLE_CHAR     , gtk_entry_get_invisible_char     (e));	
	G_H_UPDATE_LONG  (GENTRY_MAX_LENGTH         , gtk_entry_get_max_length         (e));
	G_H_UPDATE_BOOL  (GENTRY_ACTIVATES_DEFAULT  , gtk_entry_get_activates_default  (e));
	G_H_UPDATE_BOOL  (GENTRY_HAS_FRAME          , gtk_entry_get_has_frame          (e));
	G_H_UPDATE_LONG  (GENTRY_WIDTH_CHARS        , gtk_entry_get_width_chars        (e));
	G_H_UPDATE_LONG  (GENTRY_MAX_WIDTH_CHARS    , gtk_entry_get_max_width_chars    (e));
	G_H_UPDATE_DOUBLE(GENTRY_ALIGNMENT          , gtk_entry_get_alignment          (e));
	G_H_UPDATE_STRING(GENTRY_PLACEHOLDER_TEXT   , gtk_entry_get_placeholder_text   (e));
	G_H_UPDATE_DOUBLE(GENTRY_PROGRESS_FRACTION  , gtk_entry_get_progress_fraction  (e));
	G_H_UPDATE_BOOL  (GENTRY_OVERWRITE_MODE     , gtk_entry_get_overwrite_mode     (e));
	G_H_UPDATE_DOUBLE(GENTRY_PROGRESS_PULSE_STEP, gtk_entry_get_progress_pulse_step(e));
	return G_H_UPDATE_RETURN;
}

void gentry_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	long tmp_l;
	const char * tmp_s;
	double tmp_d;
	int tmp_b;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkEntry * e = GTK_ENTRY(w->intern);
	switch(Z_TYPE_P(value)){
		case IS_LONG :
			tmp_l = Z_LVAL_P(value);
			if(!strcmp(member_val, GENTRY_INVISIBLE_CHAR))
				gtk_entry_set_invisible_char(e, tmp_l);
			else if(!strcmp(member_val, GENTRY_MAX_LENGTH))
				gtk_entry_set_max_length(e, tmp_l);
			else if(!strcmp(member_val, GENTRY_WIDTH_CHARS))
				gtk_entry_set_width_chars(e, tmp_l);
			else if(!strcmp(member_val, GENTRY_MAX_WIDTH_CHARS))
				gtk_entry_set_max_width_chars(e, tmp_l);
			else
				gwidget_write_property(object, member, value, cache_slot);
			break;
		case IS_STRING :
			tmp_s = Z_STRVAL_P(value);
			if(!strcmp(member_val, GENTRY_TEXT))
				gtk_entry_set_text(e, tmp_s);
			else if(!strcmp(member_val, GENTRY_PLACEHOLDER_TEXT))
				gtk_entry_set_placeholder_text(e, tmp_s);
			else
				gwidget_write_property(object, member, value, cache_slot);
			break;
		case IS_DOUBLE :
			tmp_d = Z_DVAL_P(value);
			if(!strcmp(member_val, GENTRY_ALIGNMENT))
				gtk_entry_set_alignment(e, tmp_d);
			else if(!strcmp(member_val, GENTRY_PROGRESS_FRACTION))
				gtk_entry_set_progress_fraction(e, tmp_d);
			else if(!strcmp(member_val, GENTRY_PROGRESS_PULSE_STEP))
				gtk_entry_set_progress_pulse_step(e, tmp_d);
			else
				gwidget_write_property(object, member, value, cache_slot);
			break;
		case IS_FALSE :
		case IS_TRUE :
			tmp_b = Z_TYPE_P(value) == IS_TRUE ? 1 : 0;
			if(!strcmp(member_val, GENTRY_VISIBILITY))
				gtk_entry_set_visibility(e, tmp_b);
			else if(!strcmp(member_val, GENTRY_ACTIVATES_DEFAULT))
				gtk_entry_set_activates_default(e, tmp_b);
			else if(!strcmp(member_val, GENTRY_HAS_FRAME))
				gtk_entry_set_has_frame(e, tmp_b);
			else if(!strcmp(member_val, GENTRY_OVERWRITE_MODE))
				gtk_entry_set_overwrite_mode(e, tmp_b);
			else
				gwidget_write_property(object, member, value, cache_slot);
			break;
		case IS_OBJECT :
			if(!strcmp(member_val, GENTRY_BUFFER)){
				ze_gentry_buffer_object *ze_obj = NULL;
				ze_obj = Z_GENTRY_BUFFER_P(value);
				if(!ze_obj){

				}
				gtk_entry_set_buffer(e, ze_obj->buffer_ptr->intern);
				std_object_handlers.write_property(object, member, value, cache_slot);
			}else
				gwidget_write_property(object, member, value, cache_slot);
			break;
		default:
			gwidget_write_property(object, member, value, cache_slot);

	}
}

/***********************************/
/* GEntry class initialisation */
/***********************************/

#define DECLARE_GENTRY_PROP(name) \
DECLARE_CLASS_PROPERTY(gentry_class_entry_ce, name)

void gentry_init(int module_number){
	zend_class_entry ce;
	le_gentry = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "PGGI\\GEntry", module_number);

	memcpy(&gentry_object_handlers, gwidget_get_object_handlers(), sizeof(zend_object_handlers));
	gentry_object_handlers.read_property  = gentry_read_property;
	gentry_object_handlers.get_properties = gentry_get_properties;
	gentry_object_handlers.write_property = gentry_write_property;
	INIT_CLASS_ENTRY(ce, "PGGI\\GEntry", gentry_class_functions);
	ce.create_object = gwidget_object_new;
	gentry_class_entry_ce = zend_register_internal_class_ex(&ce, gwidget_get_class_entry());

	DECLARE_GENTRY_PROP(GENTRY_BUFFER             );
	DECLARE_GENTRY_PROP(GENTRY_TEXT               );
	DECLARE_GENTRY_PROP(GENTRY_VISIBILITY         );
	DECLARE_GENTRY_PROP(GENTRY_INVISIBLE_CHAR     );
	DECLARE_GENTRY_PROP(GENTRY_MAX_LENGTH         );
	DECLARE_GENTRY_PROP(GENTRY_ACTIVATES_DEFAULT  );
	DECLARE_GENTRY_PROP(GENTRY_HAS_FRAME          );
	DECLARE_GENTRY_PROP(GENTRY_WIDTH_CHARS        );
	DECLARE_GENTRY_PROP(GENTRY_MAX_WIDTH_CHARS    );
	DECLARE_GENTRY_PROP(GENTRY_ALIGNMENT          );
	DECLARE_GENTRY_PROP(GENTRY_PLACEHOLDER_TEXT   );
	DECLARE_GENTRY_PROP(GENTRY_OVERWRITE_MODE     );
	DECLARE_GENTRY_PROP(GENTRY_PROGRESS_FRACTION  );
	DECLARE_GENTRY_PROP(GENTRY_PROGRESS_PULSE_STEP);
}



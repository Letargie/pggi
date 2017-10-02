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


#include "gwidget.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_gwidget;
static zend_object_handlers gwidget_object_handlers;
static zend_class_entry * gwidget_class_entry_ce;

zend_class_entry * gwidget_get_class_entry(){
	return gwidget_class_entry_ce;
}

zend_object_handlers * gwidget_get_object_handlers(){
	return &gwidget_object_handlers;
}

/****************************/
/* Memory handling function */
/****************************/

void widget_destructed(GtkWidget* this, gpointer data){
	ze_gwidget_object * widget = data;
	widget->widget_ptr->intern = NULL;
}

gwidget_ptr gwidget_new(){
	gwidget_ptr tor = ecalloc(1, sizeof(gwidget_t));
	array_init(&tor->data);
	array_init(&tor->signals);
	return tor;
}

zend_object *gwidget_object_new(zend_class_entry *class_type){
	ze_gwidget_object *intern;
	intern = ecalloc(1, sizeof(ze_gwidget_object) + zend_object_properties_size(class_type));
	zend_object_std_init  (&intern->std, class_type);
	object_properties_init(&intern->std, class_type);
	intern->std.handlers = &gwidget_object_handlers;
	return &intern->std;
}

void gwidget_dtor(gwidget_ptr intern){
	zval *  zv, * tmp, * val;
	if (intern->intern){
		gtk_widget_destroy(intern->intern);
	}
	zend_hash_destroy(Z_ARRVAL_P(&intern->signals));
	zend_hash_destroy(Z_ARRVAL_P(&intern->data));
	efree(intern);
}

void gwidget_object_free_storage(zend_object *object){
	ze_gwidget_object *intern = php_gwidget_fetch_object(object);
	if (!intern) {
		return;
	}
	if (intern->widget_ptr){
		gwidget_dtor(intern->widget_ptr);
	}
	intern->widget_ptr = NULL;
	zend_object_std_dtor(&intern->std);
}

void gwidget_free_resource(zend_resource *rsrc) {
	gwidget_ptr widget = (gwidget_ptr) rsrc->ptr;
	gwidget_dtor(widget);
}

#define INDEX_ON_FUNCTION_NAME 1
#define INDEX_ON_FUNCTION_PARAM 2

void gwidget_function(gpointer data, unsigned int type, zval * args, int nb){
	zval retval;
	ze_gwidget_object * w = data;
	zval * array, * value, * zv;
	if(type){
		zend_object * this = php_gwidget_reverse_object(w);
		ZVAL_OBJ(&args[0], this);
		array = &w->widget_ptr->signals;
		value = (zend_hash_index_find(Z_ARRVAL_P(array), type));
		if(value != NULL){
			if(Z_ARRVAL_P(value))
			ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(value), zv){
				zval * function = zend_hash_index_find(Z_ARRVAL_P(zv), INDEX_ON_FUNCTION_NAME );
				zval * tmp      = zend_hash_index_find(Z_ARRVAL_P(zv), INDEX_ON_FUNCTION_PARAM);
				if(tmp){
					ZVAL_COPY(&args[1], tmp);
				}else
					ZVAL_NULL(&args[1]);
				if(call_user_function(EG(function_table), NULL, function, &retval, nb, args) != SUCCESS){
					zend_throw_exception_ex(pggi_exception_get(), 0, "Function call failed");
				}
			} ZEND_HASH_FOREACH_END();
		}
	}
}

/*********************************/
/* internal on-related functions */
/*********************************/

void gwidget_func_destroy(GtkWidget* w, gpointer data){
	zval args[2];
	gwidget_function(data, gsignal_gwidget_destroy, args, 2);
}

int gwidget_func_key_press_event(GtkWidget* w, GdkEvent *event, gpointer data){
	zval args[3];
	zval state, keyval;
	ZVAL_OBJ(&args[2], gevent_ctor(gevent_key_get_class_entry(), event));
	ZVAL_LONG(&state, event->key.state);
	ZVAL_LONG(&keyval, event->key.keyval);
	zend_update_property(gevent_key_get_class_entry(), &args[2], GEVENT_KEY_STATE, sizeof(GEVENT_KEY_STATE) - 1, &state);
	zend_update_property(gevent_key_get_class_entry(), &args[2], GEVENT_KEY_KEYVAL, sizeof(GEVENT_KEY_KEYVAL) - 1, &keyval);
	gwidget_function(data, gsignal_gwidget_key_press_event, args, 3);
	return FALSE;
}
void gwidget_adding_function(long val, char * name, GCallback f, ze_gwidget_object * ze_obj, zval * function, zval * param){
	zval * data, * narray;
	data = zend_hash_index_find(Z_ARRVAL_P(&ze_obj->widget_ptr->signals), val);
	zval * data_to_insert = ecalloc(1,sizeof(zval));
	array_init(data_to_insert);
	zend_hash_index_add(Z_ARRVAL_P(data_to_insert), INDEX_ON_FUNCTION_NAME, function);
	zval_addref_p(function);
	if(param){
		zend_hash_index_add(Z_ARRVAL_P(data_to_insert), INDEX_ON_FUNCTION_PARAM, param);
		zval_addref_p(param);
	}
	if(data == NULL){
		narray = ecalloc(1,sizeof(zval));
		array_init(narray);
		zend_hash_index_add(Z_ARRVAL_P(&ze_obj->widget_ptr->signals), val, narray);
		zend_hash_next_index_insert(Z_ARRVAL_P(narray), data_to_insert);
		g_signal_connect(ze_obj->widget_ptr->intern, name, f, (gpointer) ze_obj);
	}else{
		zend_hash_next_index_insert(Z_ARRVAL_P(data), data_to_insert);
	}
}

void gwidget_on(long val,ze_gwidget_object * ze_obj, zval * function, zval * param){
	switch(val){
		case gsignal_gwidget_destroy :
			gwidget_adding_function(val, GSIGNAL_GWIDGET_DESTROY, G_CALLBACK(gwidget_func_destroy), ze_obj, function, param);
			break;
		case gsignal_gwidget_key_press_event :
			gwidget_adding_function(val, GSIGNAL_GWIDGET_KEY_PRESS_EVENT, G_CALLBACK(gwidget_func_key_press_event), ze_obj, function, param);
			break;
		default :
			zend_throw_exception_ex(pggi_exception_get(), 0, "not handled signal");
	}
	
}

/***************/
/* PHP Methods */
/***************/

GWIDGET_METHOD(on){
	zval * function, * this, * param = NULL;
	long val;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "lz|z", &val ,&function, &param) == FAILURE)
		return;
	this = getThis();
	gwidget_on(val, Z_GWIDGET_P(this), function, param);
}


GWIDGET_METHOD(show){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return ;
	ze_obj = Z_GWIDGET_P(self);
	gtk_widget_show(ze_obj->widget_ptr->intern);
}

GWIDGET_METHOD(hide){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return ;
	ze_obj = Z_GWIDGET_P(self);
	gtk_widget_hide(ze_obj->widget_ptr->intern);
}


GWIDGET_METHOD(showAll){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return ;
	ze_obj = Z_GWIDGET_P(self);
	gtk_widget_show_all(ze_obj->widget_ptr->intern);
}

GWIDGET_METHOD(getStyleContext){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return ;
	ze_obj = Z_GWIDGET_P(self);
	GtkStyleContext * context = gtk_widget_get_style_context(ze_obj->widget_ptr->intern);
	zend_object * obj = gstyle_context_object_new(gstyle_context_get_class_entry());
	ze_gstyle_context_object * tmp = php_gstyle_context_fetch_object(obj);
	tmp->context_ptr = gstyle_context_new();
	tmp->context_ptr->intern = context;
	RETURN_OBJ(obj);
}

static const zend_function_entry gwidget_class_functions[] = {
	PHP_ME(GWidget, on             , arginfo_pggi_on                  , ZEND_ACC_PUBLIC)
	PHP_ME(GWidget, show           , arginfo_pggi_void                , ZEND_ACC_PUBLIC)
	PHP_ME(GWidget, hide           , arginfo_pggi_void                , ZEND_ACC_PUBLIC)
	PHP_ME(GWidget, showAll        , arginfo_pggi_void                , ZEND_ACC_PUBLIC)
	PHP_ME(GWidget, getStyleContext, arginfo_gwidget_get_style_context, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *gwidget_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	const char * tmp;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);

	if(!strcmp(member_val, GWIDGET_NAME)){
		tmp = gtk_widget_get_name(w->intern);
		if(tmp)
			ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
		else
			ZVAL_NULL(rv);
	}else if(!strcmp(member_val, GWIDGET_HALIGN)){
		ZVAL_LONG(rv, gtk_widget_get_halign(w->intern));
	}else if(!strcmp(member_val, GWIDGET_VALIGN)){
		ZVAL_LONG(rv, gtk_widget_get_valign(w->intern));
	}else if(!strcmp(member_val, GWIDGET_MARGIN_START)){
		ZVAL_LONG(rv, gtk_widget_get_margin_start(w->intern));
	}else if(!strcmp(member_val, GWIDGET_MARGIN_END)){
		ZVAL_LONG(rv, gtk_widget_get_margin_end(w->intern));
	}else if(!strcmp(member_val, GWIDGET_MARGIN_TOP)){
		ZVAL_LONG(rv, gtk_widget_get_margin_top(w->intern));
	}else if(!strcmp(member_val, GWIDGET_MARGIN_BOTTOM)){
		ZVAL_LONG(rv, gtk_widget_get_margin_bottom(w->intern));
	}else if(!strcmp(member_val, GWIDGET_DIRECTION)){
		ZVAL_LONG(rv, gtk_widget_get_direction(w->intern));
	}else if(!strcmp(member_val, GWIDGET_HEXPAND)){
		ZVAL_BOOL(rv, gtk_widget_get_hexpand(w->intern));
	}else if(!strcmp(member_val, GWIDGET_HEXPAND_SET)){
		ZVAL_BOOL(rv, gtk_widget_get_hexpand_set(w->intern));
	}else if(!strcmp(member_val, GWIDGET_VEXPAND)){
		ZVAL_BOOL(rv, gtk_widget_get_vexpand(w->intern));
	}else if(!strcmp(member_val, GWIDGET_VEXPAND_SET)){
		ZVAL_BOOL(rv, gtk_widget_get_vexpand_set(w->intern));
	}else if(!strcmp(member_val, GWIDGET_VISIBLE)){
		ZVAL_BOOL(rv, gtk_widget_get_visible(w->intern));
	}else if(!strcmp(member_val, GWIDGET_CAN_FOCUS)){
		ZVAL_BOOL(rv, gtk_widget_get_can_focus(w->intern));
	}else if(!strcmp(member_val, GWIDGET_CAN_DEFAULT)){
		ZVAL_BOOL(rv, gtk_widget_get_can_default(w->intern));
	}else if(!strcmp(member_val, GWIDGET_HAS_TOOLTIP)){
		ZVAL_BOOL(rv, gtk_widget_get_has_tooltip(w->intern));
	}else if(!strcmp(member_val, GWIDGET_NO_SHOW_ALL)){
		ZVAL_BOOL(rv, gtk_widget_get_no_show_all(w->intern));
	}else if(!strcmp(member_val, GWIDGET_SENSITIVE)){
		ZVAL_BOOL(rv, gtk_widget_get_sensitive(w->intern));
	}else if(!strcmp(member_val, GWIDGET_OPACITY)){
		ZVAL_DOUBLE(rv, gtk_widget_get_opacity(w->intern));
	}else
		return std_object_handlers.read_property(object, member, type, cache_slot, rv);
	return rv;
}

HashTable *gwidget_get_properties(zval *object){
	G_H_UPDATE_INIT(zend_std_get_properties(object));
	const char * tmp;
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	G_H_UPDATE_STRING(GWIDGET_NAME         , gtk_widget_get_name         (w->intern));
	G_H_UPDATE_LONG  (GWIDGET_HALIGN       , gtk_widget_get_halign       (w->intern));
	G_H_UPDATE_LONG  (GWIDGET_VALIGN       , gtk_widget_get_valign       (w->intern));
	G_H_UPDATE_LONG  (GWIDGET_MARGIN_START , gtk_widget_get_margin_start (w->intern));
	G_H_UPDATE_LONG  (GWIDGET_MARGIN_END   , gtk_widget_get_margin_end   (w->intern));
	G_H_UPDATE_LONG  (GWIDGET_MARGIN_TOP   , gtk_widget_get_margin_top   (w->intern));
	G_H_UPDATE_LONG  (GWIDGET_DIRECTION    , gtk_widget_get_direction    (w->intern));
	G_H_UPDATE_LONG  (GWIDGET_MARGIN_BOTTOM, gtk_widget_get_margin_bottom(w->intern));
	G_H_UPDATE_BOOL  (GWIDGET_HEXPAND      , gtk_widget_get_hexpand      (w->intern));
	G_H_UPDATE_BOOL  (GWIDGET_HEXPAND_SET  , gtk_widget_get_hexpand_set  (w->intern));
	G_H_UPDATE_BOOL  (GWIDGET_VEXPAND      , gtk_widget_get_vexpand      (w->intern));
	G_H_UPDATE_BOOL  (GWIDGET_VEXPAND_SET  , gtk_widget_get_vexpand_set  (w->intern));
	G_H_UPDATE_BOOL  (GWIDGET_VISIBLE      , gtk_widget_get_visible      (w->intern));
	G_H_UPDATE_BOOL  (GWIDGET_CAN_FOCUS    , gtk_widget_get_can_focus    (w->intern));
	G_H_UPDATE_BOOL  (GWIDGET_CAN_DEFAULT  , gtk_widget_get_can_default  (w->intern));
	G_H_UPDATE_BOOL  (GWIDGET_HAS_TOOLTIP  , gtk_widget_get_has_tooltip  (w->intern));
	G_H_UPDATE_BOOL  (GWIDGET_NO_SHOW_ALL  , gtk_widget_get_no_show_all  (w->intern));
	G_H_UPDATE_BOOL  (GWIDGET_SENSITIVE    , gtk_widget_get_sensitive    (w->intern));
	G_H_UPDATE_DOUBLE(GWIDGET_OPACITY      , gtk_widget_get_opacity      (w->intern));

	return G_H_UPDATE_RETURN;
}

void gwidget_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	zval * tmp_member;
	long tmp_l;
	const char * tmp_s;
	double tmp_d;
	int tmp_b;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	switch(Z_TYPE_P(value)){
		case IS_LONG :
			tmp_l = Z_LVAL_P(value);
			if(!strcmp(member_val, GWIDGET_HALIGN))
				switch(tmp_l){
					case GTK_ALIGN_FILL:
					case GTK_ALIGN_START :
					case GTK_ALIGN_END :
					case GTK_ALIGN_CENTER :
					case GTK_ALIGN_BASELINE :
						gtk_widget_set_halign(w->intern, tmp_l);
						break;
					default :
						zend_throw_exception_ex(pggi_exception_get(), 0, "Can't change the hAlign property with something not a ALIGN_* constant");
						break;
				}
			else if(!strcmp(member_val, GWIDGET_VALIGN))
				switch(tmp_l){
					case GTK_ALIGN_FILL:
					case GTK_ALIGN_START :
					case GTK_ALIGN_END :
					case GTK_ALIGN_CENTER :
					case GTK_ALIGN_BASELINE :
						gtk_widget_set_valign(w->intern, tmp_l);
						break;
					default :
						zend_throw_exception_ex(pggi_exception_get(), 0, "Can't change the vAlign property with something not a ALIGN_* constant");
						break;
				}
			else if(!strcmp(member_val, GWIDGET_MARGIN_START))
				gtk_widget_set_margin_start(w->intern, tmp_l);
			else if(!strcmp(member_val, GWIDGET_MARGIN_END))
				gtk_widget_set_margin_end(w->intern, tmp_l);
			else if(!strcmp(member_val, GWIDGET_MARGIN_TOP))
				gtk_widget_set_margin_top(w->intern, tmp_l);
			else if(!strcmp(member_val, GWIDGET_MARGIN_BOTTOM))
				gtk_widget_set_margin_bottom(w->intern, tmp_l);
			else if(!strcmp(member_val, GWIDGET_DIRECTION))
				switch(tmp_l){
					case GTK_DIR_TAB_FORWARD :
					case GTK_DIR_TAB_BACKWARD :
					case GTK_DIR_UP :
					case GTK_DIR_DOWN :
					case GTK_DIR_LEFT :
					case GTK_DIR_RIGHT :
						gtk_widget_set_direction(w->intern, tmp_l);
						break;
					default :
						zend_throw_exception_ex(pggi_exception_get(), 0, "Can't change the direction property with something not a DIR_* constant");
						break;
				}
			else
				std_object_handlers.write_property(object, member, value, cache_slot);
			break;
		case IS_FALSE :
		case IS_TRUE :
			tmp_b = Z_TYPE_P(value) == IS_TRUE ? 1 : 0;
			if(!strcmp(member_val, GWIDGET_HEXPAND))
				gtk_widget_set_hexpand(w->intern, tmp_b);
			else if(!strcmp(member_val, GWIDGET_HEXPAND_SET))
				gtk_widget_set_hexpand_set(w->intern, tmp_b);
			else if(!strcmp(member_val, GWIDGET_VEXPAND))
				gtk_widget_set_vexpand(w->intern, tmp_b);
			else if(!strcmp(member_val, GWIDGET_VEXPAND_SET))
				gtk_widget_set_vexpand_set(w->intern, tmp_b);
			else if(!strcmp(member_val, GWIDGET_VISIBLE))
				gtk_widget_set_visible(w->intern, tmp_b);
			else if(!strcmp(member_val, GWIDGET_CAN_FOCUS))
				gtk_widget_set_can_focus(w->intern, tmp_b);
			else if(!strcmp(member_val, GWIDGET_CAN_DEFAULT))
				gtk_widget_set_can_default(w->intern, tmp_b);
			else if(!strcmp(member_val, GWIDGET_HAS_TOOLTIP))
				gtk_widget_set_has_tooltip(w->intern, tmp_b);
			else if(!strcmp(member_val, GWIDGET_NO_SHOW_ALL))
				gtk_widget_set_no_show_all(w->intern, tmp_b);
			else if(!strcmp(member_val, GWIDGET_SENSITIVE))
				gtk_widget_set_sensitive(w->intern, tmp_b);
			else
				std_object_handlers.write_property(object, member, value, cache_slot);
			break;
		case IS_STRING :
			tmp_s = Z_STRVAL_P(value);
			if(!strcmp(Z_STRVAL_P(member), GWIDGET_NAME))
				gtk_widget_set_name(w->intern, tmp_s);
			else
				std_object_handlers.write_property(object, member, value, cache_slot);
			break;
		case IS_DOUBLE :
			tmp_d = Z_DVAL_P(value);
			if(!strcmp(member_val, GWIDGET_OPACITY)){
				if(tmp_d >= 0 && tmp_d <= 1)
					gtk_widget_set_opacity(w->intern, tmp_d);
				else
				zend_throw_exception_ex(pggi_exception_get(), 0, "the opacity property should be between 0 and 1");
			}else
				std_object_handlers.write_property(object, member, value, cache_slot);
			break;
		default:
			std_object_handlers.write_property(object, member, value, cache_slot);
	}
}

/********************************/
/* GWidget Class Initialization */
/********************************/

#define DECLARE_GWIDGET_PROP(name) \
DECLARE_CLASS_PROPERTY(gwidget_class_entry_ce, name)

#define GWIDGET_CONSTANT(name, value) \
zend_declare_class_constant_long(gwidget_class_entry_ce, name, sizeof(name)-1, value);


void gwidget_init(int module_number){
	zend_class_entry ce;
	le_gwidget = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "PGGI\\GWidget", module_number);

	memcpy(&gwidget_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	gwidget_object_handlers.offset         = XtOffsetOf(ze_gwidget_object, std);
	gwidget_object_handlers.free_obj       = gwidget_object_free_storage;
	gwidget_object_handlers.clone_obj      = NULL;
	gwidget_object_handlers.read_property  = gwidget_read_property;
	gwidget_object_handlers.get_properties = gwidget_get_properties;
	gwidget_object_handlers.write_property = gwidget_write_property;
	INIT_CLASS_ENTRY(ce, "PGGI\\GWidget", gwidget_class_functions);
	ce.create_object       = gwidget_object_new;
	gwidget_class_entry_ce = zend_register_internal_class(&ce);
	gwidget_class_entry_ce->ce_flags |= ZEND_ACC_ABSTRACT;

	GWIDGET_CONSTANT("ALIGN_FILL"    , GTK_ALIGN_FILL    );
	GWIDGET_CONSTANT("ALIGN_START"   , GTK_ALIGN_START   );
	GWIDGET_CONSTANT("ALIGN_END"     , GTK_ALIGN_END     );
	GWIDGET_CONSTANT("ALIGN_CENTER"  , GTK_ALIGN_CENTER  );
	GWIDGET_CONSTANT("ALIGN_BASELINE", GTK_ALIGN_BASELINE);

	// for reflection purpose only
	DECLARE_GWIDGET_PROP(GWIDGET_VALIGN       );
	DECLARE_GWIDGET_PROP(GWIDGET_HALIGN       );
	DECLARE_GWIDGET_PROP(GWIDGET_MARGIN_START );
	DECLARE_GWIDGET_PROP(GWIDGET_MARGIN_END   );
	DECLARE_GWIDGET_PROP(GWIDGET_MARGIN_TOP   );
	DECLARE_GWIDGET_PROP(GWIDGET_MARGIN_BOTTOM);
	DECLARE_GWIDGET_PROP(GWIDGET_HEXPAND      );
	DECLARE_GWIDGET_PROP(GWIDGET_HEXPAND_SET  );
	DECLARE_GWIDGET_PROP(GWIDGET_VEXPAND      );
	DECLARE_GWIDGET_PROP(GWIDGET_VEXPAND_SET  );
	DECLARE_GWIDGET_PROP(GWIDGET_OPACITY      );
	DECLARE_GWIDGET_PROP(GWIDGET_VISIBLE      );
	DECLARE_GWIDGET_PROP(GWIDGET_NO_SHOW_ALL  );
	DECLARE_GWIDGET_PROP(GWIDGET_DIRECTION    );
	DECLARE_GWIDGET_PROP(GWIDGET_SENSITIVE    );
	DECLARE_GWIDGET_PROP(GWIDGET_NAME         );
	DECLARE_GWIDGET_PROP(GWIDGET_CAN_DEFAULT  );
	DECLARE_GWIDGET_PROP(GWIDGET_CAN_FOCUS    );
}


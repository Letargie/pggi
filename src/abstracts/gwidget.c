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

#define CONTENT_FUNC_SIGNAL_GEVENT                                                                                           \
do{                                                                                                                          \
	zval args[3];                                                                                                            \
	zval state, keyval;                                                                                                      \
	ZVAL_OBJ(&args[2], gevent_ctor(gevent_key_get_class_entry(), event));                                                    \
	ZVAL_LONG(&state, event->key.state);                                                                                     \
	ZVAL_LONG(&keyval, event->key.keyval);                                                                                   \
	zend_update_property(gevent_key_get_class_entry(), &args[2], GEVENT_KEY_STATE, sizeof(GEVENT_KEY_STATE) - 1, &state);    \
	zend_update_property(gevent_key_get_class_entry(), &args[2], GEVENT_KEY_KEYVAL, sizeof(GEVENT_KEY_KEYVAL) - 1, &keyval); \
	gwidget_function(data, gsignal_gwidget_key_press_event, args, 3);                                                        \
	return FALSE;                                                                                                            \
}while(0)

int gwidget_func_draw(GtkWidget* w, cairo_t *cr, gpointer data){
	zval args[3];
	zend_object * tor = pc_context_object_new(pc_context_get_class_entry());
	ze_context_object * obj = php_context_fetch_object(tor);
	obj->std.handlers = pc_context_get_object_handlers();
	obj->context_ptr = pc_context_new();
	obj->context_ptr->intern = cr;
	ZVAL_OBJ(&args[2], tor);
	gwidget_function(data, gsignal_gwidget_draw, args, 3);
	return FALSE; // return gwidget function
}

int gwidget_func_key_press_event(GtkWidget* w, GdkEvent *event, gpointer data){
	CONTENT_FUNC_SIGNAL_GEVENT;
}

int gwidget_func_key_release_event(GtkWidget* w, GdkEvent *event, gpointer data){
	CONTENT_FUNC_SIGNAL_GEVENT;
}

int gwidget_func_button_press_event(GtkWidget* w, GdkEvent *event, gpointer data){
	CONTENT_FUNC_SIGNAL_GEVENT;
}

int gwidget_func_button_release_event(GtkWidget* w, GdkEvent *event, gpointer data){
	CONTENT_FUNC_SIGNAL_GEVENT;
}

void gwidget_adding_function(long val, char * name, GCallback f, ze_gwidget_object * ze_obj, zval * function, zval * param){
	zval * data, * narray;
	data = zend_hash_index_find(Z_ARRVAL_P(&ze_obj->widget_ptr->signals), val);
	zval * data_to_insert = ecalloc(1,sizeof(zval));
	array_init(data_to_insert);
	zend_hash_index_add(Z_ARRVAL_P(data_to_insert), INDEX_ON_FUNCTION_NAME, function);
	Z_TRY_ADDREF_P(function);
	if(param){
		zend_hash_index_add(Z_ARRVAL_P(data_to_insert), INDEX_ON_FUNCTION_PARAM, param);
		Z_TRY_ADDREF_P(param);
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
		case gsignal_gwidget_draw :
			gwidget_adding_function(val, GSIGNAL_GWIDGET_DRAW, G_CALLBACK(gwidget_func_draw), ze_obj, function, param);
			break;
		case gsignal_gwidget_key_press_event :
			gwidget_adding_function(val, GSIGNAL_GWIDGET_KEY_PRESS_EVENT, G_CALLBACK(gwidget_func_key_press_event), ze_obj, function, param);
			break;
		case gsignal_gwidget_key_release_event :
			gwidget_adding_function(val, GSIGNAL_GWIDGET_KEY_RELEASE_EVENT, G_CALLBACK(gwidget_func_key_release_event), ze_obj, function, param);
			break;
		case gsignal_gwidget_button_press_event :
			gwidget_adding_function(val, GSIGNAL_GWIDGET_BUTTON_PRESS_EVENT, G_CALLBACK(gwidget_func_button_press_event), ze_obj, function, param);
			break;
		case gsignal_gwidget_button_release_event :
			gwidget_adding_function(val, GSIGNAL_GWIDGET_BUTTON_RELEASE_EVENT, G_CALLBACK(gwidget_func_button_release_event), ze_obj, function, param);
			break;
		default :
			zend_throw_exception_ex(pggi_exception_get(), 0, "not handled signal");
	}
	
}

/***************/
/* PHP Methods */
/***************/

/*==========================================================================*/
/**
 * Connects a callback to a signal for a particular object. It will be called before the default handler of the signal.
 * @public
 *
 * @param int      $type             The type of signal. Has to be a valid PGGI\\SIGNAL_GWIDGET_*
 * @param Callable $function         The callback
 * @param mixed    $args (Optionnal) The parameters of the function
 */
GWIDGET_METHOD(on){
	zval * function, * this, * param = NULL;
	long val;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "lz|z", &val ,&function, &param) == FAILURE)
		return;
	this = getThis();
	gwidget_on(val, Z_GWIDGET_P(this), function, param);
}

/*==========================================================================*/
/**
 * Flags the widget to be displayed. Any widget that isn’t shown will not appear on the screen. 
 * If you want to show all the widgets in a container, it’s easier to call showAll() on the container, instead of individually showing the widgets.
 * Remember that you have to show the containers containing a widget, in addition to the widget itself, before it will appear onscreen.
 * When a toplevel container is shown, it is immediately realized and mapped; other shown widgets are realized and mapped when their toplevel container is realized and mapped.
 * @public
 */
GWIDGET_METHOD(show){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return ;
	ze_obj = Z_GWIDGET_P(self);
	gtk_widget_show(ze_obj->widget_ptr->intern);
}

/*==========================================================================*/
/**
 * Reverses the effects of show(), causing the widget to be hidden (invisible to the user).
 * @public
 */
GWIDGET_METHOD(hide){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return ;
	ze_obj = Z_GWIDGET_P(self);
	gtk_widget_hide(ze_obj->widget_ptr->intern);
}

/*==========================================================================*/
/**
 * Recursively shows a widget, and any child widgets (if the widget is a container).
 * @public
 */
GWIDGET_METHOD(showAll){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return ;
	ze_obj = Z_GWIDGET_P(self);
	gtk_widget_show_all(ze_obj->widget_ptr->intern);
}

/*==========================================================================*/
/**
 * Returns the style context associated to widget
 * @public
 *
 * @return \PGGI\GStyleContext
 */
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

/*==========================================================================*/
/**
 * Returns the width that has currently been allocated to widget. 
 * This function is intended to be used when implementing handlers for the PGGI_SIGNAL_GWIDGET_DRAW callback.
 * @public
 *
 * @return int
 */
GWIDGET_METHOD(getAllocatedWidth){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return ;
	ze_obj = Z_GWIDGET_P(self);
	RETURN_LONG(gtk_widget_get_allocated_width(ze_obj->widget_ptr->intern));
}

/*==========================================================================*/
/**
 * Returns the height that has currently been allocated to widget. 
 * This function is intended to be used when implementing handlers for the PGGI_SIGNAL_GWIDGET_DRAW callback.
 * @public
 *
 * @return int
 */
GWIDGET_METHOD(getAllocatedHeight){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return ;
	ze_obj = Z_GWIDGET_P(self);
	RETURN_LONG(gtk_widget_get_allocated_height(ze_obj->widget_ptr->intern));
}


/*==========================================================================*/
/**
 * Gets a Pango\Context with the appropriate font map, font description, and base direction for this widget.
 * This context is owned by the widget, and will be updated to match any changes to the widget’s attributes.
 * This can be tracked by using the PGGI_SIGNAL_GWIDGET_SCREEN_CHANGED (Not yet implemented) signal on the widget.
 * @public
 *
 * @return PGGI\Pango\Context
 */
GWIDGET_METHOD(getPangoContext){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return ;
	ze_obj = Z_GWIDGET_P(self);
	PangoContext * context = gtk_widget_get_pango_context(ze_obj->widget_ptr->intern);
	zend_object * obj = pp_context_object_new(pp_context_get_class_entry());
	ze_pp_context_object * tmp = php_pp_context_fetch_object(obj);
	tmp->context_ptr = pp_context_new();
	tmp->context_ptr->intern = context;
	RETURN_OBJ(obj);
}

/*==========================================================================*/
/**
 * Returns the widget’s window if it is realized, NULL otherwise
 * @public
 *
 * @return PGGI\Gdk\Window
 */
GWIDGET_METHOD(getWindow){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return ;
	ze_obj = Z_GWIDGET_P(self);
	GdkWindow * window = gtk_widget_get_window(ze_obj->widget_ptr->intern);
	zend_object * obj = gdk_gwindow_object_new(gdk_gwindow_get_class_entry());
	ze_gdk_gwindow_object * tmp = php_gdk_gwindow_fetch_object(obj);
	tmp->window_ptr = gdk_gwindow_new();
	tmp->window_ptr->intern = window;
	RETURN_OBJ(obj);
}

/*==========================================================================*/
/**
 * Invalidates the area of widget specified (for now it's always all the widget)
 * Once the main loop becomes idle (after the current batch of events has been processed, roughly),
 * the window will receive expose events for the union of all regions that have been invalidated.
 * Normally you would only use this function in widget implementations.
 * You might also use it to schedule a redraw of a GtkDrawingArea or some portion thereof.
 * @public
 */
GWIDGET_METHOD(queueDraw){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return ;
	ze_obj = Z_GWIDGET_P(self);
	gtk_widget_queue_draw(ze_obj->widget_ptr->intern);
}

/*==========================================================================*/
/**
 * Sets the minimum size of a widget; that is, the widget’s size request will be at least width by height.
 * You can use this function to force a widget to be larger than it normally would be.
 * In most cases, Window::setDefaultSize() is a better choice for toplevel windows than this function;
 * setting the default size will still allow users to shrink the window.
 * Setting the size request will force them to leave the window at least as large as the size request.
 * When dealing with window sizes, Window::setGeometryHints() (not implemented) can be a useful function as well.
 * Note the inherent danger of setting any fixed size - themes, translations into other languages, different fonts,
 * and user action can all change the appropriate size for a given widget. So, it's basically impossible to hardcode a size that will always be correct.
 * The size request of a widget is the smallest size a widget can accept while still functioning well and drawing itself correctly.
 * However in some strange cases a widget may be allocated less than its requested size, and in many cases a widget may be allocated more space than it requested.
 * If the size request in a given direction is -1 (unset), then the “natural” size request of the widget will be used instead.
 * The size request set here does not include any margin from the GtkWidget properties margin-left, margin-right, margin-top, and margin-bottom, 
 * but it does include pretty much all other padding or border properties set by any subclass of GtkWidget.
 * @public
 *
 * @param int $width
 * @param int $height
 */
GWIDGET_METHOD(setSizeRequest){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	long width, height;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "ll", &width, &height) == FAILURE)
		return;
	ze_obj = Z_GWIDGET_P(self);
	gtk_widget_set_size_request(ze_obj->widget_ptr->intern, width, height);
}

static const zend_function_entry gwidget_class_functions[] = {
	PHP_ME(GWidget, on                , arginfo_pggi_on                  , ZEND_ACC_PUBLIC)
	PHP_ME(GWidget, show              , arginfo_pggi_void                , ZEND_ACC_PUBLIC)
	PHP_ME(GWidget, hide              , arginfo_pggi_void                , ZEND_ACC_PUBLIC)
	PHP_ME(GWidget, showAll           , arginfo_pggi_void                , ZEND_ACC_PUBLIC)
	PHP_ME(GWidget, getStyleContext   , arginfo_gwidget_get_style_context, ZEND_ACC_PUBLIC)
	PHP_ME(GWidget, getPangoContext   , arginfo_gwidget_get_pango_context, ZEND_ACC_PUBLIC)
	PHP_ME(GWidget, getAllocatedWidth , arginfo_pggi_get_long            , ZEND_ACC_PUBLIC)
	PHP_ME(GWidget, getAllocatedHeight, arginfo_pggi_get_long            , ZEND_ACC_PUBLIC)
	PHP_ME(GWidget, queueDraw         , arginfo_pggi_void                , ZEND_ACC_PUBLIC)
	PHP_ME(GWidget, getWindow         , arginfo_pggi_get_window          , ZEND_ACC_PUBLIC)
	PHP_ME(GWidget, setSizeRequest    , arginfo_pggi_get_size_request    , ZEND_ACC_PUBLIC)
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

PHP_WRITE_PROP_HANDLER_TYPE gwidget_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	double tmp_d;
	long tmp_l;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);


	if(!strcmp(member_val, GWIDGET_HALIGN)){
		convert_to_long(value);
		tmp_l = Z_LVAL_P(value);
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
	}else if(!strcmp(member_val, GWIDGET_VALIGN)){
		convert_to_long(value);
		tmp_l = Z_LVAL_P(value);
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
	}else if(!strcmp(member_val, GWIDGET_MARGIN_START)){
		convert_to_long(value);
		gtk_widget_set_margin_start(w->intern, Z_LVAL_P(value));
	}else if(!strcmp(member_val, GWIDGET_MARGIN_END)){
		convert_to_long(value);
		gtk_widget_set_margin_end(w->intern, Z_LVAL_P(value));
	}else if(!strcmp(member_val, GWIDGET_MARGIN_TOP)){
		convert_to_long(value);
		gtk_widget_set_margin_top(w->intern, Z_LVAL_P(value));
	}else if(!strcmp(member_val, GWIDGET_MARGIN_BOTTOM)){
		convert_to_long(value);
		gtk_widget_set_margin_bottom(w->intern, Z_LVAL_P(value));
	}else if(!strcmp(member_val, GWIDGET_DIRECTION)){
		convert_to_long(value);
		tmp_l = Z_LVAL_P(value);
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
	}else if(!strcmp(member_val, GWIDGET_HEXPAND)){
		convert_to_boolean(value);
		gtk_widget_set_hexpand(w->intern, GET_BOOL_FROM_ZVAL(value));
	}else if(!strcmp(member_val, GWIDGET_HEXPAND_SET)){
		convert_to_boolean(value);
		gtk_widget_set_hexpand_set(w->intern, GET_BOOL_FROM_ZVAL(value));
	}else if(!strcmp(member_val, GWIDGET_VEXPAND)){
		convert_to_boolean(value);
		gtk_widget_set_vexpand(w->intern, GET_BOOL_FROM_ZVAL(value));
	}else if(!strcmp(member_val, GWIDGET_VEXPAND_SET)){
		convert_to_boolean(value);
		gtk_widget_set_vexpand_set(w->intern, GET_BOOL_FROM_ZVAL(value));
	}else if(!strcmp(member_val, GWIDGET_VISIBLE)){
		convert_to_boolean(value);
		gtk_widget_set_visible(w->intern, GET_BOOL_FROM_ZVAL(value));
	}else if(!strcmp(member_val, GWIDGET_CAN_FOCUS)){
		convert_to_boolean(value);
		gtk_widget_set_can_focus(w->intern, GET_BOOL_FROM_ZVAL(value));
	}else if(!strcmp(member_val, GWIDGET_CAN_DEFAULT)){
		convert_to_boolean(value);
		gtk_widget_set_can_default(w->intern, GET_BOOL_FROM_ZVAL(value));
	}else if(!strcmp(member_val, GWIDGET_HAS_TOOLTIP)){
		convert_to_boolean(value);
		gtk_widget_set_has_tooltip(w->intern, GET_BOOL_FROM_ZVAL(value));
	}else if(!strcmp(member_val, GWIDGET_NO_SHOW_ALL)){
		convert_to_boolean(value);
		gtk_widget_set_no_show_all(w->intern, GET_BOOL_FROM_ZVAL(value));
	}else if(!strcmp(member_val, GWIDGET_SENSITIVE)){
		convert_to_boolean(value);
		gtk_widget_set_sensitive(w->intern, GET_BOOL_FROM_ZVAL(value));
	}else if(!strcmp(Z_STRVAL_P(member), GWIDGET_NAME)){
		convert_to_string(value);
		gtk_widget_set_name(w->intern, Z_STRVAL_P(value));
	}else if(!strcmp(member_val, GWIDGET_OPACITY)){
		convert_to_double(value);
		tmp_d = Z_DVAL_P(value);
		if(!strcmp(member_val, GWIDGET_OPACITY)){
			if(tmp_d >= 0 && tmp_d <= 1)
				gtk_widget_set_opacity(w->intern, tmp_d);
			else
				zend_throw_exception_ex(pggi_exception_get(), 0, "the opacity property should be between 0 and 1");
		}
	}else{
		PHP_WRITE_PROP_HANDLER_RETURN(std_object_handlers.write_property(object, member, value, cache_slot));
	}
	PHP_WRITE_PROP_HANDLER_RETURN(value);
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


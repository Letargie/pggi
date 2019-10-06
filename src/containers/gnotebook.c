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


#include "gnotebook.h"

static int le_gnotebook;
static zend_object_handlers gnotebook_object_handlers;
static zend_class_entry * gnotebook_class_entry_ce;

zend_class_entry * gnotebook_get_class_entry(){
	return gnotebook_class_entry_ce;
}

zend_object_handlers * gnotebook_get_object_handlers(){
	return &gnotebook_object_handlers;
}

PHP_METHOD(GNotebook, __construct){
	ze_gwidget_object * widget;
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	widget = Z_GWIDGET_P(getThis());
	widget->std.handlers = &gnotebook_object_handlers;
	widget->widget_ptr = gwidget_new();
	widget->widget_ptr->intern = gtk_notebook_new();
	GCONTAINER_ADD_ELEMENT(widget);
	g_signal_connect(widget->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), widget);
}


PHP_METHOD(GNotebook, appendPage){
	ze_gwidget_object *ze_obj,
	                  * ze_child,
					  * ze_label,
					  * ze_menu;
	zval * self = getThis();
	zval * child,
	     * label = NULL,
		 * menu = NULL;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O|O",
	   &child , gwidget_get_class_entry(),
	   &label , gwidget_get_class_entry(),
	   &menu  , gwidget_get_class_entry()) == FAILURE)
		return;
	ze_child = Z_GWIDGET_P(child);
	ze_obj = Z_GWIDGET_P(self);
	
	GtkWidget * label_intern = NULL;

	if(menu){
		ze_menu = Z_GWIDGET_P(menu);
		if(label){
			ze_label = Z_GWIDGET_P(label);
			label_intern = ze_label->widget_ptr->intern;
			gcontainer_add_data(ze_obj->widget_ptr, label);
		}
		gtk_notebook_append_page_menu(GTK_NOTEBOOK(ze_obj->widget_ptr->intern),
									  ze_child->widget_ptr->intern,
									  label_intern,
									  ze_menu->widget_ptr->intern);
		gcontainer_add_data(ze_obj->widget_ptr, menu);
	}else{
		if(label){
			ze_label = Z_GWIDGET_P(label);
			label_intern = ze_label->widget_ptr->intern;
			gcontainer_add_data(ze_obj->widget_ptr, label);
		}
		gtk_notebook_append_page(GTK_NOTEBOOK(ze_obj->widget_ptr->intern),
								 ze_child->widget_ptr->intern,
								 label_intern);
	}
	gcontainer_add_data(ze_obj->widget_ptr, child);
}


PHP_METHOD(GNotebook, prependPage){
	ze_gwidget_object *ze_obj,
	                  * ze_child,
					  * ze_label,
					  * ze_menu;
	zval * self = getThis();
	zval * child,
	     * label = NULL,
		 * menu = NULL;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O|O",
	   &child , gwidget_get_class_entry(),
	   &label , gwidget_get_class_entry(),
	   &menu  , gwidget_get_class_entry()) == FAILURE)
		return;
	ze_child = Z_GWIDGET_P(child);
	ze_obj = Z_GWIDGET_P(self);
	
	GtkWidget * label_intern = NULL;
	
	if(menu){
		ze_menu = Z_GWIDGET_P(menu);
		if(label){
			ze_label = Z_GWIDGET_P(label);
			label_intern = ze_label->widget_ptr->intern;
			gcontainer_add_data(ze_obj->widget_ptr, label);
		}
		gtk_notebook_prepend_page_menu(GTK_NOTEBOOK(ze_obj->widget_ptr->intern),
									  ze_child->widget_ptr->intern,
									  label_intern,
									  ze_menu->widget_ptr->intern);
		gcontainer_add_data(ze_obj->widget_ptr, menu);
	}else{
		if(label){
			ze_label = Z_GWIDGET_P(label);
			label_intern = ze_label->widget_ptr->intern;
			gcontainer_add_data(ze_obj->widget_ptr, label);
		}
		gtk_notebook_prepend_page(GTK_NOTEBOOK(ze_obj->widget_ptr->intern),
								 ze_child->widget_ptr->intern,
								 label_intern);
	}
	gcontainer_add_data(ze_obj->widget_ptr, child);
}

PHP_METHOD(GNotebook, insertPage){
	ze_gwidget_object *ze_obj,
	                  * ze_child,
					  * ze_label,
					  * ze_menu;
	zval * self = getThis();
	zval * child,
	     * label = NULL,
		 * menu = NULL;
	long position;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "Ol|O",
	   &child , gwidget_get_class_entry(),
	   &position,
	   &label , gwidget_get_class_entry(),
	   &menu  , gwidget_get_class_entry()) == FAILURE)
		return;
	ze_child = Z_GWIDGET_P(child);
	ze_obj = Z_GWIDGET_P(self);
	
	GtkWidget * label_intern = NULL;

	if(menu){
		ze_menu = Z_GWIDGET_P(menu);
		if(label){
			ze_label = Z_GWIDGET_P(label);
			label_intern = ze_label->widget_ptr->intern;
			gcontainer_add_data(ze_obj->widget_ptr, label);
		}
		gtk_notebook_insert_page_menu(GTK_NOTEBOOK(ze_obj->widget_ptr->intern),
									  ze_child->widget_ptr->intern,
									  label_intern,
									  ze_menu->widget_ptr->intern,
									  position);
		gcontainer_add_data(ze_obj->widget_ptr, menu);
	}else{
		if(label){
			ze_label = Z_GWIDGET_P(label);
			label_intern = ze_label->widget_ptr->intern;
			gcontainer_add_data(ze_obj->widget_ptr, label);
		}
		gtk_notebook_insert_page(GTK_NOTEBOOK(ze_obj->widget_ptr->intern),
								 ze_child->widget_ptr->intern,
								 label_intern,
								 position);
	}
	gcontainer_add_data(ze_obj->widget_ptr, child);
}


PHP_METHOD(GNotebook, removePage){
	ze_gwidget_object *ze_obj;
	zval * self = getThis();
	long pageNum;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &pageNum) == FAILURE)
		return;
	ze_obj = Z_GWIDGET_P(self);
	gtk_notebook_remove_page(GTK_NOTEBOOK(ze_obj->widget_ptr->intern), pageNum);
}

PHP_METHOD(GNotebook, detachTab){
	ze_gwidget_object *ze_obj, *ze_child;
	zval * self = getThis();
	zval * child;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &child, gwidget_get_class_entry()) == FAILURE)
		return;
	ze_obj = Z_GWIDGET_P(self);
	ze_child = Z_GWIDGET_P(child);
	gtk_notebook_detach_tab(GTK_NOTEBOOK(ze_obj->widget_ptr->intern), ze_child->widget_ptr->intern);
}

PHP_METHOD(GNotebook, pageNum){
	ze_gwidget_object *ze_obj, *ze_child;
	zval * self = getThis();
	zval * child;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &child, gwidget_get_class_entry()) == FAILURE)
		return;
	ze_obj = Z_GWIDGET_P(self);
	ze_child = Z_GWIDGET_P(child);
	RETURN_LONG(gtk_notebook_page_num(GTK_NOTEBOOK(ze_obj->widget_ptr->intern), ze_child->widget_ptr->intern));
}

PHP_METHOD(GNotebook, nextPage){
	ze_gwidget_object *ze_obj;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_GWIDGET_P(self);
	gtk_notebook_next_page(GTK_NOTEBOOK(ze_obj->widget_ptr->intern));
}

PHP_METHOD(GNotebook, prevPage){
	ze_gwidget_object *ze_obj;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_GWIDGET_P(self);
	gtk_notebook_prev_page(GTK_NOTEBOOK(ze_obj->widget_ptr->intern));
}

PHP_METHOD(GNotebook, reorderChild){
	ze_gwidget_object *ze_obj, *ze_child;
	zval * self = getThis();
	zval * child;
	long position;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "Ol", &child, gwidget_get_class_entry(), &position) == FAILURE)
		return;
	ze_obj = Z_GWIDGET_P(self);
	ze_child = Z_GWIDGET_P(child);
	gtk_notebook_reorder_child(GTK_NOTEBOOK(ze_obj->widget_ptr->intern), ze_child->widget_ptr->intern, position);
}

PHP_METHOD(GNotebook, getCurrentPage){
	ze_gwidget_object *ze_obj;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_GWIDGET_P(self);
	RETURN_LONG(gtk_notebook_get_current_page(GTK_NOTEBOOK(ze_obj->widget_ptr->intern)));
}

PHP_METHOD(GNotebook, getPageCount){
	ze_gwidget_object *ze_obj;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_GWIDGET_P(self);
	RETURN_LONG(gtk_notebook_get_n_pages(GTK_NOTEBOOK(ze_obj->widget_ptr->intern)));
}

static const zend_function_entry gnotebook_class_functions[] = {
	PHP_ME(GNotebook, __construct   , arginfo_pggi_void              , ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(GNotebook, appendPage    , arginfo_gnotebook_pend_page    , ZEND_ACC_PUBLIC)
	PHP_ME(GNotebook, prependPage   , arginfo_gnotebook_pend_page    , ZEND_ACC_PUBLIC)
	PHP_ME(GNotebook, insertPage    , arginfo_gnotebook_insert_page  , ZEND_ACC_PUBLIC)
	PHP_ME(GNotebook, removePage    , arginfo_pggi_set_long          , ZEND_ACC_PUBLIC)
	PHP_ME(GNotebook, detachTab     , arginfo_gnotebook_detach_tab   , ZEND_ACC_PUBLIC)
	PHP_ME(GNotebook, pageNum       , arginfo_gnotebook_page_num     , ZEND_ACC_PUBLIC)
	PHP_ME(GNotebook, nextPage      , arginfo_pggi_void              , ZEND_ACC_PUBLIC)
	PHP_ME(GNotebook, prevPage      , arginfo_pggi_void              , ZEND_ACC_PUBLIC)
	PHP_ME(GNotebook, reorderChild  , arginfo_gnotebook_reorder_child, ZEND_ACC_PUBLIC)
	PHP_ME(GNotebook, getPageCount  , arginfo_pggi_get_long          , ZEND_ACC_PUBLIC)
	PHP_FE_END
};



zval *gnotebook_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	if(!strcmp(member_val, GNOTEBOOK_PAGE)){
		ZVAL_LONG(rv, gtk_notebook_get_current_page(GTK_NOTEBOOK(w->intern)));
	}else if(!strcmp(member_val, GNOTEBOOK_TAB_POS)){
		ZVAL_LONG(rv, gtk_notebook_get_tab_pos(GTK_NOTEBOOK(w->intern)));
	}else if(!strcmp(member_val,GNOTEBOOK_ENABLE_POPUP )){
		GValue tmp = G_VALUE_INIT;
		g_value_init (&tmp, G_TYPE_BOOLEAN);
		g_object_get_property(G_OBJECT(w->intern), GNOTEBOOK_ENABLE_POPUP, &tmp);
		ZVAL_BOOL(rv, g_value_get_boolean(&tmp));
	}else if(!strcmp(member_val, GNOTEBOOK_SCROLLABLE)){
		ZVAL_BOOL(rv, gtk_notebook_get_scrollable(GTK_NOTEBOOK(w->intern)));
	}else if(!strcmp(member_val, GNOTEBOOK_SHOW_BORDER)){
		ZVAL_BOOL(rv, gtk_notebook_get_show_border(GTK_NOTEBOOK(w->intern)));
	}else if(!strcmp(member_val, GNOTEBOOK_SHOW_TABS)){
		ZVAL_BOOL(rv, gtk_notebook_get_show_tabs(GTK_NOTEBOOK(w->intern)));
	}else if(!strcmp(member_val, GNOTEBOOK_GROUP_NAME)){
		const char * tmp = gtk_notebook_get_group_name(GTK_NOTEBOOK(w->intern));
		ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
	}else
		return gcontainer_read_property(object, member, type, cache_slot, rv);
	return rv;
}

HashTable *gnotebook_get_properties(zval *object){
	G_H_UPDATE_INIT(gcontainer_get_properties(object));
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	GValue tmp = G_VALUE_INIT;
	g_value_init (&tmp, G_TYPE_BOOLEAN);
	g_object_get_property(G_OBJECT(w->intern), GNOTEBOOK_ENABLE_POPUP, &tmp);
	G_H_UPDATE_LONG  (GNOTEBOOK_PAGE        , gtk_notebook_get_current_page(GTK_NOTEBOOK(w->intern)));
	G_H_UPDATE_LONG  (GNOTEBOOK_TAB_POS     , gtk_notebook_get_tab_pos     (GTK_NOTEBOOK(w->intern)));
	G_H_UPDATE_BOOL  (GNOTEBOOK_ENABLE_POPUP, g_value_get_boolean(&tmp)                             );
	G_H_UPDATE_BOOL  (GNOTEBOOK_SCROLLABLE  , gtk_notebook_get_scrollable  (GTK_NOTEBOOK(w->intern)));
	G_H_UPDATE_BOOL  (GNOTEBOOK_SHOW_BORDER , gtk_notebook_get_show_border (GTK_NOTEBOOK(w->intern)));
	G_H_UPDATE_BOOL  (GNOTEBOOK_SHOW_TABS   , gtk_notebook_get_show_tabs   (GTK_NOTEBOOK(w->intern)));
	G_H_UPDATE_STRING(GNOTEBOOK_GROUP_NAME  , gtk_notebook_get_group_name  (GTK_NOTEBOOK(w->intern)));

	return G_H_UPDATE_RETURN;
}

PHP_WRITE_PROP_HANDLER_TYPE gnotebook_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	long tmp_l;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	if(!strcmp(member_val, GNOTEBOOK_GROUP_NAME))
		gtk_notebook_set_group_name(GTK_NOTEBOOK(w->intern), Z_STRVAL_P(value));
	
	else if(!strcmp(member_val, GNOTEBOOK_PAGE)){
		convert_to_long(value);
		tmp_l = Z_LVAL_P(value);
		gtk_notebook_set_current_page(GTK_NOTEBOOK(w->intern), tmp_l);
	}else if(!strcmp(member_val, GNOTEBOOK_TAB_POS)){
		convert_to_long(value);
		tmp_l = Z_LVAL_P(value);
		switch(tmp_l){
			case GTK_POS_LEFT   :
			case GTK_POS_RIGHT  :
			case GTK_POS_TOP    :
			case GTK_POS_BOTTOM :
				gtk_notebook_set_tab_pos(GTK_NOTEBOOK(w->intern), tmp_l);
				break;
			default:
				zend_throw_exception_ex(pggi_exception_get(), 0, "the tabPos need to be a POS_*");
				break;
		}
	}else if(!strcmp(member_val, GNOTEBOOK_ENABLE_POPUP)){
		convert_to_boolean(value);
		if(GET_BOOL_FROM_ZVAL(value)){
			gtk_notebook_popup_enable(GTK_NOTEBOOK(w->intern));
		}else{
			gtk_notebook_popup_disable(GTK_NOTEBOOK(w->intern));
		}
	}else if(!strcmp(member_val, GNOTEBOOK_SCROLLABLE)){
		convert_to_boolean(value);
		gtk_notebook_set_scrollable(GTK_NOTEBOOK(w->intern), GET_BOOL_FROM_ZVAL(value));
	}else if(!strcmp(member_val, GNOTEBOOK_SHOW_BORDER)){
		convert_to_boolean(value);
		gtk_notebook_set_show_border(GTK_NOTEBOOK(w->intern), GET_BOOL_FROM_ZVAL(value));
	}else if(!strcmp(member_val, GNOTEBOOK_SHOW_TABS)){
		convert_to_boolean(value);
		gtk_notebook_set_show_tabs(GTK_NOTEBOOK(w->intern), GET_BOOL_FROM_ZVAL(value));
	}else
		PHP_WRITE_PROP_HANDLER_RETURN(gcontainer_write_property(object, member, value, cache_slot));
	PHP_WRITE_PROP_HANDLER_RETURN(value);
}

#define DECLARE_GNOTEBOOK_PROP(name) \
DECLARE_CLASS_PROPERTY(gnotebook_class_entry_ce, name)

void gnotebook_init(int module_number){
	zend_class_entry ce;
	le_gnotebook = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "PGGI\\GNotebook", module_number);

	memcpy(&gnotebook_object_handlers, gcontainer_get_object_handlers(), sizeof(zend_object_handlers));
	gnotebook_object_handlers.read_property  = gnotebook_read_property;
	gnotebook_object_handlers.get_properties = gnotebook_get_properties;
	gnotebook_object_handlers.write_property = gnotebook_write_property;	
	INIT_CLASS_ENTRY(ce, "PGGI\\GNotebook", gnotebook_class_functions);
	ce.create_object 	= gwidget_object_new;
	gnotebook_class_entry_ce	= zend_register_internal_class_ex(&ce, gcontainer_get_class_entry());

	DECLARE_GNOTEBOOK_PROP(GNOTEBOOK_ENABLE_POPUP);
	DECLARE_GNOTEBOOK_PROP(GNOTEBOOK_GROUP_NAME  );
	DECLARE_GNOTEBOOK_PROP(GNOTEBOOK_PAGE        );
	DECLARE_GNOTEBOOK_PROP(GNOTEBOOK_SCROLLABLE  );
	DECLARE_GNOTEBOOK_PROP(GNOTEBOOK_SHOW_BORDER );
	DECLARE_GNOTEBOOK_PROP(GNOTEBOOK_SHOW_TABS   );
	DECLARE_GNOTEBOOK_PROP(GNOTEBOOK_TAB_POS     );
}



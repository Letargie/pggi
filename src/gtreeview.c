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


#include "gtreeview.h"

static int le_gtree_view;
static zend_object_handlers gtree_view_object_handlers;
static zend_class_entry * gtree_view_class_entry_ce;

/***************/
/* PHP Methods */
/***************/

GTREE_VIEW_METHOD(__construct){
	ze_gwidget_object * widget = Z_GWIDGET_P(getThis());
	widget->std.handlers = &gtree_view_object_handlers;
	widget->widget_ptr = gwidget_new();
	widget->widget_ptr->intern = gtk_tree_view_new();
	GCONTAINER_ADD_ELEMENT(widget);
	g_signal_connect(widget->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), widget);
	/*GtkCellRenderer     * renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (widget->widget_ptr->intern),
                                               -1,      
                                               "Name",  
                                               renderer,
                                               "text", 0,
                                               NULL);
*/
}

// on method

void gtree_view_func_column_changed(GtkWidget * container, gpointer data){
	gwidget_function(data, gsignal_gtree_view_column_changed);
}

void gtree_view_on(long val,ze_gwidget_object * ze_obj, zval * function, zval * param){
	zval * data, * narray;
	switch(val){
		case gsignal_gtree_view_column_changed :
			gwidget_adding_function(val, GSIGNAL_GTREE_VIEW_COLUMN_CHANGED, gtree_view_func_column_changed, ze_obj, function, param);
			break;
		default :
			gcontainer_on(val, ze_obj, function, param);
			return ;
	}
}

GTREE_VIEW_METHOD(on){
	zval * function, * this, *param = NULL;
	long val;
	ze_gwidget_object *ze_obj = NULL;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "lz|z" ,&val ,&function, &param) == FAILURE) {
        RETURN_NULL();
    }
	if(!zend_is_callable(function, 0, NULL))
		zend_error(E_ERROR, "Function requires callable argument");
	this = getThis();
	if(!this)
		RETURN_NULL();
	ze_obj = Z_GWIDGET_P(this);
	gtree_view_on(val, ze_obj, function, param);
}

GTREE_VIEW_METHOD(scrollToPoint){
	zval * self = getThis();
	int x, y;
    if(pggi_parse_method_parameters_throw(ZEND_NUM_ARGS(), self, "ll" ,&x, &y) == FAILURE)
		return;
    ze_gwidget_object * ze_obj = Z_GWIDGET_P(self);
    gtk_tree_view_scroll_to_point(GTK_TREE_VIEW(ze_obj->widget_ptr->intern), x, y);
}

GTREE_VIEW_METHOD(expandAll){
	zval * self = getThis();
    if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
    ze_gwidget_object * ze_obj = Z_GWIDGET_P(self);
    gtk_tree_view_expand_all(GTK_TREE_VIEW(ze_obj->widget_ptr->intern));
}

GTREE_VIEW_METHOD(collapseAll){
	zval * self = getThis();
    if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
    ze_gwidget_object * ze_obj = Z_GWIDGET_P(self);
    gtk_tree_view_collapse_all(GTK_TREE_VIEW(ze_obj->widget_ptr->intern));
}

GTREE_VIEW_METHOD(appendColumn){
	zval * self = getThis();
	zval * column;
    if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "z", &column) == FAILURE)
		return;
    ze_gwidget_object * ze_obj = Z_GWIDGET_P(self);
	ze_gtree_view_column_object * ze_col = Z_GTREE_VIEW_COLUMN_P(column);
    gtk_tree_view_append_column(GTK_TREE_VIEW(ze_obj->widget_ptr->intern), ze_col->tree_view_column_ptr->intern);
}

GTREE_VIEW_METHOD(removeColumn){
	zval * self = getThis();
	zval * column;
    if(pggi_parse_method_parameters_throw(ZEND_NUM_ARGS(), self, "z", &column) == FAILURE)
		return;
    ze_gwidget_object * ze_obj = Z_GWIDGET_P(self);
	ze_gtree_view_column_object * ze_col = Z_GTREE_VIEW_COLUMN_P(column);
    gtk_tree_view_append_column(GTK_TREE_VIEW(ze_obj->widget_ptr->intern), ze_col->tree_view_column_ptr->intern);
}

/**
 * List of GTreeView functions and methods with their arguments
 */
static const zend_function_entry gtree_view_class_functions[] = {
	PHP_ME(GTreeView, __construct , arginfo_pggi_void      , ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(GTreeView, on          , arginfo_pggi_on        , ZEND_ACC_PUBLIC)
	PHP_ME(GTreeView, expandAll   , arginfo_pggi_void      , ZEND_ACC_PUBLIC)
	PHP_ME(GTreeView, collapseAll , arginfo_pggi_void      , ZEND_ACC_PUBLIC)
	PHP_ME(GTreeView, appendColumn, arginfo_gtree_view_pend, ZEND_ACC_PUBLIC)
	PHP_ME(GTreeView, removeColumn, arginfo_gtree_view_pend, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *gtree_view_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	zval zobj;
	zend_long lval;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkTreeView * view = GTK_TREE_VIEW(w->intern);
	if(!strcmp(member_val, GTREE_VIEW_ENABLE_GRID_LINES)){
		ZVAL_LONG(rv, gtk_tree_view_get_grid_lines(view));
	}else if(!strcmp(member_val, GTREE_VIEW_LEVEL_INDENTATION)){
		ZVAL_LONG(rv, gtk_tree_view_get_level_indentation(view));
	}else if(!strcmp(member_val, GTREE_VIEW_SEARCH_COLUMN)){
		ZVAL_LONG(rv, gtk_tree_view_get_search_column(view));
	}else if(!strcmp(member_val, GTREE_VIEW_TOOLTIP_COLUMN)){
		ZVAL_LONG(rv, gtk_tree_view_get_tooltip_column(view));
	}else if(!strcmp(member_val, GTREE_VIEW_ACTIVATE_ON_SINGLE_CLICK)){
		ZVAL_BOOL(rv, gtk_tree_view_get_activate_on_single_click(view));
	}else if(!strcmp(member_val, GTREE_VIEW_ENABLE_SEARCH)){
		ZVAL_BOOL(rv, gtk_tree_view_get_enable_search(view));
	}else if(!strcmp(member_val, GTREE_VIEW_ENABLE_TREE_LINES)){
		ZVAL_BOOL(rv, gtk_tree_view_get_enable_tree_lines(view));
	}else if(!strcmp(member_val, GTREE_VIEW_FIXED_HEIGHT_MODE)){
		ZVAL_BOOL(rv, gtk_tree_view_get_fixed_height_mode(view));
	}else if(!strcmp(member_val, GTREE_VIEW_HEADERS_CLICKABLE)){
		ZVAL_BOOL(rv, gtk_tree_view_get_headers_clickable(view));
	}else if(!strcmp(member_val, GTREE_VIEW_HEADERS_VISIBLE)){
		ZVAL_BOOL(rv, gtk_tree_view_get_headers_visible(view));
	}else if(!strcmp(member_val, GTREE_VIEW_HOVER_EXPAND)){
		ZVAL_BOOL(rv, gtk_tree_view_get_hover_expand(view));
	}else if(!strcmp(member_val, GTREE_VIEW_REORDERABLE)){
		ZVAL_BOOL(rv, gtk_tree_view_get_reorderable(view));
	}else if(!strcmp(member_val, GTREE_VIEW_RUBBER_BANDING)){
		ZVAL_BOOL(rv, gtk_tree_view_get_rubber_banding(view));
	}else if(!strcmp(member_val, GTREE_VIEW_SHOW_EXPANDERS)){
		ZVAL_BOOL(rv, gtk_tree_view_get_show_expanders(view));
	}else if(!strcmp(member_val, GTREE_VIEW_HOVER_SELECTION)){
		ZVAL_BOOL(rv, gtk_tree_view_get_hover_selection(view));
	}else if(!strcmp(member_val, GTREE_VIEW_MODEL)){
		return std_object_handlers.read_property(object, member, type, cache_slot, rv);
	}else
		return gcontainer_read_property(object, member, type, cache_slot, rv);
}

HashTable *gtree_view_get_properties(zval *object){
	G_H_UPDATE_INIT(gcontainer_get_properties(object));
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	GtkTreeView * view = GTK_TREE_VIEW(w->intern);
	G_H_UPDATE_BOOL  (GTREE_VIEW_ACTIVATE_ON_SINGLE_CLICK, gtk_tree_view_get_activate_on_single_click(view));
	G_H_UPDATE_BOOL  (GTREE_VIEW_ENABLE_SEARCH           , gtk_tree_view_get_enable_search           (view));
	G_H_UPDATE_BOOL  (GTREE_VIEW_ENABLE_TREE_LINES       , gtk_tree_view_get_enable_tree_lines       (view));
	G_H_UPDATE_BOOL  (GTREE_VIEW_FIXED_HEIGHT_MODE       , gtk_tree_view_get_fixed_height_mode       (view));
	G_H_UPDATE_BOOL  (GTREE_VIEW_HEADERS_CLICKABLE       , gtk_tree_view_get_headers_clickable       (view));
	G_H_UPDATE_BOOL  (GTREE_VIEW_HEADERS_VISIBLE         , gtk_tree_view_get_headers_visible         (view));
	G_H_UPDATE_BOOL  (GTREE_VIEW_HOVER_EXPAND            , gtk_tree_view_get_hover_expand            (view));
	G_H_UPDATE_BOOL  (GTREE_VIEW_HOVER_SELECTION         , gtk_tree_view_get_hover_selection         (view));
	G_H_UPDATE_BOOL  (GTREE_VIEW_REORDERABLE             , gtk_tree_view_get_reorderable             (view));
	G_H_UPDATE_BOOL  (GTREE_VIEW_RUBBER_BANDING          , gtk_tree_view_get_rubber_banding          (view));
	G_H_UPDATE_BOOL  (GTREE_VIEW_SHOW_EXPANDERS          , gtk_tree_view_get_show_expanders          (view));
	G_H_UPDATE_LONG  (GTREE_VIEW_ENABLE_GRID_LINES       , gtk_tree_view_get_grid_lines              (view));
	G_H_UPDATE_LONG  (GTREE_VIEW_LEVEL_INDENTATION       , gtk_tree_view_get_level_indentation       (view));
	G_H_UPDATE_LONG  (GTREE_VIEW_SEARCH_COLUMN           , gtk_tree_view_get_search_column           (view));
	G_H_UPDATE_LONG  (GTREE_VIEW_TOOLTIP_COLUMN          , gtk_tree_view_get_tooltip_column          (view));

	return G_H_UPDATE_RETURN;
}

void gtree_view_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	zval * tmp_member;
	long tmp_l;
	int tmp_b;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkTreeView * view = GTK_TREE_VIEW(w->intern);
	switch(Z_TYPE_P(value)){
		case IS_LONG :
			tmp_l = Z_LVAL_P(value);
			if(!strcmp(member_val, GTREE_VIEW_ENABLE_GRID_LINES)){
				gtk_tree_view_set_grid_lines(view, tmp_l);
			}else if(!strcmp(member_val, GTREE_VIEW_LEVEL_INDENTATION)){
				gtk_tree_view_set_level_indentation(view, tmp_l);
			}else if(!strcmp(member_val, GTREE_VIEW_SEARCH_COLUMN)){
				gtk_tree_view_set_search_column(view, tmp_l);
			}else if(!strcmp(member_val, GTREE_VIEW_TOOLTIP_COLUMN)){
				gtk_tree_view_set_tooltip_column(view, tmp_l);
			}else
				gcontainer_write_property(object, member, value, cache_slot);
			break;
		case IS_FALSE :
		case IS_TRUE :
			tmp_b = Z_TYPE_P(value) == IS_TRUE ? 1 : 0;
			if(!strcmp(member_val, GTREE_VIEW_ACTIVATE_ON_SINGLE_CLICK)){
				gtk_tree_view_set_activate_on_single_click(view, tmp_b);
			}else if(!strcmp(member_val, GTREE_VIEW_ENABLE_SEARCH)){
				gtk_tree_view_set_enable_search(view, tmp_b);
			}else if(!strcmp(member_val, GTREE_VIEW_ENABLE_TREE_LINES)){
				gtk_tree_view_set_enable_tree_lines(view, tmp_b);
			}else if(!strcmp(member_val, GTREE_VIEW_FIXED_HEIGHT_MODE)){
				gtk_tree_view_set_fixed_height_mode(view, tmp_b);
			}else if(!strcmp(member_val, GTREE_VIEW_HEADERS_CLICKABLE)){
				gtk_tree_view_set_headers_clickable(view, tmp_b);
			}else if(!strcmp(member_val, GTREE_VIEW_HEADERS_VISIBLE)){
				gtk_tree_view_set_headers_visible(view, tmp_b);
			}else if(!strcmp(member_val, GTREE_VIEW_HOVER_EXPAND)){
				gtk_tree_view_set_hover_expand(view, tmp_b);
			}else if(!strcmp(member_val, GTREE_VIEW_REORDERABLE)){
				gtk_tree_view_set_reorderable(view, tmp_b);
			}else if(!strcmp(member_val, GTREE_VIEW_RUBBER_BANDING)){
				gtk_tree_view_set_rubber_banding(view, tmp_b);
			}else if(!strcmp(member_val, GTREE_VIEW_SHOW_EXPANDERS)){
				gtk_tree_view_set_show_expanders(view, tmp_b);
			}else if(!strcmp(member_val, GTREE_VIEW_HOVER_SELECTION)){
				gtk_tree_view_set_hover_selection(view, tmp_b);
			}else
				gcontainer_write_property(object, member, value, cache_slot);
			break;
		case IS_OBJECT :
				if(!strcmp(member_val, GTREE_VIEW_MODEL)){
					ze_gtree_store_object * tmp_model = Z_GTREE_STORE_P(value);
					if(!tmp_model){
						zend_throw_exception_ex(pggi_exception_get(), 0, "that need to be a widget");
						return ;
					}
					gtree_store_ptr w = tmp_model->tree_store_ptr;
					std_object_handlers.write_property(object, member, value, cache_slot);
					gtk_tree_view_set_model(view, GTK_TREE_MODEL(w->intern));
				}else
					gcontainer_write_property(object, member, value, cache_slot);
			break;
		default :
			gcontainer_write_property(object, member, value, cache_slot);
	}
}

/**********************************/
/* GTreeView class initialisation */
/**********************************/

#define DECLARE_GTREE_VIEW_PROP(name) \
DECLARE_CLASS_PROPERTY(gtree_view_class_entry_ce, name)

void gtree_view_init(int module_number){
	DECLARE_CLASS_PROPERTY_INIT();
	le_gtree_view = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "PGGI\\GTreeView", module_number);

	memcpy(&gtree_view_object_handlers, gcontainer_get_object_handlers(), sizeof(zend_object_handlers));
	gtree_view_object_handlers.read_property  = gtree_view_read_property;
	gtree_view_object_handlers.get_properties = gtree_view_get_properties;
	gtree_view_object_handlers.write_property = gtree_view_write_property;

	INIT_CLASS_ENTRY(ce, "PGGI\\GTreeView", gtree_view_class_functions);
	gtree_view_class_entry_ce = zend_register_internal_class_ex(&ce, gcontainer_get_class_entry());
	DECLARE_GTREE_VIEW_PROP(GTREE_VIEW_ACTIVATE_ON_SINGLE_CLICK);
	DECLARE_GTREE_VIEW_PROP(GTREE_VIEW_ENABLE_GRID_LINES       );
	DECLARE_GTREE_VIEW_PROP(GTREE_VIEW_ENABLE_SEARCH           );
	DECLARE_GTREE_VIEW_PROP(GTREE_VIEW_ENABLE_TREE_LINES       );
	DECLARE_GTREE_VIEW_PROP(GTREE_VIEW_FIXED_HEIGHT_MODE       );
	DECLARE_GTREE_VIEW_PROP(GTREE_VIEW_HEADERS_CLICKABLE       );
	DECLARE_GTREE_VIEW_PROP(GTREE_VIEW_HEADERS_VISIBLE         );
	DECLARE_GTREE_VIEW_PROP(GTREE_VIEW_HOVER_EXPAND            );
	DECLARE_GTREE_VIEW_PROP(GTREE_VIEW_HOVER_SELECTION         );
	DECLARE_GTREE_VIEW_PROP(GTREE_VIEW_LEVEL_INDENTATION       );
	DECLARE_GTREE_VIEW_PROP(GTREE_VIEW_MODEL                   );
	DECLARE_GTREE_VIEW_PROP(GTREE_VIEW_REORDERABLE             );
	DECLARE_GTREE_VIEW_PROP(GTREE_VIEW_RUBBER_BANDING          );
	DECLARE_GTREE_VIEW_PROP(GTREE_VIEW_SEARCH_COLUMN           );
	DECLARE_GTREE_VIEW_PROP(GTREE_VIEW_SHOW_EXPANDERS          );
	DECLARE_GTREE_VIEW_PROP(GTREE_VIEW_TOOLTIP_COLUMN          );
}



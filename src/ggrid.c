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


#include "ggrid.h"

static int le_ggrid;
static zend_object_handlers ggrid_object_handlers;
static zend_class_entry * ggrid_class_entry_ce;

zend_class_entry * ggrid_get_class_entry(){
	return ggrid_class_entry_ce;
}

zend_object_handlers * ggrid_get_object_handlers(){
	return &ggrid_object_handlers;
}

void ggrid_add_data(gwidget_ptr intern, GtkWidget * address, zval * data){
	zval * add_folder = zend_hash_index_find(Z_ARRVAL_P(&intern->data), GWIDGET_DATA_INDEX_GGRID);
	if(add_folder){
		zend_hash_index_add(Z_ARRVAL_P(add_folder), (long) address, data);
		zval_addref_p(data);
	}else{
		//error
	}
}

PHP_METHOD(GGrid, __construct){
	ze_gwidget_object * widget;
	widget = Z_GWIDGET_P(getThis());
	widget->std.handlers = &ggrid_object_handlers;	
	if(!widget)
		RETURN_NULL();
	zval * narray = ecalloc(1,sizeof(zval));
	array_init(narray);
	zend_hash_index_add(Z_ARRVAL_P(&widget->widget_ptr->data), GWIDGET_DATA_INDEX_GCONTAINER, narray);
	zend_hash_index_add(Z_ARRVAL_P(&widget->widget_ptr->data), GWIDGET_DATA_INDEX_GGRID, narray);
	g_signal_connect(widget->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), widget);
}


PHP_METHOD(GGrid, attach){
	ze_gwidget_object *ze_obj = NULL;
	ze_gwidget_object * data;
	zval * self = getThis();
	zval * obj;
	long top, left, height = 1, width = 1;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "zll|ll", &obj, &left ,&top, &width, &height) == FAILURE){
        RETURN_NULL();
    }
	data = Z_GWIDGET_P(obj);
	if(self){
		ze_obj = Z_GWIDGET_P(self);
		gtk_grid_attach(GTK_GRID(ze_obj->widget_ptr->intern), data->widget_ptr->intern, left, top, width, height);
		ggrid_add_data(ze_obj->widget_ptr, data->widget_ptr->intern, obj);
	}
}


PHP_METHOD(GGrid, attachNextTo){
	ze_gwidget_object *ze_obj = NULL;
	ze_gwidget_object * data, * sib;
	zval * self = getThis();
	zval * obj, * sibling;
	long position, width = 1, height = 1;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "zzl|ll", &obj, &sibling ,&position, &width, &height) == FAILURE){
        RETURN_NULL();
    }
	data = Z_GWIDGET_P(obj);
	sib = Z_GWIDGET_P(sibling);
	if(self){
		ze_obj = Z_GWIDGET_P(self);
		switch(position){
			case GTK_POS_LEFT :
			case GTK_POS_RIGHT :
			case GTK_POS_TOP :
			case GTK_POS_BOTTOM :
				break;
			default :
				RETURN_NULL();
				//error
				break;
		}
		gtk_grid_attach_next_to(GTK_GRID(ze_obj->widget_ptr->intern), data->widget_ptr->intern, sib->widget_ptr->intern, position, width, height);
		ggrid_add_data(ze_obj->widget_ptr, data->widget_ptr->intern, obj);
	}
}

PHP_METHOD(GGrid, getChildAt){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	long left, top;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &left, &top) == FAILURE){
        RETURN_NULL();
    }
	if(self){
		ze_obj = Z_GWIDGET_P(self);
		GtkWidget * child = gtk_grid_get_child_at(GTK_GRID(ze_obj->widget_ptr->intern), left, top);
		if(!child)
			RETURN_NULL();
		zval * add_folder = zend_hash_index_find(Z_ARRVAL_P(&ze_obj->widget_ptr->data), GWIDGET_DATA_INDEX_GGRID);
		return_value = zend_hash_index_find(Z_ARRVAL_P(&ze_obj->widget_ptr->data), (long) child);
	}
}

PHP_METHOD(GGrid, insertRow){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	long position;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &position) == FAILURE){
        RETURN_NULL();
    }
	if(self){
		ze_obj = Z_GWIDGET_P(self);
		gtk_grid_insert_row(GTK_GRID(&ze_obj->widget_ptr->intern), position);
	}
}

PHP_METHOD(GGrid, insertColumn){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	long position;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &position) == FAILURE){
        RETURN_NULL();
    }
	if(self){
		ze_obj = Z_GWIDGET_P(self);
		gtk_grid_insert_column(GTK_GRID(&ze_obj->widget_ptr->intern), position);
	}
}

PHP_METHOD(GGrid, removeRow){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	long position;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &position) == FAILURE){
        RETURN_NULL();
    }
	if(self){
		ze_obj = Z_GWIDGET_P(self);
		gtk_grid_remove_row(GTK_GRID(&ze_obj->widget_ptr->intern), position);
	}
}

PHP_METHOD(GGrid, removeColumn){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	long position;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &position) == FAILURE){
        RETURN_NULL();
    }
	if(self){
		ze_obj = Z_GWIDGET_P(self);
		gtk_grid_remove_column(GTK_GRID(&ze_obj->widget_ptr->intern), position);
	}
}

PHP_METHOD(GGrid, insertNextTo){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	zval * obj;
	long side;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "zl", &obj, &side) == FAILURE){
        RETURN_NULL();
    }
	if(self){
		switch(side){
			case GTK_POS_LEFT :
			case GTK_POS_RIGHT :
			case GTK_POS_TOP :
			case GTK_POS_BOTTOM :
				break;
			default :
				RETURN_NULL();
				//error
				break;
		}
		ze_obj = Z_GWIDGET_P(self);
		ze_gwidget_object * data = Z_GWIDGET_P(obj);
		gtk_grid_insert_next_to(GTK_GRID(&ze_obj->widget_ptr->intern), data->widget_ptr->intern, side);
	}
}

PHP_METHOD(GGrid, __construct);
PHP_METHOD(GGrid, attach);
PHP_METHOD(GGrid, attachNextTo);
PHP_METHOD(GGrid, getChildAt);
PHP_METHOD(GGrid, insertRow);
PHP_METHOD(GGrid, insertColumn);
PHP_METHOD(GGrid, removeRow);
PHP_METHOD(GGrid, removeColumn);
PHP_METHOD(GGrid, insertNextTo);

static const zend_function_entry ggrid_class_functions[] = {
	PHP_ME(GGrid, __construct		, arginfo_pggi_void				, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR	)
	PHP_ME(GGrid, attach			, arginfo_ggrid_attach			, ZEND_ACC_PUBLIC					)
	PHP_ME(GGrid, attachNextTo		, arginfo_ggrid_attach_next_to	, ZEND_ACC_PUBLIC					)
	PHP_ME(GGrid, getChildAt		, arginfo_ggrid_get_child_at	, ZEND_ACC_PUBLIC					)
	PHP_ME(GGrid, insertRow			, arginfo_ggrid_insert_remove	, ZEND_ACC_PUBLIC					)
	PHP_ME(GGrid, insertColumn		, arginfo_ggrid_insert_remove	, ZEND_ACC_PUBLIC					)
	PHP_ME(GGrid, removeRow			, arginfo_ggrid_insert_remove	, ZEND_ACC_PUBLIC					)
	PHP_ME(GGrid, removeColumn		, arginfo_ggrid_insert_remove	, ZEND_ACC_PUBLIC					)
	PHP_ME(GGrid, insertNextTo		, arginfo_ggrid_insert_next_to	, ZEND_ACC_PUBLIC					)
	PHP_FE_END
};



zval *ggrid_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	const char * tmp;
	ZVAL_NULL(rv);
	if(!w)
		return rv;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	if(!strcmp(member_val, GGRID_BASELINE_ROW)){
		ZVAL_LONG(rv, gtk_grid_get_baseline_row(GTK_GRID(w->intern)));
		return rv;
	}else if(!strcmp(member_val, GGRID_COLUMN_SPACING)){
		ZVAL_LONG(rv, gtk_grid_get_column_spacing(GTK_GRID(w->intern)));
		return rv;
	}else if(!strcmp(member_val, GGRID_ROW_SPACING)){
		ZVAL_LONG(rv, gtk_grid_get_row_spacing(GTK_GRID(w->intern)));
		return rv;
	}else if(!strcmp(member_val, GGRID_COLUMN_HOMOGENEOUS)){
		ZVAL_BOOL(rv, gtk_grid_get_column_homogeneous(GTK_GRID(w->intern)));
		return rv;
	}else if(!strcmp(member_val, GGRID_ROW_HOMOGENEOUS)){
		ZVAL_BOOL(rv, gtk_grid_get_row_homogeneous(GTK_GRID(w->intern)));
		return rv;
	}
	return gcontainer_read_property(object, member, type, cache_slot, rv);
}

HashTable *ggrid_get_properties(zval *object){
	G_H_UPDATE_INIT(gcontainer_get_properties(object));
	const char * tmp;
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	if(!w){
		return NULL;
	}

	G_H_UPDATE_LONG(GGRID_BASELINE_ROW			, gtk_grid_get_baseline_row			(GTK_GRID(w->intern)));
	G_H_UPDATE_LONG(GGRID_COLUMN_SPACING		, gtk_grid_get_column_spacing		(GTK_GRID(w->intern)));
	G_H_UPDATE_LONG(GGRID_ROW_SPACING			, gtk_grid_get_row_spacing			(GTK_GRID(w->intern)));
	G_H_UPDATE_BOOL(GGRID_COLUMN_HOMOGENEOUS	, gtk_grid_get_column_homogeneous	(GTK_GRID(w->intern)));
	G_H_UPDATE_BOOL(GGRID_ROW_HOMOGENEOUS		, gtk_grid_get_row_homogeneous		(GTK_GRID(w->intern)));
	return G_H_UPDATE_RETURN;
}

void ggrid_write_property(zval *object, zval *member, zval *value, void **cache_slot){
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
			if(!strcmp(member_val, GGRID_BASELINE_ROW))
				gtk_grid_set_baseline_row(GTK_GRID(w->intern), tmp_l);
			else if(!strcmp(member_val, GGRID_COLUMN_SPACING))
				gtk_grid_set_column_spacing(GTK_GRID(w->intern), tmp_l);
			else if(!strcmp(member_val, GGRID_ROW_SPACING))
				gtk_grid_set_row_spacing(GTK_GRID(w->intern), tmp_l);
			else
				gbox_write_property(object, member, value, cache_slot);
			break;
		case IS_FALSE :
		case IS_TRUE :
			tmp_b = Z_TYPE_P(value) == IS_TRUE ? 1 : 0;
			if(!strcmp(member_val, GGRID_COLUMN_HOMOGENEOUS))
				gtk_grid_set_column_homogeneous(GTK_GRID(w->intern), tmp_b);
			else if(!strcmp(member_val, GGRID_ROW_HOMOGENEOUS))
				gtk_grid_set_row_homogeneous(GTK_GRID(w->intern), tmp_b);
			else
				gcontainer_write_property(object, member, value, cache_slot);
			break;
		default:
			gcontainer_write_property(object, member, value, cache_slot);
	}
}


#define DECLARE_GGRID_PROP(name) \
DECLARE_CLASS_PROPERTY(ggrid_class_entry_ce, name)

void ggrid_init(int module_number){
	zend_class_entry ce;
	le_ggrid = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "ggrid", module_number);

	memcpy(&ggrid_object_handlers, gbox_get_object_handlers(), sizeof(zend_object_handlers));
	ggrid_object_handlers.read_property  = ggrid_read_property;
	ggrid_object_handlers.get_properties = ggrid_get_properties;
	ggrid_object_handlers.write_property = ggrid_write_property;	
	INIT_CLASS_ENTRY(ce, "GGrid", ggrid_class_functions);
	ce.create_object 	= gwidget_object_new;
	ggrid_class_entry_ce= zend_register_internal_class_ex(&ce, gbox_get_class_entry());

	DECLARE_GGRID_PROP(GGRID_BASELINE_ROW);
	DECLARE_GGRID_PROP(GGRID_COLUMN_HOMOGENEOUS);
	DECLARE_GGRID_PROP(GGRID_COLUMN_SPACING);
	DECLARE_GGRID_PROP(GGRID_ROW_HOMOGENEOUS);
	DECLARE_GGRID_PROP(GGRID_ROW_SPACING);
}



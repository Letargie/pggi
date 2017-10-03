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


#include "gtreeselection.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_gtree_selection;
static zend_object_handlers gtree_selection_object_handlers;
static zend_class_entry * gtree_selection_class_entry_ce;

zend_class_entry * gtree_selection_get_class_entry(){
	return gtree_selection_class_entry_ce;
}

zend_object_handlers * gtree_selection_get_object_handlers(){
	return &gtree_selection_object_handlers;
}

/****************************/
/* Memory handling function */
/****************************/

gtree_selection_ptr gtree_selection_new(){
	gtree_selection_ptr tor = ecalloc(1, sizeof(gtree_selection_t));
	array_init(&tor->signals);
	return tor;
}

zend_object *gtree_selection_object_new(zend_class_entry *class_type){
	ze_gtree_selection_object *intern;
	intern = ecalloc(1, sizeof(ze_gtree_selection_object) + zend_object_properties_size(class_type));
	zend_object_std_init	(&intern->std, class_type);
	object_properties_init	(&intern->std, class_type);
	intern->std.handlers = &gtree_selection_object_handlers;
	return &intern->std;
}

void gtree_selection_dtor(gtree_selection_ptr intern){
	if (intern->intern){	
	/*unref tree selection?*/
	}
	zend_hash_destroy(Z_ARRVAL_P(&intern->signals));
	efree(intern);
}

void gtree_selection_object_free_storage(zend_object *object){
	ze_gtree_selection_object *intern = php_gtree_selection_fetch_object(object);
	if (!intern) {
		return;
	}
	if (intern->tree_selection_ptr){
		gtree_selection_dtor(intern->tree_selection_ptr);
	}
	intern->tree_selection_ptr = NULL;
	zend_object_std_dtor(&intern->std);
}

void gtree_selection_free_resource(zend_resource *rsrc) {
	gtree_selection_ptr tree = (gtree_selection_ptr) rsrc->ptr;
	gtree_selection_dtor(tree);
}


#define INDEX_ON_FUNCTION_NAME 1
#define INDEX_ON_FUNCTION_PARAM 2

void gtree_selection_function(gpointer data, unsigned int type){
	zval retval;
	ze_gtree_selection_object * b = data;
	zval * array, * value, * zv;
	zval args[2];
	if(type){
		zend_object * this = php_gtree_selection_reverse_object(b);
		ZVAL_OBJ(&args[0], this);
		array = &b->tree_selection_ptr->signals;
		value = (zend_hash_index_find(Z_ARRVAL_P(array), type));
		if(value != NULL){
			ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(value), zv){
				zval * function	= zend_hash_index_find(Z_ARRVAL_P(zv), INDEX_ON_FUNCTION_NAME);
				zval * tmp = zend_hash_index_find(Z_ARRVAL_P(zv), INDEX_ON_FUNCTION_PARAM);
				if(tmp){
					ZVAL_COPY(&args[1], tmp);
				}else
					ZVAL_NULL(&args[1]);
				if(call_user_function(EG(function_table), NULL, function, &retval, 1, args) != SUCCESS){
					zend_error(E_ERROR, "Function call failed");
				}
			} ZEND_HASH_FOREACH_END();
		}
	}
}

/*********************************/
/* internal on-related functions */
/*********************************/


void gtree_selection_func_changed(GtkTreeSelection* tree, gpointer data){
	gtree_selection_function(data, gsignal_gtree_selection_changed);
}

/***************/
/* PHP Methods */
/***************/

GTREE_SELECTION_METHOD(__construct){}


void gtree_selection_foreach_caller(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iterp, gpointer data){
	zval ** param = data;
	zval retval;
	zval iter_zv;
	zend_object * ze_iter = gtree_iter_object_new(gtree_iter_get_class_entry());
	ze_gtree_iter_object * iter = php_gtree_iter_fetch_object(ze_iter);
	iter->tree_iter_ptr = gtree_iter_new();
	iter->tree_iter_ptr->intern = iterp;
	iter->tree_iter_ptr->parent = model;
	ZVAL_OBJ(&iter_zv, ze_iter);

	zval args[2];
	args[0] = iter_zv;
	args[1] = *param[1];
	if(call_user_function(EG(function_table), NULL, param[0], &retval, 2, args) != SUCCESS){
		zend_error(E_ERROR, "Function call failed");
	}
}

GTREE_SELECTION_METHOD(forEach){
	zval * function, * this, * param = NULL;
	ze_gtree_selection_object *ze_obj = NULL;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "z|z" ,&function, &param) == FAILURE)
		return;
	this = getThis();
	ze_obj = Z_GTREE_SELECTION_P(this);
	zval * data[2];
	data[0] = function;
	data[1] = param;
	gtk_tree_selection_selected_foreach(ze_obj->tree_selection_ptr->intern, gtree_selection_foreach_caller, (gpointer) &data);
}

GTREE_SELECTION_METHOD(on){
	zval * function, * data,* narray, * this, * param = NULL;
	long val;
	ze_gtree_selection_object *ze_obj = NULL;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "lz|z", &val ,&function, &param) == FAILURE)
		return;
	this = getThis();
	ze_obj = Z_GTREE_SELECTION_P(this);
	switch(val){
		case gsignal_gtree_selection_changed :
			break;
		default :
			zend_error(E_ERROR, "Signal unknown");
	}
	zval * data_to_insert = ecalloc(1,sizeof(zval));
	array_init(data_to_insert);
	zend_hash_index_add(Z_ARRVAL_P(data_to_insert), INDEX_ON_FUNCTION_NAME, function);
	zval_addref_p(function);
	if(param){
		zend_hash_index_add(Z_ARRVAL_P(data_to_insert), INDEX_ON_FUNCTION_PARAM, param);
		zval_addref_p(param);
	}
	data = zend_hash_index_find(Z_ARRVAL_P(&ze_obj->tree_selection_ptr->signals), val);
	if(data == NULL){
		narray = ecalloc(1,sizeof(zval));
		array_init(narray);
		zend_hash_index_add(Z_ARRVAL_P(&ze_obj->tree_selection_ptr->signals), val, narray);
		zend_hash_next_index_insert(Z_ARRVAL_P(narray), data_to_insert);
		switch(val){
			case gsignal_gtree_selection_changed :
				g_signal_connect(ze_obj->tree_selection_ptr->intern, GSIGNAL_GTREE_SELECTION_CHANGED, G_CALLBACK (gtree_selection_func_changed), (gpointer) ze_obj);
				break;
		}
	}else{
		zend_hash_next_index_insert(Z_ARRVAL_P(data), data_to_insert);
	}
}

GTREE_SELECTION_METHOD(getSelected){
	ze_gtree_selection_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_obj = Z_GTREE_SELECTION_P(self);
	if(gtk_tree_selection_get_mode(ze_obj->tree_selection_ptr->intern) == GTK_SELECTION_MULTIPLE){
		zend_throw_exception_ex(pggi_exception_get(), 0, "you can't get the selected node if you're in mode SELECTION_MULTIPLE");
		return;
	}
	GtkTreeIter new;
	GtkTreeModel * parent;
	if(gtk_tree_selection_get_selected(ze_obj->tree_selection_ptr->intern, &parent, &new)){
		GtkTreeIter * new_node = ecalloc(1, sizeof(GtkTreeIter));
		*new_node = new;
		zend_object * ze_iter = gtree_iter_object_new(gtree_iter_get_class_entry());
		ze_gtree_iter_object * iter = php_gtree_iter_fetch_object(ze_iter);
		iter->tree_iter_ptr = gtree_iter_new();
		iter->tree_iter_ptr->intern = new_node;
		iter->tree_iter_ptr->parent = parent;
		RETURN_OBJ(ze_iter);
	}else
		RETURN_FALSE;
}

GTREE_SELECTION_METHOD(countSelectedRows){
	ze_gtree_selection_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_GTREE_SELECTION_P(self);
	RETURN_LONG(gtk_tree_selection_count_selected_rows(ze_obj->tree_selection_ptr->intern));
}

GTREE_SELECTION_METHOD(selectIter){
	ze_gtree_selection_object *ze_obj = NULL;
	ze_gtree_iter_object * iterator;
	zval * self = getThis();
	zval * iter;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &iter, gtree_iter_get_class_entry()) == FAILURE)
		return;
	iterator = Z_GTREE_ITER_P(iter);
	ze_obj = Z_GTREE_SELECTION_P(self);
	gtk_tree_selection_select_iter(ze_obj->tree_selection_ptr->intern, iterator->tree_iter_ptr->intern);
}

GTREE_SELECTION_METHOD(unselectIter){
	ze_gtree_selection_object *ze_obj = NULL;
	ze_gtree_iter_object * iterator;
	zval * self = getThis();
	zval * iter;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &iter, gtree_iter_get_class_entry()) == FAILURE)
		return;
	iterator = Z_GTREE_ITER_P(iter);
	ze_obj = Z_GTREE_SELECTION_P(self);
	gtk_tree_selection_unselect_iter(ze_obj->tree_selection_ptr->intern, iterator->tree_iter_ptr->intern);
}

GTREE_SELECTION_METHOD(iterIsSelected){
	ze_gtree_selection_object *ze_obj = NULL;
	ze_gtree_iter_object * iterator;
	zval * self = getThis();
	zval * iter;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &iter, gtree_iter_get_class_entry()) == FAILURE)
		return;
	iterator = Z_GTREE_ITER_P(iter);
	ze_obj = Z_GTREE_SELECTION_P(self);
	RETURN_BOOL(gtk_tree_selection_iter_is_selected(ze_obj->tree_selection_ptr->intern, iterator->tree_iter_ptr->intern));
}

GTREE_SELECTION_METHOD(selectAll){
	ze_gtree_selection_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_GTREE_SELECTION_P(self);
	if(gtk_tree_selection_get_mode(ze_obj->tree_selection_ptr->intern) == GTK_SELECTION_MULTIPLE)
		gtk_tree_selection_select_all(ze_obj->tree_selection_ptr->intern);
	else
		zend_throw_exception_ex(pggi_exception_get(), 0, "to select all nodes, you must be in mode SELECTION_MULTIPLE");
}

GTREE_SELECTION_METHOD(unselectAll){
	ze_gtree_selection_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_GTREE_SELECTION_P(self);
	gtk_tree_selection_unselect_all(ze_obj->tree_selection_ptr->intern);
}

static const zend_function_entry gtree_selection_class_functions[] = {
	PHP_ME(GTreeSelection, __construct      , arginfo_pggi_void                        , ZEND_ACC_PRIVATE | ZEND_ACC_CTOR)
	PHP_ME(GTreeSelection, on               , arginfo_pggi_on                          , ZEND_ACC_PUBLIC)
	PHP_ME(GTreeSelection, selectAll        , arginfo_pggi_void                        , ZEND_ACC_PUBLIC)
	PHP_ME(GTreeSelection, unselectAll      , arginfo_pggi_void                        , ZEND_ACC_PUBLIC)
	PHP_ME(GTreeSelection, iterIsSelected   , arginfo_gtree_selection_iter_is_selected , ZEND_ACC_PUBLIC)
	PHP_ME(GTreeSelection, selectIter       , arginfo_gtree_selection_iter             , ZEND_ACC_PUBLIC)
	PHP_ME(GTreeSelection, unselectIter     , arginfo_gtree_selection_iter             , ZEND_ACC_PUBLIC)
	PHP_ME(GTreeSelection, countSelectedRows, arginfo_gtree_selection_count            , ZEND_ACC_PUBLIC)
	PHP_ME(GTreeSelection, getSelected      , arginfo_gtree_selection_get_selected     , ZEND_ACC_PUBLIC)
	PHP_ME(GTreeSelection, forEach          , arginfo_gtree_selection_foreach          , ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *gtree_selection_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gtree_selection_object * intern = Z_GTREE_SELECTION_P(object);
	gtree_selection_ptr b = intern->tree_selection_ptr;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkTreeSelection * tree = GTK_TREE_SELECTION(b->intern);
	if(!strcmp(member_val, GTREE_SELECTION_MODE)){
		ZVAL_LONG(rv, gtk_tree_selection_get_mode(tree));
	}
	return std_object_handlers.read_property(object, member, type, cache_slot, rv);
}

HashTable *gtree_selection_get_properties(zval *object){
	G_H_UPDATE_INIT(zend_std_get_properties(object));
	ze_gtree_selection_object * intern = Z_GTREE_SELECTION_P(object);
	gtree_selection_ptr b = intern->tree_selection_ptr;
	GtkTreeSelection * tree = GTK_TREE_SELECTION(b->intern);
	G_H_UPDATE_LONG(GTREE_SELECTION_MODE, gtk_tree_selection_get_mode(tree));
	return G_H_UPDATE_RETURN;
}

void gtree_selection_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_gtree_selection_object * intern = Z_GTREE_SELECTION_P(object);
	gtree_selection_ptr b = intern->tree_selection_ptr;
	long tmp_l;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkTreeSelection * tree = GTK_TREE_SELECTION(b->intern);
	switch(Z_TYPE_P(value)){
		case IS_LONG :
			tmp_l = Z_LVAL_P(value);
			if(!strcmp(member_val, GTREE_SELECTION_MODE)){
				switch(tmp_l){
					case GTK_SELECTION_NONE     :
					case GTK_SELECTION_SINGLE   :
					case GTK_SELECTION_BROWSE   :
					case GTK_SELECTION_MULTIPLE :
						gtk_tree_selection_set_mode(tree, tmp_l);
						break;
					default :
						zend_throw_exception_ex(pggi_exception_get(), 0, "the selection mode needs to be a SELECTION_*");
						break;
				}
			}else
				std_object_handlers.write_property(object, member, value, cache_slot);
			break;
			
		default:
			std_object_handlers.write_property(object, member, value, cache_slot);
	}
}

/************************************/
/* GTreeSelection Class Initialization */
/************************************/

#define DECLARE_GTREE_SELECTION_PROP(name) \
DECLARE_CLASS_PROPERTY(gtree_selection_class_entry_ce, name)

#define GTREE_SELECTION_CONSTANT(name, value) \
zend_declare_class_constant_long(gtree_selection_class_entry_ce, name, sizeof(name)-1, value);


void gtree_selection_init(int module_number){
	zend_class_entry ce;
	le_gtree_selection = zend_register_list_destructors_ex(gtree_selection_free_resource, NULL, "PGGI\\GTreeSelection", module_number);

	memcpy(&gtree_selection_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	gtree_selection_object_handlers.offset         = XtOffsetOf(ze_gtree_selection_object, std);
	gtree_selection_object_handlers.free_obj       = gtree_selection_object_free_storage;
	gtree_selection_object_handlers.clone_obj      = NULL;
	gtree_selection_object_handlers.read_property  = gtree_selection_read_property;
	gtree_selection_object_handlers.get_properties = gtree_selection_get_properties;
	gtree_selection_object_handlers.write_property = gtree_selection_write_property;
	INIT_CLASS_ENTRY(ce, "PGGI\\GTreeSelection", gtree_selection_class_functions);
	ce.create_object = gtree_selection_object_new;
	gtree_selection_class_entry_ce = zend_register_internal_class(&ce);

	DECLARE_GTREE_SELECTION_PROP(GTREE_SELECTION_MODE);

}



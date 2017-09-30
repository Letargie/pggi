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


#include "gtreeiter.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_gtree_iter;
static zend_object_handlers gtree_iter_object_handlers;
static zend_class_entry * gtree_iter_class_entry_ce;

zend_class_entry * gtree_iter_get_class_entry(){
	return gtree_iter_class_entry_ce;
}

zend_object_handlers * gtree_iter_get_object_handlers(){
	return &gtree_iter_object_handlers;
}

/****************************/
/* Memory handling function */
/****************************/

gtree_iter_ptr gtree_iter_new(){
	gtree_iter_ptr tor = ecalloc(1, sizeof(gtree_iter_t));
	return tor;
}

zend_object *gtree_iter_object_new(zend_class_entry *class_type){
	ze_gtree_iter_object *intern;
	intern = ecalloc(1, sizeof(ze_gtree_iter_object) + zend_object_properties_size(class_type));
	zend_object_std_init	(&intern->std, class_type);
	object_properties_init	(&intern->std, class_type);
	intern->std.handlers = &gtree_iter_object_handlers;
	return &intern->std;
}

void gtree_iter_dtor(gtree_iter_ptr intern){
	zval *  zv, * tmp;
	if (intern->intern){	
	/*unref text buffer?*/
	}
	efree(intern);
}

void gtree_iter_object_free_storage(zend_object *object){
	ze_gtree_iter_object *intern = php_gtree_iter_fetch_object(object);
	if (!intern) {
		return;
	}
	if (intern->tree_iter_ptr){
		gtree_iter_dtor(intern->tree_iter_ptr);
	}
	intern->tree_iter_ptr = NULL;
	zend_object_std_dtor(&intern->std);
}

void gtree_iter_free_resource(zend_resource *rsrc) {
	gtree_iter_ptr tree = (gtree_iter_ptr) rsrc->ptr;
	gtree_iter_dtor(tree);
}

/***************/
/* PHP Methods */
/***************/

// listing the gtypes
GTREE_ITER_METHOD(__construct){
	RETURN_NULL();
}

GTREE_ITER_METHOD(getValue){
	ze_gtree_iter_object *ze_obj = NULL;
	zval * self = getThis();
	long column;
	// BUG " ::getValue()" must be "TreeIter::getValue()"
	//if(pggi_parse_method_parameters_throw(ZEND_NUM_ARGS(), self, "l", &column) == FAILURE)
	if(zend_parse_parameters_ex(ZEND_PARSE_PARAMS_THROW, ZEND_NUM_ARGS(), "l", &column) == FAILURE)
		return;
	ze_obj = Z_GTREE_ITER_P(self);
	GValue rval ={0,};
	gtk_tree_model_get_value(ze_obj->tree_iter_ptr->parent, ze_obj->tree_iter_ptr->intern, column, &rval);
	switch(G_VALUE_TYPE(&rval)){
		 case G_TYPE_STRING :
			RETURN_STRING(g_value_get_string(&rval));
			break;
		default :
			printf("default value \n");
	}
	// for now we don't really unset when returning value have to check for destruction
	g_value_unset(&rval);
}

GTREE_ITER_METHOD(setValue){
	ze_gtree_iter_object *ze_obj = NULL;
	zval * self = getThis();
	long column;
	zval * value;
	//if(pggi_parse_method_parameters_throw(ZEND_NUM_ARGS(), self, "lz", &column, &value) == FAILURE)
	if(zend_parse_parameters_ex(ZEND_PARSE_PARAMS_THROW, ZEND_NUM_ARGS(), "lz", &column, &value) == FAILURE)
		return;
	ze_obj = Z_GTREE_ITER_P(self);
	switch(Z_TYPE_P(value)){
		case IS_STRING :
			;
			GValue a = G_VALUE_INIT;
			g_value_init (&a, G_TYPE_STRING);
			g_value_set_static_string (&a, Z_STRVAL_P(value));
			gtk_tree_store_set_value(GTK_TREE_STORE(ze_obj->tree_iter_ptr->parent), ze_obj->tree_iter_ptr->intern, 0, &a);
			break;
		default :
			printf("Warning, for now, only Strings are handled\n");
	}
}


GTREE_ITER_METHOD(next){
	ze_gtree_iter_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_obj = Z_GTREE_ITER_P(self);
	GtkTreeIter * new = gtk_tree_iter_copy(ze_obj->tree_iter_ptr->intern);
	gtk_tree_model_iter_next(ze_obj->tree_iter_ptr->parent, new);
	if(new){
		zend_object * ze_iter = gtree_iter_object_new(gtree_iter_get_class_entry());
		ze_gtree_iter_object * iter = php_gtree_iter_fetch_object(ze_iter);
		iter->tree_iter_ptr = gtree_iter_new();
		iter->tree_iter_ptr->intern = new;
		iter->tree_iter_ptr->parent = ze_obj->tree_iter_ptr->parent;
		RETURN_OBJ(ze_iter);
	}else
		RETURN_FALSE;
}

GTREE_ITER_METHOD(previous){
	ze_gtree_iter_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_obj = Z_GTREE_ITER_P(self);
	GtkTreeIter * new = gtk_tree_iter_copy(ze_obj->tree_iter_ptr->intern);
	gtk_tree_model_iter_previous(ze_obj->tree_iter_ptr->parent, new);
	if(new){
		zend_object * ze_iter = gtree_iter_object_new(gtree_iter_get_class_entry());
		ze_gtree_iter_object * iter = php_gtree_iter_fetch_object(ze_iter);
		iter->tree_iter_ptr = gtree_iter_new();
		iter->tree_iter_ptr->intern = new;
		iter->tree_iter_ptr->parent = ze_obj->tree_iter_ptr->parent;
		RETURN_OBJ(ze_iter);
	}else
		RETURN_FALSE;
}

GTREE_ITER_METHOD(getFirstChild){
	ze_gtree_iter_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_obj = Z_GTREE_ITER_P(self);
	GtkTreeIter * new = ecalloc(1, sizeof(GtkTreeIter));
	GtkTreeIter tmp;
	if(gtk_tree_model_iter_children(ze_obj->tree_iter_ptr->parent, &tmp, ze_obj->tree_iter_ptr->intern)){
		*new = tmp;
		zend_object * ze_iter = gtree_iter_object_new(gtree_iter_get_class_entry());
		ze_gtree_iter_object * iter = php_gtree_iter_fetch_object(ze_iter);
		iter->tree_iter_ptr = gtree_iter_new();
		iter->tree_iter_ptr->intern = new;
		iter->tree_iter_ptr->parent = ze_obj->tree_iter_ptr->parent;
		RETURN_OBJ(ze_iter);
	}else
		RETURN_FALSE;
}

GTREE_ITER_METHOD(hasChild){
	ze_gtree_iter_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_obj = Z_GTREE_ITER_P(self);
	RETURN_BOOL(gtk_tree_model_iter_has_child(ze_obj->tree_iter_ptr->parent, ze_obj->tree_iter_ptr->intern));
}

GTREE_ITER_METHOD(getNbChildren){
	ze_gtree_iter_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_obj = Z_GTREE_ITER_P(self);
	RETURN_LONG(gtk_tree_model_iter_n_children(ze_obj->tree_iter_ptr->parent, ze_obj->tree_iter_ptr->intern));
}

GTREE_ITER_METHOD(getNthChild){
	ze_gtree_iter_object *ze_obj = NULL;
	zval * self = getThis();
	long number;
	if(pggi_parse_method_parameters_throw(ZEND_NUM_ARGS(), self, "l",&number) == FAILURE)
		return;
	ze_obj = Z_GTREE_ITER_P(self);
	GtkTreeIter * new = ecalloc(1, sizeof(GtkTreeIter));
	GtkTreeIter tmp;
	if(gtk_tree_model_iter_nth_child(ze_obj->tree_iter_ptr->parent, &tmp, ze_obj->tree_iter_ptr->intern, number)){
		*new = tmp;
		zend_object * ze_iter = gtree_iter_object_new(gtree_iter_get_class_entry());
		ze_gtree_iter_object * iter = php_gtree_iter_fetch_object(ze_iter);
		iter->tree_iter_ptr = gtree_iter_new();
		iter->tree_iter_ptr->intern = new;
		iter->tree_iter_ptr->parent = ze_obj->tree_iter_ptr->parent;
		RETURN_OBJ(ze_iter);
	}else
		RETURN_FALSE;
}

GTREE_ITER_METHOD(getParent){
	ze_gtree_iter_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_obj = Z_GTREE_ITER_P(self);
	GtkTreeIter * new = ecalloc(1, sizeof(GtkTreeIter));
	GtkTreeIter tmp;
	if(gtk_tree_model_iter_parent(ze_obj->tree_iter_ptr->parent, &tmp, ze_obj->tree_iter_ptr->intern)){
		*new = tmp;
		zend_object * ze_iter = gtree_iter_object_new(gtree_iter_get_class_entry());
		ze_gtree_iter_object * iter = php_gtree_iter_fetch_object(ze_iter);
		iter->tree_iter_ptr = gtree_iter_new();
		iter->tree_iter_ptr->intern = new;
		iter->tree_iter_ptr->parent = ze_obj->tree_iter_ptr->parent;
		RETURN_OBJ(ze_iter);
	}else
		RETURN_FALSE;
}

static const zend_function_entry gtree_iter_class_functions[] = {
	PHP_ME(GTreeIter, __construct  , arginfo_pggi_void               , ZEND_ACC_PRIVATE | ZEND_ACC_CTOR)
	PHP_ME(GTreeIter, getValue     , arginfo_gtree_iter_get_value    , ZEND_ACC_PUBLIC)
	PHP_ME(GTreeIter, setValue     , arginfo_gtree_iter_set_value    , ZEND_ACC_PUBLIC)
	PHP_ME(GTreeIter, next         , arginfo_pggi_void               , ZEND_ACC_PUBLIC)
	PHP_ME(GTreeIter, previous     , arginfo_pggi_void               , ZEND_ACC_PUBLIC)
	PHP_ME(GTreeIter, getFirstChild, arginfo_pggi_void               , ZEND_ACC_PUBLIC)
	PHP_ME(GTreeIter, hasChild     , arginfo_pggi_void               , ZEND_ACC_PUBLIC)
	PHP_ME(GTreeIter, getNbChildren, arginfo_pggi_void               , ZEND_ACC_PUBLIC)
	PHP_ME(GTreeIter, getNthChild  , arginfo_gtree_iter_get_nth_child, ZEND_ACC_PUBLIC)
	PHP_ME(GTreeIter, getParent    , arginfo_pggi_void               , ZEND_ACC_PUBLIC)

	PHP_FE_END
};

/************************************/
/* GTreeIter Class Initialization */
/************************************/

#define DECLARE_GTREE_ITER_PROP(name) \
DECLARE_CLASS_PROPERTY(gtree_iter_class_entry_ce, name)

#define GTREE_ITER_CONSTANT(name, value) \
zend_declare_class_constant_long(gtree_iter_class_entry_ce, name, sizeof(name)-1, value);


void gtree_iter_init(int module_number){
	zend_class_entry ce;
	le_gtree_iter = zend_register_list_destructors_ex(gtree_iter_free_resource, NULL, "PGGI\\GTreeIter", module_number);

	memcpy(&gtree_iter_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	gtree_iter_object_handlers.offset    = XtOffsetOf(ze_gtree_iter_object, std);
	gtree_iter_object_handlers.free_obj  = gtree_iter_object_free_storage;
	gtree_iter_object_handlers.clone_obj = NULL;
	INIT_CLASS_ENTRY(ce, "PGGI\\GTreeIter", gtree_iter_class_functions);
	ce.create_object = gtree_iter_object_new;
	gtree_iter_class_entry_ce = zend_register_internal_class(&ce);
}



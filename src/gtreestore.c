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


#include "gtreestore.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_gtree_store;
static zend_object_handlers gtree_store_object_handlers;
static zend_class_entry * gtree_store_class_entry_ce;

zend_class_entry * gtree_store_get_class_entry(){
	return gtree_store_class_entry_ce;
}

zend_object_handlers * gtree_store_get_object_handlers(){
	return &gtree_store_object_handlers;
}

/****************************/
/* Memory handling function */
/****************************/

gtree_store_ptr gtree_store_new(){
	gtree_store_ptr tor = ecalloc(1, sizeof(gtree_store_t));
	return tor;
}

zend_object *gtree_store_object_new(zend_class_entry *class_type){
	ze_gtree_store_object *intern;
	intern = ecalloc(1, sizeof(ze_gtree_store_object) + zend_object_properties_size(class_type));
	zend_object_std_init	(&intern->std, class_type);
	object_properties_init	(&intern->std, class_type);
	intern->std.handlers = &gtree_store_object_handlers;
	return &intern->std;
}

void gtree_store_dtor(gtree_store_ptr intern){
	zval *  zv, * tmp;
	if (intern->intern){	
	/*unref text buffer?*/
	}
	efree(intern);
}

void gtree_store_object_free_storage(zend_object *object){
	ze_gtree_store_object *intern = php_gtree_store_fetch_object(object);
	if (!intern) {
		return;
	}
	if (intern->tree_store_ptr){
		gtree_store_dtor(intern->tree_store_ptr);
	}
	intern->tree_store_ptr = NULL;
	zend_object_std_dtor(&intern->std);
}

void gtree_store_free_resource(zend_resource *rsrc) {
	gtree_store_ptr tree = (gtree_store_ptr) rsrc->ptr;
	gtree_store_dtor(tree);
}

/***************/
/* PHP Methods */
/***************/

// listing the gtypes
GTREE_STORE_METHOD(__construct){
	ze_gtree_store_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_obj = Z_GTREE_STORE_P(self);
	ze_obj->tree_store_ptr = gtree_store_new();
	ze_obj->tree_store_ptr->intern = gtk_tree_store_new(1, G_TYPE_STRING);
}

GTREE_STORE_METHOD(clear){
	zval * self = getThis();
    if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
    ze_gtree_store_object * ze_obj = Z_GTREE_STORE_P(self);
	gtk_tree_store_clear(ze_obj->tree_store_ptr->intern);
}

GTREE_STORE_METHOD(remove);
GTREE_STORE_METHOD(insert);
GTREE_STORE_METHOD(insertBefore);
GTREE_STORE_METHOD(insertAfter);
GTREE_STORE_METHOD(insertWithValues);
GTREE_STORE_METHOD(prepend);

GTREE_STORE_METHOD(append){
	zval * self = getThis();
	zval * parent = NULL;
	GtkTreeIter * iterparent = NULL;
    if(pggi_parse_method_parameters_throw(ZEND_NUM_ARGS(), self, "|z", &parent) == FAILURE)
		return;
    ze_gtree_store_object * ze_obj = Z_GTREE_STORE_P(self);
	GtkTreeIter * nrow;
	if(parent){
		ze_gtree_iter_object * par = Z_GTREE_ITER_P(parent);
		iterparent = par->tree_iter_ptr->intern;
	}
	gtk_tree_store_append(ze_obj->tree_store_ptr->intern, nrow, iterparent);
	zend_object * ze_iter = gtree_iter_object_new(gtree_iter_get_class_entry());
	ze_gtree_iter_object * iter = php_gtree_iter_fetch_object(ze_iter);
	iter->tree_iter_ptr->intern = nrow;
	iter->tree_iter_ptr->parent = GTK_TREE_MODEL(ze_obj->tree_store_ptr->intern);
	RETURN_OBJ(ze_iter);
}

GTREE_STORE_METHOD(prepend){
	zval * self = getThis();
	zval * parent = NULL;
	GtkTreeIter * iterparent = NULL;
    if(pggi_parse_method_parameters_throw(ZEND_NUM_ARGS(), self, "|z", &parent) == FAILURE)
		return;
    ze_gtree_store_object * ze_obj = Z_GTREE_STORE_P(self);
	GtkTreeIter * nrow;
	if(parent){
		ze_gtree_iter_object * par = Z_GTREE_ITER_P(parent);
		iterparent = par->tree_iter_ptr->intern;
	}
	gtk_tree_store_prepend(ze_obj->tree_store_ptr->intern, nrow, iterparent);
	zend_object * ze_iter = gtree_iter_object_new(gtree_iter_get_class_entry());
	ze_gtree_iter_object * iter = php_gtree_iter_fetch_object(ze_iter);
	iter->tree_iter_ptr->intern = nrow;
	iter->tree_iter_ptr->parent = GTK_TREE_MODEL(ze_obj->tree_store_ptr->intern);
	RETURN_OBJ(ze_iter);
}

GTREE_STORE_METHOD(isAncestor);
GTREE_STORE_METHOD(depth);
GTREE_STORE_METHOD(getNbColumns);
GTREE_STORE_METHOD(getColumnType);
GTREE_STORE_METHOD(getIterFirst);

static const zend_function_entry gtree_store_class_functions[] = {
	PHP_ME(GTreeStore, __construct, arginfo_pggi_void       , ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(GTreeStore, clear      , arginfo_pggi_void       , ZEND_ACC_PUBLIC)
	PHP_ME(GTreeStore, append     , arginfo_gtree_store_pend, ZEND_ACC_PUBLIC)
	PHP_ME(GTreeStore, prepend    , arginfo_gtree_store_pend, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/************************************/
/* GTreeStore Class Initialization */
/************************************/

#define DECLARE_GTREE_STORE_PROP(name) \
DECLARE_CLASS_PROPERTY(gtree_store_class_entry_ce, name)

#define GTREE_STORE_CONSTANT(name, value) \
zend_declare_class_constant_double(gtree_store_class_entry_ce, name, sizeof(name)-1, value);


void gtree_store_init(int module_number){
	zend_class_entry ce;
	le_gtree_store = zend_register_list_destructors_ex(gtree_store_free_resource, NULL, "tree store", module_number);

	memcpy(&gtree_store_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	gtree_store_object_handlers.offset			= XtOffsetOf(ze_gtree_store_object, std);
	gtree_store_object_handlers.free_obj		= gtree_store_object_free_storage;
	gtree_store_object_handlers.clone_obj		= NULL;
	INIT_CLASS_ENTRY(ce, "GTreeStore", gtree_store_class_functions);
	ce.create_object = gtree_store_object_new;
	gtree_store_class_entry_ce = zend_register_internal_class(&ce);
}



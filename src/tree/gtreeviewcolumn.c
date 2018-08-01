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


#include "gtreeviewcolumn.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_gtree_view_column;
static zend_object_handlers gtree_view_column_object_handlers;
static zend_class_entry * gtree_view_column_class_entry_ce;

zend_class_entry * gtree_view_column_get_class_entry(){
	return gtree_view_column_class_entry_ce;
}

zend_object_handlers * gtree_view_column_get_object_handlers(){
	return &gtree_view_column_object_handlers;
}

/****************************/
/* Memory handling function */
/****************************/

gtree_view_column_ptr gtree_view_column_new(){
	gtree_view_column_ptr tor = ecalloc(1, sizeof(gtree_view_column_t));
	array_init(&tor->signals);
	return tor;
}

zend_object *gtree_view_column_object_new(zend_class_entry *class_type){
	ze_gtree_view_column_object *intern;
	intern = ecalloc(1, sizeof(ze_gtree_view_column_object) + zend_object_properties_size(class_type));
	zend_object_std_init	(&intern->std, class_type);
	object_properties_init	(&intern->std, class_type);
	intern->std.handlers = &gtree_view_column_object_handlers;
	return &intern->std;
}

void gtree_view_column_dtor(gtree_view_column_ptr intern){
	if (intern->intern){	
	/*unref text tree?*/
	}
	zend_hash_destroy(Z_ARRVAL_P(&intern->signals));
	efree(intern);
}

void gtree_view_column_object_free_storage(zend_object *object){
	ze_gtree_view_column_object *intern = php_gtree_view_column_fetch_object(object);
	if (!intern) {
		return;
	}
	if (intern->tree_view_column_ptr){
		gtree_view_column_dtor(intern->tree_view_column_ptr);
	}
	intern->tree_view_column_ptr = NULL;
	zend_object_std_dtor(&intern->std);
}

void gtree_view_column_free_resource(zend_resource *rsrc) {
	gtree_view_column_ptr tree = (gtree_view_column_ptr) rsrc->ptr;
	gtree_view_column_dtor(tree);
}


#define INDEX_ON_FUNCTION_NAME 1
#define INDEX_ON_FUNCTION_PARAM 2

void gtree_view_column_function(gpointer data, unsigned int type){
	zval retval;
	ze_gtree_view_column_object * b = data;
	zval * array, * value, * zv;
	zval args[2];
	if(type){
		zend_object * this = php_gtree_view_column_reverse_object(b);
		ZVAL_OBJ(&args[0], this);
		array = &b->tree_view_column_ptr->signals;
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


void gtree_view_column_func_clicked(GtkTreeViewColumn* tree, gpointer data){
	gtree_view_column_function(data, gsignal_gtree_view_column_clicked);
}

/***************/
/* PHP Methods */
/***************/

GTREE_VIEW_COLUMN_METHOD(__construct){
	ze_gtree_view_column_object *ze_obj = NULL;
	zval * self = getThis();
	if(self){
		ze_obj = Z_GTREE_VIEW_COLUMN_P(self);
		ze_obj->tree_view_column_ptr = gtree_view_column_new();
		ze_obj->tree_view_column_ptr->intern = gtk_tree_view_column_new();
	}
}



GTREE_VIEW_COLUMN_METHOD(on){
	zval * function, * data,* narray, * this, * param = NULL;
	long val;
	ze_gtree_view_column_object *ze_obj = NULL;
	if(zend_parse_parameters(ZEND_NUM_ARGS(), "lz|z", &val ,&function, &param) == FAILURE)
		return;
	if(!zend_is_callable(function, 0, NULL))
		zend_error(E_ERROR, "Function requires callable argument");
	this = getThis();
	if(!this)
		RETURN_NULL();
	ze_obj = Z_GTREE_VIEW_COLUMN_P(this);
	switch(val){
		case gsignal_gtree_view_column_clicked :
			break;
		default :
			zend_error(E_ERROR, "Signal unknown");
	}
	zval * data_to_insert = ecalloc(1,sizeof(zval));
	array_init(data_to_insert);
	zend_hash_index_add(Z_ARRVAL_P(data_to_insert), INDEX_ON_FUNCTION_NAME, function);
	Z_TRY_ADDREF_P(function);
	if(param){
		zend_hash_index_add(Z_ARRVAL_P(data_to_insert), INDEX_ON_FUNCTION_PARAM, param);
		Z_TRY_ADDREF_P(param);
	}
	data = zend_hash_index_find(Z_ARRVAL_P(&ze_obj->tree_view_column_ptr->signals), val);
	if(data == NULL){
		narray = ecalloc(1,sizeof(zval));
		array_init(narray);
		zend_hash_index_add(Z_ARRVAL_P(&ze_obj->tree_view_column_ptr->signals), val, narray);
		zend_hash_next_index_insert(Z_ARRVAL_P(narray), data_to_insert);
		switch(val){
			case gsignal_gtree_view_column_clicked :
				g_signal_connect(ze_obj->tree_view_column_ptr->intern, GSIGNAL_GTREE_VIEW_COLUMN_CLICKED, G_CALLBACK (gtree_view_column_func_clicked), (gpointer) ze_obj);
				break;
		}
	}else{
		zend_hash_next_index_insert(Z_ARRVAL_P(data), data_to_insert);
	}
}

GTREE_VIEW_COLUMN_METHOD(addCellRendererText){
	ze_gtree_view_column_object *ze_obj = NULL;
	zval * self = getThis();
	zval * renderer;
	long col;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "Ol", &renderer, gcell_renderer_get_class_entry(), &col) == FAILURE)
		return;
	if(self){
		ze_obj = Z_GTREE_VIEW_COLUMN_P(self);
		ze_gcell_renderer_object * ze_rend = Z_GCELL_RENDERER_P(renderer);
		gtk_tree_view_column_pack_start (ze_obj->tree_view_column_ptr->intern, ze_rend->cell_renderer_ptr->intern, 1);
		gtk_tree_view_column_add_attribute(ze_obj->tree_view_column_ptr->intern, ze_rend->cell_renderer_ptr->intern, "text", col);
	}
}

static const zend_function_entry gtree_view_column_class_functions[] = {
	PHP_ME(GTreeViewColumn, on                 , arginfo_pggi_on                                 , ZEND_ACC_PUBLIC)
	PHP_ME(GTreeViewColumn, __construct        , arginfo_pggi_void                               , ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(GTreeViewColumn, addCellRendererText, arginfo_gtree_view_column_add_cell_renderer_text, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *gtree_view_column_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gtree_view_column_object * intern = Z_GTREE_VIEW_COLUMN_P(object);
	gtree_view_column_ptr b = intern->tree_view_column_ptr;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkTreeViewColumn * tree = GTK_TREE_VIEW_COLUMN(b->intern);
	if(!strcmp(member_val, GTREE_VIEW_COLUMN_TITLE)){
		const char * tmp;
		tmp = gtk_tree_view_column_get_title(tree);
		if(tmp){
			ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
		}else{
			ZVAL_NULL(rv);
		}
	}else if(!strcmp(member_val, GTREE_VIEW_COLUMN_SIZING)){
		ZVAL_LONG(rv, gtk_tree_view_column_get_sizing(tree));
	}else if(!strcmp(member_val, GTREE_VIEW_COLUMN_SPACING)){
		ZVAL_LONG(rv, gtk_tree_view_column_get_spacing(tree));
	}else if(!strcmp(member_val, GTREE_VIEW_COLUMN_FIXED_WIDTH)){
		ZVAL_LONG(rv, gtk_tree_view_column_get_fixed_width(tree));
	}else if(!strcmp(member_val, GTREE_VIEW_COLUMN_MIN_WIDTH)){
		ZVAL_LONG(rv, gtk_tree_view_column_get_min_width(tree));
	}else if(!strcmp(member_val, GTREE_VIEW_COLUMN_MAX_WIDTH)){
		ZVAL_LONG(rv, gtk_tree_view_column_get_max_width(tree));
	}else if(!strcmp(member_val, GTREE_VIEW_COLUMN_EXPAND)){
		ZVAL_BOOL(rv, gtk_tree_view_column_get_expand(tree));
	}else if(!strcmp(member_val, GTREE_VIEW_COLUMN_CLICKABLE)){
		ZVAL_BOOL(rv, gtk_tree_view_column_get_clickable(tree));
	}else if(!strcmp(member_val, GTREE_VIEW_COLUMN_REORDERABLE)){
		ZVAL_BOOL(rv, gtk_tree_view_column_get_reorderable(tree));
	}else if(!strcmp(member_val, GTREE_VIEW_COLUMN_VISIBLE)){
		ZVAL_BOOL(rv, gtk_tree_view_column_get_visible(tree));
	}else if(!strcmp(member_val, GTREE_VIEW_COLUMN_RESIZABLE)){
		ZVAL_BOOL(rv, gtk_tree_view_column_get_resizable(tree));
	}else if(!strcmp(member_val, GTREE_VIEW_COLUMN_ALIGNMENT)){
		ZVAL_DOUBLE(rv, gtk_tree_view_column_get_resizable(tree));
	}
	return std_object_handlers.read_property(object, member, type, cache_slot, rv);
}

HashTable *gtree_view_column_get_properties(zval *object){
	G_H_UPDATE_INIT(zend_std_get_properties(object));
	ze_gtree_view_column_object * intern = Z_GTREE_VIEW_COLUMN_P(object);
	gtree_view_column_ptr b = intern->tree_view_column_ptr;
	GtkTreeViewColumn * tree = GTK_TREE_VIEW_COLUMN(b->intern);

	G_H_UPDATE_STRING(GTREE_VIEW_COLUMN_TITLE      , gtk_tree_view_column_get_title      (tree));
	G_H_UPDATE_BOOL	 (GTREE_VIEW_COLUMN_EXPAND     , gtk_tree_view_column_get_expand     (tree));
	G_H_UPDATE_BOOL	 (GTREE_VIEW_COLUMN_CLICKABLE  , gtk_tree_view_column_get_clickable  (tree));
	G_H_UPDATE_DOUBLE(GTREE_VIEW_COLUMN_ALIGNMENT  , gtk_tree_view_column_get_alignment  (tree));
	G_H_UPDATE_BOOL	 (GTREE_VIEW_COLUMN_REORDERABLE, gtk_tree_view_column_get_reorderable(tree));
	G_H_UPDATE_LONG	 (GTREE_VIEW_COLUMN_SPACING    , gtk_tree_view_column_get_spacing    (tree));
	G_H_UPDATE_BOOL	 (GTREE_VIEW_COLUMN_VISIBLE    , gtk_tree_view_column_get_visible    (tree));
	G_H_UPDATE_BOOL	 (GTREE_VIEW_COLUMN_RESIZABLE  , gtk_tree_view_column_get_resizable  (tree));
	G_H_UPDATE_LONG	 (GTREE_VIEW_COLUMN_SIZING     , gtk_tree_view_column_get_sizing     (tree));
	G_H_UPDATE_LONG	 (GTREE_VIEW_COLUMN_FIXED_WIDTH, gtk_tree_view_column_get_fixed_width(tree));
	G_H_UPDATE_LONG	 (GTREE_VIEW_COLUMN_MIN_WIDTH  , gtk_tree_view_column_get_min_width  (tree));
	G_H_UPDATE_LONG	 (GTREE_VIEW_COLUMN_MAX_WIDTH  , gtk_tree_view_column_get_max_width  (tree));
	return G_H_UPDATE_RETURN;
}

void gtree_view_column_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_gtree_view_column_object * intern = Z_GTREE_VIEW_COLUMN_P(object);
	gtree_view_column_ptr b = intern->tree_view_column_ptr;
	long tmp_l;
	int tmp_b;
	int tmp_d;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkTreeViewColumn * tree = GTK_TREE_VIEW_COLUMN(b->intern);
	switch(Z_TYPE_P(value)){
		case IS_STRING :
			if(!strcmp(member_val, GTREE_VIEW_COLUMN_TITLE)){
				gtk_tree_view_column_set_title(tree, Z_STRVAL_P(value));
			}else
				std_object_handlers.write_property(object, member, value, cache_slot);
			break;
		case IS_LONG :
			tmp_l = Z_LVAL_P(value);
			if(!strcmp(member_val, GTREE_VIEW_COLUMN_SIZING)){
				switch(tmp_l){
					case GTK_TREE_VIEW_COLUMN_GROW_ONLY :
					case GTK_TREE_VIEW_COLUMN_AUTOSIZE  :
					case GTK_TREE_VIEW_COLUMN_FIXED     :
						gtk_tree_view_column_set_sizing(tree, tmp_l);
						break;
					default :
						zend_throw_exception_ex(pggi_exception_get(), 0, "the sizing needs to be a SIZING_*");
						break;
				}			
			}else if(!strcmp(member_val, GTREE_VIEW_COLUMN_SPACING)){
				gtk_tree_view_column_set_spacing(tree, tmp_l);
			}else if(!strcmp(member_val, GTREE_VIEW_COLUMN_FIXED_WIDTH)){
				gtk_tree_view_column_set_fixed_width(tree, tmp_l);
			}else if(!strcmp(member_val, GTREE_VIEW_COLUMN_MIN_WIDTH)){
				gtk_tree_view_column_set_min_width(tree, tmp_l);
			}else if(!strcmp(member_val, GTREE_VIEW_COLUMN_MAX_WIDTH)){
				gtk_tree_view_column_set_max_width(tree, tmp_l);
			}else
				std_object_handlers.write_property(object, member, value, cache_slot);
			break;
		case IS_TRUE  :
		case IS_FALSE :
			tmp_b = (Z_TYPE_P(value) == IS_TRUE ? 1 : 0);
			if(!strcmp(member_val, GTREE_VIEW_COLUMN_EXPAND)){
				gtk_tree_view_column_set_expand(tree, tmp_b);
			}else if(!strcmp(member_val, GTREE_VIEW_COLUMN_CLICKABLE)){
				gtk_tree_view_column_set_clickable(tree, tmp_b);
			}else if(!strcmp(member_val, GTREE_VIEW_COLUMN_REORDERABLE)){
				gtk_tree_view_column_set_reorderable(tree, tmp_b);
			}else if(!strcmp(member_val, GTREE_VIEW_COLUMN_VISIBLE)){
				gtk_tree_view_column_set_visible(tree, tmp_b);
			}else if(!strcmp(member_val, GTREE_VIEW_COLUMN_RESIZABLE)){
				gtk_tree_view_column_set_resizable(tree, tmp_b);
			}else
				std_object_handlers.write_property(object, member, value, cache_slot);
			break;
		case IS_DOUBLE :
			if(!strcmp(member_val, GTREE_VIEW_COLUMN_ALIGNMENT)){
				tmp_d = Z_DVAL_P(value);
				gtk_tree_view_column_set_resizable(tree, tmp_d);
			}else
				std_object_handlers.write_property(object, member, value, cache_slot);
			break;
			
		default:
			std_object_handlers.write_property(object, member, value, cache_slot);
	}
}

/************************************/
/* GTreeViewColumn Class Initialization */
/************************************/

#define DECLARE_GTREE_VIEW_COLUMN_PROP(name) \
DECLARE_CLASS_PROPERTY(gtree_view_column_class_entry_ce, name)

#define GTREE_VIEW_COLUMN_CONSTANT(name, value) \
zend_declare_class_constant_long(gtree_view_column_class_entry_ce, name, sizeof(name)-1, value);


void gtree_view_column_init(int module_number){
	zend_class_entry ce;
	le_gtree_view_column = zend_register_list_destructors_ex(gtree_view_column_free_resource, NULL, "PGGI\\GTreeViewColumn", module_number);

	memcpy(&gtree_view_column_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	gtree_view_column_object_handlers.offset         = XtOffsetOf(ze_gtree_view_column_object, std);
	gtree_view_column_object_handlers.free_obj       = gtree_view_column_object_free_storage;
	gtree_view_column_object_handlers.clone_obj      = NULL;
	gtree_view_column_object_handlers.read_property  = gtree_view_column_read_property;
	gtree_view_column_object_handlers.get_properties = gtree_view_column_get_properties;
	gtree_view_column_object_handlers.write_property = gtree_view_column_write_property;
	INIT_CLASS_ENTRY(ce, "PGGI\\GTreeViewColumn", gtree_view_column_class_functions);
	ce.create_object = gtree_view_column_object_new;
	gtree_view_column_class_entry_ce = zend_register_internal_class(&ce);

	DECLARE_GTREE_VIEW_COLUMN_PROP(GTREE_VIEW_COLUMN_SPACING    );
	DECLARE_GTREE_VIEW_COLUMN_PROP(GTREE_VIEW_COLUMN_VISIBLE    );
	DECLARE_GTREE_VIEW_COLUMN_PROP(GTREE_VIEW_COLUMN_RESIZABLE  );
	DECLARE_GTREE_VIEW_COLUMN_PROP(GTREE_VIEW_COLUMN_SIZING     );
	DECLARE_GTREE_VIEW_COLUMN_PROP(GTREE_VIEW_COLUMN_FIXED_WIDTH);
	DECLARE_GTREE_VIEW_COLUMN_PROP(GTREE_VIEW_COLUMN_MIN_WIDTH  );
	DECLARE_GTREE_VIEW_COLUMN_PROP(GTREE_VIEW_COLUMN_MAX_WIDTH  );
	DECLARE_GTREE_VIEW_COLUMN_PROP(GTREE_VIEW_COLUMN_TITLE      );
	DECLARE_GTREE_VIEW_COLUMN_PROP(GTREE_VIEW_COLUMN_EXPAND     );
	DECLARE_GTREE_VIEW_COLUMN_PROP(GTREE_VIEW_COLUMN_CLICKABLE  );
	DECLARE_GTREE_VIEW_COLUMN_PROP(GTREE_VIEW_COLUMN_ALIGNMENT  );
	DECLARE_GTREE_VIEW_COLUMN_PROP(GTREE_VIEW_COLUMN_REORDERABLE);

	GTREE_VIEW_COLUMN_CONSTANT("SIZING_GROW_ONLY", GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	GTREE_VIEW_COLUMN_CONSTANT("SIZING_AUTOSIZE" , GTK_TREE_VIEW_COLUMN_AUTOSIZE );
	GTREE_VIEW_COLUMN_CONSTANT("SIZING_FIXED"    , GTK_TREE_VIEW_COLUMN_FIXED    );
}



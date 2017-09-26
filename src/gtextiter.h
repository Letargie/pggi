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


#ifndef __GTEXT_ITER_DEF__
#define __GTEXT_ITER_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "hub.h"

/***********************************/
/* GTextIter Intern Data Structure */
/***********************************/

typedef struct{
    GtkTextIter * intern;
	zend_object * buffer;
} * gtext_iter_ptr, gtext_iter_t;

typedef struct{
    gtext_iter_ptr iter_ptr;
    zend_object std;
} ze_gtext_iter_object;

#define GTEXT_ITER_OFFSET              "offset"
#define GTEXT_ITER_LINE                "line"
#define GTEXT_ITER_LINE_OFFSET         "lineOffset"
#define GTEXT_ITER_LINE_INDEX          "index"
#define GTEXT_ITER_VISIBLE_LINE_INDEX  "lineIndex"
#define GTEXT_ITER_VISIBLE_LINE_OFFSET "lineOffset"

ZEND_BEGIN_ARG_INFO_EX(arginfo_text_iter_interval, 0, 0, 2)
	ZEND_ARG_INFO(0, begin)
	ZEND_ARG_INFO(0, end)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_text_iter_mov_mult, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0)
ZEND_END_ARG_INFO()


/****************************/
/* Memory handling function */
/****************************/

static inline ze_gtext_iter_object *php_gtext_iter_fetch_object(zend_object *obj){
	return (ze_gtext_iter_object *)((char*)(obj) - XtOffsetOf(ze_gtext_iter_object, std));
}

inline zend_object *php_gtext_iter_reverse_object(ze_gtext_iter_object *obj){
	return (zend_object *)((char*)(obj) + sizeof(gtext_iter_ptr));
}

#define Z_GTEXT_ITER_P(zv) php_gtext_iter_fetch_object(Z_OBJ_P((zv)))

zend_object *gtext_iter_object_new(zend_class_entry *class_type);

void gtext_iter_dtor(gtext_iter_ptr intern);

void gtext_iter_object_free_storage(zend_object *object);
void gtext_iter_free_resource(zend_resource *rsrc);

/***************/
/* PHP Methods */
/***************/

#define GTEXT_ITER_METHOD(name) \
PHP_METHOD(GTextIter, name)

GTEXT_ITER_METHOD(__construct                   );

GTEXT_ITER_METHOD(getBuffer                     );

GTEXT_ITER_METHOD(getChar                       );
GTEXT_ITER_METHOD(getSlice                      );
GTEXT_ITER_METHOD(getText                       );
GTEXT_ITER_METHOD(getVisibleSlice               );
GTEXT_ITER_METHOD(getVisibleText                );

GTEXT_ITER_METHOD(editable                      );
GTEXT_ITER_METHOD(canInsert                     );

GTEXT_ITER_METHOD(startsWord                    );
GTEXT_ITER_METHOD(endsWord                      );
GTEXT_ITER_METHOD(insideWord                    );
GTEXT_ITER_METHOD(startsLine                    );
GTEXT_ITER_METHOD(endsLine                      );
GTEXT_ITER_METHOD(startsSentence                );
GTEXT_ITER_METHOD(endsSentence                  );
GTEXT_ITER_METHOD(insideSentence                );
GTEXT_ITER_METHOD(isCursorPosition              );
GTEXT_ITER_METHOD(isEnd                         );
GTEXT_ITER_METHOD(isStart                       );

GTEXT_ITER_METHOD(getCharsInLine                );
GTEXT_ITER_METHOD(getBytesInLine                );

GTEXT_ITER_METHOD(forwardChar                   );
GTEXT_ITER_METHOD(backwardChar                  );
GTEXT_ITER_METHOD(forwardChars                  );
GTEXT_ITER_METHOD(backwardChars                 );
GTEXT_ITER_METHOD(forwardLine                   );
GTEXT_ITER_METHOD(backwardLine                  );
GTEXT_ITER_METHOD(forwardLines                  );
GTEXT_ITER_METHOD(backwardLines                 );
GTEXT_ITER_METHOD(backwardWordStart             );
GTEXT_ITER_METHOD(forwardWordEnd                );
GTEXT_ITER_METHOD(backwardWordStarts            );
GTEXT_ITER_METHOD(forwardWordEnds               );
GTEXT_ITER_METHOD(backwardCursorPosition        );
GTEXT_ITER_METHOD(forwardCursorPosition         );
GTEXT_ITER_METHOD(backwardCursorPositions       );
GTEXT_ITER_METHOD(forwardCursorPositions        );
GTEXT_ITER_METHOD(backwardSentenceStart         );
GTEXT_ITER_METHOD(forwardSentenceEnd            );
GTEXT_ITER_METHOD(backwardSentenceStarts        );
GTEXT_ITER_METHOD(forwardSentenceEnds           );
GTEXT_ITER_METHOD(backwardVisibleWordStart      );
GTEXT_ITER_METHOD(forwardVisibleWordEnd         );
GTEXT_ITER_METHOD(backwardVisibleWordStarts     );
GTEXT_ITER_METHOD(forwardVisibleWordEnds        );
GTEXT_ITER_METHOD(backwardVisibleCursorPosition );
GTEXT_ITER_METHOD(forwardVisibleCursorPosition  );
GTEXT_ITER_METHOD(backwardVisibleCursorPositions);
GTEXT_ITER_METHOD(forwardVisibleCursorPositions );
GTEXT_ITER_METHOD(forwardVisibleLine            );
GTEXT_ITER_METHOD(backwardVisibleLine           );
GTEXT_ITER_METHOD(forwardVisibleLines           );
GTEXT_ITER_METHOD(backwardVisibleLines          );
GTEXT_ITER_METHOD(forwardEnd                    );
GTEXT_ITER_METHOD(forwardLineEnd                );

GTEXT_ITER_METHOD(forwardFindChar               );
GTEXT_ITER_METHOD(backwardFindChar              );
GTEXT_ITER_METHOD(forwardSearch                 );
GTEXT_ITER_METHOD(backwardSearch                );

GTEXT_ITER_METHOD(equals                        );
GTEXT_ITER_METHOD(compare                       );
GTEXT_ITER_METHOD(inRange                       );
GTEXT_ITER_METHOD(order                         );


/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *gtext_iter_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * get properties handling function
 */
HashTable *gtext_iter_get_properties(zval *object);

/*==========================================================================*/
/** 
 * write property handling function
 */
void gtext_iter_write_property(zval *object, zval *member, zval *value, void **cache_slot);

/**********************************/
/* GTextIter Class Initialization */
/**********************************/

void gtext_iter_init(int module_number);

/* __GTEXT_BUFFER_DEF__ */

#endif

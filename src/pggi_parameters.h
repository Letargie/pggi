#ifndef __PGGI_PARAMETERS_DEF__
#define __PGGI_PARAMETERS_DEF__

ZEND_BEGIN_ARG_INFO(arginfo_pggi_void, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_gapplication_on, 0, 0, 1)
	ZEND_ARG_INFO(0, callback)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_gwindow_set_title, 0)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_gwindow_set_default_size, 0)
	ZEND_ARG_INFO(0, x)
	ZEND_ARG_INFO(0, y)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_gwidget_object_new, 0)
	ZEND_ARG_INFO(0, app)
ZEND_END_ARG_INFO()


#endif

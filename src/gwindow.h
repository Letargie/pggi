#ifndef __GWINDOW_DEF__
#define __GWINDOW_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "pggi_parameters.h"
#include "gapplication.h"
#include "gwidget.h"

PHP_METHOD(GWindow, __construct);
PHP_METHOD(GWindow, setTitle);
PHP_METHOD(GWindow, setDefaultSize);
PHP_METHOD(GWindow, close);

void gwindow_init(int module_number);

#endif

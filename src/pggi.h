#ifndef __PGGI_DEF__
#define __PGGI_DEF__

#define PGGI_VERSION "1.0"
#define PGGI_EXTNAME "pggi"

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "gapplication.h"
#include "pggi_parameters.h"
#include "gwidget.h"
#include "gwindow.h"

PHP_FUNCTION(pggi_main);

PHP_FUNCTION(pggi_main_quit);


#define pphext_pggi_ptr &pggi_module_entry

#endif

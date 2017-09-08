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

#ifndef __PGGI_DEF__
#define __PGGI_DEF__

#define PGGI_VERSION "1.0"
#define PGGI_EXTNAME "pggi"

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "gexception.h"
#include "gapplication.h"
#include "hub.h"
#include "gwidget.h"
#include "gcontainer.h"
#include "gwindow.h"
#include "glabel.h"
#include "gimage.h"
#include "gbox.h"
#include "gbutton.h"
#include "gbuttonbox.h"
#include "gmenushell.h"
#include "gmenubar.h"
#include "gmenu.h"
#include "gmenuitem.h"
#include "gtextview.h"
#include "gtextbuffer.h"
#include "gscrollwindow.h"
#include "gcombobox.h"
#include "gcomboboxtext.h"
#include "gdialog.h"
#include "gaboutdialog.h"
#include "ggrid.h"
#include "gtextiter.h"
#include "gtreeiter.h"
#include "gtreestore.h"
#include "gtreeview.h"

PHP_FUNCTION(buildUI);

#define pphext_pggi_ptr &pggi_module_entry

#endif

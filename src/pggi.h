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

#define PGGI_VERSION "0.2.0"
#define PGGI_EXTNAME "pggi"

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"

#include "gdk/rgba.h"

#include "exceptions/gexception.h"
#include "applications/gapplication.h"
#include "commons/hub.h"
#include "abstracts/gwidget.h"
#include "abstracts/gcontainer.h"
#include "applications/gwindow.h"
#include "display/glabel.h"
#include "display/gimage.h"
#include "containers/gbox.h"
#include "buttons/gbutton.h"
#include "containers/gbuttonbox.h"
#include "abstracts/gmenushell.h"
#include "menus/gmenubar.h"
#include "menus/gmenu.h"
#include "menus/gmenuitem.h"
#include "text/gtextview.h"
#include "text/gtextbuffer.h"
#include "windows/gscrollwindow.h"
#include "containers/gcombobox.h"
#include "containers/gcomboboxtext.h"
#include "windows/gdialog.h"
#include "selectors/gaboutdialog.h"
#include "tree/ggrid.h"
#include "text/gtextiter.h"
#include "tree/gtreeiter.h"
#include "tree/gtreestore.h"
#include "tree/gcellrenderer.h"
#include "tree/gcellrenderertext.h"
#include "tree/gtreeviewcolumn.h"
#include "tree/gtreeselection.h"
#include "tree/gtreeview.h"
#include "selectors/gfilechooserdialog.h"
#include "entries/gentrybuffer.h"
#include "entries/gentry.h"
#include "gdk/gevent.h"
#include "gdk/geventkey.h"
#include "theming/gstylecontext.h"
#include "theming/gcssprovider.h"
#include "gdk/gscreen.h"
#include "windows/gmessagedialog.h"
#include "display/gdrawingarea.h"
#include "display/gspinner.h"
#include "cairo/exception.h"
#include "cairo/pattern.h"
#include "cairo/pattern_solid.h"
#include "cairo/pattern_surface.h"
#include "cairo/context.h"
#include "cairo/surface.h"
#include "cairo/imagesurface.h"

#include "pango/context.h"
#include "pango/exception.h"
#include "pango/fontdescription.h"
#include "pango/layout.h"


#define pphext_pggi_ptr &pggi_module_entry

#endif

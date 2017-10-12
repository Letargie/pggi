PHP_ARG_ENABLE(pggi, pggi module,
[  --enable-pggi Enable pggi module.])

if test "$PHP_PGGI" != "no"; then
	AC_DEFINE(HAVE_PHP_PGGI, 1, [Have PHP_PGGI library])
	AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
	AC_MSG_CHECKING(for gtk+-3.0)
	if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists gtk+-3.0; then
		GTK_CFLAGS=`$PKG_CONFIG --cflags gtk+-3.0`
		GTK_LIBDIR=`$PKG_CONFIG --libs gtk+-3.0`
		GTK_VERSION=`$PKG_CONFIG --version gtk+-3.0`
		AC_MSG_RESULT(version $GTK_VERSION)
		PHP_EVAL_LIBLINE($GTK_LIBDIR, PGGI_SHARED_LIBADD)
		PHP_EVAL_INCLINE($GTK_CFLAGS)
	else
		AC_MSG_ERROR(system gtk not found)
	fi
	CFLAGS="$CFLAGS -Wall -Werror=format-security"
	PHP_NEW_EXTENSION(pggi,
		src/cairo/exception.c              \
		src/cairo/context.c                \
		src/cairo/surface.c                \
		src/cairo/imagesurface.c           \
		src/gdk/rgba.c                     \
		src/gdk/gevent.c                   \
		src/gdk/geventkey.c                \
    	src/pggi.c                         \
    	src/applications/gapplication.c    \
		src/abstracts/gwidget.c            \
		src/abstracts/gcontainer.c     	   \
		src/applications/gwindow.c 		   \
		src/display/glabel.c		       \
		src/display/gimage.c		       \
		src/buttons/gbutton.c              \
		src/containers/gbuttonbox.c        \
		src/containers/gbox.c              \
		src/exceptions/gexception.c        \
		src/abstracts/gmenushell.c         \
		src/menus/gmenubar.c               \
		src/menus/gmenu.c                  \
		src/menus/gmenuitem.c              \
		src/text/gtextview.c               \
		src/text/gtextbuffer.c             \
		src/windows/gscrollwindow.c        \
		src/containers/gcombobox.c         \
		src/containers/gcomboboxtext.c     \
		src/windows/gdialog.c              \
		src/selectors/gaboutdialog.c       \
		src/tree/ggrid.c                   \
		src/text/gtextiter.c	           \
		src/tree/gtreeiter.c               \
		src/tree/gtreestore.c              \
		src/tree/gtreeview.c               \
		src/selectors/gfilechooserdialog.c \
		src/tree/gcellrenderer.c           \
		src/tree/gcellrenderertext.c       \
		src/tree/gtreeviewcolumn.c	       \
		src/tree/gtreeselection.c          \
		src/entries/gentrybuffer.c         \
		src/entries/gentry.c               \
		src/theming/gcssprovider.c         \
		src/theming/gstylecontext.c        \
		src/gdk/gscreen.c                  \
		src/windows/gmessagedialog.c       \
		src/display/gdrawingarea.c         \
    	, $ext_shared)
	PHP_ADD_BUILD_DIR($ext_builddir/src)
	PHP_SUBST(PGGI_SHARED_LIBADD)
fi


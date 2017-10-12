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
		src/pggi.c                         \
		src/applications/gapplication.c    \
		src/applications/gwindow.c         \
		src/abstracts/gcontainer.c         \
		src/abstracts/gmenushell.c         \
		src/abstracts/gwidget.c            \
		src/buttons/gbutton.c              \
		src/cairo/exception.c              \
		src/cairo/context.c                \
		src/cairo/surface.c                \
		src/cairo/imagesurface.c           \
		src/containers/gbox.c              \
		src/containers/gbuttonbox.c        \
		src/containers/gcombobox.c         \
		src/containers/gcomboboxtext.c     \
		src/display/gdrawingarea.c         \
		src/display/gimage.c               \
		src/display/glabel.c               \
		src/entries/gentry.c               \
		src/entries/gentrybuffer.c         \
		src/exceptions/gexception.c        \
		src/gdk/gevent.c                   \
		src/gdk/geventkey.c                \
		src/gdk/gscreen.c                  \
		src/gdk/rgba.c                     \
		src/menus/gmenubar.c               \
		src/menus/gmenu.c                  \
		src/menus/gmenuitem.c              \
		src/selectors/gaboutdialog.c       \
		src/selectors/gfilechooserdialog.c \
		src/text/gtextbuffer.c             \
		src/text/gtextiter.c               \
		src/text/gtextview.c               \
		src/theming/gcssprovider.c         \
		src/theming/gstylecontext.c        \
		src/tree/gcellrenderer.c           \
		src/tree/gcellrenderertext.c       \
		src/tree/ggrid.c                   \
		src/tree/gtreeiter.c               \
		src/tree/gtreeselection.c          \
		src/tree/gtreestore.c              \
		src/tree/gtreeview.c               \
		src/tree/gtreeviewcolumn.c         \
		src/windows/gdialog.c              \
		src/windows/gmessagedialog.c       \
		src/windows/gscrollwindow.c        \
		, $ext_shared)
	PHP_ADD_BUILD_DIR($ext_builddir/src)
	PHP_ADD_BUILD_DIR($ext_builddir/src/applications)
	PHP_ADD_BUILD_DIR($ext_builddir/src/abstracts)
	PHP_ADD_BUILD_DIR($ext_builddir/src/buttons)
	PHP_ADD_BUILD_DIR($ext_builddir/src/cairo)
	PHP_ADD_BUILD_DIR($ext_builddir/src/containers)
	PHP_ADD_BUILD_DIR($ext_builddir/src/display)
	PHP_ADD_BUILD_DIR($ext_builddir/src/entries)
	PHP_ADD_BUILD_DIR($ext_builddir/src/exceptions)
	PHP_ADD_BUILD_DIR($ext_builddir/src/gdk)
	PHP_ADD_BUILD_DIR($ext_builddir/src/menus)
	PHP_ADD_BUILD_DIR($ext_builddir/src/selectors)
	PHP_ADD_BUILD_DIR($ext_builddir/src/text)
	PHP_ADD_BUILD_DIR($ext_builddir/src/theming)
	PHP_ADD_BUILD_DIR($ext_builddir/src/tree)
	PHP_ADD_BUILD_DIR($ext_builddir/src/windows)
	PHP_SUBST(PGGI_SHARED_LIBADD)
fi


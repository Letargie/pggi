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
	PHP_NEW_EXTENSION(pggi,
		src/gevent.c             \
		src/geventkey.c          \
    	src/pggi.c               \
    	src/gapplication.c       \
		src/gwidget.c            \
		src/gcontainer.c     	 \
		src/gwindow.c 		     \
		src/glabel.c		     \
		src/gimage.c		     \
		src/gbutton.c            \
		src/gbuttonbox.c         \
		src/gbox.c               \
		src/gexception.c         \
		src/gmenushell.c         \
		src/gmenubar.c           \
		src/gmenu.c              \
		src/gmenuitem.c          \
		src/gtextview.c          \
		src/gtextbuffer.c        \
		src/gscrollwindow.c      \
		src/gcombobox.c          \
		src/gcomboboxtext.c      \
		src/gdialog.c            \
		src/gaboutdialog.c       \
		src/ggrid.c              \
		src/gtextiter.c	         \
		src/gtreeiter.c          \
		src/gtreestore.c         \
		src/gtreeview.c          \
		src/gfilechooserdialog.c \
		src/gcellrenderer.c      \
		src/gcellrenderertext.c  \
		src/gtreeviewcolumn.c	 \
		src/gtreeselection.c     \
		src/gentrybuffer.c       \
		src/gentry.c             \
    	, $ext_shared)
	PHP_ADD_BUILD_DIR($ext_builddir/src)
	PHP_SUBST(PGGI_SHARED_LIBADD)
fi


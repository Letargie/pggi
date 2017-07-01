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
    	src/pggi.c 			\
    	src/gapplication.c 	\
		src/gwidget.c 		\
		src/gcontainer.c 	\
		src/gwindow.c 		\
		src/glabel.c		\
		src/gimage.c		\
		src/gbutton.c       \
		src/gbox.c          \
		src/gexception.c	\
    	, $ext_shared, , -I@ext_srcdir@/pggi/include)
	PHP_ADD_BUILD_DIR($ext_builddir/src)
	PHP_ADD_INCLUDE([$ext_srcdir/include])
	PHP_SUBST(PGGI_SHARED_LIBADD)
fi


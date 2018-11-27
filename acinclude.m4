AC_DEFUN(AC_LIB_MYSQL, [
    AC_ARG_WITH(mysql-lib,
    [  --with-mysql-lib=DIR    Look for MySQL client library in DIR],
    mysql_lib=$withval, mysql_lib="")

        AC_MSG_CHECKING([for libmysqlclient])
        AC_MSG_RESULT()

        mysql_ok=no

        SAVE_LIBS=$LIBS
    
        mysql_lib="$mysql_lib /usr/lib /usr/lib/mysql \
                   /usr/local/lib /usr/local/lib/mysql \
                   /usr/local/mysql/lib"
    
        for dir in $mysql_lib; do
            if test "x$mysql_found" != "xyes"
            then
                AC_CHECK_FILE("$dir/libmysqlclient.so",
                              mysql_found=yes, mysql_found=no)
                if test "x$mysql_found" = "xyes"
                then
                    #LIBS="-L$dir $SAVE_LIBS"
                    LIBS="-L$dir $SAVE_LIBS $LIBZ_LIB"
                    #MYSQL_LIB="$dir/libmysqlclient.so"
                    MYSQL_LIB="$dir/libmysqlclient.so $LIBZ_LIB"
                    AC_SUBST(MYSQL_LIB)
                    AC_CHECK_LIB(mysqlclient, mysql_real_connect,
                                 mysql_ok=yes, mysql_ok=no)
                fi
            fi
        done

        if test "x$mysql_ok" != "xyes"
        then
            AC_MSG_ERROR([Could not find libmysqlclient in '$mysql_lib'])
        fi
])


AC_DEFUN(AC_HEADER_MYSQL, [
    AC_ARG_WITH(mysql-include,
    [  --with-mysql-include=DIR
                          Look for MySQL include files in DIR],
    mysql_include=$withval, mysql_include="")

        AC_MSG_CHECKING([for mysql.h])
        AC_MSG_RESULT()

        mysql_found=no
        mysql_include="$mysql_include /usr/include /usr/include/mysql \
                       /usr/local/include /usr/local/include/mysql \
                       /usr/local/mysql/include"
    
        for dir in $mysql_include; do
            if test "x$mysql_found" != "xyes"
            then
                AC_CHECK_HEADER($dir/mysql.h, 
                                mysql_found=yes, mysql_found=no)
                if test "x$mysql_found" = "xyes"
                then
                   AC_MSG_CHECKING("if MySQL Version $dir is >= 4.0.0")
                   CPPFLAGS="$CPPFLAGS -I$dir"
                   AC_TRY_CPP([
                   #include <mysql_version.h>
                   #if MYSQL_VERSION_ID < 40000 
                     #error
                   #endif
                   ], mysql_version_ok=yes, mysql_version_ok=no)

                   if test "x$mysql_version_ok" != "xno"
                   then
                      MYSQL_INCLUDE="$dir"
                      AC_SUBST(MYSQL_INCLUDE)
                   fi
                   AC_MSG_RESULT($mysql_version_ok)
                fi
            fi
        done

        if test "x$mysql_found" != "xyes"
        then
            AC_MSG_ERROR([Could not find mysql.h in '$mysql_include'])
        fi

        if test "x$mysql_version_ok" != "xyes"
        then
           echo "Please upgrade your MySQL installation to version 4.0.0 or higher"
           echo "If you do have the correct version of MySQL installed somewhere"
           echo "other than $dir, please specify it as --with-mysql-include= argument"
           exit 1
        fi
])

AC_DEFUN(AC_LIB_Z, [
    AC_CHECK_LIB(z, compress, ssl)
])


AC_INIT(dcpu, version-0.1)

AM_INIT_AUTOMAKE([1.10 no-define])

AC_PROG_CC
AC_PROG_CC_C99

AC_ARG_ENABLE(debug,
AS_HELP_STRING([--enable-debug],
               [enable debugging, default: no]),
[case "${enableval}" in
             yes) debug=true ;;
             no)  debug=false ;;
             *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac],
[debug=false])

AM_CONDITIONAL(DEBUG, test x"$debug" = x"true")

AC_OUTPUT([Makefile
src/Makefile
])


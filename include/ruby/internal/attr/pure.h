#ifndef RBIMPL_ATTR_PURE_H                           /*-*-C++-*-vi:se ft=cpp:*/
#define RBIMPL_ATTR_PURE_H
/**
 * @file
 * @author     Ruby developers <ruby-core@ruby-lang.org>
 * @copyright  This  file  is   a  part  of  the   programming  language  Ruby.
 *             Permission  is hereby  granted,  to  either redistribute  and/or
 *             modify this file, provided that  the conditions mentioned in the
 *             file COPYING are met.  Consult the file for details.
 * @warning    Symbols   prefixed  with   either  `RBIMPL`   or  `rbimpl`   are
 *             implementation details.   Don't take  them as canon.  They could
 *             rapidly appear then vanish.  The name (path) of this header file
 *             is also an  implementation detail.  Do not expect  it to persist
 *             at the place it is now.  Developers are free to move it anywhere
 *             anytime at will.
 * @note       To  ruby-core:  remember  that   this  header  can  be  possibly
 *             recursively included  from extension  libraries written  in C++.
 *             Do not  expect for  instance `__VA_ARGS__` is  always available.
 *             We assume C99  for ruby itself but we don't  assume languages of
 *             extension libraries.  They could be written in C++98.
 * @brief      Defines #RBIMPL_ATTR_PURE.
 */
#include "/workspace/spring-petclinic/pypy_gc/rpython/memory/gc/ruby/include/ruby/internal/compiler_since.h"
#include "/workspace/spring-petclinic/pypy_gc/rpython/memory/gc/ruby/include/ruby/internal/has/attribute.h"
#include "/workspace/spring-petclinic/pypy_gc/rpython/memory/gc/ruby/include/ruby/assert.h"

/** Wraps (or simulates) `__attribute__((pure))` */
#if RBIMPL_HAS_ATTRIBUTE(pure)
# define RBIMPL_ATTR_PURE() __attribute__((__pure__))
#elif RBIMPL_COMPILER_SINCE(SunPro, 5, 10, 0)
# define RBIMPL_ATTR_PURE() _Pragma("does_not_write_global_data")
#else
# define RBIMPL_ATTR_PURE() /* void */
#endif

/** Enables #RBIMPL_ATTR_PURE if and only if. ! #RUBY_DEBUG. */
#if !RUBY_DEBUG
# define RBIMPL_ATTR_PURE_UNLESS_DEBUG() RBIMPL_ATTR_PURE()
#else
# define RBIMPL_ATTR_PURE_UNLESS_DEBUG() /* void */
#endif

#endif /* RBIMPL_ATTR_PURE_H */

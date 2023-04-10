#include <gio/gio.h>

#if defined (__ELF__) && ( __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 6))
# define SECTION __attribute__ ((section (".gresource.simpleMsgDialogGresource"), aligned (8)))
#else
# define SECTION
#endif

static const SECTION union { const guint8 data[2309]; const double alignment; void * const ptr;}  simpleMsgDialogGresource_resource_data = {
  "\107\126\141\162\151\141\156\164\000\000\000\000\000\000\000\000"
  "\030\000\000\000\310\000\000\000\000\000\000\050\006\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\002\000\000\000"
  "\002\000\000\000\004\000\000\000\074\020\013\335\002\000\000\000"
  "\310\000\000\000\023\000\166\000\340\000\000\000\342\001\000\000"
  "\324\265\002\000\377\377\377\377\342\001\000\000\001\000\114\000"
  "\344\001\000\000\350\001\000\000\104\305\242\041\001\000\000\000"
  "\350\001\000\000\020\000\114\000\370\001\000\000\010\002\000\000"
  "\120\133\124\114\002\000\000\000\010\002\000\000\025\000\166\000"
  "\040\002\000\000\357\005\000\000\065\154\360\033\002\000\000\000"
  "\357\005\000\000\045\000\166\000\030\006\000\000\054\010\000\000"
  "\063\220\027\132\002\000\000\000\054\010\000\000\034\000\166\000"
  "\110\010\000\000\004\011\000\000\163\151\155\160\154\145\115\163"
  "\147\104\151\141\154\157\147\056\143\163\163\000\000\000\000\000"
  "\106\002\000\000\001\000\000\000\170\332\235\221\261\156\303\060"
  "\014\104\347\350\053\004\164\013\020\244\110\201\016\366\330\256"
  "\235\374\001\201\144\321\262\120\232\114\051\251\256\021\344\337"
  "\253\046\066\072\045\101\273\221\304\335\273\023\264\135\353\227"
  "\246\321\061\115\030\310\353\216\105\307\060\034\020\336\242\177"
  "\015\006\331\353\365\126\051\167\031\217\105\100\251\322\273\307"
  "\303\227\156\014\305\372\244\324\103\001\354\007\210\321\170\330"
  "\243\261\200\263\156\063\102\360\175\221\133\106\127\353\105\012"
  "\042\054\027\266\072\252\225\065\355\273\027\316\344\066\055\043"
  "\113\245\005\134\255\126\363\062\001\042\217\145\267\054\016\312"
  "\341\271\104\107\306\340\264\305\142\255\325\302\045\116\241\233"
  "\156\201\333\336\110\022\310\021\166\277\001\063\345\076\377\043"
  "\103\114\201\351\146\002\013\165\245\057\210\305\014\377\010\011"
  "\124\276\140\060\367\162\306\036\114\372\033\336\346\224\230\176"
  "\160\127\114\117\127\115\125\317\237\040\347\046\147\355\322\202"
  "\305\220\057\257\074\175\003\313\310\303\324\000\050\165\165\141"
  "\171\051\057\000\002\000\000\000\163\151\155\160\154\145\115\163"
  "\147\104\151\141\154\157\147\057\004\000\000\000\000\000\000\000"
  "\003\000\000\000\005\000\000\000\163\151\155\160\154\145\115\163"
  "\147\104\151\141\154\157\147\056\147\154\141\144\145\000\000\000"
  "\372\033\000\000\001\000\000\000\170\332\355\131\115\217\333\066"
  "\020\275\347\127\260\274\006\134\257\275\011\272\005\154\005\150"
  "\323\344\222\264\105\067\155\321\223\061\226\146\045\326\064\251"
  "\220\224\077\376\175\051\313\253\110\153\332\224\055\003\061\220"
  "\134\014\210\232\067\034\316\173\244\071\243\361\233\365\102\220"
  "\045\152\303\225\234\320\341\315\055\045\050\143\225\160\231\116"
  "\350\137\237\336\261\173\372\046\172\061\376\201\061\362\036\045"
  "\152\260\230\220\025\267\031\111\005\044\110\356\156\356\356\157"
  "\106\204\061\147\304\245\105\375\010\061\106\057\010\031\153\374"
  "\134\160\215\206\010\076\233\320\324\316\137\322\057\023\335\335"
  "\214\136\321\301\326\116\315\376\303\330\222\130\200\061\023\372"
  "\336\316\337\162\020\052\245\204\047\023\272\100\143\040\305\335"
  "\120\151\357\020\271\126\071\152\273\041\022\026\070\241\345\057"
  "\215\176\171\170\230\266\254\307\203\047\073\077\054\006\311\036"
  "\125\134\030\032\275\003\141\060\144\077\123\072\101\315\126\074"
  "\261\031\215\136\207\314\055\267\002\051\261\032\244\021\140\141"
  "\046\334\340\006\335\154\017\174\221\013\044\037\253\140\111\267"
  "\150\127\134\046\152\305\162\145\270\165\031\244\121\214\145\266"
  "\203\121\154\162\144\231\043\206\106\211\177\036\303\123\011\142"
  "\147\236\240\261\132\155\050\311\100\046\002\365\226\267\351\002"
  "\270\234\072\062\055\045\146\005\171\216\216\030\251\052\372\234"
  "\207\070\343\042\041\133\362\235\047\266\175\234\320\345\114\255"
  "\167\204\371\110\376\331\275\335\062\134\205\305\234\365\260\066"
  "\077\306\161\303\376\371\132\174\300\045\067\334\245\236\106\237"
  "\164\201\135\000\041\131\370\060\046\207\330\155\030\032\015\137"
  "\171\315\375\031\202\270\144\162\012\032\241\261\162\157\262\012"
  "\153\225\174\236\262\103\370\016\331\153\100\175\001\237\225\310"
  "\163\223\351\303\145\040\234\054\017\211\374\020\112\300\106\025"
  "\226\031\273\051\343\014\140\267\044\264\307\016\247\276\231\367"
  "\251\333\304\323\131\065\374\034\357\213\151\206\302\173\014\374"
  "\213\346\120\160\135\070\154\204\161\212\233\116\064\006\250\074"
  "\025\252\061\106\276\104\303\022\174\204\102\330\200\030\234\203"
  "\101\305\303\336\270\333\147\163\267\321\302\123\342\072\167\047"
  "\130\160\242\175\340\043\027\342\014\330\227\163\371\366\330\262"
  "\274\361\217\007\036\055\366\320\247\124\227\220\347\157\252\227"
  "\072\353\040\276\213\363\212\304\071\374\352\342\304\065\267\227"
  "\220\347\257\316\117\057\201\066\002\371\056\321\053\222\350\350"
  "\253\113\264\274\351\136\102\242\277\317\173\011\264\021\306\265"
  "\013\064\003\163\056\264\234\265\226\365\071\363\236\015\376\246"
  "\166\325\335\045\166\225\057\001\376\305\237\265\160\377\242\117"
  "\051\067\312\130\130\131\362\322\010\145\322\031\025\272\273\171"
  "\062\264\227\235\375\363\246\123\315\073\072\265\162\053\061\127"
  "\131\262\005\332\043\341\322\171\324\267\124\173\210\265\022\002"
  "\223\177\266\255\222\135\377\310\244\037\312\003\272\172\347\336"
  "\320\123\116\342\075\364\265\037\304\345\043\244\140\221\111\260"
  "\205\006\361\304\107\177\107\031\362\064\353\162\314\172\331\362"
  "\063\366\067\307\125\256\264\255\270\132\272\247\077\312\047\037"
  "\370\030\115\117\300\143\141\365\142\250\317\076\011\347\305\237"
  "\233\017\325\275\242\321\004\235\126\127\215\103\056\072\064\106"
  "\053\017\241\110\173\247\352\022\351\072\355\252\145\252\226\352"
  "\156\231\304\337\352\354\062\105\141\220\055\100\317\213\374\374"
  "\245\256\064\364\100\033\024\116\010\320\053\331\013\130\127\033"
  "\237\305\031\150\227\237\341\355\155\330\317\241\373\315\221\133"
  "\166\010\170\000\364\315\227\320\327\177\223\012\255\067\174\047"
  "\152\257\261\365\162\134\365\237\113\215\246\150\115\215\150\015"
  "\023\215\046\127\322\270\150\330\375\323\347\213\126\327\263\145"
  "\036\166\362\123\355\244\321\234\072\321\307\217\265\217\126\377"
  "\340\104\057\257\153\057\255\042\317\343\345\331\240\251\363\027"
  "\273\143\102\203\125\232\314\161\063\241\177\242\373\227\166\145"
  "\154\365\065\307\035\267\102\031\334\175\140\253\171\030\017\032"
  "\137\350\376\007\146\031\076\367\000\050\165\165\141\171\051\110"
  "\157\167\137\164\157\137\143\157\155\160\151\154\145\137\163\157"
  "\156\147\166\151\145\167\137\162\145\163\157\165\162\143\145\163"
  "\056\164\170\164\000\000\000\000\222\005\000\000\001\000\000\000"
  "\170\332\255\123\115\217\233\060\020\275\373\127\114\057\253\366"
  "\000\271\127\352\112\004\046\301\022\340\310\230\315\162\212\050"
  "\161\051\132\076\042\240\111\176\176\315\347\046\154\262\133\251"
  "\353\223\077\306\357\275\231\171\143\263\055\010\006\046\163\067"
  "\324\101\260\014\141\000\107\237\005\334\104\037\250\247\336\204"
  "\215\120\247\371\041\223\156\235\130\151\224\225\011\340\063\232"
  "\201\060\226\016\176\121\213\020\137\112\130\163\131\227\177\252"
  "\130\202\246\301\264\377\125\105\271\074\225\325\013\244\005\354"
  "\345\121\373\055\263\003\041\302\246\012\336\207\016\335\063\134"
  "\004\266\352\366\075\265\022\144\041\254\224\044\062\243\136\127"
  "\003\262\176\316\263\026\007\225\030\352\013\352\255\047\341\335"
  "\107\037\134\043\204\045\302\023\305\055\132\140\370\260\142\216"
  "\303\266\376\167\222\214\040\112\121\023\245\131\075\117\260\007"
  "\146\236\023\166\130\112\231\041\300\103\264\374\266\132\012\164"
  "\031\120\107\264\011\264\161\346\245\152\235\264\157\126\367\060"
  "\011\062\373\204\056\065\220\170\377\132\151\102\222\054\375\251"
  "\305\145\176\110\063\251\215\362\336\350\272\312\136\325\071\221"
  "\205\254\242\106\152\103\072\352\252\211\252\104\066\077\146\037"
  "\047\110\075\046\144\215\036\162\103\250\022\165\311\335\015\065"
  "\365\176\103\070\166\055\372\060\260\055\316\073\304\252\044\036"
  "\270\354\011\027\046\333\204\013\265\341\133\116\005\302\125\027"
  "\357\003\364\162\251\327\033\205\017\105\265\050\107\123\060\036"
  "\022\322\063\214\146\356\133\323\305\104\305\130\354\321\325\267"
  "\034\055\202\326\321\155\143\164\235\220\074\172\221\020\147\062"
  "\052\340\341\001\272\323\121\126\165\132\116\347\226\317\350\134"
  "\260\341\050\104\010\156\140\332\100\105\357\236\101\206\325\012"
  "\036\033\375\325\377\066\332\022\237\005\067\114\241\336\127\234"
  "\271\037\114\331\035\367\202\074\067\125\024\067\157\176\302\142"
  "\166\063\077\353\111\026\355\045\074\302\315\173\155\300\225\373"
  "\317\041\213\353\372\006\225\272\375\137\042\273\074\355\232\162"
  "\067\314\315\256\056\213\344\230\312\323\156\032\040\275\071\067"
  "\212\372\237\342\076\071\353\353\131\175\174\167\224\057\250\377"
  "\002\155\172\330\372\000\050\165\165\141\171\051\163\151\155\160"
  "\154\145\115\163\147\104\151\141\154\157\147\107\162\145\163\157"
  "\165\162\143\145\056\170\155\154\150\001\000\000\001\000\000\000"
  "\170\332\225\220\075\017\302\040\020\100\147\371\025\204\135\320"
  "\315\001\160\061\352\342\246\163\323\320\053\041\241\320\160\375"
  "\372\371\322\241\325\270\230\216\227\173\057\227\173\362\074\065"
  "\236\016\220\320\305\240\330\221\037\030\205\140\142\345\202\125"
  "\354\365\274\356\117\354\254\211\264\011\060\366\311\000\152\102"
  "\351\147\244\155\202\332\115\212\011\164\115\353\341\201\366\342"
  "\112\037\055\323\144\047\153\347\201\232\330\144\010\021\052\305"
  "\272\324\003\323\077\050\267\276\254\100\212\231\336\140\031\304"
  "\277\316\075\216\105\027\213\171\221\201\002\143\260\203\203\261"
  "\130\277\341\335\324\155\275\174\133\154\236\333\055\162\216\042"
  "\326\052\071\230\370\056\106\310\033\113\107\176\235\000\050\165"
  "\165\141\171\051" };

static GStaticResource static_resource = { simpleMsgDialogGresource_resource_data.data, sizeof (simpleMsgDialogGresource_resource_data.data) - 1 /* nul terminator */, NULL, NULL, NULL };

G_MODULE_EXPORT
GResource *simpleMsgDialogGresource_get_resource (void);
GResource *simpleMsgDialogGresource_get_resource (void)
{
  return g_static_resource_get_resource (&static_resource);
}
/* GLIB - Library of useful routines for C programming
 * Copyright (C) 1995-1997  Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Modified by the GLib Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GLib Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GLib at ftp://ftp.gtk.org/pub/gtk/.
 */

#ifndef __G_CONSTRUCTOR_H__
#define __G_CONSTRUCTOR_H__

/*
  If G_HAS_CONSTRUCTORS is true then the compiler support *both* constructors and
  destructors, in a usable way, including e.g. on library unload. If not you're on
  your own.

  Some compilers need #pragma to handle this, which does not work with macros,
  so the way you need to use this is (for constructors):

  #ifdef G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA
  #pragma G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(my_constructor)
  #endif
  G_DEFINE_CONSTRUCTOR(my_constructor)
  static void my_constructor(void) {
   ...
  }

*/

#ifndef __GTK_DOC_IGNORE__

#if  __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 7)

#define G_HAS_CONSTRUCTORS 1

#define G_DEFINE_CONSTRUCTOR(_func) static void __attribute__((constructor)) _func (void);
#define G_DEFINE_DESTRUCTOR(_func) static void __attribute__((destructor)) _func (void);

#elif defined (_MSC_VER) && (_MSC_VER >= 1500)
/* Visual studio 2008 and later has _Pragma */

/*
 * Only try to include gslist.h if not already included via glib.h,
 * so that items using gconstructor.h outside of GLib (such as
 * GResources) continue to build properly.
 */
#ifndef __G_LIB_H__
#include "gslist.h"
#endif

#include <stdlib.h>

#define G_HAS_CONSTRUCTORS 1

/* We do some weird things to avoid the constructors being optimized
 * away on VS2015 if WholeProgramOptimization is enabled. First we
 * make a reference to the array from the wrapper to make sure its
 * references. Then we use a pragma to make sure the wrapper function
 * symbol is always included at the link stage. Also, the symbols
 * need to be extern (but not dllexport), even though they are not
 * really used from another object file.
 */

/* We need to account for differences between the mangling of symbols
 * for x86 and x64/ARM/ARM64 programs, as symbols on x86 are prefixed
 * with an underscore but symbols on x64/ARM/ARM64 are not.
 */
#ifdef _M_IX86
#define G_MSVC_SYMBOL_PREFIX "_"
#else
#define G_MSVC_SYMBOL_PREFIX ""
#endif

#define G_DEFINE_CONSTRUCTOR(_func) G_MSVC_CTOR (_func, G_MSVC_SYMBOL_PREFIX)
#define G_DEFINE_DESTRUCTOR(_func) G_MSVC_DTOR (_func, G_MSVC_SYMBOL_PREFIX)

#define G_MSVC_CTOR(_func,_sym_prefix) \
  static void _func(void); \
  extern int (* _array ## _func)(void);              \
  int _func ## _wrapper(void) { _func(); g_slist_find (NULL,  _array ## _func); return 0; } \
  __pragma(comment(linker,"/include:" _sym_prefix # _func "_wrapper")) \
  __pragma(section(".CRT$XCU",read)) \
  __declspec(allocate(".CRT$XCU")) int (* _array ## _func)(void) = _func ## _wrapper;

#define G_MSVC_DTOR(_func,_sym_prefix) \
  static void _func(void); \
  extern int (* _array ## _func)(void);              \
  int _func ## _constructor(void) { atexit (_func); g_slist_find (NULL,  _array ## _func); return 0; } \
   __pragma(comment(linker,"/include:" _sym_prefix # _func "_constructor")) \
  __pragma(section(".CRT$XCU",read)) \
  __declspec(allocate(".CRT$XCU")) int (* _array ## _func)(void) = _func ## _constructor;

#elif defined (_MSC_VER)

#define G_HAS_CONSTRUCTORS 1

/* Pre Visual studio 2008 must use #pragma section */
#define G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA 1
#define G_DEFINE_DESTRUCTOR_NEEDS_PRAGMA 1

#define G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(_func) \
  section(".CRT$XCU",read)
#define G_DEFINE_CONSTRUCTOR(_func) \
  static void _func(void); \
  static int _func ## _wrapper(void) { _func(); return 0; } \
  __declspec(allocate(".CRT$XCU")) static int (*p)(void) = _func ## _wrapper;

#define G_DEFINE_DESTRUCTOR_PRAGMA_ARGS(_func) \
  section(".CRT$XCU",read)
#define G_DEFINE_DESTRUCTOR(_func) \
  static void _func(void); \
  static int _func ## _constructor(void) { atexit (_func); return 0; } \
  __declspec(allocate(".CRT$XCU")) static int (* _array ## _func)(void) = _func ## _constructor;

#elif defined(__SUNPRO_C)

/* This is not tested, but i believe it should work, based on:
 * http://opensource.apple.com/source/OpenSSL098/OpenSSL098-35/src/fips/fips_premain.c
 */

#define G_HAS_CONSTRUCTORS 1

#define G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA 1
#define G_DEFINE_DESTRUCTOR_NEEDS_PRAGMA 1

#define G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(_func) \
  init(_func)
#define G_DEFINE_CONSTRUCTOR(_func) \
  static void _func(void);

#define G_DEFINE_DESTRUCTOR_PRAGMA_ARGS(_func) \
  fini(_func)
#define G_DEFINE_DESTRUCTOR(_func) \
  static void _func(void);

#else

/* constructors not supported for this compiler */

#endif

#endif /* __GTK_DOC_IGNORE__ */
#endif /* __G_CONSTRUCTOR_H__ */

#ifdef G_HAS_CONSTRUCTORS

#ifdef G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA
#pragma G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(simpleMsgDialogGresourceresource_constructor)
#endif
G_DEFINE_CONSTRUCTOR(simpleMsgDialogGresourceresource_constructor)
#ifdef G_DEFINE_DESTRUCTOR_NEEDS_PRAGMA
#pragma G_DEFINE_DESTRUCTOR_PRAGMA_ARGS(simpleMsgDialogGresourceresource_destructor)
#endif
G_DEFINE_DESTRUCTOR(simpleMsgDialogGresourceresource_destructor)

#else
#warning "Constructor not supported on this compiler, linking in resources will not work"
#endif

static void simpleMsgDialogGresourceresource_constructor (void)
{
  g_static_resource_init (&static_resource);
}

static void simpleMsgDialogGresourceresource_destructor (void)
{
  g_static_resource_fini (&static_resource);
}

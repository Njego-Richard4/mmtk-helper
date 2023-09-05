/**********************************************************************

  version.c -

  $Author$
  created at: Thu Sep 30 20:08:01 JST 1993

  Copyright (C) 1993-2007 Yukihiro Matsumoto

**********************************************************************/

#include "internal/cmdlineopt.h"
#include "ruby/ruby.h"
#include "version.h"
#include "vm_core.h"
#include "rjit.h"
#include "yjit.h"
#include <stdio.h>

#if USE_MMTK
#include "internal/gc.h"
#include "internal/mmtk.h"
#endif

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

#ifdef RUBY_REVISION
# if RUBY_PATCHLEVEL == -1
#  ifndef RUBY_BRANCH_NAME
#   define RUBY_BRANCH_NAME "master"
#  endif
#  define RUBY_REVISION_STR " "RUBY_BRANCH_NAME" "RUBY_REVISION
# else
#  define RUBY_REVISION_STR " revision "RUBY_REVISION
# endif
#else
# define RUBY_REVISION "HEAD"
# define RUBY_REVISION_STR ""
#endif
#if !defined RUBY_RELEASE_DATETIME || RUBY_PATCHLEVEL != -1
# undef RUBY_RELEASE_DATETIME
# define RUBY_RELEASE_DATETIME RUBY_RELEASE_DATE
#endif

# define RUBY_DESCRIPTION_PRE \
    "ruby "RUBY_VERSION             \
    RUBY_PATCHLEVEL_STR             \
    " ("RUBY_RELEASE_DATETIME       \
    RUBY_REVISION_STR")"
# define RUBY_DESCRIPTION_POST \
    " ["RUBY_PLATFORM"]"

#define PRINT(type) puts(ruby_##type)
#define MKSTR(type) rb_obj_freeze(rb_usascii_str_new_static(ruby_##type, sizeof(ruby_##type)-1))
#define MKINT(name) INT2FIX(ruby_##name)

const int ruby_api_version[] = {
    RUBY_API_VERSION_MAJOR,
    RUBY_API_VERSION_MINOR,
    RUBY_API_VERSION_TEENY,
};
#define RUBY_VERSION \
    STRINGIZE(RUBY_VERSION_MAJOR) "." \
    STRINGIZE(RUBY_VERSION_MINOR) "." \
    STRINGIZE(RUBY_VERSION_TEENY) ""
#ifndef RUBY_FULL_REVISION
# define RUBY_FULL_REVISION RUBY_REVISION
#endif
const char ruby_version[] = RUBY_VERSION;
const char ruby_revision[] = RUBY_FULL_REVISION;
const char ruby_release_date[] = RUBY_RELEASE_DATE;
const char ruby_platform[] = RUBY_PLATFORM;
const int ruby_patchlevel = RUBY_PATCHLEVEL;
const char ruby_description[] = RUBY_DESCRIPTION_PRE RUBY_DESCRIPTION_POST;
const char ruby_description_pre[] = RUBY_DESCRIPTION_PRE;
const char ruby_description_post[] = RUBY_DESCRIPTION_POST;
const char ruby_copyright[] = "ruby - Copyright (C) "
    RUBY_BIRTH_YEAR_STR "-" RUBY_RELEASE_YEAR_STR " "
    RUBY_AUTHOR;
const char ruby_engine[] = "ruby";

// Enough space for any combination of option flags
static char ruby_dynamic_description_buffer[sizeof(ruby_description) + sizeof("+MJIT +YJIT +MMTk(XXXXXXXXXXXXXXXX)") - 1];

// Might change after initialization
const char *rb_dynamic_description = ruby_description;

/*! Defines platform-depended Ruby-level constants */
void
Init_version(void)
{
    enum {ruby_patchlevel = RUBY_PATCHLEVEL};
    VALUE version = MKSTR(version);
    VALUE ruby_engine_name = MKSTR(engine);
    // MKSTR macro is a marker for fake.rb

    /*
     * The running version of ruby
     */
    rb_define_global_const("RUBY_VERSION", /* MKSTR(version) */ version);
    /*
     * The date this ruby was released
     */
    rb_define_global_const("RUBY_RELEASE_DATE", MKSTR(release_date));
    /*
     * The platform for this ruby
     */
    rb_define_global_const("RUBY_PLATFORM", MKSTR(platform));
    /*
     * The patchlevel for this ruby.  If this is a development build of ruby
     * the patchlevel will be -1
     */
    rb_define_global_const("RUBY_PATCHLEVEL", MKINT(patchlevel));
    /*
     * The GIT commit hash for this ruby.
     */
    rb_define_global_const("RUBY_REVISION", MKSTR(revision));
    /*
     * The copyright string for ruby
     */
    rb_define_global_const("RUBY_COPYRIGHT", MKSTR(copyright));
    /*
     * The engine or interpreter this ruby uses.
     */
    rb_define_global_const("RUBY_ENGINE", /* MKSTR(engine) */ ruby_engine_name);
    ruby_set_script_name(ruby_engine_name);
    /*
     * The version of the engine or interpreter this ruby uses.
     */
    rb_define_global_const("RUBY_ENGINE_VERSION", /* MKSTR(version) */ version);

    rb_provide("ruby2_keywords.rb");
}

#if USE_RJIT
#define RJIT_OPTS_ON opt->rjit.on
#else
#define RJIT_OPTS_ON 0
#endif

#if USE_YJIT
#define YJIT_OPTS_ON opt->yjit
#else
#define YJIT_OPTS_ON 0
#endif

void
Init_ruby_description(ruby_cmdline_options_t *opt)
{
    if (snprintf(ruby_dynamic_description_buffer, sizeof(ruby_dynamic_description_buffer), "%s%s%s%s%s%s%s",
            ruby_description_pre,
            RJIT_OPTS_ON ? " +RJIT" : "",
            YJIT_OPTS_ON ? " +YJIT" : "",
#if USE_MMTK
            rb_mmtk_enabled_p() ? " +MMTk(" : "",
            rb_mmtk_enabled_p() ? mmtk_plan_name() : "",
            rb_mmtk_enabled_p() ? ")" : "",
#else
            "", "", "",
#endif
            ruby_description_post) < 0) {
        rb_bug("could not format dynamic description string");
    }
    rb_dynamic_description = ruby_dynamic_description_buffer;

    /*
     * The full ruby version string, like <tt>ruby -v</tt> prints
     */
    rb_define_global_const("RUBY_DESCRIPTION", rb_obj_freeze(rb_str_new2(rb_dynamic_description)));
}

void
ruby_show_version(void)
{
    puts(rb_dynamic_description);

#ifdef RUBY_LAST_COMMIT_TITLE
    fputs("last_commit=" RUBY_LAST_COMMIT_TITLE, stdout);
#endif
#ifdef HAVE_MALLOC_CONF
    if (malloc_conf) printf("malloc_conf=%s\n", malloc_conf);
#endif
    fflush(stdout);
}

void
ruby_show_copyright(void)
{
    PRINT(copyright);
    fflush(stdout);
}

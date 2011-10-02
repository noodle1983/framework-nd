#include "RubyInterpreter.h"
#include <ruby.h>

using namespace Script::Ruby;

//-----------------------------------------------------------------------------

RubyInterpreter::RubyInterpreter()
{

}

//-----------------------------------------------------------------------------

RubyInterpreter::~RubyInterpreter()
{

}

//-----------------------------------------------------------------------------

void RubyInterpreter::init()
{
    ruby_init();

    //execute( "$KCODE = 'u'" );

    // addSearchPath
    //ruby_incpush(path.c_str());

    ruby_init_loadpath();

    rb_set_safe_level(0);

    ruby_script("ruby");

}

//-----------------------------------------------------------------------------

void RubyInterpreter::fini()
{
    ruby_finalize();
}

//-----------------------------------------------------------------------------

void RubyInterpreter::logRubyErrors(const std::string& intro, int errorcode)
{
    if (errorcode != 0)
    {
        //VALUE info = rb_inspect(ruby_errinfo);
        rb_backtrace();
        if (intro.length() > 0)
        {
        }
    }
}

//-----------------------------------------------------------------------------

bool RubyInterpreter::execute(const std::string& theCmd)
{
    int status = -1;

    rb_eval_string_protect(theCmd.c_str(), &status);

    logRubyErrors("", status);

    if ( status )
    {
        rb_eval_string_protect("print $!", &status);
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------

bool RubyInterpreter::executeFile(const std::string& theFilePath)
{
    bool error = execute("load '" + theFilePath + "'");
    return error;
}



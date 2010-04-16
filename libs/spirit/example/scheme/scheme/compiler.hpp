/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_SCHEME_COMPILER)
#define BOOST_SPIRIT_SCHEME_COMPILER

#include <vector>
#include <map>
#include <boost/bind.hpp>

#include <scheme/intrinsics.hpp>
#include <scheme/interpreter.hpp>
#include <input/parse_sexpr.hpp>

namespace scheme
{
///////////////////////////////////////////////////////////////////////////////
//  The environment
///////////////////////////////////////////////////////////////////////////////
    typedef boost::function<function(actor_list const&)> compiled_function;

    class environment
    {
    public:

        environment(environment* parent = 0)
          : outer(parent) {}

        template <typename Function>
        void define(std::string const& name, Function const& f)
        {
            // $$$ use exceptions here? $$$
            BOOST_ASSERT(definitions.find(name) == definitions.end());
            definitions[name] = compiled_function(f);
        }

        compiled_function* find(std::string const& name)
        {
            std::map<std::string, compiled_function>::iterator
                i = definitions.find(name);
            if (i != definitions.end())
                return &i->second;
            else if (outer != 0)
                return outer->find(name);
            return 0;
        }

        environment* parent() const { return outer; }

    private:

        environment* outer;
        std::map<std::string, compiled_function> definitions;
    };

///////////////////////////////////////////////////////////////////////////////
//  The compiler
///////////////////////////////////////////////////////////////////////////////
    function compile(
        utree const& ast, environment& env, actor_list& fragments);

    struct external_function : composite<external_function>
    {
        // we must hold f by reference because functions can be recursive
        boost::reference_wrapper<function const> f;

        external_function(function const& f)
          : f(f) {}

        using base_type::operator();
        function operator()(actor_list const& elements) const
        {
            return function(lambda_function(f, elements));
        }
    };

    struct compiler
    {
        typedef function result_type;
        environment& env;
        actor_list& fragments;

        compiler(environment& env, actor_list& fragments)
          : env(env), fragments(fragments)
        {
        }

        function operator()(nil) const
        {
            return scheme::val(utree());
        }

        template <typename T>
        function operator()(T const& val) const
        {
            return scheme::val(utree(val));
        }

        function operator()(utf8_symbol_range const& str) const
        {
            std::string name(str.begin(), str.end());
            if (compiled_function* mf = env.find(name))
            {
                actor_list flist;
                return (*mf)(flist);
            }
            // $$$ throw? $$$
            BOOST_ASSERT(false);
            return function();
        }

        function make_lambda(
            std::vector<std::string> const& args,
            utree const& body) const
        {
            environment local_env(&this->env);
            for (std::size_t i = 0; i < args.size(); ++i)
                local_env.define(args[i], boost::bind(arg, i));
            return compile(body, local_env, fragments);
        }

        function define_function(
            std::string const& name,
            std::vector<std::string> const& args,
            utree const& body) const
        {
            fragments.push_back(function());
            function& f = fragments.back();
            env.define(name, external_function(f));
            f = make_lambda(args, body);
            return f;
        }

        template <typename Iterator>
        function operator()(boost::iterator_range<Iterator> const& range) const
        {
            std::string name(get_symbol(*range.begin()));

            if (name == "define")
            {
                std::string fname;
                std::vector<std::string> args;

                Iterator i = range.begin(); ++i;
                if (i->which() == utree_type::list_type)
                {
                    // (define (f x) ...body...)
                    utree const& decl = *i++;
                    Iterator di = decl.begin();
                    fname = get_symbol(*di++);
                    while (di != decl.end())
                        args.push_back(get_symbol(*di++));
                }
                else
                {
                    // (define f ...body...)
                    fname = get_symbol(*i++);
                }

                return define_function(fname, args, *i);
            }

            if (name == "lambda")
            {
                // (lambda (x) ...body...)
                Iterator i = range.begin(); ++i;
                utree const& arg_names = *i++;
                Iterator ai = arg_names.begin();
                std::vector<std::string> args;
                while (ai != arg_names.end())
                    args.push_back(get_symbol(*ai++));
                return make_lambda(args, *i);
            }

            if (compiled_function* mf = env.find(name))
            {
                actor_list flist;
                Iterator i = range.begin(); ++i;
                for (; i != range.end(); ++i)
                    flist.push_back(compile(*i, env, fragments));
                return (*mf)(flist);
            }

            BOOST_ASSERT(false);
            return function(); // $$$ implement me $$$
        }

        static std::string get_symbol(utree const& s)
        {
            utf8_symbol_range symbol = s.as<utf8_symbol_range>();
            return std::string(symbol.begin(), symbol.end());
        }
    };

    inline function compile(
        utree const& ast, environment& env, actor_list& fragments)
    {
        return utree::visit(ast, compiler(env, fragments));
    }

    void compile_all(
        utree const& ast,
        environment& env,
        actor_list& results,
        actor_list& fragments)
    {
        BOOST_FOREACH(utree const& program, ast)
        {
            scheme::function f = compile(program, env, fragments);
            results.push_back(f);
        }
    }

    void build_basic_environment(environment& env)
    {
        env.define("if", if_);
        env.define("<", less_than);
        env.define("<=", less_than_equal);
        env.define("+", plus);
        env.define("-", minus);
        env.define("*", times);
    }

    ///////////////////////////////////////////////////////////////////////////
    // interpreter
    ///////////////////////////////////////////////////////////////////////////
    struct interpreter : actor<interpreter>
    {
        template <typename Source>
        interpreter(Source& in, environment* outer = 0)
        {
            if (outer == 0)
                build_basic_environment(env);

            if (input::parse_sexpr_list(in, program))
            {
                compile_all(program, env, flist, fragments);
            }
            else
            {
                // $$$ Use exceptions $$$
                BOOST_ASSERT(false);
            }
        }

        interpreter(utree const& program, environment* outer = 0)
        {
            if (outer == 0)
                build_basic_environment(env);
            compile_all(program, env, flist, fragments);
        }

        utree eval(args_type args) const
        {
            return flist.back()(args);
        }

        environment env;
        utree program;
        actor_list fragments;
        actor_list flist;
    };
}

#endif

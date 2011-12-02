/*=============================================================================
    Copyright (c) 2002 2004 2006 Joel de Guzman
    Copyright (c) 2004 Eric Niebler
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "grammar.hpp"
#include "quickbook.hpp"
#include "actions_class.hpp"
#include "post_process.hpp"
#include "utils.hpp"
#include "files.hpp"
#include "input_path.hpp"
#include "id_manager.hpp"
#include <boost/program_options.hpp>
#include <boost/filesystem/v3/path.hpp>
#include <boost/filesystem/v3/operations.hpp>
#include <boost/filesystem/v3/fstream.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/ref.hpp>
#include <boost/version.hpp>

#include <stdexcept>
#include <vector>
#include <iterator>

#if defined(_WIN32)
#include <windows.h>
#include <shellapi.h>
#endif

#if (defined(BOOST_MSVC) && (BOOST_MSVC <= 1310))
#pragma warning(disable:4355)
#endif

#define QUICKBOOK_VERSION "Quickbook Version 1.5.7 (trunk)"

namespace quickbook
{
    namespace cl = boost::spirit::classic;
    namespace fs = boost::filesystem;

    tm* current_time; // the current time
    tm* current_gm_time; // the current UTC time
    bool debug_mode; // for quickbook developers only
    bool self_linked_headers;
    bool ms_errors = false; // output errors/warnings as if for VS
    std::vector<fs::path> include_path;
    std::vector<std::string> preset_defines;
    fs::path image_location;

    static void set_macros(actions& actor)
    {
        for(std::vector<std::string>::const_iterator
                it = preset_defines.begin(),
                end = preset_defines.end();
                it != end; ++it)
        {
            parse_iterator first(it->begin());
            parse_iterator last(it->end());

            cl::parse_info<parse_iterator> info =
                cl::parse(first, last, actor.grammar().command_line_macro);

            if (!info.full) {
                detail::outerr()
                    << "Error parsing command line definition: '"
                    << detail::utf8(*it)
                    << "'"
                    << std::endl;
                ++actor.error_count;
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    //
    //  Parse a file
    //
    ///////////////////////////////////////////////////////////////////////////
    void parse_file(actions& actor, value include_doc_id, bool nested_file)
    {
        parse_iterator first(actor.current_file->source.begin());
        parse_iterator last(actor.current_file->source.end());

        cl::parse_info<parse_iterator> info = cl::parse(first, last, actor.grammar().doc_info);
        assert(info.hit);

        if (!actor.error_count)
        {
            parse_iterator pos = info.stop;
            std::string doc_type = pre(actor, pos, include_doc_id, nested_file);

            info = cl::parse(info.hit ? info.stop : first, last, actor.grammar().block);

            post(actor, doc_type);

            if (!info.full)
            {
                file_position const& pos = actor.current_file->position_of(info.stop.base());
                detail::outerr(actor.current_file->path, pos.line)
                    << "Syntax Error near column " << pos.column << ".\n";
                ++actor.error_count;
            }
        }
    }

    static int
    parse_document(
        fs::path const& filein_
      , fs::path const& fileout_
      , fs::path const& xinclude_base_
      , int indent
      , int linewidth
      , bool pretty_print)
    {
        string_stream buffer;
        id_manager ids;

        int result = 0;

        try {
            actions actor(filein_, xinclude_base_, buffer, ids);
            set_macros(actor);

            if (actor.error_count == 0) {
                actor.current_file = load(filein_); // Throws load_error

                parse_file(actor);

                if(actor.error_count) {
                    detail::outerr()
                        << "Error count: " << actor.error_count << ".\n";
                }
            }

            result = actor.error_count ? 1 : 0;
        }
        catch (load_error& e) {
            detail::outerr(filein_) << detail::utf8(e.what()) << std::endl;
            result = 1;
        }

        if (result == 0)
        {
            std::string stage2 = ids.replace_placeholders(buffer.str());

            fs::ofstream fileout(fileout_);

            if (fileout.fail()) {
                ::quickbook::detail::outerr()
                    << "Error opening output file "
                    << fileout_
                    << std::endl;

                return 1;
            }

            if (pretty_print)
            {
                try
                {
                    fileout << post_process(stage2, indent, linewidth);
                }
                catch (quickbook::post_process_failure&)
                {
                    // fallback!
                    ::quickbook::detail::outerr()
                        << "Post Processing Failed."
                        << std::endl;
                    fileout << stage2;
                    return 1;
                }
            }
            else
            {
                fileout << stage2;
            }

            if (fileout.fail()) {
                ::quickbook::detail::outerr()
                    << "Error writing to output file "
                    << fileout_
                    << std::endl;

                return 1;
            }
        }

        return result;
    }
}

///////////////////////////////////////////////////////////////////////////
//
//  Main program
//
///////////////////////////////////////////////////////////////////////////
int
main(int argc, char* argv[])
{
    try
    {
        namespace fs = boost::filesystem;
        namespace po = boost::program_options;

        using boost::program_options::options_description;
        using boost::program_options::variables_map;
        using boost::program_options::store;
        using boost::program_options::parse_command_line;
        using boost::program_options::wcommand_line_parser;
        using boost::program_options::command_line_parser;
        using boost::program_options::notify;
        using boost::program_options::positional_options_description;
        
        using quickbook::detail::input_string;

        // First thing, the filesystem should record the current working directory.
        fs::initial_path<fs::path>();
        
        // Various initialisation methods
        quickbook::detail::initialise_output();
        quickbook::detail::initialise_markups();

        options_description desc("Allowed options");
        options_description hidden("Hidden options");
        options_description all("All options");

#if QUICKBOOK_WIDE_PATHS
#define PO_VALUE po::wvalue
#else
#define PO_VALUE po::value
#endif

        desc.add_options()
            ("help", "produce help message")
            ("version", "print version string")
            ("no-pretty-print", "disable XML pretty printing")
            ("no-self-linked-headers", "stop headers linking to themselves")
            ("indent", PO_VALUE<int>(), "indent spaces")
            ("linewidth", PO_VALUE<int>(), "line width")
            ("input-file", PO_VALUE<input_string>(), "input file")
            ("output-file", PO_VALUE<input_string>(), "output file")
            ("debug", "debug mode (for developers)")
            ("ms-errors", "use Microsoft Visual Studio style error & warn message format")
            ("include-path,I", PO_VALUE< std::vector<input_string> >(), "include path")
            ("define,D", PO_VALUE< std::vector<input_string> >(), "define macro")
            ("image-location", PO_VALUE<input_string>(), "image location")
        ;

        hidden.add_options()
            ("expect-errors",
                "Succeed if the input file contains a correctly handled "
                "error, fail otherwise.")
            ("xinclude-base", PO_VALUE<input_string>(),
                "Generate xincludes as if generating for this target "
                "directory.")
        ;

        all.add(desc).add(hidden);

        positional_options_description p;
        p.add("input-file", -1);

        variables_map vm;
        int indent = -1;
        int linewidth = -1;
        bool pretty_print = true;

#if QUICKBOOK_WIDE_PATHS
        quickbook::ignore_variable(&argc);
        quickbook::ignore_variable(&argv);

        int wide_argc;
        LPWSTR* wide_argv = CommandLineToArgvW(GetCommandLineW(), &wide_argc);
        if (!wide_argv)
        {
            quickbook::detail::outerr() << "Error getting argument values." << std::endl;
            return 1;
        }

        store(
            wcommand_line_parser(wide_argc, wide_argv)
                .options(all)
                .positional(p)
                .run(), vm);

        LocalFree(wide_argv);
#else
        store(command_line_parser(argc, argv)
                .options(all)
                .positional(p)
                .run(), vm);
#endif

        notify(vm);

        bool expect_errors = vm.count("expect-errors");
        int error_count = 0;

        if (vm.count("help"))
        {
            std::ostringstream description_text;
            description_text << desc;

            quickbook::detail::out()
                << quickbook::detail::utf8(description_text.str()) << "\n";

            return 0;
        }

        if (vm.count("version"))
        {
            std::string boost_version = BOOST_LIB_VERSION;
            boost::replace(boost_version, '_', '.');
        
            quickbook::detail::out()
                << QUICKBOOK_VERSION
                << " (Boost "
                << quickbook::detail::utf8(boost_version)
                << ")"
                << std::endl;
            return 0;
        }

        if (vm.count("ms-errors"))
            quickbook::ms_errors = true;

        if (vm.count("no-pretty-print"))
            pretty_print = false;

        quickbook::self_linked_headers = !vm.count("no-self-link-headers");

        if (vm.count("indent"))
            indent = vm["indent"].as<int>();

        if (vm.count("linewidth"))
            linewidth = vm["linewidth"].as<int>();

        if (vm.count("debug"))
        {
            static tm timeinfo;
            timeinfo.tm_year = 2000 - 1900;
            timeinfo.tm_mon = 12 - 1;
            timeinfo.tm_mday = 20;
            timeinfo.tm_hour = 12;
            timeinfo.tm_min = 0;
            timeinfo.tm_sec = 0;
            timeinfo.tm_isdst = -1;
            mktime(&timeinfo);
            quickbook::current_time = &timeinfo;
            quickbook::current_gm_time = &timeinfo;
            quickbook::debug_mode = true;
        }
        else
        {
            time_t t = std::time(0);
            static tm lt = *localtime(&t);
            static tm gmt = *gmtime(&t);
            quickbook::current_time = &lt;
            quickbook::current_gm_time = &gmt;
            quickbook::debug_mode = false;
        }
        
        quickbook::include_path.clear();
        if (vm.count("include-path"))
        {
            boost::transform(
                vm["include-path"].as<std::vector<input_string> >(),
                std::back_inserter(quickbook::include_path),
                quickbook::detail::input_to_path);
        }

        quickbook::preset_defines.clear();
        if (vm.count("define"))
        {
            boost::transform(
                vm["define"].as<std::vector<input_string> >(),
                std::back_inserter(quickbook::preset_defines),
                quickbook::detail::input_to_utf8);
        }

        if (vm.count("input-file"))
        {
            fs::path filein = quickbook::detail::input_to_path(
                vm["input-file"].as<input_string>());
            fs::path fileout;

            if (vm.count("output-file"))
            {
                fileout = quickbook::detail::input_to_path(
                    vm["output-file"].as<input_string>());
            }
            else
            {
                fileout = filein;
                fileout.replace_extension(".xml");
            }
            
            fs::path xinclude_base;
            if (vm.count("xinclude-base"))
            {
                xinclude_base = quickbook::detail::input_to_path(
                    vm["xinclude-base"].as<input_string>());
            }
            else
            {
                xinclude_base = fileout.parent_path();
                if (xinclude_base.empty())
                    xinclude_base = ".";
            }

            if (!fs::is_directory(xinclude_base))
            {
                quickbook::detail::outerr()
                    << (vm.count("xinclude-base") ?
                        "xinclude-base is not a directory" :
                        "parent directory not found for output file");
                ++error_count;
            }

            if (vm.count("image-location"))
            {
                quickbook::image_location = quickbook::detail::input_to_path(
                    vm["image-location"].as<input_string>());
            }
            else
            {
                quickbook::image_location = filein.parent_path() / "html";
            }

            quickbook::detail::out() << "Generating Output File: "
                << quickbook::detail::path_to_stream(fileout)
                << std::endl;

            if (!error_count)
                error_count += quickbook::parse_document(filein, fileout, xinclude_base, indent, linewidth, pretty_print);

            if (expect_errors)
            {
                if (!error_count) quickbook::detail::outerr() << "No errors detected for --expect-errors." << std::endl;
                return !error_count;
            }
            else
            {
                return error_count;
            }
        }
        else
        {
            std::ostringstream description_text;
            description_text << desc;
        
            quickbook::detail::outerr() << "No filename given\n\n"
                << quickbook::detail::utf8(description_text.str()) << std::endl;
            return 1;
        }        
    }

    catch(std::exception& e)
    {
        quickbook::detail::outerr() << quickbook::detail::utf8(e.what()) << "\n";
        return 1;
    }

    catch(...)
    {
        quickbook::detail::outerr() << "Exception of unknown type caught\n";
        return 1;
    }

    return 0;
}

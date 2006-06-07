/*=============================================================================
    Copyright (c) 2002 2004 2006 Joel de Guzman
    Copyright (c) 2004 Eric Niebler
    Copyright (c) 2005 Thomas Guest
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <numeric>
#include <functional>
#include <boost/bind.hpp>
#include <boost/filesystem/convenience.hpp>
#include "./actions.hpp"
#include "./utils.hpp"
#include "./markups.hpp"
#include "./actions_class.hpp"
#include "../block.hpp"
#include "../phrase.hpp"

namespace quickbook
{
    void error_action::operator()(iterator const& first, iterator const& /*last*/) const
    {
        boost::spirit::file_position const pos = first.get_position();
        detail::outerr(pos.file,pos.line)
            << "Syntax Error near column " << pos.column << ".\n";
    }

    void phrase_action::operator()(iterator const& first, iterator const& last) const
    {
        std::string str = phrase.str();
        detail::convert_nbsp(str);
        phrase.str(std::string());
        out << pre << str << post;
    }

    void header_action::operator()(iterator const& first, iterator const& last) const
    {
        std::string str = phrase.str();
        detail::convert_nbsp(str);
        phrase.str(std::string());

        if (qbk_version_n < 103) // version 1.2 and below
        {
            out << "<anchor id=\""
                << section_id << '.'
                << detail::make_identifier(str.begin(), str.end())
                << "\" />"
                << pre << str << post
                ;
        }
        else // version 1.3 and above
        {
            std::string anchor = 
                library_id + '.' + qualified_section_id + '.' +
                detail::make_identifier(str.begin(), str.end());

            out << "<anchor id=\"" << anchor << "\"/>"
                << pre 
                << "<link linkend=\"" << anchor << "\">"
                << str 
                << "</link>"
                << post
                ;
        }
    }

    void generic_header_action::operator()(iterator const& first, iterator const& last) const
    {
        int level_ = section_level + 2;     // section_level is zero-based. We need to use a 
                                            // 0ne-based heading which is one greater
                                            // than the current. Thus: section_level + 2.
        if (level_ > 6)                     // The max is h6, clip it if it goes 
            level_ = 6;                     // further than that
        std::string str = phrase.str();
        detail::convert_nbsp(str);

        std::string anchor = 
            library_id + '.' + qualified_section_id + '.' +
            detail::make_identifier(str.begin(), str.end());

        phrase.str(std::string());
        out 
            << "<anchor id=\"" << anchor << "\"/>"
            << "<bridgehead renderas=\"sect" << level_ << "\">"
            << "<link linkend=\"" << anchor << "\">"
            << str 
            << "</link>"
            << "</bridgehead>"
            ;
    }

    void simple_phrase_action::operator()(iterator first, iterator const& last) const
    {
        out << pre;
        std::string str(first, last);
        if (std::string const* ptr = find(macro, str.c_str()))
        {
            out << *ptr;
        }
        else
        {
            while (first != last)
                detail::print_char(*first++, out.get());
        }
        out << post;
    }

    void list_action::operator()(iterator const& first, iterator const& last) const
    {
        BOOST_ASSERT(!list_marks.empty()); // there must be at least one item in the stack
        std::string  str = list_buffer.str();
        list_buffer.str(std::string());
        out << str;

        while (!list_marks.empty())
        {
            char mark = list_marks.top().first;
            list_marks.pop();
            out << std::string((mark == '#') ? "\n</orderedlist>" : "\n</itemizedlist>");
            if (list_marks.size() >= 1)
                out << std::string("\n</listitem>");
        }

        list_indent = -1; // reset
    }

    void list_format_action::operator()(iterator first, iterator const& last) const
    {
        int new_indent = 0;
        while (first != last && (*first == ' ' || *first == '\t'))
        {
            char mark = *first++;
            if (mark == ' ')
            {
                ++new_indent;
            }
            else // must be a tab
            {
                BOOST_ASSERT(mark == '\t');
                // hardcoded tab to 4 for now
                new_indent = ((new_indent + 4) / 4) * 4;
            }
        }

        char mark = *first;
        BOOST_ASSERT(mark == '#' || mark == '*'); // expecting a mark

        if (list_indent == -1) // the very start
        {
            BOOST_ASSERT(new_indent == 0);
        }

        if (new_indent > list_indent)
        {
            list_indent = new_indent;
            list_marks.push(mark_type(mark, list_indent));
            if (list_marks.size() > 1)
            {
                // Make this new list a child of the previous list.
                // The previous listelem has already ended so we erase
                // </listitem> to accomodate this sub-list. We'll close
                // the listelem later.

                std::string str = out.str();
                std::string::size_type pos = str.rfind("\n</listitem>");
                BOOST_ASSERT(pos <= str.size());
                str.erase(str.begin()+pos, str.end());
                out.str(std::string());
                out << str;
            }
            out << std::string((mark == '#') ? "<orderedlist>\n" : "<itemizedlist>\n");
        }

        else if (new_indent < list_indent)
        {
            BOOST_ASSERT(!list_marks.empty());
            list_indent = new_indent;

            while (!list_marks.empty() && (list_indent < list_marks.top().second))
            {
                char mark = list_marks.top().first;
                list_marks.pop();
                out << std::string((mark == '#') ? "\n</orderedlist>" : "\n</itemizedlist>");
                if (list_marks.size() >= 1)
                    out << std::string("\n</listitem>");
            }
        }

        if (mark != list_marks.top().first) // new_indent == list_indent
        {
            boost::spirit::file_position const pos = first.get_position();
            detail::outerr(pos.file,pos.line)
                << "Illegal change of list style near column " << pos.column << ".\n";
            detail::outwarn(pos.file,pos.line)
                << "Ignoring change of list style" << std::endl;
        }
    }

    void span::operator()(iterator first, iterator const& last) const
    {
        out << "<phrase role=\"" << name << "\">";
        while (first != last)
            detail::print_char(*first++, out.get());
        out << "</phrase>";
    }

    void unexpected_char::operator()(char) const
    {
        out << '#'; // print out an unexpected character
    }

    void anchor_action::operator()(iterator first, iterator const& last) const
    {
        out << "<anchor id=\"";
        while (first != last)
            detail::print_char(*first++, out.get());
        out << "\" />\n";
    }

    void do_macro_action::operator()(std::string const& str) const
    {
        if (str == quickbook_get_date)
        {
            char strdate[64];
            strftime(strdate, sizeof(strdate), "%Y-%b-%d", current_time);
            phrase << strdate;
        }
        else if (str == quickbook_get_time)
        {
            char strdate[64];
            strftime(strdate, sizeof(strdate), "%I:%M:%S %p", current_time);
            phrase << strdate;
        }
        else
        {
            phrase << str;
        }
    }

    void space::operator()(char ch) const
    {

        detail::print_space(ch, out.get());
    }

    void space::operator()(iterator first, iterator const& last) const
    {
        while (first != last)
            detail::print_space(*first++, out.get());
    }

    void pre_escape_back::operator()(iterator const& first, iterator const& last) const
    {
        save = escape_actions.phrase.str(); // save the stream
    }

    void post_escape_back::operator()(iterator const& first, iterator const& last) const
    {
        std::string str = escape_actions.phrase.str();
        escape_actions.phrase.str(save); // restore the stream
        out << str;
    }

    void code_action::operator()(iterator const& first, iterator const& last) const
    {
        std::string save = phrase.str();
        phrase.str(std::string());

        // preprocess the code section to remove the initial indentation
        std::string program_(first, last);
        detail::unindent(program_);

        // $$$ fix me $$$ this is wasteful. we have to convert
        // back to a vector<char> so we can use the same iterator type 
        // used by the rest of the system, otherwise, it is wasteful 
        // of function template instantiations

        std::vector<char> program(program_.begin(), program_.end());
        iterator first_(program.begin(), program.end());
        iterator last_(program.end(), program.end());
        first_.set_position(first.get_position());

        // print the code with syntax coloring
        if (source_mode == "c++")
        {
            parse(first_, last_, cpp_p);
        }
        else if (source_mode == "python")
        {
            parse(first_, last_, python_p);
        }
        
        std::string str = temp.str();
        temp.str(std::string());
        phrase.str(std::string());
        phrase << save;

        out << "<programlisting>\n";
        out << str;
        out << "</programlisting>\n";
    }

    void inline_code_action::operator()(iterator const& first, iterator const& last) const
    {
        std::string save = out.str();
        out.str(std::string());
 
        // print the code with syntax coloring
        if (source_mode == "c++")
        {
            parse(first, last, cpp_p);
        }
        else if (source_mode == "python")
        {
            parse(first, last, python_p);
        }
        std::string str = temp.str();
        temp.str(std::string());
        out.str(std::string());

        out << save;
        out << "<code>";
        out << str;
        out << "</code>";
    }

    void raw_char_action::operator()(char ch) const
    {
        phrase << ch;
    }

    void raw_char_action::operator()(iterator const& first, iterator const& /*last*/) const
    {
        phrase << *first;
    }

    void plain_char_action::operator()(char ch) const
    {
        detail::print_char(ch, phrase.get());
    }

    void plain_char_action::operator()(iterator const& first, iterator const& /*last*/) const
    {
        detail::print_char(*first, phrase.get());
    }

    void image_action::operator()(iterator first, iterator const& last) const
    {
        fs::path const img_path(std::string(first, last));

        phrase << "<inlinemediaobject>";

        phrase << "<imageobject><imagedata fileref=\"";
        while (first != last)
            detail::print_char(*first++, phrase.get());
        phrase << "\"></imagedata></imageobject>";

        // Also add a textobject -- use the basename of the image file.
        // This will mean we get "alt" attributes of the HTML img.
        phrase << "<textobject><phrase>";
        detail::print_string(fs::basename(img_path), phrase.get());
        phrase << "</phrase></textobject>";

        phrase << "</inlinemediaobject>";
    }

    void macro_identifier_action::operator()(iterator const& first, iterator const& last) const
    {
        actions.macro_id.assign(first, last);
        actions.macro_phrase_save = actions.phrase.str();
        actions.phrase.str(std::string());
    }

    void macro_definition_action::operator()(iterator const& first, iterator const& last) const
    {
        actions.macro.add(
            actions.macro_id.begin()
          , actions.macro_id.end()
          , actions.phrase.str());
        actions.phrase.str(actions.macro_phrase_save);
    }

    void template_body_action::operator()(iterator const& first, iterator const& last) const
    {
        BOOST_ASSERT(actions.template_info.size());
        actions.template_info.push_back(std::string(first, last));
        actions.templates.add(
            actions.template_info[0].begin()
          , actions.template_info[0].end()
          , boost::make_tuple(actions.template_info, first.get_position()));
        actions.template_info.clear();
    }

    void do_template_action::operator()(iterator const& first, iterator const&) const
    {
        boost::spirit::file_position const pos = first.get_position();
        ++actions.template_depth;
        if (actions.template_depth > actions.max_template_depth)
        {
            detail::outerr(pos.file,pos.line)
                << "Infinite loop detected" << std::endl;
            --actions.template_depth;
            return;
        }
        
        std::string result;
        actions.push(); // scope the actions' states
        {
            actions.phrase.str(std::string()); // clear the phrase
            
            simple_phrase_grammar<quickbook::actions> phrase_p(actions);
            block_grammar<quickbook::actions, true> block_p(actions);

            template_symbol const* symbol = 
                find(actions.templates, actions.template_info[0].c_str());
            BOOST_ASSERT(symbol);
            
            std::vector<std::string> template_ = boost::get<0>(*symbol);
            boost::spirit::file_position template_pos = boost::get<1>(*symbol);

            std::vector<std::string> template_info;
            std::swap(template_info, actions.template_info);
            
            if (template_.size()-1 != template_info.size())
            {
                while (template_.size()-1 != template_info.size())
                {
                    // Try to break the last argument at the first space found
                    // and push it into the back of template_info. Do this
                    // recursively until we have all the expected number of
                    // arguments, or if there is no more spaces left.
                    
                    std::string& str = template_info.back();
                    std::string::size_type l_pos = str.find_first_of(" \t\r\n");
                    if (l_pos == std::string::npos)
                        break;
                    std::string first(str.begin(), str.begin()+l_pos);
                    std::string::size_type r_pos = str.find_first_not_of(" \t\r\n", l_pos);
                    std::string second(str.begin()+r_pos, str.end());
                    str = first;
                    template_info.push_back(second);
                }
                
                if (template_.size()-1 != template_info.size())
                {
                    actions.pop(); // restore the actions' states
                    detail::outerr(pos.file,pos.line)
                        << "Invalid number of arguments passed. Expecting: "
                        << template_.size()-2
                        << " argument(s), got: "
                        << template_info.size()-1
                        << " argument(s) instead."
                        << std::endl;                    
                    --actions.template_depth;
                    return;
                }
            }

            std::vector<std::string>::const_iterator arg = template_info.begin()+1; 
            std::vector<std::string>::const_iterator tpl = template_.begin()+1; 

            // Store each of the argument passed in as local templates:
            while (arg != template_info.end())
            {
                std::vector<std::string> tinfo;
                tinfo.push_back(*tpl);
                tinfo.push_back(*arg);
                template_symbol template_(tinfo, pos);

                if (template_symbol* p = find(actions.templates, tpl->c_str()))
                {
                    *p = template_;
                }
                else
                {
                    actions.templates.add(tpl->begin(), tpl->end(), template_);
                }
                actions.phrase.str(std::string()); // clear the phrase
                ++arg; ++tpl;
            }
            
            // parse the template body:
            std::vector<char> temp;
            temp.assign(tpl->begin(), tpl->end());
            temp.reserve(temp.size()+2); // reserve 2 more
            
            // How do we know if we are to parse the template as a block or
            // a phrase? We apply a simple heuristic: if the body starts with
            // a newline, then we regard is as a block, otherwise, we parse
            // it as a phrase.
            
            std::vector<char>::const_iterator iter = temp.begin();
            while (iter != temp.end() && ((*iter == ' ') || (*iter == '\t')))
                ++iter; // skip spaces and tabs
            bool is_block = (iter != temp.end()) && ((*iter == '\r') || (*iter == '\n'));
            bool r = false;

            if (!is_block)
            {
                //  do a phrase level parse
                iterator first(temp.begin(), temp.end(), actions.filename.native_file_string().c_str());
                first.set_position(template_pos);
                iterator last(temp.end(), temp.end());
                r = boost::spirit::parse(first, last, phrase_p).full;
                result = actions.phrase.str();
            }
            else
            {
                //  do a block level parse
                //  ensure that we have enough trailing newlines to eliminate
                //  the need to check for end of file in the grammar.
                temp.push_back('\n');
                temp.push_back('\n');
                while (iter != temp.end() && ((*iter == '\r') || (*iter == '\n')))
                    ++iter; // skip initial newlines
                iterator first(iter, temp.end(), actions.filename.native_file_string().c_str());
                first.set_position(template_pos);
                iterator last(temp.end(), temp.end());
                r = boost::spirit::parse(first, last, block_p).full;
                result = actions.out.str();
            }

            if (!r)
            {
                boost::spirit::file_position const pos = first.get_position();
                detail::outerr(pos.file,pos.line)
                    << "Expanding template" << std::endl;
            }
        }

        actions.pop(); // restore the actions' states
        actions.phrase << result; // print it!!!
        --actions.template_depth;
    }

    void link_action::operator()(iterator first, iterator const& last) const
    {
        iterator save = first;
        phrase << tag;
        while (first != last)
            detail::print_char(*first++, phrase.get());
        phrase << "\">";

        // Yes, it is safe to dereference last here. When we
        // reach here, *last is certainly valid. We test if
        // *last == ']'. In which case, the url is the text.
        // Example: [@http://spirit.sourceforge.net/]

        if (*last == ']')
        {
            first = save;
            while (first != last)
                detail::print_char(*first++, phrase.get());
        }
    }

    void variablelist_action::operator()(iterator, iterator) const
    {
        actions.out << "<variablelist>\n";

        actions.out << "<title>";
        std::string::iterator first = actions.table_title.begin();
        std::string::iterator last = actions.table_title.end();
        while (first != last)
            detail::print_char(*first++, actions.out.get());
        actions.out << "</title>\n";

        std::string str = actions.phrase.str();
        actions.phrase.str(std::string());
        actions.out << str;

        actions.out << "</variablelist>\n";
        actions.table_span = 0;
        actions.table_header.clear();
        actions.table_title.clear();
    }

    void table_action::operator()(iterator, iterator) const
    {
        actions.out << "<informaltable frame=\"all\">\n"
                     << "<bridgehead renderas=\"sect4\">";

        std::string::iterator first = actions.table_title.begin();
        std::string::iterator last = actions.table_title.end();
        if (first != last) // allow no title
        {
            actions.out << "<phrase role=\"table-title\">";
            while (first != last)
                detail::print_char(*first++, actions.out.get());
            actions.out << "</phrase>";
        }
        actions.out << "</bridgehead>\n"
                     << "<tgroup cols=\"" << actions.table_span << "\">\n";

        if (!actions.table_header.empty())
        {
            actions.out << "<thead>" << actions.table_header << "</thead>\n";
        }

        actions.out << "<tbody>\n";

        std::string str = actions.phrase.str();
        detail::convert_nbsp(str);
        actions.phrase.str(std::string());
        actions.out << str;

        actions.out << "</tbody>\n"
                     << "</tgroup>\n"
                     << "</informaltable>\n";
        actions.table_span = 0;
        actions.table_header.clear();
        actions.table_title.clear();
    }

    void start_row_action::operator()(char) const
    {
        // the first row is the header
        if (header.empty() && !phrase.str().empty())
        {
            header = phrase.str();
            phrase.str(std::string());
        }

        phrase << start_row_;
        span = 0;
    }

    void start_row_action::operator()(iterator f, iterator) const
    {
        (*this)(*f);
    }

    void start_col_action::operator()(char) const
    {
        phrase << start_cell_; 
        ++span;
    }

    void begin_section_action::operator()(iterator first, iterator const& last) const
    {
        if (section_id.empty())
            section_id = detail::make_identifier(first, last);

        if (section_level != 0)
            qualified_section_id += '.';
        else
            BOOST_ASSERT(qualified_section_id.empty());
        qualified_section_id += section_id;
        ++section_level;

        if (qbk_version_n < 103) // version 1.2 and below
        {
            out << "\n<section id=\"" 
                << library_id << "." << section_id << "\">\n";
        }
        else // version 1.3 and above
        {
            out << "\n<section id=\"" << library_id 
                << "." << qualified_section_id << "\">\n";
        }
        std::string str;
        str = phrase.str();
        phrase.str(std::string());

        if (qbk_version_n < 103) // version 1.2 and below
        {
            out << "<title>" << str << "</title>\n";
        }
        else // version 1.3 and above
        {
            out << "<title>" 
                << "<link linkend=\"" << library_id
                    << "." << qualified_section_id << "\">"
                << str 
                << "</link>"
                << "</title>\n"
                ;
        }
    }

    void end_section_action::operator()(iterator const& first, iterator const& last) const
    {
        out << "</section>";

        --section_level;
        if (section_level < 0)
        {
            boost::spirit::file_position const pos = first.get_position();
            detail::outerr(pos.file,pos.line)
                << "Mismatched [endsect] near column " << pos.column << ".\n";
        }
        if (section_level == 0)
        {
            qualified_section_id.clear();
        }
        else
        {
            std::string::size_type const n = 
                qualified_section_id.find_last_of('.');
            BOOST_ASSERT(std::string::npos != n);
            qualified_section_id.erase(n, std::string::npos);
        }
    }

    fs::path path_difference(fs::path const& outdir, fs::path const& xmlfile)
    {
        fs::path outtmp, xmltmp;
        fs::path::iterator out = outdir.begin(), xml = xmlfile.begin();
        for(; out != outdir.end() && xml != xmlfile.end(); ++out, ++xml)
        {
            if(!fs::equivalent(outtmp /= *out, xmltmp /= *xml))
                break;
        }
        std::divides<fs::path> concat;
        out = (out == outdir.begin()) ? outdir.end() : out;
        xmltmp = std::accumulate(out, outdir.end(), fs::path(), boost::bind(concat, _1, ".."));
        return std::accumulate(xml, xmlfile.end(), xmltmp, concat);
    }

    void xinclude_action::operator()(iterator const& first, iterator const& last) const
    {
        // Given an xml file to include and the current filename, calculate the
        // path to the XML file relative to the output directory.
        fs::path xmlfile(std::string(first, last));
        if (!xmlfile.is_complete())
        {
            fs::path infile = fs::complete(actions.filename).normalize();
            xmlfile = (infile.branch_path() / xmlfile).normalize();
            fs::path outdir = fs::complete(actions.outdir).normalize();
            xmlfile = path_difference(outdir, xmlfile);
        }
        out << "\n<xi:include href=\"";
        detail::print_string(detail::escape_uri(xmlfile.string()), out.get());
        out << "\" />\n";
    }

    void include_action::operator()(iterator const& first, iterator const& last) const
    {
        fs::path filein(std::string(first, last), fs::native);
        std::string doc_type, doc_id, doc_dirname, doc_last_revision;

        // check to see if the path is complete and if not, make it relative to the current path
        if (!filein.is_complete())
        {
            filein = actions.filename.branch_path() / filein;
            filein.normalize();
        }

        // swap the filenames
        std::swap(actions.filename, filein);

        // save the doc info strings
        actions.doc_type.swap(doc_type);
        actions.doc_id.swap(doc_id);
        actions.doc_dirname.swap(doc_dirname);
        actions.doc_last_revision.swap(doc_last_revision);

        // scope the macros
        string_symbols macro = actions.macro;
        // scope the templates
        template_symbols templates = actions.templates;

        // if an id is specified in this include (as in [include:id foo.qbk])
        // then use it as the doc_id.
        if (!actions.include_doc_id.empty())
        {
            actions.doc_id = actions.include_doc_id;
            actions.include_doc_id.clear();
        }

        // update the __FILENAME__ macro
        *boost::spirit::find(actions.macro, "__FILENAME__") = actions.filename.native_file_string();

        // parse the file
        quickbook::parse(actions.filename.native_file_string().c_str(), actions, true);

        // restore the values
        std::swap(actions.filename, filein);

        actions.doc_type.swap(doc_type);
        actions.doc_id.swap(doc_id);
        actions.doc_dirname.swap(doc_dirname);
        actions.doc_last_revision.swap(doc_last_revision);

        // restore the macros
        actions.macro = macro;
        // restore the templates
        actions.templates = templates;
    }

    void xml_author::operator()(std::pair<std::string, std::string> const& author) const
    {
        out << "    <author>\n"
            << "      <firstname>" << author.first << "</firstname>\n"
            << "      <surname>" << author.second << "</surname>\n"
            << "    </author>\n";
    }

    void xml_year::operator()(std::string const &year) const
    {
        out << "      <year>" << year << "</year>\n";
    }

    void pre(collector& out, quickbook::actions& actions, bool ignore_docinfo)
    {
        // The doc_info in the file has been parsed. Here's what we'll do 
        // *before* anything else.

        if (actions.doc_id.empty())
            actions.doc_id = detail::make_identifier(
                actions.doc_title.begin(),actions.doc_title.end());

        if (actions.doc_dirname.empty())
            actions.doc_dirname = actions.doc_id;

        if (actions.doc_last_revision.empty())
        {
            // default value for last-revision is now

            char strdate[64];
            strftime(
                strdate, sizeof(strdate),
                (debug_mode ? 
                    "DEBUG MODE Date: %Y/%m/%d %H:%M:%S $" :
                    "$" /* prevent CVS substitution */ "Date: %Y/%m/%d %H:%M:%S $"),
                current_gm_time
            );
            actions.doc_last_revision = strdate;
        }

        // if we're ignoring the document info, we're done.
        if (ignore_docinfo)
        {
            return;
        }

        if (qbk_major_version == 0)
        {
            // hard code quickbook version to v1.1
            qbk_major_version = 1;
            qbk_minor_version = 1;
            qbk_version_n = 101;
            detail::outwarn(actions.filename.native_file_string(),1)
                << "Warning: Quickbook version undefined. "
                "Version 1.1 is assumed" << std::endl;
        }
        else
        {
            qbk_version_n = (qbk_major_version * 100) + qbk_minor_version; 
        }

        out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            << "<!DOCTYPE library PUBLIC \"-//Boost//DTD BoostBook XML V1.0//EN\"\n"
            << "     \"http://www.boost.org/tools/boostbook/dtd/boostbook.dtd\">\n"
            << '<' << actions.doc_type << "\n"
            << "    id=\"" << actions.doc_id << "\"\n"
            << "    name=\"" << actions.doc_title << "\"\n"
            << "    dirname=\"" << actions.doc_dirname << "\"\n"
            << "    last-revision=\"" << actions.doc_last_revision << "\" \n"
            << "    xmlns:xi=\"http://www.w3.org/2001/XInclude\">\n"
            << "  <" << actions.doc_type << "info>\n";

        for_each(
            actions.doc_authors.begin()
          , actions.doc_authors.end()
          , xml_author(out));

        if (!actions.doc_copyright_holder.empty())
        {
            out << "\n" << "    <copyright>\n";

            for_each(
                actions.doc_copyright_years.begin()
              , actions.doc_copyright_years.end()
              , xml_year(out));

            out << "      <holder>" << actions.doc_copyright_holder << "</holder>\n"
                << "    </copyright>\n"
                << "\n"
            ;
        }

        if (qbk_version_n < 103)
        {
            // version < 1.3 compatibility
            actions.doc_license = actions.doc_license_1_1;
            actions.doc_purpose = actions.doc_purpose_1_1;
        }

        if (!actions.doc_license.empty())
        {
            out << "    <legalnotice>\n"
                << "      <para>\n"
                << "        " << actions.doc_license << "\n"
                << "      </para>\n"
                << "    </legalnotice>\n"
                << "\n"
            ;
        }

        if (!actions.doc_purpose.empty())
        {
            out << "    <" << actions.doc_type << "purpose>\n"
                << "      " << actions.doc_purpose
                << "    </" << actions.doc_type << "purpose>\n"
                << "\n"
            ;
        }

        if (!actions.doc_category.empty())
        {
            out << "    <" << actions.doc_type << "category name=\"category:"
                << actions.doc_category
                << "\"></" << actions.doc_type << "category>\n"
                << "\n"
            ;
        }

        out << "  </" << actions.doc_type << "info>\n"
            << "\n"
        ;

        if (!actions.doc_title.empty())
        {
            out << "  <title>" << actions.doc_title;
            if (!actions.doc_version.empty())
                out << ' ' << actions.doc_version;
            out << "</title>\n\n\n";
        }
    }

    void post(collector& out, quickbook::actions& actions, bool ignore_docinfo)
    {
        // if we're ignoring the document info, do nothing.
        if (ignore_docinfo)
        {
            return;
        }

        // We've finished generating our output. Here's what we'll do
        // *after* everything else.
        out << "\n</" << actions.doc_type << ">\n\n";
    }

    void phrase_to_string_action::operator()(iterator const& first, iterator const& last) const
    {
        out = phrase.str();
        phrase.str(std::string());
    }
}

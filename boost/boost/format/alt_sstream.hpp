// ----------------------------------------------------------------------------
//  alt_sstream.hpp : alternative stringstream 
// ----------------------------------------------------------------------------

//  Copyright Samuel Krempp 2003. Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/format for library home page

// ----------------------------------------------------------------------------



#ifndef BOOST_SK_ALT_SSTREAM_HPP
#define BOOST_SK_ALT_SSTREAM_HPP

#include <string>
#include <boost/format/detail/compat_workarounds.hpp>
#include <boost/utility/base_from_member.hpp>
#include <boost/shared_ptr.hpp>


namespace boost {
    namespace io {

        template<class Ch, class Tr=::std::char_traits<Ch>, 
                 class Alloc=::std::allocator<Ch> >
        class basic_altstringbuf;

        template<class Ch, class Tr =::std::char_traits<Ch>, 
                 class Alloc=::std::allocator<Ch> >
        class basic_oaltstringstream;


        template<class Ch, class Tr, class Alloc>
        class basic_altstringbuf 
            : public ::std::basic_streambuf<Ch, Tr>
        {
            typedef ::std::basic_streambuf<Ch, Tr>  streambuf_t;
        public:
            typedef typename CompatTraits<Tr>::type_for_string stringTr;
            typedef typename CompatAlloc<Alloc>::type_for_string stringAlloc;
            typedef typename CompatTraits<Tr>::compatible_type traits_type;
            typedef Alloc   allocator_type;

            typedef ::std::basic_string<Ch, stringTr, stringAlloc> string_type;
            typedef ::std::ios_base ios_base;

            typedef ::std::streamsize streamsize;
            typedef Ch                             char_type;
            typedef typename traits_type::int_type int_type;
            typedef typename traits_type::pos_type pos_type;
            typedef typename traits_type::off_type off_type;


            explicit basic_altstringbuf(ios_base::openmode mode
                                        = ios_base::in | ios_base::out)
                : putend_(NULL), is_allocated_(false), mode_(mode) 
                {}
            explicit basic_altstringbuf(const string_type& s,
                                        ios_base::openmode mode
                                        = ios_base::in | ios_base::out)
                : putend_(NULL), is_allocated_(false), mode_(mode) 
                { dealloc(); str(s); }
            virtual ~basic_altstringbuf() 
                { dealloc(); }
            using streambuf_t::pbase;
            using streambuf_t::pptr;
            using streambuf_t::epptr;
            using streambuf_t::eback;
            using streambuf_t::gptr;
            using streambuf_t::egptr;
    
            void clear_buffer();
            void str(const string_type& s);

            // 0-copy access :
            Ch * begin() const; 
            streamsize size() const;
            streamsize cur_size() const; // stop at current pointer
            Ch * pend() const // the highest position reached by pptr() since creation
                { return ((putend_ < pptr()) ? pptr() : putend_); }
            streamsize pcount() const 
                { return static_cast<streamsize>( pptr() - pbase()) ;}

            // copy buffer to string :
            string_type str() const 
                { return string_type(begin(), size()); }
            string_type cur_str() const 
                { return string_type(begin(), cur_size()); }
        protected:
            explicit basic_altstringbuf (basic_altstringbuf * s,
                                         ios_base::openmode mode 
                                         = ios_base::in | ios_base::out)
                : putend_(NULL), is_allocated_(false), mode_(mode) 
                { dealloc(); str(s); }

            virtual pos_type seekoff(off_type off, ios_base::seekdir way, 
                                     ios_base::openmode which 
                                     = ios_base::in | ios_base::out);
            virtual pos_type seekpos (pos_type pos, 
                                      ios_base::openmode which 
                                      = ios_base::in | ios_base::out);
            virtual int_type underflow();
            virtual int_type pbackfail(int_type meta = traits_type::eof());
            virtual int_type overflow(int_type meta = traits_type::eof());
            void dealloc();
        private:
            enum { alloc_min = 256}; // minimum size of allocations

            Ch *putend_;  // remembers (over seeks) the highest value of pptr()
            bool is_allocated_;
            ios_base::openmode mode_;
            allocator_type alloc_;  // the allocator object
        };


// ---   class basic_oaltstringstream ----------------------------------------
        template <class Ch, class Tr, class Alloc>
        class basic_oaltstringstream 
            : base_from_member< shared_ptr< basic_altstringbuf< Ch, Tr, Alloc> > >  ,
              public CompatOStream< ::std::basic_ostream<Ch, Tr> > ::compatible_type
        {
            class No_Op { 
                // used as no-op deleter for (not-owner) shared_pointers
            public: 
                template<class T>
                const T & operator()(const T & arg) { return arg; }
            };
            typedef typename CompatTraits<Tr>::type_for_string stringTr;
            typedef typename CompatAlloc<Alloc>::type_for_string stringAlloc;
            typedef typename CompatOStream<std::basic_ostream<Ch, Tr> > ::compatible_type
                stream_t;
            typedef boost::base_from_member<boost::shared_ptr<
                basic_altstringbuf<Ch,Tr, Alloc> > > 
                pbase_type;
        public:
            typedef typename CompatTraits<Tr>::compatible_type traits_type;
            typedef typename CompatAlloc<Tr>::compatible_type  allocator_type;
            typedef ::std::basic_string<Ch, stringTr, stringAlloc> string_type;
            typedef basic_altstringbuf<Ch, Tr, Alloc>   stringbuf_t;
        
            basic_oaltstringstream() 
                : pbase_type(new stringbuf_t), stream_t(rdbuf()) 
                { }
            basic_oaltstringstream(::boost::shared_ptr<stringbuf_t> buf) 
                : pbase_type(buf), stream_t(rdbuf()) 
                { }
            basic_oaltstringstream(stringbuf_t * buf) 
                : pbase_type(buf, No_Op() ), stream_t(rdbuf()) 
                { }
            stringbuf_t * rdbuf() const 
                { return pbase_type::member.get(); }
            void clear_buffer() 
                { rdbuf()->clear_buffer(); }

            // 0-copy access :
            Ch * begin() const 
                { return rdbuf()->begin(); }
            ::std::streamsize size() const 
                { return rdbuf()->size(); }
            ::std::streamsize cur_size() const // stops at current position
                { return rdbuf()->cur_size(); }

            // copy buffer to string :
            string_type str()     const   // [pbase, epptr[
                { return rdbuf()->str(); } 
            string_type cur_str() const   // [pbase, pptr[
                { return rdbuf()->cur_str(); }
        };

    } // N.S. io
} // N.S. boost

#include <boost/format/alt_sstream_impl.hpp>

#endif // include guard


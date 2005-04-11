// (C) Copyright Jonathan Turkanis 2003.
// (C) Copyright Craig Henderson 2002.   'boost/memmap.hpp' from sandbox
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

//
// This header and its accompanying source file libs/iostreams/memmap.cpp are
// an adaptation of Craig Henderson's memmory mapped file library. The 
// interface has been revised significantly, but the underlying OS-specific
// code is essentially the same, with some code from Boost.Filesystem
// mixed in. (See notations in source.)
//
// The following changes have been made:
//  
// 1. OS-specific code put in a .cpp file.
// 2. Name of main class changed to mapped_file.
// 3. mapped_file given an interface similar to std::fstream (open(),
//    is_open(), close()) and std::string (data(), size(), begin(), end()).
// 4. An additional class readonly_mapped_file has been provided as a 
//    convenience.
// 5. [Obsolete: Error states are reported using filesystem::error_code.]
// 6. Read-only or read-write states are specified using ios_base::openmode.
// 7. Access to the underlying file handles and to security parameters
//    has been removed.
//

#ifndef BOOST_IOSTREAMS_MAPPED_FILE_HPP_INCLUDED
#define BOOST_IOSTREAMS_MAPPED_FILE_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif  

#include <boost/config.hpp>                   // make sure size_t is in std.
#include <cstddef>                            // size_t.
#include <string>                             // pathnames.
#include <utility>                            // pair.
#include <boost/cstdint.hpp>                  // intmax_t.
#include <boost/config.hpp>                   // BOOST_MSVC.
#include <boost/detail/workaround.hpp>
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/detail/config/auto_link.hpp>
#include <boost/iostreams/detail/config/dyn_link.hpp>
#include <boost/iostreams/detail/ios.hpp>     // openmode.
#include <boost/iostreams/operations.hpp>
#include <boost/shared_ptr.hpp>

// Must come last.
#include <boost/iostreams/detail/config/disable_warnings.hpp>
#include <boost/config/abi_prefix.hpp>        

namespace boost { namespace iostreams {

namespace detail {

class mapped_file; 
struct mapped_file_impl; 

} // End namespace detail.

//------------------Definition of mapped_file_source--------------------------//

class BOOST_IOSTREAMS_DECL mapped_file_source {
private:
    struct safe_bool_helper { int x; };         // From Bronek Kozicki.
    typedef int safe_bool_helper::* safe_bool;
    friend struct detail::direct_impl<mapped_file_source>;
public:
    typedef char               char_type;
    struct io_category
        : public source_tag,
          public direct_tag,
          public closable_tag
        { };
    typedef std::size_t        size_type;
    typedef const char*        iterator;
    BOOST_STATIC_CONSTANT(size_type, max_length = static_cast<size_type>(-1));

    mapped_file_source() { }
    mapped_file_source( const std::string& path, 
                        size_type length = max_length,
                        boost::intmax_t offset = 0 );

    //--------------Stream interface------------------------------------------//

    void open( const std::string& path, 
               size_type length = max_length,
               boost::intmax_t offset = 0 );
    bool is_open() const; 
    void close();

    operator safe_bool() const;
    bool operator!() const;
    BOOST_IOS::openmode mode() const;
                    
    //--------------Container interface---------------------------------------//

    size_type size() const;
    const char* data() const;
    iterator begin() const;
    iterator end() const;

    //--------------Query admissible offsets----------------------------------//

    // Returns the allocation granularity for virtual memory. Values passed
    // as offsets must be multiples of this value.
    static int alignment();
private:
    friend class mapped_file;
    typedef detail::mapped_file_impl impl_type;
    void open( const std::string& pathname, 
               BOOST_IOS::openmode,
               size_type length, boost::intmax_t offset );

    boost::shared_ptr<impl_type> pimpl_;
};

//------------------Definition of mapped_file---------------------------------//

class mapped_file {
private:
    typedef mapped_file_source delegate_type;
    delegate_type delegate_;
    friend struct detail::direct_impl<mapped_file>;
public:
    typedef char                           char_type;
    struct io_category
        : public seekable_device_tag,
          public direct_tag,
          public closable_tag
        { };
    typedef mapped_file_source::size_type  size_type;
    typedef char*                          iterator;
    typedef const char*                    const_iterator;
    BOOST_STATIC_CONSTANT(size_type, max_length = delegate_type::max_length);
    mapped_file() { }
    mapped_file( const std::string& path, 
                 BOOST_IOS::openmode mode = 
                    BOOST_IOS::in | BOOST_IOS::out,
                 size_type length = max_length,
                 boost::intmax_t offset = 0 ) 
    { delegate_.open(path, mode, length, offset); }

    //--------------Conversion to readonly_mapped_file------------------------//

    operator mapped_file_source&() { return delegate_; }
    operator const mapped_file_source&() const { return delegate_; }

    //--------------Stream interface------------------------------------------//

    void open( const std::string& path, 
               BOOST_IOS::openmode mode = 
                   BOOST_IOS::in | BOOST_IOS::out,
               size_type length = max_length,
               boost::intmax_t offset = 0 )
    { delegate_.open(path, mode, length, offset); }
    bool is_open() const { return delegate_.is_open(); }
    void close() { delegate_.close(); }
    operator delegate_type::safe_bool() const { return delegate_; }
    bool operator!() const { return !is_open(); }
    BOOST_IOS::openmode mode() const { return delegate_.mode(); }

    //--------------Container interface---------------------------------------//

    size_type size() const { return delegate_.size(); }
    char* data() const { return const_cast<char*>(delegate_.data()); }
    const char* const_data() const { return delegate_.data(); }
    iterator begin() const { return data(); } 
    const_iterator const_begin() const { return data(); } 
    iterator end() const { return data() + size(); } 
    const_iterator const_end() const { return data() + size(); } 

    //--------------Query admissible offsets----------------------------------//

    // Returns the allocation granularity for virtual memory. Values passed
    // as offsets must be multiples of this value.
    static int alignment() { return mapped_file_source::alignment(); }
};

struct mapped_file_sink : private mapped_file {
    friend struct detail::direct_impl<mapped_file_sink>;
    typedef char char_type;
    struct io_category
        : public sink_tag,
          public direct_tag,
          public closable_tag
        { };
    using mapped_file::close;
    mapped_file_sink( const std::string& path, 
                      size_type length = max_length,
                      boost::intmax_t offset = 0 ) 
        : mapped_file(path, BOOST_IOS::out | BOOST_IOS::trunc) { }
};
                    
//------------------Specialization of direct_impl-----------------------------//

namespace detail {

template<>
struct direct_impl<boost::iostreams::mapped_file_source> {
    static std::pair<char*, char*> 
    input_sequence(boost::iostreams::mapped_file_source& src)
    { 
        return std::make_pair( const_cast<char*>(src.begin()), 
                               const_cast<char*>(src.end()) ); 
    }
};

template<>
struct direct_impl<boost::iostreams::mapped_file_sink> {
    static std::pair<char*, char*> 
    output_sequence(boost::iostreams::mapped_file_sink& sink)
    { return std::make_pair(sink.begin(), sink.end()); }
};

template<>
struct direct_impl<boost::iostreams::mapped_file> {
    static std::pair<char*, char*> 
    input_sequence(boost::iostreams::mapped_file& file)
    { return std::make_pair(file.begin(), file.end()); }

    static std::pair<char*, char*> 
    output_sequence(boost::iostreams::mapped_file& file)
    { return std::make_pair(file.begin(), file.end()); }
};

} // End namespace detail.

} } // End namespaces iostreams, boost.

#include <boost/config/abi_suffix.hpp> // pops abi_suffix.hpp pragmas
#include <boost/iostreams/detail/config/enable_warnings.hpp> // MSVC.

#endif // #ifndef BOOST_IOSTREAMS_MAPPED_FILE_HPP_INCLUDED

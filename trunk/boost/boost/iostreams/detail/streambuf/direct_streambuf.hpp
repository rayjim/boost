// (C) Copyright Jonathan Turkanis 2003.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef BOOST_IOSTREAMS_DETAIL_DIRECT_STREAMBUF_HPP_INCLUDED
#define BOOST_IOSTREAMS_DETAIL_DIRECT_STREAMBUF_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif              

#include <cassert>
#include <cstddef>
#include <typeinfo>
#include <utility>                              // pair.
#include <boost/config.hpp>                     // BOOST_DEDUCED_TYPENAME.
#include <boost/iostreams/detail/char_traits.hpp>
#include <boost/iostreams/detail/config/wide_streams.hpp>
#include <boost/iostreams/detail/ios.hpp>
#include <boost/iostreams/detail/optional.hpp>
#include <boost/iostreams/detail/streambuf.hpp>
#include <boost/iostreams/detail/streambuf/linked_streambuf.hpp>
#include <boost/iostreams/detail/error.hpp>
#include <boost/iostreams/operations.hpp>
#include <boost/iostreams/traits.hpp>

// Must come last.
#include <boost/iostreams/detail/config/disable_warnings.hpp> // MSVC.

namespace boost { namespace iostreams { 
    
namespace detail {

template< typename T,
          typename Tr = 
              BOOST_IOSTREAMS_CHAR_TRAITS(
                 BOOST_DEDUCED_TYPENAME char_type_of<T>::type 
              ) >
class direct_streambuf 
    : public linked_streambuf<BOOST_DEDUCED_TYPENAME char_type_of<T>::type, Tr>
{
public:
    typedef typename char_type_of<T>::type                char_type;
    BOOST_IOSTREAMS_STREAMBUF_TYPEDEFS(Tr)
private:
    typedef linked_streambuf<char_type, traits_type>      base_type;
    typedef typename category_of<T>::type                 category;
    typedef BOOST_IOSTREAMS_BASIC_STREAMBUF(
                char_type, traits_type
            )                                             streambuf_type;
public: // stream_facade needs access.
    void open(const T& t, int buffer_size, int pback_size);
    bool is_open();
    void close();
    bool auto_close() const { return auto_close_; }
    void set_auto_close(bool close) { auto_close_ = close; }
    bool strict_sync() { return true; }

    // Declared in linked_streambuf.
    T* component() { return storage_.get(); }
protected:
#if !BOOST_WORKAROUND(__GNUC__, == 2)
    BOOST_IOSTREAMS_USING_PROTECTED_STREAMBUF_MEMBERS(base_type)
#endif
    direct_streambuf();

    //--------------Virtual functions-----------------------------------------//

    int_type underflow();
    int_type pbackfail(int_type c);
    int_type overflow(int_type c);
    pos_type seekoff( off_type off, BOOST_IOS::seekdir way,
                      BOOST_IOS::openmode which );
    pos_type seekpos(pos_type sp, BOOST_IOS::openmode which);

    // Declared in linked_streambuf.
    void close(BOOST_IOS::openmode m);
    const std::type_info& component_type() const { return typeid(T); }
    void* component_impl() { return component(); } 
private:
    pos_type seek_impl( off_type off, BOOST_IOS::seekdir way,
                        BOOST_IOS::openmode which );
    void init_input(any_tag) { }
    void init_input(input);
    void init_output(any_tag) { }
    void init_output(output);
    void init_get_area();
    void init_put_area();
    bool one_head() const;
    bool two_head() const;
    optional<T>  storage_;
    char_type   *ibeg_, *iend_, *obeg_, *oend_;
    bool         auto_close_;
};
                    
//------------------Implementation of direct_streambuf------------------------//

template<typename T, typename Tr>
direct_streambuf<T, Tr>::direct_streambuf() 
    : ibeg_(0), iend_(0), obeg_(0), oend_(0), auto_close_(true) 
{ this->set_true_eof(true); }

template<typename T, typename Tr>
void direct_streambuf<T, Tr>::open(const T& t, int, int)
{
    storage_.reset(t);
    init_input(category());
    init_output(category());
    setg(0, 0, 0);
    setp(0, 0);
}

template<typename T, typename Tr>
bool direct_streambuf<T, Tr>::is_open() { return ibeg_ != 0 && !obeg_ != 0; }

template<typename T, typename Tr>
void direct_streambuf<T, Tr>::close() 
{ 
    using namespace std;
    try { close(BOOST_IOS::in); } catch (std::exception&) { }
    try { close(BOOST_IOS::out); } catch (std::exception&) { }
    storage_.reset();
}

template<typename T, typename Tr>
typename direct_streambuf<T, Tr>::int_type 
direct_streambuf<T, Tr>::underflow()
{
    if (!ibeg_) 
        throw cant_read();
    if (!gptr()) 
        init_get_area();
    return gptr() != iend_ ? 
        traits_type::to_int_type(*gptr()) : 
        traits_type::eof();
}

template<typename T, typename Tr>
typename direct_streambuf<T, Tr>::int_type 
direct_streambuf<T, Tr>::pbackfail(int_type c)
{
    using namespace std;
    if (!ibeg_) 
        throw cant_read();
    if (gptr() != 0 && gptr() != ibeg_) {
        gbump(-1);
        if (!traits_type::eq_int_type(c, traits_type::eof()))
            *gptr() = traits_type::to_char_type(c);
        return traits_type::not_eof(c);
    }
    throw bad_putback();
}

template<typename T, typename Tr>
typename direct_streambuf<T, Tr>::int_type 
direct_streambuf<T, Tr>::overflow(int_type c)
{
    using namespace std;
    if (!obeg_) throw BOOST_IOSTREAMS_FAILURE("no write access");
    if (!pptr()) init_put_area();
    if (!traits_type::eq_int_type(c, traits_type::eof())) {
        if (pptr() == oend_)
            throw BOOST_IOSTREAMS_FAILURE("write area exhausted");
        *pptr() = traits_type::to_char_type(c);
        pbump(1);
        return c;
    }
    return traits_type::not_eof(c);
}

template<typename T, typename Tr>
inline typename direct_streambuf<T, Tr>::pos_type
direct_streambuf<T, Tr>::seekoff
    (off_type off, BOOST_IOS::seekdir way, BOOST_IOS::openmode which)
{ return seek_impl(off, way, which); }

template<typename T, typename Tr>
inline typename direct_streambuf<T, Tr>::pos_type
direct_streambuf<T, Tr>::seekpos
    (pos_type sp, BOOST_IOS::openmode)
{ return seek_impl(sp, BOOST_IOS::beg, BOOST_IOS::in | BOOST_IOS::out); }

template<typename T, typename Tr>
void direct_streambuf<T, Tr>::close(BOOST_IOS::openmode which)
{
    if (which == BOOST_IOS::in && ibeg_ != 0) {
        setg(0, 0, 0);
        ibeg_ = iend_ = 0;
    }
    if (which == BOOST_IOS::out && obeg_ != 0) {
        sync();
        setp(0, 0);
        obeg_ = oend_ = 0;
    }
    boost::iostreams::close(*storage_, which);
}

template<typename T, typename Tr>
typename direct_streambuf<T, Tr>::pos_type direct_streambuf<T, Tr>::seek_impl
    (off_type off, BOOST_IOS::seekdir way, BOOST_IOS::openmode which)
{
    using namespace std;
    BOOST_IOS::openmode both = BOOST_IOS::in | BOOST_IOS::out;
    if (two_head() && (which & both) == both)
        throw bad_seek();
    off_type result = -1;
    bool one = one_head();
    if (one && (pptr() != 0 || gptr()== 0))
        init_get_area(); // Switch to input mode, for code reuse.
    if (one || (which & BOOST_IOS::in) != 0 && ibeg_ != 0) {
        if (!gptr()) setg(ibeg_, ibeg_, iend_);
        ptrdiff_t next = 0;
        switch (way) {
        case BOOST_IOS::beg: next = off; break;
        case BOOST_IOS::cur: next = (gptr() - ibeg_) + off; break;
        case BOOST_IOS::end: next = (iend_ - ibeg_) + off; break;
        default: assert(0);
        }
        if (next < 0 || next > (iend_ - ibeg_))
            throw bad_seek();
        setg(ibeg_, ibeg_ + next, iend_);
        result = next;
    }
    if (!one && (which & BOOST_IOS::out) != 0 && obeg_ != 0) {
        if (!pptr()) setp(obeg_, oend_);
        ptrdiff_t next = 0;
        switch (way) {
        case BOOST_IOS::beg: next = off; break;
        case BOOST_IOS::cur: next = (pptr() - obeg_) + off; break;
        case BOOST_IOS::end: next = (oend_ - obeg_) + off; break;
        default: assert(0);
        }
        if (next < 0 || next > (oend_ - obeg_))
            throw bad_seek();
        pbump(static_cast<int>(next - (pptr() - obeg_)));
        result = next;
    }
    return result;
}

template<typename T, typename Tr>
void direct_streambuf<T, Tr>::init_input(input)
{
    std::pair<char_type*, char_type*> p = input_sequence(*storage_);
    ibeg_ = p.first;
    iend_ = p.second;
}

template<typename T, typename Tr>
void direct_streambuf<T, Tr>::init_output(output)
{
    std::pair<char_type*, char_type*> p = output_sequence(*storage_);
    obeg_ = p.first;
    oend_ = p.second;
}

template<typename T, typename Tr>
void direct_streambuf<T, Tr>::init_get_area()
{
    setg(ibeg_, ibeg_, iend_);
    if (one_head() && pptr()) {
        gbump(static_cast<int>(pptr() - obeg_));
        setp(0, 0);
    }
}

template<typename T, typename Tr>
void direct_streambuf<T, Tr>::init_put_area()
{
    setp(obeg_, oend_);
    if (one_head() && gptr()) {
        pbump(static_cast<int>(gptr() - ibeg_));
        setg(0, 0, 0);
    }
}

template<typename T, typename Tr>
inline bool direct_streambuf<T, Tr>::one_head() const
{ return ibeg_ && obeg_ && ibeg_ == obeg_; }

template<typename T, typename Tr>
inline bool direct_streambuf<T, Tr>::two_head() const
{ return ibeg_ && obeg_ && ibeg_ != obeg_; }

//----------------------------------------------------------------------------//

} // End namespace detail.

} } // End namespaces iostreams, boost.

#include <boost/iostreams/detail/config/enable_warnings.hpp> // MSVC

#endif // #ifndef BOOST_IOSTREAMS_DETAIL_DIRECT_STREAMBUF_HPP_INCLUDED

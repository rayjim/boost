// (C) Copyright Jonathan Turkanis 2003.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef BOOST_IOSTREAMS_DETAIL_CHAIN_HPP_INCLUDED
#define BOOST_IOSTREAMS_DETAIL_CHAIN_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif              

#include <algorithm>                            // for_each.
#include <cassert>
#include <exception>
#include <functional>                           // unary_function.
#include <list>
#include <memory>                               // allocator, auto_ptr.
#include <stdexcept>                            // logic_error.
#include <boost/checked_delete.hpp>
#include <boost/config.hpp>                     // BOOST_MSVC, template friends.
#include <boost/detail/workaround.hpp>
#include <boost/iostreams/constants.hpp>
#include <boost/iostreams/traits.hpp>
#include <boost/iostreams/detail/access_control.hpp>
#include <boost/iostreams/detail/char_traits.hpp>
#include <boost/iostreams/detail/push.hpp>
#include <boost/iostreams/detail/streambuf.hpp> // pubsync.
#include <boost/iostreams/detail/wrap_unwrap.hpp>
#include <boost/iostreams/traits.hpp>           // is_filter.
#include <boost/iostreams/streambuf_facade.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_convertible.hpp>

namespace boost { namespace iostreams {

namespace detail {

template<typename Chain> class chain_client;

//
// Concept name: Chain.
// Description: Represents a chain of stream buffers which provides access 
//     to the first buffer in the chain and send notifications when the 
//     streambufs are added to or removed from chain.
// Refines: Closable device with mode equal to typename Chain::mode.
// Models: chain, converting_chain.
// Example:
//
//    class chain {
//    public:
//        typedef xxx chain_type;
//        typedef xxx client_type;
//        typedef xxx mode;
//        bool is_complete() const;                  // Ready for i/o.
//        template<typename T>
//        void push( const T& t,                     // Adds a stream buffer to
//                   streamsize,                     // chain, based on t, with 
//                   streamsize );                   // given buffer and putback
//                                                   // buffer sizes. Pass -1 to
//                                                   // request default size.
//    protected:
//        void register_client(client_type* client); // Associate client.
//        void notify();                             // Notify client.
//    };
// 

//
// Description: Represents a chain of filters with an optional device at the
//      end.
// Template parameters:
//      Self - A class deriving from the current instantiation of this template.
//          This is an example of the Curiously Recurring Template Pattern.
//      Ch - The character type.
//      Tr - The character traits type.
//      Alloc - The allocator type.
//      Mode - A mode tag.
//
template<typename Self, typename Ch, typename Tr, typename Alloc, typename Mode>
class chain_base {
public:
    typedef Ch                                     char_type;
    BOOST_IOSTREAMS_STREAMBUF_TYPEDEFS(Tr)
    typedef Alloc                                  allocator_type;
    typedef Mode                                   mode;
    struct io_category
        : Mode,
          device_tag,
          closable_tag 
        { };
    typedef chain_client<Self>                     client_type;
    friend class chain_client<Self>;
private:
    typedef linked_streambuf<Ch>                   streambuf_type;
    typedef std::list<streambuf_type*>             list_type;
    typedef chain_base<Self, Ch, Tr, Alloc, Mode>  my_type;
protected:
    chain_base() : pimpl_(new chain_impl) { }
    chain_base(const chain_base& rhs): pimpl_(rhs.pimpl_) { }
public:

    //----------Buffer sizing-------------------------------------------------//

    // Sets the size of the buffer created for the devices to be added to this
    // chain. Does not affect the size of the buffer for devices already
    // added.
    void set_device_buffer_size(int n) { pimpl_->device_buffer_size_ = n; }

    // Sets the size of the buffer created for the filters to be added
    // to this chain. Does not affect the size of the buffer for filters already
    // added.
    void set_filter_buffer_size(int n) { pimpl_->filter_buffer_size_ = n; }

    // Sets the size of the putback buffer for filters and devices to be added
    // to this chain. Does not affect the size of the buffer for filters or
    // devices already added.
    void set_pback_size(int n) { pimpl_->pback_size_ = n; }

    //----------Device interface----------------------------------------------//

    std::streamsize read(char_type* s, std::streamsize n);
    void write(const char_type* s, std::streamsize n);
    std::streamoff seek(std::streamoff off, BOOST_IOS::seekdir way);

    //----------Direct stream buffer access-----------------------------------//

    streambuf_type& operator*() { return *list().front(); }
    streambuf_type* operator->() { return list().front(); }
public:
    typedef typename list_type::size_type size_type;
    BOOST_IOSTREAMS_DEFINE_PUSH(push, mode, char_type, push_impl)
    void pop();
    bool empty() const { return list().empty(); }
    size_type size() const { return list().size(); }
    void reset();

    // Returns true if this chain is non-empty and its final link
    // is a source or sink, i.e., if it is ready to perform i/o.
    bool is_complete() const;
    bool auto_close() const;
    void set_auto_close(bool close);
private:
    template<typename T>
    void push_impl(const T& t, int buffer_size = -1, int pback_size = -1)
        {
            typedef typename iostreams::io_category<T>::type  category;
            typedef typename unwrap_ios<T>::type              policy_type;
            typedef streambuf_facade<
                        policy_type, 
                        BOOST_IOSTREAMS_CHAR_TRAITS(char_type),
                        Alloc, Mode
                    >                                         facade_type;
            BOOST_STATIC_ASSERT((is_convertible<category, Mode>::value));
            if (is_complete()) 
                throw std::logic_error("chain complete");
            streambuf_type* prev = !empty() ? list().back() : 0;
            buffer_size = 
                buffer_size != -1 ? 
                    buffer_size : 
                    iostreams::optimal_buffer_size(t);
            pback_size = 
                pback_size != -1 ? 
                    pback_size : 
                    pimpl_->pback_size_;
            std::auto_ptr<facade_type>
                buf(new facade_type(t, buffer_size, pback_size));
            list().push_back(buf.get());
            buf.release();
            if (is_device<policy_type>::value)
                pimpl_->flags_ |= f_complete;
            if (prev) prev->set_next(list().back());
            notify();
        }

    list_type& list() { return pimpl_->links_; }
    const list_type& list() const { return pimpl_->links_; }
    void register_client(client_type* client) { pimpl_->client_ = client; }
    void notify() { if (pimpl_->client_) pimpl_->client_->notify(); }

    //----------Nested classes------------------------------------------------//

    // The static member close and the structs closer are used by
    // chain_impl::close. A more elegant solution using boost::bind failed on
    // VC6 and Borland.

    static void close(streambuf_type* b, BOOST_IOS::openmode m)
    { 
        if (m & BOOST_IOS::out) 
            b->BOOST_IOSTREAMS_PUBSYNC(); 
        b->close(m); 
    }

    struct closer  : public std::unary_function<streambuf_type*, void>  {
        closer(BOOST_IOS::openmode m) : mode_(m) { }
        void operator() (streambuf_type* b) { close(b, mode_); }
        BOOST_IOS::openmode mode_;
    };
    friend struct closer;

    enum { 
        f_complete = 1, 
        f_auto_close = 2
    };

    struct chain_impl {
        chain_impl()
            : client_(0), device_buffer_size_(default_device_buffer_size),
              filter_buffer_size_(default_filter_buffer_size),
              pback_size_(default_pback_buffer_size),
              flags_(f_auto_close)
            { }
        ~chain_impl()
            {
                try { 
                    if ((flags_ & f_auto_close) != 0)
                        close(); 
                } catch (std::exception&) { }
                std::for_each( links_.begin(), links_.end(),
                               checked_deleter<streambuf_type>() );
            }
        void close()
            {
                if ((flags_ & f_complete) == 0) 
                    return;
                links_.front()->BOOST_IOSTREAMS_PUBSYNC();
                if (is_convertible<Mode, input>::value)
                    std::for_each( links_.rbegin(), links_.rend(), 
                                   closer(BOOST_IOS::in) );
                if (is_convertible<Mode, output>::value)
                    std::for_each( links_.begin(), links_.end(), 
                                   closer(BOOST_IOS::out) );
            }
        list_type     links_;
        client_type*  client_;
        int           device_buffer_size_,
                      filter_buffer_size_,
                      pback_size_;
        int           flags_;
    };
    friend struct chain_impl;

    //----------Member data---------------------------------------------------//

private:
    shared_ptr<chain_impl> pimpl_;
};

//
// Macro: BOOST_IOSTREAMS_DECL_CHAIN(name, category)
// Description: Defines a template derived from chain_base appropriate for a
//      particular i/o category. The template has the following parameters:
//      Ch - The character type.
//      Tr - The character traits type.
//      Alloc - The allocator type.
// Macro parameters:
//      name_ - The name of the template to be defined.
//      category_ - The i/o category of the template to be defined.
//
#define BOOST_IOSTREAMS_DECL_CHAIN(name_, default_char_) \
    template< typename Mode, typename Ch = default_char_, \
              typename Tr = BOOST_IOSTREAMS_CHAR_TRAITS(Ch), \
              typename Alloc = std::allocator<Ch> > \
    class name_ : public boost::iostreams::detail::chain_base< \
                            name_<Mode, Ch, Tr, Alloc>, \
                            Ch, Tr, Alloc, Mode \
                         > \
    { \
    public: \
        struct io_category : device_tag, Mode { }; \
        typedef Mode                                   mode; \
    private: \
        typedef boost::iostreams::detail::chain_base< \
                    name_<Mode, Ch, Tr, Alloc>, \
                    Ch, Tr, Alloc, Mode \
                >                                      base_type; \
    public: \
        typedef Ch                                     char_type; \
        typedef Tr                                     traits_type; \
        typedef typename traits_type::int_type         int_type; \
        typedef typename traits_type::off_type         off_type; \
        name_() { } \
        name_(const name_& rhs) { *this = rhs; } \
        name_& operator=(const name_& rhs) \
        { base_type::operator=(rhs); return *this; } \
    }; \
    /**/
BOOST_IOSTREAMS_DECL_CHAIN(chain, char)
BOOST_IOSTREAMS_DECL_CHAIN(wchain, wchar_t)
#undef BOOST_IOSTREAMS_DECL_CHAIN

//--------------Definition of chain_client------------------------------------//

//
// Template name: chain_client
// Description: Class whose instances provide access to an underlying chain
//      using an interface similar to the chains.
// Subclasses: the various stream and stream buffer templates.
//
template<typename Chain>
class chain_client {
public:
    typedef Chain                             chain_type;
    typedef typename chain_type::char_type    char_type;
    typedef typename chain_type::traits_type  traits_type;
    typedef typename chain_type::size_type    size_type;
    typedef typename chain_type::mode         mode;

    chain_client(chain_type* chn = 0) : chain_(chn ) { }
    chain_client(chain_client* client) : chain_(client->chain_) { }
    virtual ~chain_client() { }

        // Returns a copy of the underlying chain.
    chain_type filters() { return *chain_; }
    chain_type filters() const { return *chain_; }

    bool is_complete() const { return chain_->is_complete(); }
    bool auto_close() const { return chain_->auto_close(); }
    void set_auto_close(bool close) { chain_->set_auto_close(close); }
    void set_device_buffer_size(std::streamsize n) 
        { chain_->set_device_buffer_size(n); }
    void set_filter_buffer_size(std::streamsize n)
        { chain_->set_filter_buffer_size(n); }
    void set_pback_size(std::streamsize n) { chain_->set_pback_size(n); }
    BOOST_IOSTREAMS_DEFINE_PUSH(push, mode, char_type, push_impl)
    void pop() { chain_->pop(); }
    bool empty() const { return chain_->empty(); }
    size_type size() { return chain_->size(); }
    void reset() { chain_->reset(); }
protected:
    template<typename T>
    void push_impl(const T& t BOOST_IOSTREAMS_PUSH_PARAMS())
    { chain_->push(t BOOST_IOSTREAMS_PUSH_ARGS()); }
    chain_type& ref() { return *chain_; }
    void set_chain(chain_type* c) 
    { chain_ = c; chain_->register_client(this); }
#if !defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS) && \
    (!BOOST_WORKAROUND(__BORLANDC__, < 0x600))
    template<typename S, typename C, typename T, typename A, typename M>
    friend class chain_base;
#else
    public:
#endif
    virtual void notify() { }
private:
    chain_type* chain_;
};

//--------------Implementation of chain_base----------------------------------//

template<typename Self, typename Ch, typename Tr, typename Alloc, typename Mode>
inline std::streamsize chain_base<Self, Ch, Tr, Alloc, Mode>::read
    (char_type* s, std::streamsize n)
{ return list().front()->sgetn(s, n); }

template<typename Self, typename Ch, typename Tr, typename Alloc, typename Mode>
inline void chain_base<Self, Ch, Tr, Alloc, Mode>::write
    (const char_type* s, std::streamsize n)
{ list().front()->sputn(s, n); }

template<typename Self, typename Ch, typename Tr, typename Alloc, typename Mode>
inline std::streamoff chain_base<Self, Ch, Tr, Alloc, Mode>::seek
    (std::streamoff off, BOOST_IOS::seekdir way)
{ return list().front()->pubseekoff(off, way); }

template<typename Self, typename Ch, typename Tr, typename Alloc, typename Mode>
void chain_base<Self, Ch, Tr, Alloc, Mode>::reset()
{
    using namespace std;
    pimpl_->close();
    for_each(list().begin(), list().end(), checked_deleter<streambuf_type>());
    list().clear();
    pimpl_->flags_ &= ~f_complete;
}

template<typename Self, typename Ch, typename Tr, typename Alloc, typename Mode>
bool chain_base<Self, Ch, Tr, Alloc, Mode>::is_complete() const 
{ 
    return (pimpl_->flags_ & f_complete) != 0;
}

template<typename Self, typename Ch, typename Tr, typename Alloc, typename Mode>
bool chain_base<Self, Ch, Tr, Alloc, Mode>::auto_close() const 
{ 
    return (pimpl_->flags_ & f_auto_close) != 0;
}

template<typename Self, typename Ch, typename Tr, typename Alloc, typename Mode>
void chain_base<Self, Ch, Tr, Alloc, Mode>::set_auto_close(bool close) 
{ 
    pimpl_->flags_ = 
        (pimpl_->flags_ & ~f_auto_close) |
        (close ? f_auto_close : 0);
}

template<typename Self, typename Ch, typename Tr, typename Alloc, typename Mode>
void chain_base<Self, Ch, Tr, Alloc, Mode>::pop()
{
    assert(!empty());
    pimpl_->close();
    delete list().back();
    list().pop_back();
    pimpl_->flags_ &= ~f_complete;
}

} // End namespace detail.

} } // End namespaces iostreams, boost.

#endif // #ifndef BOOST_IOSTREAMS_DETAIL_CHAIN_HPP_INCLUDED

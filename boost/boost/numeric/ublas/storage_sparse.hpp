//
//  Copyright (c) 2000-2002
//  Joerg Walter, Mathias Koch
//
//  Permission to use, copy, modify, distribute and sell this software
//  and its documentation for any purpose is hereby granted without fee,
//  provided that the above copyright notice appear in all copies and
//  that both that copyright notice and this permission notice appear
//  in supporting documentation.  The authors make no representations
//  about the suitability of this software for any purpose.
//  It is provided "as is" without express or implied warranty.
//
//  The authors gratefully acknowledge the support of
//  GeNeSys mbH & Co. KG in producing this work.
//

#ifndef BOOST_UBLAS_STORAGE_SPARSE_H
#define BOOST_UBLAS_STORAGE_SPARSE_H

#include <map>

#include <boost/numeric/ublas/storage.hpp>


namespace boost { namespace numeric { namespace ublas {

    namespace detail {

        template<class I, class T, class C>
        BOOST_UBLAS_INLINE
        I lower_bound (const I &begin, const I &end, const T &t, C compare) {
            // t <= *begin <=> ! (*begin < t)
            if (begin == end || ! compare (*begin, t))
                return begin;
            if (compare (*(end - 1), t))
                return end;
            return std::lower_bound (begin, end, t, compare);
        }
        template<class I, class T, class C>
        BOOST_UBLAS_INLINE
        I upper_bound (const I &begin, const I &end, const T &t, C compare) {
            if (begin == end || compare (t, *begin))
                return begin;
            // (*end - 1) <= t <=> ! (t < *end)
            if (! compare (t, *(end - 1)))
                return end;
            return std::upper_bound (begin, end, t, compare);
        }

        template<class P>
        struct less_pair {
            BOOST_UBLAS_INLINE
            bool operator () (const P &p1, const P &p2) {
                return p1.first < p2.first;
            }
        };
        template<class T>
        struct less_triple {
            BOOST_UBLAS_INLINE
            bool operator () (const T &t1, const T &t2) {
                return t1.first.first < t2.first.first ||
                       (t1.first.first == t2.first.first && t1.first.second < t2.first.second);
            }
        };

    }

#ifdef BOOST_UBLAS_STRICT_MAP_ARRAY
    template<class A>
    class sparse_storage_element:
       public container_reference<A> {
    public:
        typedef A array_type;
        typedef typename A::key_type index_type;
        typedef typename A::mapped_type data_value_type;
        // typedef const data_value_type &data_const_reference;
        typedef typename type_traits<data_value_type>::const_reference data_const_reference;
        typedef data_value_type &data_reference;
        typedef typename A::value_type value_type;
        typedef value_type *pointer;

        // Construction and destruction
        BOOST_UBLAS_INLINE
        sparse_storage_element (array_type &a, pointer it):
            container_reference<array_type> (a), it_ (it), i_ (it->first), d_ (it->second), dirty_ (false) {}
        BOOST_UBLAS_INLINE
        sparse_storage_element (array_type &a, index_type i):
            container_reference<array_type> (a), it_ (), i_ (i), d_ (), dirty_ (false) {
            pointer it = (*this) ().find (i_);
            if (it == (*this) ().end ())
                it = (*this) ().insert ((*this) ().end (), value_type (i_, d_));
            d_ = it->second;
        }
        BOOST_UBLAS_INLINE
        ~sparse_storage_element () {
            if (dirty_) {
                if (! it_)
                    it_ = (*this) ().find (i_);
                BOOST_UBLAS_CHECK (it_ != (*this) ().end (), internal_logic ());
                it_->second = d_;
            }
        }

        // Element access - only if data_const_reference is defined
        BOOST_UBLAS_INLINE
        typename data_value_type::data_const_reference
        operator [] (index_type i) const {
            return d_ [i];
        }

        // Assignment
        BOOST_UBLAS_INLINE
        sparse_storage_element &operator = (const sparse_storage_element &p) {
            // Overide the implict copy assignment
            d_ = p.d_;
            dirty_ = true;
            return *this;
        }
        template<class D>
        BOOST_UBLAS_INLINE
        sparse_storage_element &operator = (const D &d) {
            d_ = d;
            dirty_ = true;
            return *this;
        }
        template<class D>
        BOOST_UBLAS_INLINE
        sparse_storage_element &operator += (const D &d) {
            d_ += d;
            dirty_ = true;
            return *this;
        }
        template<class D>
        BOOST_UBLAS_INLINE
        sparse_storage_element &operator -= (const D &d) {
            d_ -= d;
            dirty_ = true;
            return *this;
        }
        template<class D>
        BOOST_UBLAS_INLINE
        sparse_storage_element &operator *= (const D &d) {
            d_ *= d;
            dirty_ = true;
            return *this;
        }
        template<class D>
        BOOST_UBLAS_INLINE
        sparse_storage_element &operator /= (const D &d) {
            d_ /= d;
            dirty_ = true;
            return *this;
        }

        // Comparison
        template<class D>
        BOOST_UBLAS_INLINE
        bool operator == (const D &d) const {
            return d_ == d;
        }
        template<class D>
        BOOST_UBLAS_INLINE
        bool operator != (const D &d) const {
            return d_ != d;
        }

        // Conversion
        BOOST_UBLAS_INLINE
        operator data_const_reference () const {
            return d_;
        }

        // Swapping
        BOOST_UBLAS_INLINE
        void swap (sparse_storage_element p) {
            if (this != &p) {
                dirty_ = true;
                p.dirty_ = true;
                std::swap (d_, p.d_);
            }
        }
        BOOST_UBLAS_INLINE
        friend void swap (sparse_storage_element p1, sparse_storage_element p2) {
            p1.swap (p2);
        }

    private:
        pointer it_;
        index_type i_;
        data_value_type d_;
        bool dirty_;
    };
#endif


    // Default map type is simply forwarded to std::map
    // FIXME should use ALLOC for map but std::allocator of std::pair<const I, T> and std::pair<I,T> fail to compile
    template<class I, class T, class ALLOC>
    class map_std : public std::map<I, T /*, ALLOC */> {
    };


    // Map array
    //  Implementation requires pair<I, T> allocator definition (without const)
    template<class I, class T, class ALLOC>
    class map_array {
    public:
        typedef ALLOC allocator_type;
        typedef typename ALLOC::size_type size_type;
        typedef typename ALLOC::difference_type difference_type;
        typedef std::pair<I,T> value_type;
        typedef I key_type;
        typedef T mapped_type;
        typedef const value_type &const_reference;
        typedef value_type &reference;
        typedef const value_type *const_pointer;
        typedef value_type *pointer;
        typedef const T &data_const_reference;
#ifndef BOOST_UBLAS_STRICT_MAP_ARRAY
        typedef T &data_reference;
#else
        typedef sparse_storage_element<map_array> data_reference;
#endif

        // Construction and destruction
        BOOST_UBLAS_INLINE
        map_array (const ALLOC &a = ALLOC()):
            alloc_(a), capacity_ (0), size_ (0) {
                data_ = 0;
        }
        BOOST_UBLAS_INLINE
        map_array (const map_array &c):
            alloc_ (c.alloc_), capacity_ (c.size_), size_ (c.size_) {
            if (capacity_) {
                data_ = alloc_.allocate (capacity_);
                std::uninitialized_copy (data_, data_ + capacity_, c.data_);
                // capacity != size_ requires uninitialized_fill (size_ to capacity_)
            }
            else
                data_ = 0;
        }
        BOOST_UBLAS_INLINE
        ~map_array () {
            if (capacity_) {
                std::for_each (data_, data_ + capacity_, static_destroy);
                alloc_.deallocate (data_, capacity_);
            }
        }

    private:
        // Resizing - implicitly exposses uninitialized (but default constructed) mapped_type
        BOOST_UBLAS_INLINE
        void resize (size_type size) {
            BOOST_UBLAS_CHECK (size_ <= capacity_, internal_logic ());
            if (size > capacity_) {
                const size_type capacity = size << 1;
                BOOST_UBLAS_CHECK (capacity, internal_logic ());
                pointer data = alloc_.allocate (capacity);
                std::uninitialized_copy (data_, data_ + (std::min) (size, size_), data);
                std::uninitialized_fill (data + (std::min) (size, size_), data + capacity, value_type ());

                if (capacity_) {
                    std::for_each (data_, data_ + capacity_, static_destroy);
                    alloc_.deallocate (data_, capacity_);
                }
                capacity_ = capacity;
                data_ = data;
            }
            size_ = size;
            BOOST_UBLAS_CHECK (size_ <= capacity_, internal_logic ());
        }
    public:

        // Reserving
        BOOST_UBLAS_INLINE
        void reserve (size_type capacity) {
            BOOST_UBLAS_CHECK (size_ <= capacity_, internal_logic ());
            // Reduce capacity_ if size_ allows
            BOOST_UBLAS_CHECK (capacity >= size_, bad_size ());
            pointer data;
            if (capacity) {
                data = alloc_.allocate (capacity);
                std::uninitialized_copy (data_, data_ + size_, data);
                std::uninitialized_fill (data + size_, data + capacity, value_type ());
            }
            else
                data = 0;
                
            if (capacity_) {
                std::for_each (data_, data_ + capacity_, static_destroy);
                alloc_.deallocate (data_, capacity_);
            }
            capacity_ = capacity;
            data_ = data;
            BOOST_UBLAS_CHECK (size_ <= capacity_, internal_logic ());
        }

        BOOST_UBLAS_INLINE
        size_type size () const {
            return size_;
        }
        BOOST_UBLAS_INLINE
        size_type capacity () const {
            return capacity_;
        }

        // Element access
        BOOST_UBLAS_INLINE
        data_reference operator [] (key_type i) {
#ifndef BOOST_UBLAS_STRICT_MAP_ARRAY
            pointer it = find (i);
            if (it == end ())
                it = insert (end (), value_type (i, mapped_type (0)));
            BOOST_UBLAS_CHECK (it != end (), internal_logic ());
            return it->second;
#else
            return data_reference (*this, i);
#endif
        }

        // Assignment
        BOOST_UBLAS_INLINE
        map_array &operator = (const map_array &a) {
            if (this != &a) {
                resize (a.size_);
                std::copy (a.data_, a.data_ + a.size_, data_);
            }
            return *this;
        }
        BOOST_UBLAS_INLINE
        map_array &assign_temporary (map_array &a) {
            swap (a);
            return *this;
        }

        // Swapping
        BOOST_UBLAS_INLINE
        void swap (map_array &a) {
            if (this != &a) {
                std::swap (capacity_, a.capacity_);
                std::swap (data_, a.data_);
                std::swap (size_, a.size_);
            }
        }
        BOOST_UBLAS_INLINE
        friend void swap (map_array &a1, map_array &a2) {
            a1.swap (a2);
        }

        // Element insertion and deletion
        // BOOST_UBLAS_INLINE This function seems to be big. So we do not let the compiler inline it.    
        pointer push_back (pointer it, const value_type &p) {
            if (size () == 0 || (it = end () - 1)->first < p.first) {
                resize (size () + 1);
                *(it = end () - 1) = p;
                return it;
            }
            external_logic ().raise ();
            return it;
        }
        // BOOST_UBLAS_INLINE This function seems to be big. So we do not let the compiler inline it.    
        pointer insert (pointer it, const value_type &p) {
            it = detail::lower_bound (begin (), end (), p, detail::less_pair<value_type> ());
            difference_type n = it - begin ();
            BOOST_UBLAS_CHECK (size () == 0 || size () == size_type (n) || it->first != p.first, external_logic ());
            resize (size () + 1);
            it = begin () + n;
            std::copy_backward (it, end () - 1, end ());
            *it = p;
            return it;
        }
        // BOOST_UBLAS_INLINE This function seems to be big. So we do not let the compiler inline it.    
        void erase (pointer it) {
            BOOST_UBLAS_CHECK (begin () <= it && it < end (), bad_index ());
            // Fixed by George Katsirelos.
            // (*it).second = mapped_type (0);
            std::copy (it + 1, end (), it);
            resize (size () - 1);
        }
        // BOOST_UBLAS_INLINE This function seems to be big. So we do not let the compiler inline it.    
        void erase (pointer it1, pointer it2) {
            BOOST_UBLAS_CHECK (begin () <= it1 && it1 < it2 && it2 <= end (), bad_index ());
            // Fixed by George Katsirelos.
            // while (it1 != it2) {
            //     BOOST_UBLAS_CHECK (begin () <= it1 && it1 < end (), bad_index ());
            //     (*it1).second = mapped_type (0);
            //     ++ it1;
            // }
            std::copy (it2, end (), it1);
            resize (size () - (it2 - it1));
        }
        // BOOST_UBLAS_INLINE This function seems to be big. So we do not let the compiler inline it.    
        void clear () {
            resize (0);
        }

        // Element lookup
        // BOOST_UBLAS_INLINE This function seems to be big. So we do not let the compiler inline it.    
        const_pointer find (key_type i) const {
            const_pointer it (detail::lower_bound (begin (), end (), value_type (i, mapped_type (0)), detail::less_pair<value_type> ()));
            if (it == end () || it->first != i)
                it = end ();
            return it;
        }
        // BOOST_UBLAS_INLINE This function seems to be big. So we do not let the compiler inline it.    
        pointer find (key_type i) {
            pointer it (detail::lower_bound (begin (), end (), value_type (i, mapped_type (0)), detail::less_pair<value_type> ()));
            if (it == end () || it->first != i)
                it = end ();
            return it;
        }
        // BOOST_UBLAS_INLINE This function seems to be big. So we do not let the compiler inline it.    
        const_pointer lower_bound (key_type i) const {
            return detail::lower_bound (begin (), end (), value_type (i, mapped_type (0)), detail::less_pair<value_type> ());
        }
        // BOOST_UBLAS_INLINE This function seems to be big. So we do not let the compiler inline it.    
        pointer lower_bound (key_type i) {
            return detail::lower_bound (begin (), end (), value_type (i, mapped_type (0)), detail::less_pair<value_type> ());
        }

        // Iterators simply are pointers.

        typedef const_pointer const_iterator;

        BOOST_UBLAS_INLINE
        const_iterator begin () const {
            return data_;
        }
        BOOST_UBLAS_INLINE
        const_iterator end () const {
            return data_ + size_;
        }

        typedef pointer iterator;

        BOOST_UBLAS_INLINE
        iterator begin () {
            return data_;
        }
        BOOST_UBLAS_INLINE
        iterator end () {
            return data_ + size_;
        }

        // Reverse iterators
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;

        BOOST_UBLAS_INLINE
        const_reverse_iterator rbegin () const {
            return const_reverse_iterator (end ());
        }
        BOOST_UBLAS_INLINE
        const_reverse_iterator rend () const {
            return const_reverse_iterator (begin ());
        }
        BOOST_UBLAS_INLINE
        reverse_iterator rbegin () {
            return reverse_iterator (end ());
        }
        BOOST_UBLAS_INLINE
        reverse_iterator rend () {
            return reverse_iterator (begin ());
        }

        // Allocator
        allocator_type get_allocator () {
            return alloc_;
        }

    private:
        // Provide destroy as a non member function
        BOOST_UBLAS_INLINE
        static void static_destroy (reference p) {
            (&p) -> ~value_type ();
        }
        ALLOC alloc_;
        size_type capacity_;
        pointer data_;
        size_type size_;
    };


    namespace detail {
        template<class A, class T>
        struct map_traits {
            typedef typename A::mapped_type &reference;
        };
        template<class I, class T, class ALLOC>
        struct map_traits<map_array<I, T, ALLOC>, T > {
            typedef typename map_array<I, T, ALLOC>::data_reference reference;
        };

        // reserve helpers for map_array and generic maps
        // ISSUE should be in map_traits but want to use on all compilers

        template<class M>
        BOOST_UBLAS_INLINE
        void map_reserve (M &/* m */, typename M::size_type /* capacity */) {
        }
        template<class I, class T, class ALLOC>
        BOOST_UBLAS_INLINE
        void map_reserve (map_array<I, T, ALLOC> &m, typename map_array<I, T, ALLOC>::size_type capacity) {
            m.reserve (capacity);
        }

        template<class M>
        struct map_capacity_traits {
            typedef typename M::size_type type ;
            type operator() ( M const& m ) const {
               return m.size ();
            }
        } ;

        template<class I, class T, class ALLOC>
        struct map_capacity_traits< map_array<I, T, ALLOC> > {
            typedef typename map_array<I, T, ALLOC>::size_type type ;
            type operator() ( map_array<I, T, ALLOC> const& m ) const {
               return m.capacity ();
            }
        } ;

        template<class M>
        BOOST_UBLAS_INLINE
        typename map_capacity_traits<M>::type map_capacity (M const& m) {
            return map_capacity_traits<M>() ( m );
        }
    }

}}}

#endif

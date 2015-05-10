#ifndef _STL_SMAP_H
#define _STL_SMAP_H 1

#include <bits/functexcept.h>
#include <bits/concept_check.h>
#include <bits/stl_skiplist.h>
#include <initializer_list>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_CONTAINER

  template <typename _Key, typename _Tp, typename _Compare = std::less<_Key>,
            typename _Alloc = std::allocator<std::pair<const _Key, _Tp> > >
    class smap
    {
      public:
		typedef _Key                                          key_type;
      	typedef _Tp                                           mapped_type;
      	typedef std::pair<const _Key, _Tp>                    value_type;
      	typedef _Compare                                      key_compare;
      	typedef _Alloc                                        allocator_type;

    private:
      typedef typename _Alloc::value_type                   _Alloc_value_type;

      __glibcxx_class_requires(_Tp, _SGIAssignableConcept)
      __glibcxx_class_requires4(_Compare, bool, _Key, _Key, _BinaryFunctionConcept)
      __glibcxx_class_requires2(value_type, _Alloc_value_type, _SameTypeConcept)

	public:
	  class value_compare : public std::binary_function<value_type, value_type, bool>
      {
      	  friend class smap<_Key, _Tp, _Compare, _Alloc>;
        protected:
          _Compare comp;

          value_compare(_Compare __c) : comp(__c)
		  {
		  }

        public:
          bool operator()(const value_type& __x, const value_type& __y) const
          {
              return comp(__x.first, __y.first);
		  }
      };

    private:
      	  typedef typename _Alloc::template rebind<value_type>::other _Pair_alloc_type;
      	  typedef _SkipList<key_type, value_type, _Select1st<value_type>,
		       key_compare, _Pair_alloc_type> _Rep_type;

      _Rep_type _M_t;

    public:
      typedef typename _Pair_alloc_type::pointer         pointer;
      typedef typename _Pair_alloc_type::const_pointer   const_pointer;
      typedef typename _Pair_alloc_type::reference       reference;
      typedef typename _Pair_alloc_type::const_reference const_reference;
      typedef typename _Rep_type::iterator               iterator;
      typedef typename _Rep_type::const_iterator         const_iterator;
      typedef typename _Rep_type::size_type              size_type;
      typedef typename _Rep_type::difference_type        difference_type;
      typedef typename _Rep_type::reverse_iterator       reverse_iterator;
      typedef typename _Rep_type::const_reverse_iterator const_reverse_iterator;

      smap() : _M_t()
      {
      }

     explicit smap(const _Compare& __comp, const allocator_type& __a = allocator_type())
      : _M_t(__comp, __a)
      {
      }

      smap(const smap& __x): _M_t(__x._M_t)
	  {
	  }

      template<typename _InputIterator>
        smap(_InputIterator __first, _InputIterator __last) : _M_t()
        {
            _M_t._M_insert_unique(__first, __last);
		}

	  template<typename _InputIterator>
        smap(_InputIterator __first, _InputIterator __last,
	    const _Compare& __comp, const allocator_type& __a = allocator_type())
        : _M_t(__comp, __a)
        {
        	_M_t._M_insert_unique(__first, __last);
		}

      smap& operator=(const smap& __x)
      {
          _M_t = __x._M_t;
          return *this;
      }

      allocator_type get_allocator() const
      {
          return _M_t._get_allocator();
	  }

      iterator begin()
      {
          return _M_t._begin();
	  }

      const_iterator begin() const
      {
          return _M_t._begin();
	  }

      iterator end()
      {
          return _M_t._end();
	  }

      const_iterator end() const
      {
          return _M_t._end();
	  }

      reverse_iterator rbegin()
      {
          return _M_t._rbegin();
	  }

      const_reverse_iterator rbegin() const
      {
          return _M_t._rbegin();
	  }

      reverse_iterator rend()
      {
          return _M_t._rend();
	  }

      const_reverse_iterator rend() const
      {
          return _M_t._rend();
	  }

	  bool empty() const
      {
          return _M_t._empty();
	  }

      size_type size() const
      {
          return _M_t._size();
	  }

	  size_type max_size() const
      {
          return _M_t.max_size();
	  }

      mapped_type& operator[](const key_type& _k)
      {
	      iterator _i = lower_bound(_k);
	      if (_i == end() || key_comp()(_k, (*_i).first))
              _i = insert(_i, value_type(_k, mapped_type()));
          return (*_i).second;
      }

      mapped_type& at(const key_type& _k)
      {
          iterator _i = lower_bound(_k);
          if (_i == end() || key_comp()(_k, (*_i).first))
              __throw_out_of_range(__N("smap::at"));
          return (*_i).second;
      }

      template<typename _InputIterator>
      void insert(_InputIterator _first, _InputIterator _last)
      {
          _M_t._insert_unique(_first, _last);
      }

      iterator insert(iterator _itr, const value_type& _x)
      {
          if((_itr == end()) || (begin() == end()))
              return insert(_x).first;
          return _M_t._insert_unique(_itr, _x);
      }

      pair<iterator, bool> insert(const value_type& _x)
      {
          return _M_t._insert_unique(_x);
      }

      void erase(iterator _itr)
      {
          if(_itr != end())
            _M_t._erase(_itr);
      }

      size_type erase(const key_type& _x)
      {
          return _M_t._erase(_x);
      }

      void erase(iterator _first, iterator _last)
      {
          while(_first != _last)
          {
               iterator _tmp = _first;
               ++_first;
               erase(_tmp);
          }
      }

      void swap(smap& _x)
      {
          _M_t._swap(_x._M_t);
      }

      void clear()
      {
          _M_t._clear();
      }

      key_compare key_comp() const
      {
          return _M_t._key_comp();
      }

      value_compare value_comp() const
      {
          return value_compare(_M_t._key_comp());
      }

      iterator find(const key_type& _x)
      {
          return _M_t._find(_x);
      }

      const_iterator find(const key_type& _x) const
      {
          return _M_t._find(_x);
      }

      size_type count(const key_type& __x) const
      {
          return _M_t.find(__x) == _M_t.end() ? 0 : 1;
      }

      iterator lower_bound(const key_type& _x)
      {
          return _M_t._lower_bound(_x);
      }

      const_iterator lower_bound(const key_type& _x) const
      {
          return _M_t.lower_bound(_x);
      }

      iterator upper_bound(const key_type& _x)
      {
          return _M_t._upper_bound(_x);
      }

      const_iterator upper_bound(const key_type& _x) const
      {
          return _M_t._upper_bound(_x);
      }

      std::pair<iterator, iterator>
      equal_range(const key_type& __x)
      {
          return _M_t.equal_range(__x);
      }

      std::pair<const_iterator, const_iterator>
      equal_range(const key_type& __x) const
      {
          return _M_t.equal_range(__x);
      }

      template<typename _K1, typename _T1, typename _C1, typename _A1>
        friend bool operator==(const smap<_K1, _T1, _C1, _A1>&,
		   const smap<_K1, _T1, _C1, _A1>&);

      template<typename _K1, typename _T1, typename _C1, typename _A1>
        friend bool operator<(const smap<_K1, _T1, _C1, _A1>&,
		  const smap<_K1, _T1, _C1, _A1>&);
    };


   template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
    inline bool operator==(const smap<_Key, _Tp, _Compare, _Alloc>& __x,
        const smap<_Key, _Tp, _Compare, _Alloc>& __y)
    {
        return __x._M_t == __y._M_t;
    }

  template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
    inline bool operator<(const smap<_Key, _Tp, _Compare, _Alloc>& __x,
        const smap<_Key, _Tp, _Compare, _Alloc>& __y)
    {
        return __x._M_t < __y._M_t;
    }

  template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
    inline bool operator!=(const smap<_Key, _Tp, _Compare, _Alloc>& __x,
        const smap<_Key, _Tp, _Compare, _Alloc>& __y)
    {
        return !(__x == __y);
    }

  template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
    inline bool operator>(const smap<_Key, _Tp, _Compare, _Alloc>& __x,
        const smap<_Key, _Tp, _Compare, _Alloc>& __y)
    {
        return __y < __x;
    }

  template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
    inline bool operator<=(const smap<_Key, _Tp, _Compare, _Alloc>& __x,
        const smap<_Key, _Tp, _Compare, _Alloc>& __y)
    {
        return !(__y < __x);
    }

  template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
    inline bool operator>=(const smap<_Key, _Tp, _Compare, _Alloc>& __x,
        const smap<_Key, _Tp, _Compare, _Alloc>& __y)
    {
        return !(__x < __y);
    }

  template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
    inline void swap(smap<_Key, _Tp, _Compare, _Alloc>& __x,
	 smap<_Key, _Tp, _Compare, _Alloc>& __y)
    {
        __x.swap(__y);
    }


_GLIBCXX_END_NAMESPACE_CONTAINER
} // namespace std

#endif

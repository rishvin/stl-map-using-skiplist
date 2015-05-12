#ifndef _STL_SKIPLIST_H
#define _STL_SKIPLIST_H 1

#include <bits/stl_algobase.h>
#include <bits/allocator.h>
#include <bits/stl_function.h>
#include <bits/cpp_type_traits.h>
#include <math.h>
#include <limits>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  #define _RT(_P, I)        _P->_right(I)
  #define _LT(_P, I)        _P->_left(I)
  #define _AT(_P, _I)       _P[_I]
  #define _INVALID      (numeric_limits<size_t>::max())

  typedef size_t size_type;


  //
  // Basic classes to create skiplist node
  //
  template<typename _Tp>
  struct _Slots
  {
      void _set_size(size_type _size)
      {
          __try
          {
              _mp_chunk = static_cast<_Tp*>(operator new(sizeof(_Tp) * _size));
              _m_size = _size;
          }
          __catch(...)
          {
          }
      }

      void _clear()
      {
          operator delete(_mp_chunk);
      }

      size_type _size()
      {
          return _m_size;
      }

      _Tp& operator[](size_type _i) const
      {
          return _AT(_mp_chunk, _i);
      }

    private:
      _Tp *_mp_chunk;
      size_type _m_size;
  };


  struct _SkipList_node_base
  {
      struct _DLL_node
      {
          _SkipList_node_base *_mp_left;
          _SkipList_node_base *_mp_right;
      };

      void _set_size(size_type _size)
      {
           _m_slot._set_size(_size);
      }

      void _clear()
      {
          _m_slot._clear();
      }

      size_type _size()
      {
          return _m_slot._size();
      }

      _SkipList_node_base* & _right(size_type _i) const
      {
          return _AT(_m_slot, _i)._mp_right;
      }

      _SkipList_node_base* & _left(size_type _i) const
      {
          return _AT(_m_slot, _i)._mp_left;
      }

    private:
      _Slots<_DLL_node> _m_slot;
  };


  template<typename _Tp>
  struct _SkipList_node: public _SkipList_node_base
  {
      _Tp _m_value;
  };


  typedef _SkipList_node_base               _Base_node;
  typedef _SkipList_node_base*              _Base_ptr;
  typedef const _SkipList_node_base*        _Const_Base_ptr;
  typedef _Slots<_SkipList_node_base*>      _Ptr_slot;




  //
  // List of helper functions
  //
  void _h_set_size(_Base_ptr _p, size_type _s)
  {
      _p->_set_size(_s);
  }

  void _h_set_size(_Ptr_slot &_p, size_type _s)
  {
      _p._set_size(_s);
  }

  void _h_clear(_Base_ptr _p)
  {
      _p->_clear();
  }

  void _h_clear(_Ptr_slot &_p)
  {
      _p._clear();
  }

  size_type _h_size(_Base_ptr _p)
  {
      return _p->_size();
  }

  size_type _size(_Ptr_slot &_p)
  {
      return _p._size();
  }

  void _h_dll_link(_Base_ptr _p1, _Base_ptr _p2, size_type _i)
  {
      _RT(_p1, _i) = _p2;
      _LT(_p2, _i) = _p1;
  }

  void _h_add_link(_Base_ptr _p1, _Base_ptr _p2, size_type _i)
  {
      _h_dll_link(_p2, _RT(_p1, _i), _i);
      _h_dll_link(_p1, _p2, _i);
  }

  void _h_del_link(_Base_ptr _p, size_t _i)
  {
      _h_dll_link(_LT(_p, _i), _RT(_p, _i), _i);
  }

  void _h_copy_link(_Base_ptr _p1, _Base_ptr _p2, size_type _i)
  {
      _RT(_p1, _i) = _RT(_p2, _i);
      _LT(_p1, _i) = _LT(_p2, _i);
  }

  void _h_swap_link(_Base_ptr _p1, _Base_ptr _p2, size_type _i)
  {
      std::swap(_RT(_p1, _i), _RT(_p2, _i));
      std::swap(_LT(_p1, _i), _LT(_p2, _i));
  }

  void _h_assign_inter_link(_Base_ptr _p1, _Base_ptr _p2, size_type _i)
  {
      _LT(_RT(_p1, _i), _i) =  _p2;
      _RT(_LT(_p1, _i), _i) =  _p2;
  }

  void _h_swap_inter_link(_Base_ptr _p1, _Base_ptr _p2, size_type _i)
  {
      std::swap(_LT(_RT(_p1, _i), _i), _LT(_RT(_p2, _i), _i));
      std::swap(_RT(_LT(_p1, _i), _i), _RT(_LT(_p2, _i), _i));
  }

  void _h_range_add_link(_Ptr_slot& _p_frm,
                         _Base_ptr _p_to,
                         size_type _frm,
                         size_type _to)
  {
      for(; _frm != _to; ++_frm)
          _h_add_link(_AT(_p_frm, _frm), _p_to, _frm);
  }

  void _h_range_add_link(_Base_ptr _p_frm,
                         _Base_ptr _p_to,
                         size_type _frm,
                         size_type _to)
  {
      for(; _frm != _to; ++_frm)
          _h_add_link(_p_frm, _p_to, _frm);
  }

  void _h_range_dll_link(_Base_ptr _p_frm,
                          _Base_ptr _p_to,
                          size_type _frm,
                          size_type _to)
  {
      for(; _frm != _to; ++_frm)
          _h_dll_link(_p_frm, _p_to, _frm);
  }

  void _h_range_del_link(_Base_ptr _p,
                         size_type _frm,
                         size_type _to)
  {
      for(; _frm != _to; ++_frm)
          _h_del_link(_p, _frm);
  }




  //
  // STL implementations starts from here
  //
  template<typename _Tp>
  struct _SkipList_iterator
  {
      typedef _Tp                           value_type;
      typedef _Tp&                          reference;
      typedef _Tp*                          pointer;

      typedef bidirectional_iterator_tag    iterator_category;
      typedef ptrdiff_t                     difference_type;

      typedef _SkipList_iterator<_Tp>       _Self;
      typedef _SkipList_node<_Tp>*          _Link_ptr;


      _SkipList_iterator() : _m_node()
      {
      }

      explicit _SkipList_iterator(_Base_ptr __x): _m_node(__x)
      {
      }

      reference operator*() const
      {
          return static_cast<_Link_ptr>(_m_node)->_m_value;
      }

      pointer operator->() const
      {
          return std::__addressof(static_cast<_Link_ptr>(_m_node)->_m_value);
      }

      _Self& operator++()
      {
          _m_node = _RT(_m_node, 0);
          return *this;
      }

      _Self operator++(int)
      {
          _Self __tmp = *this;
          _m_node = _RT(_m_node, 0);
          return __tmp;
      }

      _Self& operator--()
      {
          _m_node = _LT(_m_node, 0);
          return *this;
      }

      _Self operator--(int)
      {
          _Self __tmp = *this;
          _m_node = _LT(_m_node, 0);
          return __tmp;
      }

      bool operator==(const _Self& __x) const
      {
          return _m_node == __x._m_node;
      }

      bool operator!=(const _Self& __x) const
      {
          return _m_node != __x._m_node;
      }

      _Base_ptr _m_node;
    };



  template<typename _Tp>
  struct _SkipList_const_iterator
  {
      typedef _Tp                               value_type;
      typedef const _Tp&                        reference;
      typedef const _Tp*                        pointer;

      typedef _SkipList_iterator<_Tp>           iterator;
      typedef bidirectional_iterator_tag        iterator_category;
      typedef ptrdiff_t                         difference_type;

      typedef _SkipList_const_iterator<_Tp>     _Self;
      typedef const _SkipList_node<_Tp>*        _Link_ptr;


      _SkipList_const_iterator() : _m_node()
      {
      }

      explicit _SkipList_const_iterator(_Const_Base_ptr __x) : _m_node(__x)
      {
      }

      _SkipList_const_iterator(const iterator& __it) : _m_node(__it._m_node)
      {
      }

      iterator _const_cast() const
      {
          return iterator(static_cast<typename iterator::_Link_ptr>
			(const_cast<typename iterator::_Const_Base_ptr>(_m_node)));
      }

      reference operator*() const
      {
          return static_cast<_Link_ptr>(_m_node)->_m_value;
      }

      pointer operator->() const
      {
          return std::__addressof(static_cast<_Link_ptr>
				(_m_node)->_m_value);
      }

      _Self& operator++()
      {
          _m_node = _RT(_m_node, 0);
          return *this;
      }

      _Self operator++(int)
      {
          _Self __tmp = *this;
          _m_node = _RT(_m_node, 0);
          return __tmp;
      }

      _Self& operator--()
      {
          _m_node = _LT(_m_node, 0);
          return *this;
      }

      _Self operator--(int)
      {
          _Self __tmp = *this;
          _m_node = _LT(_m_node, 0);
          return __tmp;
      }

      bool operator==(const _Self& __x) const
      {
          return _m_node == __x._m_node;
      }

      bool operator!=(const _Self& __x) const
      {
          return _m_node != __x._m_node;
      }

      _Const_Base_ptr _m_node;
    };

  template<typename _Val>
  inline bool operator==(const _SkipList_iterator<_Val>& __x,
      const _SkipList_const_iterator<_Val>& __y)
  {
      return __x._m_node == __y._m_node;
  }

  template<typename _Val>
  inline bool operator!=(const _SkipList_iterator<_Val>& __x,
      const _SkipList_const_iterator<_Val>& __y)
  {
      return __x._m_node != __y._m_node;
  }



  template<typename _Key, typename _Val, typename _KeyOfValue,
           typename _Compare, typename _Alloc = allocator<_Val> >
    class _SkipList
    {

      public:
        typedef _Key                                    key_type;
        typedef _Key*                                   key_type_ptr;
        typedef _Val                                    value_type;
        typedef value_type*                             pointer;
        typedef const value_type*                       const_pointer;
        typedef value_type&                             reference;
        typedef const value_type&                       const_reference;
        typedef _SkipList_node<_Val>*                   _Link_ptr;
        typedef const _SkipList_node<_Val>*             _Const_Link_ptr;
        typedef size_t size_type;
        typedef ptrdiff_t                               difference_type;
        typedef _Alloc                                  allocator_type;

        typedef _SkipList_iterator<value_type>          iterator;
        typedef _SkipList_const_iterator<value_type>    const_iterator;
        typedef std::reverse_iterator<iterator>         reverse_iterator;
        typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;

        _SkipList()
        {
            _init();
        }

        _SkipList(const _Compare& __comp, const allocator_type& __a = allocator_type())
        : _m_impl(__comp, __a)
        {
            _init();
        }

        _SkipList(const _SkipList& _x)
        : _m_impl(_x._m_impl._m_key_compare, _x._get_Node_allocator())
        {
            _init();
            _copy(_x._begin(), _x._end());
        }

        ~_SkipList()
        {
          _clear();
          _h_clear(_root());
        }

        _SkipList& operator=(const _SkipList& _x)
        {
            _clear();
            _copy(_x._begin(), _x._end());

            return *this;
        }

        iterator _begin()
        {
            return iterator(_RT(_root(), 0));
        }

        const_iterator _begin() const
        {
            return const_iterator(_RT(_root(), 0));
        }

        iterator _end()
        {
            return iterator(_root());
        }

        const_iterator _end() const
        {
            return const_iterator(_root());
        }

        reverse_iterator _rbegin()
        {
            return reverse_iterator(_end());
        }

        const_reverse_iterator _rbegin() const
        {
            return const_reverse_iterator(_end());
        }

        reverse_iterator _rend()
        {
            return reverse_iterator(_begin());
        }

        const_reverse_iterator _rend() const
        {
            return const_reverse_iterator(_begin());
        }

        bool _empty() const
        {
            return _m_size == 0;
        }

        size_type _size() const
        {
            return _m_size;
        }

        void _clear()
        {
            _Base_ptr _p;

            while((_p = _RT(_root(), 0)) != _root())
            {
                _h_del_link(_p, 0);
                _destroy_node_and_dcr_size(_to_link_ptr(_p));
            }
            _h_range_dll_link(_root(), _root(), 0, _max_level);
        }

      protected:
        typedef typename _Alloc::template rebind<_SkipList_node<_Val> >::other  _Node_allocator;


        template<typename _Key_compare, bool _Is_pod_comparator = __is_pod(_Key_compare)>
         struct _SkipList_impl : public _Node_allocator
         {
             _Key_compare		_m_key_compare;
             _SkipList_node_base 	_m_list;

             _SkipList_impl()
                 : _Node_allocator(), _m_key_compare(), _m_list()
             {
             }

             _SkipList_impl(const _Key_compare& __comp, const _Node_allocator& __a)
                 : _Node_allocator(__a), _m_key_compare(__comp), _m_list()
             {
             }

        };

        //
        // static methods
        //

        static const _Val& _value(_Link_ptr _p)
        {
            return _p->_m_value;
        }

        static const _Val& _value(_Const_Link_ptr _p)
        {
            return _p->_m_value;
        }

        static _Link_ptr _to_link_ptr(_Base_ptr _p)
        {
            return static_cast<_Link_ptr>(_p);
        }

        static bool _has_key(const _Val& _v, const _Key& _k)
        {
            return _KeyOfValue()(_v) == _k;
        }

        static bool _has_key(_Const_Link_ptr _p, const _Key& _k)
        {
            return _has_key(_value(_p), _k);
        }

        static _Base_ptr _to_node(iterator _itr)
        {
            return _itr._m_node;
        }
        //
        //
        //

        _Base_ptr _root()
        {
            return std::__addressof(_head);
        }

        _Const_Base_ptr _root() const
        {
            return std::__addressof(_head);
        }

        void _init()
        {
            _h_set_size(_root(), _max_level);
            _h_range_dll_link(_root(), _root(), 0, _max_level);
            _m_size = 0;
            srand(time(NULL));
        }

        _Node_allocator& _get_Node_allocator()
        {
            return *static_cast<_Node_allocator*>(std::__addressof(this->_m_impl));
        }

        const _Node_allocator& _get_Node_allocator() const
        {
            return *static_cast<const _Node_allocator*>(std::__addressof(this->_m_impl));
        }

        allocator_type _get_allocator() const
        {
            return allocator_type(_get_Node_allocator());
        }

        _Link_ptr _get_node()
        {
            return _m_impl._Node_allocator::allocate(1);
        }

        void _put_node(_Link_ptr _p)
        {
            _m_impl._Node_allocator::deallocate(_p, 1);
        }

        _Link_ptr _create_node_and_inc_size(const _Val& _v, size_type _size)
        {
            _Link_ptr _tmp = _get_node();
            __try
            {
                _get_allocator().construct(std::__addressof(_tmp->_m_value), _v);
                _h_set_size(_tmp, _size);
                _m_size++;

            }
            __catch(...)
            {
                _put_node(_tmp);
                __throw_exception_again;
            }
            return _tmp;
        }

        void _destroy_node_and_dcr_size(_Link_ptr _p)
        {
            _get_allocator().destroy(std::__addressof(_p->_m_value));
            _h_clear(_p);
            _put_node(_p);
            _m_size--;
        }

        bool _sentinal(_Const_Base_ptr _p)
        {
            return _p == _root();
        }

        size_type _flip_coin()
        {
            size_type _h   =    1;
            size_type _rnd =    rand();

            while(_rnd && (_rnd & 1))
            {
                _h++;
                _rnd >>= 1;
            }
            return (_h > _max_level ? _max_level : _h);
        }

        _Base_ptr _low_cmp(_Base_ptr _p, size_type _i, const key_type& _k)
        {
            _Base_ptr _p_tmp;
             while((!_sentinal(_p_tmp = _LT(_p, _i))) &&
                 !_key_comp()(_KeyOfValue()(_value(_to_link_ptr(_p_tmp))), _k))
                _p = _p_tmp;
            return _p;
        }

        _Base_ptr _up_cmp(_Base_ptr _p, size_type _i, const key_type& _k)
         {
            _Base_ptr _p_tmp;
             while(!_sentinal(_p_tmp = _LT(_p, _i)) &&
                 _key_comp()(_k, _KeyOfValue()(_value(_to_link_ptr(_p_tmp)))))
                 _p = _p_tmp;
            return _p;
        }

         size_type _test_and_trc_path(_Base_ptr _p, size_type _i,
            const key_type& _k, _Slots<_Base_ptr>& _slot)
        {
            for(; _i != _INVALID; --_i)
            {
                _p = _low_cmp(_p, _i, _k);
                if(!_sentinal(_p) && _has_key(_to_link_ptr(_p), _k))
                {
                     _AT(_slot, _i) = _p;
                     return _i;
                }
                _AT(_slot, _i) = _LT(_p, _i);
            }
            return _i;
        }

        void _trc_path(_Base_ptr _p, size_type _frm, size_type _to,
            const key_type& _k, _Slots<_Base_ptr>& _slot)
        {
            for(; _frm != _to; --_frm)
            {
                _p = _low_cmp(_p, _frm, _k);
                _AT(_slot, _frm) = _LT(_p, _frm);
            }
        }

        void _copy(const_iterator _itr1, const_iterator _itr2)
        {
            for(; _itr1 != _itr2; ++_itr1)
                _insert_unique(*_itr1);
        }

        void _swap_head(_Base_ptr _p_with)
        {
            _Base_ptr _p_this = _root();

            size_type _i;

            for(_i = 0;
                (_i < _max_level) &&
                (_RT(_p_this, _i) !=  _p_this) &&
                (_RT(_p_with, _i) != _p_with);
                ++_i)
            {
                _h_swap_link(_p_this, _p_with, _i);
                _h_swap_inter_link(_p_this, _p_with, _i);
            }

            if(_i < _max_level)
            {
                if(_RT(_p_with, _i) == _p_with)
                {
                    std::swap(_p_this, _p_with);
                }

                for(; (_i < _max_level) && (_RT(_p_this, _i) != _p_this); ++_i)
                {
                    _h_copy_link(_p_this, _p_with, _i);
                    _h_dll_link(_p_this, _p_this, _i);
                    _h_assign_inter_link(_p_this, _p_this, _i);
                }
            }
        }

      public:
        void _insert_unique(iterator _itr1, iterator _itr2)
        {
            _copy(_itr1, _itr2);
        }

        pair<iterator, bool> _insert_unique(const _Val& _v)
        {
            _Base_ptr _first;
            bool _second;

            _Ptr_slot _slot;
            _h_set_size(_slot, _h_size(_root()));


            size_type _i = _test_and_trc_path(_root(),
                                              _h_size(_root()) - 1,
                                              _KeyOfValue()(_v),
                                              _slot);
            if(_i != _INVALID)
            {
                _first = _AT(_slot, _i);
                _second = false;
            }
            else
            {
                _Link_ptr _p_new = _create_node_and_inc_size(_v, _flip_coin());
                _h_range_add_link(_slot, _p_new, 0, _h_size(_p_new));

                _first = _p_new;
                _second = true;
            }

            _h_clear(_slot);
            return make_pair(iterator(_first), _second);
        }

        iterator _insert_unique(iterator _itr, const _Val& _v)
        {
            const key_type &_k1 = _KeyOfValue()(*_itr);
            const key_type &_k2 = _KeyOfValue()(_v);

            if(_k1 == _k2)
                return _itr;

            _Base_ptr _ret;

            _Base_ptr _p = _to_node(_itr);
            _Ptr_slot _slot;
            _h_set_size(_slot, _h_size(_root()));

            size_type _i;
            size_type _node_size;

            _node_size = _h_size(_p);

            if(_k1 > _k2)
            {
                _i = _test_and_trc_path(_p,
                    _node_size - 1, _k2, _slot);
            }
            else
            {
                _i = _test_and_trc_path(_root(), _node_size - 1,
                    _k2, _slot);
            }

            if(_i != _INVALID)
                _ret = _AT(_slot, _i);
            else
            {
                _Link_ptr _p_new = _create_node_and_inc_size(_v, _flip_coin());

                if(_node_size > _h_size(_p_new))
                    _node_size = _h_size(_p_new);

                _h_range_add_link(_slot, _p_new, 0, _node_size);

                if(_h_size(_p_new) > _node_size)
                {
                    _trc_path(_root(), _h_size(_p_new) - 1, _node_size - 1, _k2, _slot);
                    _h_range_add_link(_slot, _p_new, _node_size, _h_size(_p_new));
                }

                _ret = _p_new;
            }

            _h_clear(_slot);
            return iterator(_ret);
        }

        size_type _erase(const key_type& _x)
        {
            iterator _itr = _find(_x);
            if(_itr == _end())
                return 0;

            _Link_ptr _p = _to_link_ptr(_to_node(_itr));
            _h_range_del_link(_p, 0, _h_size(_p));
            _destroy_node_and_dcr_size(_p);
            return 1;
        }

        void _erase(iterator _itr)
        {
            _Link_ptr _p = _to_link_ptr(_to_node(_itr));
            _h_range_del_link(_p, 0, _h_size(_p));
            _destroy_node_and_dcr_size(_p);
        }

        iterator _find(const key_type& _k)
        {
            iterator _itr = _lower_bound(_k);
            if(_itr != _end() && _has_key(*_itr, _k))
                return _itr;
            return _end();
        }

        iterator _lower_bound(const key_type& _k)
        {
           _Base_ptr _p = _root();
            for(size_type _i = _h_size(_p) - 1; _i != _INVALID; --_i)
                _p = _low_cmp(_p, _i, _k);
            return iterator(_p);
        }

        iterator _upper_bound(const key_type& _k)
        {
            _Base_ptr _p = _root();
            for(size_type _i = _h_size(_p) - 1; _i != _INVALID; --_i)
                _p = _up_cmp(_p, _i, _k);
            return iterator(_p);
        }

        _Compare _key_comp() const
        {
            return _m_impl._m_key_compare;
        }

        void _swap(_SkipList &_s)
        {
           _swap_head(std::__addressof(_s._head));
           std::swap(_m_size, _s._m_size);
           std::swap(_m_impl._m_key_compare, _s._m_impl._m_key_compare);
           std::__alloc_swap<_Node_allocator>::
               _S_do_it(_get_Node_allocator(), _s._get_Node_allocator());
        }

      private:
        _Base_node _head;
        _SkipList_impl<_Compare> _m_impl;
        size_type _m_size;
        enum { _max_level = 24 };
    };


_GLIBCXX_END_NAMESPACE_VERSION
}

#endif

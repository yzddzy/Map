/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP
// only for std::less<T>
#include <functional>
#include <cstddef>

#include "utility.hpp"
#include "exceptions.hpp"
#include <iostream>

namespace sjtu
{
	enum colorT
	{
		RED,
		BLACK
	};

	template <
		class Key,
		class T,
		class Compare = std::less<Key>>
	class map
	{
	public:
		/**
		 * the internal type of data.
		 * it should have a default constructor, a copy constructor.
		 * You can use sjtu::map as value_type by typedef.
		 */
		typedef pair<const Key, T> value_type;

	private:
		struct RBnode
		{
			value_type *data;
			RBnode *left, *right, *father, *next, *prev;
			colorT color;
			RBnode() { data = NULL, left = NULL, right = NULL, father = NULL, prev = NULL, next = NULL, color = BLACK; }
			RBnode(const value_type &element, RBnode *_left = NULL, RBnode *_right = NULL, RBnode *_father = NULL, RBnode *_prev = NULL, RBnode *_next = NULL, colorT _color = RED)
			{
				left = _left, right = _right, father = _father, prev = _prev, next = _next, color = _color;
				data = new value_type(element);
			}
			~RBnode()
			{
				if (data)
					delete data;
			}
		};
		RBnode *root;
		RBnode*head, *tail;
		RBnode *nulptr;
		size_t currentSize;
		Compare cmp;

	public:
		/**
		 * see BidirectionalIterator at CppReference for help.
		 *
		 * if there is anything wrong throw invalid_iterator.
		 *     like it = map.begin(); --it;
		 *       or it = map.end(); ++end();
		 */
		class const_iterator;
		class iterator
		{
			friend class map;

		private:
			/**
			 * TODO add data members
			 *   just add whatever you want.
			 */
			map *its;
			RBnode *cur_p;

		public:
			iterator(map *_its = NULL, RBnode *_cur_p = NULL) : its(_its), cur_p(_cur_p) {}
			iterator(const iterator &other) : its(other.its), cur_p(other.cur_p) {}
			iterator(const const_iterator &other) : its(other.its), cur_p(other.cur_p) {}
			/**
			 * return a new iterator which pointer n-next elements
			 *   even if there are not enough elements, just return the answer.
			 * as well as operator-
			 */
			/**
			 * TODO iter++
			 */
			iterator operator++(int)
			{
				if (cur_p == its->nulptr)
					throw invalid_iterator();
				iterator cur_q = *this;
				cur_p = cur_p->next;
				return cur_q;
			}
			/**
			 * TODO ++iter
			 */
			iterator &operator++()
			{
				if (cur_p == its->nulptr)
					throw invalid_iterator();
				cur_p = cur_p->next;
				return *this;
			}
			/**
			 * TODO iter--
			 */
			iterator operator--(int)
			{
				iterator cur_q = *this;
				if (cur_p == its->nulptr)
				{
					if (its->tail == its->nulptr)
						throw invalid_iterator();
					cur_p = its->tail;
					return cur_q;
				}
				if (cur_p->prev == its->nulptr)
					throw invalid_iterator();
				cur_p = cur_p->prev;
				return cur_q;
			}
			/**
			 * TODO --iter
			 */
			iterator &operator--()
			{
				if (cur_p == its->nulptr)
				{
					if (its->tail == its->nulptr)
						throw invalid_iterator();
					cur_p = its->tail;
					return *this;
				}
				if (cur_p->prev == its->nulptr)
					throw invalid_iterator();
				cur_p = cur_p->prev;
				return *this;
			}
			/**
			 * a operator to check whether two iterators are same (pointing to the same memory).
			 */
			value_type &operator*() const
			{
				return *(cur_p->data);
			}

			bool operator==(const iterator &rhs) const
			{
				return its == rhs.its && cur_p == rhs.cur_p;
			}
			bool operator==(const const_iterator &rhs) const
			{
				return its == rhs.its && cur_p == rhs.cur_p;
			}
			/**
			 * some other operator for iterator.
			 */
			bool operator!=(const iterator &rhs) const
			{
				return its != rhs.its || cur_p != rhs.cur_p;
			}
			bool operator!=(const const_iterator &rhs) const
			{
				return its != rhs.its || cur_p != rhs.cur_p;
			}
			/**
			 * for the support of it->first.
			 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
			 */
			value_type *operator->() const noexcept
			{
				return cur_p->data;
			}
		};
		class const_iterator
		{ // it should has similar member method as iterator.
			//  and it should be able to construct from an iterator.
			friend class map;

		private:
			// data members.
			const map *its;
			RBnode *cur_p;

		public:
			const_iterator(const map *_its = NULL, RBnode *_cur_p = NULL) : its(_its), cur_p(_cur_p) {}
			const_iterator(const iterator &other) : its(other.its), cur_p(other.cur_p) {}
			const_iterator(const const_iterator &other) : its(other.its), cur_p(other.cur_p) {}
			// And other methods in iterator.
			// And other methods in iterator.
			// And other methods in iterator.
			const_iterator operator++(int)
			{
				if (cur_p == its->nulptr)
					throw invalid_iterator();
				const_iterator cur_q = *this;
				cur_p = cur_p->next;
				return cur_q;
			}
			const_iterator &operator++()
			{
				if (cur_p == its->nulptr)
					throw invalid_iterator();
				cur_p = cur_p->next;
				return *this;
			}

			const_iterator operator--(int)
			{
				const_iterator cur_q = *this;
				if (cur_p == its->nulptr)
				{
					if (its->tail == its->nulptr)
						throw invalid_iterator();
					cur_p = its->tail;
					return cur_q;
				}
				if (cur_p->prev == its->nulptr)
					throw invalid_iterator();
				cur_p = cur_p->prev;
				return cur_q;
			}
			const_iterator &operator--()
			{
				if (cur_p == its->nulptr)
				{
					if (its->tail == its->nulptr)
						throw invalid_iterator();
					cur_p = its->tail;
					return *this;
				}
				if (cur_p->prev == its->nulptr)
					throw invalid_iterator();
				cur_p = cur_p->prev;
				return *this;
			}
			value_type &operator*() const
			{
				return *(cur_p->data);
			}

			bool operator==(const iterator &rhs) const
			{
				return its == rhs.its && cur_p == rhs.cur_p;
			}
			bool operator==(const const_iterator &rhs) const
			{
				return its == rhs.its && cur_p == rhs.cur_p;
			}
			bool operator!=(const iterator &rhs) const
			{
				return its != rhs.its || cur_p != rhs.cur_p;
			}
			bool operator!=(const const_iterator &rhs) const
			{
				return its != rhs.its || cur_p != rhs.cur_p;
			}
			value_type *operator->() const noexcept
			{
				return cur_p->data;
			}
		};
		void copy(RBnode *p, RBnode *&q, RBnode *father, RBnode *leftsFather, RBnode *rightsFather, RBnode *&head, RBnode *&tail)
		{
			if (p->data == NULL)
				return;
			q = new RBnode(*(p->data), nulptr, nulptr, father, nulptr, nulptr, p->color);
			RBnode *leftMin = nulptr, *leftMax = nulptr, *rightMin = nulptr, *rightMax = nulptr;
			copy(p->left, q->left, q, q, rightsFather, leftMin, leftMax);
			copy(p->right, q->right, q, leftsFather, q, rightMin, rightMax);
			   if (leftMax == nulptr)
            {
                q->prev = rightsFather;
            }
            else
                q->prev = leftMax;
            if (rightMin == nulptr)
            {
                q->next = leftsFather;
            }
            else
                q->next = rightMin;
            if (leftMin == nulptr)
            {
                head = q;
            }
            else
                head = leftMin;
            if (rightMax == nulptr)
            {
                tail = q;
            }
            else
                tail = rightMax;
        
			
			
		}
		void destory(RBnode *p)
		{
			if (p == nulptr)
				return;
			destory(p->left);
			destory(p->right);
			delete p;
		}
		/**
		 * TODO two constructors
		 */
		map()
		{
			currentSize = 0;
			root = head = tail = nulptr = new RBnode();
			nulptr->left = nulptr->right = nulptr->father = nulptr->prev = nulptr->next = nulptr;
		}
		/**
		 * TODO assignment operator
		 */
		map(const map &other)
		{
			currentSize = other.currentSize;
			root = head = tail = nulptr = new RBnode();
			nulptr->left = nulptr->right = nulptr->father = nulptr->prev = nulptr->next = nulptr;
			copy(other.root, root, nulptr, nulptr, nulptr, head, tail);
		}
		map &operator=(const map &other)
		{
			if (this == &other)
				return *this;
			currentSize = other.currentSize;
			if (root != nulptr)
				destory(root);
			nulptr->color = BLACK;
			nulptr->left = nulptr->right = nulptr->father = nulptr->prev = nulptr->next = nulptr;
			root = head = tail = nulptr;
			copy(other.root, root, nulptr, nulptr, nulptr, head, tail);
			return *this;
		}
		/**
		 * TODO Destructors
		 */
		~map()
		{
			if (root != nulptr)
				destory(root);
			delete nulptr;
		}

		RBnode *locate(RBnode *p, const Key &key) const
		{
			if (p == nulptr)
				return p;
			if (cmp(key, p->data->first))
				return locate(p->left, key);
			if (cmp(p->data->first, key))
				return locate(p->right, key);
			return p;
		}

		/**
		 * TODO
		 * access specified element with bounds checking
		 * Returns a reference to the mapped value of the element with key equivalent to key.
		 * If no such element exists, an exception of type `index_out_of_bound'
		 */
		T &at(const Key &key)
		{
			RBnode *p = locate(root, key);
			if (p == nulptr)
				throw index_out_of_bound();
			return p->data->second;
		}
		const T &at(const Key &key) const
		{
			RBnode *p = locate(root, key);
			if (p == nulptr)
				throw index_out_of_bound();
			return p->data->second;
		}
		/**
		 * TODO
		 * access specified element
		 * Returns a reference to the value that is mapped to a key equivalent to key,
		 *   performing an insertion if such key does not already exist.
		 */
		T &operator[](const Key &key)
		{
			RBnode *p = locate(root, key);
			if (p != nulptr)
				return p->data->second;
			pair<iterator, bool> res = insert(value_type(key, T()));
			return (res.first.cur_p)->data->second;
		}
		/**
		 * behave like at() throw index_out_of_bound if such key does not exist.
		 */
		const T &operator[](const Key &key) const
		{
			RBnode *p = locate(root, key);
			if (p == nulptr)
				throw index_out_of_bound();
			return p->data->second;
		}
		/**
		 * return a iterator to the beginning
		 */
		iterator begin()
		{
			return iterator(this, head);
		}
		const_iterator cbegin() const
		{
			return const_iterator(this, head);
		}
		/**
		 * return a iterator to the end
		 * in fact, it returns past-the-end.
		 */
		iterator end()
		{
			return iterator(this, nulptr);
		}
		const_iterator cend() const
		{
			return const_iterator(this, nulptr);
		}
		/**
		 * checks whether the container is empty
		 * return true if empty, otherwise false.
		 */
		bool empty() const
		{
			return currentSize == 0;
		}
		/**
		 * returns the number of elements.
		 */
		size_t size() const
		{
			return currentSize;
		}
		/**
		 * clears the contents
		 */
		void clear()
		{
			currentSize = 0;
			if (root != nulptr)
				destory(root);
			root = head = tail = nulptr;
			nulptr->left = nulptr->right = nulptr->father = nulptr->prev = nulptr->next = nulptr;
			nulptr->color = BLACK;
		}
		void L(RBnode *x)
		{
			RBnode *y = x->right;
			if (y == nulptr)
				return;
			x->right = y->left;
			if (y->left != nulptr)
				y->left->father = x;

			if (x->father == nulptr)
				root = y;
			else if (x->father->left == x)
				x->father->left = y;
			else
				x->father->right = y;
			y->father = x->father;

			x->father = y;
			y->left = x;
		}
		void R(RBnode *x)
		{
			RBnode *y = x->left;
			if (y == nulptr)
				return;
			x->left = y->right;
			if (y->right != nulptr)
				y->right->father = x;

			if (x->father == nulptr)
				root = y;
			else if (x->father->left == x)
				x->father->left = y;
			else
				x->father->right = y;
			y->father = x->father;

			x->father = y;
			y->right = x;
		}
		/**
		 * insert an element.
		 * return a pair, the first of the pair is
		 *   the iterator to the new element (or the element that prevented the insertion),
		 *   the second one is true if insert successfully, or false.
		 */
		RBnode *findPrev(RBnode *p)
		{
			if (p->left != nulptr)
			{
				RBnode *q = p->left;
				while (q->right != nulptr)
				{
					q = q->right;
				}
				p = q;
				return p;
			}

			while (p != root && p->father->left == p)
			{
				p = p->father;
			}
			return p->father;
		}
		RBnode *findNext(RBnode *p)
		{
			if (p->right != nulptr)
			{
				RBnode *q = p->right;
				while (q->left != nulptr)
				{
					q = q->left;
				}
				p = q;
				return p;
			}

			while (p != root && p->father->right == p)
			{
				p = p->father;
			}
			return p->father;
		}
		void insertAdjust(RBnode *self)
		{
			while (self != root && self->father->color == RED)
			{
				RBnode *fa = self->father;
				if (fa == fa->father->left)
				{
					RBnode *uncle = fa->father->right;
					if (uncle->color == RED)
					{
						fa->color = uncle->color = BLACK;
						fa->father->color = RED;
						self = fa->father;
					}
					else
					{
						if (self == fa->right)
						{
							self = fa;
							L(self);
							fa = self->father;
						}
						fa->color = BLACK;
						fa->father->color = RED;
						R(fa->father);
					}
				}
				else
				{
					RBnode *uncle = fa->father->left;
					if (uncle->color == RED)
					{
						fa->color = uncle->color = BLACK;
						fa->father->color = RED;
						self = fa->father;
					}
					else
					{
						if (self == fa->left)
						{
							self = fa;
							R(self);
							fa = self->father;
						}
						fa->color = BLACK;
						fa->father->color = RED;
						L(fa->father);
					}
				}
			}
		}
		
		pair<iterator, bool> insert(const value_type &value)
		{
			Key key = value.first;
			if (root == nulptr)
			{
				currentSize++;
				root = new RBnode(value, nulptr, nulptr, nulptr, nulptr, nulptr, BLACK);
				head = tail = root;
				return pair<iterator, bool>(iterator(this, root), true);
			}
			RBnode *fa = root;
			while (fa != nulptr)
			{
				if (cmp(key, fa->data->first))
				{
					if (fa->left == nulptr)
						break;
					fa = fa->left;
				}
				else if (cmp(fa->data->first, key))
				{
					if (fa->right == nulptr)
						break;
					fa = fa->right;
				}
				else
					return pair<iterator, bool>(iterator(this, fa), false);
			}
			currentSize++;
			RBnode *self = new RBnode(value, nulptr, nulptr, fa, nulptr, nulptr, RED);
			if (cmp(key, fa->data->first))
				fa->left = self;
			else
				fa->right = self;

			insertAdjust(self);
			self->prev = findPrev(self);
			self->next = findNext(self);
			if (self->prev != nulptr)
				self->prev->next = self;
			else
				head = self;
			if (self->next != nulptr)
				self->next->prev = self;
			else
				tail = self;
			return pair<iterator, bool>(iterator(this, self), true);
		}
		/**
		 * erase the element at pos.
		 *
		 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
		 */
		void eraseAdjust(RBnode *self)
		{
			while (self != root && self->color == BLACK)
			{
				RBnode *fa = self->father;
				if (self == fa->left)
				{
					RBnode *brother = fa->right;
					if (brother->color == RED)
					{
						brother->color = BLACK;
						fa->color = RED;
						L(fa);
						brother = fa->right;
					}
					if (brother->left->color == BLACK && brother->right->color == BLACK)
					{
						brother->color = RED;
						self = self->father;
					}
					else
					{
						if (brother->right->color == BLACK)
						{
							brother->left->color = BLACK;
							brother->color = RED;
							R(brother);
							brother = fa->right;
						}
						brother->color = fa->color;
						fa->color = BLACK;
						brother->right->color = BLACK;
						L(fa);
						self = root;
					}
				}
				else
				{
					RBnode *brother = fa->left;
					if (brother->color == RED)
					{
						brother->color = BLACK;
						fa->color = RED;
						R(fa);
						brother = fa->left;
					}
					if (brother->left->color == BLACK && brother->right->color == BLACK)
					{
						brother->color = RED;
						self = self->father;
					}
					else
					{
						if (brother->left->color == BLACK)
						{
							brother->right->color = BLACK;
							brother->color = RED;
							L(brother);
							brother = fa->left;
						}
						brother->color = fa->color;
						fa->color = BLACK;
						brother->left->color = BLACK;
						R(fa);
						self = root;
					}
				}
			}
			self->color = BLACK;
		}
		void erase(iterator iter)
		{
			if (iter.cur_p == nulptr || iter.its != this)
				throw invalid_iterator();
			RBnode *p = iter.cur_p, *q;

			currentSize--;

			if (currentSize == 0)
			{
				clear();
				return;
			}

			if (p->left != nulptr && p->right != nulptr)
			{
				RBnode *real = p->next;

				if (p == root)
					root = real;
				else
				{
					if (p->father->left == p)
						p->father->left = real;
					else
						p->father->right = real;
				}
				if (real->father->left == real)
					real->father->left = p;
				else
					real->father->right = p;

				RBnode *tmp = real->father;
				real->father = p->father;
				p->father = tmp;

				tmp = p->left;
				tmp->father = real;
				p->left = real->left;
				real->left = tmp;

				tmp = p->right;
				tmp->father = real;
				p->right = real->right;
				real->right = tmp;

				real->prev = p->prev;
				if (p->prev != nulptr)
					p->prev->next = real;
				else
					head = real;
			}
			else
			{
				if (p->prev != nulptr)
					p->prev->next = p->next;
				else
					head = p->next;
				if (p->next != nulptr)
					p->next->prev = p->prev;
				else
					tail = p->prev;
			}

			
            if (p->left == nulptr)
            {
                q = p->right;
            }
            else
                q=p->left;
			q->father = p->father;
			if (p->father == nulptr)
				root = q;
			else if (p == p->father->left)
				p->father->left = q;
			else
				p->father->right = q;

			if (p->color == BLACK)
				eraseAdjust(q);
			nulptr->color = BLACK;
			nulptr->left = nulptr->right = nulptr->father = nulptr->prev = nulptr->next = nulptr;
			delete p;
		}
		/**
		 * Returns the number of elements with key
		 *   that compares equivalent to the specified argument,
		 *   which is either 1 or 0
		 *     since this container does not allow duplicates.
		 * The default method of check the equivalence is !(a < b || b > a)
		 */
		size_t count(const Key &key) const
		{
			return locate(root, key) != nulptr;
		}
		/**
		 * Finds an element with key equivalent to key.
		 * key value of the element to search for.
		 * Iterator to an element with key equivalent to key.
		 *   If no such element is found, past-the-end (see end()) iterator is returned.
		 */
		iterator find(const Key &key)
		{
			return iterator(this, locate(root, key));
		}
		const_iterator find(const Key &key) const
		{
			return const_iterator(this, locate(root, key));
		}
	};

}

#endif
/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// Tiny Template Library: an impementation of red-black binary tree
//
// Base on algorithm description and code examples in Wikipedia article
//
// This code is Public Domain
//
#ifndef _TINY_TEMPLATE_LIBRARY_RBTREE_HPP_
#define _TINY_TEMPLATE_LIBRARY_RBTREE_HPP_ 1

#include "utility.hpp"

namespace ttl
{
   struct rbnode
   {
      rbnode *parent, *left, *right;
      bool color;
      static const bool RED = true;
      static const bool BLACK = false;
   };

   //
   // Left-leaning red-black tree
   //
   // Robert Sedgwick, "Left-leaning Red-black Trees", September 2008
   //
   // Iterative implementation of insertion and deletion based on
   // LLRB.h by William Ahern, 2013,
   // http://www.25thandclement.com/~william/projects/llrb.h.html
   //
   class rbtree_base
   {
   private:
      rbtree_base(const rbtree_base &);
      rbtree_base &operator=(const rbtree_base &);
   protected:
      rbnode *root_;
   public:
      rbtree_base(): root_(0) {}
      ~rbtree_base() {}

      struct link
      {
         rbnode **pos;
         rbnode *parent;
         link(rbnode **p, rbnode *pp): pos(p), parent(pp) {}

         rbnode *operator*() { return *pos; }
         const rbnode *operator*() const { return *pos; }
         rbnode *node() const { return pos ? *pos: 0; }

         link left() { return link(&(*pos)->left, *pos); }
         link right() { return link(&(*pos)->right, *pos); }
      };
      struct const_link
      {
         rbnode * const *pos;
         rbnode *parent;
         const_link(const link &h): pos(h.pos), parent(h.parent) {}
         const_link(rbnode * const *p, rbnode *pp): pos(p), parent(pp) {}

         rbnode *operator*() { return *pos; }
         const rbnode *operator*() const { return *pos; }
         rbnode *node() const { return pos ? *pos: 0; }

         const_link left() const { return const_link(&(*pos)->left, *pos); }
         const_link right() const { return const_link(&(*pos)->right, *pos); }
      };

      link get_root() { return link(&root_, 0); }
      const_link get_root() const { return const_link(&root_, 0); }
      const_link get_croot() const { return const_link(&root_, 0); }

      static rbnode *min_node(rbnode *n)
      {
         while (n && n->left)
            n = n->left;
         return n;
      }

      static rbnode *max_node(rbnode *n)
      {
         while (n && n->right)
            n = n->right;
         return n;
      }

      static rbnode *next_node(rbnode *n)
      {
         if  (n->right)
            return min_node(n->right);
         if (n->parent)
         {
            if (n == n->parent->left)
               return n->parent;
            while (n->parent && n == n->parent->right)
               n = n->parent;
            return n->parent;
         }
         return (rbnode *)0;
      }

      rbnode *insert(const link &pos, rbnode *newnode)
      {
         newnode->color = rbnode::RED;
         newnode->left = newnode->right = 0;
         newnode->parent = pos.parent;
         *pos.pos = newnode;
         insert_rebalance(pos);
         return newnode;
      }

      static rbnode *rotate_left(rbnode *a)
      {
         rbnode *b = a->right;
         a->right = b->left;
         if (a->right)
            a->right->parent = a;
         b->left = a;
         b->color = a->color;
         a->color = rbnode::RED;
         b->parent = a->parent;
         a->parent = b;
         return b;
      }

      static rbnode *rotate_right(rbnode *b)
      {
         rbnode *a = b->left;
         b->left = a->right;
         if (b->left)
            b->left->parent = b;
         a->right = b;
         a->color = b->color;
         b->color = rbnode::RED;
         a->parent = b->parent;
         b->parent = a;
         return a;
      }

      static void flip_colors(rbnode *n)
      {
         n->color = !n->color;
         n->left->color = !n->left->color;
         n->right->color = !n->right->color;
      }

      static bool is_red(rbnode *n)
      {
         return n && n->color == rbnode::RED;
      }

      static rbnode *fixup(rbnode *root)
      {
         if (is_red(root->right) && !is_red(root->left))
            root = rotate_left(root);
         if (is_red(root->left) && is_red(root->left->left))
            root = rotate_right(root);
         if (is_red(root->left) && is_red(root->right))
            flip_colors(root);
         return root;
      }

      rbnode **edge(rbnode *h)
      {
         if (h == root_)
            return &root_;
         if (h == h->parent->left)
            return &h->parent->left;
         return &h->parent->right;
      }

      void insert_rebalance(const link &h)
      {
         rbnode **root = h.pos, *parent = h.parent;
         while (parent && (is_red(parent->left) || is_red(parent->right)))
         {
            root = edge(parent);
            parent = parent->parent;
            *root = fixup(*root);
         }
         root_->color = rbnode::BLACK;
      }

      static rbnode *move_left(rbnode *pivot)
      {
         flip_colors(pivot);
         if (is_red(pivot->right->left))
         {
            pivot->right = rotate_right(pivot->right);
            pivot = rotate_left(pivot);
            flip_colors(pivot);
         }
         return pivot;
      }
      static rbnode *move_right(rbnode *pivot)
      {
         flip_colors(pivot);
         if (is_red(pivot->left->left))
         {
            pivot = rotate_right(pivot);
            flip_colors(pivot);
         }
         return pivot;
      }

      rbnode *delete_min(rbnode **root)
      {
         rbnode **pivot = root;
         while ((*pivot)->left)
         {
            if (!is_red((*pivot)->left) && !is_red((*pivot)->left->left))
               *pivot = move_left(*pivot);
            pivot = &(*pivot)->left;
         }
         rbnode *deleted = *pivot;
         rbnode *parent = deleted->parent;
         *pivot = 0;
         while (root != pivot)
         {
            pivot = edge(parent);
            parent = parent->parent;
            *pivot = fixup(*pivot);
         }
         return deleted;
      }

   };


   template <class K, class KV, class KeyOfValue, class Compare>
   class rbtree: public rbtree_base
   {
   public:
      typedef KeyOfValue keyof_type;

      struct node: rbnode
      {
         KV data;
         node(const KV &d): data(d) {}
      };

      rbtree() {}
      ~rbtree() { clear(); }

      node *insert_equal(const KV &data);
      node *insert_unique(const KV &data);

      node *remove(const K &key);

      const_link find(const K &) const;
      link find(const K &key)
      {
         const const_link h = static_cast<const rbtree *>(this)->find(key);
         return link(const_cast<rbnode **>(h.pos), h.parent);
      }

      const_link lower_bound(const K &) const;
      link lower_bound(const K &key)
      {
         const const_link h = static_cast<const rbtree *>(this)->lower_bound(key);
         return link(const_cast<rbnode **>(h.pos), h.parent);
      }

      const_link upper_bound(const K &) const;
      link upper_bound(const K &key)
      {
         const const_link h = static_cast<const rbtree *>(this)->upper_bound(key);
         return link(const_cast<rbnode **>(h.pos), h.parent);
      }

      pair<const_link,const_link> equal_range(const K &k) const
      {
         return pair<const_link,const_link>(lower_bound(k), upper_bound(k));
      }
      pair<link,link> equal_range(const K &k)
      {
         return pair<link,link>(lower_bound(k), upper_bound(k));
      }

      size_t count(const K &k) const;

      void clear();

   private:

      void preorder_destroy(node *n)
      {
         if (n->left)
            preorder_destroy(static_cast<node *>(n->left));
         if (n->right)
            preorder_destroy(static_cast<node *>(n->right));
         delete n;
      }
   };

   template <class K, class KV, class KeyOfValue, class Compare>
   void rbtree<K,KV,KeyOfValue,Compare>::clear()
   {
      node *root = static_cast<node *>(root_);
      root_ = 0;
      preorder_destroy(root);
   }

   template <class K, class KV, class KeyOfValue, class Compare>
   size_t rbtree<K,KV,KeyOfValue,Compare>::count(const K &key) const
   {
      Compare is_less;
      KeyOfValue keyof;
      const_link h = get_root();
      size_t c = 0;
      while (*h)
      {
         const K &hkey = keyof(static_cast<const node *>(*h)->data);
         if (is_less(hkey, key))
            h = h.right();
         else
         {
            if (hkey == key)
               ++c;
            h = h.left();
         }
      }
      return c;
   }

   template <class K, class KV, class KeyOfValue, class Compare>
   rbtree_base::const_link rbtree<K,KV,KeyOfValue,Compare>::find(const K &key) const
   {
      Compare is_less;
      KeyOfValue keyof;
      const_link h = get_root();
      while (*h)
      {
         const K &hkey = keyof(static_cast<const node *>(*h)->data);
         if (is_less(key, hkey))
            h = h.left();
         else if (key == hkey)
            break;
         else
            h = h.right();
      }
      return h;
   }

   template <class K, class KV, class KeyOfValue, class Compare>
   rbtree_base::const_link rbtree<K,KV,KeyOfValue,Compare>::lower_bound(const K &key) const
   {
      Compare is_less;
      KeyOfValue keyof;
      const_link h = get_root();
      const_link prev(0, 0);
      while (*h)
      {
         if (is_less(keyof(static_cast<const node *>(*h)->data), key))
            h = h.right();
         else
            prev = h, h = h.left();
      }

      return prev;
   }

   template <class K, class KV, class KeyOfValue, class Compare>
   rbtree_base::const_link rbtree<K,KV,KeyOfValue,Compare>::upper_bound(const K &key) const
   {
      Compare is_less;
      KeyOfValue keyof;
      const_link h = get_root();
      const_link prev(0, 0);
      while (*h)
      {
         if (is_less(key, keyof(static_cast<const node *>(*h)->data)))
            prev = h, h = h.left();
         else
            h = h.right();
      }
      return prev;
   }

   template <class K, class KV, class KeyOfValue, class Compare>
   typename rbtree<K,KV,KeyOfValue,Compare>::node *rbtree<K,KV,KeyOfValue,Compare>::insert_equal(const KV &data)
   {
      Compare is_less;
      KeyOfValue keyof;
      const K &key = keyof(data);
      link h = get_root();
      while (*h)
      {
         if (is_less(key, keyof(static_cast<const node *>(*h)->data)))
            h = h.left();
         else
            h = h.right();
      }
      node *newnode = new node(data);
      (void)insert(h, newnode);
      return newnode;
   }

   template <class K, class KV, class KeyOfValue, class Compare>
   typename rbtree<K,KV,KeyOfValue,Compare>::node *rbtree<K,KV,KeyOfValue,Compare>::insert_unique(const KV &data)
   {
      Compare is_less;
      KeyOfValue keyof;
      const K &key = keyof(data);
      link h = get_root();
      while (*h)
      {
         const K &hkey = keyof(static_cast<const node *>(*h)->data);
         if (is_less(key, hkey))
            h = h.left();
         else if (key == hkey)
            return (node *)0;
         else
            h = h.right();
      }
      node *newnode = new node(data);
      (void)insert(h, newnode);
      return newnode;
   }

   template <class K, class KV, class KeyOfValue, class Compare>
   typename rbtree<K,KV,KeyOfValue,Compare>::node *rbtree<K,KV,KeyOfValue,Compare>::remove(const K &key)
   {
      Compare compare;
      KeyOfValue keyof;
      rbnode **root  = &root_, *parent = 0, *deleted = 0;
      while (*root)
      {
         parent = (*root)->parent;
         bool isless = compare(key, keyof(static_cast<const node *>(*root)->data));
         if (isless)
         {
            if ((*root)->left && !is_red((*root)->left) && !is_red((*root)->left->left))
               *root = move_left(*root);
            root = &(*root)->left;
         }
         else
         {
            if (is_red((*root)->left))
            {
               *root = rotate_right(*root);
               isless = compare(key, keyof(static_cast<const node *>(*root)->data));
            }
            if (!isless &&
                key == keyof(static_cast<const node *>(*root)->data) &&
                !(*root)->right)
            {
               deleted = *root;
               *root = 0;
               break;
            }
            if ((*root)->right && !is_red((*root)->right) && !is_red((*root)->right->left))
            {
               *root = move_right(*root);
               isless = compare(key, keyof(static_cast<const node *>(*root)->data));
            }
            if (key == keyof(static_cast<const node *>(*root)->data))
            {
               rbnode *orphan = delete_min(&(*root)->right);
               orphan->color = (*root)->color;
               orphan->parent = (*root)->parent;
               orphan->right = (*root)->right;
               if (orphan->right)
                  orphan->right->parent = orphan;
               orphan->left = (*root)->left;
               if (orphan->left)
                  orphan->left->parent = orphan;
               deleted = *root;
               *root = orphan;
               parent = *root;
               break;
            }
            else
               root = &(*root)->right;
         }
      }
      while (parent)
      {
         root = edge(parent);
         parent = parent->parent;
         *root = fixup(*root);
      }
      if (root_)
         root_->color = rbnode::BLACK;
      return static_cast<node *>(deleted);
   }
}

inline bool operator==(const ttl::rbtree_base::link &a, const ttl::rbtree_base::link &b)
{
   return a.pos == b.pos;
}
inline bool operator==(const ttl::rbtree_base::const_link &a, const ttl::rbtree_base::const_link &b)
{
   return a.pos == b.pos;
}
inline bool operator==(const ttl::rbtree_base::link &a, const ttl::rbtree_base::const_link &b)
{
   return a.pos == b.pos;
}
inline bool operator==(const ttl::rbtree_base::const_link &a, const ttl::rbtree_base::link &b)
{
   return a.pos == b.pos;
}
inline bool operator!=(const ttl::rbtree_base::link &a, const ttl::rbtree_base::link &b)
{
   return a.pos != b.pos;
}
inline bool operator!=(const ttl::rbtree_base::const_link &a, const ttl::rbtree_base::const_link &b)
{
   return a.pos != b.pos;
}
inline bool operator!=(const ttl::rbtree_base::link &a, const ttl::rbtree_base::const_link &b)
{
   return a.pos != b.pos;
}
inline bool operator!=(const ttl::rbtree_base::const_link &a, const ttl::rbtree_base::link &b)
{
   return a.pos != b.pos;
}
#endif // _TINY_TEMPLATE_LIBRARY_RBTREE_HPP_

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

      static rbnode *min_node(const rbnode *n)
      {
         while (n && n->left)
            n = n->left;
         return const_cast<rbnode *>(n);
      }

      static rbnode *max_node(const rbnode *n)
      {
         while (n && n->right)
            n = n->right;
         return const_cast<rbnode *>(n);
      }

      static rbnode *next_node(const rbnode *n)
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
      static rbnode *prev_node(const rbnode *n)
      {
         if (n->left)
            return max_node(n->left);
         if (n->parent)
         {
            if (n == n->parent->right)
               return n->parent;
            while (n->parent && n == n->parent->left)
               n = n->parent;
            return n->parent;
         }
         return (rbnode *)0;
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

      rbnode **edge(rbnode *h) const
      {
         if (h == root_)
            return const_cast<rbnode **>(&root_);
         if (h == h->parent->left)
            return &h->parent->left;
         return &h->parent->right;
      }

      void insert_rebalance(rbnode **root, rbnode *parent)
      {
         (*root)->color = rbnode::RED;
         (*root)->left = (*root)->right = 0;
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

      void assign(const rbtree &);

      node *insert_equal(const KV &data);
      node *insert_unique(const KV &data);

      node *remove(const K &key);

      node *get_root() { return static_cast<node *>(root_); }
      const node *get_root() const { return static_cast<const node *>(root_); }
      const node *get_croot() const { return static_cast<const node *>(root_); }

      const node *find(const K &) const;
      node *find(const K &key)
      {
         return const_cast<node *>(static_cast<const rbtree *>(this)->find(key));
      }

      const node *lower_bound(const K &) const;
      node *lower_bound(const K &key)
      {
         return const_cast<node *>(static_cast<const rbtree *>(this)->lower_bound(key));
      }

      const node *upper_bound(const K &) const;
      node *upper_bound(const K &key)
      {
         return const_cast<node *>(static_cast<const rbtree *>(this)->upper_bound(key));
      }

      pair<const node *, const node *> equal_range(const K &k) const
      {
         return pair<const node *, const node *>(lower_bound(k), upper_bound(k));
      }
      pair<node *, node *> equal_range(const K &k)
      {
         return pair<node *, node *>(lower_bound(k), upper_bound(k));
      }

      size_t count(const K &k) const;

      void clear();

   protected:
      KeyOfValue keyof_;
      Compare is_less_;

      void postorder_destroy(node *n)
      {
         if (!n)
            return;

         if (n->left)
            postorder_destroy(static_cast<node *>(n->left));
         if (n->right)
            postorder_destroy(static_cast<node *>(n->right));
         delete n;
      }

      rbnode *preorder_copy(const node *n)
      {
         if (!n)
            return 0;
         node *nc = new node(n->data);
         nc->color = n->color;
         if (n->left)
            nc->left = preorder_copy(static_cast<const node *>(n->left)),
            nc->left->parent = nc;
         else
            nc->left = 0;
         if (n->right)
            nc->right = preorder_copy(static_cast<const node *>(n->right)),
            nc->right->parent = nc;
         else
            nc->right = 0;
         return nc;
      }
   };

   template <class K, class KV, class KeyOfValue, class Compare>
   void rbtree<K,KV,KeyOfValue,Compare>::assign(const rbtree &other)
   {
      if (root_)
         clear();
      root_ = preorder_copy(other.get_croot());
      if (root_)
         root_->parent = 0;
   }

   template <class K, class KV, class KeyOfValue, class Compare>
   void rbtree<K,KV,KeyOfValue,Compare>::clear()
   {
      node *root = static_cast<node *>(root_);
      root_ = 0;
      postorder_destroy(root);
   }

   template <class K, class KV, class KeyOfValue, class Compare>
   size_t rbtree<K,KV,KeyOfValue,Compare>::count(const K &key) const
   {
      const rbnode *n = root_;
      size_t c = 0;
      while (n)
      {
         const K &nkey = keyof_(static_cast<const node *>(n)->data);
         if (is_less_(nkey, key))
            n = n->right;
         else
         {
            if (nkey == key)
               ++c;
            n = n->left;
         }
      }
      return c;
   }

   template <class K, class KV, class KeyOfValue, class Compare>
   const typename rbtree<K,KV,KeyOfValue,Compare>::node *
   rbtree<K,KV,KeyOfValue,Compare>::find(const K &key) const
   {
      const rbnode *n = root_;
      while (n)
      {
         const K &nkey = keyof_(static_cast<const node *>(n)->data);
         if (is_less_(key, nkey))
            n = n->left;
         else if (key == nkey)
            break;
         else
            n = n->right;
      }
      return static_cast<const node *>(n);
   }

   template <class K, class KV, class KeyOfValue, class Compare>
   const typename rbtree<K,KV,KeyOfValue,Compare>::node *
   rbtree<K,KV,KeyOfValue,Compare>::lower_bound(const K &key) const
   {
      const rbnode *n = root_, *prev = 0;
      while (n)
      {
         if (is_less_(keyof_(static_cast<const node *>(n)->data), key))
            n = n->right;
         else
            prev = n, n = n->left;
      }

      return static_cast<const node *>(prev);
   }

   template <class K, class KV, class KeyOfValue, class Compare>
   const typename rbtree<K,KV,KeyOfValue,Compare>::node *
   rbtree<K,KV,KeyOfValue,Compare>::upper_bound(const K &key) const
   {
      const rbnode *n = root_, *prev = 0;
      while (n)
      {
         if (is_less_(key, keyof_(static_cast<const node *>(n)->data)))
            prev = n, n = n->left;
         else
            n = n->right;
      }
      return static_cast<const node *>(prev);
   }

   template <class K, class KV, class KeyOfValue, class Compare>
   typename rbtree<K,KV,KeyOfValue,Compare>::node *
   rbtree<K,KV,KeyOfValue,Compare>::insert_equal(const KV &data)
   {
      rbnode *parent = 0;
      rbnode **edge = &root_;
      for (const K &key = keyof_(data); *edge;)
      {
         parent = *edge;
         if (is_less_(key, keyof_(static_cast<const node *>(*edge)->data)))
            edge = &(*edge)->left;
         else
            edge = &(*edge)->right;
      }
      node *newnode = new node(data);
      newnode->parent = parent;
      *edge = newnode;
      insert_rebalance(edge, parent);
      return newnode;
   }

   template <class K, class KV, class KeyOfValue, class Compare>
   typename rbtree<K,KV,KeyOfValue,Compare>::node *
   rbtree<K,KV,KeyOfValue,Compare>::insert_unique(const KV &data)
   {
      rbnode *parent = 0;
      rbnode **edge = &root_;
      for (const K &key = keyof_(data); *edge;)
      {
         const K &ekey = keyof_(static_cast<const node *>(*edge)->data);
         if (is_less_(key, ekey))
            parent = *edge, edge = &(*edge)->left;
         else if (key == ekey)
            return (node *)0;
         else
            parent = *edge, edge = &(*edge)->right;
      }
      node *newnode = new node(data);
      newnode->parent = parent;
      *edge = newnode;
      insert_rebalance(edge, parent);
      return newnode;
   }

   template <class K, class KV, class KeyOfValue, class Compare>
   typename rbtree<K,KV,KeyOfValue,Compare>::node *
   rbtree<K,KV,KeyOfValue,Compare>::remove(const K &key)
   {
      rbnode **root  = &root_, *parent = 0, *deleted = 0;
      while (*root)
      {
         parent = (*root)->parent;
         bool isless = is_less_(key, keyof_(static_cast<const node *>(*root)->data));
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
               isless = is_less_(key, keyof_(static_cast<const node *>(*root)->data));
            }
            if (!isless &&
                key == keyof_(static_cast<const node *>(*root)->data) &&
                !(*root)->right)
            {
               deleted = *root;
               *root = 0;
               break;
            }
            if ((*root)->right && !is_red((*root)->right) && !is_red((*root)->right->left))
            {
               *root = move_right(*root);
               isless = is_less_(key, keyof_(static_cast<const node *>(*root)->data));
            }
            if (key == keyof_(static_cast<const node *>(*root)->data))
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

#endif // _TINY_TEMPLATE_LIBRARY_RBTREE_HPP_

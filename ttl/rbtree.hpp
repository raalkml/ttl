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
      rbnode header_;
      rbnode **root_edge() const { return const_cast<rbnode **>(&header_.parent); }
      rbnode *root_() { return header_.parent; }
      const rbnode *root_() const { return header_.parent; }
   public:
      rbtree_base()
      {
         header_.parent = header_.left = header_.right = 0;
         header_.color = rbnode::RED;
      }
      ~rbtree_base() {}

      static rbnode *min_node(const rbnode *n);
      static rbnode *max_node(const rbnode *n);
      static rbnode *next_node(const rbnode *n);
      static rbnode *prev_node(const rbnode *n);
      static rbnode *rotate_left(rbnode *a);
      static rbnode *rotate_right(rbnode *b);
      static void flip_colors(rbnode *n);
      static bool is_red(rbnode *n);
      static rbnode *fixup(rbnode *root);

      rbnode **edge(rbnode *h) const;
      void insert_rebalance(rbnode **root, rbnode *parent);

      static rbnode *move_left(rbnode *pivot);
      static rbnode *move_right(rbnode *pivot);

      rbnode *delete_min(rbnode **root);
   };

   inline void rbtree_base::flip_colors(rbnode *n)
   {
      n->color = !n->color;
      n->left->color = !n->left->color;
      n->right->color = !n->right->color;
   }

   inline bool rbtree_base::is_red(rbnode *n)
   {
      return n && n->color == rbnode::RED;
   }

   inline rbnode **rbtree_base::edge(rbnode *h) const
   {
      if (h == root_())
         return root_edge();
      if (h == h->parent->left)
         return &h->parent->left;
      return &h->parent->right;
   }

#ifndef RBTREE_INLINEABLE
#define RBTREE_INLINEABLE inline
#define RBTREE_INCLUDE_INLINEABLE 1
#endif

#if (RBTREE_INCLUDE_INLINEABLE == 1)

   RBTREE_INLINEABLE rbnode *rbtree_base::min_node(const rbnode *n)
   {
      while (n && n->left)
         n = n->left;
      return const_cast<rbnode *>(n);
   }

   RBTREE_INLINEABLE rbnode *rbtree_base::max_node(const rbnode *n)
   {
      while (n && n->right)
         n = n->right;
      return const_cast<rbnode *>(n);
   }

   RBTREE_INLINEABLE rbnode *rbtree_base::next_node(const rbnode *n)
   {
      if  (n->right)
         return min_node(n->right);
      if (n == n->parent->left)
         return n->parent;
      while (n == n->parent->right)
         n = n->parent;
      return n->parent;
   }

   RBTREE_INLINEABLE rbnode *rbtree_base::prev_node(const rbnode *n)
   {
      if (n->left)
         return max_node(n->left);
      if (n == n->parent->right)
         return n->parent;
      while (n == n->parent->left)
         n = n->parent;
      return n->parent;
   }

   RBTREE_INLINEABLE rbnode *rbtree_base::rotate_left(rbnode *a)
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

   RBTREE_INLINEABLE rbnode *rbtree_base::rotate_right(rbnode *b)
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

   RBTREE_INLINEABLE rbnode *rbtree_base::fixup(rbnode *root)
   {
      if (is_red(root->right) && !is_red(root->left))
         root = rotate_left(root);
      if (is_red(root->left) && is_red(root->left->left))
         root = rotate_right(root);
      if (is_red(root->left) && is_red(root->right))
         flip_colors(root);
      return root;
   }

   RBTREE_INLINEABLE void rbtree_base::insert_rebalance(rbnode **root, rbnode *parent)
   {
      (*root)->color = rbnode::RED;
      (*root)->left = (*root)->right = 0;
      while (parent != &header_ && (is_red(parent->left) || is_red(parent->right)))
      {
         root = edge(parent);
         parent = parent->parent;
         *root = fixup(*root);
      }
      root_()->color = rbnode::BLACK;
   }

   RBTREE_INLINEABLE rbnode *rbtree_base::move_left(rbnode *pivot)
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

   RBTREE_INLINEABLE rbnode *rbtree_base::move_right(rbnode *pivot)
   {
      flip_colors(pivot);
      if (is_red(pivot->left->left))
      {
         pivot = rotate_right(pivot);
         flip_colors(pivot);
      }
      return pivot;
   }

   RBTREE_INLINEABLE rbnode *rbtree_base::delete_min(rbnode **root)
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
#endif //  RBTREE_MERGE(RBTREE_INLINEABLE) == 1

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
      pair<node *, bool> insert_unique(const KV &data);

      node *remove(const K &key);

      node *get_root() { return static_cast<node *>(root_()); }
      const node *get_root() const { return static_cast<const node *>(root_()); }
      const node *get_croot() const { return static_cast<const node *>(root_()); }

      node *end() { return static_cast<node *>(&header_); }
      const node *end() const { return static_cast<const node *>(&header_); }

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

      void postorder_destroy(node *n);
      rbnode *preorder_copy(const node *n);
   };

   template <class K, class KV, class KeyOfValue, class Compare>
   void rbtree<K,KV,KeyOfValue,Compare>::postorder_destroy(node *n)
   {
      if (!n)
         return;

      if (n->left)
         postorder_destroy(static_cast<node *>(n->left));
      if (n->right)
         postorder_destroy(static_cast<node *>(n->right));
      delete n;
   }

   template <class K, class KV, class KeyOfValue, class Compare>
   rbnode *rbtree<K,KV,KeyOfValue,Compare>::preorder_copy(const node *n)
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

   template <class K, class KV, class KeyOfValue, class Compare>
   void rbtree<K,KV,KeyOfValue,Compare>::assign(const rbtree &other)
   {
      if (root_())
         clear();
      const node *otherroot = other.get_root();
      if (otherroot)
         (*root_edge() = preorder_copy(otherroot))->parent = &header_;
   }

   template <class K, class KV, class KeyOfValue, class Compare>
   void rbtree<K,KV,KeyOfValue,Compare>::clear()
   {
      node *root = static_cast<node *>(root_());
      *root_edge() = 0;
      postorder_destroy(root);
   }

   template <class K, class KV, class KeyOfValue, class Compare>
   size_t rbtree<K,KV,KeyOfValue,Compare>::count(const K &key) const
   {
      const rbnode *n = root_();
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
      const rbnode *n = root_();
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
      return static_cast<const node *>(n ? n: &header_);
   }

   template <class K, class KV, class KeyOfValue, class Compare>
   const typename rbtree<K,KV,KeyOfValue,Compare>::node *
   rbtree<K,KV,KeyOfValue,Compare>::lower_bound(const K &key) const
   {
      const rbnode *n = root_(), *prev = &header_;
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
      const rbnode *n = root_(), *prev = &header_;
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
      rbnode *parent = &header_;
      rbnode **edge = root_edge();
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
   ttl::pair<typename rbtree<K,KV,KeyOfValue,Compare>::node *, bool>
   rbtree<K,KV,KeyOfValue,Compare>::insert_unique(const KV &data)
   {
      rbnode *parent = &header_;
      rbnode **edge = root_edge();
      for (const K &key = keyof_(data); *edge;)
      {
         const K &ekey = keyof_(static_cast<const node *>(*edge)->data);
         if (is_less_(key, ekey))
            parent = *edge, edge = &(*edge)->left;
         else if (key == ekey)
            return pair<node *, bool>(static_cast<node *>(*edge), false);
         else
            parent = *edge, edge = &(*edge)->right;
      }
      node *newnode = new node(data);
      newnode->parent = parent;
      *edge = newnode;
      insert_rebalance(edge, parent);
      return pair<node *, bool>(static_cast<node *>(newnode), true);
   }

   template <class K, class KV, class KeyOfValue, class Compare>
   typename rbtree<K,KV,KeyOfValue,Compare>::node *
   rbtree<K,KV,KeyOfValue,Compare>::remove(const K &key)
   {
      rbnode **root = root_edge(), *parent = &header_, *deleted = 0;
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
      while (parent != &header_)
      {
         root = edge(parent);
         parent = parent->parent;
         *root = fixup(*root);
      }
      if (root_())
         root_()->color = rbnode::BLACK;
      return static_cast<node *>(deleted);
   }
}

#endif // _TINY_TEMPLATE_LIBRARY_RBTREE_HPP_

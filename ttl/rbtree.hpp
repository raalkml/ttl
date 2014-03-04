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
   // Iterative implementation of insertion and deletion inspired by
   // LLRB.h by William Ahern, 2013,
   // http://www.25thandclement.com/~william/projects/llrb.h.html
   //
   class llrbtree
   {
   private:
      llrbtree(const llrbtree &);
      llrbtree &operator=(const llrbtree &);
   protected:
      rbnode *root_;
   public:
      llrbtree(): root_(NULL) {}
      ~llrbtree() {}

      struct hint
      {
         rbnode **pos;
         rbnode *parent;
         hint(rbnode **p, rbnode *pp): pos(p), parent(pp) {}

         rbnode *operator*() { return *pos; }
         const rbnode *operator*() const { return *pos; }

         hint left() { return hint(&(*pos)->left, *pos); }
         hint right() { return hint(&(*pos)->right, *pos); }
      };
      struct const_hint
      {
         rbnode * const *pos;
         rbnode *parent;
         const_hint(const hint &h): pos(h.pos), parent(h.parent) {}
         const_hint(rbnode * const *p, rbnode *pp): pos(p), parent(pp) {}

         rbnode *operator*() { return *pos; }
         const rbnode *operator*() const { return *pos; }

         const_hint left() const { return const_hint(&(*pos)->left, *pos); }
         const_hint right() const { return const_hint(&(*pos)->right, *pos); }
      };

      hint get_root() { return hint(&root_, NULL); }
      const_hint get_root() const { return const_hint(&root_, NULL); }
      const_hint get_croot() const { return const_hint(&root_, NULL); }

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
         return NULL;
      }

      rbnode *insert(const hint &, rbnode *newnode);

      template<typename Node, typename Pred>
      rbnode *insert(Node *n, const Pred &pred);

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

      void insert_rebalance(const hint &h)
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
         *pivot = NULL;
         while (root != pivot)
         {
            pivot = edge(parent);
            parent = parent->parent;
            *pivot = fixup(*pivot);
         }
         return deleted;
      }

   };

   inline rbnode *llrbtree::insert(const hint &pos, rbnode *newnode)
   {
      newnode->color = rbnode::RED;
      newnode->left = newnode->right = NULL;
      newnode->parent = pos.parent;
      *pos.pos = newnode;
      insert_rebalance(pos);
      return newnode;
   }

   template<typename Node, typename Pred>
   rbnode *llrbtree::insert(Node *newnode, const Pred &pred)
   {
      hint h = get_root();
      while (*h)
      {
         if (pred(newnode->first, static_cast<const Node *>(*h)->first))
            h = h.left();
         else if (newnode->first == static_cast<const Node *>(*h)->first)
            return NULL;
         else
            h = h.right();
      }
      return insert(h, newnode);
   }

   template <class T, class Compare>
   class rbtree: public llrbtree
   {
   public:
      struct node: rbnode
      {
         T data;
         node(const T &d): data(d) {}
      };

      rbtree() {}
      ~rbtree() {}

      node *insert_equal(const T &data)
      {
         Compare compare;
         hint h = get_root();
         while (*h)
         {
            if (compare(data, static_cast<const node *>(*h)->data))
               h = h.left();
            else
               h = h.right();
         }
         node *newnode = new node(data);
         (void)llrbtree::insert(h, newnode);
         return newnode;
      }

      node *insert_unique(const T &data)
      {
         Compare compare;
         hint h = get_root();
         while (*h)
         {
            if (compare(data, static_cast<const node *>(*h)->data))
               h = h.left();
            else if (data == static_cast<const node *>(*h)->data)
               return NULL;
            else
               h = h.right();
         }
         node *newnode = new node(data);
         (void)llrbtree::insert(h, newnode);
         return newnode;
      }

      node *remove(rbnode *elm)
      {
         rbnode **root  = &root_, *parent = NULL, *deleted = NULL;
         while (*root)
         {
            parent = (*root)->parent;
         }
      }
   };

}
#endif // _TINY_TEMPLATE_LIBRARY_RBTREE_HPP_

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

      struct rbnode *grandparent() const
      {
         return parent ? parent->parent: NULL;
      }

      struct rbnode *uncle() const
      {
         struct rbnode *g = grandparent();

         if (!g)
            return NULL; // No grandparent means no uncle

         return parent == g->left ? g->right: g->left;
      }

      struct rbnode *sibling() const
      {
         return this == parent->left ? parent->right: parent->left;
      }

      void init_root()
      {
         parent = left = right = NULL;
         color = BLACK;
      }
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
   class rbtree
   {
   protected:
      rbnode *root_;
   public:
      rbtree(): root_(NULL) {}
      ~rbtree() {}

      template<typename Node, typename Pred>
      rbnode *insert(Node *n, Pred pred);

      rbnode *__root() const { return root_; }

   private:
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
      static void flip_colors(rbnode *root)
      {
         root->color = !root->color;
         root->left->color = !root->left->color;
         root->right->color = !root->right->color;
      }
      static bool is_red(rbnode *h)
      {
         return h && h->color == rbnode::RED;
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
      void rebalance(rbnode **root, rbnode *parent)
      {
         while (parent && (is_red(parent->left) || is_red(parent->right)))
         {
            root = edge(parent);
            parent = parent->parent;
            *root = fixup(*root);
         }
         root_->color = rbnode::BLACK;
      }
   };

   template<typename Node, typename Pred>
   rbnode *rbtree::insert(Node *newnode, Pred pred)
   {
      rbnode **root = &root_, *parent = NULL;
      while (*root)
      {
         parent = *root;
         if (pred(newnode->first, static_cast<const Node *>(*root)->first))
            root = &(*root)->left;
         else if (newnode->first == static_cast<const Node *>(*root)->first)
         {
            fprintf(stderr, "the key already present\n");
            return NULL;
         }
         else
            root = &(*root)->right;
      }
      newnode->color = rbnode::RED;
      newnode->left = newnode->right = NULL;
      newnode->parent = parent;
      *root = newnode;
      rebalance(root, parent);
      return newnode;
   }
}
#endif // _TINY_TEMPLATE_LIBRARY_RBTREE_HPP_

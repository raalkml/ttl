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
      enum { RED, BLACK } color;

      void flip_color()
      {
         if (color == RED)
            color = BLACK;
         else
            color = RED;
      }

      struct rbnode *grandparent() const
      {
         return parent ? parent->parent: NULL;
      }

      struct rbnode *uncle() const
      {
         struct rbnode *g = grandparent();

         if (g == NULL)
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
   // Left-leaning red-black tree (Sedgwick)
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
      template<typename Node, typename Pred>
      rbnode *insert(rbnode *root, Node *newnode, Pred pred);
      static rbnode *rotate_left(rbnode *h)
      {
         rbnode *x = h->right;
         h->right = x->left;
         x->left = h;
         x->color = h->color;
         h->color = rbnode::RED;
         return x;
      }
      static rbnode *rotate_right(rbnode *h)
      {
         rbnode *x = h->left;
         h->left = x->right;
         x->right = h;
         x->color = h->color;
         h->color = rbnode::RED;
         return x;
      }
      static bool is_red(rbnode *h)
      {
         return h && h->color == rbnode::RED;
      }
   };

   template<typename Node, typename Pred>
   rbnode *rbtree::insert(Node *newnode, Pred pred)
   {
      newnode->color = rbnode::RED;
      newnode->left = newnode->right = NULL;
      root_ = insert(root_, newnode, pred);
      root_->color = rbnode::BLACK;
      return newnode;
   }
   template<typename Node, typename Pred>
   rbnode *rbtree::insert(rbnode *h, Node *newnode, Pred pred)
   {
      if (!h)
         return newnode;
      if (is_red(h->left) && is_red(h->right))
      {
         h->flip_color();
         h->left->flip_color();
         h->right->flip_color();
      }
      if (newnode->first == static_cast<Node *>(h)->first)
      {
         fprintf(stderr, "duplicate!\n");
      }
      else if (pred(newnode->first, static_cast<Node *>(h)->first))
         h->left = insert(h->left, newnode, pred);
      else
         h->right = insert(h->right, newnode, pred);

      if (is_red(h->right) && !is_red(h->left))
         h = rotate_left(h);
      if (is_red(h->left) && is_red(h->left->left))
         h = rotate_right(h);
      return h;
   }
}
#endif // _TINY_TEMPLATE_LIBRARY_RBTREE_HPP_

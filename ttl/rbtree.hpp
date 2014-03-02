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
         parent = NULL;
         left = NULL;
         right = NULL;
         color = BLACK;
      }
   };
}
#endif // _TINY_TEMPLATE_LIBRARY_RBTREE_HPP_

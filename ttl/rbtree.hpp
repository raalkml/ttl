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
         parent = left = right = NULL;
         color = BLACK;
      }
   };

   //
   // Red-Black Tree: a binary tree with special property to stop it from
   // becoming unbalanced.
   //
   // 1. A node is either red or black
   // 2. The root is black
   // 3. All leaves (the NULL children) are black (same as root)
   // 4. Every red node must have two black child nodes
   // 5. Every path from a given node to any of its descendant leaves
   //    contains the same number of black nodes.
   //
   // Wikipedia: "This constraints enforce the critical property of
   // red-black trees: that the path from the root to the furthest leaf is
   // no more twice as long as the path from the root to the nearest leaf".
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

      void post_insert(rbnode *n)
      {
         insert_case1(n);
      }
   private:
      // the node is at the root. Just paint it black to satisfy the
      // Red-Black tree property 2, and since this operation adds a black
      // node to all the paths in the tree, the rule 5 is also not violated
      void insert_case1(rbnode *n)
      {
         if (!n->parent)
            n->color = rbnode::BLACK;
         else
            insert_case2(n);
      }
      void insert_case2(rbnode *n)
      {
         if (n->parent->color == rbnode::BLACK)
            return; // tree is still valid
         else
            insert_case3(n);
      }
      void insert_case3(rbnode *n)
      {
         rbnode *u = n->uncle(), *g;
         if (u && u->color == rbnode::RED)
         {
            n->parent->color = rbnode::BLACK;
            u->color = rbnode::BLACK;
            g = n->grandparent();
            g->color = rbnode::RED;
            insert_case1(n);
         }
         else
            insert_case4(n);
      }
      void rotate_left(rbnode *n, rbnode *g)
      {
         rbnode *saved_p = g->left, *saved_left_n = n->left;
         g->left = n;
         n->left = saved_p;
         saved_p->right = saved_left_n;
         // XXX? ... and modify the parent's nodes properly
      }
      void rotate_right(rbnode *n, rbnode *g)
      {
         rbnode *saved_p = g->right, *saved_right_n = n->right;
         g->right = n;
         n->right = saved_p;
         saved_p->left = saved_right_n;
      }
      void insert_case4(rbnode *n)
      {
         rbnode *g = n->grandparent();
         if (n == n->parent->right && n->parent == g->left)
         {
            rotate_left(n->parent, g);
            n = n->left;
         }
         else if (n == n->parent->left && n->parent == g->right)
         {
            rotate_right(n->parent, g);
            n = n->right;
         }
         insert_case5(n);
      }
      void insert_case5(rbnode *n)
      {
         rbnode *g = n->grandparent();
         n->parent->color = rbnode::BLACK;
         g->color = rbnode::RED;
         if (n == n->parent->left)
            rotate_right(g, g->grandparent());
         else
            rotate_left(g, g->grandparent());
      }
   };

   template<typename Node, typename Pred>
   rbnode *rbtree::insert(Node *newnode, Pred pred)
   {
      newnode->left = newnode->right = NULL;
      rbnode *n = root_;
      while (n)
      {
         if (pred(newnode->first, static_cast<Node *>(n)->first))
         {
            if (n->left)
               n = n->left;
            else
            {
               newnode->parent = n;
               n->left = newnode;
               return newnode;
            }
         }
         else
         {
            if (n->right)
               n = n->right;
            else
            {
               newnode->parent = n;
               n->right = newnode;
               return newnode;
            }
         }
      }
      newnode->parent = NULL;
      root_ = newnode;
      return newnode;
   }
}
#endif // _TINY_TEMPLATE_LIBRARY_RBTREE_HPP_

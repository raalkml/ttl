/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// Tiny Template Library: an implementation of a node in single-linked list
//
// This code is Public Domain
//
#ifndef _TINY_TEMPLATE_LIBRARY_SLIST_NODE_HPP_
#define _TINY_TEMPLATE_LIBRARY_SLIST_NODE_HPP_ 1

#include "types.hpp"
#include "utility.hpp"

namespace ttl
{
   struct slist_node
   {
      slist_node *next;

      slist_node *insert_after(slist_node *n)
      {
         n->next = next;
         next = n;
         return n;
      }
      slist_node *unlink_next()
      {
         slist_node *n = next;
         next = n->next;
         return n;
      }
      void splice_after(slist_node *o)
      {
         slist_node *n = o->next;
         o->next = n->next;
         n->next = next;
         next = n;
      }
      void splice_after(slist_node *f, slist_node *l)
      {
         slist_node *n = next;
         next = f->next;
         slist_node *o = f;
         while (o->next != l)
            o = o->next;
         o->next = n;
         f->next = l;
      }
      void reverse()
      {
         slist_node *reversed = 0;
         slist_node *orig = next;
         while (orig)
         {
            slist_node *n = orig;
            orig = n->next;
            n->next = reversed;
            reversed = n;
         }
         next = reversed;
      }
   };
}

#endif // _TINY_TEMPLATE_LIBRARY_SLIST_NODE_HPP_


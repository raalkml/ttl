// vim: sw=3 ts=8 et
#include "t.hpp"
#include "ttl/functional.hpp"
#include "ttl/rbtree.hpp"

typedef ttl::rbtree<int,
        ttl::pair<int,char>,
        ttl::select_first< ttl::pair<int,char> >,
        ttl::less<int> > rbtree_map;

template<typename T>
struct select_same
{
   const T &operator()(const T &r) const { return r; }
};

typedef ttl::rbtree<int, int, select_same<int>, ttl::less<int> > rbtree_set;

template<typename Container>
static void inorder(const ttl::rbtree_base::const_hint &h, int depth = 0)
{
   ttl::rbtree_base::const_hint c = h.left();
   if (*c)
      inorder<Container>(c, depth + 1);
   for (int i = depth; i--;)
      fputc(' ', stdout);
   typename Container::keyof_type keyof;
   printf("%d\n", keyof(static_cast<const typename Container::node *>(*h)->data));
   c = h.right();
   if (*c)
      inorder<Container>(c, depth + 1);
}

void test()
{
   printf("sizeof rbnode %lu, rbtree_map::node %lu, rbtree_set::node %lu\n",
          (unsigned long)sizeof(ttl::rbnode),
          (unsigned long)sizeof(rbtree_map::node),
          (unsigned long)sizeof(rbtree_set::node));
   rbtree_map t;
   for (unsigned c = 10; c--; )
   {
      ttl::pair<int,char> d(c, -c);
      rbtree_map::node *pos = t.insert_equal(d);
      printf("%p %d (%d)\n", pos, d.first, d.second);
   }

   printf("red-black tree with 10 elements\n");
   inorder<rbtree_map>(t.get_croot());

   ttl::pair<int,char> dupe(5, -5);
   assert(NULL == t.insert_unique(dupe));
   assert(NULL != t.insert_equal(dupe));

   printf("inserted dupe (5)\n");
   inorder<rbtree_map>(t.get_croot());

   t.delete_min(t.get_root().pos);
   t.delete_min(t.get_root().pos);
   t.delete_min(t.get_root().pos);

   printf("3 calls to delete_min from root\n");
   inorder<rbtree_map>(t.get_croot());

   t.remove(7);
   printf("remove 7\n");
   inorder<rbtree_map>(t.get_croot());

   printf("find & lower_boundary\n");
   {
      ttl::rbtree_base::const_hint h = t.find(3);
      int k = rbtree_map::keyof_type()(static_cast<const rbtree_map::node *>(*h)->data);
      printf("find: %d\n", k);
      assert(3 == k);

      h = t.lower_bound(3);
      k = h.pos ? rbtree_map::keyof_type()(static_cast<const rbtree_map::node *>(*h)->data): 0;
      printf("lower_bound(%d): %d\n", 3, k);
      assert(k >= 3);

      h = t.lower_bound(100);
      k = h.pos ? rbtree_map::keyof_type()(static_cast<const rbtree_map::node *>(*h)->data): 0;
      printf("lower_bound(%d): %p %d\n", 100, h.pos ? *h: NULL, k);
      assert(h.pos == NULL);

      h = t.lower_bound(7); // removed value
      k = h.pos ? rbtree_map::keyof_type()(static_cast<const rbtree_map::node *>(*h)->data): 0;
      printf("lower_bound(%d): %p %d\n", 7, h.pos ? *h: NULL, k);
      assert(k >= 7);

      h = t.upper_bound(7); // removed value
      k = h.pos ? rbtree_map::keyof_type()(static_cast<const rbtree_map::node *>(*h)->data): 0;
      printf("upper_bound(%d): %p %d\n", 7, h.pos ? *h: NULL, k);
      assert(k > 7);

      h = t.upper_bound(5);
      k = h.pos ? rbtree_map::keyof_type()(static_cast<const rbtree_map::node *>(*h)->data): 0;
      printf("upper_bound(%d): %p %d\n", 5, h.pos ? *h: NULL, k);
      assert(k > 5);

      h = t.upper_bound(100);
      k = h.pos ? rbtree_map::keyof_type()(static_cast<const rbtree_map::node *>(*h)->data): 0;
      printf("upper_bound(%d): %p %d\n", 100, h.pos ? *h: NULL, k);
      assert(h.pos == NULL);
   }

   rbtree_set s;
   s.insert_unique(1);
   s.insert_unique(2);
   s.insert_unique(3);
   s.insert_unique(3);
   printf("set of three elements\n");
   inorder<rbtree_set>(s.get_croot());

   s.remove(1);
   printf("set of two elements\n");
   inorder<rbtree_set>(s.get_croot());
}

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
static void inorder(const ttl::rbnode *n, int depth = 0)
{
   if (n && n->left)
      inorder<Container>(n->left, depth + 1);
   for (int i = depth; i--;)
      fputc(' ', stdout);
   typename Container::keyof_type keyof;
   printf("%p:%d\n", n, n ? keyof(static_cast<const typename Container::node *>(n)->data): -1);
   if (n && n->right)
      inorder<Container>(n->right, depth + 1);
}

void test()
{
   printf("sizeof rbnode %lu, rbtree_map::node %lu, rbtree_set::node %lu\n",
          (unsigned long)sizeof(ttl::rbnode),
          (unsigned long)sizeof(rbtree_map::node),
          (unsigned long)sizeof(rbtree_set::node));
   printf("sizeof rbtree_map %lu, rbtree_set %lu, rbtree_base %lu\n",
          (unsigned long)sizeof(ttl::rbtree_base),
          (unsigned long)sizeof(rbtree_map),
          (unsigned long)sizeof(rbtree_set));
   rbtree_map t;
   for (unsigned c = 10; c--; )
   {
      ttl::pair<int,char> d(c, -c);
      rbtree_map::node *pos = t.insert_equal(d);
      printf("%p %d (%d)\n", pos, d.first, d.second);
   }

   printf("red-black tree with 10 elements, root %p\n", t.get_croot());
   inorder<rbtree_map>(t.get_croot());

   ttl::pair<int,char> dupe(5, -5);
   assert(NULL == t.insert_unique(dupe));
   assert(NULL != t.insert_equal(dupe));

   printf("inserted dupe (5)\n");
   inorder<rbtree_map>(t.get_croot());

   delete static_cast<rbtree_map::node *>(t.delete_min(t.edge(t.get_root())));
   delete static_cast<rbtree_map::node *>(t.delete_min(t.edge(t.get_root())));
   delete static_cast<rbtree_map::node *>(t.delete_min(t.edge(t.get_root())));

   printf("3 calls to delete_min from root\n");
   inorder<rbtree_map>(t.get_croot());

   delete t.remove(7);
   printf("remove 7\n");
   inorder<rbtree_map>(t.get_croot());

   printf("find & lower_boundary\n");
   {
      rbtree_map::keyof_type keyof;
      rbtree_map::node *h = t.find(3);
      int k = h ? keyof(h->data): -1;
      printf("find(%d): %p(%d)\n", 3, h, k);
      assert(3 == k);

      h = t.lower_bound(3);
      k = h ? keyof(h->data): -1;
      printf("lower_bound(%d): %p(%d)\n", 3, h, k);
      assert(k >= 3);

      h = t.lower_bound(100);
      k = h ? keyof(h->data): -1;
      printf("lower_bound(%d): %p %d (out of range)\n", 100, h, k);
      assert(h == NULL);

      h = t.lower_bound(5);
      k = h ? keyof(h->data): -1;
      printf("lower_bound(%d): %p %d (duplicate)\n", 5, h, k);
      assert(k >= 5);

      h = t.lower_bound(7);
      k = h ? keyof(h->data): -1;
      printf("lower_bound(%d): %p %d (removed)\n", 7, h, k);
      assert(k >= 7);

      h = t.upper_bound(7);
      k = h ? keyof(h->data): -1;
      printf("upper_bound(%d): %p %d (removed)\n", 7, h, k);
      assert(k > 7);

      h = t.upper_bound(5);
      k = h ? keyof(h->data): -1;
      printf("upper_bound(%d): %p %d\n", 5, h, k);
      assert(k > 5);

      h = t.upper_bound(100);
      k = h ? keyof(h->data): -1;
      printf("upper_bound(%d): %p %d (out of range)\n", 100, h, k);
      assert(h == NULL);

      ttl::pair<rbtree_map::node *,rbtree_map::node *> r = t.equal_range(5);
      k = r.first ? keyof(r.first->data): -1;
      printf("equal_range(%d): %p(%d) %p\n", 5, r.first, k, r.second);
      assert(keyof(r.first->data) == 5);
      assert(r.first != r.second);

      printf("count(%d): %lu\n", 5, (unsigned long)t.count(5));
      assert(2 == t.count(5));
   }

   rbtree_set s;
   s.insert_unique(1);
   s.insert_unique(2);
   s.insert_unique(3);
   s.insert_unique(3);
   printf("set of three elements\n");
   inorder<rbtree_set>(s.get_croot());

   delete s.remove(1);
   printf("set of two elements\n");
   inorder<rbtree_set>(s.get_croot());
}

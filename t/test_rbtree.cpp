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
static void inorder(const ttl::rbnode *n, bool print_pointer = false, int depth = 0)
{
   if (n && n->left)
      inorder<Container>(n->left, print_pointer, depth + 1);
   for (int i = depth; i--;)
      fputc(' ', stdout);
   if (print_pointer)
      printf("%p:", n);
   typename Container::keyof_type keyof;
   printf("%d\n", n ? keyof(static_cast<const typename Container::node *>(n)->data): -1);
   if (n && n->right)
      inorder<Container>(n->right, print_pointer, depth + 1);
}

void test()
{
   printf("sizeof rbnode %lu, rbtree_map::node %lu, rbtree_set::node %lu\n",
          (unsigned long)sizeof(ttl::rbnode),
          (unsigned long)sizeof(rbtree_map::node),
          (unsigned long)sizeof(rbtree_set::node));
   printf("sizeof rbtree_map %lu, rbtree_set %lu, rbtree_base %lu\n",
          (unsigned long)sizeof(rbtree_map),
          (unsigned long)sizeof(rbtree_set),
          (unsigned long)sizeof(ttl::rbtree_base));
   {
      printf("empty rbtree construction and destruction\n");
      rbtree_map();
      rbtree_set();
   }

   rbtree_map::keyof_type keyof;
   rbtree_map t;
   for (unsigned c = 10; c--; )
   {
      ttl::pair<int,char> d(c, -c);
      rbtree_map::node *n = t.insert_equal(d);
      printf("%p %d (%d)\n", n, d.first, d.second);
   }

   printf("generated red-black tree with 10 unique elements, root %p\n", t.get_croot());

   ttl::pair<int,char> dupe(5, -5);
   assert(t.insert_unique(dupe).second == false);
   assert(keyof(t.insert_unique(dupe).first->data) == 5);
   assert(t.end() != t.insert_equal(dupe));

   printf("inserted dupe (5)\n");
   inorder<rbtree_map>(t.get_croot(), true);

   printf("rbtree_base::min_node() and ...::next_node():\n");
   {
      ttl::rbnode *i = ttl::rbtree_base::min_node(t.get_root());
      while (i != t.end())
      {
         printf(" %d", keyof(static_cast<rbtree_map::node *>(i)->data));
         i = ttl::rbtree_base::next_node(i);
      }
      printf("\n");
   }
   printf("rbtree_base::max_node() and ...::prev_node():\n");
   {
      ttl::rbnode *i = ttl::rbtree_base::max_node(t.get_root());
      while (i != t.end())
      {
         printf(" %d", keyof(static_cast<rbtree_map::node *>(i)->data));
         i = ttl::rbtree_base::prev_node(i);
      }
      printf("\n");
   }

   delete static_cast<rbtree_map::node *>(t.delete_min(t.edge(t.get_root())));
   delete static_cast<rbtree_map::node *>(t.delete_min(t.edge(t.get_root())));

   printf("2 calls to delete_min from root\n");
   inorder<rbtree_map>(t.get_croot());

   delete t.remove(2);
   printf("remove 2 (leaf)\n");
   inorder<rbtree_map>(t.get_croot());

   delete t.remove(7);
   printf("remove 7\n");
   inorder<rbtree_map>(t.get_croot());

   rbtree_map::node *six = t.remove(6);
   printf("remove 6 (root) and put its values back\n");
   assert(t.insert_unique(six->data).second != false);
   delete six;
   inorder<rbtree_map>(t.get_croot());

   printf("assign\n");
   {
      rbtree_map tmp;
      tmp.assign(t);
      inorder<rbtree_map>(tmp.get_croot());
   }

   printf("find & lower_boundary\n");
   {
      rbtree_map::node *n = t.find(3);
      int k = n ? keyof(n->data): -1;
      printf("find(%d): %p(%d)\n", 3, n, k);
      assert(3 == k);

      n = t.lower_bound(3);
      k = n ? keyof(n->data): -1;
      printf("lower_bound(%d): %p(%d)\n", 3, n, k);
      assert(k >= 3);

      n = t.lower_bound(100);
      k = n ? keyof(n->data): -1;
      printf("lower_bound(%d): %p %d (out of range)\n", 100, n, k);
      assert(n == t.end());

      n = t.lower_bound(5);
      k = n ? keyof(n->data): -1;
      printf("lower_bound(%d): %p %d (duplicate)\n", 5, n, k);
      assert(k >= 5);

      n = t.lower_bound(7);
      k = n ? keyof(n->data): -1;
      printf("lower_bound(%d): %p %d (removed)\n", 7, n, k);
      assert(k >= 7);

      n = t.upper_bound(7);
      k = n ? keyof(n->data): -1;
      printf("upper_bound(%d): %p %d (removed)\n", 7, n, k);
      assert(k > 7);

      n = t.upper_bound(5);
      k = n ? keyof(n->data): -1;
      printf("upper_bound(%d): %p %d\n", 5, n, k);
      assert(k > 5);

      n = t.upper_bound(100);
      k = n ? keyof(n->data): -1;
      printf("upper_bound(%d): %p %d (out of range)\n", 100, n, k);
      assert(n == t.end());

      ttl::pair<rbtree_map::node *,rbtree_map::node *> r = t.equal_range(5);
      k = r.first ? keyof(r.first->data): -1;
      printf("equal_range(%d): %p(%d) %p\n", 5, r.first, k, r.second);
      assert(keyof(r.first->data) == 5);
      assert(r.first != r.second);
      for (ttl::rbnode *i = r.first; i != r.second; i = ttl::rbtree_base::next_node(i))
         assert(5 == keyof(static_cast<rbtree_map::node *>(i)->data));

      printf("count(%d): %lu (not existing)\n", -1, (unsigned long)t.count(-1));
      assert(0 == t.count(-1));
      printf("count(%d) (duplicate): %lu\n", 5, (unsigned long)t.count(5));
      assert(2 == t.count(5));
      printf("count(%d): %lu\n", 8, (unsigned long)t.count(8));
      assert(1 == t.count(8));
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

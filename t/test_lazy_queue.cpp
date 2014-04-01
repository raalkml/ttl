// vim: sw=3 ts=8 et
#include "ttl/utility.hpp"
#include "ttl/lazy_queue.hpp"
#include "t.hpp"

#define TRIVIAL_TEST_TYPE 0
#if TRIVIAL_TEST_TYPE == 0
#define SIZE_TEST_TYPE testtype
#else
#define SIZE_TEST_TYPE int
#endif

struct lazy_queue_int
{
   struct node
   {
      node *next;
      SIZE_TEST_TYPE v;
   };
   node *head, **tail, *dead;

   lazy_queue_int(): head(NULL), tail(&head), dead(NULL) {}

   ~lazy_queue_int()
   {
      while (head)
      {
         node *n = head;
         head = head->next;
         delete n;
      }
      while (dead)
      {
         node *n = dead;
         dead = dead->next;
         ::operator delete(n);
      }
   }
   node *get_node(const SIZE_TEST_TYPE &v)
   {
      node *n = dead ? dead: static_cast<node *>(::operator new(sizeof(node)));
      if (dead)
         dead = dead->next;
      ::new(&n->v) SIZE_TEST_TYPE(v);
      return n;
   }
   void put_node(node *n)
   {
#if TRIVIAL_TEST_TYPE == 0
      n->v.~SIZE_TEST_TYPE();
#endif
      n->next = dead;
      dead = n;
   }
   void push_back(const SIZE_TEST_TYPE &v)
   {
      node *n = get_node(v);
      n->next = NULL;
      *tail = n;
      tail = &n->next;
   }
   void pop_front()
   {
      node *n = head;
      head = head->next;
      if (!head)
         tail = &head;
      put_node(n);
   }
};

void test()
{
   ttl::lazy_queue<testtype> lq;
   printf("lazy_queue %s\n", lq.empty() ? "empty": "not empty");
   lq.push_front(1);
   print_iter("lazy_queue: push_front: ", lq.cbegin(), lq.cend());
   printf("lazy_queue: front %d\n", lq.front().value);
   printf("lazy_queue: back %d\n", lq.back().value);
   lq.pop_front();
   printf("lazy_queue: pop_front: %s\n", lq.empty() ? "empty": "not empty");
   lq.push_back(2);
   lq.push_back(999);
   print_iter("lazy_queue: push_back: ", lq.cbegin(), lq.cend());
   lq.pop_front();
   static const int nums[] = { 1, 2,3,4,5,};
   lq.insert_after(lq.cbefore_end(), nums, nums + countof(nums));
   print_iter("lazy_queue: insert_after(pos, first, last):", lq.cbegin(), lq.cend());
   lq.roll_head();
   print_iter("lazy_queue: roll_head:", lq.cbegin(), lq.cend());
   ttl::lazy_queue<testtype> lq1, lq2;
   lq1 = lq;
   print_iter("lazy_queue: operator=:", lq1.cbegin(), lq1.cend());
   lq2.swap(lq1);
   print_iter("lazy_queue: swap:", lq2.cbegin(), lq2.cend());
   lq2.erase_after(lq2.cbegin());
   print_iter("lazy_queue: erase_after(pos):", lq2.cbegin(), lq2.cend());
   lq2.erase_after(lq2.cbegin(), ++ ++ ++ ++ lq2.cbegin());
   print_iter("lazy_queue: erase_after(first, last):", lq2.cbegin(), lq2.cend());

#if 0
   ttl::lazy_queue<SIZE_TEST_TYPE> lqi_t;
   lqi_t.push_back(1);
   lqi_t.push_back(2);
   lqi_t.push_back(3);
   lqi_t.push_back(4);
   lqi_t.pop_front();
   lqi_t.pop_front();
   lqi_t.pop_front();
   lqi_t.pop_front();
   lqi_t.push_back(3);
   lqi_t.push_back(4);
   lqi_t.push_back(4);
   lqi_t.push_back(4);
   lqi_t.push_back(4);
#endif
#if 1
   lazy_queue_int lqi_p;
   lqi_p.push_back(1);
   lqi_p.push_back(2);
   lqi_p.push_back(3);
   lqi_p.push_back(4);
   lqi_p.pop_front();
   lqi_p.pop_front();
   lqi_p.pop_front();
   lqi_p.pop_front();
   lqi_p.push_back(3);
   lqi_p.push_back(4);
   lqi_p.push_back(4);
   lqi_p.push_back(4);
   lqi_p.push_back(4);
#endif
}

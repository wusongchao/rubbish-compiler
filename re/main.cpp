//#include <iostream>
//#include "FixedMemoryPool.h"
//#include <cassert>
//
//using std::cin;
//using std::cout;
//using std::endl;
//
//struct A {
//    int x;
//    int y;
//
//    A(int x, int y)
//        :x(x), y(y)
//    {
//
//    }
//    
//    ~A()
//    {
//        cout << "destructor call" << endl;
//    }
//};
//
//void test1()
//{
//    FixedMemoryPool<A> pool;
//    A* a1 = pool.newElement(1, 2);
//    cout << a1->x << ' ' << a1->y << endl;
//}
//
//void test2()
//{
//    FixedMemoryPool<A> pool(2);
//
//    constexpr int n = 3;
//    A* ptrs[n];
//    for (int i = 0; i < n; i++) {
//        ptrs[i] = pool.newElement(i, i + 1);
//    }
//
//    for (int i = 0; i < n; i++) {
//        A* p = ptrs[i];
//        assert((p->x == i) && (p->y == i+1));
//    }
//}
//
//int main()
//{
//    test1();
//    test2();
//    cin.get();
//    return 0;
//}
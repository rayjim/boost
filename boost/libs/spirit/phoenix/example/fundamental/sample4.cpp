#include <vector>
#include <algorithm>
#include <iostream>
#include "boost/spirit/phoenix/operators.hpp"
#include "boost/spirit/phoenix/primitives.hpp"
#include "boost/spirit/phoenix/statements.hpp"
#include "boost/spirit/phoenix/special_ops.hpp"

using namespace std;
using namespace phoenix;

int
main()
{
    int init[] = { 2, 10, 4, 5, 1, 6, 8, 3, 9, 7 };
    vector<int> c(init, init + 10);
    typedef vector<int>::iterator iterator;

    //  Print all odd contents of an stl container c
    for_each(c.begin(), c.end(),
        if_(arg1 % 2 == 1)
        [
            cout << arg1 << ' '
        ]
    );

    return 0;
}

// File: test3.cpp

#include "boost/test/minimal.hpp"
#include "boost/variant.hpp"
#include "boost/incomplete.hpp"

#include <iostream>
#include <sstream>
#include <string>

/////////////////////////////////////////////////////////////////////

using boost::variant;
using boost::incomplete;
using std::cout;
using std::endl;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

struct Add;
struct Sub;

typedef variant<int, incomplete<Add>, incomplete<Sub> > Expr;

struct Sub
{
   Sub();
   Sub(const Expr& l, const Expr& r);
   Sub(const Sub& other);

   Expr lhs_;
   Expr rhs_;
};

struct Add
{
   Add() { }
   Add(const Expr& l, const Expr& r) : lhs_(l), rhs_(r) { }
   Add(const Add& other) : lhs_(other.lhs_), rhs_(other.rhs_) { }

   Expr lhs_;
   Expr rhs_;
};

Sub::Sub() { }
Sub::Sub(const Expr& l, const Expr& r) : lhs_(l), rhs_(r) { }
Sub::Sub(const Sub& other) : lhs_(other.lhs_), rhs_(other.rhs_) { }


//
// insert-to operators
//
std::ostream& operator<<(std::ostream& out, const Sub& a);

std::ostream& operator<<(std::ostream& out, const Add& a)
{
   out << '(' << a.lhs_ << '+' << a.rhs_ << ')';
   return out;
}

std::ostream& operator<<(std::ostream& out, const Sub& a)
{
   out << '(' << a.lhs_ << '-' << a.rhs_ << ')';
   return out;
}

//
// Expression evaluation visitor
//
struct Calculator : boost::static_visitor<int>
{
   Calculator()  { }

   int operator()(Add& x) const
   {
      Calculator calc;
      int n1 = boost::apply_visitor(calc, x.lhs_);
      int n2 = boost::apply_visitor(calc, x.rhs_);
      
      return n1 + n2;
   }

   int operator()(Sub& x) const
   {
      return boost::apply_visitor(Calculator(), x.lhs_) 
         - boost::apply_visitor(Calculator(), x.rhs_);
   }

   int operator()(Expr& x) const
   {
      Calculator calc;
      return boost::apply_visitor(calc, x);
   }

   int operator()(int x) const
   {
      return x;
   }

}; // Calculator


/////////////////////////////////////////////////////////////////////


int test_main(int, char* [])
{

   int n = 13;
   Expr e1( Add(n, Sub(Add(40,2),Add(10,4))) ); //n + (40+2)-(10+14) = n+28

   std::ostringstream e1_str;
   e1_str << e1;

   BOOST_CHECK(e1.type() == typeid(Add));
   BOOST_CHECK(e1_str.str() == "(13+((40+2)-(10+4)))");

   //Evaluate expression
   int res = boost::apply_visitor(Calculator(), e1);
   BOOST_CHECK(res == n + 28);

   return 0;
}


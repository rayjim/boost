// (C) Copyright Jeremy Siek 2001. 
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all
// copies. This software is provided "as is" without express or
// implied warranty, and with no claim as to its suitability for any
// purpose.

#include <vector>
#include <algorithm> // for std::min
#include <fstream>
#include <boost/test/test_tools.hpp>

// Extract the bit at position n from num.
template <typename Block>
inline bool nth_bit(Block num, std::size_t n)
{
  // Move the nth bit to position 0 and then clear all other bits.
  return (num >> n) & 1;           
}
inline unsigned long max_num(std::size_t num_bits)
{  
  return (unsigned long)(std::pow((double)2, (double)num_bits)); 
}


// constructors
//   default (can't do this generically)

//   from unsigned long

template <typename Bitset>
struct bitset_test {

  static void from_unsigned_long(Bitset b, unsigned long num)
  {
    // initializes the first M bit position to the cooresponding bit
    // values in val. M is the smaller of N and the value CHAR_BIT *
    // sizeof(unsigned long)

    // missing from the std?
    //   if M < N then the remaining bit positions are initialized to zero

    std::size_t N = b.size();
    std::size_t M = std::min(N, CHAR_BIT * sizeof(unsigned long));
    std::size_t I;
    for (I = 0; I < M; ++I)
      BOOST_TEST(b[I] == nth_bit(num, I));
    for (; I < N; ++I)
      BOOST_TEST(b[I] == 0);
  }

  //   from string
  static void from_string(const std::string& str, std::size_t pos,
                          std::size_t n)
  {
    if (pos > str.size()) {
      try {
        Bitset b(str, pos, n);
        BOOST_TEST(false); // constructor failed to throw
      } catch (std::out_of_range) {
        // Good!
      } catch (...) {
        BOOST_TEST(false); // constructor threw the wrong exception
      }
    } else {
      std::size_t rlen = std::min(n, str.size() - pos);

      // Throws invalid_argument if any of the rlen characters in str
      // beginning at position pos is other than 0 or 1.
      bool any_non_zero_or_one = false;
      for (std::size_t i = pos; i < pos + rlen; ++i)
        if (! (str[i] == '0' || str[i] == '1'))
          any_non_zero_or_one = true;
      if (any_non_zero_or_one) {
        try {
          Bitset b(str, pos, n);
          BOOST_TEST(false); // constructor failed to throw
        } catch (std::invalid_argument) {
          // Good!
        } catch (...) {
          BOOST_TEST(false); // constructor threw the wrong exception
        }
      } else {
        // Construct an object, initializing the first M bit position to
        // values determined from the corresponding characters in the
        // str. M is the smaller of N and rlen.  Character position pos
        // + M - 1 corresponds to bit position zero.  Subsequent
        // decreasing character position correspond to increasing bit
        // positions.

        Bitset b(str, pos, n);
        std::size_t N = b.size();
        std::size_t M = std::min(N, rlen);
        std::size_t j;
        for (j = 0; j < M; ++j)
          BOOST_TEST(b[j] == (str[pos + M - 1 - j] == '1'));
        // If M < N, remaining bit positions are initialize to zero
        for (; j < N; ++j)
          BOOST_TEST(b[j] == 0);
      }
    }
  }

  typedef typename Bitset::block_type Block;

  // PRE: std::equal(first1, last1, first2) == true
  static void from_block_range(std::vector<Block> blocks)
  {
    Bitset bset(blocks.begin(), blocks.end());
    std::size_t n = blocks.size();
    for (std::size_t b = 0; b < n; ++b) {
      for (std::size_t i = 0; i < sizeof(Block) * CHAR_BIT; ++i) {
        std::size_t bit = b * sizeof(Block) * CHAR_BIT + i;
        BOOST_TEST(bset[bit] == nth_bit(blocks[b], i));
      }
    }
  }

  // copy constructor (absent from std::bitset)
  static void copy_constructor(const Bitset& b)
  {
    Bitset copy(b);
    BOOST_TEST(b == copy);

    // Changes to the copy do not affect the original
    if (b.size() > 0) {
      std::size_t pos = copy.size() / 2;
      copy.flip(pos);
      BOOST_TEST(copy[pos] != b[pos]);
    }
  }

  // assignment operator (absent from std::bitset)
  static void assignment_operator(const Bitset& lhs, const Bitset& rhs)
  {
    Bitset b(lhs);
    b = rhs;
    BOOST_TEST(b == rhs);
    
    // Changes to the copy do not affect the original
    if (b.size() > 0) {
      std::size_t pos = b.size() / 2;
      b.flip(pos);
      BOOST_TEST(b[pos] != rhs[pos]);
    }
  }

  static void resize(const Bitset& lhs)
  {
    Bitset b(lhs);

    // Test no change in size
    b.resize(lhs.size());
    BOOST_TEST(b == lhs);

    // Test increase in size
    b.resize(lhs.size() * 2, true);

    std::size_t i;
    for (i = 0; i < lhs.size(); ++i)
      BOOST_TEST(b[i] == lhs[i]);
    for (; i < b.size(); ++i)
      BOOST_TEST(b[i] == true);

    // Test decrease in size
    b.resize(lhs.size());
    for (i = 0; i < lhs.size(); ++i)
      BOOST_TEST(b[i] == lhs[i]);
  }

  static void clear(const Bitset& lhs)
  {
    Bitset b(lhs);
    b.clear();
    BOOST_TEST(b.size() == 0);
    try {
      bool x = b[0];
      b[0] = x;
    } catch(std::out_of_range) {
      // Good!
    }
  }

  static void append_bit(const Bitset& lhs)
  {
    Bitset b(lhs);
    b.push_back(true);
    BOOST_TEST(b.size() == lhs.size() + 1);
    BOOST_TEST(b[b.size() - 1] == true);
    for (std::size_t i = 0; i < lhs.size(); ++i)
      BOOST_TEST(b[i] == lhs[i]);
    
    b.push_back(false);
    BOOST_TEST(b.size() == lhs.size() + 2);
    BOOST_TEST(b[b.size() - 1] == false);
    BOOST_TEST(b[b.size() - 2] == true);
    for (std::size_t i = 0; i < lhs.size(); ++i)
      BOOST_TEST(b[i] == lhs[i]);
  }

  static void append_block(const Bitset& lhs)
  {
    Bitset b(lhs);
    Block value(128);
    b.append(value);
    BOOST_TEST(b.size() == lhs.size() + Bitset::block_size);
    for (std::size_t i = 0; i < Bitset::block_size; ++i)
      BOOST_TEST(b[lhs.size() + i] == bool((value >> i) & 1));
  }
  
  static void append_block_range(const Bitset& lhs, std::vector<Block> blocks)
  {
    Bitset b(lhs), c(lhs);
    b.append(blocks.begin(), blocks.end());
    for (typename std::vector<Block>::iterator i = blocks.begin();
         i != blocks.end(); ++i)
      c.append(*i);
    BOOST_TEST(b == c);
  }

  // operator[] and reference members
  // PRE: b[i] == bit_vec[i]
  static void operator_bracket(const Bitset& lhs, const std::vector<bool>& bit_vec)
  {
    Bitset b(lhs);
    std::size_t i, j, k;

    // x = b[i]
    // x = ~b[i]
    for (i = 0; i < b.size(); ++i) {
      bool x = b[i];
      BOOST_TEST(x == bit_vec[i]);
      x = ~b[i];
      BOOST_TEST(x == !bit_vec[i]);
    }
    Bitset prev(b);

    // b[i] = x
    for (j = 0; j < b.size(); ++j) {
      bool x = !prev[j];
      b[j] = x;
      for (k = 0; k < b.size(); ++k)
        if (j == k)
          BOOST_TEST(b[k] == x);
        else
          BOOST_TEST(b[k] == prev[k]);
      b[j] = prev[j];
    }
    b.flip();

    // b[i] = b[j]
    for (i = 0; i < b.size(); ++i) {
      b[i] = prev[i];
      for (j = 0; j < b.size(); ++j) {
        if (i == j)
          BOOST_TEST(b[j] == prev[j]);
        else
          BOOST_TEST(b[j] == !prev[j]);
      }
      b[i] = !prev[i];
    }

    // b[i].flip()
    for (i = 0; i < b.size(); ++i) {
      b[i].flip();
      for (j = 0; j < b.size(); ++j) {
        if (i == j)
          BOOST_TEST(b[j] == prev[j]);
        else
          BOOST_TEST(b[j] == !prev[j]);
      }
      b[i].flip();
    }
  }

  //===========================================================================
  // bitwise operators

  // bitwise and assignment

  // PRE: b.size() == rhs.size()
  static void and_assignment(const Bitset& b, const Bitset& rhs)
  {
    Bitset lhs(b);
    Bitset prev(lhs);
    lhs &= rhs;
    // Clears each bit in lhs for which the corresponding bit in rhs is
    // clear, and leaves all other bits unchanged.
    for (std::size_t I = 0; I < lhs.size(); ++I)
      if (rhs[I] == 0)
        BOOST_TEST(lhs[I] == 0);
      else
        BOOST_TEST(lhs[I] == prev[I]);
  }

  // PRE: b.size() == rhs.size()
  static void or_assignment(const Bitset& b, const Bitset& rhs)
  {
    Bitset lhs(b);
    Bitset prev(lhs);
    lhs |= rhs;
    // Sets each bit in lhs for which the corresponding bit in rhs is set, and 
    // leaves all other bits unchanged.
    for (std::size_t I = 0; I < lhs.size(); ++I)
      if (rhs[I] == 1)
        BOOST_TEST(lhs[I] == 1);
      else
        BOOST_TEST(lhs[I] == prev[I]);
  }

  // PRE: b.size() == rhs.size()
  static void xor_assignment(const Bitset& b, const Bitset& rhs)
  {
    Bitset lhs(b);    
    Bitset prev(lhs);
    lhs ^= rhs;
    // Flips each bit in lhs for which the corresponding bit in rhs is set,
    // and leaves all other bits unchanged.
    for (std::size_t I = 0; I < lhs.size(); ++I)
      if (rhs[I] == 1)
        BOOST_TEST(lhs[I] == !prev[I]);
      else
        BOOST_TEST(lhs[I] == prev[I]);
  }

  static void shift_left_assignment(const Bitset& b, std::size_t pos)
  {
    Bitset lhs(b);    
    Bitset prev(lhs);
    lhs <<= pos;
    // Replaces each bit at position I in lhs with the following value:
    // - If I < pos, the new value is zero
    // - If I >= pos, the new value is the previous value of the bit at 
    //   position I - pos
    for (std::size_t I = 0; I < lhs.size(); ++I)
      if (I < pos)
        BOOST_TEST(lhs[I] == 0);
      else
        BOOST_TEST(lhs[I] == prev[I - pos]);
  }

  static void shift_right_assignment(const Bitset& b, std::size_t pos)
  {
    Bitset lhs(b);
    Bitset prev(lhs);
    lhs >>= pos;
    // Replaces each bit at position I in lhs with the following value:
    // - If pos >= N - I, the new value is zero
    // - If pos < N - I, the new value is the previous value of the bit at 
    //    position I + pos
    std::size_t N = lhs.size();
    for (std::size_t I = 0; I < N; ++I)
      if (pos >= N - I)
        BOOST_TEST(lhs[I] == 0);
      else
        BOOST_TEST(lhs[I] == prev[I + pos]);
  }


  static void set_all(const Bitset& b)
  {
    Bitset lhs(b);
    lhs.set();
    for (std::size_t I = 0; I < lhs.size(); ++I)
      BOOST_TEST(lhs[I] == 1);
  }

  static void set_one(const Bitset& b, std::size_t pos, bool value)
  {
    Bitset lhs(b);
    std::size_t N = lhs.size();
    if (pos < N) {
      Bitset prev(lhs);
      // Stores a new value in the bit at position pos in lhs.
      lhs.set(pos, value);
      BOOST_TEST(lhs[pos] == value);

      // All other values of lhs remain unchanged
      for (std::size_t I = 0; I < N; ++I)
        if (I != pos)
          BOOST_TEST(lhs[I] == prev[I]);
    } else {
      try {
        lhs.set(pos, value);
      } catch (std::out_of_range) {
        // Good!
      } catch (...) {
        BOOST_TEST(false); // threw the wrong exception
      }
    }
  }

  static void reset_all(const Bitset& b)
  {
    Bitset lhs(b);
    // Resets all bits in lhs
    lhs.reset();
    for (std::size_t I = 0; I < lhs.size(); ++I)
      BOOST_TEST(lhs[I] == 0);
  }

  static void reset_one(const Bitset& b, std::size_t pos)
  {
    Bitset lhs(b);
    std::size_t N = lhs.size();
    if (pos < N) {
      Bitset prev(lhs);
      lhs.reset(pos);
      // Resets the bit at position pos in lhs
      BOOST_TEST(lhs[pos] == 0);

      // All other values of lhs remain unchanged
      for (std::size_t I = 0; I < N; ++I)
        if (I != pos)
          BOOST_TEST(lhs[I] == prev[I]);
    } else {
      try {
        lhs.reset(pos);
        BOOST_TEST(false); // It should have thrown and exception
      } catch (std::out_of_range) {
        // Good!
      } catch (...) {
        BOOST_TEST(false); // threw the wrong exception
      }
    }
  }

  static void operator_flip(const Bitset& b)
  {
    Bitset lhs(b);
    Bitset x(lhs);
    BOOST_TEST(~lhs == x.flip());
  }

  static void flip_all(const Bitset& b)
  {
    Bitset lhs(b);
    std::size_t N = lhs.size();
    Bitset prev(lhs);
    lhs.flip();
    // Toggles all the bits in lhs 
    for (std::size_t I = 0; I < N; ++I)
      BOOST_TEST(lhs[I] == !prev[I]);
  }

  static void flip_one(const Bitset& b, std::size_t pos)
  {
    Bitset lhs(b);
    std::size_t N = lhs.size();
    if (pos < N) {
      Bitset prev(lhs);
      lhs.flip(pos);
      // Toggles the bit at position pos in lhs
      BOOST_TEST(lhs[pos] == !prev[pos]);

      // All other values of lhs remain unchanged
      for (std::size_t I = 0; I < N; ++I)
        if (I != pos)
          BOOST_TEST(lhs[I] == prev[I]);
    } else {
      try {
        lhs.flip(pos);
        BOOST_TEST(false); // It should have thrown and exception
      } catch (std::out_of_range) {
        // Good!
      } catch (...) {
        BOOST_TEST(false); // threw the wrong exception
      }
    }
  }

  // to_ulong()
  static void to_ulong(const Bitset& lhs)
  {
    std::size_t N = lhs.size();
    std::size_t n = CHAR_BIT * sizeof(unsigned long);
    bool will_overflow = false;
    for (std::size_t I = n; I < N; ++I)
      if (lhs[I] != 0)
        will_overflow = true;
    if (will_overflow) {
      try {
        (void)lhs.to_ulong();
        BOOST_TEST(false); // It should have thrown and exception
      } catch (std::overflow_error) {
        // Good!
      } catch (...) {
        BOOST_TEST(false); // threw the wrong exception
      }
    } else {
      unsigned long num = lhs.to_ulong();
      // Make sure the number is right
      for (std::size_t I = 0; I < N; ++I)
        BOOST_TEST(lhs[I] == nth_bit(num, I));
    }
  }

  // to_string()
  static void to_string(const Bitset& b)
  {
    // Construct a string object of the appropriate type and initializes
    // it to a string of length N characters. Each character is determined
    // by the value of its corresponding bit position in b. Character
    // position N - 1 corresponds to bit position zero. Sebsequent
    // decreasing character positions correspond to increasing bit
    // positions. Bit value zero becomes the charactet 0, bit value one
    // becomes the character 1.
    std::string str;
    boost::to_string(b, str);
    std::size_t N = b.size();
    BOOST_TEST(str.size() == b.size());
    for (std::size_t I = 0; I < b.size(); ++I)
      BOOST_TEST(b[I] == 0 ? (str[N - 1 - I] == '0') : (str[N - 1 - I] == '1'));
  }

  static void count(const Bitset& b)
  {
    std::size_t c = b.count();
    std::size_t c_real = 0;
    for (std::size_t I = 0; I < b.size(); ++I)
      if (b[I])
        ++c_real;
    BOOST_TEST(c == c_real);
  }

  static void size(const Bitset& b)
  {
    BOOST_TEST(Bitset(b).set().count() == b.size());
  }

  static void any(const Bitset& b)
  {
    BOOST_TEST(b.any() == (b.count() > 0));
  }

  static void none(const Bitset& b)
  {
    BOOST_TEST(b.none() == (b.count() == 0));
  }

  static void operator_equal(const Bitset& a, const Bitset& b)
  {
    if (a == b) {
      for (std::size_t I = 0; I < a.size(); ++I)
        BOOST_TEST(a[I] == b[I]);
    } else {
      if (a.size() == b.size()) {
        bool diff = false;
        for (std::size_t I = 0; I < a.size(); ++I)
          if (a[I] != b[I]) {
            diff = true;
            break;
          }
        BOOST_TEST(diff);
      }
    }
  }

  static void operator_not_equal(const Bitset& a, const Bitset& b)
  {
    if (a != b) {
      if (a.size() == b.size()) {
        bool diff = false;
        for (std::size_t I = 0; I < a.size(); ++I)
          if (a[I] != b[I]) {
            diff = true;
            break;
          }
        BOOST_TEST(diff);
      }
    } else {
      for (std::size_t I = 0; I < a.size(); ++I)
        BOOST_TEST(a[I] == b[I]);
    }
  }

  static bool less_than(const Bitset& a, const Bitset& b)
  {
    // Compare from most significant to least.
    // Careful, don't send unsigned int into negative territory!
    if (a.size() == 0)
      return false;

    std::size_t I;
    for (I = a.size() - 1; I > 0; --I)
      if (a[I] < b[I])
        return true;
      else if (a[I] > b[I])
        return false;
      // if (a[I] = b[I]) skip to next

    if (a[0] < b[0])
      return true;
    else
      return false;
  }
  
  static void operator_less_than(const Bitset& a, const Bitset& b)
  {
    if (less_than(a, b))
      BOOST_TEST(a < b);
    else
      BOOST_TEST(!(a < b));
  }

  static void operator_greater_than(const Bitset& a, const Bitset& b)
  {
    if (less_than(a, b) || a == b)
      BOOST_TEST(!(a > b));
    else
      BOOST_TEST(a > b);
  }

  static void operator_less_than_eq(const Bitset& a, const Bitset& b)
  {
    if (less_than(a, b) || a == b)
      BOOST_TEST(a <= b);
    else
      BOOST_TEST(!(a <= b));
  }

  static void operator_greater_than_eq(const Bitset& a, const Bitset& b)
  {
    if (less_than(a, b))
      BOOST_TEST(!(a >= b));
    else
      BOOST_TEST(a >= b);
  }

  static void test_bit(const Bitset& b, std::size_t pos)
  {
    Bitset lhs(b);
    std::size_t N = lhs.size();
    if (pos < N) {
      BOOST_TEST(lhs.test(pos) == lhs[pos]);
    } else {
      try {
        (void)lhs.test(pos);
        BOOST_TEST(false); // It should have thrown and exception
      } catch (std::out_of_range) {
        // Good!
      } catch (...) {
        BOOST_TEST(false); // threw the wrong exception
      }
    }
  }

  static void operator_shift_left(const Bitset& lhs, std::size_t pos)
  {
    Bitset x(lhs);
    BOOST_TEST((lhs << pos) == (x <<= pos));
  }

  static void operator_shift_right(const Bitset& lhs, std::size_t pos)
  {
    Bitset x(lhs);
    BOOST_TEST((lhs >> pos) == (x >>= pos));
  }

  // operator|
  static
  void operator_or(const Bitset& lhs, const Bitset& rhs)
  {
    Bitset x(lhs);
    BOOST_TEST((lhs | rhs) == (x |= rhs));
  }

  // operator&
  static
  void operator_and(const Bitset& lhs, const Bitset& rhs)
  {
    Bitset x(lhs);
    BOOST_TEST((lhs & rhs) == (x &= rhs));
  }

  // operator^
  static
  void operator_xor(const Bitset& lhs, const Bitset& rhs)
  {
    Bitset x(lhs);
    BOOST_TEST((lhs ^ rhs) == (x ^= rhs));
  }

  // operator<<(ostream,
  // operator>>(istream,

  static
  void stream_read_write(const Bitset& out, const Bitset& in)
  {
    Bitset x(in);
    {
      std::ofstream f("tmp");
      f << out;
    }
    {
      std::ifstream f("tmp");
      f >> x;
      BOOST_TEST(out == x);
    }
  }

};

#pragma once

/// dan: Unit test framework
//
namespace ku { namespace dan {

template <typename Pred>
struct Not
{
  template <typename T1, typename T2>
  bool operator()( const T1& expected, const T2& actual ) const
  {
    return !Pred()(expected, actual);
  }
};

template <typename Pred1, typename Pred2>
struct And
{
  template <typename T1, typename T2>
  bool operator()( const T1& expected, const T2& actual ) const
  {
    return Pred1()(expected, actual) && Pred2()(expected, actual);
  }
};

template <typename Pred1, typename Pred2>
struct Or
{
  template <typename T1, typename T2>
  bool operator()( const T1& expected, const T2& actual ) const
  {
    return Pred1()(expected, actual) || Pred2()(expected, actual);
  }
};


struct Eq
{
  template <typename T1, typename T2>
  bool operator()(const T1& expected, const T2& actual) const
  {
    return expected == actual;
  }
};

struct Lt
{
  template <typename T1, typename T2>
  bool operator()(const T1& expected, const T2& actual) const
  {
    return expected < actual;
  }
};

typedef Not<Eq>     Ne;
typedef Or<Lt, Eq>  Le;
typedef Not<Le>     Gt;
typedef Not<Lt>     Ge;



template <typename Pred, typename... Args>
bool expect(Args... args)
{
  if (!Pred()(args...))
    std::cout << "fail" << std::endl;
}

template <typename Expr>
bool expect(Expr expr)
{
  if (!expr)
    std::cout << "fail" << std::endl;
}

template <typename... Args>
bool assert(Args... args)
{
}

} } // namespace ku::dan


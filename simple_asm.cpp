#include <cstdint>
#include <typeinfo>
#include <iostream>
#include <tuple>

/*
start := instruction+ <EOF>;

instruction := push | pop | add | sub | mul | div;

push := 'push' int;

pop := 'pop' ;

add := 'add' ;

sub := 'sub' ;

mul := 'mul' ;

div := 'div' ;

*/

constexpr char user_code[] =
#include "user_code.txt"
;


// virtual cpu
struct cpu
{
  constexpr cpu() = default;
  static constexpr size_t stack_size = 0xff;
  size_t tos = 0;
  uint64_t stack[stack_size]={0};
  constexpr uint64_t pop(){ return stack[--tos]; }
  constexpr void push(const uint64_t val) { stack[tos++] = val; }
};




// text identifiers
namespace token
{
  struct push{};
  struct pop{};
  struct mul{};
  struct div{};
  struct add{};
  struct sub{};
  template <uint64_t _val> using num = std::integral_constant<uint64_t, _val>;
  template <typename ... _ts> using list = std::tuple<_ts...>;
}



//scanner generic form
template <size_t _len, const char (_str)[_len], size_t _pos=0, typename _token_list_t = token::list<>, char ... _chs> struct scanner;


//scanner terminator
template <size_t _len, const char (_str)[_len], typename ... _token_ts, char ... _chs>
struct scanner<_len, _str, _len, token::list<_token_ts...>, _chs...>
{
  using token_list_type = token::list<_token_ts...>;
};

//character appender
template <size_t _len, const char (_str)[_len], size_t _pos, typename ... _token_ts, char ... _chs>
struct scanner<_len, _str, _pos, token::list<_token_ts...>, _chs...>
  : scanner<_len, _str, 1+_pos, token::list<_token_ts...>, _chs..., _str[_pos]>{};

// carriage return skipper
template <size_t _len, const char (_str)[_len], size_t _pos, typename ... _token_ts>
struct scanner<_len, _str, _pos, token::list<_token_ts...>, '\r'>
  : scanner<_len, _str, 1+_pos, token::list<_token_ts...>,  _str[_pos]>{};

// space skipper
template <size_t _len, const char (_str)[_len], size_t _pos, typename ... _token_ts>
struct scanner<_len, _str, _pos, token::list<_token_ts...>, ' '>
  : scanner<_len, _str, 1+_pos, token::list<_token_ts...>,  _str[_pos]>{};

// newline skipper
template <size_t _len, const char (_str)[_len], size_t _pos, typename ... _token_ts>
struct scanner<_len, _str, _pos, token::list<_token_ts...>, '\n'>
  : scanner<_len, _str, 1+_pos, token::list<_token_ts...>,  _str[_pos]>{};

// tab skipper
template <size_t _len, const char (_str)[_len], size_t _pos, typename ... _token_ts>
struct scanner<_len, _str, _pos, token::list<_token_ts...>, '\t'>
  : scanner<_len, _str, 1+_pos, token::list<_token_ts...>,  _str[_pos]>{};

// push
template <size_t _len, const char (_str)[_len], size_t _pos, typename ... _token_ts>
struct scanner<_len, _str, _pos, token::list<_token_ts...>, 'p', 'u', 's', 'h'>
  : scanner<_len, _str, 1+_pos, token::list<_token_ts..., token::push>,  _str[_pos]>{};

// pop
template <size_t _len, const char (_str)[_len], size_t _pos, typename ... _token_ts>
struct scanner<_len, _str, _pos, token::list<_token_ts...>, 'p', 'o', 'p'>
  : scanner<_len, _str, 1+_pos, token::list<_token_ts..., token::pop>,  _str[_pos]>{};

// add
template <size_t _len, const char (_str)[_len], size_t _pos, typename ... _token_ts>
struct scanner<_len, _str, _pos, token::list<_token_ts...>, 'a', 'd', 'd'>
  : scanner<_len, _str, 1+_pos, token::list<_token_ts..., token::add>,  _str[_pos]>{};

// sub
template <size_t _len, const char (_str)[_len], size_t _pos, typename ... _token_ts>
struct scanner<_len, _str, _pos, token::list<_token_ts...>, 's', 'u', 'b'>
  : scanner<_len, _str, 1+_pos, token::list<_token_ts..., token::sub>,  _str[_pos]>{};

// mul
template <size_t _len, const char (_str)[_len], size_t _pos, typename ... _token_ts>
struct scanner<_len, _str, _pos, token::list<_token_ts...>, 'm', 'u', 'l'>
  : scanner<_len, _str, 1+_pos, token::list<_token_ts..., token::mul>,  _str[_pos]>{};

// div
template <size_t _len, const char (_str)[_len], size_t _pos, typename ... _token_ts>
struct scanner<_len, _str, _pos, token::list<_token_ts...>, 'd', 'i', 'v'>
  : scanner<_len, _str, 1+_pos, token::list<_token_ts..., token::div>,  _str[_pos]>{};


// 0
template <size_t _len, const char (_str)[_len], size_t _pos, typename ... _token_ts>
struct scanner<_len, _str, _pos, token::list<_token_ts...>, '0'>
  : scanner<_len, _str, 1+_pos, token::list<_token_ts..., token::num<0>>,  _str[_pos]>{};

// 1
template <size_t _len, const char (_str)[_len], size_t _pos, typename ... _token_ts>
struct scanner<_len, _str, _pos, token::list<_token_ts...>, '1'>
  : scanner<_len, _str, 1+_pos, token::list<_token_ts..., token::num<1>>,  _str[_pos]>{};

// 2
template <size_t _len, const char (_str)[_len], size_t _pos, typename ... _token_ts>
struct scanner<_len, _str, _pos, token::list<_token_ts...>, '2'>
  : scanner<_len, _str, 1+_pos, token::list<_token_ts..., token::num<2>>,  _str[_pos]>{};

// 3
template <size_t _len, const char (_str)[_len], size_t _pos, typename ... _token_ts>
struct scanner<_len, _str, _pos, token::list<_token_ts...>, '3'>
  : scanner<_len, _str, 1+_pos, token::list<_token_ts..., token::num<3>>,  _str[_pos]>{};

// 4
template <size_t _len, const char (_str)[_len], size_t _pos, typename ... _token_ts>
struct scanner<_len, _str, _pos, token::list<_token_ts...>, '4'>
  : scanner<_len, _str, 1+_pos, token::list<_token_ts..., token::num<4>>,  _str[_pos]>{};

// 5
template <size_t _len, const char (_str)[_len], size_t _pos, typename ... _token_ts>
struct scanner<_len, _str, _pos, token::list<_token_ts...>, '5'>
  : scanner<_len, _str, 1+_pos, token::list<_token_ts..., token::num<5>>,  _str[_pos]>{};

// 6
template <size_t _len, const char (_str)[_len], size_t _pos, typename ... _token_ts>
struct scanner<_len, _str, _pos, token::list<_token_ts...>, '6'>
  : scanner<_len, _str, 1+_pos, token::list<_token_ts..., token::num<6>>,  _str[_pos]>{};

// 7
template <size_t _len, const char (_str)[_len], size_t _pos, typename ... _token_ts>
struct scanner<_len, _str, _pos, token::list<_token_ts...>, '7'>
  : scanner<_len, _str, 1+_pos, token::list<_token_ts..., token::num<7>>,  _str[_pos]>{};

// 8
template <size_t _len, const char (_str)[_len], size_t _pos, typename ... _token_ts>
struct scanner<_len, _str, _pos, token::list<_token_ts...>, '8'>
  : scanner<_len, _str, 1+_pos, token::list<_token_ts..., token::num<8>>,  _str[_pos]>{};

// 9
template <size_t _len, const char (_str)[_len], size_t _pos, typename ... _token_ts>
struct scanner<_len, _str, _pos, token::list<_token_ts...>, '9'>
  : scanner<_len, _str, 1+_pos, token::list<_token_ts..., token::num<9>>,  _str[_pos]>{};


// generic form
template <typename _token_list_t> struct compiler;


// specialization for exhausted token::list
template <> struct compiler<token::list<>>{
  static constexpr uint64_t eval(cpu& oCPU){
    return (oCPU.pop());
  }
};

// push specialization
template <int _val, typename ... _tail_ts>
struct compiler<token::list<token::push, token::num<_val>, _tail_ts...>>
{
  static constexpr uint64_t eval(cpu& oCPU){
    oCPU.push(_val);
    return compiler<token::list<_tail_ts...>>::eval(oCPU);
  }
};

// pop
template <typename ... _tail_ts>
struct compiler<token::list<token::pop, _tail_ts...>>
{
  static constexpr uint64_t eval(cpu& oCPU){
    oCPU.pop();
    return compiler<token::list<_tail_ts...>>::eval(oCPU);
  }
};

// add
template <typename ... _tail_ts>
struct compiler<token::list<token::add, _tail_ts...>>
{
  static constexpr uint64_t eval(cpu& oCPU){
    oCPU.push( oCPU.pop() + oCPU.pop() );
    return compiler<token::list<_tail_ts...>>::eval(oCPU);
  }
};

// sub
template <typename ... _tail_ts>
struct compiler<token::list<token::sub, _tail_ts...>>
{
  static constexpr uint64_t eval(cpu& oCPU){
    oCPU.push( oCPU.pop() - oCPU.pop() );
    return compiler<token::list<_tail_ts...>>::eval(oCPU);
  }
};

// mul
template <typename ... _tail_ts>
struct compiler<token::list<token::mul, _tail_ts...>>
{
  static constexpr uint64_t eval(cpu& oCPU) {
    oCPU.push( oCPU.pop() * oCPU.pop() );
    return compiler<token::list<_tail_ts...>>::eval(oCPU);
  }
};

// div
template <typename ... _tail_ts>
struct compiler<token::list<token::div, _tail_ts...>>
{
  static constexpr uint64_t eval(cpu& oCPU) {
    oCPU.push( oCPU.pop() / oCPU.pop() );
    return compiler<token::list<_tail_ts...>>::eval(oCPU);
  }
};



int main(){

  cpu oCPU;
  std::cout << "Compiling user_code.txt: " << user_code << "\n\n";

  using scanner_t = scanner<sizeof(user_code), user_code>;
  std::cout << "scanner_t: " << typeid(scanner_t).name() << "\n\n";

  using token_list_t = typename scanner_t::token_list_type;
  std::cout << "token_list_t: " << typeid(token_list_t).name() << "\n\n";

  using compiler_t = compiler<token_list_t>;
  std::cout << "compiler_t: " << typeid(compiler_t).name() << "\n\n";

  return compiler_t::eval(oCPU);

}
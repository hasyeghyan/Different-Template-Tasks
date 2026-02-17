#include <iostream>
#include <utility>
#include <string>

//integer_const
template <typename T, T v>
struct integer_const {
    using value_type = T;
    using type = integer_const;
    constexpr static T value = v;

    constexpr operator value_type() { return value; }
    constexpr value_type operator() () { return value; }
};

using true_type = integer_const<bool, true>;
using false_type = integer_const<bool, false>;

//is_same
template <typename T, typename U>
struct is_same : false_type {};

template <typename T>
struct is_same<T,T> : true_type {};

template <typename T, typename U>
constexpr static bool is_same_v = is_same<T,U>::value;

//is_void
template <typename T>
struct is_void : false_type {};

template <>
struct is_void<void> : true_type {};

template <typename T>
constexpr static bool is_void_v = is_void<T>::value;

//is_pointer
template <typename T>
struct is_pointer : false_type {};

template <typename T>
struct is_pointer<T*> : true_type {};

template <typename T>
struct is_pointer<T* const> : true_type {};

template <typename T>
struct is_pointer<T* volatile> : true_type {};

template <typename T>
struct is_pointer<T* const volatile> : true_type {};

template <typename T>
constexpr static bool is_pointer_v = is_pointer<T>::value;

//remove_reference
template <typename T>
struct remove_reference {
    using type = T;
};

template <typename T>
struct remove_reference<T&> {
    using type = T;
};

template <typename T>
struct remove_reference<T&&> {
    using type = T;
};

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

//add_lvalue_reference
template <typename T>
struct add_lvalue_reference {
    using type = T&;
};

template <typename T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

//add_rvalue_reference
template <typename T>
struct add_rvalue_reference {
    using type = T&&;
};

template <typename T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

//is_reference
template <typename T>
struct is_reference : false_type {};

template <typename T>
struct is_reference<T&> : true_type {};

template <typename T>
struct is_reference<T&&> : true_type {};

template <typename T>
constexpr static bool is_reference_v = is_reference<T>::value;

//is_const
template <typename T>
struct is_const : false_type {};

template <typename T>
struct is_const<const T> : true_type {};

template <typename T>
constexpr static bool is_const_v = is_const<T>::value;

//remove_const
template <typename T>
struct remove_const {
    using type = T;
};

template <typename T>
struct remove_const<const T> {
    using type = T;
};

template <typename T>
using remove_const_t = typename remove_const<T>::type;

//is_convertible
template <typename From, typename To>
struct is_convertible {
    template <typename F, typename T>
    static auto test(int) -> decltype(static_cast<T>(std::declval<F>()), true_type{});
    
    template <typename F, typename T>
    static false_type test(...);
    
    constexpr static bool value = decltype(test<From, To>(0))::value;
};

template <typename T, typename U>
constexpr static bool is_convertible_v = is_convertible<T,U>::value;

//is_base_of
template <typename Base, typename Derived>
struct is_base_of {
    template <typename B, typename D>
    static true_type test(B*);
    
    template <typename B, typename D>
    static false_type test(...);
    
    constexpr static bool value = !std::is_same_v<Base,Derived> && std::is_class_v<Base> 
    && std::is_class_v<Derived>  && decltype(test<Base,Derived>(static_cast<Derived*>(nullptr)))::value;
};

template <typename T, typename U>
constexpr static bool is_base_of_v = is_base_of<T,U>::value;

class Base {
    public:
        friend void operator+(Base b1, Base b2) {} 
};
class Derived : public Base {};
class Derived2 : public Base {};

//is_default_constructable
template <typename T>
struct is_default_constructable {
    template <typename U>
    static auto test(int) -> decltype(U(), true_type{});
    
    template <typename U>
    static false_type test(...);
    
    constexpr static bool value = decltype(test<T>(0))::value;
};

template <typename T>
constexpr static bool is_default_constructable_v = is_default_constructable<T>::value;

//has_operator_plus
template <typename T>
struct has_operator_plus {
    template <typename U>
    static auto test(int) -> decltype(std::declval<U>() + std::declval<U>(), true_type{});
    
    template <typename U>
    static false_type test(...);
    
    constexpr static bool value = decltype(test<T>(0))::value;
};

template <typename T>
constexpr static bool has_operator_plus_v = has_operator_plus<T>::value;

// //decay version1
// template <typename T>
// struct decay {
//     using U = remove_reference_t<T>;
//     using type = std::conditional_t<std::is_array_v<U>,std::remove_extent_t<U>*, 
//     std::conditional_t<std::is_function_v<U>, U*, 
//     remove_const_t<std::remove_volatile_t<U>>>>;
// };

// template <typename T>
// using decay_t = typename decay<T>::type;

//decay version2
template <typename T>
struct decay {
    using U = remove_reference_t<T>;
    using type = remove_const_t<std::remove_volatile_t<U>>;
};

template <typename T, size_t N>
struct decay<T[N]> {
    using type = T*;
};

template <typename T>
struct decay<T[]> {
    using type = T*;
};

template <typename T, typename... Types>
struct decay<T(Types... args)> {
    using type = T(*)(Types... args);
};

template <typename T>
using decay_t = typename decay<T>::type;

//factorial
template <size_t N>
struct factorial {
    constexpr static int value = (N * factorial<N - 1>::value);
};

template <>
struct factorial<0> {
    constexpr static int value = 1;
};

//fibonachi
template <int N>
struct fibonachi {
    constexpr static int value = (fibonachi<N - 1>::value + fibonachi<N - 2>::value);
};

template <>
struct fibonachi<0> {
    constexpr static int value = 0;
};

template <>
struct fibonachi<1> {
    constexpr static int value = 1;
};

//is_prime
template <int N, int D>
struct is_prime_helper {
    constexpr static bool value = (N % D) && (is_prime_helper<N,D - 1>::value);
};

template <int N>
struct is_prime_helper<N,1> {
    constexpr static bool value = true;
};

template <int N>
struct is_prime {
    constexpr static bool value = is_prime_helper<N,N / 2>::value;
};

template <>
struct is_prime<1> {
    constexpr static bool value = false;
};

template <>
struct is_prime<0> {
    constexpr static bool value = false;
};

//next_prime
template <int N, bool B>
struct next_prime_helper {
    constexpr static size_t value = next_prime_helper<N + 1,is_prime<N + 1>::value>::value;
};

template <int N>
struct next_prime_helper<N, true> {
    constexpr static size_t value = N;
};

template <int N>
struct next_prime {
    constexpr static size_t value = next_prime_helper<N,false>::value;
};

//unique_type
template <typename T,typename... Types>
struct unique_types {
    constexpr static bool value = (!(... || is_same_v<T,Types>)) && (unique_types<Types...>::value);
};

template <typename T>
struct unique_types<T> {
    constexpr static bool value = true;
};

int main() {

    // static_assert(is_same_v<int,int>);
    // static_assert(is_same_v<int,double>, "Error");


    // static_assert(is_void_v<void>);
    // static_assert(is_void_v<int>, "Error");

    // static_assert(is_pointer_v<int>);
    // static_assert(is_pointer_v<const int* const volatile >, "Error");

    // static_assert(is_same_v<remove_reference_t<int&>, int>);

    // static_assert(is_same_v<add_lvalue_reference_t<int>, int&>);
    // static_assert(is_same_v<add_rvalue_reference_t<int>, int&&>);

    // static_assert(is_reference_v<int&&>);

    // static_assert(is_const_v<int* const>);

    // static_assert(is_same_v<remove_const_t<int* const>,int*>);

    // static_assert(is_convertible_v<int,float>);

    // static_assert(is_base_of_v<Base,Derived>);
  
    // static_assert(is_default_constructable_v<Base>);
   
    // static_assert(has_operator_plus_v<Derived>);

    // static_assert(is_same_v<decay_t<void (int)>, void(*)(int)>);
    
    // std::cout << factorial<5>::value << std::endl;
   
    // std::cout << fibonachi<7>::value << std::endl;

    // std::cout << is_prime<4>::value << std::endl;

    // std::cout << next_prime<1>::value << std::endl;
  
    // std::cout << unique_types<int,double,float,bool>::value << std::endl;

    return 0;
}
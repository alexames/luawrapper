/*
 * Copyright (c) 2010-2013 Alexander Ames
 * Alexander.Ames@gmail.com
 */

// API Summary:
//
// This is a set of utility functions that add to the functionalit of
// LuaWrapper. Over time I found myself repeating a few patterns, such as
// writing many trvial getter and setter functions, and wanting pass ownership
// of one object to another and have the Lua script properly handle that case.
//
// This file contains the additional functions that I've added but that do
// not really belong in the core API.

#ifndef LUAWRAPPERUTILS_HPP_
#define LUAWRAPPERUTILS_HPP_

#include <type_traits>

#include "luawrapper.hpp"

////////////////////////////////////////////////////////////////////////////////
//
// This template removes reference and const qualifier from the type
//
template <typename T>
struct luaU_RemoveConstRef {
  typedef typename std::remove_const<typename std::remove_reference<T>::type>::type type;
};

////////////////////////////////////////////////////////////////////////////////
//
// A set of templated luaL_check and lua_push functions for use in the getters
// and setters below
//
// It is often useful to override luaU_check, luaU_to and/or luaU_push to
// operate on your own simple types rather than register your type with
// LuaWrapper, especially with small objects.
//

// clang-format off
template <typename U> bool luaU_is(lua_State* L, int index);
template <> inline bool luaU_is<bool>       (lua_State* L, int index) { return lua_isboolean(L, index); }
template <> inline bool luaU_is<uint8_t>    (lua_State* L, int index) { return lua_isnumber(L, index); }
template <> inline bool luaU_is<int8_t>     (lua_State* L, int index) { return lua_isnumber(L, index); }
template <> inline bool luaU_is<uint16_t>   (lua_State* L, int index) { return lua_isnumber(L, index); }
template <> inline bool luaU_is<int16_t>    (lua_State* L, int index) { return lua_isnumber(L, index); }
template <> inline bool luaU_is<uint32_t>   (lua_State* L, int index) { return lua_isnumber(L, index); }
template <> inline bool luaU_is<int32_t>    (lua_State* L, int index) { return lua_isnumber(L, index); }
template <> inline bool luaU_is<uint64_t>   (lua_State* L, int index) { return lua_isnumber(L, index); }
template <> inline bool luaU_is<int64_t>    (lua_State* L, int index) { return lua_isnumber(L, index); }
template <> inline bool luaU_is<float>      (lua_State* L, int index) { return lua_isnumber(L, index);  }
template <> inline bool luaU_is<double>     (lua_State* L, int index) { return lua_isnumber(L, index);  }
template <> inline bool luaU_is<const char*>(lua_State* L, int index) { return lua_isstring(L, index);  }

template <typename U>
U luaU_to(lua_State* L, int index);
template <> inline bool        luaU_to(lua_State* L, int index) { return lua_toboolean(L, index) != 0;                   }
template <> inline uint8_t     luaU_to(lua_State* L, int index) { return static_cast<uint8_t>(lua_tointeger(L, index));  }
template <> inline int8_t      luaU_to(lua_State* L, int index) { return static_cast<int8_t>(lua_tointeger(L, index));   }
template <> inline uint16_t    luaU_to(lua_State* L, int index) { return static_cast<uint16_t>(lua_tointeger(L, index)); }
template <> inline int16_t     luaU_to(lua_State* L, int index) { return static_cast<int16_t>(lua_tointeger(L, index));  }
template <> inline uint32_t    luaU_to(lua_State* L, int index) { return static_cast<uint32_t>(lua_tointeger(L, index)); }
template <> inline int32_t     luaU_to(lua_State* L, int index) { return static_cast<int32_t>(lua_tointeger(L, index));  }
template <> inline uint64_t    luaU_to(lua_State* L, int index) { return static_cast<uint64_t>(lua_tointeger(L, index)); }
template <> inline int64_t     luaU_to(lua_State* L, int index) { return static_cast<int64_t>(lua_tointeger(L, index));  }
template <> inline float       luaU_to(lua_State* L, int index) { return static_cast<float>(lua_tonumber(L, index));     }
template <> inline double      luaU_to(lua_State* L, int index) { return static_cast<double>(lua_tonumber(L, index));    }
template <> inline const char* luaU_to(lua_State* L, int index) { return lua_tostring(L, index);                         }

template <typename U>
U luaU_check(lua_State* L, int index);
template <> inline bool        luaU_check(lua_State* L, int index) {  return lua_toboolean(L, index) != 0;                       }
template <> inline uint8_t     luaU_check(lua_State* L, int index) {  return static_cast<uint8_t>(luaL_checkinteger(L, index));  }
template <> inline int8_t      luaU_check(lua_State* L, int index) {  return static_cast<int8_t>(luaL_checkinteger(L, index));   }
template <> inline uint16_t    luaU_check(lua_State* L, int index) {  return static_cast<uint16_t>(luaL_checkinteger(L, index)); }
template <> inline int16_t     luaU_check(lua_State* L, int index) {  return static_cast<int16_t>(luaL_checkinteger(L, index));  }
template <> inline uint32_t    luaU_check(lua_State* L, int index) {  return static_cast<uint32_t>(luaL_checkinteger(L, index)); }
template <> inline int32_t     luaU_check(lua_State* L, int index) {  return static_cast<int32_t>(luaL_checkinteger(L, index));  }
template <> inline uint64_t    luaU_check(lua_State* L, int index) {  return static_cast<uint64_t>(luaL_checkinteger(L, index)); }
template <> inline int64_t     luaU_check(lua_State* L, int index) {  return static_cast<int64_t>(luaL_checkinteger(L, index));  }
template <> inline float       luaU_check(lua_State* L, int index) {  return static_cast<float>(luaL_checknumber(L, index));     }
template <> inline double      luaU_check(lua_State* L, int index) {  return static_cast<double>(luaL_checknumber(L, index));    }
template <> inline const char* luaU_check(lua_State* L, int index) {  return luaL_checkstring(L, index);                         }

inline void luaU_push(lua_State* L, bool        value) { lua_pushboolean(L, value); }
inline void luaU_push(lua_State* L, uint8_t     value) { lua_pushinteger(L, value); }
inline void luaU_push(lua_State* L, int8_t      value) { lua_pushinteger(L, value); }
inline void luaU_push(lua_State* L, uint16_t    value) { lua_pushinteger(L, value); }
inline void luaU_push(lua_State* L, int16_t     value) { lua_pushinteger(L, value); }
inline void luaU_push(lua_State* L, uint32_t    value) { lua_pushinteger(L, value); }
inline void luaU_push(lua_State* L, int32_t     value) { lua_pushinteger(L, value); }
inline void luaU_push(lua_State* L, uint64_t    value) { lua_pushinteger(L, value); }
inline void luaU_push(lua_State* L, int64_t     value) { lua_pushinteger(L, value); }
inline void luaU_push(lua_State* L, float       value) { lua_pushnumber(L, value);  }
inline void luaU_push(lua_State* L, double      value) { lua_pushnumber(L, value);  }
inline void luaU_push(lua_State* L, const char* value) { lua_pushstring(L, value);  }
// clang-format on

////////////////////////////////////////////////////////////////////////////////
//
// This is slightly different than the previous three functions in that you
// shouldn't need to write your own version of it, since it uses luaU_check
// automatically.
//
template <typename U>
U luaU_opt(lua_State* L, int index, const U& fallback = U()) {
  if (lua_isnoneornil(L, index)) {
    return fallback;
  } else {
    return luaU_check<U>(L, index);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
// These are just some functions I've always felt should exist
//

template <typename U>
inline U luaU_getfield(lua_State* L, int index, const char* field) {
  static_assert(!std::is_same<U, const char*>::value,
                "luaU_getfield is not safe to use on const char*'s. (The string will be popped from the stack.)");
  lua_getfield(L, index, field);
  U val = luaU_to<U>(L, -1);
  lua_pop(L, 1);
  return val;
}

template <typename U>
inline U luaU_checkfield(lua_State* L, int index, const char* field) {
  static_assert(!std::is_same<U, const char*>::value,
                "luaU_checkfield is not safe to use on const char*'s. (The string will be popped from the stack.)");
  lua_getfield(L, index, field);
  U val = luaU_check<U>(L, -1);
  lua_pop(L, 1);
  return val;
}

template <typename U>
inline U luaU_optfield(lua_State* L, int index, const char* field, const U& fallback = U()) {
  static_assert(!std::is_same<U, const char*>::value,
                "luaU_getfield is not safe to use on const char*'s. (The string will be popped from the stack.)");
  lua_getfield(L, index, field);
  U val = luaU_opt<U>(L, -1, fallback);
  lua_pop(L, 1);
  return val;
}

template <typename U>
inline void luaU_setfield(lua_State* L, int index, const char* field, U val) {
  luaU_push(L, val);
  lua_setfield(L, luaW_correctindex(L, index, 1), field);
}

///////////////////////////////////////////////////////////////////////////////
//
// A set of trivial getter and setter templates. These templates are designed
// to call trivial getters or setters.
//
// There are four forms:
//  1. Getting or setting a public member variable that is of a primitive type
//  2. Getting or setting a public member variable that is a pointer to an
//     object
//  3. Getting or setting a private member variable that is of a primitive type
//     through a getter or setter
//  3. Getting or setting a private member variable that is is a pointer to an
//     object through a getter or setter
//
// The interface to all of them is the same however. In addition to plain
// getter and setter functions, there is a getset which does both - if an
// argument is supplied it attempts to set the value, and in either case it
// returns the value. In your lua table declaration in C++ rather than write
// individiual wrappers for each getter and setter you may do the following:
//
// static luaL_reg Foo_metatable[] =
// {
//     { "GetBar", luaU_get<Foo, bool, &Widget::GetBar> },
//     { "SetBar", luaU_set<Foo, bool, &Widget::SetBar> },
//     { "Bar", luaU_getset<Foo, bool, &Widget::GetBar, &Widget::SetBar> },
//     { NULL, NULL }
// }
//
// Getters and setters must have one of the following signatures:
//    void T::Setter(U value);
//    void T::Setter(U* value);
//    void T::Setter(const U& value);
//    U Getter() const;
//    U* Getter() const;
//
// In this example, the variable Foo::bar is private and so it is accessed
// through getter and setter functions. If Foo::bar were public, it could be
// accessed directly, like so:
//
// static luaL_reg Foo_metatable[] =
// {
//     { "GetBar", luaU_get<Foo, bool, &Widget::bar> },
//     { "SetBar", luaU_set<Foo, bool, &Widget::bar> },
//     { "Bar", luaU_getset<Foo, bool, &Widget::bar> },
// }
//
// In a Lua script, you can now use foo:GetBar(), foo:SetBar() and foo:Bar()
//

template <typename T, typename U, U T::*Member>
int luaU_get(lua_State* L) {
  T* obj = luaW_check<T>(L, 1);
  luaU_push(L, obj->*Member);
  return 1;
}

template <typename T, typename U, U* T::*Member>
int luaU_get(lua_State* L) {
  T* obj = luaW_check<T>(L, 1);
  luaW_push<U>(L, obj->*Member);
  return 1;
}

template <typename T, typename U, U (T::*Getter)() const>
int luaU_get(lua_State* L) {
  T* obj = luaW_check<T>(L, 1);
  luaU_push(L, (obj->*Getter)());
  return 1;
}

template <typename T, typename U, const U& (T::*Getter)() const>
int luaU_get(lua_State* L) {
  T* obj = luaW_check<T>(L, 1);
  luaU_push(L, (obj->*Getter)());
  return 1;
}

template <typename T, typename U, U* (T::*Getter)() const>
int luaU_get(lua_State* L) {
  T* obj = luaW_check<T>(L, 1);
  luaW_push<U>(L, (obj->*Getter)());
  return 1;
}

template <typename T, typename U, U T::*Member>
int luaU_set(lua_State* L) {
  T* obj = luaW_check<T>(L, 1);
  if (obj) {
    obj->*Member = luaU_check<U>(L, 2);
  }
  return 0;
}

template <typename T, typename U, U* T::*Member>
int luaU_set(lua_State* L) {
  T* obj = luaW_check<T>(L, 1);
  if (obj) {
    U* member = luaW_opt<U>(L, 2);
    obj->*Member = member;
  }
  return 0;
}

template <typename T, typename U, const U* T::*Member>
int luaU_set(lua_State* L) {
  T* obj = luaW_check<T>(L, 1);
  if (obj) {
    U* member = luaW_opt<U>(L, 2);
    obj->*Member = member;
  }
  return 0;
}

template <typename T, typename U, const U* T::*Member>
int luaU_setandrelease(lua_State* L) {
  T* obj = luaW_check<T>(L, 1);
  if (obj) {
    U* member = luaW_opt<U>(L, 2);
    obj->*Member = member;
    if (member) {
      luaW_release<U>(L, member);
    }
  }
  return 0;
}

template <typename T, typename U, void (T::*Setter)(U)>
int luaU_set(lua_State* L) {
  T* obj = luaW_check<T>(L, 1);
  if (obj) {
    (obj->*Setter)(luaU_check<U>(L, 2));
  }
  return 0;
}

template <typename T, typename U, void (T::*Setter)(const U&)>
int luaU_set(lua_State* L) {
  T* obj = luaW_check<T>(L, 1);
  if (obj) {
    (obj->*Setter)(luaU_check<U>(L, 2));
  }
  return 0;
}

template <typename T, typename U, void (T::*Setter)(U*)>
int luaU_set(lua_State* L) {
  T* obj = luaW_check<T>(L, 1);
  if (obj) {
    U* member = luaW_opt<U>(L, 2);
    (obj->*Setter)(member);
  }
  return 0;
}

template <typename T, typename U, void (T::*Setter)(U*)>
int luaU_setandrelease(lua_State* L) {
  T* obj = luaW_check<T>(L, 1);
  if (obj) {
    U* member = luaW_opt<U>(L, 2);
    (obj->*Setter)(member);
    if (member) {
      luaW_release<U>(L, member);
    }
  }
  return 0;
}

template <typename T, typename U, U T::*Member>
int luaU_getset(lua_State* L) {
  T* obj = luaW_check<T>(L, 1);
  if (obj && lua_gettop(L) >= 2) {
    obj->*Member = luaU_check<U>(L, 2);
    return 0;
  } else {
    luaU_push(L, obj->*Member);
    return 1;
  }
}

template <typename T, typename U, U* T::*Member>
int luaU_getset(lua_State* L) {
  T* obj = luaW_check<T>(L, 1);
  if (obj && lua_gettop(L) >= 2) {
    U* member = luaW_opt<U>(L, 2);
    obj->*Member = member;
    return 0;
  } else {
    luaW_push<U>(L, obj->*Member);
    return 1;
  }
}

template <typename T, typename U, U* T::*Member>
int luaU_getsetandrelease(lua_State* L) {
  T* obj = luaW_check<T>(L, 1);
  if (obj && lua_gettop(L) >= 2) {
    U* member = luaW_opt<U>(L, 2);
    obj->*Member = member;
    if (member) {
      luaW_release<U>(L, member);
    }
    return 0;
  } else {
    luaW_push<U>(L, obj->*Member);
    return 1;
  }
}

template <typename T, typename U, U (T::*Getter)() const, void (T::*Setter)(U)>
int luaU_getset(lua_State* L) {
  T* obj = luaW_check<T>(L, 1);
  if (obj && lua_gettop(L) >= 2) {
    (obj->*Setter)(luaU_check<U>(L, 2));
    return 0;
  } else {
    luaU_push(L, (obj->*Getter)());
    return 1;
  }
}

template <typename T, typename U, U (T::*Getter)() const, void (T::*Setter)(const U&)>
int luaU_getset(lua_State* L) {
  T* obj = luaW_check<T>(L, 1);
  if (obj && lua_gettop(L) >= 2) {
    (obj->*Setter)(luaU_check<U>(L, 2));
    return 0;
  } else {
    luaU_push(L, (obj->*Getter)());
    return 1;
  }
}

template <typename T, typename U, const U& (T::*Getter)() const, void (T::*Setter)(const U&)>
int luaU_getset(lua_State* L) {
  T* obj = luaW_check<T>(L, 1);
  if (obj && lua_gettop(L) >= 2) {
    (obj->*Setter)(luaU_check<U>(L, 2));
    return 0;
  } else {
    luaU_push(L, (obj->*Getter)());
    return 1;
  }
}

template <typename T, typename U, U* (T::*Getter)() const, void (T::*Setter)(U*)>
int luaU_getset(lua_State* L) {
  T* obj = luaW_check<T>(L, 1);
  if (obj && lua_gettop(L) >= 2) {
    U* member = luaW_opt<U>(L, 2);
    (obj->*Setter)(member);
    return 0;
  } else {
    luaW_push<U>(L, (obj->*Getter)());
    return 1;
  }
}

template <typename T, typename U, U* (T::*Getter)() const, void (T::*Setter)(U*)>
int luaU_getsetandrelease(lua_State* L) {
  T* obj = luaW_check<T>(L, 1);
  if (obj && lua_gettop(L) >= 2) {
    U* member = luaW_opt<U>(L, 2);
    (obj->*Setter)(member);
    if (member) {
      luaW_release<U>(L, member);
    }
    return 0;
  } else {
    luaW_push<U>(L, (obj->*Getter)());
    return 1;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
// luaU_func is a special macro that expands into a simple function wrapper.
// Unlike the getter setters above, you merely need to name the function you
// would like to wrap.
//
// For example,
//
// struct Foo
// {
//     int DoSomething(int, const char*);
// };
//
// static luaL_reg Foo_metatable[] =
// {
//     { "DoSomething", luaU_func(&Foo::DoSomething) },
//     { NULL, NULL }
// }
//
// void RegisterFoo(lua_State* L)
// {
//     luaW_register<Foo>(L, "Foo", NULL, Foo_metatable);
// }
//
// This macro will expand based on the function signature of Foo::DoSomething
// In this example, it would expand into the following wrapper:
//
//     luaU_push(luaW_check<T>(L, 1)->DoSomething(luaU_check<int>(L, 2), luaU_check<const char*>(L, 3)));
//     return 1;
//
// In this example there is only one member function called DoSomething. In some
// cases there may be multiple overrides for a function. For those cases, an
// additional macro has been provided, luaU_funcsig, which takes the entire
// function signature. The arguments to the macro reflect the order they would
// appear in the function signature: return type, type name, function name, and
// finally a list of all the argument types. For example:
//
// struct Foo
// {
//     int DoSomething (const char*);
//     int DoSomething (const char*, int);
// };
//
// const struct luaL_Reg Foo_metatable[] =
// {
//     {"DoSomething1", luaU_funcsig(int, Foo, DoSomething, const char*)) },
//     {"DoSomething1", luaU_funcsig(int, Foo, DoSomething, const char*, int)) },
//     { NULL, NULL }
// };
//
// There`s also support for static and freestanding functions. Macros luaU_staticfunc
// and luaU_staticfuncsig work equally to luaU_func and luaU_funcsig, except for that
// you need to provide a separate metatable for static functions.
// For example,
//
// struct Foo
// {
//     int DoSomething(int, const char*);
//     static void DoSomethingElse(int a, int b, float c);
// };
//
// static luaL_reg Foo_metatable[] =
// {
//     { "DoSomething", luaU_func(&Foo::DoSomething) },
//     { NULL, NULL }
// };
//
// static luaL_reg Foo_metatable_static[] =
// {
//     { "DoSomethingElse", luaU_staticfunc(&Foo::DoSomethingElse) },
//     { NULL, NULL }
// };
//
// void RegisterFoo(lua_State* L)
// {
//     luaW_register<Foo>(L, "Foo", Foo_metatable_static, Foo_metatable);
// }
//
// After that you will be able to use Foo class from Lua like this:
// local foo = Foo.new()
// foo:DoSomething(42, 'The Ultimate Question of Life, the Universe, and Everything.') -- member function call
// Foo:DoSomethingElse(30, 12, 3.1459) -- Static function call
//
// These macros and it's underlying templates are somewhat experimental and some
// refinements are probably needed.  There are cases where it does not
// currently work and I expect some changes can be made to refine its behavior.
//

#define luaU_func(memberfunc) &luaU_MemberFuncWrapper<decltype(memberfunc), memberfunc>::call
#define luaU_funcsig(returntype, type, funcname, ...) luaU_func(static_cast<returntype (type::*)(__VA_ARGS__)>(&type::funcname))

#define luaU_staticfunc(func) &luaU_StaticFuncWrapper<decltype(func), func>::call
#define luaU_staticfuncsig(returntype, type, funcname, ...) luaU_staticfunc(static_cast<returntype (*)(__VA_ARGS__)>(&type::funcname))

template <int... ints>
struct luaU_IntPack {};
template <int start, int count, int... tail>
struct luaU_MakeIntRangeType {
  typedef typename luaU_MakeIntRangeType<start, count - 1, start + count - 1, tail...>::type type;
};
template <int start, int... tail>
struct luaU_MakeIntRangeType<start, 0, tail...> {
  typedef luaU_IntPack<tail...> type;
};
template <int start, int count>
inline typename luaU_MakeIntRangeType<start, count>::type luaU_makeIntRange() {
  return typename luaU_MakeIntRangeType<start, count>::type();
}

///////////////////////////////////////////////////////////////////////////////
//
// Member function wrapper
//
template <class MemFunPtrType, MemFunPtrType MemberFunc>
struct luaU_MemberFuncWrapper;

template <class T, class ReturnType, class... Args, ReturnType (T::*MemberFunc)(Args...)>
struct luaU_MemberFuncWrapper<ReturnType (T::*)(Args...), MemberFunc> {
 public:
  static int call(lua_State* L) { return callImpl(L, luaU_makeIntRange<2, sizeof...(Args)>()); }

 private:
  template <int... indices>
  static int callImpl(lua_State* L, luaU_IntPack<indices...>) {
    luaU_push<ReturnType>(L, (luaW_check<T>(L, 1)->*MemberFunc)(luaU_check<typename luaU_RemoveConstRef<Args>::type>(L, indices)...));
    return 1;
  }
};

template <class T, class... Args, void (T::*MemberFunc)(Args...)>
struct luaU_MemberFuncWrapper<void (T::*)(Args...), MemberFunc> {
 public:
  static int call(lua_State* L) { return callImpl(L, luaU_makeIntRange<2, sizeof...(Args)>()); }

 private:
  template <int... indices>
  static int callImpl(lua_State* L, luaU_IntPack<indices...>) {
    (luaW_check<T>(L, 1)->*MemberFunc)(luaU_check<typename luaU_RemoveConstRef<Args>::type>(L, indices)...);
    return 0;
  }
};

///////////////////////////////////////////////////////////////////////////////
//
// static function wrapper
//

template <class FunPtrType, FunPtrType Func>
struct luaU_StaticFuncWrapper;

template <class ReturnType, class... Args, ReturnType (*Func)(Args...)>
struct luaU_StaticFuncWrapper<ReturnType (*)(Args...), Func> {
 public:
  static int call(lua_State* L) { return callImpl(L, luaU_makeIntRange<2, sizeof...(Args)>()); }

 private:
  template <int... indices>
  static int callImpl(lua_State* L, luaU_IntPack<indices...>) {
    luaU_push<ReturnType>(L, (*Func)(luaU_check<typename luaU_RemoveConstRef<Args>::type>(L, indices)...));
    return 1;
  }
};

template <class... Args, void (*Func)(Args...)>
struct luaU_StaticFuncWrapper<void (*)(Args...), Func> {
 public:
  static int call(lua_State* L) { return callImpl(L, luaU_makeIntRange<2, sizeof...(Args)>()); }

 private:
  template <int... indices>
  static int callImpl(lua_State* L, luaU_IntPack<indices...>) {
    (*Func)(luaU_check<typename luaU_RemoveConstRef<Args>::type>(L, indices)...);
    return 0;
  }
};

///////////////////////////////////////////////////////////////////////////////
//
// Calls the copy constructor for an object of type T.
// Arguments may be passed in, in case they're needed for the postconstructor
//
// e.g.
//
// C++:
// luaL_reg Foo_Metatable[] =
// {
//     { "clone", luaU_clone<Foo> },
//     { NULL, NULL }
// };
//
// Lua:
// foo = Foo.new()
// foo2 = foo:clone()
//
template <typename T>
int luaU_clone(lua_State* L) {
  // obj ...
  T* obj = new T(*luaW_check<T>(L, 1));
  lua_remove(L, 1);  // ...
  int numargs = lua_gettop(L);
  luaW_push<T>(L, obj);  // ... clone
  luaW_hold<T>(L, obj);
  luaW_postconstructor<T>(L, numargs);
  return 1;
}

///////////////////////////////////////////////////////////////////////////////
//
// luaU_build is intended to be used to initialize many values by passing in a
// table. They keys of the table are used as function names, and values are
// used as arguments to the function. This is intended to be used on functions
// that are simple setters.
//
// For example, if luaU_build is set as the post constructor, you can
// initialize an object as so:
//
// f = Foo.new
// {
//     X = 10;
//     Y = 20;
// }
//
// After the object is constructed, luaU_build will do the equivalent of
// calling f:X(10) and f:Y(20).
//
template <typename T>
int luaU_build(lua_State* L) {
  // obj {}
  lua_insert(L, -2);  // {} obj
  if (lua_type(L, 1) == LUA_TTABLE) {
    for (lua_pushnil(L); lua_next(L, 1); lua_pop(L, 1)) {
      // {} obj k v
      lua_pushvalue(L, -2);  // {} obj k v k
      lua_gettable(L, -4);   // {} obj k v ud[k]
      lua_pushvalue(L, -4);  // {} obj k v ud[k] ud
      lua_pushvalue(L, -3);  // {} obj k v ud[k] ud v
      lua_call(L, 2, 0);     // {} obj k v
    }
    // {} ud
  }
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// Takes the object of type T at the top of the stack and stores it in on a
// table with the name storagetable, on the table at the specified index.
//
// You may manually call luaW_hold and luaW_release to handle pointer
// ownership, but it is often easier to simply store a Lua userdata on a table
// that is owned by its parent. This ensures that your object will not be
// prematurely freed, and that it can only be destoryed after its parent.
//
// e.g.
//
// Foo* parent = luaW_check<Foo>(L, 1);
// Bar* child = luaW_check<Bar>(L, 2);
// if (parent && child)
// {
//     luaU_store<Bar>(L, 1, "Children");
//     parent->AddChild(child);
// }
//
template <typename T>
void luaU_store(lua_State* L, int index, const char* storagetable, const char* key = NULL) {
  // ... store ... obj
  lua_getfield(L, index, storagetable);  // ... store ... obj store.storagetable
  if (key) {
    lua_pushstring(L, key);  // ... store ... obj store.storagetable key
  } else {
    LuaWrapper<T>::identifier(L, luaW_to<T>(L, -2));  // ... store ... obj store.storagetable key
  }
  lua_pushvalue(L, -3);  // ... store ... obj store.storagetable key obj
  lua_settable(L, -3);   // ... store ... obj store.storagetable
  lua_pop(L, 1);         // ... store ... obj
}

#endif  // LUAWRAPPERUTILS_HPP_
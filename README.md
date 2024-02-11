# cpp-bind-any-methods
The way you can bind any methods(even class's member functions) to map and call it dynamically.

## Usage
### Code
```cpp
#include "my_function_map.h"
std::string add_str(std::string i, std::string j) {
	return i + " " +  j;
}

int main() {
  MyFunctionMap myFunctionMap;
  myFunctionMap.bindFunction("add_str", &add_str);
  std::string value = std::any_cast<std::string>(myFunctionMap.getFunction("add_str")->call({ std::string("Hello"), std::string("World") }));
  std::cout << value << "\n";
}
```
### Console Output
```
Hello World
```

## How does It works?
1. The `Function` class does not have any template factors.
2. Only `CallableFunction` and `CallableClassFunction`, which inherit the `Function` class, have template factors.
3. The `MyFunctionMap` class has a pointer of the `Function` class as information of the map.
4. The `call` function of the `Function` class is declared as a virtual function.
5. In the `call` function, the internal binded function is called through the C++'s `variadic template` and `std::apply`.

## Where do I use it?
You can use this technique to your own dynamic language interpreter.

## Warning
- Only C++ 17+ compiler can compile this source.
- When you call `Function::call(const std::vector<std::any>& args)` function, You should pass explicit value. If type of given parameters are not the same with info of binded function, It will throw std::bad_any_cast.
- The binded function must uses `decltype(nullptr)` instead of void at return type('Cause we can't covert void type into std::any).

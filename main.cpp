#include "my_function_map.h"

std::string add_str(std::string i, std::string j) {
	return i + " " +  j;
}

class MyClass {
	
public:
	decltype(nullptr) print_sum(int x, int y) {
		std::cout << x + y << "\n";
		return nullptr;
	}
};

int main() {
	MyFunctionMap  myFunctionMap;
	{
		myFunctionMap.bindFunction("add_str", &add_str);
		std::string value = std::any_cast<std::string>(myFunctionMap.getFunction("add_str")->call({ std::string("Hello"), std::string("World") }));
		std::cout << value << "\n";
	}
	{
		MyClass myclass;
		myFunctionMap.bindClassFunction("print_sum", &MyClass::print_sum, &myclass);
		myFunctionMap.getFunction("print_sum")->call({ 1, 2 });
	}
}

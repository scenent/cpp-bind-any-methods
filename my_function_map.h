#ifndef MY_FUNCTION_MAP_H
#define MY_FUNCTION_MAP_H

#include <any>
#include <assert.h>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>

class Function {
public:
    virtual std::any call(const std::vector<std::any>& args) = 0;
};

template<typename ReturnType, typename... Args>
class CallableFunction : public Function {
public:
    CallableFunction(ReturnType(*func)(Args...)) : func_(func) {}

    std::any call(const std::vector<std::any>& args) override {
        assert(sizeof...(Args) == args.size());
        try {
            std::tuple<Args...> arguments;
            fill_tuple(arguments, args);
            return std::apply([this](auto&&... args) { return func_(args ...); }, arguments);
        }
        catch (std::bad_any_cast& bac) {
            std::cerr << "Error : Bad Any Cast. Returning Nullptr.\n";
            return std::any(nullptr);
        }
    }

private:
    ReturnType(*func_)(Args...);

    template<std::size_t... I>
    void fill_tuple(std::tuple<Args...>& t, const std::vector<std::any>& args, std::index_sequence<I...>) {
        ((std::get<I>(t) = std::any_cast<typename std::tuple_element<I, std::tuple<Args...>>::type>(args[I])), ...);
    }

    void fill_tuple(std::tuple<Args...>& t, const std::vector<std::any>& args) {
        fill_tuple(t, args, std::index_sequence_for<Args...>{});
    }
};

template<typename ClassType, typename ReturnType, typename... Args>
class CallableClassFunction : public Function {
public:
    CallableClassFunction(ReturnType(ClassType::* func)(Args...), ClassType* obj)
        : func_(func), obj_(obj) {}

    std::any call(const std::vector<std::any>& args) override {
        assert(sizeof...(Args) == args.size());
        try {
            std::tuple<Args...> arguments;
            fill_tuple(arguments, args);
            return std::apply([this](auto&&... args){ return (obj_->*func_)(args...); }, arguments);
        }
        catch (std::bad_any_cast& bac) {
            std::cerr << "Error : Bad Any Cast. Returning Nullptr.\n";
            return std::any(nullptr);
        }
    }

private:
    ReturnType(ClassType::* func_)(Args...);
    ClassType* obj_;

    template<std::size_t... I>
    void fill_tuple(std::tuple<Args...>& t, const std::vector<std::any>& args, std::index_sequence<I...>) {
        ((std::get<I>(t) = std::any_cast<typename std::tuple_element<I, std::tuple<Args...>>::type>(args[I])), ...);
    }

    void fill_tuple(std::tuple<Args...>& t, const std::vector<std::any>& args) {
        fill_tuple(t, args, std::index_sequence_for<Args...>{});
    }
};


class MyFunctionMap {
public:
    template<typename ReturnType, typename... Args>
    void bindFunction(const std::string& name, ReturnType(*func)(Args...)) {
        functions[name] = std::make_shared<CallableFunction<ReturnType, Args...>>(func);
    }

    template<typename ClassType, typename ReturnType, typename... Args>
    void bindClassFunction(const std::string& name, ReturnType(ClassType::* func)(Args...), ClassType* obj) {
        functions[name] = std::make_shared<CallableClassFunction<ClassType, ReturnType, Args...>>(func, obj);
    }

    std::shared_ptr<Function> getFunction(const std::string& name) {
        return functions[name];
    }

private:
    std::map<std::string, std::shared_ptr<Function>> functions;
};



#endif

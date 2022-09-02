#include <iostream>
#include <type_traits>

#include "./app.h"

template <typename TState, typename TReducer, typename TActions>
App<TState, TReducer, TActions>::App() : state() {
    std::cout << "App created!" << std::endl;
}

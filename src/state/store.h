#pragma once

template <typename TState, typename TReducer, typename TActions> class Store {
  private:
    TState state;
    TReducer reducer;

  public:
    Store(TReducer r) : state(), reducer(r) {
        std::cout << "Store created!" << std::endl;
    }

    TState get_state() const { return state; }

    void dispatch(TActions action) { state = reducer(state, action); }
};

#pragma once

template <typename S, typename A> using Reducer = std::function<S(S, A)>;

template <typename TState, typename TAction> class StoreBase {
  private:
    TState state;
    Reducer<TState, TAction> reducer;

  public:
    StoreBase(Reducer<TState, TAction> r) : state(), reducer(r) {
        std::cout << "Store created!" << std::endl;
    }

    TState get_state() const { return state; }

    void dispatch(TAction action) { state = reducer(state, action); }
};

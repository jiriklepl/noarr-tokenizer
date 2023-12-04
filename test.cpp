#include <iostream>

#include <noarr/structures_extended.hpp>

#include <noarr/tokenizer.hpp>

template<class ...Fs>
struct overload : public Fs... {
    using Fs::operator()...;
};

int main() {
    using namespace noarr;
    auto structure = scalar<int>() ^ vector<'x'>() ^ set_length<'x'>(32);

    auto primitive_factory = make_token_factory(overload {
        []<class T>(scalar<T>) { return token_list<std::string>("scalar"); },
        []<IsDim auto Dim>(dim_param<Dim>) { return token_list<decltype(Dim)>(Dim); },
        []<IsDim auto Dim, class T>(vector_t<Dim, T>) { return token_list<std::string>("vector"); },
        [](auto) { return token_list<int>(42); }
    });

    auto tokens = make_tokenizer(structure).tokenize(primitive_factory);
    print_tokens([](auto i) { std::cout << i << ' '; }, tokens);
    std::cout << std::endl;
}
#include <iostream>

#include <noarr/structures_extended.hpp>
#include <noarr/structures/extra/mangle.hpp>

#include "noarr/tokenizer.hpp"

template<class ...Fs>
struct overload : public Fs... {
    using Fs::operator()...;
};

int main() {
    using namespace noarr;
    auto structure = scalar<int>() ^ vector<'y'>() ^ vector<'x'>() ^ set_length<'x'>(32) ^ set_length<'y'>(32);
    auto structure2 = scalar<int>() ^ vector<'x'>() ^ vector<'y'>() ^ set_length<'x'>(32) ^ set_length<'y'>(32);

    auto primitive_factory = generic_token_factory(overload {
        []<class T>(scalar<T>) { return token_list<std::string>("scalar") + token_list<std::string>(char_seq_to_str<typename helpers::scalar_name<T>::type>::c_str); },
        []<IsDim auto Dim>(dim_param<Dim>) { return token_list(Dim); },
        []<IsDim auto Dim, class T>(vector_t<Dim, T>) { return token_list<std::string>("^") + token_list<std::string>("vector") + token_list(Dim); },
        []<IsDim auto Dim, class T, class LenT>(set_length_t<Dim, T, LenT> sl) { return token_list<std::string>("^") + token_list<std::string>("set_length") + token_list(Dim) + token_list(sl.len()); },
        [](auto) { return token_list<>(); }
    });

    auto tokens = tokenizer(structure).tokenize(primitive_factory);
    auto tokens2 = tokenizer(structure2).tokenize(primitive_factory);

    print_tokens([](auto i) { std::cout << i << ' '; }, tokens);
    std::cout << std::endl;

    print_tokens([](auto i) { std::cout << i << ' '; }, tokens2);
    std::cout << std::endl;
}

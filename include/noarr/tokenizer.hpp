#ifndef NOARR_TOKENIZER_HPP
#define NOARR_TOKENIZER_HPP

#include <type_traits>
#include <utility>

#include "noarr/structures/base/contain.hpp"
#include "noarr/structures/base/structs_common.hpp"
#include "noarr/structures/structs/scalar.hpp"

namespace noarr {

template<class NoarrClass>
class tokenizer;

template<class NoarrClass>
tokenizer(NoarrClass&&) -> tokenizer<std::remove_cvref_t<NoarrClass>>;


template<class NoarrClass>
constexpr auto make_tokenizer(NoarrClass&& noarr_class) {
    return tokenizer(std::forward<NoarrClass>(noarr_class));
}

template<class F>
class generic_token_factory : public F {
public:
    using F::operator();
    template<class T>
    constexpr auto make_token(T&& value) const {
        return (*this)(std::forward<T>(value));
    }
};

template<class T>
constexpr auto make_token_factory(T &&t) {
    return generic_token_factory<T>(std::forward<T>(t));
}

template<class ...Tokens>
using token_list = flexible_contain<Tokens...>;

template<class Printer, class ...Tokens>
constexpr auto print_tokens(Printer&& printer, const token_list<Tokens...>& tokens) {
    []<std::size_t ...Is>(auto&& tokens, Printer&& printer, std::index_sequence<Is...>){ return (void(9), ... , std::forward<Printer>(printer)(tokens.template get<Is>())); }(tokens, std::forward<Printer>(printer), std::index_sequence_for<Tokens...>());
}

template<IsStruct Structure>
class tokenizer<Structure> : flexible_contain<Structure> {
public:
    using flexible_contain<Structure>::flexible_contain;
    template<class TokenFactory>
    constexpr auto tokenize(TokenFactory&& tf) const {
        return contain_cat(make_tokenizer(this->get().sub_structure()).tokenize(std::forward<TokenFactory>(tf)), std::forward<TokenFactory>(tf).make_token(this->get()), make_tokenizer(typename std::remove_cvref_t<decltype(this->get())>::params()).tokenize(std::forward<TokenFactory>(tf)));
    }
};

template<class T>
class tokenizer<scalar<T>> : flexible_contain<scalar<T>> {
public:
    using flexible_contain<scalar<T>>::flexible_contain;
    template<class TokenFactory>
    constexpr auto tokenize(TokenFactory&& tf) const {
        return contain_cat(std::forward<TokenFactory>(tf).make_token(this->get()));
    }
};

template<class ...Params>
class tokenizer<struct_params<Params...>> : flexible_contain<struct_params<Params...>> {
public:
    using flexible_contain<struct_params<Params...>>::flexible_contain;
    template<class TokenFactory>
    constexpr auto tokenize(TokenFactory&& tf) const {
        return tokenize(std::forward<TokenFactory>(tf), std::make_index_sequence<sizeof...(Params)>());
    }

private:
    template<class TokenFactory, std::size_t ...Is>
    constexpr auto tokenize(TokenFactory&& tf, std::index_sequence<Is...>) const {
        return contain_cat(std::forward<TokenFactory>(tf).make_token(this->get().template get<Is>())...);
    }
};

}

#endif // NOARR_TOKENIZER_HPP
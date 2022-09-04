#include "./parse.h"
#include <boost/tokenizer.hpp>
#include <codecvt>
#include <fstream>
#include <functional>
#include <iostream>
#include <locale>
#include <optional>
#include <scope_guard.hpp>
#include <sstream>
#include <string>

namespace ui {

    typedef boost::char_separator<wchar_t> separator;
    typedef boost::tokenizer<separator, std::wstring::const_iterator,
                             std::wstring>
        tokenizer;

    struct Parsing {
        struct None {};
        struct Error {
            std::wstring message;
        };
        struct String {
            std::wstring content;
        };
        struct Tag {
            std::wstring content;
        };
        struct Comment {
            std::wstring content;
        };
        typedef std::variant<None, Error, String, Tag, Comment> Token;
    };

    const std::wstring& take_until_match(const std::wstring& until,
                                         tokenizer& tokens) {
        std::wstringstream ss;
        for (const auto& token : tokens) {
            if (token == until)
                break;
            ss << token;
        }
        auto s = ss.str();
        return s;
    }
    Parsing::Token match_token(Parsing::Token token_state,
                               const std::wstring& token,
                               std::wostream& logger) {

        if (std::holds_alternative<Parsing::None>(token_state) &&
            token == L"\"") {
            return Parsing::String{};
        }
        if (std::holds_alternative<Parsing::None>(token_state) &&
            token == L"#") {
            return Parsing::Comment{};
        }

        return token_state;
    }

    template <typename T, typename V>
    bool apply(V& variant, std::function<void(T&)> cb) {
        if (std::holds_alternative<T>(variant)) {
            T& value = std::get<T>(variant);
            cb(value);
            return true;
        }
        return false;
    }

    const std::vector<Parsing::Token>& consume_tokens(tokenizer& tokens,
                                                      std::wostream& logger) {
        std::vector<Parsing::Token> parsed_tokens{};

        Parsing::Token token_state = Parsing::None{};

        for (const auto& token : tokens) {
            token_state = match_token(token_state, token, logger);

#ifndef USE_APPLY
            if (std::holds_alternative<Parsing::Error>(token_state)) {
                auto error = std::get<Parsing::Error>(token_state);
                logger << "\nERROR\n'" << error.message << "'\n" << std::endl;
            }

            if (std::holds_alternative<Parsing::String>(token_state)) {
                auto string = std::get<Parsing::String>(token_state);
                string.content = take_until_match(L"\"", tokens);
                parsed_tokens.push_back(string);

                token_state = Parsing::None{};
                continue;
            }

            if (std::holds_alternative<Parsing::Comment>(token_state)) {
                auto comment = std::get<Parsing::Comment>(token_state);
                comment.content = take_until_match(L"\n", tokens);
                parsed_tokens.push_back(comment);

                token_state = Parsing::None{};
                continue;
            }
#else
            if (apply<Parsing::Error>(
                    token_state, [&logger](const auto& error) {
                        logger << "\nERROR\n'" << error.message << "'\n"
                               << std::endl;
                    })) {
                // error handled
                token_state = Parsing::None{};
                continue;
            }

            if (apply<Parsing::String>(
                    token_state, [&tokens, &parsed_tokens](auto& string) {
                        string.content = take_until_match(L"\"", tokens);
                        parsed_tokens.push_back(string);
                    })) {
                token_state = Parsing::None{};
                continue;
            }

            if (apply<Parsing::Comment>(token_state, [&](auto& comment) {
                    comment.content = take_until_match(L"\n", tokens);
                    parsed_tokens.push_back(comment);
                })) {
                token_state = Parsing::None{};
                continue;
            }
#endif
        }
        return parsed_tokens;
    }

    std::shared_ptr<Node> parse(const std::string& path) {
        std::wstringstream ss;
        {
            std::wifstream wif{path};
            wchar_t c;
            std::wstring ws;
            while (wif.get(c))
                ws += c;
            ss << ws;
        }
        std::wstring s = ss.str();

        separator sep{L"", L" \t\n{}[]+=-*.,<>'\""};
        tokenizer tok{s, sep};

        const auto& tokens = consume_tokens(tok, std::wcout);
        std::cout << "\ntokens parsed!\n" << std::endl;

        return std::make_shared<Node>();
    }

} // namespace ui

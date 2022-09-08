#include "./parse.h"
#include <boost/algorithm/string.hpp>
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

#define LOG_VERBOSE 0

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

    std::wstring to_string(const Parsing::Token& token) {
        if (std::holds_alternative<Parsing::None>(token)) {
            return L"None";
        }
        if (std::holds_alternative<Parsing::Error>(token)) {
            std::wstringstream ws;
            ws << "Error(" << std::get<Parsing::Error>(token).message << ") ";
            return ws.str();
        }
        if (std::holds_alternative<Parsing::String>(token)) {
            std::wstringstream ws;
            ws << "String(" << std::get<Parsing::String>(token).content << ") ";
            return ws.str();
        }
        if (std::holds_alternative<Parsing::Tag>(token)) {
            std::wstringstream ws;
            ws << "Tag(" << std::get<Parsing::Tag>(token).content << ") ";
            return ws.str();
        }
        if (std::holds_alternative<Parsing::Comment>(token)) {
            std::wstringstream ws;
            ws << "Comment(" << std::get<Parsing::Comment>(token).content
               << ") ";
            return ws.str();
        }
        return {};
    }

    std::wstring take_until_match(const std::wstring& until,
                                  tokenizer::iterator& token_iterator) {
#if LOG_VERBOSE
        std::wcout << "take_until_match:'" << until << "'" << std::endl;
#endif
        std::wstringstream ss;
        while (!token_iterator.at_end()) {
            const auto& token = *token_iterator;
#if LOG_VERBOSE
            std::wcout << "take_until_match:token'" << token << "'"
                       << std::endl;
#endif
            if (token == until)
                break;
            ss << token;
            ++token_iterator;
        }
        auto s = ss.str();

        boost::algorithm::trim(s);
#if LOG_VERBOSE
        std::wcout << "take_until_match:return'" << s << "'" << std::endl;
#endif
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

    std::vector<Parsing::Token> consume_tokens(tokenizer& tokens,
                                               std::wostream& logger) {
        std::vector<Parsing::Token> parsed_tokens{};

        Parsing::Token token_state = Parsing::None{};
    restart:

        auto iter = tokens.begin();
        while (!iter.at_end()) {
            auto token = *iter;
            ++iter;
            std::wcout << "token is read, iterator.next()" << std::endl;
            // for (const auto& token : tokens) {
            logger << "token: '" << token << "'" << std::endl;
            token_state = match_token(token_state, token, logger);

#ifndef USE_APPLY
            if (std::holds_alternative<Parsing::Error>(token_state)) {
                auto error = std::get<Parsing::Error>(token_state);
                logger << "\nERROR\n'" << error.message << "'\n" << std::endl;
                // goto restart;
            } else if (std::holds_alternative<Parsing::String>(token_state)) {
                std::wcout << "token is string" << std::endl;
                auto string = std::get<Parsing::String>(token_state);
                string.content = take_until_match(L"\"", iter);
                parsed_tokens.push_back(string);

                token_state = Parsing::None{};
                // goto restart;
            } else if (std::holds_alternative<Parsing::Comment>(token_state)) {
                std::wcout << "token is start-of-comment" << std::endl;
                auto comment = std::get<Parsing::Comment>(token_state);
                auto result = take_until_match(L"\n", iter);
                std::wcout << "full comment '" << result << "'" << std::endl;
                comment.content = result;
                parsed_tokens.push_back(comment);

                token_state = Parsing::None{};
                // goto restart;
            } else {
                std::wcout << "unmatched token '" << token << "'" << std::endl;
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
        return std::move(parsed_tokens);
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
        std::cout << "\ntokens (" << tokens.size() << ") parsed!\n"
                  << std::endl;

        for (const auto& token : tokens) {
            std::wcout << "token: " << to_string(token) << std::endl;
        }

        return std::make_shared<Node>();
    }

} // namespace ui

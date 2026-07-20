#include <algorithm>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string_view>
#include <memory>
#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include <utils/Env/Env.hpp>
#include <utils/Config/ConfigParser.hpp>

namespace Utils::Config {
    using std::ranges::find_if;
    using std::ranges::find;
    using std::filesystem::exists;
    using std::filesystem::is_regular_file;
    using std::exception;
    using std::invalid_argument;
    using std::ifstream;
    using std::filesystem::file_size;
    using std::ios;
    using std::string_view;
    using std::shared_ptr;
    using std::make_shared;
    using std::streamsize;
    using std::stoll;
    using std::cout;
    using std::endl;
    using std::pair;
    using std::make_pair;
    using nlohmann::json;
    using Utils::Env::Get;

    pair<shared_ptr<invalid_argument>, bool> parse_config(
        const int argc,
        const char* argv[],
        vector<Argument>& arguments
        ) {

        const vector<string> argv_string(argv, argv + argc);

        if (find_if(argv_string, [](const string& argument) {
            static const vector<string> HELP_KEYS{"-h", "--help", "help"};
            return find(HELP_KEYS, argument) != HELP_KEYS.end();
        }) != argv_string.end()) {
            return {nullptr, true};
        }

        map<string, string> cli_args;
        for (auto cli_arg = argv_string.begin(); cli_arg != argv_string.end(); ++cli_arg) {
            if (!cli_arg->starts_with("--")) {
                continue;
            }
            const string arg_name = cli_arg->substr(2);
            if (cli_arg + 1 == argv_string.end()) {
                return {make_shared<invalid_argument>(fmt::format("Argument {} not found", *cli_arg)), false};
            }
            cli_args[arg_name] = *(cli_arg + 1);
        }

        json env_json = json::object();
        if (cli_args.contains("env-file")) {
            const string& file_name = cli_args.at("env-file");
            if (!is_regular_file(file_name)) {
                return {make_shared<invalid_argument>(fmt::format("Env file {} read access denied!", file_name)), false};
            }
            ifstream file_stream(file_name, ios::binary);
            streamsize size = static_cast<streamsize>(file_size(file_name));
            string file_content(static_cast<size_t>(size), '\0');
            file_stream.read(file_content.data(), size);

            if (!json::accept(file_content)) {
                return {make_shared<invalid_argument>(fmt::format("Failed to parse json in env file {}!", file_name)), false};
            }

            env_json = json::parse(file_content);
            if (!env_json.is_object()) {
                return {make_shared<invalid_argument>(fmt::format("Env file {} is not object!", file_name)), false};
            }
        }

        for (Argument& argument : arguments) {
            if (argument.is_required
                && Get(argument.name).empty()
                && !env_json.contains(argument.name)
                && !cli_args.contains(argument.name)
            ) {
                return {make_shared<invalid_argument>(fmt::format("Required argument {} not found!", argument.name)), false};
            }

            string arg_string = argument.default_value;
            if (!Get(argument.name).empty()) {
                arg_string = Get(argument.name);
            }
            if (env_json.contains(argument.name)) {
                if (argument.long_long_value != nullptr) {
                    if (!env_json.at(argument.name).is_number_integer()) {
                        return {make_shared<invalid_argument>(fmt::format("Argument {} is not a number!", argument.name)), false};
                    }
                    *argument.long_long_value = env_json[argument.name].get<long long>();
                }
                if (argument.string_value != nullptr) {
                    if (!env_json.at(argument.name).is_string()) {
                        return {make_shared<invalid_argument>(fmt::format("Argument {} is not a string!", argument.name)), false};
                    }
                    *argument.string_value = env_json[argument.name].get<string>();
                }
                if (argument.vector_long_long_value != nullptr) {
                    if (!env_json.at(argument.name).is_array()) {
                        return {make_shared<invalid_argument>(fmt::format("Argument {} is not array!", argument.name)), false};
                    }
                    try {
                        *argument.vector_long_long_value = env_json[argument.name].get<vector<long long>>();
                    } catch (const exception&) {
                        return {make_shared<invalid_argument>(fmt::format("Failed to parse array of integer argument {}!", argument.name)), false};
                    }
                }
                continue;
            }
            if (cli_args.contains(argument.name)) {
                arg_string = cli_args[argument.name];
            }
            if (argument.long_long_value != nullptr) {

                try {
                    *argument.long_long_value = stoll(arg_string);
                } catch (const exception& exc) {
                    return {make_shared<invalid_argument>(fmt::format("Failed to parse integer argument {} ({})", argument.name, exc.what())), false};
                }
                continue;
            }
            if (argument.string_value != nullptr) {
                *argument.string_value = arg_string;
                continue;
            }
            if (argument.vector_long_long_value != nullptr) {
                if (!json::accept(arg_string)) {
                    return {make_shared<invalid_argument>(fmt::format("Failed to parse array of integer argument {}", argument.name)), false};
                }
                try {
                    *argument.vector_long_long_value = json::parse(arg_string).get<vector<long long>>();
                } catch (const exception&) {
                    return {make_shared<invalid_argument>(fmt::format("Failed to parse array of integer argument {}!", argument.name)), false};
                }
                continue;
            }
            cout << argument.name << "=";
            if (argument.string_value != nullptr) {
                cout << *argument.string_value;
            } else if (argument.long_long_value != nullptr) {
                cout << *argument.long_long_value;
            } else if (argument.vector_long_long_value != nullptr) {
                cout << json(*argument.vector_long_long_value).dump();
            }
            cout << endl;
        }

        return {nullptr, false};
    }

    string get_help_by_vector_arguments(string_view service, const vector<Argument>& arguments) {
        string help = fmt::format("Service: {}\nArguments:\n  --env-file <file.json>", service);
        for (const Argument& argument : arguments) {
            help += fmt::format(
                "\n\n  --{} <{}>{}\n    required: {}{}",
                argument.name,
                (
                    argument.string_value != nullptr ? "STRING"
                    : argument.long_long_value != nullptr ? "INT"
                    : argument.vector_long_long_value != nullptr ? "ARRAY_INT"
                    : "UNKNOWN"
                ),
                (!argument.description.empty() ? "\n    description: " + argument.description : ""),
                argument.is_required ? "true" : "false",
                (!argument.is_required ? "\n    default: " + argument.default_value : "")
            );
        }
        return help;
    }
}
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


namespace po = boost::program_options;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;


struct transition
{
    std::string state_from;
    std::string state_to;
    std::string label;

    bool valid() const
    {
        return !state_from.empty() && !state_to.empty();
    }

    void fix_names()
    {
        fix_name(state_from);
        fix_name(state_to);
        const size_t pos = label.rfind(':');
        if (pos != std::string::npos) {
            label = label.substr(pos+1);
        }
    }

    void clear()
    {
        state_from = "";
        state_to = "";
        label = "";
    }

private:
    void fix_name(std::string& name)
    {
        boost::algorithm::erase_all(name, "state_type::");
        boost::algorithm::replace_all(name, "::", "__");
    }
};

struct fsm_def
{
    std::vector<transition> transitions;
    std::string name;
    std::vector<std::string> general_options;

    fsm_def()
        : general_options({"node [shape = circle]"})
    {

    }



    std::vector<std::string> get_states() const
    {
        std::vector<std::string> states;
        for (const auto& transition : transitions) {
            states.push_back(transition.state_from);
            states.push_back(transition.state_to);
        }
        std::sort(states.begin(), states.end());
        auto last = std::unique(states.begin(), states.end());
        states.erase(last, states.end());
        return states;
    }

    bool empty() const { return transitions.empty(); }
};


inline std::ostream& operator <<(std::ostream& os, const transition& t)
{
    //Example: off -> start_close [label="power toggle (on)"]
    os << t.state_from << " -> " << t.state_to
       << "  [label=\"" << t.label << "\"];"
       ;
    return os;
}

inline std::ostream& operator <<(std::ostream& os,	fsm_def& sm)
{
    os << "digraph " << sm.name << " {\n";
    for (const auto& state : sm.get_states()) {
        os << "\t" << state << ";\n";
    }
    os << "\n";
    for (const auto& option : sm.general_options) {
        os << "\t" << option << ";\n";
    }
    os << "\n";

    for (const auto& transition : sm.transitions) {
        os << "\t" << transition << "\n";
    }
    os << "}\n";
    return os;
}

template <typename Iterator>
void parse_cpp_fsm_part(fsm_def& sm, Iterator first, Iterator last)
{
    if (first == last) {
        return;
    }

    using namespace qi::labels;
    using qi::lit;
    using qi::lexeme;
    using ascii::char_;
    using ascii::string;
    using qi::eol;
    using qi::eoi;

    using qi::double_;
    using qi::phrase_parse;
    using qi::_1;
    using ascii::space;
    using phoenix::push_back;
    using phoenix::at_c;


    qi::rule<Iterator, std::string(), ascii::space_type> text;
    qi::rule<Iterator, std::string(), ascii::space_type> transitions_declaration;
    qi::rule<Iterator, std::string(), ascii::space_type> transitions_from;
    qi::rule<Iterator, std::string(), ascii::space_type> switch_to;
    qi::rule<Iterator, std::string(), ascii::space_type> when;
    qi::rule<Iterator, std::string(), ascii::space_type> when_mem_fun;
    qi::rule<Iterator, std::string(), ascii::space_type> when_lambda;
    qi::rule<Iterator, std::string(), ascii::space_type> quoted_string;

    transition cur_transition;
    auto transitions_from_action = [&](const std::string& s) {
        cur_transition.clear();
        cur_transition.state_from = s;
//        std::cout << "transitions_from_action: '" << s << "'" << std::endl;
    };

    auto switch_to_action = [&](const std::string& s)
    {
        cur_transition.state_to = s;
    };

    auto when_action = [&](const std::string& s)
    {
        cur_transition.label = s;
        cur_transition.fix_names();
        sm.transitions.push_back(cur_transition);
    };

    auto transitions_parsed_action = [&](const std::string&)
    {
    };

    quoted_string %= lexeme['"' >> +(char_ - '"') >> '"'];
    transitions_from =
            *(char_ - 't')
            >> lit("transitions_from")
            >> '('
            >> lexeme[+(char_ - ')')       [_val += _1]]
            >> ')'
    ;

    switch_to =
           lit("switch_to")
            >> '('
            >> lexeme[+(char_ - ')')       [_val += _1]]
            >> ')'
    ;

    when_mem_fun =
           lit("when")
            >> '('
            >> '&' >> lexeme[+(char_ - ')') [_val += _1]]
            >> ')'
    ;

    when_lambda =
           lit("when")
            >> '('
            >> quoted_string [_val += _1]
            >> ','
    ;

    when =
           when_lambda | when_mem_fun
    ;

    transitions_declaration =
        transitions_from[transitions_from_action]
        >> *( '.' >> switch_to[switch_to_action] >> '.' >> when[when_action])
        >> *(char_ - ';')
        >> char_(';')
    ;

    bool parsed_ok = phrase_parse(first, last,

        //  Begin grammar
        (
                transitions_declaration[transitions_parsed_action]
        )
        ,
        //  End grammar

        space);

    if (!parsed_ok) { // fail if we did not get a full match
        std::string::iterator some = first+80;
        std::string context(first, (some>last)?last:some);
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed\n";
        std::cout << "stopped at: \": " << context << "...\"\n";
        std::cout << "-------------------------\n";
    }
}

fsm_def fsm_from_file(const boost::filesystem::path& input_file_path)
{
    namespace ba = boost::algorithm;
    fsm_def sm;
    std::ifstream file (input_file_path.native());
    if (file.is_open()) {
        std::cout << "Reading statemachine from: " << input_file_path.native() << std::endl;
        std::string line;
        std::string transistions_chunk ("");
        bool in_transistions_chunk = false;
        while (!file.eof()) {
            std::getline(file, line);
            boost::algorithm::trim(line);
            if (ba::contains(line, ".transitions_from")) {
                if (in_transistions_chunk) {
                    if (!transistions_chunk.empty()) {
                        transistions_chunk = "   " + transistions_chunk + "  ;";
//                        std::cout << "--- parse transistions_chunk ---\n'" << transistions_chunk << "'" << std::endl;
                        parse_cpp_fsm_part(sm, transistions_chunk.begin(), transistions_chunk.end());
                    }
                    transistions_chunk.clear();
                }
                in_transistions_chunk = true;
            }
            else if ( ba::contains(line, ".end_transitions") ||
                 ba::contains(line, ".otherwise_loop") ||
                 line.empty()
               ) {
                in_transistions_chunk = false;
                if (!line.empty()) {
                    transistions_chunk.append(line + "\n");
                }
                if (!transistions_chunk.empty()) {
                    transistions_chunk = "   " + transistions_chunk + "  ;";
//                    std::cout << "--- parse transistions_chunk ---\n'" << transistions_chunk << "'" << std::endl;
                    parse_cpp_fsm_part(sm, transistions_chunk.begin(), transistions_chunk.end());
                }
                transistions_chunk.clear();
            }
            if (in_transistions_chunk) {
                if (!line.empty()) {
                    transistions_chunk.append(line + "\n");
                }
            }
        }
    }
    else {
        std::cout << "Unable to open file: " << input_file_path.native() << std::endl;
        exit(1);
    }

    return sm;
}

int main(int argc, const char *argv[])
{
    po::variables_map vm;
    using namespace std;

    const auto source_dir = boost::filesystem::path(__FILE__).parent_path();
    const auto default_in_file_path = source_dir / "state_machine_playground.cpp";
    std::string file_data;

    try {
        // Declare a group of options that will be allowed only on command line
        po::options_description basic("Basic options");
        basic.add_options()
                ("version,v",   "print version string")
                ("help,h",      "produce help message")
                ;

        // Declare a group of options that will be
        // allowed both on command line and in config file
        po::options_description config("Configuration");
        config.add_options()
                ("file,f", po::value<std::string>()->default_value(default_in_file_path.string()), "C++ file with state machine definition")
                ("dot-stdout", "Write dot file also to stdout")
                ("dot-path,dot", po::value<std::string>()->default_value("dot"), "Full path (or name if in PATH) of the dot executable")
                ("output-dir,dir", po::value<std::string>()->default_value(""), "Output directory for dot and image file")
                ("output,o", po::value<std::string>()->default_value(""), "Output file name(s) for .dot file and image file")
                ("image-ext,ext", po::value<std::string>()->default_value("png"), "Image extension")
                ;

        // Hidden options, will be allowed both on command line and
        // in config file, but will not be shown to the user.
        // Hidden options are those without argument names (unanmed arguments).
        // In this case we use them for the php script to execute along
        // with the command line parameters for this script.
        po::options_description hidden("Hidden options");
        hidden.add_options()
                ("dot-params", po::value< std::vector<std::string> >(), "(Graphviz) dot parameters")
                ;


        po::options_description cmdline_options;
        cmdline_options.add(basic).add(config).add(hidden);

        po::options_description config_file_options;
        config_file_options.add(config).add(hidden);

        po::options_description visible("Allowed options");
        visible.add(basic).add(config);

        po::positional_options_description p;
        p.add("dot-params", -1);

        po::store(po::command_line_parser(argc, argv).options(cmdline_options).positional(p).run(), vm);
        po::notify(vm);

        notify(vm);

        if (vm.count("help")) {
            std::cout << visible << "\n";
            return 0;
        }

        if (vm.count("version")) {
            std::cout << "fsm2dot version 1.0" << std::endl;
            return 0;
        }

        // PHP miner script and parameters for this
        if (vm.count("dot-params")) {
        }

        auto input_file_path = boost::filesystem::path(vm["file"].as<std::string >());
        {
            fsm_def sm = fsm_from_file(input_file_path);

            const auto dot_file_path   = input_file_path.replace_extension("gv");
            const auto image_file_path = input_file_path.replace_extension(vm["image-ext"].as<std::string >());
            {
                std::cout << "Writing dot file to: '" << dot_file_path.native() << "'" << std::endl;
                std::ofstream os(dot_file_path.native());
                os << sm << std::endl;
            }
            std::string dot_cmd = vm["dot-path"].as<std::string >()
                    + " -T" + vm["image-ext"].as<std::string >()
                    + " " + dot_file_path.native()
                    + " -o " + image_file_path.native();

            std::cout << "Running dot with following command:\n" << dot_cmd << std::endl;
            // dot -Tpng fsm.gv -o myFiniteStateMachine.png

            if (std::system(dot_cmd.c_str()) != 0) {
                std::cout << "Error when running dot!" << std::endl;
                return 1;
            }
            if (vm.count("dot-stdout")) {
                std::cout << sm << std::endl;
            }
        }

    }
    catch(std::exception& e)
    {
        std::cout << "Error: Unknown arguments given:" << std::endl;
        std::cout << e.what() << "\n";
        return 1;
    }
    return 0;
}

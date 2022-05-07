#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
////////////////////////////////////


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

namespace client
{
    namespace fusion = boost::fusion;
    namespace phoenix = boost::phoenix;
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;

    ///////////////////////////////////////////////////////////////////////////
    //  Our mini XML tree representation
    ///////////////////////////////////////////////////////////////////////////
    //[tutorial_xml1_structures
    struct mini_xml;

    typedef
        boost::variant<
            boost::recursive_wrapper<mini_xml>
          , std::string
        >
    mini_xml_node;

    struct mini_xml
    {
        std::string name;                           // tag name
        std::vector<mini_xml_node> children;        // children
    };
    //]
}

// We need to tell fusion about our mini_xml struct
// to make it a first-class fusion citizen
//[tutorial_xml1_adapt_structures
BOOST_FUSION_ADAPT_STRUCT(
    client::mini_xml,
    (std::string, name)
    (std::vector<client::mini_xml_node>, children)
)
//]

namespace client
{
    ///////////////////////////////////////////////////////////////////////////
    //  Print out the mini xml tree
    ///////////////////////////////////////////////////////////////////////////
    int const tabsize = 4;

    void tab(int indent)
    {
        for (int i = 0; i < indent; ++i)
            std::cout << ' ';
    }

    struct mini_xml_printer
    {
        mini_xml_printer(int indent = 0)
          : indent(indent)
        {
        }

        void operator()(mini_xml const& xml) const;

        int indent;
    };

    struct mini_xml_node_printer : boost::static_visitor<>
    {
        mini_xml_node_printer(int indent = 0)
          : indent(indent)
        {
        }

        void operator()(mini_xml const& xml) const
        {
            mini_xml_printer(indent+tabsize)(xml);
        }

        void operator()(std::string const& text) const
        {
            tab(indent+tabsize);
            std::cout << "text: \"" << text << '"' << std::endl;
        }

        int indent;
    };

    void mini_xml_printer::operator()(mini_xml const& xml) const
    {
        tab(indent);
        std::cout << "tag: " << xml.name << std::endl;
        tab(indent);
        std::cout << '{' << std::endl;

        BOOST_FOREACH(mini_xml_node const& node, xml.children)
        {
            boost::apply_visitor(mini_xml_node_printer(indent), node);
        }

        tab(indent);
        std::cout << '}' << std::endl;
    }

    ///////////////////////////////////////////////////////////////////////////
    //  Our mini XML grammar definition
    ///////////////////////////////////////////////////////////////////////////
    //[tutorial_xml1_grammar
    template <typename Iterator>
    struct mini_xml_grammar : qi::grammar<Iterator, mini_xml(), ascii::space_type>
    {
        mini_xml_grammar() : mini_xml_grammar::base_type(xml)
        {
            using qi::lit;
            using qi::lexeme;
            using ascii::char_;
            using ascii::string;
            using namespace qi::labels;

            using phoenix::at_c;
            using phoenix::push_back;

            auto transitions_from_print = [](const std::string& s) {std::cout << "transitions_from: '" << s  << "'" << std::endl; };
            auto switch_to_print = [](const std::string& s) {std::cout << "switch_to: '" << s  << "'" << std::endl; };
            auto when_print = [](const std::string& s) {std::cout << "when: '" << s  << "'" << std::endl; };

            text = lexeme[+(char_ - '<')        [_val += _1]];
            node = (xml | text)                 [_val = _1];

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

            when =
                   lit("when")
                >> '('
                >> lexeme[+(char_ - ')')       [_val += _1]]
                >> ')'
            ;

            start_tag =
                    '<'
                >>  !lit('/')
                >>  lexeme[+(char_ - '>')       [_val += _1]]
                >>  '>'
            ;

            end_tag =
                    "</"
                >>  string(_r1)
                >>  '>'
            ;

            transitions_declaration =
                    transitions_from[transitions_from_print]
                >>  *( '.' >> switch_to[switch_to_print] >> '.' >> when[when_print])
                >>  ';'
            ;

//            xml =
//                    transitions_declaration
//                    >> '}'


            xml =
                start_tag                 [at_c<0>(_val) = _1]
                >>  *node                 [push_back(at_c<1>(_val), _1)]
                >>  end_tag(at_c<0>(_val))
            ;
//            xml =
//                    start_tag                   [at_c<0>(_val) = _1]
//                >>  *node                       [push_back(at_c<1>(_val), _1)]
//                >>  end_tag(at_c<0>(_val))
//            ;
        }

        qi::rule<Iterator, mini_xml(), ascii::space_type> xml;
        qi::rule<Iterator, mini_xml_node(), ascii::space_type> node;
        qi::rule<Iterator, std::string(), ascii::space_type> text;
        qi::rule<Iterator, std::string(), ascii::space_type> start_tag;
        qi::rule<Iterator, void(std::string), ascii::space_type> end_tag;

        qi::rule<Iterator, std::string(), ascii::space_type> transitions_declaration;
        qi::rule<Iterator, std::string(), ascii::space_type> transitions_from;
        qi::rule<Iterator, std::string(), ascii::space_type> switch_to;
        qi::rule<Iterator, std::string(), ascii::space_type> when;
    };
    //]
}

///////////////////////////////////////////////////////////////////////////////
//  Main program
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    char const* filename;
    if (argc > 1)
    {
        filename = argv[1];
    }
    else
    {
        std::cerr << "Error: No input file provided." << std::endl;
        return 1;
    }

    std::ifstream in(filename, std::ios_base::in);

    if (!in)
    {
        std::cerr << "Error: Could not open input file: "
            << filename << std::endl;
        return 1;
    }

    std::string storage; // We will read the contents here.
    in.unsetf(std::ios::skipws); // No white space skipping!
    std::copy(
        std::istream_iterator<char>(in),
        std::istream_iterator<char>(),
        std::back_inserter(storage));

    typedef client::mini_xml_grammar<std::string::const_iterator> mini_xml_grammar;
    mini_xml_grammar xml; // Our grammar
    client::mini_xml ast; // Our tree

    using boost::spirit::ascii::space;
    std::string::const_iterator iter = storage.begin();
    std::string::const_iterator end = storage.end();
    bool r = phrase_parse(iter, end, xml, space, ast);

    if (r && iter == end)
    {
        std::cout << "-------------------------\n";
        std::cout << "Parsing succeeded\n";
        std::cout << "-------------------------\n";
        client::mini_xml_printer printer;
        printer(ast);
        return 0;
    }
    else
    {
        std::string::const_iterator some = iter+30;
        std::string context(iter, (some>end)?end:some);
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed\n";
        std::cout << "stopped at: \": " << context << "...\"\n";
        std::cout << "-------------------------\n";
        return 1;
    }
}



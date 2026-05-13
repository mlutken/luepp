#include "uri.h"

using namespace std;
using namespace std::string_view_literals;

namespace lue::concepts {

const string& uri::protocol() const
{
    return protocol_;
}


} // namespace lue::concepts

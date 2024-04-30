module;

module Print;

import <string>;

namespace Print
{
    void PrintMsg(const std::string& msg)
    {
        printf("%s\n", msg.c_str());
    }
}

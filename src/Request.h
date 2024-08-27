#ifndef __REQUEST_H
#define __REQUEST_H

#include <vector>
#include <functional>

using namespace std;

namespace RAMbench
{

    class Request
    {
    public:
        long unsigned addr;
        enum class Type
        {
            READ,
            WRITE,
            MAX
        } type;
    };

} /*namespace RAMbench*/

#endif /*__REQUEST_H*/

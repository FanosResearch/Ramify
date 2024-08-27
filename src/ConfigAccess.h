#ifndef __CONFIGACCESS_H
#define __CONFIGACCESS_H

#include <stdio.h>
#include <cstring>
#include <map>

namespace RAMbench
{

    class ConfigAccess
    {
    public:
        ConfigAccess(){};

        void setTranSize(int transaction_size) { m_transaction_size = transaction_size; };
        int getTranSize() const { return m_transaction_size; };

        void setAccessMode(std::string access_mode) { m_access_mode = access_mode; };
        std::string getAccessMode() const { return m_access_mode; };

        void setAddrFormat(std::string addr_format) { m_addr_format = addr_format; };
        std::string getAddrFormat() const { return m_addr_format; };

        void setReadFormat(std::string read_format) { m_read_format = read_format; };
        std::string getReadFormat() const { return m_read_format; };

        void setWriteFormat(std::string write_format) { m_write_format = write_format; };
        std::string getWriteFormat() const { return m_write_format; };

    private:
        int m_transaction_size;

        std::string m_access_mode;
        std::string m_addr_format;
        std::string m_read_format;
        std::string m_write_format;
    };

};

#endif /* _CONFIGACCESS_H */

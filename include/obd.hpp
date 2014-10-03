#include <boost/asio.hpp>
#include <string>

#include "easylogging++.h"

class obd2 {
    public:
        obd2(std::string port);
        void setProtocol(int proto);
        void init();
        std::string getProtocol();
        void writePid(int mode, int pid);
        void write(std::string s);
        std::string readLine();
    private:
        boost::asio::io_service io;
        boost::asio::serial_port serial;
};

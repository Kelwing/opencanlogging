#include <sstream>
#include "include/obd.hpp"

obd2::obd2(std::string port) : io(), serial(io,port){
    serial.set_option(boost::asio::serial_port_base::baud_rate(115200));
    serial.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
    setProtocol(0);
    LOG(INFO) << "Protocol selected: " << getProtocol();
}

/**
 * Set Protocol
 * @param proto The protocol to set to, 0 for automatic
 */
void obd2::setProtocol(int proto){
    std::stringstream ss;
    ss << "ATZ"; // reset the device
    this->write(ss.str());
    readLine();
    ss.str("");
    ss << "ATE0"; // Turn echo off (damn is this annoying)
    this->write(ss.str());
    ss.str("");
    ss << "ATSP" << proto;
    this->write(ss.str());
}

std::string obd2::getProtocol(){
    this->write("ATDP"); // Get the protocol
    return readLine();
}

void obd2::writePid(int mode, int pid){
    char buf[5];
    sprintf(buf,"%02d%02d",mode,pid);
    std::stringstream ss;
    ss << buf;
    this->write(ss.str());
}

void obd2::write(std::string s){
    LOG(DEBUG) << "Writing to serial: " << s;
    s = s + '\r';
    boost::asio::write(serial,boost::asio::buffer(s.c_str(),s.size()));
}

std::string obd2::readLine() {
    using namespace boost;
    char c;
    std::string result;
    for(;;)
    {
        asio::read(serial,asio::buffer(&c,1));
	//LOG(DEBUG) << c;
        switch(c)
        {
            case '>':
                break;
            case '\r':
                return result;
            default:
                result+=c;
        }
    }
}



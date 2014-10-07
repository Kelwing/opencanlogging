#include <sstream>
#include "include/obd.hpp"

obd2::obd2(std::string port) : io(), serial(io,port){
    serial.set_option(boost::asio::serial_port_base::baud_rate(115200));
    serial.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
    init();
    setProtocol(0);
    LOG(INFO) << "Protocol selected: " << getProtocol();
}

/**
 * Set Protocol
 * @param proto The protocol to set to, 0 for automatic
 */
void obd2::setProtocol(int proto){
    LOG(INFO) << "Setting protocol to " << proto;
    std::stringstream ss;
    ss << "ATSP" << proto;
    this->write(ss.str());
    LOG(DEBUG) << readLine();
}

void obd2::init(){
    LOG(INFO) << "Initializing ELM device";
    std::stringstream ss;
    ss << "ATZ";
    this->write(ss.str());
    sleep(2);
    LOG(DEBUG) << readLine();
    ss.str("");
    ss << "ATE0";  // Turn echo off
    this->write(ss.str());
    LOG(DEBUG) << readLine();
}

std::string obd2::getProtocol(){
    this->write("ATDP"); // Get the protocol
    return readLine();
}

void obd2::writePid(int mode, int pid){
    char buf[5];
    sprintf(buf,"%02X%02X",mode,pid);
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
    int count;
    std::string result;
    for(;;)
    {
        count += asio::read(serial,asio::buffer(&c,1));
        if(c == '>' && count > 0)
            break;
        else if(c != '>' && c != ' ')
            result.append(&c, 1);
    }
    return result;
}

unsigned int reverseBits(unsigned int num)
{
    unsigned int  NO_OF_BITS = sizeof(num) * 8;
    unsigned int reverse_num = 0, i, temp;

    for (i = 0; i < NO_OF_BITS; i++)
    {
        temp = (num & (1 << i));
        if(temp)
            reverse_num |= (1 << ((NO_OF_BITS - 1) - i));
    }

    return reverse_num;
}

vector<int> dumpPids(){
    string output;
    writePid(0x01, 0x00);
    output = readLine();
    vector<int> pid_vector;
    output = removeSpaces(output);
    cout << "output " << output << endl;
    long pids_available = stol(output, 0, 16);
    int pid = 0x21;
    for(int i = 0x00; i < 0x20; i++){
        pid--;
        if(((pids_available & (1 << i)) >> i) == 1){ 
            pid_vector.push_back(pid);
            cout << "Found param " << hex << pid << endl;
        }
    }
    return pid_vector;
}

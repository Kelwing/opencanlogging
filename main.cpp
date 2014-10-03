#include <boost/program_options.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>  /* String function definitions */
#include <iostream>
#include "include/easylogging++.h"
#include "include/obd.hpp"

using namespace std;

_INITIALIZE_EASYLOGGINGPP
obd2 * serial;

void TimerFire(boost::asio::deadline_timer * t){
    serial->writePid(0x01, 0x0C);
    LOG(INFO) << serial->readLine();    
    t->expires_at(t->expires_at() + boost::posix_time::milliseconds(200));
    t->async_wait(boost::bind(TimerFire,t));
}

int main(int argc, char ** argv){
    el::Configurations conf("opencanlog.log");
    el::Loggers::reconfigureAllLoggers(conf);
    LOG(INFO) << "Starting OpenCANLogging";
    namespace po = boost::program_options;
    po::options_description desc("Options");
    desc.add_options()
        ("device,d", po::value<string>(), "Device");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);


    if(vm.count("device")) {
        serial = new obd2(vm["device"].as<string>());
    } else {
        LOG(ERROR) << "No device specified";
    }

    boost::asio::io_service io;
    boost::asio::deadline_timer t(io,boost::posix_time::seconds(1));
    t.async_wait(boost::bind(TimerFire,&t));
    io.run();
    return 0;
}

#include <boost/program_options.hpp>
#include <string>  /* String function definitions */
#include <iostream>
#include "include/easylogging++.h"
#include "include/obd.hpp"

using namespace std;

_INITIALIZE_EASYLOGGINGPP

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
        obd2 serial(vm["device"].as<string>());
    } else {
        LOG(ERROR) << "No device specified";
    }

    return 0;
}

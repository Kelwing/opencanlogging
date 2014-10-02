#include <boost/program_options.hpp>
#include <string>  /* String function definitions */
#include <iostream>
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

using namespace std;

/*
 *  'open_port()' - Open serial port 1.
 *   
 *     Returns the file descriptor on success or -1 on error.
 */

int open_port(char * device)
{
    int fd; /* File descriptor for the port */


    fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)
    {
        /*
         * Could not open the port.
         */

        perror("open_port");
    }
    else
        fcntl(fd, F_SETFL, 0);

    struct termios options;

    /*
     * Get the current options for the port...
     */

    tcgetattr(fd, &options);

    /*
     * Set the baud rates to 19200...
     */

    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);

    /*
     * Enable the receiver and set local mode...
     */

    options.c_cflag |= (CLOCAL | CREAD);

    /*
     * Set the new options for the port...
     */

    tcsetattr(fd, TCSANOW, &options);

    return (fd);
}

int main(int argc, char ** argv){
    namespace po = boost::program_options;
    po::options_description desc("Options");
    desc.add_options()
        ("device,d", po::value<string>(), "Device");
}

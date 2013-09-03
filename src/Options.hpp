
#ifndef OPTIONS
#define OPTIONS

#include <string>

class Options
{
    public:
        static bool handleFlags(int argc, char** argv);

    private:
        Options();
        static bool highVerbosity();
        static std::string getConnectionIP();
        static std::string getConnectionPort();
        static std::string getPassword();

        static bool bounceSnapshots();
        static bool cycleSnapshots();
        static int renderMode();
        static std::string connectionPassword();

    private:
        static bool highVerbosity_;
};

#endif

/*
The CLI options should be implemented:
--help [string] Print help screen or help on a particular option and exit.
--license Print license information and quit.
-v or --verbose Increase verbosity level
--version Print application version and exit.

Configuration options in the format "--< option > < value >" or "--< option >=< value >" or "--< option >"
--connect < string=127.0.0.1:36330 > An address and port to connect to in the form :
--bounce-snapshots < boolean=false > When the last snapshot is reached, should the animation run backwards or not.
--cycle-snapshots < boolean=true > Iterate through all available snapshots.
--mode < integer=4 > Render mode.
--password < string= > A password for accessing the remote client.*/

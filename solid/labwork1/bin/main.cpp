#include "../lib/file_stats.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        HelpDisplayer::showUsage(argv[0]);
        return 1;
    }

    FileStatsApplication app;
    app.run(argc, argv);

    return 0;
}
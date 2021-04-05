#include "rizen/rizen.h"

int main()
{
    App app;
    AppInfo info;
    app.init(info);

    while(!app.onQuit()) {
        app.update();
    }

    return 0;
}
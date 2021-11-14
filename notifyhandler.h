#ifndef NOTIFYHANDLER_H
#define NOTIFYHANDLER_H

#include <iostream>
#include "win-toast/wintoastlib.h"

using namespace WinToastLib;

class NotifyHandler : public IWinToastHandler {

public:
    void toastActivated() const {
        std::wcout << L"The user clicked in this toast" << std::endl;
    }

    void toastActivated(int actionIndex) const {
        (void)actionIndex;
    }

    void toastFailed() const {
        std::wcout << L"Error showing current toast" << std::endl;
    }

    void toastDismissed(WinToastDismissalReason state) const {
        switch (state) {
        case UserCanceled:
            std::wcout << L"The user dismissed this toast" << std::endl;
            break;
        case ApplicationHidden:
            std::wcout <<  L"The application hid the toast" << std::endl;
            break;
        case TimedOut:
            std::wcout << L"The toast has timed out" << std::endl;
            break;
        default:
            std::wcout << L"Toast not activated" << std::endl;
            break;
        }
    }
};

#endif // NOTIFYHANDLER_H

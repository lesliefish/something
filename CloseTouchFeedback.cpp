#include "CloseTouchFeedback.h"
#include <Windows.h>
#include <WinUser.h>

CloseTouchFeedback::CloseTouchFeedback(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    bool b = false;
    SetWindowFeedbackSetting((HWND)winId(), FEEDBACK_TOUCH_DOUBLETAP, 0, 0, &b);
    SetWindowFeedbackSetting((HWND)winId(), FEEDBACK_TOUCH_CONTACTVISUALIZATION, 0, 0, &b);
    SetWindowFeedbackSetting((HWND)winId(), FEEDBACK_GESTURE_PRESSANDTAP, 0, 0, &b);
    SetWindowFeedbackSetting((HWND)winId(), FEEDBACK_TOUCH_TAP, 0, 0, &b);
    SetWindowFeedbackSetting((HWND)winId(), FEEDBACK_TOUCH_PRESSANDHOLD, 0, 0, &b);
}

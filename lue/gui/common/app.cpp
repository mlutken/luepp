#include "app.h"

#include <gui/common/platform_qt6/app_impl.h>

using namespace std;

namespace lue::gui::common {

app::app(int& argc, char* argv[]) :
    lue::system::core_app{argc, argv},
    pimpl_{make_unique<lue::gui::common::app_impl>(argc, argv)}
{}

app::~app() = default;

int app::start() {
    return pimpl_->exec();
}


// -----------------------------------
// --- Example overriding notify() ---
// -----------------------------------

// // Override the notify() method to customize event handling
// bool notify(QObject *receiver, QEvent *event) override {
//     // Example: Log all mouse press events
//     if (event->type() == QEvent::MouseButtonPress) {
//         qDebug() << "Mouse pressed on:" << receiver;
//     }
//     return QApplication::notify(receiver, event);
// }


} // namespace lue::gui::common


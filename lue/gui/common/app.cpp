#include "app.h"



namespace lue::gui::common {

app::app(int& argc, char* argv[])
    : lue::system::app{argc, argv}
{}



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


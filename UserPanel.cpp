#include "UserPanel.h"

UserPanel::UserPanel(QWidget* parent)
    : QWidget(parent) {

    setWindowTitle("User Panel");

    bookFlightButton = new QPushButton("Book Flight", this);
    cancelBookingButton = new QPushButton("Cancel Booking", this);
    viewBookedFlightsButton = new QPushButton("View Booked Flights", this);
    findFlightButton = new QPushButton("Find Flight", this);
    logoutButton = new QPushButton("Logout", this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(bookFlightButton);
    layout->addWidget(cancelBookingButton);
    layout->addWidget(viewBookedFlightsButton);
    layout->addWidget(findFlightButton);
    layout->addWidget(logoutButton);

    connect(logoutButton, &QPushButton::clicked, this, &UserPanel::logoutRequested);
}

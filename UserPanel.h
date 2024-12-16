#ifndef USERPANEL_H
#define USERPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class UserPanel : public QWidget {
    Q_OBJECT

public:
    UserPanel(QWidget* parent = nullptr);

signals:
    void logoutRequested();

private:
    QPushButton* bookFlightButton;
    QPushButton* cancelBookingButton;
    QPushButton* viewBookedFlightsButton;
    QPushButton* findFlightButton;
    QPushButton* logoutButton;
};

#endif // USERPANEL_H

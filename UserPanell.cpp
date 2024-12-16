#include "UserPanell.h"
#include "ui_UserPanell.h"
#include <iostream>

UserPanell::UserPanell(DataBase* db, int userId, QWidget* parent)
    : BasePanel(db, parent), ui(new Ui::UserPanell), currentRow(-1), userId(userId) {
    ui->setupUi(this);
    setWindowTitle("Flightmanager");
    loadData("Flights");
    setupTableView(ui->tableView);
    bookedFlightsQueue = new Queue<Flight>();
    bookedFlightsWindow = new BookedFlightsWindow(bookedFlightsQueue, database, userId);
    auto bookedFlights = database->getBookedFlights(userId);
    for (const auto& flight : bookedFlights) {
        bookedFlightsQueue->enqueue(flight);
    }
}

UserPanell::~UserPanell() {
    delete ui;
}

void UserPanell::on_BookFlight_clicked() {
    try {
        if (currentRow < 0) {
            throw Warning("No flight selected for booking.");
        }
        QModelIndex proxyIndex = proxyModel->index(currentRow, 0);
        QModelIndex sourceIndex = proxyModel->mapToSource(proxyIndex);
        int flightNumber = tableModel->data(tableModel->index(sourceIndex.row(), 0)).toInt();
        QString destination = tableModel->data(tableModel->index(sourceIndex.row(), 1)).toString();
        int day = tableModel->data(tableModel->index(sourceIndex.row(), 2)).toInt();
        int month = tableModel->data(tableModel->index(sourceIndex.row(), 3)).toInt();
        int time = tableModel->data(tableModel->index(sourceIndex.row(), 4)).toInt();
        int seatsAvailable = tableModel->data(tableModel->index(sourceIndex.row(), 5)).toInt();

        if (seatsAvailable > 0) {
            int updatedSeats = seatsAvailable - 1;
            tableModel->setData(tableModel->index(sourceIndex.row(), 5), updatedSeats);
            Flight updatedFlight(flightNumber, destination.toStdString(), day, month, time, updatedSeats);
            database->editFlight(updatedFlight);
            bookedFlightsQueue->enqueue(updatedFlight);
            database->bookSeat(flightNumber, userId);

            QMessageBox::information(nullptr, "Information", "Flight booked successfully");
        } else {
            throw GeneralException("No seats available for flight.");
        }
    } catch (const Warning& e) {
        e.showErrorMessage();
    } catch (const GeneralException& e) {
        e.showErrorMessage();
    }
}

void UserPanell::on_ListABooked_clicked() {
    if (bookedFlightsQueue->isEmpty()) {
        QMessageBox::information(this, "No Booked Flights", "You have not booked any flights yet.");
        return;
    }
    bookedFlightsWindow->loadBookedFlights();
    bookedFlightsWindow->show();
}

void UserPanell::on_tableView_clicked(const QModelIndex& index) {
    currentRow = index.row();
}

void UserPanell::on_UpdateButton_clicked() {
    tableModel->select();
}

void UserPanell::on_logoutButton_clicked() {
    logoutRequested();
}

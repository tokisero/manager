#pragma once

#include "Flight.h"
#include <vector>
#include <QString>
#include <QSqlDatabase>
#include "ManagerException.h"

class DataBase {
private:
    QString filename;
    QSqlDatabase db;
    void close();
    void initialize();

public:
    explicit DataBase(const QString& file);
    void open();
    ~DataBase();
    std::vector<Flight> readFromFile() const;
    void writeToFile(const std::vector<Flight>& flights) const;
    void addFlight(const Flight& flight) const;
    void editFlight(const Flight& flight) const;
    void deleteFlight(int flightNumber) const;
    QSqlDatabase& getDatabase();
    void updateSeats(int flightNumber, int seats) const;
    std::optional<Flight> getFlight(int flightNumber) const;
    void bookSeat(int flightNumber, int userId);
    void unbookSeat(int flightNumber, int userId);
    std::vector<Flight> getBookedFlights(int userId) const;
    int getUserIdByUsername(const QString& username) const;
    bool authenticateUser(const QString& username, const QString& password) const;
    int getUserRole(const QString& username) const;
    void registerUser(const QString& username, const QString& password, int role) const;
};

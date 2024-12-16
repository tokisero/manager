#include "DataBase.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <iostream>
#include <vector>
#include <optional>

DataBase::DataBase(const QString& file) : filename(file) {
    open();
    try {
        QSqlQuery query(db);
        if (QSqlDatabase::contains("my_unique_connection")) {
            db = QSqlDatabase::database("my_unique_connection");
        } else {
            db = QSqlDatabase::addDatabase("QSQLITE", "my_unique_connection");
            db.setDatabaseName(filename);
            if (!db.open()) {
                throw DatabaseException(db.lastError().text());
            }
        }
    } catch (const DatabaseException& e) {
        e.showErrorMessage();
    }
    initialize();
}

DataBase::~DataBase() {
    close();
}

void DataBase::open() {
    try {
        if (QSqlDatabase::contains("my_unique_connection")) {
            db = QSqlDatabase::database("my_unique_connection");
        } else {
            db = QSqlDatabase::addDatabase("QSQLITE", "my_unique_connection");
            db.setDatabaseName(filename);
            if (!db.open()) {
                throw DatabaseException(db.lastError().text());
            }
        }
    } catch (const DatabaseException& e) {
        e.showErrorMessage();
        throw;
    }
}

void DataBase::close() {
    if (db.isOpen()) {
        db.close();
    }
}

void DataBase::initialize() {
    QSqlQuery query(db);
    QString createFlightTable = R"(
        CREATE TABLE IF NOT EXISTS Flights (
            flightNumber INTEGER PRIMARY KEY,
            destination TEXT NOT NULL,
            day INTEGER NOT NULL,
            month INTEGER NOT NULL,
            time INTEGER NOT NULL,
            seats INTEGER NOT NULL
        );
    )";
    query.exec(createFlightTable);

    QString createUserTable = R"(
        CREATE TABLE IF NOT EXISTS Users (
            userId INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL,
            password TEXT NOT NULL,
            role INTEGER NOT NULL DEFAULT 0
        );
    )";
    query.exec(createUserTable);

    QString createBookedFlightsTable = R"(
        CREATE TABLE IF NOT EXISTS BookedFlights (
            flightNumber INTEGER NOT NULL,
            userId INTEGER NOT NULL,
            PRIMARY KEY (flightNumber, userId),
            FOREIGN KEY (flightNumber) REFERENCES Flights(flightNumber),
            FOREIGN KEY (userId) REFERENCES Users(userId)
        );
    )";
    query.exec(createBookedFlightsTable);
}

void DataBase::registerUser(const QString& username, const QString& password, int role) const {
    try {
        QSqlQuery checkQuery(db);
        checkQuery.prepare("SELECT COUNT(*) FROM Users WHERE username = :username");
        checkQuery.bindValue(":username", username);

        if (!checkQuery.exec()) {
            throw DatabaseException(checkQuery.lastError().text());
        }

        if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {
            throw InputException("User with this username already exists.");
        }

        QSqlQuery query(db);
        query.prepare("INSERT INTO Users (username, password, role) VALUES (:username, :password, :role)");
        query.bindValue(":username", username);
        query.bindValue(":password", password);
        query.bindValue(":role", role);
        if (!query.exec()) {
            throw DatabaseException(query.lastError().text());
        }
    } catch (const ManagerException& e) {
        e.showErrorMessage();
        throw;
    }
}

int DataBase::getUserRole(const QString& username) const {
    QSqlQuery query(db);
    query.prepare("SELECT role FROM Users WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return -1;
}

bool DataBase::authenticateUser(const QString& username, const QString& password) const {
    try {
        if (!db.isOpen()) {
            throw DatabaseException("Database is not open!");
        }

        QSqlQuery query(db);
        query.prepare("SELECT password FROM Users WHERE username = :username");
        query.bindValue(":username", username);

        if (query.exec() && query.next()) {
            QString storedPassword = query.value(0).toString();
            if (storedPassword != password) {
                throw LoginException("Invalid username or password.");
            }
            return true;
        }
        return false;
    } catch (const ManagerException& e) {
        e.showErrorMessage();
        throw;
    }
}

std::vector<Flight> DataBase::getBookedFlights(int userId) const {
    std::vector<Flight> bookedFlights;
    QSqlQuery query(db);
    query.prepare("SELECT flightNumber FROM BookedFlights WHERE userId = :userId");
    query.bindValue(":userId", userId);

    if (query.exec()) {
        while (query.next()) {
            int flightNumber = query.value(0).toInt();
            auto flight = getFlight(flightNumber);
            if (flight.has_value()) {
                bookedFlights.push_back(flight.value());
            }
        }
    } else {
        throw DatabaseException(query.lastError().text());
    }
    return bookedFlights;
}

void DataBase::editFlight(const Flight& flight) const {
    try {
        QSqlQuery query(db);
        QString update = R"(
        UPDATE Flights
        SET destination = :destination, day = :day, month = :month, time = :time, seats = :seats
        WHERE flightNumber = :flightNumber;
    )";
        query.prepare(update);
        query.bindValue(":flightNumber", flight.getFlightNumber());
        query.bindValue(":destination", QString::fromStdString(flight.getDestination()));
        query.bindValue(":day", flight.getDay());
        query.bindValue(":month", flight.getMonth());
        query.bindValue(":time", flight.getTime());
        query.bindValue(":seats", flight.getSeats());

        if (!query.exec()) {
            throw DatabaseException(query.lastError().text());
        } else {
            std::cout << "Flight updated successfully." << std::endl;
        }
    } catch (const DatabaseException& e) {
        e.showErrorMessage();
    }
}

void DataBase::updateSeats(int flightNumber, int seats) const {
    try {
        QSqlQuery query(db);
        query.prepare("UPDATE Flights SET seats = :seats WHERE flightNumber = :flightNumber");
        query.bindValue(":seats", seats);
        query.bindValue(":flightNumber", flightNumber);
        if (!query.exec()) {
            throw DatabaseException(query.lastError().text());
        }
    } catch (const DatabaseException& e) {
        e.showErrorMessage();
    }
}

QSqlDatabase& DataBase::getDatabase() {
    return db;
}

void DataBase::bookSeat(int flightNumber, int userId) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO BookedFlights (flightNumber, userId) VALUES (:flightNumber, :userId)");
    query.bindValue(":flightNumber", flightNumber);
    query.bindValue(":userId", userId);

    if (!query.exec()) {
        throw DatabaseException(query.lastError().text());
    }
}

void DataBase::unbookSeat(int flightNumber, int userId) {
    QSqlQuery query(db);
    query.prepare("DELETE FROM BookedFlights WHERE flightNumber = :flightNumber AND userId = :userId");
    query.bindValue(":flightNumber", flightNumber);
    query.bindValue(":userId", userId);

    if (!query.exec()) {
        throw DatabaseException(query.lastError().text());
    }
}

int DataBase::getUserIdByUsername(const QString& username) const {
    QSqlQuery query(db);
    query.prepare("SELECT userId FROM Users WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    } else {
        std::cerr << "Error: " << query.lastError().text().toStdString() << std::endl;
        return -1;
    }
}

std::optional<Flight> DataBase::getFlight(int flightNumber) const {
    QSqlQuery query(db);
    query.prepare("SELECT flightNumber, destination, day, month, time, seats FROM Flights WHERE flightNumber = :flightNumber");
    query.bindValue(":flightNumber", flightNumber);

    if (query.exec() && query.next()) {
        int flightNum = query.value(0).toInt();
        QString destination = query.value(1).toString();
        int day = query.value(2).toInt();
        int month = query.value(3).toInt();
        int time = query.value(4).toInt();
        int seats = query.value(5).toInt();

        return Flight(flightNum, destination.toStdString(), day, month, time, seats);
    }
    return std::nullopt;
}

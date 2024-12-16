#include "Flight.h"
#include <string_view>

Flight::Flight(int flightNumber, const std::string& destination, int day, int month, int time, int seats)
    : flightNumber(flightNumber), destination(destination), day(day), month(month), time(time), seats(seats) {}

int Flight::getFlightNumber() const {
    return flightNumber;
}

const std::string& Flight::getDestination() const {
    return destination;
}

int Flight::getDay() const {
    return day;
}

int Flight::getMonth() const {
    return month;
}

int Flight::getTime() const {
    return time;
}

int Flight::getSeats() const {
    return seats;
}

void Flight::setDestination(std::string_view dest) {
    destination = dest;
}

void Flight::setMonth(const int mnth) {
    month = mnth;
}

void Flight::setDay(const int d) {
    day = d;
}

void Flight::setTime(const int tm) {
    time = tm;
}

void Flight::setSeats(int s) {
    seats = s;
}

bool Flight::bookSeat() {
    if (seats > 0) {
        seats--;
        return true;
    }
    return false;
}

bool Flight::unbookSeat() {
    seats++;
    return true;
}

bool Flight::operator==(const Flight& flight) const {
    return this->getFlightNumber() == flight.getFlightNumber();
}

std::ostream& operator<<(std::ostream& os, const Flight& flight) {
    os << "Flight number: " << flight.flightNumber << "\n"
       << "Destination: " << flight.destination << "\n"
       << "Date: " << flight.day << "." << flight.month << "\n"
       << "Time: " << flight.time << ".00\n"
       << "Seats available: " << flight.seats << "\n\n";
    return os;
}

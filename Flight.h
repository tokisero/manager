#pragma once
#include <iostream>
#include <string>
#include <QString>

class Flight {
private:
    int flightNumber;
    std::string destination;
    int day;
    int month;
    int time;
    int seats;
public:
    Flight() : flightNumber(0), destination(""), day(0), month(0), time(0), seats(0) {}
    explicit Flight(int flightNumber, const std::string& destination, int day, int month, int time, int seats);
    Flight(const Flight& other)
        : flightNumber(other.flightNumber), destination(other.destination), day(other.day),
        month(other.month), time(other.time), seats(other.seats) {
    }
    int getFlightNumber() const;
    const std::string& getDestination() const;
    int getDay() const;
    int getMonth() const;
    int getTime() const;
    int getSeats() const;
    void setDestination(std::string_view dest);
    void setDay(int d);
    void setMonth(int mnth);
    void setTime(int tm);
    void setSeats(int s);
    bool bookSeat();
    bool unbookSeat();
    bool operator==(const Flight& flight) const;
    friend std::ostream& operator<<(std::ostream& os, const Flight& flight);
};

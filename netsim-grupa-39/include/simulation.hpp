//
// Created by OEM on 07.01.2021.
//

#ifndef NET_SIMULATION_SIMULATION_HPP
#define NET_SIMULATION_SIMULATION_HPP

#include "factory.hpp"

/**
 * Funkcja symulująca zachowanie linii produkcyjnej
 * @param f - fabryka
 * @param d - całkowity czas symulacji
 * @param rf - funkcja generaująca raport. Przykład: [&spec_notifier](Factory& f, TimeOffset t_offset) {
                                                            if (spec_notifier.should_generate_report(t_offset)) {
                                                                 generate_structure_report(f, std::cout);
                                                            }
                                                        }
    (Wewnątrz symulacji wystarczy wywołać rf() aby dostać raport)
 */
void simulate(Factory& f, TimeOffset d, std::function<void (Factory &, Time)> rf);


class ReportNotifier{
public:
    /**
     * funkcja zwracająca prawdę w turach w których ma zostać wygenerowany raport
     * @param t
     * @return
     */
    virtual bool should_generate_report(Time t) = 0;
};

/**
 * Klasa odpowiedzialna za zwracanie prawdy do to-ą turę
 */
class IntervalReportNotifier : public ReportNotifier{
public:
    IntervalReportNotifier(TimeOffset to): TimeDivider_(to){};
    bool should_generate_report(Time t) { return (t-1) % TimeDivider_ == 0; }

private:
    Time TimeDivider_;

};

/**
 * Klasa odpowiedzialna za zwracanie prawdy w turach ze zbioru turns
 */
class SpecificTurnsReportNotifier : public ReportNotifier{
public:
    SpecificTurnsReportNotifier(std::set<Time> turns): TimeStamps_(turns){};
    bool should_generate_report(Time t) {return (TimeStamps_.count(t)>0);}

private:
    std::set<Time> TimeStamps_;

};


#endif //NET_SIMULATION_SIMULATION_HPP

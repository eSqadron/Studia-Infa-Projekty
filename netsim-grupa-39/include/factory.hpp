//
// Created by OEM on 22.12.2020.
//

#ifndef NET_SIMULATION_FACTORY_HPP
#define NET_SIMULATION_FACTORY_HPP

#include "types.hpp"
#include "nodes.hpp"
#include <istream>


template <class Node>
class NodeCollection{
public:
    typedef typename std::list<Node>::iterator iterator;
    typedef typename std::list<Node>::const_iterator const_iterator;

    void add(Node&& node) { l_.push_back(std::move(node)); };

    //FIXME - naprawić powtarzanie się kodu (usuwanie workera i storehousa wygląda prawie tak samo)
    void remove_by_id(ElementID id) {l_.remove_if([&id](Node& elem) { return elem.get_id() == id; }); }; //SHEISE AHTUNG! może być przekombinowane

    typename NodeCollection<Node>::iterator find_by_id(ElementID id) {return std::find_if(l_.begin(), l_.end(),
                                                                                          [&id](const Node& elem){ return elem.get_id() == id; }); }; // Dlaczemu typ NodeCollection<Node>::iterator?
    typename NodeCollection<Node>::const_iterator find_by_id(ElementID id) const {return std::find_if(l_.begin(), l_.end(),
                                                                                                 [&id](const Node& elem){ return elem.get_id() == id; }); }; //querry
                                                                                                 // TODO - trzeba ogarnąć żeby delegowało tą funkcję do funkcji powyżej
    typename NodeCollection<Node>::const_iterator cbegin(void) const {return l_.cbegin(); };
    typename NodeCollection<Node>::const_iterator cend(void) const {return l_.cend(); };

    typename NodeCollection<Node>::iterator begin(void) {return l_.begin(); };
    typename NodeCollection<Node>::iterator end(void) {return l_.end(); };
    void sort_nodes(void) ;
    static bool comp (const Node& lhs, const Node& rhs ) {return lhs.get_id() < rhs.get_id();};
private:
    std::list<Node> l_;
};


class Factory{
public:
    void add_ramp(Ramp&& r){ramps_.add(std::move(r)); sort_ramps();};
    void remove_ramp(ElementID id) {ramps_.remove_by_id(id); };
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) {return ramps_.find_by_id(id); };
    NodeCollection<Ramp>::const_iterator  find_ramp_by_id(ElementID id) const {return ramps_.find_by_id(id); }; //querry //chyba jedyna różnica jaka ma być między tą a poprzednią funkcją to ) const {

    NodeCollection<Ramp>::const_iterator ramp_cbegin(void) const {return ramps_.cbegin(); };//querry
    NodeCollection<Ramp>::const_iterator ramp_cend(void) const {return ramps_.cend(); };//querry

    NodeCollection<Ramp>::iterator ramp_begin(void) {return ramps_.begin(); };//querry
    NodeCollection<Ramp>::iterator ramp_end(void) {return ramps_.end(); };//querry
    void sort_ramps(void) {ramps_.sort_nodes();}

    void add_worker(Worker&& w) {workers_.add(std::move(w)); sort_workers();};
    void remove_worker(ElementID id);
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) {return workers_.find_by_id(id);};
    NodeCollection<Worker>::const_iterator  find_worker_by_id(ElementID id) const {return workers_.find_by_id(id);};//querry

    NodeCollection<Worker>::const_iterator worker_cbegin(void) const {return workers_.cbegin();};//querry
    NodeCollection<Worker>::const_iterator worker_cend(void) const {return workers_.cend();};//querry

    NodeCollection<Worker>::iterator worker_begin(void) {return workers_.begin(); };//querry
    NodeCollection<Worker>::iterator worker_end(void) {return workers_.end(); };//querry
    void sort_workers(void) {workers_.sort_nodes();}

    void add_storehouse(Storehouse&& s) {storehouses_.add(std::move(s)); sort_storehouses();};
    void remove_storehouse(ElementID id);
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) {return storehouses_.find_by_id(id);};
    NodeCollection<Storehouse>::const_iterator  find_storehouse_by_id(ElementID id) const {return storehouses_.find_by_id(id);};//querry

    NodeCollection<Storehouse>::const_iterator storehouse_cbegin(void) const { return storehouses_.cbegin();};//querry
    NodeCollection<Storehouse>::const_iterator storehouse_cend(void) const { return  storehouses_.cend();};//querry

    NodeCollection<Storehouse>::iterator storehouse_begin(void) {return storehouses_.begin(); };//querry
    NodeCollection<Storehouse>::iterator storehouse_end(void) {return storehouses_.end(); };//querry
    void sort_storehouses(void) {storehouses_.sort_nodes();}

    bool is_consistent(void);//querry
    void do_deliveries(Time t);
    void do_package_passing(void);
    void do_work(Time t);
private:
    template <typename Node>
    void remove_receiver(NodeCollection<Node>& collection, ElementID id);

    NodeCollection<Ramp> ramps_{}; //Chyba tak to trzeba przechowywać, nie jestem pewny
    NodeCollection<Worker> workers_{};
    NodeCollection<Storehouse> storehouses_{};


};



/**
 * Wczytuje fabrykę z pliku
 * @param is strumień z plikiem opisującym fabrykę
 * @return wczytana fabryka
 */
Factory load_factory_structure(std::istream& is);


/**
 * Zapisywanie fabryki do pliku, odwrotność load_factory_structure
 * @param factory
 * @param os
 */
void save_factory_structure(Factory& factory, std::ostream& os );



#endif //NET_SIMULATION_FACTORY_HPP

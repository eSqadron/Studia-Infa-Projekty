//
// Created by Goon on 06.01.2021.
//
#include "reports.hpp"


using namespace std;

//funkcja wypisująca odbiorców
void sort(NodeCollection<Worker>::const_iterator i, ostream& os) {
    os << "  Receivers:" << endl;
    list<int> w_id_list;
    list<int> s_id_list;
    for (auto j = i->receiver_preferences_.cbegin(); j != i->receiver_preferences_.cend(); j++) {
        if (j->first->get_receiver_type() == ReceiverType::WORKER) {
            w_id_list.push_back(j->first->get_id());
        }
        if (j->first->get_receiver_type() == ReceiverType::STOREHOUSE) {
            s_id_list.push_back(j->first->get_id());
        }
    }
    w_id_list.sort();
    s_id_list.sort();
    for(auto j = w_id_list.cbegin(); j != w_id_list.cend(); j++){
        string w_id = to_string(*j);
        os << "    worker #" << w_id << endl;
    }
    for(auto j = s_id_list.cbegin(); j != s_id_list.cend(); j++){
        string s_id = to_string(*j);
        os << "    storehouse #" << s_id << endl;
    }
}

void generate_structure_report(const Factory& f, ostream& os){
    if(f.ramp_cbegin() != f.ramp_cend()) { // Przed tym sortować jak nie sortowane
        os << endl <<"== LOADING RAMPS ==" << endl << endl;

        for (auto i = f.ramp_cbegin(); i != f.ramp_cend(); i++) {
            string r_id = to_string(i->get_id());
            string di = to_string(i->get_delivery_interval());
            os << "LOADING RAMP #" << r_id << endl << "  Delivery interval: " << di << endl;

            if (i->receiver_preferences_.cbegin() != i->receiver_preferences_.cend()) {
                list<int> w_list;
                for(auto j = i->receiver_preferences_.cbegin(); j != i->receiver_preferences_.cend(); j++){
                    w_list.push_back(j->first->get_id());
                }
                w_list.sort();
                os << "  Receivers:" << endl;
                for(auto j = w_list.begin(); j != w_list.end(); j++){
                    string w_id = to_string(*j);
                    os << "    worker #" << w_id << endl;
                    }
                }
            else{
                os << "No receivers" << endl;
            }
            os << endl;
        }
    }
    else{
        os << "There is no ramps!" << endl;
    }
    os << endl;

    if(f.worker_cbegin() != f.worker_cend()){ //tu tez przed tym segregowanie
        os << "== WORKERS ==" << endl << endl;

        for(auto i = f.worker_cbegin(); i != f.worker_cend(); i++){
            string w_id = to_string(i->get_id());
            string w_pd = to_string(i->get_processing_duration());
            PackageQueueType queue = i->get_queue()->get_queue_type();
            string w_qt;
            if (queue == PackageQueueType::FIFO){
                w_qt = "FIFO";
            }
            else if (queue == PackageQueueType::LIFO){
                w_qt = "LIFO";
            }
            else{
                w_qt = "Wrong queue type!";
            }
            os << "WORKER #" << w_id << endl << "  Processing time: " << w_pd << endl << "  Queue type: " << w_qt << endl;
            if(i->receiver_preferences_.cbegin() != i->receiver_preferences_.cend()){
                sort(i, os);
            }
            else{
                os << "  No receivers" << endl;
            }
            os << endl;
        }
    }
    else{
        os << "There is no workers!" << endl;
    }
    os << endl;

    if(f.storehouse_cbegin() != f.storehouse_cend()){
        os << "== STOREHOUSES ==" << endl;

        for (auto i = f.storehouse_cbegin(); i != f.storehouse_cend(); i++){ //przed tym też segregować
            string s_id = to_string(i->get_id());
            os << endl << "STOREHOUSE #" << s_id << endl;
        }
    }
    else{
        os << endl << "No storehouses" << endl;
    }
    os << endl;
}

void generate_simulation_turn_report(const Factory& f, ostream& os, Time t) {
    string time = to_string(t);
    os << "=== [ Turn: " << time << " ] ===" << endl << endl;
    os << "== WORKERS ==" << endl << endl;

    for(auto i = f.worker_cbegin(); i != f.worker_cend(); i++){
        string w_id = to_string(i->get_id());
        os << "WORKER #" << w_id << endl;

        if(i->get_processing_buffer().has_value()){
            string b_id = to_string(i->get_processing_buffer()->get_id());
            string p_t = to_string(t - i->get_package_processing_start_time() + 1);
            os << "  PBuffer: #" << b_id << " (pt = " << p_t << ")" << endl;
        }
        else{
            os << "  PBuffer: (empty)" << endl;
        }

        os << "  Queue: ";
        if(i->get_queue()->cbegin() != i->get_queue()->cend()){
            int a = i->get_queue()->size();
            for(auto j = i->get_queue()->cbegin();j != i->get_queue()->cend();j++){
                string q = to_string(j->get_id());
                if (a > 1){
                    os << "#" << q << ", ";
                }
                else{
                    os << "#" << q;
                }
                a--;
            }
        }
        else{
            os << "(empty)";
        }
        os << endl;

        if(i->get_sending_buffer().has_value()){
            string s_b = to_string(i->get_sending_buffer()->get_id());
            os << "  SBuffer: #" << s_b;
        }
        else{
            os << "  SBuffer: (empty)";
        }
        os << endl << endl;
    }
    os << endl;

    os << "== STOREHOUSES ==" << endl << endl;
    for(auto i = f.storehouse_cbegin(); i != f.storehouse_cend(); i++){
        string s_id = to_string(i->get_id());
        os << "STOREHOUSE #" << s_id << endl;

        os << "  Stock: ";
        if(i->get_stock()->cbegin() != i->get_stock()->cend()){
            int a = i->get_stock()->size();
            for(auto j = i->get_stock()->cbegin(); j != i->get_stock()->cend(); j++){
                string p_id = to_string(j->get_id());
                if (a > 1){
                    os << "#" << p_id << ", ";
                }
                else {
                    os << "#" << p_id;
                }
                a--;
            }
        }
        else{
            os << "(empty)";
        }
    }
    os << endl << endl;
}

/*
Student Name: Ezgi Tekdemir
Student Number: 2014400090
Project Number: 2
Operating System: Virtua1 Machine
Compile Status: {Compiling}
Program Status: {Working}
Notes: 
*/

#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>
#include <queue>
#include <iomanip>

using namespace std;

class Packet{
public:
		int packet_number = 0; 
		int unit_number = 0; //the number of unit in either first level or second level
		double arrival_time = 0.0;
		double time = 0.0; //total process time & waiting time & arrival time
		double waiting_time = 0.0; //the total time a packet waits in a queue
		bool is_waiting = true; 
		int place = 0; //the packet is initially in the heap. 1 for flu, 2 for slu
};

class ProcessUnit{
public:
	double process_time = 0.0; //time it takes to process a packet
	bool isEmpty = true; 
};


class DES{

public:
    ofstream out;
    bool fileAlreadyOpen = false;

	double GLOBAL_TIME = 0.0; //the time that the simulation is in
	int NUMBER_OF_FLU = 0;
	int NUMBER_OF_SLU = 0;
	int NUMBER_OF_CARGO = 0;
	ProcessUnit* FLU = NULL; 
	ProcessUnit* SLU = NULL;
    double first_arrival = 0.0;

    double line1, line4, line5, line6 = 0.0;
    double line1_2, line4_2, line5_2, line6_2 = 0.0;

    int max_queue1 = 0; int line2 = 0; int max_queue2 = 0; int line3 = 0;

	queue<Packet> queue1; 
	queue<Packet> queue2;
    vector<queue<Packet>> queue_layout2; //vector of second level queues

	vector<Packet> pq; //min heap used for getting the event with min time
    vector<Packet> pq2; //holds just the arrival times in order to reset the heap later
	vector<Packet> result; //

public:
	void importData(char*); //function to get data from input file
	void runSimulation(int); //function to run simulation and create input file
    void print(char*);
    void resetData();
};


struct comp {
	bool operator()(const Packet& p1, const Packet& p2) const{
		return p1.time>p2.time;
	}
};

void DES::importData (char* inputFile){
	ifstream in (inputFile);
	
	in >> NUMBER_OF_FLU;
	FLU = new ProcessUnit [NUMBER_OF_FLU];

	for(int i=0; i<NUMBER_OF_FLU; i++){
		double timef = 0.0;
		in >> timef;
		ProcessUnit tmp;
		tmp.process_time = timef;
		tmp.isEmpty = true;
		FLU[i] = tmp;
	}

	in >> NUMBER_OF_SLU;
	SLU = new ProcessUnit [NUMBER_OF_SLU];

	for(int i=0; i<NUMBER_OF_SLU; i++){
		double times = 0.0;
		in >> times;
		ProcessUnit tmp;
		tmp.process_time = times;
		tmp.isEmpty = true;
		SLU[i] = tmp;
        queue_layout2.push_back(queue<Packet>());

	}

	in >> NUMBER_OF_CARGO;

	for(int i=0; i<NUMBER_OF_CARGO; i++){
		Packet p;
		in >> p.time;
		p.arrival_time = p.time;
        if(i==0) first_arrival = p.arrival_time;
		p.place = 0;
		p.packet_number = i;
		pq.push_back(p);
	}

	make_heap(pq.begin(), pq.end(), comp());
    pq2 = pq;
}

void DES::runSimulation(int layout){

		int ptr_flu = 0; //ptr to empty flu
		int ptr_slu = 0; //ptr to empty slu

	while (!pq.empty()){

		GLOBAL_TIME = pq.front().time;

		if(pq.front().place == 0){

			cout << "Packet " << pq.front().packet_number << " arrived" <<endl;
			queue1.push(pq.front());
            max_queue1 ++;

            int i = 0;
                    while(!FLU[i].isEmpty && i < NUMBER_OF_FLU){
                        i++;
                    }
                    ptr_flu = i;

            if(max_queue1 > line2) line2 = max_queue1;
			queue1.back().is_waiting = true;
			pop_heap(pq.begin(), pq.end(), comp()); pq.pop_back();

                if (ptr_flu >= NUMBER_OF_FLU){

                    cout << "packet " << queue1.back().packet_number << " is waiting in queue1" <<endl;
                }
				else if(!queue1.empty() && ptr_flu < NUMBER_OF_FLU){
					queue1.front().unit_number = ptr_flu;
					cout << "Packet is delivered to FLU " << queue1.front().unit_number <<endl;

					queue1.front().time += FLU[ptr_flu].process_time;
					cout << "Processing of packet " << queue1.front().packet_number << " will end at "
					<< queue1.front().time << endl;

					queue1.front().is_waiting = false;
					FLU[ptr_flu].isEmpty = false;
					queue1.front().place = 1;

					pq.push_back(queue1.front());
					queue1.pop();
                    max_queue1--;
					make_heap(pq.begin(), pq.end(), comp());


				}


		} else if (pq.front().place == 1){

            int unum = pq.front().unit_number/2;
			
            if(layout==1){

                queue2.push(pq.front());
                queue2.back().is_waiting = true;
                max_queue2++;
            }else{
                queue_layout2.at(unum).push(pq.front());
                queue_layout2.at(unum).back().is_waiting = true;

            }

            ptr_flu = pq.front().unit_number;
             
            if(layout==1){
             int i = 0;
                    while(!SLU[i].isEmpty && i < NUMBER_OF_SLU){
                        i++;
                    }
                    ptr_slu = i;
            }

            FLU[ptr_flu].isEmpty = true;
			pop_heap(pq.begin(), pq.end(), comp()); pq.pop_back();

            if(!queue1.empty()){
                        queue1.front().waiting_time = GLOBAL_TIME - queue1.front().arrival_time;
                        queue1.front().unit_number = ptr_flu;
                        cout << "Packet " << queue1.front().packet_number 
                        << " is delivered to FLU " << queue1.front().unit_number <<endl;

                        queue1.front().time = GLOBAL_TIME + FLU[ptr_flu].process_time;
                    
                        cout << "Processing of packet " << queue1.front().packet_number << " will end at "
                        << queue1.front().time << endl;

                        queue1.front().is_waiting = false;
                        queue1.front().place = 1;
                        FLU[ptr_flu].isEmpty = false;

                        pq.push_back(queue1.front());
                        queue1.pop();
                        max_queue1 --;
                        make_heap(pq.begin(), pq.end(), comp());
                        ptr_flu = NUMBER_OF_FLU;
                    } 

            if(layout==1){
				if(!queue2.empty() && ptr_slu < NUMBER_OF_SLU){
		
    					queue2.front().unit_number = ptr_slu;
    					cout << "Packet " << queue2.front().packet_number 
    						<< " is delivered to SLU " << queue2.front().unit_number <<endl;

    					queue2.front().time += SLU[ptr_slu].process_time;
    					cout << "Processing of packet " << queue2.front().packet_number << " will end at "
    					<< queue2.front().time << endl;


    					queue2.front().is_waiting = false;
    					queue2.front().place = 2;
    					SLU[ptr_slu].isEmpty = false;

                        pq.push_back(queue2.front());
    					queue2.pop();
                        max_queue2 --;
    					make_heap(pq.begin(), pq.end(), comp());

    					ptr_slu++;

				} else if (ptr_slu >= NUMBER_OF_SLU){
					cout << "packet " << queue2.back().packet_number << " is waiting in queue2" <<endl;
				}

			     if(max_queue2 > line3) line3 = max_queue2;

             } else {
                if(!queue_layout2.at(unum).empty() && SLU[unum].isEmpty){
        
                        queue_layout2.at(unum).front().unit_number = unum;
                        queue_layout2.at(unum).front().time += SLU[unum].process_time;


                        queue_layout2.at(unum).front().is_waiting = false;
                        queue_layout2.at(unum).front().place = 2;
                        SLU[unum].isEmpty = false;

                        pq.push_back(queue_layout2.at(unum).front());
                        queue_layout2.at(unum).pop();
                        make_heap(pq.begin(), pq.end(), comp());


                }
                if(queue_layout2.at(unum).size() > (unsigned int) line3) line3 = queue_layout2.at(unum).size();

             }

		}  else if (pq.front().place == 2){

			ptr_slu = pq.front().unit_number;
			SLU[ptr_slu].isEmpty = true;

        if(layout==1){

			if(!queue2.empty()){
						queue2.front().waiting_time += (GLOBAL_TIME - queue2.front().time);
						queue2.front().unit_number = ptr_slu;
                        cout << "Packet " << queue2.front().packet_number 
                        << " is delivered to SLU " << queue2.front().unit_number <<endl;

					    queue2.front().time = GLOBAL_TIME + SLU[ptr_slu].process_time;
                        cout << "Processing of packet " << queue2.front().packet_number << " will end at "
                        << queue2.front().time << endl;

						queue2.front().is_waiting = false;
						queue2.front().place = 2;
						SLU[ptr_slu].isEmpty = false;

                        pq.push_back(queue2.front());
						queue2.pop();
                        max_queue2--;
						make_heap(pq.begin(), pq.end(), comp());
                        ptr_slu = NUMBER_OF_SLU;

			} 
        }else{
            if(!queue_layout2.at(ptr_slu).empty()){
                        queue_layout2.at(ptr_slu).front().waiting_time += (GLOBAL_TIME - queue_layout2.at(ptr_slu).front().time);
                        queue_layout2.at(ptr_slu).front().unit_number = ptr_slu;
                        
                        queue_layout2.at(ptr_slu).front().time = GLOBAL_TIME + SLU[ptr_slu].process_time;


                        queue_layout2.at(ptr_slu).front().is_waiting = false;
                        queue_layout2.at(ptr_slu).front().place = 2;
                        SLU[ptr_slu].isEmpty = false;

                        pq.push_back(queue_layout2.at(ptr_slu).front());
                        queue_layout2.at(ptr_slu).pop();
                        make_heap(pq.begin(), pq.end(), comp());
            }
        }


			result.push_back(pq.front());
            cout << "processing of packet " << pq.front().packet_number << " at SLU " << pq.front().unit_number << " is completed" << endl;
			pop_heap(pq.begin(), pq.end(), comp()); pq.pop_back();
		}
	}
}


void DES::print (char* outputFile){

    if(!fileAlreadyOpen){
        out.open (outputFile);
        fileAlreadyOpen = true;
    }

    out.precision(3);
    out.setf(ios::fixed);
    out.setf(ios::showpoint);

    for(unsigned int j=0; j<result.size(); j++){
    	line1 += result.at(j).time - result.at(j).arrival_time;
    	line4 += result.at(j).waiting_time;
    	if(result.at(j).waiting_time > line5) 
            line5 = result.at(j).waiting_time;
    }
    line6 = result.at(result.size()-1).time - first_arrival;

    out << line1/result.size() << "\n" << line2 << "\n" << line3 
    << "\n" << line4/result.size() << "\n" << line5 << "\n" << line6 << "\n" << endl;
}


void DES::resetData(){
    GLOBAL_TIME = 0.0;

    for(int i=0; i<NUMBER_OF_FLU; i++){
        FLU[i].isEmpty = true;
    }

    for(int i=0; i<NUMBER_OF_SLU; i++){
        SLU[i].isEmpty = true;
    }

    line1 = 0.0, line2 = 0.0; line4 = 0.0, line5 = 0.0; line6 = 0.0;
    line2 = 0; line3 = 0;

    while(!queue1.empty()) queue1.pop();
    while(!queue2.empty()) queue2.pop();

    result.clear();
    pq = pq2;
}

int main(int argc, char *argv[]) {
	DES* des = new DES();
	des->importData(argv[1]);
	
    des->runSimulation(1);
    des->print(argv[2]);

    des->resetData();

    des->runSimulation(2);
    des->print(argv[2]);


	return 0;
}
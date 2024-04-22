#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include <fstream>
#include <cmath>

using namespace std;

class Disk
{
public:
    const int AMOUNT_OF_SECTORS = 10;
    const int AMOUNT_OF_TRACKS = 257;
    const int FULL_ROTATION = 5;
    const double TRANSFER_TIME = 1.2;
    int arrival_time;
    int request_sector;
    int requested_arrival_track;
    int ids;
    double total_time;
    Disk(int arrival, int req_sec, int req_arr_track, int id, int T_T)
        : arrival_time(arrival), request_sector(req_sec), requested_arrival_track(req_arr_track), ids(id), total_time(T_T)  {}

    Disk() : arrival_time(0), request_sector(0), requested_arrival_track(0), ids(0), total_time(0) {}
};

std::ostream& operator<<(std::ostream& cout, const Disk* d)
{
    return cout << "Arrival Time: " << d->arrival_time << "\tRequested Track: " << d->requested_arrival_track
        << "\tRequested Sector: " << d->request_sector;
}



double calculate_total_time(int T, int S) 
{
    return ((12 + ( 0.1 * T))) + (S*5) + 1.2;
}

double calculate_track_or_sector(int T_1, int T_2) 
{
    return abs(T_1 - T_2);
}

Disk* closest_seek_time(double time,  std::vector<Disk*>& disks)
{
    auto sba = [](const Disk* d1, const Disk* d2)
        {
            return (d2->total_time < d1->total_time);
        };
    std::priority_queue<Disk*, std::vector<Disk*>, decltype(sba) > sorted_by_total_time(sba);

    double time_2 = 0;
    

    for (int i = 0; i < disks.size(); i++) 
    {
        time_2 = calculate_total_time(disks[i]->requested_arrival_track, disks[i]->request_sector);
        double diff = abs(time - time_2);
        disks[i]->total_time = diff;
        sorted_by_total_time.push(disks[i]);
        time = time_2;
    }
    return sorted_by_total_time.top();
    
}
void FCFS(const std::vector<Disk*>& disk_process)
{
    double total_time = 0;
    int track = 0;
    int sector = 0;
    auto sba = [](const Disk* d1, const Disk* d2)
        {
            return (d2->arrival_time < d1->arrival_time);
        };

    std::priority_queue<Disk*, std::vector<Disk*>, decltype(sba) > sorted_by_arrival_time(sba);
    
    for (auto disk : disk_process) 
    {
        sorted_by_arrival_time.push(disk);
    }
    
    while (!sorted_by_arrival_time.empty()) 
    {
        
        double total_time = 0;
        int track = 0;
        int sector = 0;

        track = calculate_track_or_sector(track, sorted_by_arrival_time.top()->requested_arrival_track);
        sector = calculate_track_or_sector(sector, sorted_by_arrival_time.top()->request_sector);

        total_time += calculate_total_time(track, sector);
        sorted_by_arrival_time.pop();

        
        std::cout << "Total Time is: " << total_time << std::endl;
    }
}

void SSTF(const std::vector<Disk*>& disk_process) 
{
    
    Disk* temp = new Disk;
    
    int track = 0;
    int sector = 0;
    double total_time = 0;

    auto sba = [](const Disk* d1, const Disk* d2)
        {
            return (d2->arrival_time < d1->arrival_time);
        };
    std::priority_queue<Disk*, std::vector<Disk*>, decltype(sba) > sorted_by_arrival_time(sba);
    std::vector<Disk*> disks;
    std::vector<Disk*> temps;

    for (auto disk : disk_process)
    {
        sorted_by_arrival_time.push(disk);
        disks.push_back(disk);
    }

    track = calculate_track_or_sector(track, sorted_by_arrival_time.top()->requested_arrival_track); //Sets track to first arrival time
    sector = calculate_track_or_sector(sector, sorted_by_arrival_time.top()->request_sector); //Sets sector to first arrival time
    total_time += calculate_total_time(track, sector); //calculates the total time of the first arrived 
    sorted_by_arrival_time.pop(); //pops the first arrival time

    while (!disks.empty()) 
    {
        temp = closest_seek_time(total_time, disks);
        
        if (total_time >= temp->arrival_time) 
        {
            total_time += calculate_total_time(temp->requested_arrival_track, temp->request_sector);
            for (int i = 0; i < disks.size(); i++) 
            {
                if (temp->total_time == disks[i]->total_time) 
                {
                    disks.erase(disks.begin() + i);
                    i = disks.size();
                }
            }
        }
        else 
        {
            for (int i = 0; i < disks.size(); i++)
            {
                if (temp->total_time == disks[i]->total_time)
                {
                    temps.push_back(disks[i]);
                    disks.erase(disks.begin() + i);
                    i = disks.size();
                }
            }
            
            for (int i = 0; i < temps.size(); i++) 
            {
                if (total_time >= temps[i]->arrival_time) 
                {

                }
            }
        }
    }

    
}
std::vector<int> createProcess(const std::string& fileName)//takes file name as string and creates a vector of numbers
{
    std::vector<int> totalNumbers;
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << fileName << std::endl;
        return totalNumbers;
    }
    int number;
    while (file >> number) {
        totalNumbers.push_back(number);
    }
    file.close();
    return totalNumbers;
}
int main() 
{
    std::string fileName;
    fileName = "Tracks.txt";
    std::vector<int> tracks = createProcess(fileName);
    std::vector<int> arrival_times;
    std::vector<int> requested_sectors;
    std::vector<int> requested_tracks;
    std::vector<Disk*> all_disk_request;

    
    for (int i = 0; i < tracks.size(); i += 3) 
    {
        arrival_times.push_back(tracks[i]);
        requested_tracks.push_back(tracks[i + 1]);
        requested_sectors.push_back(tracks[i + 2]);
    }

    for (int i = 0; i < tracks.size() / 3; i++) 
    {
        Disk* newDisk = new Disk;
        all_disk_request.push_back(newDisk);
    }

    for (int i = 0; i < all_disk_request.size(); i++) 
    {
        all_disk_request[i]->arrival_time = arrival_times[i];
        all_disk_request[i]->request_sector = requested_sectors[i];
        all_disk_request[i]->requested_arrival_track = requested_tracks[i];
        all_disk_request[i]->ids = i;
    }
  
    FCFS(all_disk_request);
}


// Datastructures.hh
//
// Student name:
// Student email:
// Student number:

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <memory>

// Types for IDs
using StationID = std::string;
using TrainID = std::string;
using RegionID = unsigned long long int;
using Name = std::string;
using Time = unsigned short int;

// Return values for cases where required thing was not found
StationID const NO_STATION = "---";
TrainID const NO_TRAIN = "---";
RegionID const NO_REGION = -1;
Name const NO_NAME = "!NO_NAME!";
Time const NO_TIME = 9999;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    int d1 = std::hypot(c1.x, c1.y);
    int d2 = std::hypot(c2.x, c2.y);
    if (d1 < d2) { return true; }
    else if (d2 < d1) { return false; }
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: getting container size is constant timed
    unsigned int station_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: linear clear() operations in series
    void clear_all();

    // Estimate of performance: O(1)
    // Short rationale for estimate: just returning an existing vector
    std::vector<StationID> all_stations();

    // Estimate of performance: O(n)
    // Short rationale for estimate: inserting to map is the most expensive operation
    bool add_station(StationID id, Name const& name, Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: searching from unordered map by key
    Name get_station_name(StationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: searching from unordered map by key
    Coord get_station_coordinates(StationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: looping through a map n times
    std::vector<StationID> stations_alphabetically();

    // Estimate of performance: O(n)
    // Short rationale for estimate: looping through a map n times
    std::vector<StationID> stations_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate: looping through unordered map to find a value
    StationID find_station_with_coord(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: searching from unordered map by key
    bool change_station_coord(StationID id, Coord newcoord);

    // Estimate of performance: O(n)
    // Short rationale for estimate: searching from unordered map and inserting one item to set
    bool add_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(n)
    // Short rationale for estimate: searching from unordered map and erasing from set
    bool remove_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(n)
    // Short rationale for estimate: constant time operations for each of station's departures
    std::vector<std::pair<Time, TrainID>> station_departures_after(StationID stationid, Time time);

    // Estimate of performance: O(n)
    // Short rationale for estimate: inserting one item to unordered map
    bool add_region(RegionID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: O(n)
    // Short rationale for estimate: constant time operation for n items
    std::vector<RegionID> all_regions();

    // Estimate of performance: O(n)
    // Short rationale for estimate: searching from unordered map by key
    Name get_region_name(RegionID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: searching from unordered map by key
    std::vector<Coord> get_region_coords(RegionID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: searching from unordered map by key, pushing back to vector
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: searching from unordered map by key
    bool add_station_to_region(StationID id, RegionID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: linear in the height of the region tree
    std::vector<RegionID> station_in_regions(StationID id);

    // Non-compulsory operations

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: preorder traversal of tree nodes and O(n) operations for each
    std::vector<RegionID> all_subregions_of_region(RegionID id);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: inserting to map n times
    std::vector<StationID> stations_closest_to(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: searcing from vector by value
    bool remove_station(StationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: linear operations for two regions
    RegionID common_parent_of_regions(RegionID id1, RegionID id2);

private:
    // Calculates the distance between two coords c1 and c2
    Distance distance_between(Coord c1, Coord c2);

    // Returns all direct and indirect parent regions of a region with id
    std::vector<RegionID> all_parents_of_region(RegionID id);

    // Sturct for storing station data
    struct Station {
        StationID id = NO_STATION;
        Name name = NO_NAME;
        Coord coord = NO_COORD;
        RegionID location = NO_REGION;
        std::set<std::pair<Time, TrainID>> departures = {};
    };
    // Node for a tree structure storing region data and relationships
    struct Region {
        RegionID id = NO_REGION;
        Name name = NO_NAME;
        std::vector<Coord> limits = {};
        Region* parent = nullptr;
        std::vector<Region*> subregions = {};
    };
    // Stations mapped to their IDs
    std::unordered_map<StationID, std::shared_ptr<Station>> stations_to_ids;

    // Station ids mapped to coords
    std::map<Coord, StationID> station_ids_to_coords;

    // Station ids mapped to names
    std::set<std::pair<Name, StationID>> station_ids_to_names;

    // Regions mapped to their IDs
    std::unordered_map<RegionID, std::shared_ptr<Region>> regions_to_ids;
};

#endif // DATASTRUCTURES_HH

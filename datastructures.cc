// Datastructures.cc
//
// Student name:
// Student email:
// Student number:

#include "datastructures.hh"
#include <random>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

/**
 * @brief Datastructures::Datastructures constructor of the class
 */
Datastructures::Datastructures() {}

/**
 * @brief Datastructures::~Datastructures destructor of the class
 */
Datastructures::~Datastructures() {}

/**
 * @brief Datastructures::station_count counts all stations
 * @return the number of stations saved to the datastructure
 */
unsigned int Datastructures::station_count()
{
    unsigned int station_count = stations_to_ids.size(); // O(1)
    return station_count;
}

/**
 * @brief Datastructures::clear_all clears all containers in the datastructure
 */
void Datastructures::clear_all()
{
    stations_to_ids.clear(); // O(n)
    regions_to_ids.clear(); // O(n)
    station_ids_to_coords.clear(); // O(n)
    station_ids_to_names.clear(); // O(n)
    return;
}

/**
 * @brief Datastructures::all_stations lists all stations by their id
 * @return vector containing ids for all stations saved to the datastructure
 */
std::vector<StationID> Datastructures::all_stations()
{
    std::vector<StationID> all_ids;
    for (const auto& station_to_id : stations_to_ids)
    {
        auto& id = station_to_id.first;
        all_ids.push_back(id);
    }
    return all_ids;
}

/**
 * @brief Datastructures::add_station saves a new station to the datastructure
 * @param id the unique indentifier of the new station
 * @param name the name of the new station
 * @param xy the coordinates of the new station
 * @return bool value indicating if saving the station was succesfull
 */
bool Datastructures::add_station(StationID id, const Name& name, Coord xy)
{
    if (stations_to_ids.find(id) != stations_to_ids.end())
    {
        return false;
    }
    std::shared_ptr<Station> new_station(new Station({id, name, xy}));
    std::pair<StationID, std::shared_ptr<Station>> id_station_pair = {id, new_station};
    bool add_success = stations_to_ids.insert(id_station_pair).second; // O(n), 0(1)
    if (add_success)
    {
        station_ids_to_coords.insert({xy, id}); // O(logn)
        station_ids_to_names.insert({name, id}); // O(logn)
    }
    return add_success;
}

/**
 * @brief Datastructures::get_station_name finds the name of the station with given id
 * @param id the id of the station
 * @return name of the station
 */
Name Datastructures::get_station_name(StationID id)
{
    auto station = stations_to_ids.find(id); // O(n), 0(1)
    if (station == stations_to_ids.end())
    {
        return NO_NAME;
    }
    return station->second->name;
}

/**
 * @brief Datastructures::get_station_coordinates finds the coordinates of the station with given id
 * @param id the id of the station
 * @return the coordinates of the station
 */
Coord Datastructures::get_station_coordinates(StationID id)
{
    auto station = stations_to_ids.find(id); // O(n), 0(1)
    if (station == stations_to_ids.end())
    {
        return NO_COORD;
    }
    return station->second->coord;
}

/**
 * @brief Datastructures::stations_alphabetically lists the ids of all stations sorted alphabetically by their names
 * @return vector containing the sorted station ids
 */
std::vector<StationID> Datastructures::stations_alphabetically()
{
    std::vector<StationID> sorted_stations;
    sorted_stations.reserve(stations_to_ids.size());
    for (const auto& id_to_name : station_ids_to_names) // O(n)
    {
        auto& id = id_to_name.second;
        sorted_stations.push_back(id); // O(1)
    }
    return sorted_stations;
}

/**
 * @brief Datastructures::stations_distance_increasing lists the ids of all stations sorted ascendingly by their coordinates
 * @return vector containing the sorted station ids
 */
std::vector<StationID> Datastructures::stations_distance_increasing()
{
    std::vector<StationID> sorted_stations;
    sorted_stations.reserve(stations_to_ids.size());
    for (const auto& id_to_coord : station_ids_to_coords) // O(n)
    {
        auto& id = id_to_coord.second;
        sorted_stations.push_back(id); // O(1)
    }
    return sorted_stations;
}

/**
 * @brief Datastructures::find_station_with_coord finds the id of a station located in given coordinates
 * @param xy the coordinates where a station is searched from
 * @return id of the found station
 */
StationID Datastructures::find_station_with_coord(Coord xy)
{
    auto found_station = station_ids_to_coords.find(xy); // O(n), 0(1)
    if (found_station == station_ids_to_coords.end())
    {
        return NO_STATION;
    }
    return found_station->second;
}

/**
 * @brief Datastructures::change_station_coord changes the coordinates of a station with given id
 * @param id the id of the station whose coordinates are to be changed
 * @param newcoord the new coordinates of the station
 * @return bool value indicating if changing coordinates was successful
 */
bool Datastructures::change_station_coord(StationID id, Coord newcoord)
{   
    auto station = stations_to_ids.find(id);
    if (station == stations_to_ids.end()) // O(n), 0(1)
    {
        return false;
    }
    Coord& oldcoord = station->second->coord; // O(n), 0(1)
    auto id_to_coord = station_ids_to_coords.extract(oldcoord); // O(n), 0(1)
    id_to_coord.key() = newcoord;
    station_ids_to_coords.insert(std::move(id_to_coord)); // O(logn)
    oldcoord = newcoord;

    return true;
}

/**
 * @brief Datastructures::add_departure saves a train departure for given station
 * @param stationid the id of the station that the train departs from
 * @param trainid the id of the train departing
 * @param time the time of the departure
 * @return bool value indicating if saving the departure was successful
 */
bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time)
{   
    auto station = stations_to_ids.find(stationid); // O(n), 0(1)
    if (station == stations_to_ids.end())
    {
        return false;
    }
    auto& departures = station->second->departures;

    // O(logn)
    std::pair<Time, TrainID> new_departure = {time, trainid};
    auto add_success = departures.insert(new_departure).second;

    return add_success;
}

/**
 * @brief Datastructures::remove_departure removes a train departure from a given station
 * @param stationid the id of the station that the departure is removed from
 * @param trainid the id of the train departing
 * @param time the time of the departure
 * @return bool value indicating if removing the departure was successful
 */
bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time)
{    
    auto station = stations_to_ids.find(stationid); // O(n), 0(1)
    if (station == stations_to_ids.end())
    {
        return false;
    }
    auto& departures = station->second->departures;
    auto departure_to_remove = departures.find({time, trainid}); // O(logn)
    if (departure_to_remove == departures.end())
    {
        return false;
    }
    departures.erase(departure_to_remove); // O(n)

    return true;
}

/**
 * @brief Datastructures::station_departures_after lists the departures from given station after given time
 * @param stationid the id of the station whose departures are listed
 * @param time the earliest time of the day for which departures are listed
 * @return vector containing the departures as time, train pairs
 */
std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID stationid, Time time)
{
    auto station = stations_to_ids.find(stationid); // O(n), 0(1)
    if (station == stations_to_ids.end())
    {
        return {{NO_TIME, NO_TRAIN}};
    }
    auto& all_departures = station->second->departures; // d = number of departures

    auto first_dep = std::find_if(all_departures.begin(), all_departures.end(),
                     [&time](auto departure){ return departure.first >= time; }); // m = 1 ... d operations

    int num_of_departures = std::distance(first_dep, all_departures.end()); // d - m operations
    std::vector<std::pair<Time, TrainID>> timetable;
    timetable.reserve(num_of_departures);

    std::for_each(first_dep, all_departures.end(),
                  [&timetable](auto departure){timetable.push_back(departure);}); // d - m operations

    return timetable;
}

/**
 * @brief Datastructures::add_region saves a new region to the datastructure
 * @param id unique identifier of the new region
 * @param name the name of the new region
 * @param coords vector containing the geographical limits of the new region as coordinates
 * @return bool value indicating if saving the new region was successful
 */
bool Datastructures::add_region(RegionID id, const Name &name, std::vector<Coord> coords)
{
    std::shared_ptr<Region> new_region(new Region({id, name, coords}));
    std::pair<RegionID, std::shared_ptr<Region>> id_region_pair = {id, new_region};
    bool add_success = regions_to_ids.insert(id_region_pair).second; // O(n), 0(1)
    return add_success;
}

/**
 * @brief Datastructures::all_regions lists all regions by their id
 * @return vector containing ids for all regions saved to the datastructure
 */
std::vector<RegionID> Datastructures::all_regions()
{
    std::vector<RegionID> all_regions;
    all_regions.reserve(regions_to_ids.size());
    for (const auto& region : regions_to_ids) // O(n)
    {
        RegionID id = region.first;
        all_regions.push_back(id); // O(1)
    }
    return all_regions;
}

/**
 * @brief Datastructures::get_region_name finds the name of the region with given id
 * @param id the id of the region
 * @return name of the station
 */
Name Datastructures::get_region_name(RegionID id)
{
    auto region = regions_to_ids.find(id); // O(n), 0(1)
    if (region == regions_to_ids.end())
    {
        return NO_NAME;
    }
    return region->second->name;
}

/**
 * @brief Datastructures::get_region_coords finds the coordinates bordering a given region
 * @param id the id of the region
 * @return vector containing the geographical limits of the region as coordinates
 */
std::vector<Coord> Datastructures::get_region_coords(RegionID id)
{
    auto region = regions_to_ids.find(id); // O(n), 0(1)
    if (region == regions_to_ids.end())
    {
        return {NO_COORD};
    }
    return region->second->limits;
}

/**
 * @brief Datastructures::add_subregion_to_region saves a parent-child relationship between two regions
 * @param id the id of the region acting as subregion
 * @param parentid the if of the region acting as parent region
 * @return bool value indicating if saving the relationship was successful
 */
bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    auto region_ptr = regions_to_ids.find(id); // O(n), 0(1)
    if (region_ptr == regions_to_ids.end())
    {
        return false;
    }
    auto parent_ptr = regions_to_ids.find(parentid); // O(n), 0(1)
    if (parent_ptr == regions_to_ids.end())
    {
        return false;
    }
    auto& subregion = region_ptr->second;
    auto& new_parent = parent_ptr->second;
    auto& old_parent = subregion->parent;

    if (old_parent != nullptr)
    {
        return false;
    }
    old_parent = new_parent.get();
    new_parent->subregions.push_back(subregion.get()); // O(1)

    return true;
}

/**
 * @brief Datastructures::add_station_to_region saves a region the location for given station
 * @param id the id of the station
 * @param parentid the id of the region
 * @return bool value indicating if saving the station-region relationship was successful
 */
bool Datastructures::add_station_to_region(StationID id, RegionID parentid)
{
    auto station = stations_to_ids.find(id); // O(n), 0(1)
    if (station == stations_to_ids.end())
    {
        return false;
    }
    auto region = regions_to_ids.find(parentid); // O(n), 0(1)
    if (region == regions_to_ids.end())
    {
        return false;
    }
    auto& location = station->second->location;
    if (location != NO_REGION)
    {
        return false;
    }
    location = parentid;
    return true;
}

/**
 * @brief Datastructures::station_in_regions lists the regions that given station belogns to
 * @param id the id of the station
 * @return vector containing ids of all regions that the station belogns to
 */
std::vector<RegionID> Datastructures::station_in_regions(StationID id)
{
    auto station = stations_to_ids.find(id); // O(n), 0(1)
    if (station == stations_to_ids.end())
    {
        return {NO_REGION};
    }
    auto& location = station->second->location;
    if (location == NO_REGION)
    {
        return {};
    }
    return all_parents_of_region(location);
}

/**
 * @brief Datastructures::all_subregions_of_region lists all direct and indirect subregions for given region
 * @param id the id of the region
 * @return vector containing ids of all direct and indirect subregions
 */
std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID id)
{
    std::vector<RegionID> ids = {};
    auto region = regions_to_ids.find(id); // O(n), 0(1)
    if (region == regions_to_ids.end())
    {
        return {NO_REGION};
    }
    auto& subregions = region->second->subregions;
    for (const auto& subregion : subregions) // O(n)
    {
        auto& sub_id = subregion->id;
        ids.push_back(sub_id);
        std::vector<RegionID> subsubs = all_subregions_of_region(sub_id);
        ids.insert(ids.end(), subsubs.begin(), subsubs.end());
    }
    return ids;
}

/**
 * @brief Datastructures::stations_closest_to finds 3 stations located closest to the given coordinate
 * @param xy the coordinate for which the closest stations are searched
 * @return vector containing ids for the 3 closest stations
 */
std::vector<StationID> Datastructures::stations_closest_to(Coord xy)
{
    std::vector<StationID> closest_stations;
    closest_stations.reserve(3);
    std::map<Distance, std::set<std::pair<int, StationID>>> sorted_ids;

    for (const auto& id_to_coord : station_ids_to_coords) // O(n)
    {
        auto& coord = id_to_coord.first;
        int y_coord = coord.y;
        auto& id = id_to_coord.second;
        Distance distance = distance_between(coord, xy);
        sorted_ids[distance].insert({y_coord, id}); // O(logn) + O(logn)
    }
    auto iter = sorted_ids.begin();
    while (closest_stations.size() < 3 && iter != sorted_ids.end()) // O(3)
    {
        auto& ids_to_y_coords = iter->second;
        for (auto& id_to_y_coord : ids_to_y_coords)
        {
            if (closest_stations.size() < 3)
            {
                auto& id = id_to_y_coord.second;
                closest_stations.push_back(id);
            }
        }
        ++iter;
    }
    return closest_stations;
}

/**
 * @brief Datastructures::remove_station removes a station from the datastructure
 * @param id the id of the station that is to be removed
 * @return bool value indicating if the removal was successful
 */
bool Datastructures::remove_station(StationID id)
{
    auto station = stations_to_ids.find(id); // O(n), 0(1)
    if (station == stations_to_ids.end())
     {
        return false;
    }
    auto coord_to_remove = station->second->coord;
    auto name_to_remove = station->second->name;

    station_ids_to_coords.erase(coord_to_remove); // O(logn)
    station_ids_to_names.erase({name_to_remove, id}); // O(logn)
    stations_to_ids.erase(id); // O(n), 0(1)

    return true;
}

/**
 * @brief Datastructures::common_parent_of_regions finds the common parent region nearest in tree hierarchy for two regions
 * @param id1 the id of the first region
 * @param id2 the id of the second region
 * @return id of the nearest common parents of the two region
 */
RegionID Datastructures::common_parent_of_regions(RegionID id1, RegionID id2)
{
    auto region1 = regions_to_ids.find(id1); // O(n), 0(1)
    auto region2 = regions_to_ids.find(id2); // O(n), 0(1)

    if (region1 == regions_to_ids.end() || region2 == regions_to_ids.end())
    {
        return NO_REGION;
    }
    auto parent1 = region1->second->parent;
    auto parent2 = region2->second->parent;

    if (parent1 == nullptr || parent2 == nullptr)
    {
        return NO_REGION;
    }
    std::vector<RegionID> parents1 = all_parents_of_region(parent1->id); // O(n)
    std::vector<RegionID> parents2 = all_parents_of_region(parent2->id); // O(n)

    // O(a*b), where a and b are distances from regions 1 and 2 to root node, respectively
    auto common_parent = std::find_first_of(parents1.begin(), parents1.end(),
                                            parents2.begin(), parents2.end());
    if (common_parent == parents1.end())
    {
        return NO_REGION;
    }
    return *common_parent;
}

/**
 * @brief Datastructures::distance_between calculates the euclidean distance between two coordinates
 * @param c1 first coordinate
 * @param c2 second coordinate
 * @return the distance between the two coordinates
 */
Distance Datastructures::distance_between(Coord c1, Coord c2)
{
    Distance distance = hypot(c1.x - c2.x, c1.y - c2.y);
    return abs(distance);
}

/**
 * @brief Datastructures::all_parents_of_region finds all regions that given region belogns to directly or indirectly
 * @param id the id of the region
 * @return vector containing ids for all regions that the subregion belogns to
 */
std::vector<RegionID> Datastructures::all_parents_of_region(RegionID id)
{
    std::vector<RegionID> all_parents;

    auto current_region = regions_to_ids.at(id).get(); // O(n), 0(1)

    // loop runs r times, where r is the distance from region node to root
    while (current_region != nullptr)
    {
        all_parents.push_back(current_region->id);
        current_region = current_region->parent;
    }
    return all_parents;
}

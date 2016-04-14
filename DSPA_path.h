#ifndef DSPA_PATH_H_INCLUDED
#define DSPA_PATH_H_INCLUDED

#include <vector>

struct DSPA_path
{
    std::vector<bool> path;  // TODO: change this to not have to save so much?
    unsigned long long int cost;
    size_t end_node;

    bool operator< (const DSPA_path& rhs) const
    {
        return (cost < rhs.cost) || ((cost == rhs.cost) && path_less(rhs));
    }

    bool path_less(const DSPA_path& rhs) const
    {
        size_t index = 0;
        size_t min_size = path.size() < rhs.path.size() ? path.size() : rhs.path.size();
        while (index < min_size)
        {
            if (path[index] < rhs.path[index])
                return true;
            if (rhs.path[index] < path[index])
                return false;
            // if ==, continue
            ++index;
        }
        return false;
    }
};

struct comparator
{
    /** for min heap */
    bool operator()(const DSPA_path& i, const DSPA_path& j)
    {
        return j < i;
    }
};

#endif // DSPA_PATH_H_INCLUDED

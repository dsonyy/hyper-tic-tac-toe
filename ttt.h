#ifndef _TTT_HEADER_
#define _TTT_HEADER_

#include <iostream>
#include <vector>
#include <functional>
#include <cstdlib>
using namespace std;

enum Field
{ 
	EMPTY,
	O, 
	X,
};

typedef vector<Field> Map;
typedef int MapPos;
typedef vector<int> VMapPos;

void write(const Map & map);

int get_offset_by_dim(size_t dim);

bool is_first_in_dim(MapPos pos, size_t dim);
bool is_last_in_dim(MapPos pos, size_t dim);

MapPos get_first_in_this_dim(MapPos pos, size_t dim);
MapPos get_last_in_this_dim(MapPos pos, size_t dim);

VMapPos pos_to_vector(MapPos pos);
MapPos  vector_to_pos(VMapPos vpos);

Field get_field(const Map & map, MapPos pos);

bool set_field(Map & map, Field field, MapPos pos, bool overwrite = false);


Field check_win(const Map & map, MapPos pos);
vector<int> get_neighbours_offsets(MapPos pos);
bool check_line(const Map & map, MapPos pos, int offset);
bool check(const Map & map, MapPos pos, size_t dim, int offset = 0);


#endif

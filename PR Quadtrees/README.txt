This project is for the construction of PR Quadtree over footpath data as well as performing searching over the Quadtree. Examples of footpath data are located in the examples folder. There are two modes for the search: point search and range search. 

Point Search(mode 3) takes an input longitude, and latitude coordinate and outputs the specified output file to all footpaths with points inside the region of the quadtree that the point being searched would be in. 

Region search(mode 4) takes starting longitude and latitude, as well as ending longitude and latitude. The program then outputs to the specified output file all footpaths with points inside the specified region.

Both modes output to stdout the directions taken(e.g. NW SW). And both need you to define starting longitude and latitude, as well as ending longitude and latitude to define the range of the PR Quadtree

How to use the program:
Point Search example:
./pointSearcher 3 example/dataset_20.csv 144.9375 -37.8750 145.0000 -37.6875 <example/example_point_input.in

Region Search example:
./pointSearcher 3 example/dataset_20.csv 144.9375 -37.8750 145.0000 -37.6875 <example/example_region_input.in


144.9375 -37.8750 145.0000 -37.6875 defines the starting longitude, starting latitude, ending longitude and latitude respectively for the PQ quad tree.
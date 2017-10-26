#pragma once

#include <fstream>
#include <type_traits>
#include "GraphRepresentation.h"


namespace TSP
{
	template<typename R>
	R ParseGraphFile(std::string filepath);	

	inline std::vector<Vertex> ReadCoordinates(std::fstream & stream)
	{
		uint id, x, y;
		auto coordinates = std::vector<Vertex>();
		while (stream >> id >> x >> y)
			coordinates.push_back(Vertex{ id, x, y });
		return coordinates;
	}

	template<typename R>
	inline R  ParseGraphFile(std::string filepath)
	{

		std::fstream file;
		file.open(filepath, std::ios_base::in);

		if (!file)
			throw std::runtime_error("Could not open the file");

		std::string file_type;
		std::getline(file, file_type);
		R rep;


		if (file_type == "NODE_COORD")
		{
			std::vector<Vertex> coordinates = ReadCoordinates(file);
			rep = R(coordinates);
		}

		/*else if (file_type == "EDGE_WEIGHT")	
			int y = 7'*/
		else
			throw std::runtime_error("Invlid file type identifier");

		file.close();
		return rep;
	}
}

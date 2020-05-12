#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <map>
#include <set>

struct Entity {
	int id;
	std::string entity_nm;
	std::string card_id;
	int player;
	std::map<std::string, std::string> tags;
};

std::ostream &operator<<(std::ostream &out, const Entity &entity)
{
	out << "Entity " << entity.id << std::endl;
	out << "\t name = " << entity.entity_nm << std::endl;
	out << "\t CardID = " << entity.card_id << std::endl;
	out << "\t player = " << entity.player << std::endl;
	for (auto &[tag, value] : entity.tags) {
		out << "\t\t " << tag << " = " << value << std::endl;
	}
	// out << "\t -----------" << std::endl;
	// out << "\t times = " << entity.times << std::endl;
	// out << "\t line_id = " << entity.line_id << std::endl;
	return out;
}

int main()
{
	std::ifstream ifs("hs_logs/Power.log");

	if (ifs.is_open()) {
		int entity_id = 0;
		int line_id = 0;
		bool in_full_entity_flg = false;
		std::string line;

		std::map<int, Entity> card_entities;
		std::map<std::string, Entity> global_entities;

		// regexp
		std::regex re_full_entity_create("FULL_ENTITY - Creating ID=(\\d+) "
		                                 "CardID=(\\w+)");
		std::regex re_full_entity_update("FULL_ENTITY - Updating "
		                                 "\\[entityName=(.*?) "
		                                 "id=(\\d+).*?cardId=(\\w+) player=(\\d+)");
		std::regex re_tag_change_global_entity("TAG_CHANGE Entity=(.*?) tag=(\\w+) "
		                                       "value=(\\w+)");
		std::regex re_tag_change_card_entity("TAG_CHANGE "
		                                     "Entity=\\[entityName=(.*?) "
		                                     "id=(\\d+).*?cardId=(\\w*) "
		                                     "player=(\\d+)\\] tag=(\\w+) "
		                                     "value=(\\w+)");
		std::regex re_tag("tag=(\\w+) value=(\\w+)");
		// regexp match
		std::smatch match;

		while (line_id < 90000) {
			while (std::getline(ifs, line)) {
				line_id++;
				if (line.find("FULL_ENTITY") != std::string::npos) {
					// creating entity
					if (std::regex_search(line, match, re_full_entity_create)) {
						entity_id = std::stoi(match.str(1));
						card_entities[entity_id].id = entity_id;
						card_entities[entity_id].card_id = match.str(2);
						in_full_entity_flg = true;
					}
					// updating entity
					else if (std::regex_search(line, match, re_full_entity_update)) {
						entity_id = std::stoi(match.str(2));
						card_entities[entity_id].id = entity_id;
						card_entities[entity_id].card_id = match.str(3);
						card_entities[entity_id].entity_nm = match.str(1);
						card_entities[entity_id].player = std::stoi(match.str(4));
						in_full_entity_flg = true;
					}
				}
				else if (line.find("TAG_CHANGE") != std::string::npos) {
					// changing tag card entity
					if (std::regex_search(line, match, re_tag_change_card_entity)) {
						entity_id = std::stoi(match.str(2));
						card_entities[entity_id].id = entity_id;
						card_entities[entity_id].card_id = match.str(3);
						card_entities[entity_id].entity_nm = match.str(1);
						card_entities[entity_id].player = std::stoi(match.str(4));
						card_entities[entity_id].tags[match.str(5)] = match.str(6);
						in_full_entity_flg = false;
					}
					// changing tag global entity
					else if (std::regex_search(line, match,
					                           re_tag_change_global_entity)) {
						global_entities[match.str(1)].entity_nm = match.str(1);
						global_entities[match.str(1)].tags[match.str(2)] = match.str(3);
						in_full_entity_flg = false;
					}
				}
				// in full entity tags value
				else if (in_full_entity_flg) {
					if (std::regex_search(line, match, re_tag)) {
						card_entities[entity_id].tags[match.str(1)] = match.str(2);
					}
					// full_entity end
					else {
						in_full_entity_flg = false;
					}
				}
			}
			if (!ifs.eof())
				break; // Ensure end of read was EOF.
			ifs.clear();
			// You may want a sleep in here to avoid
			// being a CPU hog.
		}
		for (auto [id, global_entity] : global_entities) {
			std::cout << global_entity << std::endl;
		}
	}
	else {
		std::cerr << "hs_logs/Power.log"
		          << "\n";
	}
	return 0;
}
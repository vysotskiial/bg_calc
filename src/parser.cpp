#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <map>
#include <set>

struct Entity
{
    int id;
    std::string card_nm;
    std::string card_id;
    int player;
    int attack;
    int health;

    int times;
    int line_id;

    // friend std::ostream& operator<< (std::ostream &out, const Entity &Entity);
};

std::ostream& operator<< (std::ostream &out, const Entity &entity)
{
    out << "Entity " << entity.id << std::endl;
    out << "\t CardID = " << entity.card_id << std::endl;
    out << "\t player = " << entity.player << std::endl;
    out << "\t name = " << entity.card_nm << std::endl;
    out << "\t attack = " << entity.attack << std::endl;
    out << "\t health = " << entity.health << std::endl;
    out << "\t -----------" << std::endl;
    out << "\t times = " << entity.times << std::endl;
    out << "\t line_id = " << entity.line_id << std::endl;
    return out;
}

struct Block
{
    int id;
    std::string block_tp;
    std::string entity_nm;
    std::set<int> entities;

    int start_line_id;
    int end_line_id;
    int next_start_line_id;
    int counted_lines;

    // friend std::ostream& operator<< (std::ostream &out, const Block &blocks);
};

std::ostream& operator<< (std::ostream &out, const Block &blocks)
{
    out << "##################################" << std::endl;
    out << "blocks " << blocks.id << std::endl;
    out << "\t blocks type = " << blocks.block_tp << std::endl;
    out << "\t entity name = " << blocks.entity_nm << std::endl;
    out << "\tentity:" << std::endl;
    for (auto& entity : blocks.entities)
    {
        out << "\t\t" << entity << std::endl;
    }
    
    out << "\t -----------" << std::endl;
    out << "\t start_line_id = " << blocks.start_line_id << std::endl;
    out << "\t end_line_id = " << blocks.end_line_id << std::endl;
    out << "\t next_start_line_id = " << blocks.next_start_line_id << std::endl;
    out << "\t counted_lines = " << blocks.counted_lines << std::endl;
    
    return out;
}

int main()
{
    std::ifstream ifs("hs_logs/Power.log");

    if (ifs.is_open())
    {
        int entity_id = 0;
        int block_id = 0;
        int line_id = 0;
        bool in_full_entity_flg = false;

        std::map<int, Entity> entities;
        std::map<int, Block> blocks;

        std::string line;

        // control regexp
        std::regex re_full_entity("FULL_ENTITY");
        std::regex re_tag("\\stag=");
        std::regex re_entity("entity.*\\sid=(\\d+) CardId=\\w+", std::regex::icase);

        // header entity regexp
        std::regex re_card_id("CardID=(\\w+)", std::regex::icase);
        std::regex re_entity_id("\\sid=(\\d+)");
        std::regex re_entity_name("entityName=(.*)\\sid=(\\d+)", std::regex::icase);
        std::regex re_player("\\splayer=(\\d+)");

        // body entity regexp
        std::regex re_health("tag=HEALTH value=(\\d+)");
        std::regex re_attack("tag=ATK value=(\\d+)");

        // blocks regexp
        std::regex re_block_start("Block.{1}START", std::regex::icase);
        std::regex re_block_end("Block.{1}END", std::regex::icase);
        std::regex re_block_type("BlockType=(\\w+)", std::regex::icase);
        std::regex re_block_entity("Entity=(\\w+)", std::regex::icase);

        std::smatch match;

        while (line_id < 90000)
        {
            while (std::getline(ifs, line)) 
            {
                line_id++;
                // new (update) entity description
                if (std::regex_search(line, match, re_entity))
                {
                    entity_id = std::stoi(match.str(1));
                    entities[entity_id].id = entity_id;
                    blocks[block_id].entities.insert(entity_id);
                    // header
                    if (std::regex_search(line, match, re_card_id))
                    {
                        entities[entity_id].card_id = match.str(1);
                    }
                    if (std::regex_search(line, match, re_entity_name))
                    {
                        entities[entity_id].card_nm = match.str(1);
                    }
                    if (std::regex_search(line, match, re_player))
                    {
                        entities[entity_id].player = std::stoi(match.str(1));
                    }
                    // debugging
                    if (std::regex_search(line, re_full_entity))
                    {
                        entities[entity_id].times++;
                        entities[entity_id].line_id = line_id;
                    }
                }
                // entity body
                if (std::regex_search(line, match, re_health))
                {
                    entities[entity_id].health = std::stoi(match.str(1));
                    blocks[block_id].counted_lines++;
                }
                else if (std::regex_search(line, match, re_attack))
                {   
                    entities[entity_id].attack = std::stoi(match.str(1));
                    blocks[block_id].counted_lines++;
                }
                // block
                if (std::regex_search(line, re_block_start))
                {
                    block_id++;
                    blocks[block_id].id = block_id;
                    blocks[block_id].start_line_id = line_id;
                    if (block_id > 1)
                        blocks[block_id - 1].next_start_line_id = line_id;
                    if (std::regex_search(line, match, re_block_type))
                        blocks[block_id].block_tp = match.str(1);
                    if (std::regex_search(line, match, re_entity_name))
                        blocks[block_id].entity_nm = match.str(1);
                }
                else if (std::regex_search(line, re_block_end))
                {
                    blocks[block_id].end_line_id = line_id;
                }
            }
            if (!ifs.eof()) break; // Ensure end of read was EOF.
            ifs.clear();

            // You may want a sleep in here to avoid
            // being a CPU hog.
        }
        // int key;
        // for (auto& [key, block] : blocks)
        // {
        //     // Block &block = k_block.second;
        //     std::cout << block << std::endl;
        //     for (auto& entity_id : block.entities)
        //     {
        //         std::cout << "*********************" << std::endl;
        //         std::cout << entities[entity_id] << std::endl;
        //     }
        // }
    }
    else
    {
        std::cerr << "hs_logs/Power.log" << "\n";
    }
    return 0;
}
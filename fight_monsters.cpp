#include <iostream>
#include <limits>
#include <string>
#include "random.hpp"

constexpr int g_inc_damage_cost = 100;

class Potion {
	public:
	enum Type {
		health,
		strength,
		poison,
		max_types,
	};

	enum Effect {
		small,
		medium,
		large,
		max_effects,
	};

	Potion(Type type, Effect effect) : m_type(type), m_effect(effect) {}

	const Type get_type() const {
		return m_type;
	}

	const int get_effect_size() const {
		std::size_t type_index = static_cast<std::size_t>(m_type);
		std::size_t effect_index = static_cast<std::size_t>(m_effect);
		return effect_sizes[type_index][effect_index];
	}

	const std::string get_name() const {
		std::string name = "";
		switch (m_effect) {
		case small:
			name += "Small";
			break;
		case medium:
			name += "Medium";
			break;
		case large:
			name += "Large";
			break;
		}
		name += " potion of ";
		switch (m_type) {
		case health:
			name += "Health";
			break;
		case strength:
			name += "Strength";
			break;
		case poison:
			name += "Poison";
			break;
		}
		return name;
	}

	static Potion get_random_potion() {
		std::size_t type_index = static_cast<std::size_t>(
			Random::get(0, static_cast<int>(max_types) - 1)
		);
		std::size_t effect_index = static_cast<std::size_t>(
			Random::get(0, static_cast<int>(max_effects) - 1)
		);
		return Potion(all_types[type_index], all_effects[effect_index]);
	}

	private:
	Type m_type;
	Effect m_effect;

	const static Type all_types[];
	const static Effect all_effects[];
	const static int effect_sizes[3][3];
};

const Potion::Type Potion::all_types[] = {Potion::health, Potion::strength,
	Potion::poison};
const Potion::Effect Potion::all_effects[] = {Potion::small, Potion::medium,
	Potion::large};
const int Potion::effect_sizes[3][3] = {{2, 2, 5}, {1, 1, 1}, {-1, -1, -1}};

class Creature {
	public:
	Creature(const std::string &name, char symbol, int health, int damage,
	int gold) : m_name(name), m_health(health), m_damage(damage), m_gold(gold),
	m_symbol(symbol) {}

	Creature(const Creature &creature) = default;

	const std::string &get_name() const {
		return m_name;
	}

	const int get_health() const {
		return m_health >= 0 ? m_health : 0;
	}

	const int get_damage() const {
		return m_damage;
	}

	const int get_gold() const {
		return m_gold;
	}

	const char get_symbol() const {
		return m_symbol;
	}

	void reduce_health(int damage) {
		m_health -= damage;
	}

	bool is_dead() const {
		return m_health <= 0;
	}

	void add_gold(int amount) {
		m_gold += amount;
	}

	protected:
	std::string m_name;
	int m_health = 0;
	int m_damage = 0;
	int m_gold = 0;
	char m_symbol = '\0';
};

class Player : public Creature {
	public:
	Player(const std::string &name)
	: Creature(name, '@', 10, 1, 0), m_level(1) {}

	const int get_level() const {
		return m_level;
	}

	void level_up() {
		++m_level;
		++m_damage;
	}

	bool has_won() const {
		return m_level >= 20;
	}

	void increase_damage(int n_points) {
		if (m_gold >= n_points * g_inc_damage_cost) {
			m_damage += n_points;
			m_gold -= n_points * g_inc_damage_cost;
		}
	}

	void drink_potion(const Potion &potion) {
		int effect = potion.get_effect_size();
		switch (potion.get_type()) {
		case Potion::health:
		case Potion::poison:
			m_health += effect;
			break;
		case Potion::strength:
			m_damage += effect;
			break;
		}
	}

	private:
	int m_level = 1;
};

class Monster : public Creature {
	public:
	enum Type {
		dragon,
		orc,
		slime,
		max_types,
	};

	Monster(const Type type)
	: Creature(monster_data[static_cast<std::size_t>(type)]) {}

	static Monster get_random_monster() {
		return Monster(static_cast<Type>(Random::get(0, Type::max_types - 1)));
	}

	private:
	const static Creature monster_data[];
	// C++17
	//const static inline Creature monster_data[] = {
	//	Creature("dragon", 'D', 20, 4, 100), Creature("orc", 'o', 4, 2, 25),
	//	Creature("slime", 's', 1, 1, 10)
	//};
};

const Creature Monster::monster_data[] = {
	Creature("dragon", 'D', 20, 4, 100), Creature("orc", 'o', 4, 2, 25),
	Creature("slime", 's', 1, 1, 10)
};


std::string get_name_input() {
	std::string name;
	while (true) {
		std::cout << "Enter your name: ";
		std::getline(std::cin >> std::ws, name);
		if (!std::cin) {
			if (std::cin.eof()) {
				std::exit(0);
			}
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		break;
	}
	return name;
}

char get_character_input(const std::string &message) {
	char input;
	while (true) {
		std::cout << message;
		std::cin >> input;
		if (!std::cin || std::cin.peek() != '\n') {
			if (std::cin.eof()) {
				std::exit(0);
			}
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		break;
	}
	return input;
}

void buy_more_damage(Player &player) {
	if (player.get_gold() < g_inc_damage_cost) {
		return;
	}
	int n_points = player.get_gold() / g_inc_damage_cost;
	std::cout << "You have " << player.get_gold()
		<< " gold that you could use to increase your damage by " << n_points
		<< " points. Would you like to spend " << n_points * g_inc_damage_cost
		<< " gold? ";
	char choice;
	do {
		choice = get_character_input("[y/n]: ");
	} while (choice != 'y' && choice != 'n');
	if (choice == 'y') {
		player.increase_damage(n_points);
		std::cout << "You now have " << player.get_gold() << " gold.\n";
	}
}

void attack_player(Player &player, const Monster &monster) {
	player.reduce_health(monster.get_damage());
	std::cout << "The " << monster.get_name() << " hit you for "
		<< monster.get_damage() << " damage. You have " << player.get_health()
		<< " health left.\n";
}

void attack_monster(const Player &player, Monster &monster) {
	monster.reduce_health(player.get_damage());
	std::cout << "You hit the " << monster.get_name() << " for "
		<< player.get_damage() << " damage. It has " << monster.get_health()
		<< " health left.\n";
}

void discover_potion(Player &player) {
	if (Random::get(0, 9) >= 3) {
		return;
	}
	std::cout << "You found a mythical potion! Do you want to drink it? ";
	char choice;
	do {
		choice = get_character_input("[y/n]: ");
	} while (choice != 'y' && choice != 'n');
	if (choice == 'y') {
		Potion potion = Potion::get_random_potion();
		player.drink_potion(potion);
		std::cout << "You drank a " << potion.get_name()
			<< ". Your health is now " << player.get_health()
			<< " and you can deal " << player.get_damage() << " damage.\n";
	}
}

void fight_monster(Player &player, Monster &monster) {
	while (!monster.is_dead() && !player.is_dead()) {
		char choice;
		do {
			choice = get_character_input("(R)un or (F)ight: ");
		} while (choice != 'r' && choice != 'f');
		int escape;
		switch (choice) {
		case 'r':
			escape = Random::get(0, 1);
			if (!escape) {
				std::cout << "You failed to flee.\n";
				attack_player(player, monster);
			}
			else {
				std::cout << "You successfully fled.\n";
			}
			return;
		case 'f':
			attack_monster(player, monster);
			if (monster.is_dead()) {
				std::cout << "You killed the " << monster.get_name() << ".\n";
				player.level_up();
				std::cout << "You are now level " << player.get_level()
					<< ".\n";
				std::cout << "You found " << monster.get_gold() << " gold.\n";
				player.add_gold(monster.get_gold());
				discover_potion(player);
				return;
			}
			else {
				attack_player(player, monster);
			}
			break;
		}
	}
}

int main() {
	Player player(get_name_input());
	std::cout << "Welcome, " << player.get_name() << '\n';
	while (!player.is_dead() && !player.has_won()) {
		buy_more_damage(player);
		std::cout << "You are level " << player.get_level()
			<< " and you can deal " << player.get_damage()
			<< " damage. You currently have " << player.get_health()
			<< " health.\n";
		Monster monster = Monster::get_random_monster();
		std::cout << "You have encountered a " << monster.get_name() << " ("
			<< monster.get_symbol() << "). It has " << monster.get_health()
			<< " health.\n";
		fight_monster(player, monster);
	}
	if (player.is_dead()) {
		std::cout << "You died at level " << player.get_level() << " and with "
			<< player.get_gold() << " gold.\n";
		std::cout << "Too bad you can't take it with you!\n";
	}
	else {
		std::cout << "You escaped with " << player.get_gold() << " gold!\n";
	}
	return 0;
}

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <limits>
#include "random.hpp"

struct Card {
	enum Rank {
		rank_ace,
		rank_2,
		rank_3,
		rank_4,
		rank_5,
		rank_6,
		rank_7,
		rank_8,
		rank_9,
		rank_10,
		rank_jack,
		rank_queen,
		rank_king,
		max_ranks,
	};

	enum Suit {
		suit_club,
		suit_diamond,
		suit_heart,
		suit_spade,
		max_suits,
	};

	Rank rank;
	Suit suit;

	static const std::array<Rank,max_ranks> all_ranks;
	static const std::array<Suit,max_suits> all_suits;
	static const std::array<char,max_ranks> rank_symbols;
	static const std::array<char,max_suits> suit_symbols;

	friend std::ostream &operator<<(std::ostream &out, const Card &card);

	const int get_value() const;
};

const std::array<Card::Rank,Card::max_ranks> Card::all_ranks = {rank_ace,
	rank_2, rank_3, rank_4, rank_5, rank_6, rank_7, rank_8, rank_9, rank_10,
	rank_jack, rank_queen, rank_king};
const std::array<Card::Suit,Card::max_suits> Card::all_suits = {suit_club,
	suit_diamond, suit_heart, suit_spade};
const std::array<char,Card::max_ranks> Card::rank_symbols = {'A', '2', '3', '4',
	'5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K'};
const std::array<char,Card::max_suits> Card::suit_symbols = {'C', 'D', 'H',
	'S'};

const int Card::get_value() const {
	switch (rank) {
	case rank_ace:
		return 11;
	case rank_jack:
	case rank_queen:
	case rank_king:
		return 10;
	default:
		return static_cast<int>(rank) + 1;
	}
}

std::ostream &operator<<(std::ostream &out, const Card &card) {
	out << Card::rank_symbols[card.rank] << Card::suit_symbols[card.suit];
	return out;
}

class Deck {
	public:
	Deck() : m_index(0) {
		int i = 0;
		for (const Card::Suit suit : Card::all_suits) {
			for (const Card::Rank rank : Card::all_ranks) {
				m_cards[i++] = Card {rank, suit};
			}
		}
	}

	const Card &deal_card() {
		assert(m_index < 52);
		return m_cards[m_index++];
	}

	void shuffle() {
		m_index = 0;
		std::shuffle(m_cards.begin(), m_cards.end(), Random::rng);
	}

	private:
	std::array<Card,52> m_cards;
	int m_index;
};

namespace Settings {
	const int bust_score = 21;
	const int dealer_stop = 17;
};

class Player {
	public:
	Player() : m_score(0), m_aces(0) {}

	const int get_score() const {
		return m_score;
	}

	void add_card(const Card &card) {
		if (card.rank == Card::rank_ace) {
			++m_aces;
		}
		m_score += card.get_value();
		while (m_aces > 0 && m_score > Settings::bust_score) {
			--m_aces;
			m_score -= 10;
		}
	}

	private:
	int m_score = 0;
	int m_aces = 0;
};

char hit_or_stand() {
	char input;
	while (true) {
		std::cout << "(h) to hit, or (s) to stand: ";
		std::cin >> input;
		if (!std::cin || std::cin.peek() != '\n') {
			if (std::cin.eof()) {
				std::exit(0);
			}
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input.\n";
			continue;
		}
		if (input != 'h' && input != 's') {
			std::cout << "Invalid input.\n";
			continue;
		}
		break;
	}
	return input;
}

bool player_turn(Player &player, Deck &deck) {
	while (player.get_score() <= Settings::bust_score) {
		char input = hit_or_stand();
		if (input == 'h') {
			Card card = deck.deal_card();
			player.add_card(card);
			std::cout << "You were dealt " << card << ". You now have: "
				<< player.get_score() << '\n';
		}
		else {
			return true;
		}
	}
	return false;
}

bool dealer_turn(Player &dealer, Deck &deck) {
	while (dealer.get_score() < Settings::dealer_stop) {
		Card card = deck.deal_card();
		dealer.add_card(card);
		std::cout << "The dealer flips a " << card << ". They now have: "
			<< dealer.get_score() << '\n';
	}
	if (dealer.get_score() > Settings::bust_score) {
		return true;
	}
	return false;
}

int play_blackjack(Player &player, Player &dealer, Deck &deck) {
	Card card = deck.deal_card();
	dealer.add_card(card);
	std::cout << "The dealer is showing " << card << " (" << dealer.get_score()
		<< ")\n";
	card = deck.deal_card();
	std::cout << "You are showing " << card << ' ';
	player.add_card(card);
	card = deck.deal_card();
	player.add_card(card);
	std::cout << card << " (" << player.get_score() << ")\n";
	if (!player_turn(player, deck)) {
		std::cout << "You went bust!\n";
		return -1;
	}
	if (dealer_turn(dealer, deck)) {
		std::cout << "The dealer went bust!\n";
		return 1;
	}
	return player.get_score() - dealer.get_score();
}

int main() {
	Player player;
	Player dealer;
	Deck deck;
	deck.shuffle();
	int win = play_blackjack(player, dealer, deck);
	if (win > 0) {
		std::cout << "You win!\n";
	}
	else if (win < 0) {
		std::cout << "You lose!\n";
	}
	else {
		std::cout << "Tie!\n";
	}
	return 0;
}

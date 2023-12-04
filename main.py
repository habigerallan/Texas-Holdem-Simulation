import random
import itertools
from collections import Counter
import multiprocessing
from tqdm import tqdm

# Constants for suits and ranks
HEARTS, DIAMONDS, CLUBS, SPADES = 1, 2, 3, 4
TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE = range(2, 15)

class Card:
    def __init__(self, suit, rank):
        self.suit = suit
        self.rank = rank
        self.value = rank

    def __repr__(self):
        suit_names = {1: "Hearts", 2: "Diamonds", 3: "Clubs", 4: "Spades"}
        rank_names = {11: "Jack", 12: "Queen", 13: "King", 14: "Ace"}
        rank_name = rank_names.get(self.rank, str(self.rank))
        return f"{rank_name} of {suit_names[self.suit]}"

class Deck:
    def __init__(self):
        self.cards = [Card(suit, rank) for suit in [HEARTS, DIAMONDS, CLUBS, SPADES] for rank in range(2, 15)]
        self.index = 0

    def shuffle(self):
        self.index = 0
        random.shuffle(self.cards)

    def deal(self):
        if self.index >= 51:
            self.shuffle()

        card_to_deal = self.cards[self.index]
        self.index += 1
        return card_to_deal
    
class Player:
    def __init__(self, name):
        self.name = name
        self.hand = []
        
    def reset(self):
        self.hand = []

    def __repr__(self):
        return f"{self.name} with hand {self.hand}"

class PokerHand:
    def __init__(self, cards):
        self.cards = sorted(cards, key=lambda card: card.value, reverse=True)
        self.rank_counts = Counter(card.rank for card in cards)
        self.suit_counts = Counter(card.suit for card in cards)
        self.is_flush_flag = max(self.suit_counts.values()) >= 5
        self.straight_high_card = self.find_straight_high_card()  # Stores the high card of the straight if it exists

    def find_straight_high_card(self):
        values = [card.value for card in self.cards]
        value_set = set(values)
        for i in range(14, 4, -1):  # Check from Ace down to 5
            if all(val in value_set for val in range(i, i - 5, -1)):
                return i
        return None

    def is_flush(self):
        return self.is_flush_flag

    def is_straight(self):
        return self.straight_high_card is not None

    def is_royal_flush(self):
        return self.is_flush() and self.straight_high_card == ACE

    def is_straight_flush(self):
        return self.is_flush() and self.is_straight()
    
    def is_four_of_a_kind(self):
        return 4 in self.rank_counts.values()

    def is_full_house(self):
        return 3 in self.rank_counts.values() and 2 in self.rank_counts.values()

    def is_three_of_a_kind(self):
        return 3 in self.rank_counts.values()

    def is_two_pair(self):
        return len([rank for rank, count in self.rank_counts.items() if count == 2]) == 2

    def is_one_pair(self):
        return 2 in self.rank_counts.values()

    def evaluate_hand(self):
        if self.is_royal_flush():
            return 10, self.get_high_card_values()
        elif self.is_straight_flush():
            return 9, self.get_high_card_values()
        elif self.is_four_of_a_kind():
            return 8, self.get_rank_values(4)
        elif self.is_full_house():
            return 7, self.get_full_house_values()
        elif self.is_flush():
            return 6, self.get_high_card_values()
        elif self.is_straight():
            return 5, self.get_high_card_values()
        elif self.is_three_of_a_kind():
            return 4, self.get_rank_values(3)
        elif self.is_two_pair():
            return 3, self.get_two_pair_values()
        elif self.is_one_pair():
            return 2, self.get_rank_values(2)
        else:
            return 1, self.get_high_card_values()

    def get_high_card_values(self):
        return [card.value for card in self.cards[:5]]

    def get_rank_values(self, count):
        rank_values = [rank for rank, cnt in self.rank_counts.items() if cnt == count]
        kickers = [card.value for card in self.cards if card.rank not in rank_values][:5 - len(rank_values)]
        return rank_values + kickers

    def get_full_house_values(self):
        three_of_a_kind = [rank for rank, cnt in self.rank_counts.items() if cnt == 3]
        pair = [rank for rank, cnt in self.rank_counts.items() if cnt == 2]
        return three_of_a_kind + pair

    def get_two_pair_values(self):
        pairs = [rank for rank, cnt in self.rank_counts.items() if cnt == 2]
        kicker = [card.value for card in self.cards if card.rank not in pairs][0]
        return pairs + [kicker]
    
    def __repr__(self):
        return ', '.join(str(card) for card in self.cards)
    
def play_chunk(num_players, chunk_size):
    local_hand_type_count = Counter()
    
    game = TexasHoldem(num_players)
    for _ in range(chunk_size):
        winning_hand_type = game.play()
        if winning_hand_type is not None:
            winning_hole_cards_str = ', '.join(str(card) for card in winning_hand_type)
            local_hand_type_count[winning_hole_cards_str] += 1
            
        game.players[0].reset()
        game.community_cards = []

    return local_hand_type_count

class TexasHoldem:
    def __init__(self, num_players):
        self.deck = Deck()
        self.players = [Player(f"Player {i+1}") for i in range(num_players)]
        self.community_cards = []

    def deal_hole_cards(self):
        for player in self.players:
            player.hand = [self.deck.deal(), self.deck.deal()]

    def deal_community_cards(self):
        self.community_cards.extend([self.deck.deal() for _ in range(3)])  # Flop
        self.community_cards.append(self.deck.deal())  # Turn
        self.community_cards.append(self.deck.deal())  # River

    def find_winner(self):
        best_hand = None
        winning_player_hole_cards = None
        for player in self.players:
            all_cards = player.hand + self.community_cards
            for combo in itertools.combinations(all_cards, 5):
                poker_hand = PokerHand(list(combo))
                hand_rank, hand_values = poker_hand.evaluate_hand()
                if not best_hand or hand_rank > best_hand[0] or (hand_rank == best_hand[0] and self.compare_hands(hand_values, best_hand[1])):
                    best_hand = (hand_rank, hand_values)
                    winning_player_hole_cards = player.hand
        return winning_player_hole_cards

    def compare_hands(self, hand1_values, hand2_values):
        return hand1_values > hand2_values

    def play(self):
        self.deck.shuffle()
        self.deal_hole_cards()
        self.deal_community_cards()
        winning_hand_type = self.find_winner()  # Update this line
        return winning_hand_type

def main():
    num_players = 1
    total_iterations = 10000000
    process_count = 16
    chunk_size = 100000  # Size of each work chunk

    # Calculate total chunks
    total_chunks = total_iterations // chunk_size
    final_counts = Counter()
    # Setup a progress bar
    with tqdm(total=total_iterations) as pbar:
        with multiprocessing.Pool(process_count) as pool:
            results = [pool.apply_async(play_chunk, args=(num_players, chunk_size)) for _ in range(total_chunks)]

            for result in results:
                local_hand_type_count = result.get()
                final_counts.update(local_hand_type_count)
                pbar.update(chunk_size)

            pool.close()
            pool.join()

    final_counts = Counter()
    for result in results:
        final_counts.update(result.get())

    # Calculate and display win percentages for each hole card combination
    total_games = sum(final_counts.values())
    win_percentages = {hole_cards: (count / total_games * 100) for hole_cards, count in final_counts.items()}

    # Sort the win percentages in descending order
    sorted_win_percentages = sorted(win_percentages.items(), key=lambda x: x[1], reverse=True)[:10]

    for hole_cards, win_pct in sorted_win_percentages:
        print(f"Hole cards {hole_cards}: {win_pct:.2f}% win rate")

if __name__ == "__main__":
    main()

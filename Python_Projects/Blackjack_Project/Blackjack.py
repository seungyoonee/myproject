import random
suits = ['♠︎','♣︎','♥︎','♦︎']

class Card(object):  
    def __init__(self, suit, rank):
        self.suit = suit
        self.rank = rank
   
    def __str__(self):
        return self.suit + str(self.rank)

class CardCollection(object):
    # Creates an empty deck.
    def __init__(self):
        self.cards = []
        
    # Adds a new card to the deck.
    def add_card(self, card): 
        self.cards.append(card)
        
    # Remove a card from the collection and return it.
    def draw_card(self):
        draw = self.cards[len(self.cards) - 1]
        self.cards.remove(self.cards[len(self.cards) - 1])
        
        return draw
   
    # Populate the collection with a fresh deck of 52 cards then shuffle the deck.
    def make_deck(self):
        for suit in range(4):
            self.cards.append(Card(suits[suit], "A"))
            for rank in range(2,11):
                new_card = Card(suits[suit], rank)
                self.cards.append(new_card)
            self.cards.append(Card(suits[suit], "J"))
            self.cards.append(Card(suits[suit], "Q"))
            self.cards.append(Card(suits[suit], "K"))

        random.shuffle(self.cards)

    # Returns the total value of the cards.
    def value(self):
        total_value = 0
        contains_ace = False
        for i in range(len(self.cards)):
            if self.cards[i].rank not in ["A","J","Q","K"]:
                total_value += int(self.cards[i].rank)
            elif self.cards[i].rank in ["J","Q","K"]:
                total_value += 10
            elif self.cards[i].rank == "A":
                contains_ace = True
                total_value += 1
                
        if contains_ace:
            if total_value < 11:
                total_value -= 1
                total_value += 11
                
        return total_value
    
def main():
    # initialize a deck
    deck = CardCollection()
    deck.make_deck()
    
    # initialize the player hand
    player_hand = CardCollection()
    
    # draw a card from the deck (player)
    deal = deck.draw_card()
    player_hand.add_card(deal)
    print("You drew", deal)
    print("sum:", player_hand.value())
    
    game_over = False
    # keep asking player to deal or stay
    another_card = input("Do you want another card (y/n)? ")
    while another_card == "y":
        deal = deck.draw_card()
        player_hand.add_card(deal)
        # dealer wins if player value exceeds 21
        print("You drew", deal)
        print("Your sum:", player_hand.value())
        # dealer wins if the player value exceeds 21
        if player_hand.value() > 21:
            game_over = True
            print("\nYour value exceeded 21.\nI win.")
            break
        # player wins if the player value is 21
        elif player_hand.value() == 21:
            game_over = True
            print("\nYou win.")
            break
        another_card = input("Do you want another card (y/n)? ")

    # initialize the dealer hand
    dealer_hand = CardCollection()
    
    if not game_over:
        print("\nMy turn")
        # draw a card from the deck (dealer)
        while dealer_hand.value() < 18:
            deal = deck.draw_card()
            dealer_hand.add_card(deal)
            print("\nI drew", deal)
            print("My sum:", dealer_hand.value())
        
        # decide who has won
        if dealer_hand.value() < player_hand.value() or dealer_hand.value() > 21:
            print("\nYou win.")
        elif dealer_hand.value() > player_hand.value():
            print("\nI win.")
        elif dealer_hand.value() == player_hand.value():
            print("\nThe game is a push")

if __name__ == "__main__":
    main()


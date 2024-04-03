#pragma once

#include <vector>
#include <iostream>
using namespace std;

//class card
class Card{
public:
    enum suit{hearts, diamonds, clubs, spades};
    enum rank{ace=1, two=2, three=3, four=4, five=5, six=6, seven=7, eight=8, nine=9, ten=10, jack=10, queen=10, king=10};
    bool face;
    rank m_rank;
    suit m_suit;
    bool m_face;
    Card(rank r =ace, suit s = spades, bool faceup = true);
    int GetValue() const;
    void flip();
    friend ostream& operator<<(ostream& o, const Card& acard);
};
//class hand
class hand{

protected:
    vector <Card*> m_cards;

public:
    hand();
    virtual ~hand();
    void clear();
    int getvalue() const;
    void Add(class Card *c);


};
//class  genPlayer
class GenPlayer : public hand{
public:
    friend ostream& operator<<(ostream& o, const GenPlayer& aGenP);
    GenPlayer (const string& name="");
    virtual ~GenPlayer();
    virtual bool IsHitting() const = 0;
    bool bosted() const;
    void bust() const;
protected:
    string m_name;
};
//class player
class player : public GenPlayer{
public:
    player(const string& name=""){};
    virtual ~player() = default;
    bool IsHitting() const;
    void win() const;
    void lose() const;
    void push() const;
    string getName();
};
//class house
class house : public GenPlayer{
public:
    house(const string& name="house"){};
    virtual ~house() = default;
    virtual bool IsHitting() const;
    void FlipFirstCard();
};
//class deck
class deck : public hand{
public:
    deck();
    virtual ~deck() = default;
    void populate();
    void shuffle();
    void Deal(hand& aHand);
    void AddCard(GenPlayer& aGenP);
};
//class game
class game{
public:
    game(const vector<string>& name);
    virtual ~game() = default;
    void play();
private:
    deck m_deck;
    house m_house;
    vector<player> m_players;
};


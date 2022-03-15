
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


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

Card::Card(rank r , suit s, bool fu) : m_rank(r), m_suit(s), m_face(fu){}
int Card::GetValue()const {
       int value=0;
       if (m_face==1){
           value=m_rank;
       }
       return value;
   }

void Card::flip(){
       if (face==0)
           face=1;
       else
           face=0;
   }

ostream& operator<<(ostream& o, const Card& acard){
    const string RANK[] = {"0","A","2","3","4","5","6","7","8","9","10","J","Q","K"};
    const string SUITS[] = {"c","d","h","s"};
    if (acard.m_face)
        o<<RANK[acard.m_rank]<<SUITS[acard.m_suit];
    else
        o<<"XX";
        return o;
}

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

hand::hand(){
    m_cards.reserve(7);
};
hand::~hand(){
    clear();
}
void hand::Add(class Card *c){
    m_cards.push_back(c);
}
void hand::clear(){
    vector<Card*>::iterator iter = m_cards.begin();
    for(iter=m_cards.begin(); iter!=m_cards.end();++iter){
        delete *iter;
        *iter=0;
    };
    m_cards.clear();
}

int hand::getvalue() const {
    if (m_cards.empty())
        return 0;
    if (m_cards[0]->GetValue() == 0)
        return 0;
    int total=0;
    vector<Card*>::const_iterator iter;
    for (iter=m_cards.begin(); iter!=m_cards.end();++iter){
        total += (*iter)->GetValue();
    }
    bool ace=false;
    for (iter=m_cards.begin(); iter!=m_cards.end();++iter){
        if((*iter)->GetValue() ==Card::ace)
            ace=true;
    }
    if(ace && total <=11)
        total+=11;
    
    return total;
}

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

GenPlayer::GenPlayer (const string& name):m_name(name){}
GenPlayer::~GenPlayer(){}
bool GenPlayer::bosted() const{
    return (getvalue()>21);
}
void GenPlayer::bust() const{
    cout<<m_name<<"busts\n";
}
ostream& operator<<(ostream& o, const GenPlayer& aGenP){
    o<<aGenP.m_name<<":\t";
    vector<Card*>::const_iterator pCard;
    if(!aGenP.m_cards.empty()){
        for(pCard = aGenP.m_cards.begin(); pCard != aGenP.m_cards.end(); ++pCard)
            o<<*(*pCard)<<"\t";
        if (aGenP.getvalue()!=0)
            cout<<"("<<aGenP.getvalue()<<")";
    }
    else
    o<<"empty";
    return o;
};

class player : GenPlayer{
public:
    player(const string& name="");
    virtual ~player();
    virtual bool IsHitting() const=0;
    void win() const;
    void lose() const;
    void push() const;
};

bool player::IsHitting() const {
    cout<<m_name<<"want more?(Y/N)";
    char answ;
    cin>>answ;
    return (answ =='y' || answ=='Y');
}
void player::win() const{
    cout<<m_name<<"win\n";
}
void player::lose() const{
    cout<<m_name<<"lose\n";
}
void player::push() const{
    cout<<m_name<<"push\n";
}

class house : public GenPlayer{
public:
    house(const string& name="house");
    virtual ~house();
    virtual bool IsHitting() const;
    void FlipFirstCard();
};
bool house::IsHitting() const {
    return(getvalue()<=16);
}
void house::FlipFirstCard(){
    if (!(m_cards.empty()))
        m_cards[0]->flip();
    else
        cout<<"no card to flip\n";
}


class deck : public hand{
public:
    deck();
    virtual ~deck();
    void populate();
    void shuffle();
    void Deal(hand& aHand);
    void AddCard(GenPlayer& aGenP);
};
deck::deck(){
    m_cards.reserve(52);
    populate();
}
deck::~deck(){};

void deck::populate(){
    clear();
    for(int s = Card::hearts; s <= Card::spades; ++s){
        for(int r = Card::ace; r<=Card::king; ++r)
            Add(new Card(static_cast<Card::rank>(r),static_cast<Card::suit>(s)));
    }
};
    
void deck::shuffle(){
    random_shuffle()(m_cards.begin(), m_cards.end());
};
void deck::Deal(hand &aHand){
    if(!m_cards.empty()){
        aHand.Add(m_cards.back());
        m_cards.pop_back();
    }
    else{
        cout<<"out of cards";
    }
}
void deck::AddCard(GenPlayer& aGenP){
    cout<<endl;
    while (!(aGenP.bosted()) && aGenP.IsHitting()){
        Deal(aGenP);
        cout<<aGenP<<endl;
        if(aGenP.bosted()){
            aGenP.bust();
        }
    }
};



class game{
public:
    game(const vector<string>& name);
    ~game();
    void play();
private:
    deck m_deck;
    house m_house;
    vector<player> m_players;
};
game::game(const vector<string>& name){
    vector<string>::const_iterator pName;
    for(pName = name.begin(); pName != name.end(); ++pName)
        m_players.push_back(player(*pName));
    srand(static_cast<unsigned int>(time(0)));
    m_deck.populate();
    m_deck.shuffle();
};
game::~game(){};

void game::play(){
    vector<player>::iterator pPlayer;
    for (int i=0;i<2;i++){
        for (pPlayer = m_players.begin(); pPlayer !=m_players.end(); ++pPlayer)
            m_deck.Deal(*pPlayer);
        m_deck.Deal(m_house);
    }
    m_house.FlipFirstCard();
    for (pPlayer = m_players.begin(); pPlayer !=m_players.end(); ++pPlayer){
        cout<<pPlayer<<endl;
    }
    cout<<m_house<<endl;
    for (pPlayer = m_players.begin(); pPlayer !=m_players.end(); ++pPlayer){
        m_deck.AddCard(*pPlayer);
    }
    m_house.FlipFirstCard();
    cout<<endl<<m_house;
    m_deck.AddCard(m_house);
    if(m_house.bosted()){
        for (pPlayer = m_players.begin(); pPlayer !=m_players.end(); ++pPlayer){
            if(!(pPlayer->bosted()))
                pPlayer->win();
        }
    }
    else{
        for (pPlayer = m_players.begin(); pPlayer !=m_players.end(); ++pPlayer){
            if(pPlayer->getvalue() > m_house.getvalue()){
                pPlayer->win();
            }
            if(pPlayer->getvalue() < m_house.getvalue()){
                pPlayer->lose();
            }
            if(pPlayer->getvalue() == m_house.getvalue()){
                pPlayer->push();
            }
        }
    }
    for (pPlayer = m_players.begin(); pPlayer !=m_players.end(); ++pPlayer){
        pPlayer->clear();
    }
    m_house.clear();
};

int main(int argc, const char * argv[]) {
    cout<<"start prog";
    int players=0;
    while (players < 1 || players >7){
        cout<<"enter num of players";
        cin >>players;}
    vector <string> names;
    string name;
    for (int i=0; i<players; i++){
        cout<<"enter name\n";
        cin>>name;
        names.push_back(name);
    }
    
    game aGame(names);
    int again = 0;
    while (again !=1){
        aGame.play();
        cout<<"play again? 1 if yes. 0 if no";
        cin >>again;
    }
    return 0;
}

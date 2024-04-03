#include "classes.h"

////////////////card/////////////
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

////////////////hand
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

/////////////////genPlayer
GenPlayer::GenPlayer (const string& name):m_name(name){}
GenPlayer::~GenPlayer(){}
bool GenPlayer::bosted() const{
    return (getvalue()>21);
}
void GenPlayer::bust() const{
    cout<<m_name<<"busts\n";
}


///////////////////////player
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
string player::getName(){
    return this->m_name;
}

////////////////////////////house
bool house::IsHitting() const {
    return(getvalue()<=16);
}
void house::FlipFirstCard(){
    if (!(m_cards.empty()))
        m_cards[0]->flip();
    else
        cout<<"no card to flip\n";
}

//////////////////////////////////deck
deck::deck(){
    m_cards.reserve(52);
    populate();
}
void deck::populate(){
    clear();
    for(int s = Card::hearts; s <= Card::spades; ++s){
        for(int r = Card::ace; r<=Card::king; ++r)
            Add(new Card(static_cast<Card::rank>(r),static_cast<Card::suit>(s)));
    }
};
void deck::shuffle(){
    //TODO no shaffle!
    //random_shuffle()(m_cards.begin(), m_cards.end());
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

////////////////////////////////game
game::game(const vector<string>& name){
    vector<string>::const_iterator pName;
    for(pName = name.begin(); pName != name.end(); ++pName)
        m_players.push_back(player(*pName));
    srand(static_cast<unsigned int>(1));
    m_deck.populate();
    m_deck.shuffle();
};
void game::play(){
    vector<player>::iterator pPlayer;
    for (int i=0;i<2;i++){
        for (pPlayer = m_players.begin(); pPlayer !=m_players.end(); ++pPlayer)
            m_deck.Deal(*pPlayer);
        m_deck.Deal(m_house);
    }
    m_house.FlipFirstCard();
    for (pPlayer = m_players.begin(); pPlayer !=m_players.end(); ++pPlayer){
        cout<<pPlayer->getName()<<endl;
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



//////////////////opertors


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
/*
ostream& operator<<(ostream& o, const player& aGenP){
    o<<aGenP.m_name <<":\t";
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
*/
ostream& operator<<(ostream& o, const Card& acard){
    const string RANK[] = {"0","A","2","3","4","5","6","7","8","9","10","J","Q","K"};
    const string SUITS[] = {"c","d","h","s"};
    if (acard.m_face)
        o<<RANK[acard.m_rank]<<SUITS[acard.m_suit];
    else
        o<<"XX";
    return o;
}



#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

typedef map<string, int> COSTUME_SHOP_TYPE;
typedef map<string, string> PEOPLE_TYPE;
typedef map<string, vector<string> > RENTAL_TYPE;

// prototypes for the helper functions
void addCostume(COSTUME_SHOP_TYPE &costume_shop);
void rentCostume(COSTUME_SHOP_TYPE& costume_shop, PEOPLE_TYPE& people, RENTAL_TYPE& rentals);
void lookup(COSTUME_SHOP_TYPE& costume_shop, RENTAL_TYPE& rentals);
void printPeople(PEOPLE_TYPE& people);

int main() {
  // two data structures store all of the information for efficiency
  COSTUME_SHOP_TYPE costume_shop;
  PEOPLE_TYPE people;
  RENTAL_TYPE rentals;
  char c;
  while (std::cin >> c) {
    if (c == 'a') {
      addCostume(costume_shop);
    } else if (c == 'r') {
      rentCostume(costume_shop, people, rentals);
    } else if (c == 'l') {
      lookup(costume_shop, rentals);
    } else if (c == 'p') {
      printPeople(people);
    } else {
      std::cerr << "error unknown char " << c << std::endl;
      exit(0);
    }
  }
}


void addCostume(COSTUME_SHOP_TYPE& costume_shop) 
{
  std::string costume_name;
  int num;
  std::cin >> costume_name >> num;

  costume_shop[costume_name] += num;
  if(num == 1)
    cout << "Added " << num << " " << costume_name << " costume." << endl;
  else
    cout << "Added " << num << " " << costume_name << " costumes." << endl;
}


void rentCostume(COSTUME_SHOP_TYPE& costume_shop, PEOPLE_TYPE& people, RENTAL_TYPE& rentals) 
{
  std::string first_name;  
  std::string last_name;
  std::string costume_name;
  std::cin >> first_name >> last_name >> costume_name;

  string name = last_name + " " + first_name;

  PEOPLE_TYPE::const_iterator people_it = people.find(name);
  if(people_it == people.end())
    people[name] = "";

  COSTUME_SHOP_TYPE::const_iterator cost_it = costume_shop.find(costume_name);
  if(cost_it == costume_shop.end())
    cout << "The shop doesn't carry " << costume_name << " costumes." << endl;
  else if(cost_it -> second == 0)
    cout << "No " << costume_name << " costumes available." << endl;
  else if(people[name] == costume_name)
    cout << first_name << " " << last_name << " already has a " << costume_name << " costume." << endl;
  else
  {
    bool first_costume = true;
    string current_costume;
    if(people[name] != "") //they have a costume atm
    {
      first_costume = false;
      current_costume = people[name];
      costume_shop[current_costume]++; //return costume person already has
      RENTAL_TYPE::iterator rental_it = rentals.find(current_costume);
      vector<string>::iterator str_it = find(rental_it->second.begin(), rental_it->second.end(), name);
      rental_it->second.erase(str_it);
    }

    people[name] = costume_name; //give person new costume
    costume_shop[costume_name]--; //decrement inventory
    rentals[costume_name].push_back(name); //add the person to the rentals list -> conveniently in order of who ordered first

    unsigned int space = name.find(" ");
    if(first_costume)
      cout << name.substr(space+1) << " " << name.substr(0,space) << " rents a " << costume_name << " costume." << endl;
    else
      cout <<  name.substr(space+1) << " " << name.substr(0,space) << " returns a " << current_costume << " costume before renting a " << costume_name << " costume." << endl;
  }
}


void lookup(COSTUME_SHOP_TYPE& costume_shop, RENTAL_TYPE& rentals) 
{
  std::string costume_name;
  std::cin >> costume_name;

  COSTUME_SHOP_TYPE::iterator cost_it = costume_shop.find(costume_name);
  if(cost_it == costume_shop.end())
  {
    cout << "The shop doesn't carry " << costume_name << " costumes." << endl;
    return;
  }
    
  cout << "Shop info for " << costume_name << " costumes:" << endl;
  if(cost_it->second != 0)
  {
    if(cost_it->second == 1)
      cout << "  " << cost_it->second << " copy available" << endl;
    else
      cout << "  " << cost_it->second << " copies available" << endl;
  }
  if(rentals[costume_name].size() != 0)
  {
    if(rentals[costume_name].size() == 1)
      cout << "  1 copy rented by:" << endl;
    else
      cout << "  " << rentals[costume_name].size() << " copies rented by:" << endl;

    for(unsigned int i = 0; i < rentals[costume_name].size(); i++)
    {
      unsigned int space = rentals[costume_name][i].find(" ");
      cout << "    " <<  rentals[costume_name][i].substr(space+1) << " " << rentals[costume_name][i].substr(0,space) << endl;
    }
  }

}


void printPeople(PEOPLE_TYPE& people) 
{
  if(people.empty())
  {
    cout << "No customers." << endl;
    return;
  }

  if(people.size() == 1)
    cout << "Costume info for the " << people.size() << " party attendee:" << endl;
  else
    cout << "Costume info for the " << people.size() << " party attendees:" << endl;
  for(PEOPLE_TYPE::const_iterator p_itr = people.begin(); p_itr != people.end(); p_itr++)
  {
    unsigned int space = p_itr->first.find(" ");
    cout << "  " << p_itr->first.substr(space+1) << " " << p_itr->first.substr(0,space);

    if(p_itr -> second == "")
      cout << " does not have a costume." << endl;
    else
      cout << " is wearing a " << p_itr->second << " costume." << endl;
  }
}

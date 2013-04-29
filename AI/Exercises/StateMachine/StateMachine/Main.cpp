#include <iostream>
#include <vector>

enum State
{
	ST_ANY,
	ST_CORRIDOR,
	ST_ROOM
};

enum Event
{
	EV_ANY,
	EV_MOVE_NORTH,
	EV_MOVE_SOUTH,
	EV_UNKNOWN
};

struct Transition
{
	Transition() :
		theState(ST_ANY), theEvent(EV_ANY), theFunction(NULL)
	{
	}

    State theState; 
    Event theEvent; 
    State (* theFunction)();
};

Event getNextEvent()
{
	std::cout << "What would you like to do? > ";

	char move;
	std::cin >> move;

	if (move == 'n')
	{
		return EV_MOVE_NORTH;
	}

	if (move == 's')
	{
		return EV_MOVE_SOUTH;
	}

	return EV_UNKNOWN;
}

State enterRoom()
{
	std::cout << "You have now entered the room." << std::endl;
	return ST_ROOM;
}

State enterCorridor()
{
	std::cout << "You have now entered the corridor." << std::endl;
	return ST_CORRIDOR;
}

int main()
{
	std::vector<Transition> transitions;

	Transition north;
	north.theState = ST_CORRIDOR;
	north.theEvent = EV_MOVE_NORTH;
	north.theFunction = &enterRoom;
	transitions.push_back(north);

	Transition south;
	south.theState = ST_ROOM;
	south.theEvent = EV_MOVE_SOUTH;
	south.theFunction = &enterCorridor;
	transitions.push_back(south);

	State currentState = ST_CORRIDOR; 
	while (currentState != ST_ANY)
	{ 
		Event theEvent = getNextEvent(); 
		for (unsigned int index = 0; index < transitions.size(); index++)
		{ 
			if (currentState == transitions.at(index).theState || ST_ANY == transitions.at(index).theState)
			{ 
				if (theEvent == transitions.at(index).theEvent || EV_ANY == transitions.at(index).theEvent)
				{ 
					currentState = transitions.at(index).theFunction(); 
					break; 
				}
			} 
		} 
	} 
}
